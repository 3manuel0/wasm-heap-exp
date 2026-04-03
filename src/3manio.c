#include "../includes/3manio.h"
#include "../includes/wasm_mem.h"
#include "../includes/3manstr.h"


FILE *stdout = &(FILE){0, 0, 0, 1};
FILE *stderr = &(FILE){0, 0, 0, 2};

FILE *fopen(void *address, size_t size){
    FILE *f = malloc(sizeof(FILE));
    jsprintf("file is in : %d\n", f);
    f->f_ptr = (u8*)address;
    f->offset = 0;
    f->size = size;
    f->fdesc = 3;
    return f;
}

i32 fclose(FILE *stream){
    if(stream == NULL)
        return -1;
    if(stream->f_ptr == NULL)
        return -1;

    stream->size = 0;
    stream->fdesc = 0;
    stream->offset = 0;
    u8 *t = stream->f_ptr;
    memset(stream->f_ptr, 0, stream->size);
    stream->f_ptr = 0;
    free(t);
    return 0;
}

i32 fgetc(FILE *stream){
    if(stream == NULL)
        return -1;

    if(stream->f_ptr == NULL || stream->size == 0)
        return -1;

    i32 temp = *(stream->f_ptr + stream->offset);

    if(stream->offset < stream->size - 1)
        stream->offset++;

    return temp;
}

i32 ftell(FILE *stream){
    if(stream == NULL)
        return -1;

    if(stream->f_ptr == NULL || stream->size == 0)
        return -1;

    return stream->offset;
}

i32 fseek(FILE *stream, ssize_t off, i32 whence){
    if(stream == NULL)
        return -1;

    if(stream->f_ptr == NULL || stream->size == 0)
        return -1;

    if(whence > 3 || whence < 0) return -1;
    switch (whence) {
        case SEEK_SET:
            if(0 + off > stream->size || 0 + off < 0)
                return -1;
            stream->offset = 0;
            stream->offset += off;
        break;
        case SEEK_CUR:
            if(stream->offset + off > stream->size || stream->offset + off < 0)
                return -1;
            stream->offset += off;
        break;
        case SEEK_END:
            if((stream->size - 1) + off > stream->size || (stream->size - 1) + off < 0)
                return -1;
            stream->offset = stream->size - 1;
            stream->offset += off;
        break;
    }
    return 0;
}