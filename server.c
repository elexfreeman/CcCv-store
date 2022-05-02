
/*
    simple implementation of key-value store
*/

#include <arpa/inet.h> //inet_addr
#include <pthread.h>   //for threading , link with lpthread
#include <semaphore.h>
#include <signal.h>
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

void set_termitate() {
  fprintf(stderr, "\n Start exit...\n");
  is_app_terminate = 1;
  sleep(2);
  abort();
}

void sig_handler(int sig) {
  printf("signal: %d \n", sig);
  switch (sig) {
  case SIGSEGV:
    set_termitate();
  case SIGTERM:
    set_termitate();
  default:
    set_termitate();
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
  store_init(conf);

  if (pthread_create(&sniffer_thread_disc_sync, NULL, thread_disc_sync, NULL) <
      0) {
    perror("could not create thread disc sync");
    return 1;
  }

  // thread for process tasks
  if (pthread_create(&sniffer_thread_tasks_01, NULL, thread_task_manager,
                     NULL) < 0) {
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
  c = sizeof(struct sockaddr_in);
  fd_set read_fd_set;

  puts("Waiting for incoming connections...");
  while (IS_APP_TRM) {

    client_sock =
        accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c);

    if (client_sock >= 0) {
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
  }

  pthread_join(sniffer_thread_tasks_01, NULL);
  //  pthread_join(sniffer_thread_tasks_02, NULL);
  puts("EXIT");

  sem_destroy(&sem_task);

  free(conf);

  return 0;
}

int main(int argc, char *argv[]) {
  if (signal(SIGINT, sig_handler) == SIG_ERR)
    printf("\ncan't catch SIGINT\n");

  int i;
  bool is_server = true;
  printf("The following arguments were passed to main(): ");
  for (i = 1; i < argc; i++) {
    printf("arg = %s", argv[i]);
    if (i == 1) {
      if (strcmp(argv[i], "client")) {
        is_server = false;
      }
    }
  }
  printf("%s ", argv[i]);

  if (is_server) {
    return main_server();
  }

  return 0;
}
