
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "ht.c"

void exit_nomem(void) {
    fprintf(stderr, "out of memory\n");
    exit(1);
}

int main (int argc, char **argv)
{
  struct ht* a_map = ht_create();
  char* word1 = "word1";
  char* word2 = "word122222";

  char* key1 = "key1";
  char* key2 = "key122222";
  char* key3 = "key322222";

  void* getVal;
//sprintf (str, "%s %d %c", "one", 2, '3');
  if (a_map == NULL) {
      exit_nomem();
  }
  printf(">> Hash table init = DONE \r\n" );


  if (ht_set(a_map, key1, word1) == NULL) {
      exit_nomem();
  }

  if (ht_set(a_map, key2, word2) == NULL) {
      exit_nomem();
  }

//  getVal = ht_get(a_map, key1);
//  printf("getVal 1 =  %s", ((char*)getVal));
//
//  getVal = ht_get(a_map, key2);
//  printf("getVal 2 =  %s", ((char*)getVal));

  ht_remove(a_map, key2);

  getVal = ht_get(a_map, key1);
  printf("getVal 1 =  %s \r\n", ((char*)getVal));

  getVal = ht_get(a_map, key2);
  printf("getVal 2 =  %s \r\n", ((char*)getVal));


  if (ht_set(a_map, key2, word2) == NULL) {
      exit_nomem();
  }
  getVal = ht_get(a_map, key2);
  printf("getVal 2 =  %s \r\n", ((char*)getVal));

  getVal = ht_get(a_map, key3);
  printf("getVal 3 =  %s \r\n", ((char*)getVal));

  return 0;
}

