/* Example: parse a simple configuration file */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

typedef struct {
  const char *host;
  int port;
} configuration;

static int handler(void *user, const char *section, const char *name,
                   const char *value) {
  configuration *pconfig = (configuration *)user;

  if (MATCH_CONF_VARS("server", "host")) {
    pconfig->host = strdup(value);
  } else if (MATCH_CONF_VARS("server", "port")) {
    pconfig->port = atoi(value);
  } else {
    return 0; /* unknown section/name, error */
  }
  return 1;
}

int main(int argc, char *argv[]) {
  configuration config;
  config.host = NULL;
  config.port = 0;

  if (ini_parse("CcCvStore.ini", handler, &config) < 0) {
    printf("Can't load 'CcCvStore.ini'\n");
    return 1;
  }
  printf("Config loaded from 'CcCvStore.ini': host=%s port=%d\n",
         config.host, config.port);

  if (config.host)
    free((void *)config.host);

  return 0;
}
