#ifndef _THREAD_CONNECTION_C
#define _THREAD_CONNECTION_C

#include <arpa/inet.h> //inet_addr
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h> //strlen
#include <string.h> //strlen
#include <sys/socket.h>
#include <unistd.h> //write

#include "config.h"
#include "global.h"
#include "msg_parser.c"
#include "task.c"

void tasks_branch(int sock, void *buff) {

  int cmd;
  // got data from the client.
  cmd = parse_client_msg_cmd(buff);
  if (cmd == CMD_SET) {
    struct stru_task *task = malloc(sizeof(struct stru_task *));
    task->cmd = CMD_SET;
    task->data = parse_client_msg_set(sock, buff);
    StsQueue.push(g_queue_task_in, task);
  } else if (cmd == CMD_GET) {
    struct stru_task *task = malloc(sizeof(struct stru_task *));
    task->cmd = CMD_GET;
    task->data = parse_client_msg_get(sock, buff);
    StsQueue.push(g_queue_task_in, task);
  } else if (cmd == CMD_DELETE) {
    struct stru_task *task = malloc(sizeof(struct stru_task *));
    task->cmd = CMD_DELETE;
    task->data = parse_client_msg_delete(sock, buff);
    StsQueue.push(g_queue_task_in, task);
  }
}

/*
 * This will handle connection for each client
 * */
void *thread_connection(void *socket_desc) {
  // Get the socket descriptor
  int sock = *(int *)socket_desc;
  int read_size;
  char *message;
  int cmd;

  fd_set read_sd;
  FD_ZERO(&read_sd);
  FD_SET(sock, &read_sd);

  while (true) {
    fd_set rsd = read_sd;
    int sel = select(sock + 1, &rsd, 0, 0, 0);

    if (sel > 0) {

      void *buff = malloc(CLIENT_MSG_SIZE);
      // client has performed some activity (sent data or disconnected?)
      read_size = recv(sock, buff, CLIENT_MSG_SIZE, 0);

      if (read_size > 0) {
        tasks_branch(sock, buff);

        printf("> buff= %s \r\n" , (char*)buff);

        int *sem_val = malloc(sizeof(int));
        sem_getvalue(&sem_task, sem_val);
        if(*sem_val <1) { 
        sem_post(&sem_task);
        }

        free(sem_val);

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
}

#endif
