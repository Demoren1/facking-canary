#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/debug.h"
#include "../include/stack_objects.h"

extern FILE* log_file;

int stack_ctor(Stack *stk, ssize_t capacity, const char* name_function, const char* name_file, const char* name_variable, int num_line)            
{   
    assert(stk != NULL);
    
    stk->flag++;
    if (stk->flag != 1)
    {
        stk->code_of_error |= STACK_ERROR_DOUBLE_CTOR;
    }

    stk->data   = (elem*) calloc(1, capacity * sizeof(elem) + 2 * sizeof(canary_t)); // todo rejimes
    *((canary_t*)stk->data) = ARR_CANARY; 
    stk->data = (elem*)((canary_t*)stk->data + 1);     
    *((canary_t*)((char*)stk->data +  capacity * sizeof(elem) + sizeof(canary_t))) = ARR_CANARY;

    stk->capacity      = capacity;
    stk->size          = 0;
    stk->code_of_error = 0;
    
    stk->r_canary = STRUCT_CANARY;
    stk->l_canary = STRUCT_CANARY;
    
    stk->dump_info ={};

    stack_rehash(stk);

    ASSERT_OK(stk);
    
    stack_dump_info_ctor(stk, name_function, name_file, name_variable, num_line) || ASSERTED();

    stack_rehash(stk);

    stack_poison_get(stk, stk->size, stk->capacity) || ASSERTED();
    
    ASSERT_OK(stk);
    return 1; //todo return error to user
}   

int stack_dump_info_ctor(Stack *stk, const char* name_function, const char* name_file, const char* name_variable, int num_line)
{   
    ASSERT_OK(stk);

    stk->dump_info.name_of_func     = name_function;
    stk->dump_info.name_of_file     = name_file;   
    stk->dump_info.name_of_variable = name_variable + 1;
    stk->dump_info.num_of_str       = num_line;

    stack_rehash(stk);

    ASSERT_OK(stk);
    return 1;
}

int stack_push(Stack *stk, elem value)
{   
    ASSERT_OK(stk);

    if (stk->size + 1 >= stk->capacity)
    {
       fprintf(log_file,"\n" "size was %zd and capacity was %zd \n", stk->size, stk->capacity);
            
       stack_resize(stk, stk->capacity * 2) || ASSERTED();
    }
    
    stack_rehash(stk);
    ASSERT_OK(stk);

    stk->data[stk->size] = value;
    stk->size++;
    
    stack_rehash(stk);
    ASSERT_OK(stk);
    
    return 1;
}

elem stack_pop(Stack *stk, int *pop_error)
{
    ASSERT_OK(stk);

    elem value = 0;

    if (stk->size <= 0)
    {
        stk->size = -1; 
        ASSERT_OK(stk);
        return -1;
    }

    if (stk->size * 4 < stk->capacity)
    {
        fprintf(log_file,"\n" "size was %zd and capacity was %zd \n", stk->size, stk->capacity); 
            
        stack_resize(stk, stk->capacity / 2) || ASSERTED();
    }

    value = stk->data[stk->size-1];

    stack_poison_get(stk, stk->size, stk->capacity) || ASSERTED();
    stk->size--;

    stack_rehash(stk);
    ASSERT_OK(stk);
    return value;
}

int stack_dtor(Stack *stk)
{
    ASSERT_OK(stk);
    stack_poison_get(stk, 0, stk->capacity);
    stk->flag++;
    stk->capacity = -1;
    stk->size     = -1;
    if (stk->flag != 2)
    {
        stk->code_of_error |= STACK_ERROR_DOUBLE_DTOR;
    }

    free((char*) stk->data - sizeof(canary_t));

    return 0;
}

int stack_resize(Stack *stk, ssize_t new_capacity)
{   
    ASSERT_OK(stk);

    elem* tmp_ptr = (elem*) realloc((char*)stk->data - sizeof(canary_t), new_capacity * sizeof(elem) + 2 * sizeof(canary_t));

    if (tmp_ptr == NULL)
    {
        stk->capacity = -1;                               
        return 0;
    }
    stk->data     = tmp_ptr;
    stk->capacity = new_capacity;

    *((canary_t*)stk->data) = ARR_CANARY;
    stk->data   = (elem*)((canary_t*)stk->data + 1);     
    *((canary_t*)((char*)stk->data +  new_capacity * sizeof(elem) + sizeof(canary_t))) = ARR_CANARY;

    stack_rehash(stk);
    
    stack_poison_get(stk, stk->size, stk->capacity) || ASSERTED();
    
    ASSERT_OK(stk);

    return 1;
}

int stack_poison_get(Stack *stk, size_t size, size_t capacity)
{
    ASSERT_OK(stk);
    size_t i = size;
    for (i; i < capacity; i++)
    {
        *(stk->data + i) = NAN;
    }

    stack_rehash(stk);
    ASSERT_OK(stk);
    return 1;
}

void stack_rehash(Stack *stk)
{   
    stk->hash        = hash(stk->data, stk->capacity * sizeof(elem));
    stk->hash_struct = hash(stk, sizeof(Stack) - sizeof(stk->hash_struct) - 4);
}