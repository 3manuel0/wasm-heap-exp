#include "../includes/wasm_mem.h"
#include "../includes/lib3man.h"

Free_mem FREE_MEM = {0};
u8 *HEAP_BASE = &__heap_base;
u8 *CURRENT_PTR = &__heap_base;
size_t PAGE_LEN = KiB(64);

unsigned char *heap_base(){
    long mb = MiB(1);
    float s =  (float)mb / PAGE_LEN;
    // jsprintf("%f %d\n", s, PAGE_LEN);
    __builtin_wasm_memory_grow(0, 1);
    // jsprintf("%d %d\n", HEAP_BASE, __builtin_wasm_memory_size(0));
    return HEAP_BASE;
}

// TODO : ADD THE MEMORY HEADER BEFORE THE ALLOCATION FOR FREE
unsigned char *wmalloc(unsigned long size){
    // jsprintf("CURRENT_PTR = %d\n", CURRENT_PTR);
    unsigned char *ptr = CURRENT_PTR + sizeof(mem_header);
    size_t alloc_size = ((size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1));
    size_t total_size = sizeof(mem_header) + alloc_size;
    unsigned long pages = __builtin_wasm_memory_size(0);
    // jsprintf("size of mem_header : %u\n", sizeof(mem_header));
    if((unsigned long)CURRENT_PTR + total_size <= pages * PAGE_LEN){
        ((mem_header *)CURRENT_PTR)->size = alloc_size;
        ((mem_header *)CURRENT_PTR)->flag = 1;
        // DEBUG 
        // jsprintf("CURRENT_PTR = %d ptr = %d  %d %d\n", CURRENT_PTR, ptr, ((mem_header *)CURRENT_PTR)->size, ((mem_header *)CURRENT_PTR)->flag);
        CURRENT_PTR += total_size;
        return ptr;
    }
    unsigned long i = size / PAGE_LEN;
    if(i == 0){
        if (__builtin_wasm_memory_grow(0, 1) == -1)
  	        return NULL;
    }
    else{
        if(__builtin_wasm_memory_grow(0, i + 1) == -1){
            return NULL;
        }
    }
    ((mem_header *)CURRENT_PTR)->size = alloc_size;
    ((mem_header *)CURRENT_PTR)->flag = 1;
    CURRENT_PTR += total_size;
    // DEBUG 
    // jsprintf("CURRENT_PTR = %d\n", CURRENT_PTR);
    return ptr;
}

void *malloc(size_t size){
    return wmalloc(size);
}

void free(void *ptr){
    mem_header *h = (mem_header *) ptr - 1;
    if(h->flag == false){
        jsprintf("memory already freed\n");
        return;
    } 
    h->flag = false;
    FREE_MEM.free_mem[FREE_MEM.count] = ptr;
    FREE_MEM.count += 1;
    return;
}

