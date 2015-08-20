#include <cstdint>

extern "C" {

typedef void*   fcontext_t;

intptr_t jump_fcontext( fcontext_t * ofc, fcontext_t nfc, intptr_t vp, bool preserve_fpu = false);
fcontext_t make_fcontext( void * sp, std::size_t size, void (* fn)( intptr_t) );

}

static char stack[4096];
fcontext_t old_context;
fcontext_t new_context;

#include <iostream>

int b;
// Returns -1 for stack overflow.
int Bar(int a) {
    // 1024 is only for demostration.
    if (static_cast<void*>(stack+1024) >= __builtin_frame_address(0)) {
	return -1;
    }
    char data[100] __attribute__((unused));
    return b = Bar(a+1);
}

void Foo(intptr_t ret) {
    *reinterpret_cast<int*>(ret) = Bar(0);
    jump_fcontext(&new_context, old_context, 0);
}

int main() {
    new_context = make_fcontext(stack+sizeof(stack), sizeof(stack), &Foo);
    int ret;
    jump_fcontext(&old_context, new_context, reinterpret_cast<intptr_t>(&ret));
    if (ret == -1) {  // stack overflow
	std::cout << "Throwing stack overflow exception\n";
	throw nullptr;
    }

    return 0;
}
