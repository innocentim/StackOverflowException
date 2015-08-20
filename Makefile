all:
	g++ -std=c++11 stack_overflow_exception.cc jump_x86_64_sysv_elf_gas.S make_x86_64_sysv_elf_gas.S -o stack_overflow_exception
