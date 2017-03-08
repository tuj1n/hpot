#include "handle.h"
#include "util.h"
#include "parse.h"
#include "response.h"
#include <sys/sendfile.h>

int handle_request(request_t *r) {
  buffer_t *rb = &r->recv_buffer;
  int err = buffer_recv(rb, r->fd);

  if (err != AGAIN) {
    return ERROR;
  }

  r->recv_handler = handle_request_line;
  do {
    err = r->recv_handler(r);
  } while (err == OK && !r->done);

  return err;
}

int handle_response(request_t *r) {
  //
}

int handle_request_line(request_t *r) {
  int err = parse_request_line(r);
  request_disable_in(r);
  request_enable_out(r);
  return err; // temp
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

int send_response_buffer(request_t *r) {
    buffer_t *sb = r->send_buffer；
    int err = buffer_send(sb, r->fd);
    if (err == OK) {
      buffer_init(sb);
      if (r->file_fd != -1) {
        r->send_handler = send_response_file;
        return OK;
      }
      request_disable_out(r);
    }
    else if (err == ERROR) {
      return ERROR;
    }
    return AGAIN;
}

int send_response_file(request_t* r) {
    int fd = r->fd;
    while (1) {
        int len = sendfile(fd, r->file_fd, NULL, r->file_len);
        if (len == 0) {
            r->response_done = true;
            close(r->file_fd);
            r->file_fd = -1;
            return OK;
        } else if (len < 0) {
            if (errno == EAGAIN)
                return AGAIN;
            return ERROR;
        }
    }
    return OK;
}
