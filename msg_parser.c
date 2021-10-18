#ifndef _MSG_PARSER_C
#define _MSG_PARSER_C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "global.h"


#include "task.c"

int parse_client_msg_cmd(const char *msg)
{
    int cmd = 0;
    if (msg[0] == '1')
    {
        cmd = CMD_SET;
    }

    return cmd;
}

// cmd|key|data
// 1|key|data
struct stru_task_set *parse_client_msg_set(const char *msg)
{
    const char *ptr = msg;
    int cmd = 0;

    cmd = parse_client_msg_cmd(msg);
    if (cmd == 0)
        return NULL;

    struct stru_task_set *resp = malloc(sizeof(struct stru_task_set *));
    char *key = malloc(MAX_KEY_SIZE);
    char *data = malloc(CLIENT_MSG_SIZE);

    int key_size = 0;
    int data_size = 0;

    char *tmp;

    int idx = 0;

    bool b_find_cmd = true;
    bool b_find_key = false;
    bool b_find_data = false;

    int iterator_key = 2;
    int iterator_data = 0;

    b_find_cmd = false;

    if (cmd == CMD_SET)
    {
        for (char c = *ptr; c; c = *++ptr)
        {
            if (idx == iterator_key)
            {
                b_find_key = true;
            }

            if (b_find_key)
            {
                if (msg[idx] == '|')
                {
                    b_find_key = false;
                    b_find_data = true;
                    iterator_data = idx;
                }
                else
                {
                    if (key_size < MAX_KEY_SIZE - 1)
                    {
                        key[key_size] = msg[idx];
                        key[key_size + 1] = '\0';
                        key_size++;
                    }
                }
            }

            if (b_find_data)
            {
                if (idx > iterator_data)
                {
                    if (data_size < CLIENT_MSG_SIZE - 1)
                    {
                        data[data_size] = msg[idx];
                        data[data_size + 1] = '\0';
                        data_size++;
                    }
                }
            }

            idx++;
        }
    }

//    printf(">> key: %s \r\n ", key);
//    printf(">> data: %s \r\n ", data);

    resp->data = data;
    resp->key = key;

    return resp;
}

#endif