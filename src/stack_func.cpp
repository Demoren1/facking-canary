#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/debug.h"
#include "../include/stack_objects.h"

extern FILE* log_file;

int stack_ctor(Stack *stk, ssize_t capacity)
{   
    assert(stk != NULL);
    
    stk->start_arr = (int*) calloc(capacity, sizeof(elem) + 2 * sizeof(ARR_CANARY));
    stk->start_arr[0] = ARR_CANARY;
    stk->data = (elem*)((char*)stk->start_arr + sizeof(ARR_CANARY));
    stk->end_arr = (int*) ((char*)stk->start_arr +  capacity * sizeof(elem) + sizeof(ARR_CANARY));
    stk->end_arr[0] = ARR_CANARY;
    stk->capacity = capacity;
    stk->size = 0;
    stk->r_canary = STRUCT_CANARY;
    stk->l_canary = STRUCT_CANARY;
    stk->hash = stack_hash_func_arr(stk->data, stk->size);
    stk->previous_hash = stk->hash; 

    ASSERT_OK(stk);
    return 1;
}   

int stack_push(Stack *stk, elem value)
{   
    assert(stk != NULL);

    if (stk->size + 1 >= stk->capacity)
    {
       fprintf(log_file,"\nsize was %zd and capacity was %zd \n", stk->size, stk->capacity);
            
       stack_resize(stk, stk->capacity * 2) || ASSERTED();
    }
    
    stk->previous_hash = stk->hash;
    stk->hash = stack_hash_func_arr(stk->data, stk->size);
    ASSERT_OK(stk);

    stk->data[stk->size] = value;
    stk->size++;
    stk->previous_hash = stk->hash;
    stk->hash = stack_hash_func_arr(stk->data, stk->size);
    ASSERT_OK(stk);
    return 1;
}

int stack_pop(Stack *stk, elem *value)
{
    assert(stk != NULL);
    ASSERT_OK(stk);
    stk->previous_hash = stk->hash;
    stk->hash = stack_hash_func_arr(stk->data, stk->size);

    if (stk->size <= 0)
    {
        fprintf(log_file, "You attained the end of stack\n");
        stk->size = -1; 
        ASSERT_OK(stk);
        return -1;
    }

    if (stk->size * 4 < stk->capacity)
    {
        fprintf(log_file,"\nsize was %zd and capacity was %zd \n", stk->size, stk->capacity); // to resize
            
        stack_resize(stk, stk->capacity / 2) || ASSERTED();
    }

    *value = stk->data[stk->size-1];
    stk->size--;

    stk->previous_hash = stk->hash;
    stk->hash = stack_hash_func_arr(stk->data, stk->size);
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
    int* tmp_ptr = (int*) realloc(stk->start_arr, new_capacity * sizeof(elem) + 2 * sizeof(ARR_CANARY));

    if (tmp_ptr == NULL)
    {
        stk->capacity = -1; 
        return 0;
    }

    stk->start_arr = tmp_ptr;
    stk->end_arr = (int*) ((char*)stk->start_arr +  sizeof(elem)*new_capacity + sizeof(ARR_CANARY));
    stk->end_arr[0] = ARR_CANARY;
    stk->data = (elem*)((char*)stk->start_arr + sizeof(ARR_CANARY));
    stk->capacity = new_capacity;

    ASSERT_OK(stk);
    fprintf(log_file,"now size is %zd and capacity is %zd \n\n", stk->size, stk->capacity);

    return 1;
}
