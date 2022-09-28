#include <stdio.h>
#include <assert.h>
#include "../include/stack_objects.h"

FILE* no_buff_open(const char* name_file, const char* regime)
{
    FILE* file_ptr = fopen(name_file, regime);
    assert(file_ptr != NULL);

    setvbuf(file_ptr, NULL, _IONBF, 0);
    
    return file_ptr;
}

