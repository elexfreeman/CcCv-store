
/*
    simple implementation of key-value store
*/

#include <arpa/inet.h> //inet_addr
#include <pthread.h>   //for threading , link with lpthread
#include <signal.h>
#include <stdio.h>
#include <stdlib.h> //strlen
#include <string.h> //strlen
#include <sys/socket.h>
#include <unistd.h> //write
#include <semaphore.h>

#include "ht.c"
#include "sts_queue.c"

#include "add_lib.c"
#include "config.h"
#include "global.h"
#include "task.c"

#include "thread_connection.c"
#include "thread_disc_sync.c"
#include "thread_task_manager.c"


void sig_handler(int sig) {
  switch (sig) {
  case SIGSEGV:
    fprintf(stderr, "give out a backtrace or something...\n");
    abort();
  default:
    fprintf(stderr, "wasn't expecting that!\n");
    abort();
  }
}

int main_server() {
  pthread_t sniffer_thread_tasks_01;
//  pthread_t sniffer_thread_tasks_02;

  sem_init(&sem_task, 0, 1);

  pthread_t sniffer_thread_disc_sync;


  int socket_desc, client_sock, c, *new_sock;
  struct sockaddr_in server, client;
  char *message = "Heleleoe client";

  g_queue_task_in = StsQueue.create();
  g_store = ht_create();

  struct stru_config *conf = malloc(sizeof(struct stru_config));
  conf->address = 0;
  conf->data_file_name = "database.db";

  store_init(conf);

  if (pthread_create(&sniffer_thread_disc_sync, NULL, thread_disc_sync, NULL) <
      0) {
    perror("could not create thread disc sync");
    return 1;
  }

  // thread for process tasks
  if (pthread_create(&sniffer_thread_tasks_01, NULL, thread_task_manager, NULL) <
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

    if (pthread_create(&sniffer_thread, NULL, thread_connection,
                       (void *)new_sock) < 0) {
      perror("could not create thread");
      return 1;
    }

    // Now join the thread , so that we dont terminate before the thread
    pthread_join(sniffer_thread, NULL);
  }

  pthread_join(sniffer_thread_tasks_01, NULL);
//  pthread_join(sniffer_thread_tasks_02, NULL);
  puts("EXIT");

  sem_destroy(&sem_task);

  free(conf);
}

void main_client() {
  int sockfd, numbytes;
  char buf[CLIENT_MSG_SIZE];
  struct hostent *he;
  struct sockaddr_in their_addr; /* connector's address information */

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  their_addr.sin_family = AF_INET; /* host byte order */
  their_addr.sin_port =
      htons(SERVER_DEFAULT_PORT_NUM); /* short, network byte order */
  their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bzero(&(their_addr.sin_zero), 8); /* zero the rest of the struct */

  if (connect(sockfd, (struct sockaddr *)&their_addr,
              sizeof(struct sockaddr)) == -1) {
    perror("connect");
    exit(1);
  }
  while (1) {
    if (send(sockfd, "Hello, world!\n", 14, 0) == -1) {
      perror("send");
      exit(1);
    }
    printf("After the send function \n");

    if ((numbytes = recv(sockfd, buf, CLIENT_MSG_SIZE, 0)) == -1) {
      perror("recv");
      exit(1);
    }

    buf[numbytes] = '\0';

    printf("Received in pid=%d, text=: %s \n", getpid(), buf);
    sleep(1);
  }

  close(sockfd);
}

int main(int argc, char *argv[]) {
  signal(SIGSEGV, sig_handler);
  int i;
  bool is_server = true;
  printf("The following arguments were passed to main(): ");
  for (i = 1; i < argc; i++) {
    printf("arg = %s", argv[i]);
    if (i == 1) {
      if (strcmp(argv[i], "client")) {
        is_server = false;
        main_client();
      }
    }
  }
  printf("%s ", argv[i]);

  if (is_server) {
    return main_server();
  }

  return 0;
}
