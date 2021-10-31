#ifndef _DISC_DATA_C
#define _DISC_DATA_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../ht.c"

#include "../config.h"
#include "disc_types.h"

// -----------------------------
// DATA FILE

/**
 * get index seek of key in file 
 * need for add index to file
 * */
static long dt_get_index_key(st_fnc_arg arg)
{
    int resp = -1;
    st_dt dt_data;
    int n = 0;
    int read_flag = 0;
    // Move the file pointer to the start.
    fseek(arg.fp, 0, SEEK_SET);

    for (n = 0; !feof(arg.fp); ++n)
    {
        read_flag = fread(&dt_data, sizeof(st_dt), 1, arg.fp);
        if (read_flag != 1)
            break;

        if (strcmp(dt_data.key, arg.key) == 0)
        {
            resp = n;
            break;
        }
    }

    return resp;
}

/**
 * add position to mem and file
 * */
void dt_add_key_position(st_fnc_arg arg, char *value)
{
    st_dt add_data;
    int dt_file_add_position = -1;

    strcpy(add_data.key, arg.key);
    strcpy(add_data.value, value);

    // find key in memory
    st_dt *dt_data = (st_dt *)ht_get(arg.idx_map, arg.key);

    if (dt_data == NULL)
    {
        // go to end of file
        fseek(arg.fp, 0, SEEK_END);

        // add to memory
        char *key = (char *)malloc(MAX_KEY_SIZE);
        char *val = (char *)malloc(MAX_VALUE_SIZE);

        strcpy(key, arg.key);
        strcpy(val, value);


        ht_set(arg.idx_map, key, val);
    }
    else
    {
        fseek(arg.fp, (long)sizeof(st_dt) * dt_get_index_key(arg), SEEK_SET);
    }
    fwrite(&add_data, sizeof(st_dt), 1, arg.fp);
    // write to disk
    fflush(arg.fp);

    return;
}


void dt_remove_key(st_fnc_arg arg)
{
}

/**
 * Load index data from file to memory 
 * */
void dt_init(FILE *fp_idx, struct ht *dt_map)
{
    st_dt item;
    int read_flag = 0;
    int n = 0;

    while (feof(fp_idx) || fread(&item, sizeof(st_dt), 1, fp_idx) == 1)
    {
        char *key = (char *)malloc(MAX_KEY_SIZE);
        char *val = (char *)malloc(MAX_VALUE_SIZE);

        strcpy(key, item.key);
        strcpy(val, item.value);

        printf("key = %s | val = %s \r\n", key, val);

        ht_set(dt_map, key, val);
    }
    puts(" ");
}


#endif