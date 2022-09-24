#ifndef DEBUG_HEADER_GUARD
#define DEBUG_HEADER_GUARD

#define DBG printf("%s:%d -- %s\n", __FILE__, __LINE__, __FUNCTION__);

#define SHOW_ELEMENTS(stk)  int i = stk.size - 1;                                   \
                            while(i >= 0)                                           \
                            {                                                       \
                                fprintf(log_file, "[%3d] = %3g, ", i, stk.data[i]);   \
                                i--;                                                \
                                if (i%10 == 0)                                      \
                                    fprintf(log_file, "\n");                        \
                            }                                                       \
                            fprintf(log_file, "\n");                 

#define ASSERTED() fprintf(log_file, "ERROR: Something go wrong on %d line, %s func, %s file\n", __LINE__, __FUNCTION__, __FILE__);

const unsigned int STACK_ERROR_MEMNULL_FLAG          = 0x1;

const unsigned int STACK_ERROR_SIZE_SMALLER_ZERO     = 0x2;

const unsigned int STACK_ERROR_CAPACITY_SMALLER_ZERO = 0x4;

const unsigned int STACK_ERROR_SIZE_BIGGER_CAPACITY  = 0x8;

const unsigned int STACK_ERROR_RIGHT_CANARY_DIED     = 0x10;

const unsigned int STACK_ERROR_LEFT_CANARY_DIED      = 0x20;

#endif 



/* 
CRC32
MD5
murmur.hash
*/ 