#include "config.h"

static int handler(void *data, const char *section, const char *name,
                   const char *value) {
  struct stru_config *pconfig = (struct stru_config *)data;

  if (MATCH_CONF_VARS("server", "host")) {
    pconfig->address = strdup(value);
  }
  if (MATCH_CONF_VARS("server", "port")) {
    pconfig->port = atoi(value);
  }
  if (MATCH_CONF_VARS("store", "db_file")) {
    pconfig->db_file = strdup(value);
  }
  return 1;
}

void init_config() {
  config = (struct stru_config *)malloc(sizeof(struct stru_config));
  config->port = 0;
  config->address = NULL;
  config->db_file = NULL;

  if (ini_parse("CcCvStore.ini", handler, &config) < 0) {
    printf("Can't load 'CcCvStore.ini'\n");
    return;
  }

  is_config_init = 1;
}

struct stru_config *get_config() {

  if (is_config_init == 0) {
    return NULL;
  }

  return config;
}

