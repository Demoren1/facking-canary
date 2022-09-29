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

    stk->code_of_error |= CHECK(stk->hash_struct != hash(stk, sizeof(Stack) - sizeof(stk->hash_struct) - 4), STACK_ERROR_WRONG_STRUCT_HASH);

    stk->code_of_error |= CHECK(!stk->data, STACK_ERROR_MEMNULL_FLAG); 
    
    stk->code_of_error |= CHECK(stk->size < 0, STACK_ERROR_SIZE_SMALLER_ZERO);
    
    stk->code_of_error |= CHECK(stk->capacity < 0, STACK_ERROR_CAPACITY_SMALLER_ZERO);
 
    stk->code_of_error |= CHECK(stk->size > stk->capacity, STACK_ERROR_SIZE_BIGGER_CAPACITY);

    stk->code_of_error |= CHECK(stk->l_canary != STRUCT_CANARY, STACK_ERROR_LEFT_CANARY_DIED);
    
    stk->code_of_error |= CHECK(stk->r_canary != STRUCT_CANARY, STACK_ERROR_RIGHT_CANARY_DIED);
    
    stk->code_of_error |= CHECK((size_t) *(stk->start_arr) != ARR_CANARY, STACK_ERROR_ARR_LEFT_CANARY_DIED);
    
    stk->code_of_error |= CHECK((size_t) *(stk->end_arr) != ARR_CANARY, STACK_ERROR_ARR_RIGHT_CANARY_DIED);
    
    stk->code_of_error |= CHECK(stk->hash != hash(stk->data, stk->capacity * sizeof(elem)), STACK_ERROR_WRONG_HASH);

    return stk->code_of_error;
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

    if(code_of_error & STACK_ERROR_WRONG_STRUCT_HASH)
    {
        fputs("\nERROR: wrong struct hash\n", log_file);
    }
}

void stack_dump(Stack *stk, const char* name_of_inner_func, const char* name_of_inner_file, int num_of_inner_str, unsigned int flag_of_error)
{
    fprintf(log_file, "%s() at %s (%d):\n", name_of_inner_func, name_of_inner_file, num_of_inner_str);
    fprintf(log_file, "Stack[%p] (%s) \"%s\" at %s() at %s (%zd) \n", stk, (flag_of_error > 0) ? "ERROR" : "OK",
        stk->dump_info.name_of_variable, stk->dump_info.name_of_func, stk->dump_info.name_of_file, stk->dump_info.num_of_str);

    fprintf(log_file, "{\n \t size                = %zd;\n"           \
                          "\t capacity            = %zd \n"           \
                          "\t data[%p]                 \n"            \
                          "\t left_arr_canary     = 0x%0x\n"          \
                          "\t right_arr_canary    = 0x%0x\n"          \
                          "\t left_struct_canary  = 0x%0lx\n"         \
                          "\t right_struct_canary = 0x%0lx\n"         \
                          "\t hash                = 0x%0x\n"          \
                          "\t struct hash         = 0x%0x\n",
                      stk->size, stk->capacity, stk->data, (int) *(stk->start_arr), (int) *(stk->end_arr), stk->l_canary, stk->r_canary, stk->hash, stk->hash_struct);
    for (int i = 0; i < stk->capacity; i++)
    {
        if (isnan(stk->data[i])) 
            fprintf(log_file,"   [%d] = %s\n", i, "NAN(POISON)");
        else if (stk->data[i] != 0)
            fprintf(log_file," * [%d] = %g\n", i, stk->data[i]);
    } 

}


int hash(void* v_arr, size_t size)
{
    char* arr = (char*) v_arr;

    int hash = 0;
    unsigned int tmp1  = 0, tmp2 = 0;
    for (int i = 0; i < size; i++)
    {
        hash += ((int) arr[i]) & 0x0000000F;

        tmp1 = 0xFFF00000;
        tmp2 = 0x00000FFF;

        tmp1 = ((hash & tmp1) >> 20) & tmp2;
        hash <<= 12;
        hash |= tmp1; 

    }
    return hash;
}