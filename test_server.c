

/*
    simple implementation of key-value store
*/

#include <arpa/inet.h> //inet_addr
#include <pthread.h>   //for threading , link with lpthread
#include <stdio.h>
#include <stdlib.h> //strlen
#include <string.h> //strlen
#include <sys/socket.h>
#include <unistd.h> //write

#include "ht.c"
#include "sts_queue.c"

#include "add_lib.c"
#include "config.h"
#include "global.h"
#include "task.c"

#include "thread_connection.c"
#include "thread_disc_sync.c"
#include "thread_task_manager.c"

#include "store.c"

struct stru_message {
  char cmd;
  char key[MAX_KEY_SIZE];
  char data[MAX_VALUE_SIZE];
};


void *thread_connection_test(void *socket_desc) {
  // Get the socket descriptor
  int sock = *(int *)socket_desc;
  int read_size;
  char *message;
  int cmd;

  fd_set read_sd;
  FD_ZERO(&read_sd);
  FD_SET(sock, &read_sd);

  // Send some messages to the client
  message = "Greetings! I am your connection handler\n";
  write(sock, message, strlen(message));

  while (true) {
    fd_set rsd = read_sd;
    int sel = select(sock + 1, &rsd, 0, 0, 0);

    if (sel > 0) {
      void *buff = malloc(CLIENT_MSG_SIZE);
      // client has performed some activity (sent data or disconnected?)
      read_size = recv(sock, buff, CLIENT_MSG_SIZE, 0);

      printf("msg: %s \n", (char *)buff);
      printf("read_size: %d \n", read_size);

      if (read_size > 0) {
        // got data from the client.
        struct stru_message *data = malloc(CLIENT_MSG_SIZE);
        memcpy(data, buff, read_size);
        printf("cmd: %d", data->cmd);
        printf("data: %s", data->data);

        free(buff);
      } else if (read_size == 0) {
        free(buff);
        // client disconnected.
        break;
      } else {
        // error receiving data from client. You may want to break from
        // while-loop here as well.
        free(buff);
        break;
      }
    } else if (sel < 0) {
      // grave error occurred.
      break;
    }
  }
  close(*((int *)socket_desc));
  puts("Client disconnected");

  return NULL;
}

int main_server() {
  pthread_t sniffer_thread_tasks;
  pthread_t sniffer_thread_disc_sync;
  int socket_desc, client_sock, c, *new_sock;
  struct sockaddr_in server, client;
  char *message = "Heleleoe client";

  g_queue_task_in = StsQueue.create();
  g_store = ht_create();

  // thread for process tasks
  if (pthread_create(&sniffer_thread_disc_sync, NULL, thread_disc_sync, NULL) <
      0) {
    perror("could not create thread disc sync");
    return 1;
  }

  // thread for disc sync
  if (pthread_create(&sniffer_thread_tasks, NULL, thread_task_manager, NULL) <
      0) {
    perror("could not create thread");
    return 1;
  }

  // Create socket
  socket_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_desc == -1) {
    printf("Could not create socket");
  }
  puts("Socket created");

  // Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(SERVER_DEFAULT_PORT_NUM);

  // Bind
  if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    // print the error message
    perror("bind failed. Error");
    return 1;
  }
  puts("bind done");

  // Listen
  listen(socket_desc, 3);

  // Accept and incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);

  while (1) {
    client_sock =
        accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

    puts("Connection accepted");

    pthread_t sniffer_thread;
    new_sock = malloc(1);
    *new_sock = client_sock;

    if (pthread_create(&sniffer_thread, NULL, thread_connection_test,
                       (void *)new_sock) < 0) {
      perror("could not create thread");
      return 1;
    }

    // Now join the thread , so that we dont terminate before the thread
    pthread_join(sniffer_thread, NULL);
  }

  pthread_join(sniffer_thread_tasks, NULL);
  puts("EXIT");
}

int main(int argc, char *argv[]) {
  main_server();
  return 0;
}
