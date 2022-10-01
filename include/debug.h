#ifndef DEBUG_HEADER_GUARD
#define DEBUG_HEADER_GUARD

#define DBG printf("%s:%d -- %s\n", __FILE__, __LINE__, __FUNCTION__);

#define SHOW_ELEMENTS(stk)  static ssize_t counter = stk.size - 1;                                \
                            FILE *elements = fopen("elements_of_stack.txt", "w");                 \
                            while(counter >= 0)                                                   \
                            {                                                                     \
                                fprintf(elements, "[%3ld] = %3g, ", counter, stk.data[counter]);  \
                                counter--;                                                        \
                                if (counter % 10 == 0)                                            \
                                    fprintf(elements, "\n");                                      \
                            }                                                                     \
                            fprintf(elements, "\n");                 

#define ASSERTED() stack_print_in_logs(__LINE__, __FUNCTION__, __FILE__);
enum errors 
{
    STACK_ERROR_MEMNULL_BUFF          = 1 << 0,
    STACK_ERROR_SIZE_SMALLER_ZERO     = 1 << 1,
    STACK_ERROR_CAPACITY_SMALLER_ZERO = 1 << 2,
    STACK_ERROR_SIZE_BIGGER_CAPACITY  = 1 << 3,
    STACK_ERROR_RIGHT_CANARY_DIED     = 1 << 4,
    STACK_ERROR_LEFT_CANARY_DIED      = 1 << 5,
    STACK_ERROR_ARR_RIGHT_CANARY_DIED = 1 << 6,
    STACK_ERROR_ARR_LEFT_CANARY_DIED  = 1 << 7,
    STACK_ERROR_WRONG_HASH            = 1 << 8,
    STACK_ERROR_WRONG_STRUCT_HASH     = 1 << 9,
    STACK_ERROR_DOUBLE_CTOR           = 1 << 10,
    STACK_ERROR_DOUBLE_DTOR           = 1 << 11,
    STACK_ERROR_STK_WRONG_PTR         = 1 << 12,
    STACK_ERROR_POP_FROM_VOID_STACK   = 1 << 13
};

enum errors_in_process
{
    STACK_CREATED              = 1 << 0,
    STACK_DESTROYED            = 1 << 1,
};

#define CHECK(condition, code_of_error)  (condition) ? code_of_error : 0;

#define PRINT_ERROR(testing_var, code_of_error) (testing_var & code_of_error) ? fprintf(log_file, "%s\n", #code_of_error) : 0;

// #define HASH_PROT

#ifdef HASH_PROT
#define ON_HASH_PROT(part_of_hash_prot) part_of_hash_prot 
#else 
#define ON_HASH_PROT(part_of_hash_prot1)
#endif

#endif 