
/*
    C socket server example, handles multiple clients using threads

*/

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h>   //for threading , link with lpthread

#include "sts_queue.c"
#include "ht.c"

#include "config.h"
#include "global.h"
#include "task.c"
#include "add_lib.c"

#include "thread_connection.c"
#include "thread_task_manager.c"


void *task_get(struct stru_task *task)
{
    return NULL;
}



int main(int argc, char *argv[])
{
    pthread_t sniffer_thread_tasks;
    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

    g_queue_task_in = StsQueue.create();
    g_store = ht_create();

    if (pthread_create(&sniffer_thread_tasks, NULL, thread_task_manager, NULL) < 0)
    {
        perror("could not create thread");
        return 1;
    }

    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(SERVER_DEFAULT_PORT_NUM);

    //Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc, 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    while (1)
    {
        client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

        puts("Connection accepted");

        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if (pthread_create(&sniffer_thread, NULL, thread_connection, (void *)new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        //pthread_join(sniffer_thread, NULL);
    }

    pthread_join(sniffer_thread_tasks, NULL);
    puts("EXIT");

    return 0;
}
