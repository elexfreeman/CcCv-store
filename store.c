#ifndef _STORE_C
#define _STORE_C

#include "sqlite3.h"
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
  int rc = 0;

  if (config->db_file == NULL) {
    int rc = sqlite3_open(":memory:", &db);

  } else {
    int rc = sqlite3_open(config->db_file, &db);
  }
  // int rc = sqlite3_open("file::memory:?cache=shared", &db);
  // int rc = sqlite3_open("file::memory:", &db);
  // int rc = sqlite3_open(config->data_file_name, &db);

  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    db = NULL;
    is_ok = 0;
  }

  if (is_ok) {
    char *sql = "CREATE TABLE IF NOT EXISTS 'data_tbl1' ( idx TEXT NOT NULL "
                "UNIQUE, data BLOB, PRIMARY KEY('idx'))";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK) {
      fprintf(stderr, "ERROR SQL: %s\n", sqlite3_errmsg(db));
      sqlite3_free(err_msg);
      sqlite3_close(db);
      return 0;
    }
  }

  is_ok = 1;

  return is_ok;
}

void *store_get(char *key) {
  if (!is_ok) {
    return 0;
  }

  void *val = NULL;

  char *err_msg = 0;
  sqlite3_stmt *p_stmt;

  char *sql = "SELECT data FROM data_tbl1 WHERE idx = ?";

  int rc = sqlite3_prepare_v2(db, sql, -1, &p_stmt, 0);

  if (rc == SQLITE_OK) {
    sqlite3_bind_text(p_stmt, 1, key, strlen(key), SQLITE_TRANSIENT);
  } else {
    fprintf(stderr, "ERROR SQL get: %s\n", sqlite3_errmsg(db));
    return NULL;
  }

  int step = sqlite3_step(p_stmt);

  if (step == SQLITE_ROW) {
    const void *blob = sqlite3_column_blob(p_stmt, 0);
    size_t blob_bytes = sqlite3_column_bytes(p_stmt, 0);
    val = malloc(blob_bytes);
    memcpy(val, blob, blob_bytes);
  }

  sqlite3_finalize(p_stmt);
  return val;
}

int store_remove(char *key) {
  if (!is_ok) {
    return 0;
  }
  int rc = 0;
  int ret = 0;

  sqlite3_stmt *p_stmt;
  const char *p_oz;

  if (!is_ok) {
    fprintf(stderr, "ERROR: store is not init\n");
    return 0;
  }

  const char *istmt = "DELETE FROM data_tbl1 WHERE idx=?;";

  ret = sqlite3_prepare_v2(db, istmt, strlen(istmt), &p_stmt, &p_oz);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "ERROR SQL remove: %s\n", sqlite3_errmsg(db));
    return 0;
  }

  sqlite3_bind_text(p_stmt, 1, key, strlen(key), SQLITE_TRANSIENT);
  ret = sqlite3_step(p_stmt);
  sqlite3_finalize(p_stmt);

  if (ret != SQLITE_DONE) {
    fprintf(stderr, "ERROR SQL remove: %s\n", sqlite3_errmsg(db));
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

int store_set(char *key, char *val) {
  int rc = 0;
  int ret = 0;

  sqlite3_stmt *p_stmt;
  const char *p_oz;

  if (!is_ok) {
    fprintf(stderr, "ERROR: store is not init\n");
    return 0;
  }

  store_remove(key);

  const char *istmt = "INSERT INTO data_tbl1 (idx, data) Values(?,?);";

  ret = sqlite3_prepare_v2(db, istmt, strlen(istmt), &p_stmt, &p_oz);

  if (ret != SQLITE_OK) {
    fprintf(stderr, "ERROR SQL set: %s\n", sqlite3_errmsg(db));
    return 0;
  }

  sqlite3_bind_text(p_stmt, 1, key, strlen(key), SQLITE_TRANSIENT);
  sqlite3_bind_blob(p_stmt, 2, val, strlen(val), SQLITE_STATIC);
  //  sqlite3_bind_text(p_stmt, 2, val, strlen(val), SQLITE_TRANSIENT);
  ret = sqlite3_step(p_stmt);
  sqlite3_finalize(p_stmt);

  if (ret != SQLITE_DONE) {
    fprintf(stderr, "ERROR SQL set: %s\n", sqlite3_errmsg(db));
    return 0;
  }

  return 1;
}
#endif
