#ifndef _HPOT_REQUEST_H_
#define _HPOT_REQUEST_H_

#include "server.h"
#include "buffer.h"
#include <sys/epoll.h>
#include <time.h>

#define OK  0
#define AGAIN 1
#define ERROR -1


typedef struct request{
  int fd;
  int epfd;
  struct epoll_event event;

  uint8_t version_major;
  uint8_t version_minor;

  method_t method;
  char *uri;
  char *client_ip;

  time_t active_time;

  int state;  // 200 301 304...

  buffer_t recv_buffer;
  buffer_t send_buffer;

  // 请求文件描述符和长度
  int file_fd;
  int file_len;

  int (*recv_handler)(struct request *r);
  int (*send_handler)(struct request *r);
} request_t;

int request_init(request_t *r, int fd, int epfd);
int update_active(request_t *r);
int close_request(request_t *r);
#endif
