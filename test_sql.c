#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h> //strlen

#include "config.h"
#include "store.c"

int main(void) {
  struct stru_config *conf = malloc(sizeof(struct stru_config));
  conf->address = 0;
  conf->data_file_name = "database.db";

  store_init(conf); 
  printf("Store is ok %d \n", store_is_ok());

  store_set("key122","123456789");

  const void* data =  store_get("key122");
  printf("Store data  %s \n",(char*)data);
  store_remove("key122");

  store_close();
  printf("Store is ok close %d \n", store_is_ok());

  return 0;
}

