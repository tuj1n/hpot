#include "parse.h"
#include "util.h"
#include "server.h"

int parse_request_line(request_t *r) {
  buffer_t *rb = &r->recv_buffer;
  int err;
  char *p;
  log("setp2");
  for (p = rb->start; p != rb->end; p++) {
    char c = *p;
    switch (r->state) {
      case RL_S_BEGIN:
        if (c >= 'A' && c <= 'Z') {
          r->request_line = p;
          r->state = RL_S_METHOD;
          break;
        }
        else {
          return ERR_REQUEST;
        }
      case RL_S_METHOD:
        if (c >= 'A' && c <= 'Z')
          break;
        else if (c == ' ') {
          if ((r->method = parse_request_method(r->request_line, p)) == ERR_METHOD)
            return ERR_METHOD;
          r->state = RL_S_SP_BEFORE_URI;
          break;
        }
        else
          return ERR_REQUEST;

      case RL_S_SP_BEFORE_URI:
        switch(c) {
          case ' ':
            break;
          default:
            printf("%c\n", c);
        }
        break;
    }
  }
}

int parse_request_method(char *start, char * end) {
  int len = end - start;
  switch(len) {
    case 3:
      if (issame(start, "GET", len))
        return GET;
      if (issame(start, "PUT", len))
        return PUT;
      else
        return ERR_METHOD;
    case 4:
      if (issame(start, "POST", len))
        return POST;
      if (issame(start, "HEAD", len))
        return HEAD;
      else
        return ERR_METHOD;
    case 5:
      if (issame(start, "TRACE", len))
        return TRACE;
      return ERR_METHOD;
    case 6:
      if (issame(start, "DELETE", len))
        return DELETE;
      return ERR_METHOD;
    case 7:
      if (issame(start, "OPTIONS", len))
        return OPTIONS;
      return ERR_METHOD;
    default:
      return ERR_METHOD;
  }
  return ERR_METHOD;
}

int parse_request_header(request_t *r) {
  return 0;
}

int parse_request_body(request_t *r) {
  return 0;
}
