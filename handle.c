#include "handle.h"
#include "util.h"
#include "parse.h"

int handle_request(request_t *r) {
  buffer_t *rb = &r->recv_buffer;
  int err = buffer_recv(rb, r->fd);

  if (err != AGAIN) {
    return ERROR;
  }

  do {
    err = r->recv_handler(r);
  } while (err == OK && !r->done);

  return err;
}

int handle_response(request_t *r) {
  return 0;
}

int handle_request_line(request_t *r) {
  int err = parse_request_line(r);
}

int handle_request_header(request_t *r) {
  return 0;
}

int handle_request_body(request_t *r) {
  return 0;
}
