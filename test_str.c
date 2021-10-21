#include <ctype.h>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <malloc.h>

#include <unistd.h> //write

size_t allocated_size(void *ptr)
{
  return ((size_t *)ptr)[-1];
}

int main(int argc, char **argv)
{
  int *buff = malloc(100 * sizeof(int *));
  printf("buff size = %d \r\n", (int)sizeof(buff));

  for (int k = 0; k < 100; k++)
  {
    int c = k + 18;
    buff[k] = c;
  }

  char *msg = malloc(2000);
  char *msg2;

  int idx = 0;
  for (int i = 50; i < 60; i++)
  {
    msg[idx] = (char)buff[i];
    idx++;
  }
  msg[idx] = '\0';

  idx = 0;
  for (int i = 0; i < (int)malloc_usable_size(msg); i++)
  {
    if(msg[i] != '\0')
    {
      idx++;
    } 
  }

  printf("let = %d \r\n", idx);
  msg2 = malloc(idx);
  strcpy(msg2, msg);
  printf("msg1 = %s \r\n", msg);
  printf("msg2 = %s \r\n", msg2);



  printf("size: %d", (int)malloc_usable_size(msg));
  printf("size: %d", (int)malloc_usable_size(msg2));

  return 0;
}
