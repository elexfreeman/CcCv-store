#ifndef _THREAD_TASK_MANAGER_C
#define _THREAD_TASK_MANAGER_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"
#include "global.h"
#include "task.c"

void task_delete(struct stru_task *task)
{
    struct stru_task_delete *task_data;

    if (!check_task(task))
    {
        return;
    }

    if (task->cmd != CMD_DELETE)
        return;

    if (task->data == NULL)
        return;

    task_data = (struct stru_task_delete *)task->data;

    ht_remove(g_store, task_data->key);

    free(task_data);
}

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
    //    hti it = ht_iterator(g_store);
    //    while (ht_next(&it)) {
    //        printf("%s %s \r\n", it.key, (char*)it.value);
    //    }

    free(task_data);
}

void task_get(struct stru_task *task)
{
    struct stru_task_get *task_data;
    void *ptr_data;

    if (!check_task(task))
    {
        return;
    }

    if (task->cmd != CMD_GET)
        return;

    if (task->data == NULL)
        return;

    task_data = (struct stru_task_get *)task->data;
    ptr_data = ht_get(g_store, task_data->key);

    // TODO: calc msg size
    char *msg = malloc(CLIENT_MSG_SIZE);
    if (ptr_data)
    {
        snprintf(msg, CLIENT_MSG_SIZE, "2|%s|%s", task_data->key, (char *)ptr_data);
        write(task_data->sock, msg, strlen(msg));
    }
    else
    {
        snprintf(msg, CLIENT_MSG_SIZE, "2|%s|", task_data->key);
        write(task_data->sock, msg, strlen(msg));
    }

    free(msg);
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
            task_set(ptr_data);
            task_get(ptr_data);
            task_delete(ptr_data);
            free(ptr_data);
        }
    }
}

#endif