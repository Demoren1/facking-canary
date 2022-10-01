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

    STACK_CTOR(&stack, capacity);

    for (int i = 1; i < 100; i++)
    {
        stack_push(&stack, i*i);
    }

    elem value = 0;

    for (int i = 1; i < 100; i++)
    {
        stack_pop(&stack, &value);
    }
    
    SHOW_ELEMENTS(stack);

    test_func(&stack);

    stack_dtor(&stack); 

    return 0;
}

int test_func(Stack *stk)
{
    *stk->data = 3333;
    ASSERT_OK(stk);
    return 1;
}