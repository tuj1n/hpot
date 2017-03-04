#ifndef _HPOT_SERVER_H_
#define _HPOT_SERVER_H_

int startup(uint16_t port); // 返回 listen_fd
int set_non_blocking(int fd);
#endif
