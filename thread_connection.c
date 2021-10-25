#ifndef _THREAD_CONNECTION_C
#define _THREAD_CONNECTION_C

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include "config.h"
#include "global.h"
#include "task.c"
#include "msg_parser.c"

/*
 * This will handle connection for each client
 * */
void *thread_connection(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size;
    char *message;
    int cmd;

    fd_set read_sd;
    FD_ZERO(&read_sd);
    FD_SET(sock, &read_sd);

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    while (true)
    {
        fd_set rsd = read_sd;

        int sel = select(sock + 1, &rsd, 0, 0, 0);

        if (sel > 0)
        {
            void* buff = malloc(CLIENT_MSG_SIZE);
            // client has performed some activity (sent data or disconnected?)
            read_size = recv(sock, buff, CLIENT_MSG_SIZE, 0);

            if (read_size > 0)
            {
                 puts((char*)buff);
                // got data from the client.
                cmd = parse_client_msg_cmd(buff);
                if (cmd == CMD_SET)
                {
                    struct stru_task *task = malloc(sizeof(struct stru_task *));
                    task->cmd = CMD_SET;
                    task->data = parse_client_msg_set(buff);
                    StsQueue.push(g_queue_task_in, task);
                }
                else if(cmd == CMD_GET)
                {
                    struct stru_task *task = malloc(sizeof(struct stru_task *));
                    task->cmd = CMD_GET;
                    task->data = parse_client_msg_get(sock, buff);
                    StsQueue.push(g_queue_task_in, task);
                }
                else if(cmd == CMD_DELETE)
                {
                    struct stru_task *task = malloc(sizeof(struct stru_task *));
                    task->cmd = CMD_DELETE;
                    task->data = parse_client_msg_delete(sock, buff);
                    StsQueue.push(g_queue_task_in, task);
                }
                free(buff);
            }
            else if (read_size == 0)
            {
                free(buff);
                // client disconnected.
                break;
            }
            else
            {
                // error receiving data from client. You may want to break from
                // while-loop here as well.
                free(buff);
                break;
            }
        }
        else if (sel < 0)
        {
            // grave error occurred.
            break;
        }
    }
    close(*((int *)socket_desc));
    puts("Client disconnected");
}

#endif