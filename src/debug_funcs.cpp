#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/debug.h"
#include "../include/stack_objects.h"

extern FILE* log_file;

unsigned int stack_error(Stack *stk)
{
    assert(stk != NULL);

    unsigned int code_of_error = 0;

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

    if (stk->r_canary != STRUCT_CANARY)
    {
        code_of_error |= STACK_ERROR_LEFT_CANARY_DIED;
    }

    if (stk->r_canary != STRUCT_CANARY)
    {
        code_of_error |= STACK_ERROR_RIGHT_CANARY_DIED;
    }

    if ((int) *(stk->start_arr) != ARR_CANARY)
    {
        code_of_error |= STACK_ERROR_ARR_LEFT_CANARY_DIED;
    }

    if ((int) *(stk->end_arr) != ARR_CANARY)
    {
        code_of_error |= STACK_ERROR_ARR_RIGHT_CANARY_DIED;
    }

    if (stk->hash != stack_hash_func_arr(stk->data, stk->size))
    {
        code_of_error |= STACK_ERROR_WRONG_HASH;
    }
    return code_of_error;

}

void stack_err_decoder(unsigned int code_of_error)
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

    if (code_of_error & STACK_ERROR_LEFT_CANARY_DIED)
    {
        fputs("\nERROR: left canary died\n", log_file);
    }

    if (code_of_error & STACK_ERROR_RIGHT_CANARY_DIED)
    {
        fputs("\nERROR: right canary died\n", log_file);
    }

    if (code_of_error & STACK_ERROR_ARR_LEFT_CANARY_DIED)
    {
        fputs("\nERROR: left array canary died\n", log_file);
    }

    if (code_of_error & STACK_ERROR_ARR_RIGHT_CANARY_DIED)
    {
        fputs("\nERROR: right array canary died\n", log_file);
    }

    if (code_of_error & STACK_ERROR_WRONG_HASH)
    {
        fputs("\nERROR: wrong hash\n", log_file);
    }
}

void stack_dump(Stack *stk, const char* name_of_inner_func, const char* name_of_inner_file, int num_of_inner_str, unsigned int flag_of_error)
{
    fprintf(log_file, "%s() at %s (%d):\n", name_of_inner_func, name_of_inner_file, num_of_inner_str);
    fprintf(log_file, "Stack[%p] (%s) \"%s\" at %s() at %s (%zd) \n", stk, (flag_of_error > 0) ? "ERROR" : "OK",
        stk->dump_info.name_of_variable, stk->dump_info.name_of_func, stk->dump_info.name_of_file, stk->dump_info.num_of_str);

    fprintf(log_file, "{\n \t size              = %zd;\n"           \
                          "\t capacity          = %zd \n"           \
                          "\t data[%p]         \n"                  \
                          "\t left_arr_canary   = %d\n"             \
                          "\t right_arr_canary  = %d\n"             \
                          "\t previous hash     = %g\n"             \
                          "\t hash              = %g\n",
                      stk->size, stk->capacity, stk->data, (int) *(stk->start_arr), (int) *(stk->end_arr), stk->previous_hash, stk->hash);
    for (int i = 0; i < stk->capacity; i++)
    {
        if (stk->data[i] != 0)
            fprintf(log_file," * [%d] = %g\n", i, stk->data[i]);
        else    
            fprintf(log_file,"   [%d] = %s\n", i, "NAN(POISON)");
    } 

}

elem stack_hash_func_arr(elem *arr, ssize_t size)
{
    elem s = 0;
    for (int i = 0; i < size; i++)
    {
        s += arr[i] / (fabs(s)+1);
    }
    return s;
}