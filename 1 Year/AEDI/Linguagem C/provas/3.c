#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    char title[100];
    int year;
} MOVIES_T;

MOVIES_T *abc()
{
    MOVIES_T *ptr;
    char mystr[5];

    ptr = (MOVIES_T *)malloc(sizeof(MOVIES_T));

    strcpy(ptr->title, "o dia D");
    strcpy(mystr, "2021");
    ptr->year = atoi(mystr);
    return (ptr);
}

int main(int argc, char *argv[])
{
    MOVIES_T *pmovie;

    pmovie = abc();
    printf("%s %d", pmovie->title, pmovie->year);
    free(pmovie);
    return 0;
}