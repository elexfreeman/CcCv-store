#ifndef _TASK_C
#define _TASK_C
#include <stdio.h>
#include <stdbool.h>

// commands consts
#define CMD_SET 1
#define CMD_GET 2
#define CMD_DELETE 3


struct stru_task
{
  int cmd;
  void *data;
  char* key;
};


bool check_task(struct stru_task *task)
{
    if (task == NULL)
    {
        return false;
    }
    if (task->data == NULL)
    {
        return false;
    }
    return true;
}

#endif
