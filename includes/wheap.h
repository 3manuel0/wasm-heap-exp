#ifndef WASM_HEAP
#define WASM_HEAP

extern unsigned char __heap_base;
extern unsigned long __builtin_wasm_memory_grow(int memory_index, unsigned long pages);
extern unsigned long __builtin_wasm_memory_size(int memory_index);
int jprintf(const char * t, ...);
unsigned char * wmalloc(unsigned long size);

#define KiB(x) ((unsigned long)(x) << 10)
#define MiB(x) ((unsigned long)(x) << 20)

#endif