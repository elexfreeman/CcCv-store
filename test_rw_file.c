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

    fp_idx = fopen(INDEX_FILE_NAME, "r+");
    fp_data = fopen(INDEX_FILE_NAME, "r+");

    idx_init(fp_idx, idx_map);

    // .........

    ht_destroy(idx_map);

    fclose(fp_data);
    fclose(fp_idx);

    return 0;
}