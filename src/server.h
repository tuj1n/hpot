#ifndef _HPOT_SERVER_H_
#define _HPOT_SERVER_H_

#include <stdint.h>
#include <sys/epoll.h>
#include "util.h"

#define PORT 8888
#define MAX_LISTEN 1024
#define MAX_EVENTS 65535
#define ROOT "/home/terry/github/hpot/"

typedef struct mime_type {
  char *type;
  char *value;
} mime_type_t;


typedef struct uri {
  string_t scheme;  // e.g. http
  string_t host;
  string_t post;
  string_t user;
  string_t pass;
  string_t path;
  // querys key=value <map>
  string_t fragment;  // after the hashmark
} uri_t;

typedef enum {
  GET,
  HEAD,
  POST,
  PUT,
  TRACE,
  OPTIONS,
  DELETE,
} method_t;

typedef enum {
  RL_S_BEGIN,
  RL_S_METHOD,
  RL_S_SP_BEFORE_URI,
  RL_S_URI,
  RL_S_SP_BEFROE_VERSION,
  RL_S_HTTP_H,
  RL_S_HTTP_HT,
  RL_S_HTTP_HTT,
  RL_S_HTTP_HTTP,
  RL_S_HTTP_VERSION_SLASH,
  RL_S_HTTP_VERSION_MAJOR,
  RL_S_HTTP_VERSION_DOT,
  RL_S_HTTP_VERSION_MINOR,
  RL_S_SP_AFTER_VERSION,
  RL_S_CR,
  RL_S_LF,

} request_state;

int startup(uint16_t port);
int setnonblocking(int fd);
int accept_connection(int listen_fd, int epoll_fd);
#endif
