#include "includes/lib3wasm.h"


Arena a = {0};

void test(){
    sv test = sv_from_lit("test sv from wasm\n");
    jprintf("%d %d\n", test.len, test.str);
    wmalloc(533);
    sv_print(&test);
}

void *test2(){
    a = create_Arena(500);
    jprintf("capacity :%d adress :%d cur_size: %d\n", a.capacity, a.address, a.cur_size);
    
    char * s = arena_Alloc(&a, 200);
    jprintf("capacity :%d adress :%d cur_size: %d\n", a.capacity, a.address, a.cur_size);
    for(int i = 0; i < 199; i ++){
        s[i] = 'S';
    }
    s[199] = 0;
    jprintf("s address = %d\n", s);
    return s;
}