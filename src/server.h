#ifndef _HPOT_SERVER_H_
#define _HPOT_SERVER_H_

#include <stdint.h>

#define PORT 8888
#define MAX_LISTEN 1024
#define MAX_EVENTS 65535

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
