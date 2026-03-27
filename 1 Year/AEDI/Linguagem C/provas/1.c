#include <stdio.h>
#include <stdlib.h>

void a(int *x, int *y, int *z)
{
    *x += *y + *x;
    *y -= *z;
}
void b(int *x, int *y)
{
    a(y, x, y);
}

int main(void)
{

    int f = 27, s = 18;
    int *p1, *p2;

    p1 = &f;
    p2 = &s;
    a(p2, p1, &s);
    b(&f, p2);
    printf("\nf: %d", f);
    printf("\ns: %d\n", s);

    system("pause");
    return 0;
}
