#ifndef _MSG_PARSER_C
#define _MSG_PARSER_C

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include "add_lib.c"
#include "config.h"
#include "global.h"

#include "task.c"

int parse_client_msg_cmd(const struct uv_buf_t *buf) {
  if (buf->len == 0) {
    return 0;
  }
  int cmd = 0;
  if (buf->base[0] == '1') {
    cmd = CMD_SET;
  }
  if (buf->base[0] == '2') {
    cmd = CMD_GET;
  }
  if (buf->base[0] == '3') {
    cmd = CMD_DELETE;
}
return cmd;
}

static int check_msg_valid(const struct uv_buf_t *buf) {
  if (buf->len < 3) {
    return 0;
  }
  return 1;
}

static int get_end_of_key(const uv_buf_t *buf) {
  if (!check_msg_valid(buf)) {
    return 0;
  }
  int idx = 0;
  // 1|key|data
  // 012 - idx
  for (idx = 2; idx < buf->len; idx++) {
    if (buf->base[idx] == '|') {
      break;
    }
  }
  return idx;
}

char *parse_client_msg_key(const uv_buf_t *buf) {
  if (!check_msg_valid(buf)) {
    return NULL;
  }

  int key_size = get_end_of_key(buf) - 2;
  char *key = str_slice(buf->base, 2, get_end_of_key(buf));

  return key;
}

char *parse_client_msg_data(const struct uv_buf_t *buf) {
  if (!check_msg_valid(buf)) {
    return NULL;
  }

  if (buf->len - get_end_of_key(buf) < 1) {
    return NULL;
  }

  return str_slice(buf->base, get_end_of_key(buf) + 1, (int)buf->len);
}

struct stru_task *parse_client_msg(const struct uv_buf_t *buf) {
  struct stru_task *resp = malloc(sizeof(struct stru_task *));
  resp->cmd = parse_client_msg_cmd(buf);
  resp->key = parse_client_msg_key(buf);
  resp->data = parse_client_msg_data(buf);

  return resp;
}

void free_msg(struct stru_task *p_msg) {
  free(p_msg->data);
  free(p_msg->key);
  free(p_msg);
}

#endif
