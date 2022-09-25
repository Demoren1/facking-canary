#ifndef STACK_HEADER_GUARD
#define STACK_HEADER_GUARD

#define ASSERT_OK(stk)      if (stack_error(stk) != 0)                                                      \
                            {                                                                               \
                                stack_err_decoder(stack_error(stk));                                        \
                                stack_dump(stk, __FUNCTION__, __FILE__, __LINE__, stack_error(stk));        \
                                return 0;                                                                   \
                            }                                                                               \

#define GET_INFO_FOR_DUMP(stk)  stack.dump_info.name_of_func     = __FUNCTION__;    \
                                stack.dump_info.name_of_file     = __FILE__;        \
                                stack.dump_info.name_of_variable = #stk;            \
                                stack.dump_info.num_of_str       = __LINE__;

const int STRUCT_CANARY     = 420;
const int ARR_CANARY = 69;

typedef double elem;

struct info_of_funcs
{
    const char* name_of_func;
    const char* name_of_file;
    const char* name_of_variable;
    size_t   num_of_str;
};

struct Stack 
{
    int r_canary;
    int *start_arr;
    elem *data;
    int *end_arr;
    ssize_t size;
    ssize_t capacity;
    info_of_funcs dump_info;
    int l_canary;
    elem hash;
    elem previous_hash;
};

unsigned int stack_error(Stack *stk);

int stack_ctor(Stack *stk, ssize_t capacity);

void stack_err_decoder(unsigned int code_of_error);

int stack_push(Stack *stk, elem value);

int stack_resize(Stack *stk, ssize_t new_capacity);

int stack_pop(Stack *stk, elem *value);

void stack_detor(Stack *stk);

void stack_dump(Stack *stk, const char* name_of_inner_func, const char* name_of_inner_file, int num_of_inner_str, unsigned int flag_of_error);

elem stack_hash_func_arr(elem *arr, ssize_t size);

FILE* no_buff_open(const char* name_file, const char* regime);

#endif