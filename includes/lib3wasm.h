#ifndef LIB_3MAN
#define LIB_3MAN
#include <stdint.h>

// ### WASM #########################################
typedef __SIZE_TYPE__ size_t; //size_t from clang 
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;

typedef int FILE;//place holder for now 

#define ALIGNMENT 8 // memory alignment for wasm32 (since it can store a double or int64 (64bits))
#define false 0
#define true 1
#define NULL ((void *)0)

#define stdout 1
#define stderr 2

typedef struct mem_header{
  i32 flag;
  size_t size;
}mem_header;

typedef struct Free_mem{
    u8 *free_mem[100];
    size_t count;
}Free_mem;

extern unsigned char __heap_base;
extern unsigned char * HEAP_BASE;
extern unsigned char * CURRENT_PTR;
extern unsigned long PAGE_LEN;
extern Free_mem FREE_MEM;
extern unsigned long __builtin_wasm_memory_grow(int memory_index, unsigned long pages);

extern unsigned long __builtin_wasm_memory_size(int memory_index);

int jsprintf(const char * t, ...);

unsigned char * wmalloc(unsigned long size);

void *malloc(size_t size);

int fwrite(const void *, unsigned long, unsigned long, FILE filedesc);

size_t strlen(const char *s);

int fprintf(FILE desc, const char *__restrict format, ...); // maybe I don't need fprintf

void *memcpy(void* dest, const void* src, size_t count);

// TODO : IMPLIMENT THESE

void *realloc(void * ptr, size_t size);

void free(void *ptr);

// ############################################################################

// ############ Arena allocator ##############################################
#define KiB(x) ((uint64_t)(x) << 10)
#define MiB(x) ((uint64_t)(x) << 20)
#define GiB(x) ((uint64_t)(x) << 30)

// single arena
typedef struct {
  void *memory;
  void *address;
  size_t capacity;
  size_t cur_size;
} Arena;

// for multiple ArenaList (linked list of Arenas)
typedef struct ArenaList {
  Arena arena;
  struct ArenaList *next;
  struct ArenaList *prev;
} ArenaList;

Arena create_Arena(size_t arena_size);
void *arena_Alloc(Arena *arena, size_t size); // reserves a size inside the arena and returns a pointer to the start of it
void arena_reset(Arena *arena); // resets the arena (!!!data inside the arena can be overwritten)
void arena_free(Arena *arena);

// for multiple Arenas (linked list of Arenas)
ArenaList *create_ArenaList(size_t size);
void *arenaList_Alloc(ArenaList **arenalist, size_t size);// reserves a size inside the arenaList and returns a pointer to the start of it
void *arenaList_Realloc(ArenaList **arenaList, void *p, size_t oldsz , size_t newsz); 
void arenaList_free(ArenaList *head);
// #############################################################################

// ############ Length-Based string and dynamicly allocated string #############
typedef struct {
  char *str;
  size_t len;
} string_view;

typedef struct {
  char *str;
  size_t len;
  size_t cap;
} string_buffer;

typedef string_view sv;
typedef string_buffer sb;

enum { str_fail = -1, str_succ, str_err };

#define sv_from_lit(str) (string_view){str, sizeof(str) - 1}

// string-view functions ###############################################
sv sv_from_cstr_sz(const char *str, size_t size);// creating a string view from char * + size

sv sv_from_cstr(const char *str);// creating a string view from char *

sv sv_from_sb(const sb *sb);// string view from string buffer (a view to that string buffer)

size_t sb_split_svs_char(const sb * sb, char delimiter, sv * sv_arr /* can be NULL*/, size_t sv_arr_len /* can be 0*/);// splits sb into sv_arr (use own array), returns the number of sub string (svs) in that sb

int sv_cmp(const sv *sv1, const sv *sv2); // compare 2 string-views

int sv_to_int64(const sv *sv, i64 *out);// return true if succesful, out is the pointer to which it writes the number

int sv_to_int32(const sv *sv, i32 *out);// return true if succesful, out is the pointer to which it writes the number

int sv_to_float64(const sv *sv, f64 *out);// TODO

void sv_println(const sv *sv); // prints sdtring-view with new line(\n)

void sv_print(const sv *sv); // prints sdtring-view  without new line

void sv_writef(const sv *sv, FILE *file); // wirtes sv to a file or stdout/stderr

// string_buffer functions ###########################################################
sb sb_from_cstr(const char *str);// creating a string-buffer from char *

sb create_sb_inside_arenaList(ArenaList **arenaList, size_t cap);

sb sb_arenaList_from_cstr_sz(ArenaList **arenaList, const char *str, size_t size); // creating a string-buffer from char * with it's size inside an areanaList

int sb_arenaList_push_cstr_sz(ArenaList **arenaList, sb *sb, const char *str, size_t size);

int sb_arenaList_push_sv(ArenaList **arenaList, sb *sb, sv sv);

sb *sb_cat(sb *dest, sb *src); // concatanate two string-buffers in the heap

sb sb_from_sv(const sv *sv); // creates a string-buffer from a string view in the heap

int sb_push_sv(sb *sb, const sv *sv); // append (push) a string-view inside a string-buffer

int sb_push_cstr(sb *sb, const char *str);// append (push) a char * inside a string-buffer

int sb_push_cstr_sz(sb *sb, const char *str, size_t size);// append (push) a char * (with its size) inside a string-buffer

int sb_push_char(sb *sb, char ch);// append (push) a charachter inside a string-buffer

char * cstr_from_sb(const sb *sb);// char * with /0 at the end from string_buffer

void sb_println(const sb *sb); // prints a string-buffer (current used bytes (chars)) with new line(\n)

void sb_print(const sb *sb); // prints a string-buffer without new line

void sb_writef(const sb *sb, FILE *file);// wirtes sb to a file or stdout/stderr

void sb_free(sb *sb); // frees string-buffer in the heap
// ###########################################################################################

#endif