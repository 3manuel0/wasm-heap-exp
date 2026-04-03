#include "../includes/3manstr.h"

size_t strlen(const char *s){
    size_t len = 0;
    while(*s++){
        len++;
    }
    return len;
}

void *memcpy(void *dest, const void *src, size_t count){
    if(dest == NULL || src == NULL)
        return NULL;
    for(size_t i = 0; i < count; i++){
        ((u8 *)dest)[i] = ((u8 *)src)[i];
    }
    return dest;
}

void *memset(void * s, i32 c, size_t n){
    return __builtin_memset(s, c, n);
}