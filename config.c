#include "config.h" 

static int handler(void *user, const char *section, const char *name, const char *value) {
  struct stru_config *pconfig = (struct stru_config *)user;

  if (MATCH_CONF_VARS("server", "host")) {
    pconfig->address = strdup(value);
  } else if (MATCH_CONF_VARS("server", "port")) {
    pconfig->port = atoi(value);
  } else {
    return 0; /* unknown section/name, error */
  }
  return 1;
}

struct stru_config *get_config() {

  struct stru_config *conf =(struct stru_config*)malloc(sizeof(struct stru_config));
  conf->port = 0;
  conf->address = NULL;
  conf->data_file_name = NULL;
  conf->store_file = NULL;

  if (ini_parse("CcCvStore.ini", handler, &conf) < 0) {
    printf("Can't load 'CcCvStore.ini'\n");
    return NULL;
  }

  return conf;
}
