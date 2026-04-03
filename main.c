#include "includes/lib3man.h"



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
    u8 * fake_file = malloc(200);
    FILE *fp = fopen(fake_file, 200);
    memset(fake_file, 'A', 200);
    jsprintf("fp->offset : %d fp->size : %d %d\n", fp->offset, fp->size, fake_file);
    for(int i = 0; i < 10; i++)
        jsprintf("fgetc : %c\n", fgetc(fp));
    jsprintf("ftell : %d\n", ftell(fp));
    arenaList_free(a);
    return;
}

void test_sb(){
    return;
}