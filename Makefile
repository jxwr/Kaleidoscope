
LD_FLAGS = $(shell llvm-config --ldflags)
LLVM_CONFIG = $(shell llvm-config --libs core --cppflags) $(LD_FLAGS) 

all:
	g++ main.cpp llvm.cpp ast.cpp -o spi -std=c++11 -Werror $(LLVM_CONFIG)
