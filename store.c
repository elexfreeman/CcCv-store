
#ifndef _STORE_C
#define _STORE_C

#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h> //strlen
#include <string.h> //strlen

#include "config.h"

static int is_ok = 1;
static sqlite3 *db = NULL;

int store_is_ok() { return is_ok; }

int store_init(struct stru_config *config) {

  char *err_msg = 0;
  sqlite3_stmt *statement;

  int rc = sqlite3_open(":memory:", &db);
  // int rc = sqlite3_open("file::memory:?cache=shared", &db);
  // int rc = sqlite3_open("file::memory:", &db);
  //int rc = sqlite3_open(config->data_file_name, &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    db = NULL;
    is_ok = 0;
  }

  if (is_ok) {
    char *sql = "CREATE TABLE IF NOT EXISTS 'data_tbl1' ( idx TEXT NOT NULL UNIQUE, data TEXT, PRIMARY KEY('idx'))";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) { 
      fprintf(stderr, "ERROR SQL: %s\n", sqlite3_errmsg(db));
      sqlite3_free(err_msg);
      sqlite3_close(db); 
      return 0;
    }

//    rc = sqlite3_prepare_v2(db, sql, strlen(sql), &statement, NULL);
//    if (rc == SQLITE_OK) {
//      rc = sqlite3_step(statement);
//      sqlite3_finalize(statement);
//    } else {
//      fprintf(stderr, "ERROR SQL: %s\n", sqlite3_errmsg(db));
//      is_ok = 0;
//      sqlite3_close(db);
//      return 0;
//    }
  }

  is_ok = 1;

  return is_ok;
}

int store_set(char *key, char *val) {
  int rc = 0;
  int ret = 0;

  sqlite3_stmt *p_stmt;
  const char *p_oz;

  if (!is_ok) {
    fprintf(stderr, "ERROR: store is not init\n");
    return 0;
  }

  const char *istmt = "INSERT INTO data_tbl1 (idx, data) Values(?,?);";

  ret = sqlite3_prepare_v2(db, istmt, strlen(istmt), &p_stmt, &p_oz);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "ERROR SQL: %s\n", sqlite3_errmsg(db));
    return 0;
  }

  sqlite3_bind_text(p_stmt, 1, key, strlen(key), SQLITE_TRANSIENT);
  sqlite3_bind_text(p_stmt, 2, val, strlen(val), SQLITE_TRANSIENT);
  ret = sqlite3_step(p_stmt);
  sqlite3_finalize(p_stmt);

  if (ret != SQLITE_DONE) {
    fprintf(stderr, "ERROR SQL: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
  }

  return 1;
}

char *store_get(char *key) {
  if (!is_ok) {
    return NULL;
  }
  char *data = NULL;
  return data;
}

int store_remove(char *key) {
  if (!is_ok) {
    return 0;
  }
  return 1;
}

int store_close() {
  if (db != NULL) {
    sqlite3_close(db);
  }
  is_ok = 0;
  return 1;
}

#endif
