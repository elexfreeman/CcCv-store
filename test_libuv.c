
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>

#include "add_lib.c"
#include "config.h"
#include "global.h"
#include "task.c"

#include "thread_connection.c"
#include "thread_disc_sync.c"
#include "thread_task_manager.c"

#define DEFAULT_BACKLOG 128

uv_loop_t *loop;
struct sockaddr_in addr;

typedef struct {
  uv_write_t req;
  uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req) {
  write_req_t *wr = (write_req_t *)req;
  free(wr->buf.base);
  free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = (char *)malloc(suggested_size);
  buf->len = suggested_size;
}

void on_close(uv_handle_t *handle) {
  fprintf(stdout, "on_close \r\n");
  free(handle);
}

void echo_write(uv_write_t *req, int status) {
  if (status) {
    fprintf(stderr, "Write error %s\n", uv_strerror(status));
  }
  free_write_req(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
  if (nread > 0) {
    fprintf(stdout, ">> %s\n", (char *)buf->base);
    write_req_t *req = (write_req_t *)malloc(sizeof(write_req_t));
    req->buf = uv_buf_init(buf->base, nread);
    uv_write((uv_write_t *)req, client, &req->buf, 1, echo_write);
    return;
  }
  if (nread < 0) {
    if (nread != UV_EOF) {
      fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    }
    fprintf(stdout, "end read \r\n");
    uv_close((uv_handle_t *)client, on_close);
  }

  free(buf->base);
}

void on_new_connection(uv_stream_t *server, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    // error!
    return;
  }

  fprintf(stdout, "New client connected \r\n");
  uv_tcp_t *client = (uv_tcp_t *)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, client);
  if (uv_accept(server, (uv_stream_t *)client) == 0) {
    uv_read_start((uv_stream_t *)client, alloc_buffer, echo_read);
  } else {
    fprintf(stdout, "client exit");
    uv_close((uv_handle_t *)client, on_close);
  }
}

void app_finish() {
  struct stru_config *config = get_config();
  free(config);
  fprintf(stdout, "App exit...\n");
  abort();
}

void sig_handler(int sig) {
  switch (sig) {
  case SIGSEGV:
    app_finish();
  default:
    app_finish();
  }
}

int main() {
  signal(SIGSEGV, sig_handler);
  if (init_config() == 0) {
    fprintf(stdout, "ERROR: bad config file r\n");
    return 1;
  }

  struct stru_config *config = get_config();
  if (config == NULL) {
    fprintf(stdout, "Empty config r\n");
    return 1;
  }

  fprintf(stdout, "server start at 0.0.0.0:%d r\n", config->port);
  loop = uv_default_loop();

  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  uv_ip4_addr("0.0.0.0", config->port, &addr);

  uv_tcp_bind(&server, (const struct sockaddr *)&addr, 0);
  int r = uv_listen((uv_stream_t *)&server, DEFAULT_BACKLOG, on_new_connection);
  if (r) {
    fprintf(stderr, "Listen error %s\n", uv_strerror(r));
    return 1;
  }
  return uv_run(loop, UV_RUN_DEFAULT);
}
