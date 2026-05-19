#include <stdio.h>
#include <stdbool.h>

static int    pas_read_int(void)    { int v = 0; (void)scanf("%d", &v); return v; }
static float  pas_read_real(void)   { float v = 0.0f; (void)scanf("%f", &v); return v; }
static char   pas_read_char(void)   { char v = 0; (void)scanf(" %c", &v); return v; }
static void   pas_write_int(int v)  { (void)printf("%d", v); }
static void   pas_write_real(float v)  { (void)printf("%f", v); }
static void   pas_write_char(char v)   { (void)printf("%c", v); }
static void   pas_write_str(const char* v) { (void)printf("%s", v); }
static void   pas_writeln(void)     { (void)printf("\n"); }

int a = 0;
int b = 0;
int result = 0;

int add(int x, int y);

int add(int x, int y) {
    int __ret_add = 0;
    __ret_add = (x + y);
    return __ret_add;
}

int main(void) {
    a = 10;
    b = 20;
    result = add ( a, b );
    pas_write_int(result);
    return 0;
}
