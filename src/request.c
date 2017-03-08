#include "request.h"
#include "server.h"
#include "handle.h"
#include "util.h"
#include <stdbool.h>

int request_init(request_t *r, int fd, int epfd) {
  r->fd = fd;
  r->epfd = epfd;
  r->active_time = time(NULL);

  buffer_init(&r->recv_buffer);
  buffer_init(&r->send_buffer);

  r->state = RL_S_BEGIN;
  r->recv_handler = handle_request_line;
  r->send_handler = send_response_buffer;
  r->done = false;
  r->response_done = false;

  string_init(&r->uri.path);
}

int update_active(request_t *r) {
  return 0;
}

int close_request(request_t *r) {
  return 0;
}


int request_disable_in(request_t *r) {
  if (r->event.events & EPOLLIN) {
    r->event.events &= EPOLLIN;
    return epoll_ctl(r->epfd, EPOLL_CTL_MOD, r->fd, &r->event);
  }
  return 0;
}

int request_enable_in(request_t *r) {
  if (!(r->event.events & EPOLLIN)) {
    r->event.events |= EPOLLIN;
    return epoll_ctl(r->epfd, EPOLL_CTL_MOD, r->fd, &r->event);
  }
  return 0;
}

int request_disable_out(request_t *r) {
  if (r->event.events & EPOLLOUT) {
    r->event.events &= EPOLLOUT;
    return epoll_ctl(r->epfd, EPOLL_CTL_MOD, r->fd, &r->event);
  }
  return 0;
}

int request_enable_out(request_t *r) {
  if (!(r->event.events & EPOLLOUT)) {
    r->event.events |= EPOLLOUT;
    return epoll_ctl(r->epfd, EPOLL_CTL_MOD, r->fd, &r->event);
  }
  return 0;
}
