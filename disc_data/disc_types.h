
#ifndef _DISC_TYPES_H
#define _DISC_TYPES_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../ht.c"

#include "../config.h"

// struct for idx data file
typedef struct
{
    int position;
    char key[MAX_KEY_SIZE];
} st_idx_data;

// struct for data file
typedef struct
{
    char key[MAX_KEY_SIZE];
    char value[MAX_VALUE_SIZE];
} st_dt;

// struct for args function
typedef struct
{
    FILE *fp;
    char key[MAX_KEY_SIZE];
    struct ht *idx_map;
} st_fnc_arg;

#endif
