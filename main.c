#include "includes/wheap.h"

unsigned char * HEAP_BASE = &__heap_base;
unsigned char * CURRENT_PTR = &__heap_base;
unsigned long PAGE_LEN = KiB(64);

unsigned char *heap_base(){
    long mb = MiB(1);
    float s =  (float)mb / PAGE_LEN;
    jprintf("%f %d", s, PAGE_LEN);
    __builtin_wasm_memory_grow(0, 1);
    jprintf("%d %d", HEAP_BASE, __builtin_wasm_memory_size(0));
    return HEAP_BASE;
}

unsigned char *wmalloc(unsigned long size){
    unsigned char* ptr = CURRENT_PTR;
    unsigned long pages = __builtin_wasm_memory_size(0);
    if((unsigned long)CURRENT_PTR + size < pages * PAGE_LEN){
        CURRENT_PTR += size;
        return ptr;
    }
    unsigned long i = size / PAGE_LEN;
    if(i == 0)
        __builtin_wasm_memory_grow(0, 1);
    else 
        __builtin_wasm_memory_grow(0, i + 1);
    CURRENT_PTR += size;
    return ptr;

}