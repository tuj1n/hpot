#ifndef _HPOT_EPOLL_H
#define _HPOT_EPOLL_H

#include <sys/epoll.h>

#define MAX_EVENTS 1024
void epoll_add(int epfd, int fd, starcut epoll_event *event, int flags);
#endif
