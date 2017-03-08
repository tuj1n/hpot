#include "parse.h"
#include "util.h"
#include "server.h"
#include "string.h"
#include <stdlib.h>
#include <stdbool.h>

int parse_request_line(request_t *r) {
  buffer_t *rb = &r->recv_buffer;
  int err;
  char *p;
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
            r->uri.path.str = p;
            r->state = RL_S_URI;
            break;
        }
        break;
      case RL_S_URI:
        if (c != ' ') {
          break;
        }
        else {
          parse_request_uri(r, r->uri.path.str, p);
          r->state = RL_S_SP_BEFROE_VERSION;
        }
      case RL_S_SP_BEFROE_VERSION:  // todo: parse the version and headers.
        r->done = true;
        return OK;
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

int parse_request_uri(request_t *r, char *start, char *end) {
  int len = end - start;
  char *p;
  char *pathend = end;

  r->uri.path.len = pathend - start;
  for (p = start; p != end; p++)
    if (*p == '?') {
      pathend = p;
      r->uri.path.len = pathend - p;
      break;
    }
  return 0;
}
