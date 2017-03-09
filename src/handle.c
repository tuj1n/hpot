#include "handle.h"
#include "util.h"
#include "parse.h"
#include "response.h"
#include <unistd.h>
#include <errno.h>
#include <sys/sendfile.h>

mime_type_t mimes[] = {
  {".html", "text/html"},
  {".xml", "text/xml"},
  {".xhtml", "application/xhtml+xml"},
  {".txt", "text/plain"},
  {".rtf", "application/rtf"},
  {".pdf", "application/pdf"},
  {".word", "application/msword"},
  {".png", "image/png"},
  {".gif", "image/gif"},
  {".jpg", "image/jpeg"},
  {".jpeg", "image/jpeg"},
  {".ico","image/x-icon"},
  {".au", "audio/basic"},
  {".mpeg", "video/mpeg"},
  {".mpg", "video/mpeg"},
  {".avi", "video/x-msvideo"},
  {".gz", "application/x-gzip"},
  {".tar", "application/x-tar"},
  {".css", "text/css"},
  {".js", "application/javascript"},
  {NULL, "text/plain"}
};

int handle_request(request_t *r) {
  buffer_t *rb = &r->recv_buffer;
  int err = buffer_recv(rb, r->fd);

  if (err != AGAIN) {
    return ERROR;
  }

  r->recv_handler = handle_request_line;
  do {
    err = r->recv_handler(r);
  } while (err == OK && !r->request_done);

  return err;
}

int handle_response(request_t *r) {
  char res[] = "HTTP 200 OK\r\nContent-Type:text/html\r\nServer:hpot\r\n\r\n<html><head><title>test</title></head><body><h1>test</h1><body></html>";
  write(r->fd, res, sizeof(res));
  // printf("fd : %d\n", r->file_fd);
  // char buf[1024];
  // int n;
  // while ((n = read(r->file_fd, buf, sizeof(buf))) > 0){
  //   write(r->fd, buf, n);
  // }
  // fflush(NULL);
  close(r->fd);
  return ERROR;
}

int handle_request_line(request_t *r) {
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

int send_response_buffer(request_t *r) {
    buffer_t *sb = &r->send_buffer;
    int err;
    err = buffer_send(sb, r->fd);
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
