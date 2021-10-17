#include <stdio.h>
#include <stdlib.h> //strlen
#include "add_lib.c"


int main(int argc, char **argv)
{
  char * str = "My str";

  puts(str);
  str = lib_append(str, "="[0]);
  puts(str);

  return 0;
}
