extern unsigned char __heap_base;
extern unsigned long __builtin_wasm_memory_grow(int memory_index, unsigned long pages);
int jprintf(const char * t, ...);

unsigned char *heap_base(){
    *(&__heap_base) = 16;
    jprintf("%d", __heap_base);
    __builtin_wasm_memory_grow(0, 1);
    return &__heap_base;
}

