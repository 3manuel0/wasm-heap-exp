#include "includes/lib3wasm.h"



i64 test_sv(){
    sv tests[] = {
        sv_from_lit("123.456"),
        sv_from_lit("-987"),
        sv_from_lit("000.000100"),
        sv_from_lit(".505"),
        sv_from_lit("1.23e-4"),
        sv_from_lit("5.5E+2"),
        sv_from_lit("0.12345678901234567"),
        sv_from_lit("3.14159junk"),
        sv_from_lit("12.34.56"),
        sv_from_lit("4.9e-324"),// too small rounds to 0 
        sv_from_lit("1.7976931348623157891575E+308"),// max_float64
        sv_from_lit("1.8e+308"), // too big for (inf/infinity)
        sv_from_lit("80")
    };
    f64 t = 0;
    for(int i = 0; i < 13; i++){
        jsprintf("sv : ");
        sv_println(&tests[i]);
        i32 s = sv_to_float64(&tests[i], &t);
        jsprintf("float64 : %lf sv_ptr : %p\n", t, tests[i].str);
    }
    return t;
}

void test_ArenaList(){
    ArenaList * a = create_ArenaList(KiB(5));
    jsprintf("capacity :%d adress :%d cur_size: %d\n", a->arena.capacity, a->arena.address, a->arena.cur_size);
    
    string_view sv = sv_from_lit("Testing testing 1234 sdasdadas sdasdasdasd asdasdadasdaasd adadads");
    for(int i = 0; i < 50; i++){
        string_view *s = arenaList_Alloc(&a, sv.len);
        jsprintf("capacity :%d adress :%d cur_size: %d\n", a->arena.capacity, a->arena.address, a->arena.cur_size);
        jsprintf("string_view *: %d\n", s);
        sv_println(&sv);
    }
    char *s = arenaList_Alloc(&a, 800);
    arenaList_Alloc(&a, KiB(5));
    arenaList_Alloc(&a, 800);
    // s[0] = '1';
    jsprintf("capacity :%d adress :%d cur_size: %d next: %d prev: %d\n", a->arena.capacity, a->arena.address, a->arena.cur_size, a->next, a->prev);
    arenaList_free(a);
    for(int i = 0; i < FREE_MEM.count;  i++){
        jsprintf("%d %d \n", FREE_MEM.free_mem[i], ((mem_header*)FREE_MEM.free_mem[i] - 1)->size);
    }
    arenaList_free(a);

    return;
}

void test_sb(){
    return;
}