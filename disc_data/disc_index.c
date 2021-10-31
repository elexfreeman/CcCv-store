#ifndef _DISC_INDEX_C
#define _DISC_INDEX_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "../ht.c"

#include "../config.h"
#include "disc_types.h"

// -----------------------------
// INDEX FILE

/**
 * get index seek of key in idx file 
 * need for add index to index file
 * */
static long idx_get_index_key(st_fnc_arg arg)
{
    int resp = -1;
    st_idx_data idx_data;
    int n = 0;
    int read_flag = 0;
    // Move the file pointer to the start.
    fseek(arg.fp, 0, SEEK_SET);

    for (n = 0; !feof(arg.fp); ++n)
    {
        read_flag = fread(&idx_data, sizeof(st_idx_data), 1, arg.fp);
        if (read_flag != 1)
            break;

        if (strcmp(idx_data.key, arg.key) == 0)
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
void idx_add_key_position(st_fnc_arg arg, int position)
{
    st_idx_data add_data;
    int idx_file_add_position = -1;

    strcpy(add_data.key, arg.key);
    add_data.position = position;

    // find key in memory
    st_idx_data *idx_data = (st_idx_data *)ht_get(arg.idx_map, arg.key);

    if (idx_data == NULL)
    {
        // go to end of file
        fseek(arg.fp, 0, SEEK_END);

        // add to memory
        char *key = (char *)malloc(MAX_KEY_SIZE);
        int *val = (int *)malloc(sizeof(int *));

        strcpy(key, arg.key);
        *val = position;

        ht_set(arg.idx_map, key, val);
    }
    else
    {
        fseek(arg.fp, (long)sizeof(st_idx_data) * idx_get_index_key(arg), SEEK_SET);
    }
    fwrite(&add_data, sizeof(st_idx_data), 1, arg.fp);
    // write to disk
    fflush(arg.fp);

    return;
}

int idx_get_key_pos(st_fnc_arg arg)
{
    void *ptr;
    st_idx_data *data;
    int resp = -1;

    printf("get key = %s \r\n", arg.key);
    ptr = ht_get(arg.idx_map, arg.key);
    if (ptr != NULL)
    {
        printf("ptr data = %s \r\n", (char *)ptr);
        data = (st_idx_data *)ptr;
        resp = data->position;
    }

    return resp;
}

void idx_remove_key(st_fnc_arg arg)
{
}

/**
 * Load index data from file to memory 
 * */
void idx_init(FILE *fp_idx, struct ht *idx_map)
{
    st_idx_data item;
    int read_flag = 0;
    int n = 0;

    while (feof(fp_idx) || fread(&item, sizeof(st_idx_data), 1, fp_idx) == 1)
    {
        char *key = (char *)malloc(MAX_KEY_SIZE);
        int *val = (int *)malloc(sizeof(int *));

        strcpy(key, item.key);
        *val = item.position;

        printf("key = %s | val = %d \r\n", key, *val);

        ht_set(idx_map, key, val);
    }
    puts(" ");
}

// ---------------------------
// DATA

/**
 * get index seek of key in idx file 
 * need for add index to index file
 * */
static long data_get_key_position(st_fnc_arg arg)
{
    int resp = -1;
    st_idx_data idx_data;
    int n = 0;
    int read_flag = 0;
    // Move the file pointer to the start.
    fseek(arg.fp, 0, SEEK_SET);

    for (n = 0; !feof(arg.fp); ++n)
    {
        read_flag = fread(&idx_data, sizeof(st_idx_data), 1, arg.fp);
        if (read_flag != 1)
            break;

        if (strcmp(idx_data.key, arg.key) == 0)
        {
            resp = n;
            break;
        }
    }

    return resp;
}

#endif