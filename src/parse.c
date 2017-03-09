#include "parse.h"
#include "util.h"
#include "server.h"
#include "string.h"
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>

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
        if (c != ' ') // 跳到URI最后
          break;
        else {
          parse_request_uri(r, r->uri.path.str, p);
          r->state = RL_S_SP_BEFROE_VERSION;
          break;
        }
      case RL_S_SP_BEFROE_VERSION:  // todo: parse the version and headers.
        r->request_done = true;
        return OK;
    }
  }
  return AGAIN;
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

  char path[r->uri.path.len];
  int path_len = strlen(ROOT) + r->uri.path.len;
  char abs_path[path_len];
  strncat(strcpy(abs_path, ROOT), r->uri.path.str, r->uri.path.len);

  struct stat sbuf;
  printf("the abs_path: %s\n", abs_path);

  if (stat(abs_path, &sbuf) < 0) {
    return ERROR;
  }

  // 没有读取权限
  if (!(S_IRUSR & sbuf.st_mode)) {
  }

  if (S_ISDIR(sbuf.st_mode)) {
    char fullname[path_len + 11];
    strcat(strcpy(fullname, abs_path), "/index.html");
    r->mime.type = ".html";
    r->mime.value = "text/html";
    printf("full: %s\n", fullname);
  }

  char *fullname = abs_path;

  r->file_fd = open(fullname, O_RDONLY);

  char buf[1024];
  int n;
  while ((n = read(r->file_fd, buf, sizeof(buf))) > 0)
    write(1, buf, n);

  stat(fullname, &sbuf);
  r->file_len = sbuf.st_size;

  request_disable_in(r);
  request_enable_out(r);

  return 0;
}
