#ifndef _ROUTER_C
#define _ROUTER_C
#include <stdio.h>
#include <stdlib.h> //strlen
#include <string.h> //strlen

#include "config.h"
#include "msg_parser.c"
#include "store.c"
#include "task.c"

static char *route_delete(struct stru_task *task) {
  struct stru_task_delete *task_data;

  if (!check_task(task)) {
    return NULL;
  }

  if (task->cmd != CMD_DELETE)
    return NULL;

  if (task->data == NULL)
    return NULL;

  task_data = (struct stru_task_delete *)task->data;

  store_remove(task_data->key);
  char *msg = malloc(3 + strlen(task_data->key));
  snprintf(msg, CLIENT_MSG_SIZE, "3|%s|", task_data->key);

  return msg;
}

static char *route_set(struct stru_task *task) {
  struct stru_task_set *task_data;

  if (!check_task(task)) {
    return NULL;
  }

  if (task->cmd != CMD_SET)
    return NULL;

  if (task->data == NULL)
    return NULL;

  task_data = (struct stru_task_set *)task->data;
  store_set(task_data->key, task_data->data);

  // send confirm set data
  char *msg = malloc(3 + strlen(task_data->key));
  snprintf(msg, CLIENT_MSG_SIZE, "1|%s|", task_data->key);

  free(task_data);

  return msg;
}

static char *route_get(struct stru_task *task) {
  struct stru_task_get *task_data;
  void *ptr_data;

  if (!check_task(task)) {
    return NULL;
  }

  if (task->cmd != CMD_GET)
    return NULL;

  if (task->data == NULL)
    return NULL;

  task_data = (struct stru_task_get *)task->data;
  ptr_data = store_get(task_data->key);

  // TODO: calc msg size
  char *msg = malloc(CLIENT_MSG_SIZE);
  if (ptr_data) {
    snprintf(msg, CLIENT_MSG_SIZE, "2|%s|%s", task_data->key, (char *)ptr_data);
  } else {
    snprintf(msg, CLIENT_MSG_SIZE, "2|%s|", task_data->key);
  }

  free(msg);
  free(task_data);

  return msg;
}

char *router(void *buff) {
  int cmd;
  char *resp;
  // got data from the client.
  fprintf(stdout, "cmd \n");
  cmd = parse_client_msg_cmd((char*)buff);

  if (cmd == CMD_SET) {
    struct stru_task *task = malloc(sizeof(struct stru_task *));
    task->cmd = CMD_SET;
    fprintf(stdout, "1 \n");
    task->data = parse_client_msg_set(1, buff);
    fprintf(stdout, "2 \n");
    resp = route_set(task);
  } else if (cmd == CMD_GET) {
    struct stru_task *task = malloc(sizeof(struct stru_task *));
    task->cmd = CMD_GET;
    task->data = parse_client_msg_get(1, buff);
    resp = route_get(task);
  } else if (cmd == CMD_DELETE) {
    struct stru_task *task = malloc(sizeof(struct stru_task *));
    task->cmd = CMD_DELETE;
    task->data = parse_client_msg_delete(1, buff);
    resp = route_delete(task);
  }

  return resp;
}

#endif
