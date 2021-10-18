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
    char *message, client_message[CLIENT_MSG_SIZE];
    char *q = "q";
    int cmd;
    void* buf;

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    message = "Now type something and i shall repeat what you type \n";
    write(sock, message, strlen(message));

    //Receive a message from client
    while ((read_size = recv(sock, client_message, CLIENT_MSG_SIZE, 0)) > 0)
    {
        cmd = parse_client_msg_cmd(client_message);
        if (cmd == CMD_SET)
        {
            struct stru_task *task = malloc(sizeof(struct stru_task *));
            task->cmd = CMD_SET;
            task->data = parse_client_msg_set(client_message);
            StsQueue.push(g_queue_task_in, task);
        }

        int resp = send(sock, buf, sizeof(buf), MSG_NOSIGNAL);

        if (resp == -1)
        { /* other side gone away */
        //Send the message back to client
            puts("Client disconnected and error");
            write(sock, client_message, strlen(client_message));
        }
        //Send the message back to client
        //write(sock, client_message, strlen(client_message));
    }

    close(*((int *)socket_desc));
    puts("Client disconnected");
    //fflush(stdout);

    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }
}

#endif