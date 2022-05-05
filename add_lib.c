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


/**
 * Extracts a selection of string and return a new string or NULL.
 * It supports both negative and positive indexes.
 */
char * str_slice(char str[], int slice_from, int slice_to)
{
    // if a string is empty, returns nothing
    if (str[0] == '\0')
        return NULL;

    char *buffer;
    size_t str_len, buffer_len;

    // for negative indexes "slice_from" must be less "slice_to"
    if (slice_to < 0 && slice_from < slice_to) {
        str_len = strlen(str);

        // if "slice_to" goes beyond permissible limits
        if (abs(slice_to) > str_len - 1)
            return NULL;

        // if "slice_from" goes beyond permissible limits
        if (abs(slice_from) > str_len)
            slice_from = (-1) * str_len;

        buffer_len = slice_to - slice_from;
        str += (str_len + slice_from);

    // for positive indexes "slice_from" must be more "slice_to"
    } else if (slice_from >= 0 && slice_to > slice_from) {
        str_len = strlen(str);

        // if "slice_from" goes beyond permissible limits
        if (slice_from > str_len - 1)
            return NULL;

        buffer_len = slice_to - slice_from;
        str += slice_from;

    // otherwise, returns NULL
    } else
        return NULL;

    buffer = calloc(buffer_len, sizeof(char));
    strncpy(buffer, str, buffer_len);
    return buffer;
}
#endif
