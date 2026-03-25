#include "../includes/lib3wasm.h"

unsigned char *HEAP_BASE = &__heap_base;
unsigned char *CURRENT_PTR = &__heap_base;
unsigned long PAGE_LEN = KiB(64);

unsigned char *heap_base(){
    long mb = MiB(1);
    float s =  (float)mb / PAGE_LEN;
    jprintf("%f %d", s, PAGE_LEN);
    __builtin_wasm_memory_grow(0, 1);
    jprintf("%d %d", HEAP_BASE, __builtin_wasm_memory_size(0));
    return HEAP_BASE;
}

// TODO : ADD THE MEMORY HEADER BEFORE THE ALLOCATION FOR FREE
unsigned char *wmalloc(unsigned long size){
    unsigned char *ptr = CURRENT_PTR + sizeof(mem_header);
    size_t alloc_size = ((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
    size_t total_size = sizeof(mem_header) + alloc_size;
    unsigned long pages = __builtin_wasm_memory_size(0);
    jprintf("size of mem_header : %u ", sizeof(mem_header));
    if((unsigned long)CURRENT_PTR + total_size <= pages * PAGE_LEN){
        ((mem_header *)CURRENT_PTR)->size = alloc_size;
        ((mem_header *)CURRENT_PTR)->flag = 1;
        // DEBUG 
        jprintf("CURRENT_PTR = %d ptr = %d  %d %d", CURRENT_PTR, ptr, ((mem_header *)CURRENT_PTR)->size, ((mem_header *)CURRENT_PTR)->flag);
        CURRENT_PTR += total_size;
        return ptr;
    }
    unsigned long i = size / PAGE_LEN;
    if(i == 0)
        __builtin_wasm_memory_grow(0, 1);
    else 
        __builtin_wasm_memory_grow(0, i + 1);
    ((mem_header *)CURRENT_PTR)->size = alloc_size;
    ((mem_header *)CURRENT_PTR)->flag = 1;
    CURRENT_PTR += total_size;
    // DEBUG 
    jprintf("CURRENT_PTR = %d", CURRENT_PTR);
    return ptr;
}

void *memcpy(void *dest, const void *src, size_t count){
    for(size_t i = 0; i < count; i++){
        ((u8 *)dest)[i] = ((u8 *)src)[i];
    }
    return dest;
}

void *malloc(size_t size){
    return wmalloc(size);
}

size_t strlen(const char *s){
    size_t len = 0;
    while(*s++){
        len++;
    }
    return len;
}
