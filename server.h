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

int startup(uint16_t port);
int setnonblocking(int fd);
int accept_connection(int listen_fd, int epoll_fd);
#endif
