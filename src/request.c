#include "request.h"
#include "server.h"

int request_init(request_t *r, int fd, int epfd) {
  r->fd = fd;
  r->epfd = epfd;
  r->active_time = time(NULL);

  buffer_init(&r->recv_buffer);
  buffer_init(&r->send_buffer);

  r->state = RL_S_BEGIN;
}

int update_active(request_t *r) {
  return 0;
}

int close_request(request_t *r) {
  return 0;
}
