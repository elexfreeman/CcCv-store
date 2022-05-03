#include "config.h" 

static int handler(void *user, const char *section, const char *name, const char *value) {
  struct stru_config *pconfig = (struct stru_config *)user;

  if (MATCH_CONF_VARS("server", "host")) {
    pconfig->address = strdup(value);
  } 
  if (MATCH_CONF_VARS("server", "port")) {
    pconfig->port = atoi(value);
  } 
  if (MATCH_CONF_VARS("store", "db_file")) {
    pconfig->db_file = strdup(value);
  } 
  if (MATCH_CONF_VARS("server", "max_connection")) {
    pconfig->port = atoi(value);
  } 
  return 1;
}

struct stru_config *get_config() {

  struct stru_config *conf =(struct stru_config*)malloc(sizeof(struct stru_config));
  conf->port = 0;
  conf->address = NULL;
  conf->db_file = NULL;

  if (ini_parse("CcCvStore.ini", handler, &conf) < 0) {
    printf("Can't load 'CcCvStore.ini'\n");
    return NULL;
  }

  return conf;
}
