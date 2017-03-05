#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>
#include "server.h"
#include "request.h"
#include "handle.h"
#include "heap.h"
#include "util.h"

int startup(uint16_t port) {
  int listen_fd = 0;
  int option = 1;
  struct sockaddr_in server_addr;

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return -1;

  // 重用 bind 中的地址
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    return -1;

  if (listen(listen_fd, MAX_LISTEN) < 0)
    return -1;

  return listen_fd;
}

int setnonblocking(int fd) {
  int flags;
  if ((flags = fcntl(fd, F_GETFL, 0)) < 0)
    return -1;

  flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) < 0)
    return -1;

  return 0;
}

int accept_connection(int listen_fd, int epoll_fd) {
  struct sockaddr_in client_addr;
  socklen_t slen = sizeof(struct sockaddr_in);
  memset(&client_addr, 0, slen);

  // 可能有多个连接到来
  while (true) {
    int conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &slen);
    if (conn_fd < 0) {
      if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        break;
      }
      else {
        log_err("accept");
        break;
      }
    }
    setnonblocking(conn_fd);

    request_t *r = (request_t *)malloc(sizeof(request_t));
    request_init(r, conn_fd, epoll_fd);

    // TODO: 注册request到堆中
    r->event.events = EPOLLIN | EPOLLET;
    r->event.data.ptr = (void*)r;
    r->client_ip = inet_ntoa(client_addr.sin_addr);
    log_info("%s", r->client_ip);

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, r->fd, &r->event);
  }
}

int main(int argc, char *argv[]) {

  log("server started");
  // 创建 socket 连接并监听
  int listen_fd = startup(PORT);
  setnonblocking(listen_fd);

  int epoll_fd = epoll_create1(0);
  struct epoll_event ev, events[MAX_EVENTS];

  ev.events = EPOLLIN | EPOLLET;
  ev.data.ptr = (void *)&listen_fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

  while (true) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    for (int i = 0; i != nfds; i++) {
      int fd = *((int *)(events[i].data.ptr));

      if (fd == listen_fd) {
        accept_connection(listen_fd, epoll_fd);
      }

      request_t *r = (request_t *)events[i].data.ptr;
      int err;
      if (events[i].events & EPOLLIN) {
        if ((err = handle_request(r)) < 0)
          close_request(r);
        else
          update_active(r);
      }
      if (events[i].events & EPOLLOUT) {
        if ((err == handle_response(r)) < 0)
          close_request(r);
        else
          update_active(r);
      }
    }
    check_timeout();
  }

  close(listen_fd);
  return 0;
}
