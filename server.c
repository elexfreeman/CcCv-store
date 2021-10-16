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
#include "list.c"
#include "queue.c"
#include "ht.c"

void task_manager(void* task_queue)
{
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
    char *message, client_message[2000];
    char* q = "q";

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(sock, message, strlen(message));

    message = "Now type something and i shall repeat what you type \n";
    write(sock, message, strlen(message));

    //Receive a message from client
    // while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    while (bCanFree==0)
    { 
        read_size = recv(sock, client_message, 2000, 0);
        if (strcmp(client_message, q) == 0)
        {
            bCanFree = 1;
            puts("quit");
        }
        else 
        {
          puts(client_message);
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
    struct Queue* q = createQueue();
    struct ht* a_map = ht_create();

    struct stru_list *head = NULL;
    struct stru_list *current = NULL;

    head = insert(head, 1000);

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
    server.sin_port = htons(8888);

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
    while(1)
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
      pthread_join( sniffer_thread , NULL);
    }


    return 0;
}

