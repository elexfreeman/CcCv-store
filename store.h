
#ifndef _STORE_H
#define _STORE_H

#include "config.h"

int store_is_ok();

int store_init(struct stru_config *config);

void *store_get(char *key);

int store_remove(char *key);

int store_close();

int store_set(char *key, char *val);
#endif
