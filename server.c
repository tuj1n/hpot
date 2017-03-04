#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <time.h>
#include "util.h"

#define PORT 8888
#define MAX_LISTEN 1024
#define MAX_EVENTS 65535

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

int main(int argc, char *argv[]) {
  // 创建 socket 连接并监听
  int listen_fd = startup(PORT);
  setnonblocking(listen_fd);

  struct sockaddr_in client_addr;
  socklen_t client_addrlen = 1;
  memset(&client_addr, 0, sizeof(struct sockaddr_in));

  // 创建 epoll, 注册 listen_fd
  int epoll_fd = epoll_create1(0);
  struct epoll_event ev, events[MAX_EVENTS];
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = listen_fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

  while (1) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    int i, fd;

    for (i = 0; i != nfds; i++) {
      fd = events[i].data.fd;
      char buf[1024];
      int n;

      // 新连接
      if (fd == listen_fd) {
        printf("connect(), time: %ld\n", time(NULL));
        int conn_fd;
          conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addrlen);
          setnonblocking(conn_fd);
          ev.data.fd = conn_fd;
          epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &ev);
      }
      else if (events[i].events & EPOLLIN) {
        printf("EPOLLIN\n");
          while ((n = read(fd, buf, sizeof(buf))) > 0) {
            write(STDOUT_FILENO, buf, n);
          }
          char res[] = "HTTP 200/OK\r\nContent-type: text/plain\r\nContent-length: 19\r\n\r\nHi! I'm a message!\r\n";
          write(fd, res, sizeof(res));
          close(fd);
      }
      else if (events[i].events & EPOLLOUT) {
        printf("EPOLLOUT\n");
      }
    }
  }
}
