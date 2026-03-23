#include "includes/lib3masm.h"


Arena a = {0};

void test(){
    sv test = sv_from_lit("test sv from wasm");
    jprintf("%d %d", test.len, test.str);
    wmalloc(533);
    sv_print(&test);
}

void *test2(){
    a = create_Arena(500);
    jprintf("capacity :%d adress :%d cur_size: %d", a.capacity, a.address, a.cur_size);
    return arena_Alloc(&a, 2);
}