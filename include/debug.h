#ifndef DEBUG_HEADER_GUARD
#define DEBUG_HEADER_GUARD

#define DBG printf("%s:%d -- %s\n", __FILE__, __LINE__, __FUNCTION__);

#define SHOW_ELEMENTS(stk)  static ssize_t counter = stk.size - 1;                                       \
                            while(counter >= 0)                                                   \
                            {                                                                     \
                                fprintf(log_file, "[%3ld] = %3g, ", counter, stk.data[counter]);  \
                                counter--;                                                        \
                                if (counter % 10 == 0)                                            \
                                    fprintf(log_file, "\n");                                      \
                            }                                                                     \
                            fprintf(log_file, "\n");                 

#define ASSERTED() fprintf(log_file, "ERROR: Something go wrong on %d line, %s func, %s file\n", __LINE__, __FUNCTION__, __FILE__);
enum errors 
{
    STACK_ERROR_MEMNULL_FLAG          = 0x1, // 1 << 0
    STACK_ERROR_SIZE_SMALLER_ZERO     = 0x2, // 1 << 1
    STACK_ERROR_CAPACITY_SMALLER_ZERO = 0x4,
    STACK_ERROR_SIZE_BIGGER_CAPACITY  = 0x8,
    STACK_ERROR_RIGHT_CANARY_DIED     = 0x10,
    STACK_ERROR_LEFT_CANARY_DIED      = 0x20,
    STACK_ERROR_ARR_RIGHT_CANARY_DIED = 0x40,
    STACK_ERROR_ARR_LEFT_CANARY_DIED  = 0x80,
    STACK_ERROR_WRONG_HASH            = 0x100
};

#define CHECK(condition, code_of_error)  (condition) ? code_of_error : 0;
#endif 