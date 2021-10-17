
#include <string.h> //strlen
#include <stdio.h>
#include <stdlib.h> //strlen

char *lib_append(char *orig, char c)
{
    size_t sz = strlen(orig);
    char *str = malloc(sz + 2);
    strcpy(str, orig);
    str[sz] = c;
    str[sz + 1] = '\0';
    return str;
}
