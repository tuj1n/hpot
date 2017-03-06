#include "handle.h"
#include "util.h"

int handle_request(request_t *r) {
  buffer_t *rb = &r->recv_buffer;
  int err = buffer_recv(rb, r->fd);

  if (err != AGAIN) {
    return ERROR;
  }

  do {
    ;
  } while(1);
}

int handle_response(request_t *r) {
  return 0;
}

int handle_request_line(request_t *r) {
  return 0;
}
