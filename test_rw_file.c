#include <ctype.h>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <malloc.h>
#include <unistd.h> //write
#include "config.h"

#include "disc_data/disc_data.c"

int main()
{
    FILE *fp_data;
    FILE *fp_idx;

    struct ht *idx_map;
    idx_map = ht_create();
    st_idx_data idx_data;
    st_fnc_arg arg;

    // check file exists

    if (access(DATA_FILE_NAME, F_OK) != 0)
    {
        fp_data = fopen(DATA_FILE_NAME, "w+");
        fclose(fp_data);
    }

    if (access(INDEX_FILE_NAME, F_OK) != 0)
    {
        fp_idx = fopen(INDEX_FILE_NAME, "w+");
        fclose(fp_idx);
    }

    // .........

    fp_idx = fopen(INDEX_FILE_NAME, "r+b");
    fp_data = fopen(INDEX_FILE_NAME, "r+b");

    idx_init(fp_idx, idx_map);

    arg.fp = fp_idx;
    arg.idx_map = idx_map;

    strcpy(arg.key, "key2");
    idx_add_key_position(arg, 3);

    strcpy(arg.key, "key1");
    idx_add_key_position(arg, 1);

    strcpy(arg.key, "key3");
    idx_add_key_position(arg, 5);
    strcpy(arg.key, "key3");
    int pos = idx_get_key_pos(arg);

    printf("key pos = %d", pos);

    // Print out words and frequencies, freeing values as we go.
    //            snprintf(idx_data.key, CLIENT_MSG_SIZE, "2|%s|%s", task_data->key, (char *)ptr_data);

    // .........

    ht_destroy(idx_map);

    fclose(fp_data);
    fclose(fp_idx);

    puts("");
    puts("");

    return 0;
}