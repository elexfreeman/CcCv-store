#ifndef _THREAD_TASK_MANAGER_C
#define _THREAD_TASK_MANAGER_C

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen

#include "config.h"
#include "global.h"
#include "task.c"


void task_set(struct stru_task *task)
{
    struct stru_task_set *task_data;

    if (!check_task(task))
    {
        return;
    }

    if (task->cmd != CMD_SET)
        return;

    if (task->data == NULL)
        return;

    task_data = (struct stru_task_set *)task->data;
    ht_set(g_store, task_data->key, task_data->data);

    // Print out words and frequencies, freeing values as we go.
    hti it = ht_iterator(g_store);
//    while (ht_next(&it)) {
//        printf("%s %s \r\n", it.key, (char*)it.value);
//    }

    free(task_data);
}

void *thread_task_manager(void *data)
{
    struct stru_task *ptr_data;
    while (true)
    {
        ptr_data = (struct stru_task *)StsQueue.pop(g_queue_task_in);
        if (ptr_data != NULL)
        {
            if (ptr_data->cmd == CMD_SET)
            {
                task_set(ptr_data);
                free(ptr_data);
            }
        }
    }
}

#endif