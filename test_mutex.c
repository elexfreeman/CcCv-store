#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t id_do;
pthread_t id_locker;

pthread_mutex_t lock;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void *doSomeThing(void *arg) {
  while (1) { 
    printf("do start \n");
    pthread_cond_wait(&c, &lock);
    printf("do finish go sleep 7 \n");
    sleep(7);
  }
  return NULL;
}

void *locker(void *arg) {
  printf("locker start \n");
  sleep(4);
  pthread_cond_signal(&c);
  printf("locker signal 5 \n");
  sleep(5);
  pthread_cond_signal(&c);
  printf("locker signal 5 \n");
  return NULL;
}

int main(void) {
  int i = 0;
  int err;

  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init failed\n");
    return 1;
  }

  err = pthread_create(&id_do, NULL, &doSomeThing, NULL);
  if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));

  err = pthread_create(&id_locker, NULL, &locker, NULL);
  if (err != 0)
    printf("\ncan't create thread :[%s]", strerror(err));

  pthread_join(id_do, NULL);
  pthread_join(id_locker, NULL);

  pthread_mutex_destroy(&lock);

  return 0;
}
