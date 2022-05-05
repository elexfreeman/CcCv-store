
#include <ctype.h>
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <malloc.h>
#include <uv.h>

#include <unistd.h> //write

#include "msg_parser.c"

int main(int argc, char **argv)
{
  char* data = "1|keysize|data_size|";
  int size = 19;

  fprintf(stdout, "data= %s, %i \r\n", data, (int)sizeof(data));

  struct uv_buf_t *buf = (uv_buf_t*)malloc(sizeof(uv_buf_t));
  buf->base = (char *)malloc(size);
  strcpy(buf->base, data);
  buf->len = size;

  int end_of_key = get_end_of_key(buf);
  fprintf(stdout, "end_of_key= %d\r\n", end_of_key);

  char* key = parse_client_msg_key(buf); 
  fprintf(stdout, "key= %s\r\n", key);


  char* parce_data = parse_client_msg_data(buf);
  fprintf(stdout, "data= %s \r\n", parce_data);

  free(buf->base);
  free(buf);
  free(key);
  free(parce_data);

  char* msg = (char*)malloc(8);

  snprintf(msg, 9, "3|%s|", "12345");
  fprintf(stdout, "msg= %s \r\n", msg);

  fprintf(stdout, "data len= %d \r\n", (int)strlen(msg));



  return 0;
}
