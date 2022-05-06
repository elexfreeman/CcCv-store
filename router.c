#ifndef _ROUTER_C
#define _ROUTER_C
#include "router.h"

#include <stdio.h>
#include <stdlib.h> //strlen
#include <string.h> //strlen
#include <uv.h>

#include "config.h"
#include "msg_parser.c"
#include "store.c"
#include "task.c"

static char *route_delete(struct stru_task *task) {
  if (task->cmd != CMD_DELETE)
    return NULL;

  int msg_len = strlen(task->key) + 4;
  char *msg = (char *)malloc(msg_len);

  store_remove(task->key);
  snprintf(msg, msg_len, "3|%s|", task->key);

  return msg;
}

static char *route_set(struct stru_task *task) {

  if (task->cmd != CMD_SET)
    return NULL;

  if (task->data == NULL) {
    return NULL;
  }

  store_set(task->key, task->data);

  // send confirm set data
  int msg_len = strlen(task->key) + 4;
  char *msg = (char *)malloc(msg_len);

  snprintf(msg, msg_len, "1|%s|", task->key);

  return msg;
}

static char *route_get(struct stru_task *task) {
  char *p_data;

  if (task->cmd != CMD_GET)
    return NULL;

  p_data = (char *)store_get(task->key);

  char *msg = NULL;

  if (p_data) {
    int msg_len = strlen(task->key) + 4 + strlen(p_data);
    msg = (char *)malloc(msg_len);
    snprintf(msg, msg_len, "2|%s|%s", task->key, (char *)p_data);
  } else {
    int msg_len = strlen(task->key) + 4;
    msg = (char *)malloc(msg_len);
    snprintf(msg, msg_len, "2|%s|", task->key);
  }

  free(p_data);

  return msg;
}

char *router(const uv_buf_t *buf) {
  int cmd;
  char *resp = NULL;

  struct stru_task *task = parse_client_msg(buf);

  if (task->cmd == CMD_SET) {
    resp = route_set(task);
  }

  if (task->cmd == CMD_GET) {
    resp = route_get(task);
  }

  if (task->cmd == CMD_DELETE) {
    resp = route_delete(task);
  }

  if (resp == NULL) {
    resp = (char *)malloc(1);
    snprintf(resp, 2, "0");
  }

  free_msg(task);

  return resp;
}

#endif
