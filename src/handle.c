#include "handle.h"
#include "util.h"
#include "parse.h"
#include "response.h"

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
  log("step1");
  int err = parse_request_line(r);
  if (err = AGAIN)
    return err;
  else if (err != OK) {
    r->status = 400;
    return build_response_err(r);
  }

  if (r->version_major != 1 || r->version_minor >= 2) {
    r->status = 505;
    return build_response_err(r);
  }

  if (r->version_minor == 1)
    r->keep_alive = true;

  r->recv_handler = handle_request_header;
  return handle_request_uri(r);
}

int handle_request_header(request_t *r) {
  return 0;
}

int handle_request_body(request_t *r) {
  return 0;
}

int handle_request_uri(request_t *r) {
  return 0;
}
