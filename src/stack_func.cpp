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
    
    stk->start_arr    = (size_t*) calloc(1, capacity * sizeof(elem) + 2 * sizeof(ARR_CANARY));     
    stk->start_arr[0] = ARR_CANARY;
    stk->data         = (elem*)((char*)stk->start_arr + sizeof(ARR_CANARY));
    stk->end_arr      = (size_t*) ((char*)stk->start_arr +  capacity * sizeof(elem) + sizeof(ARR_CANARY));
    stk->end_arr[0]   = ARR_CANARY;
    
    stk->capacity      = capacity;
    stk->size          = 0;
    stk->code_of_error = 0;
    
    stk->r_canary = STRUCT_CANARY;
    stk->l_canary = STRUCT_CANARY;
    
    stk->dump_info ={};
    
    stack_rehash(stk);
    
    stack_dump_info_ctor(stk, name_function, name_file, name_variable, num_line) || ASSERTED();

    stack_rehash(stk);

    stack_poison_get(stk, stk->size, stk->capacity) || ASSERTED();
    
    ASSERT_OK(stk);
    return 1;
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
    assert(stk != NULL);

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

int stack_pop(Stack *stk, elem *value)
{
    assert(stk != NULL);
    ASSERT_OK(stk);

    stack_rehash(stk);

    if (stk->size <= 0)
    {
        fprintf(log_file, "You attained the end of stack\n");
        stk->size = -1; 
        ASSERT_OK(stk);
        return -1;
    }

    if (stk->size * 4 < stk->capacity)
    {
        fprintf(log_file,"\n" "size was %zd and capacity was %zd \n", stk->size, stk->capacity); 
            
        stack_resize(stk, stk->capacity / 2) || ASSERTED();
    }

    *value = stk->data[stk->size-1];

    stack_poison_get(stk, stk->size, stk->capacity) || ASSERTED();
    stk->size--;

    stack_rehash(stk);
    ASSERT_OK(stk);
    return 1;
}

void stack_detor(Stack *stk)
{
    free(stk->start_arr);
    stk->capacity = -1;
    stk->size     = -1;
}

int stack_resize(Stack *stk, ssize_t new_capacity)
{   
    ASSERT_OK(stk);

    size_t* tmp_ptr = (size_t*) realloc(stk->start_arr, new_capacity * sizeof(elem) + 2 * sizeof(ARR_CANARY));

    if (tmp_ptr == NULL)
    {
        stk->capacity = -1;                               //todo add error_code in stack struct or return error code or global variable
        return 0;
    }

    stk->start_arr = tmp_ptr;
    stk->end_arr = (size_t*) ((char*)stk->start_arr +  sizeof(elem)*new_capacity + sizeof(ARR_CANARY));
    stk->end_arr[0] = ARR_CANARY;
    stk->data = (elem*)((char*)stk->start_arr + sizeof(ARR_CANARY));
    stk->capacity = new_capacity;
    stack_rehash(stk);

    stack_poison_get(stk, stk->size, stk->capacity) || ASSERTED();
    ASSERT_OK(stk);
    fprintf(log_file,"now size is %zd and capacity is %zd \n\n", stk->size, stk->capacity);

    return 1;
}

int stack_poison_get(Stack *stk, int size, int capacity)
{
    ASSERT_OK(stk);

    for (int i = size; i < capacity; i++)
    {
        stk->data[i] = NAN;
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