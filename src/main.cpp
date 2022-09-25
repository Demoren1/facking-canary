#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/stack_objects.h"
#include "../include/debug.h"

FILE *log_file = no_buff_open("log_file.txt", "w");

int main()
{   
    ssize_t capacity = 5;
    Stack stack= {};

    GET_INFO_FOR_DUMP(stk);
    stack_ctor(&stack, capacity) || ASSERTED();
    
    for (int i = 1; i < 10; i++)
    {
        GET_INFO_FOR_DUMP(stk);
        stack_push(&stack, i*i) || ASSERTED();
    }

    elem value  = 0;

    for (int i = 1; i < 5; i++)
    {
        GET_INFO_FOR_DUMP(stk);
        stack_pop(&stack, &value) || ASSERTED();
    }


    GET_INFO_FOR_DUMP(stk);
    stack_detor(&stack);

    return 0;
}
