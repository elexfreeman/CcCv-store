#ifndef _DISC_DATA_C
#define _DISC_DATA_C

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
} st_data;

// struct for args function
typedef struct
{
    FILE *fp;
    char key[MAX_KEY_SIZE];
    struct ht *idx_map;
} st_fnc_arg;

// -----------------------------
// INDEX FILE

/**
 * get index seek of key in idx file 
 * need for add index to index file
 * */
int idx_get_index_key(st_fnc_arg arg)
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

        if (idx_data.key == arg.key)
        {
            resp = n;
            break;
        }
    }

    return resp;
}

/**
 * Добавить позицию ключа дата файла в индекс
 * */
int idx_add_key_position(st_fnc_arg arg, int position)
{
    st_idx_data add_data;
    int idx_file_add_position = -1;

    strcpy(add_data.key, arg.key);
    add_data.position = position;

    // проверяем ключ в таблице ключей
    st_idx_data *idx_data = (st_idx_data *)ht_get(arg.idx_map, arg.key);

    if (idx_data == NULL)
    {
        // переходим в конец файла
        fseek(arg.fp, 0, SEEK_END);
        idx_file_add_position = ftell(arg.fp) / sizeof(st_idx_data);
    }
    else
    {
        // переходим на позицию записи
        idx_file_add_position = idx_get_index_key(arg);
        fseek(arg.fp, sizeof(st_idx_data), idx_get_index_key(arg));
    }

    fwrite(&add_data, sizeof(st_idx_data), 1, arg.fp);
    // write to disk
    fflush(arg.fp);

    return idx_file_add_position;
}

int idx_get_key_pos(st_fnc_arg arg)
{
    void *ptr;
    st_idx_data *data;
    int resp = 0;

    ptr = ht_get(arg.idx_map, arg.key);
    if (ptr != NULL)
    {
        data = (st_idx_data *)ptr;
        resp = data->position;
    }

    return resp;
}

void idx_remove_key(st_fnc_arg arg)
{
}

void idx_init(FILE *fp_idx, struct ht *idx_map)
{
    st_idx_data item;
    int read_flag = 0;
    int n = 0;

    for (n = 0; !feof(fp_idx); ++n)
    {
        read_flag = fread(&item, sizeof(st_idx_data), 1, fp_idx);
        if (read_flag != 1)
        {
            break;
        }
        // ...
        ht_set(idx_map, item.key, &item.position);
    }
}
#endif