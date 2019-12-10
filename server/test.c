#include <stdio.h>

const char *func()
{
    static char string[10] = "ola";
    return string;
}

void main()
{
    char *string;
    string = func();
    printf("%s", string);
}