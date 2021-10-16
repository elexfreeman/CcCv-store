
#include <ctype.h>
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

void exit_nomem(void) {
    fprintf(stderr, "out of memory\n");
    exit(1);
}

void task_manager(void* task_queue)
{
  struct Queue* q = (struct Queue*)task_queue;
  void* ptr_data;
  int* ptr_int;
  while(true) 
  {
//    printf("Thread deQueue \n");
 	ptr_data = deQueue(q);
    
    if(ptr_data != NULL) 
    {
//      printf("Thread ptr_int= \n");
      ptr_int = (int*)ptr_data;
//      printf("ptr_int \n");
      printf("Thread Queue Front : %d \n", *((int*)ptr_int));
    }
    else 
    {
//      printf("Thread task_queue empty \n");

    }
  }
}

int main (int argc, char **argv)
{
  struct Queue* task_queue = createQueue();
  int counter = 0;
  int k = 333;
  void* ptr_data;
  int* ptr_int = malloc( sizeof(int) );


  pthread_t sniffer_thread;
  if (pthread_create(&sniffer_thread, NULL, task_manager, (void *)task_queue) < 0)
  {
      perror("could not create thread");
      return 1;
  }


  printf(">> counter %d \r\n", k );

  *ptr_int = k;

  ptr_data = ptr_int;

  printf(">> counter ptr_data %d \r\n", *((int*)ptr_data) );

  k = 2;

  printf(">> counter ptr_data %d \r\n", *((int*)ptr_data) );

  enQueue(task_queue, ptr_data );

  ptr_int = malloc( sizeof(int) );
  *ptr_int = 4444;
  ptr_data = ptr_int;
  enQueue(task_queue, ptr_data );

  if(ptr_data != NULL)
  {
    printf(">> ptr_data %d \r\n", *((int*)ptr_data) );
  }

  while(true)
  {
    if(counter < 1000)
    {
      ptr_int = malloc( sizeof(int) );
      *ptr_int = counter;
      ptr_data = ptr_int;
      enQueue(task_queue, ptr_data );
    }
    counter++;
  }
  //
  //Now join the thread , so that we dont terminate before the thread
  pthread_join( sniffer_thread , NULL);



//  struct ht* a_map = ht_create();
//  char* word1 = "word1";
//  char* word2 = "word122222";
//
//  char* key1 = "key1";
//  char* key2 = "key122222";
//  char* key3 = "key322222";
//
//  void* getVal;
////sprintf (str, "%s %d %c", "one", 2, '3');
//  if (a_map == NULL) {
//      exit_nomem();
//  }
//  printf(">> Hash table init = DONE \r\n" );
//
//
//  if (ht_set(a_map, key1, word1) == NULL) {
//      exit_nomem();
//  }
//
//  if (ht_set(a_map, key2, word2) == NULL) {
//      exit_nomem();
//  }
//
////  getVal = ht_get(a_map, key1);
////  printf("getVal 1 =  %s", ((char*)getVal));
////
////  getVal = ht_get(a_map, key2);
////  printf("getVal 2 =  %s", ((char*)getVal));
//
//  ht_remove(a_map, key2);
//
//  getVal = ht_get(a_map, key1);
//  printf("getVal 1 =  %s \r\n", ((char*)getVal));
//
//  getVal = ht_get(a_map, key2);
//  printf("getVal 2 =  %s \r\n", ((char*)getVal));
//
//
//  if (ht_set(a_map, key2, word2) == NULL) {
//      exit_nomem();
//  }
//  getVal = ht_get(a_map, key2);
//  printf("getVal 2 =  %s \r\n", ((char*)getVal));
//
//  getVal = ht_get(a_map, key3);
//  printf("getVal 3 =  %s \r\n", ((char*)getVal));

  return 0;
}

