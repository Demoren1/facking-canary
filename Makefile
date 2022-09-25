all : mkdir stack
SRC_DIR = src/
OBJ_DIR = obj/
CC = g++

CFLAGS=#-D _DEBUG -ggdb3 -std=c++20 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-check -fstack-protector -fstrict-overflow -flto-odr-type-merging \
-fno-omit-frame-pointer -fPIE -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr \
-pie -Wlarger-than=8192 -Wstack-usage=8192

stack : $(OBJ_DIR)main.o $(OBJ_DIR)stack_func.o $(OBJ_DIR)input_output.o $(OBJ_DIR)debug_funcs.o
	@$(CC) $(CFLAGS) $(OBJ_DIR)main.o $(OBJ_DIR)stack_func.o $(OBJ_DIR)input_output.o $(OBJ_DIR)debug_funcs.o -o stack

$(OBJ_DIR)main.o : $(SRC_DIR)main.cpp 
	@$(CC) $(CFLAGS) $(SRC_DIR)main.cpp -c -o $(OBJ_DIR)main.o

$(OBJ_DIR)stack_func.o : $(SRC_DIR)stack_func.cpp
	@$(CC) $(CFLAGS) $(SRC_DIR)stack_func.cpp -c -o $(OBJ_DIR)stack_func.o

$(OBJ_DIR)input_output.o : $(SRC_DIR)input_output.cpp
	@$(CC) $(CFLAGS) $(SRC_DIR)input_output.cpp -c -o $(OBJ_DIR)input_output.o

$(OBJ_DIR)debug_funcs.o : $(SRC_DIR)debug_funcs.cpp
	@$(CC) $(CFLAGS) $(SRC_DIR)debug_funcs.cpp -c -o $(OBJ_DIR)debug_funcs.o

mkdir :
	@mkdir $(OBJ_DIR) -p

clean:
	rm $(OBJ_DIR)*.o *.save stack
