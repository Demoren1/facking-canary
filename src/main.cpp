#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/stack_objects.h"
#include "../include/debug.h"

int test_func(Stack *stk);

int main()
{   
    ssize_t capacity = 10;
    Stack stack= {};

    open_logs();
    
    STACK_CTOR(&stack, capacity);

    for (int i = 1; i < 100; i++)
    {
        stack_push(&stack, i*i);
    }

    test_func(&stack);
    
    elem value = 0;

    for (int i = 1; i < 10; i++)
    {
        stack_pop(&stack, &value);
    }
    
    SHOW_ELEMENTS(stack);

    stack_dtor(&stack); 

    close_logs();

    return 0;
}

int test_func(Stack *stk)
{
    stk->l_canary = 0;
    
    ASSERT_OK(stk);
    return 1;
}
