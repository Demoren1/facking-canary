#ifndef STACK_HEADER_GUARD
#define STACK_HEADER_GUARD

#define ASSERT_OK(stk)      if (stack_error(stk) != 0)                                                      \
                            {                                                                               \
                                stack_err_decoder(stack_error(stk));                                        \
                                stack_dump(stk, __FUNCTION__, __FILE__, __LINE__, stack_error(stk));        \
                                return 0;                                                                   \
                            }                                                                               \

#define GET_INFO_FOR_DUMP(stack)  stack.dump_info.name_of_func     = __FUNCTION__;    \
                                  stack.dump_info.name_of_file     = __FILE__;        \
                                  stack.dump_info.name_of_variable = #stack;          \
                                  stack.dump_info.num_of_str       = __LINE__;
#define VAR_INFO(stack) __FUNCTION__, __FILE__, #stack, __LINE__ 

#define STACK_CTOR(stack, capacity) stack_ctor(stack, capacity, VAR_INFO(stack))

const size_t STRUCT_CANARY     = 0xF00DB00B;  
const size_t ARR_CANARY        = 0xFAABCCBA;   

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
    size_t l_canary;
    size_t *start_arr;
    elem *data;
    size_t *end_arr;
    ssize_t size;
    ssize_t capacity;
    info_of_funcs dump_info;
    size_t r_canary;
    elem hash;
    elem previous_hash;   //todo think about delete
};

unsigned int stack_error(Stack *stk);

int stack_ctor(Stack *stk, ssize_t capacity, const char* name_function, const char* name_file, const char* nmae_variable, int num_line);

int stack_dump_info_ctor(Stack *stk, const char* name_function, const char* name_file, const char* name_variable, int num_line);

void stack_err_decoder(unsigned int code_of_error);

int stack_push(Stack *stk, elem value);

int stack_resize(Stack *stk, ssize_t new_capacity);

int stack_pop(Stack *stk, elem *value);

int stack_poison_get(Stack *stk, int size, int capacity);

void stack_detor(Stack *stk);

void stack_dump(Stack *stk, const char* name_of_inner_func, const char* name_of_inner_file, int num_of_inner_str, unsigned int flag_of_error);

elem stack_hash_func_arr(elem *arr, ssize_t size);     //todo stack_hash_func(void *)

// ToDo:
// hash (void*, size) // не связана со стеком
// rehash (stk) {hash (stk); hash (stk->buf)}
// offsetof

FILE* no_buff_open(const char* name_file, const char* regime);

#endif