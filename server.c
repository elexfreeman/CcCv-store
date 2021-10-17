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
#include "task.c"
#include "add_lib.c"

#define CLIENT_MSG_SIZE 2000
#define SERVER_DEFAULT_PORT_NUM 8888
#define MAX_KEY_SIZE 1000 

StsHeader *task_in_queue;
struct ht *a_map;

void *task_get(struct stru_task *task)
{
    return NULL;
}

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
    ht_set(a_map, task_data->key, task_data->data);

    free(task_data);
}

void *task_manager(void *task_in_queue)
{
    struct stru_task *ptr_data;
    StsHeader *q = (StsHeader *)task_in_queue;

    while (true)
    {
        ptr_data = (struct stru_task *)StsQueue.pop(q);
        if (ptr_data != NULL)
        {
            if (ptr_data->cmd == CMD_SET)
            {
                task_set(ptr_data);
            }
        }
    }
}

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

    cmd = parse_client_msg_cmd(msg);

    if (cmd == 0)
        return;

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
                    if(key_size < MAX_KEY_SIZE -1)
                    {
                        key[key_size] = msg[idx];
                        key[key_size+1] = '\0';
                        key_size++;
                    }
                }
            }

            if (b_find_data)
            {
                if (idx > iterator_data)
                {
                    if(data_size < CLIENT_MSG_SIZE -1)
                    {
                        data[data_size] = msg[idx];
                        data[data_size+1] = '\0';
                        data_size++;
                    }
                }
            }

            idx++;
        }
    }

    printf(">> key: %s \r\n ", key);
    printf(">> data: %s \r\n ", data);

    resp->data = data;
    resp->key = key;

    return resp;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    int bCanFree = 0;
    //Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size;
    char *message, client_message[CLIENT_MSG_SIZE];
    char *q = "q";
    int cmd;

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    message = "Now type something and i shall repeat what you type \n";
    write(sock, message, strlen(message));

    //Receive a message from client
    while ((read_size = recv(sock, client_message, CLIENT_MSG_SIZE, 0)) > 0)
    {
        if (strcmp(client_message, q) == 0)
        {
            bCanFree = 1;
            puts("quit");
        }
        else
        {
            cmd = parse_client_msg_cmd(client_message);
            if (cmd == CMD_SET)
            {
                StsQueue.push(task_in_queue, parse_client_msg_set(client_message));
            }

            //Send the message back to client
            write(sock, client_message, strlen(client_message));
        }
    }

    close(*((int *)socket_desc));
    puts("Client disconnected");
    fflush(stdout);

    //    if (read_size == 0)
    //    {
    //        puts("Client disconnected");
    //        fflush(stdout);
    //    }
    //    else if (read_size == -1)
    //    {
    //        perror("recv failed");
    //    }
}

int main(int argc, char *argv[])
{

    task_in_queue = StsQueue.create();
    a_map = ht_create();

    int socket_desc, client_sock, c, *new_sock;
    struct sockaddr_in server, client;

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

        if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }

        //Now join the thread , so that we dont terminate before the thread
        pthread_join(sniffer_thread, NULL);
    }

    return 0;
}
