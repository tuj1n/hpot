#ifndef _HPOT_REQUEST_H_
#define _HPOT_REQUEST_H_

#include "server.h"
#include <sys/epoll.h>
#include <time.h>

#define OK  0
#define AGAIN 1
#define ERROR -1

typedef struct {
  int fd;
  int epfd;
  struct epoll_event event;

  method_t method;
  char *client_ip;

  time_t active_time;

  int state;  // 200 301 304...

} request_t;

int request_init(request_t *r, int fd, int epfd);
int update_active(request_t *r);
int close_request(request_t *r);
#endif
