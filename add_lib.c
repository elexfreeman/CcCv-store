#ifndef _ADD_LIB_C
#define _ADD_LIB_C
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

char *lib_append(char *orig, char c)
{
    size_t sz = strlen(orig);
    char *str = malloc(sz + 2);
    strcpy(str, orig);
    str[sz] = c;
    str[sz + 1] = '\0';
    return str;
}

char *str_optimize(char *str)
{
    char *str_opt;
    int idx = 0;

    for (int i = 0; i < (int)malloc_usable_size(str); i++)
    {
        if (str[i] != '\0')
        {
            idx++;
        }
    }

    str_opt = malloc(idx);
    strcpy(str_opt, str);
    
    return str_opt;
}
#endif