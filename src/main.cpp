#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "../include/stack_objects.h"
#include "../include/debug.h"

FILE *log_file = no_buff_open("log_file.txt", "w");

int main()
{   
    int tmp1 = 0xABCDDCBA;
    int tmp2 = 0xFFF00000;
    int tmp3 = 0x00000FFF;
    int tmp4 = 0;
    tmp4 += 8;
    tmp2 = ((tmp1 & tmp2) >> 20) & tmp3;
    // printf("%0x\n", tmp1);
    // printf("%0x\n", tmp2);
    // printf("%0x\n", tmp4);
    // printf("%0x\n", tmp4);

    ssize_t capacity = 10;
    Stack stack= {};

    GET_INFO_FOR_DUMP(stack); //todo redo as define in ctor
    STACK_CTOR(&stack, capacity) || ASSERTED();

    for (int i = 1; i < 11; i++)
    {
        stack_push(&stack, i*i) || ASSERTED();
    }
    

    elem value  = 0;
/*
    for (int i = 1; i < 80; i++)
    {
        stack_pop(&stack, &value) || ASSERTED();
    }
*/
    //stack.l_canary = 31;
    SHOW_ELEMENTS(stack);
    ASSERT_OK(&stack);

    stack_detor(&stack);

    return 0;
}
