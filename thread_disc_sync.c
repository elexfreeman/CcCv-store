#ifndef _THREAD_DISC_SYNC_C
#define _THREAD_DISC_SYNC_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "global.h"
#include "task.c"
#include "ht.c"

//void task_delete(struct stru_task *task)
//{
//struct stru_task_delete *task_data;

//if (!check_task(task))
//{
//return;
//}

//if (task->cmd != CMD_DELETE)
//return;

//if (task->data == NULL)
//return;

//task_data = (struct stru_task_delete *)task->data;

//ht_remove(g_store, task_data->key);

//free(task_data);
//}

//void task_set(struct stru_task *task)
//{
//struct stru_task_set *task_data;

//if (!check_task(task))
//{
//return;
//}

//if (task->cmd != CMD_SET)
//return;

//if (task->data == NULL)
//return;

//task_data = (struct stru_task_set *)task->data;
//ht_set(g_store, task_data->key, task_data->data);

//// Print out words and frequencies, freeing values as we go.
////    hti it = ht_iterator(g_store);
////    while (ht_next(&it)) {
////        printf("%s %s \r\n", it.key, (char*)it.value);
////    }

//free(task_data);
//}

//void task_get(struct stru_task *task)
//{
//struct stru_task_get *task_data;
//void *ptr_data;

//if (!check_task(task))
//{
//return;
//}

//if (task->cmd != CMD_GET)
//return;

//if (task->data == NULL)
//return;

//task_data = (struct stru_task_get *)task->data;
//ptr_data = ht_get(g_store, task_data->key);

//// TODO: calc msg size
//char *msg = (char *)malloc(CLIENT_MSG_SIZE);
//if (ptr_data)
//{
//snprintf(msg, CLIENT_MSG_SIZE, "2|%s|%s", task_data->key, (char *)ptr_data);
//write(task_data->sock, msg, strlen(msg));
//}
//else
//{
//snprintf(msg, CLIENT_MSG_SIZE, "2|%s|", task_data->key);
//write(task_data->sock, msg, strlen(msg));
//}

//free(msg);
//free(task_data);
//}



void *thread_disc_sync(void *data)
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

    ht_destroy(idx_map);
}

#endif