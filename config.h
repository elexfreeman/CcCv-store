#ifndef _CONFIG_H
#define _CONFIG_H

#define SERVER_DEFAULT_PORT_NUM 8888
#define MAX_KEY_SIZE 128 
#define MAX_VALUE_SIZE 2048 
#define CMD_SIZE 1
#define CLIENT_MSG_SIZE CMD_SIZE + 1 +MAX_KEY_SIZE + 1 + MAX_VALUE_SIZE + 1

#define DATA_FILE_NAME "db.dat"
#define INDEX_FILE_NAME "idx.dat"

struct stru_config
{
  int port;
  char *address;
  char* data_file_name;
  char *store_file;   /* Database filename (UTF-8) */ 
};

#endif
