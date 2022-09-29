#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/stack_objects.h"
#include "../include/debug.h"

int test_func(Stack *stk);

FILE *log_file = no_buff_open("log_file.txt", "w");

int main()
{   
    ssize_t capacity = 10;
    Stack stack= {};

    STACK_CTOR(&stack, capacity) || ASSERTED();

    for (int i = 1; i < 100; i++)
    {
        stack_push(&stack, i*i) || ASSERTED();
    }
    

    elem value  = 0;

    for (int i = 1; i < 50; i++)
    {
        stack_pop(&stack, &value) || ASSERTED();
    }
    
    SHOW_ELEMENTS(stack);

    //test_func(&stack);

    stack_detor(&stack);

    return 0;
}

int test_func(Stack *stk)
{
    stk->l_canary = 0;
    ASSERT_OK(stk);
    return 1;
}