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
    
    stk->data = (elem*) calloc(capacity, sizeof(elem) + 2 * sizeof(ARR_CANARY));
    stk->capacity = capacity;
    stk->size = 0;
    stk->r_canary = CANARY;
    stk->l_canary = CANARY;

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

    ASSERT_OK(stk);

    stk->data[stk->size] = value;
    stk->size++;

    ASSERT_OK(stk);
    return 1;
}

int stack_pop(Stack *stk, elem *value)
{
    assert(stk != NULL);
    ASSERT_OK(stk);
    
    if (stk->size == 0)
    {
        fprintf(log_file, "You attained the end of stack\n");
        stk->size = 1; 
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

    ASSERT_OK(stk);
    return 1;
}

void stack_detor(Stack *stk)
{
    if(stk->data == NULL)
    {
        free(stk->data);
    }
    stk->capacity = -1;
    stk->size     = -1;
}

int stack_error(Stack *stk)
{
    assert(stk != NULL);

    int code_of_error = 0;

    if (!stk->data) 
    {
        code_of_error |= STACK_ERROR_MEMNULL_FLAG;
    }
    
    if(stk->size < 0)
    {
        code_of_error |= STACK_ERROR_SIZE_SMALLER_ZERO;
    }
    
    if (stk->capacity < 0)
    {
        code_of_error |= STACK_ERROR_CAPACITY_SMALLER_ZERO;
    }
 
    if (stk->size > stk->capacity)
    {
        code_of_error |= STACK_ERROR_SIZE_BIGGER_CAPACITY;
    }

    if(stk->r_canary != CANARY)
    {
        code_of_error |= STACK_ERROR_RIGHT_CANARY_DIED;
    }

    if(stk->r_canary != CANARY)
    {
        code_of_error |= STACK_ERROR_LEFT_CANARY_DIED;
    }

    return code_of_error;

}

void stack_err_decoder(int code_of_error)
{
    if (code_of_error & STACK_ERROR_MEMNULL_FLAG)
    {
        fputs("\nERROR: data point on NULL\n", log_file);
    }
    
    if (code_of_error & STACK_ERROR_SIZE_SMALLER_ZERO)
    {
        fputs("\nERROR: size < 0\n", log_file);
    }
    
    if(code_of_error & STACK_ERROR_CAPACITY_SMALLER_ZERO)
    {
        fputs("\nERROR: capacity < 0\n", log_file);
    }

    if (code_of_error & STACK_ERROR_SIZE_BIGGER_CAPACITY)
    {
        fputs("\nERROR: size > capacity\n", log_file);
    }
}

int stack_resize(Stack *stk, ssize_t new_capacity)
{   
    
    elem* tmp_ptr = (elem*) realloc(stk->data, new_capacity * sizeof(elem));

    if (tmp_ptr == NULL)
    {
        stk->capacity = -1; 
        return 0;
    }

    stk->data = tmp_ptr;
    stk->capacity = new_capacity;
    ASSERT_OK(stk);
    fprintf(log_file,"now size is %zd and capacity is %zd \n\n", stk->size, stk->capacity);

    return 1;
}

void stack_dump(Stack *stk, const char* name_of_inner_func, const char* name_of_inner_file, int num_of_inner_str, int flag_of_error)
{
    fprintf(log_file, "%s() at %s (%d):\n", name_of_inner_func, name_of_inner_file, num_of_inner_str);
    fprintf(log_file, "Stack[%p] (%s) \"%s\" at %s() at %s (%zd) \n", stk, (flag_of_error > 0) ? "ERROR" : "OK",
        stk->dump_info.name_of_variable, stk->dump_info.name_of_func, stk->dump_info.name_of_file, stk->dump_info.num_of_str);

    fprintf(log_file, "{\n \t size     = %zd;\n"            \
                          "\t capacity = %zd \n"            \
                          "\t data[%p] \n",
                      stk->size, stk->capacity, stk->data);
    for (int i = 0; i < stk->capacity; i++)
    {
        if (stk->data[i] != 0)
            fprintf(log_file," * [%d] = %g\n", i, stk->data[i]);
        else    
            fprintf(log_file," * [%d] = %s\n", i, "NAN(POISON)");
    } 

}