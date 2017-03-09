#include "buffer.h"
#include "util.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int buffer_recv(buffer_t *buf, int fd) {
  while (!buffer_full(buf)) {
    int rest_buf_size = buffer_rest(buf); //缓冲区剩余大小
    int n = recv(fd, buf->end, rest_buf_size, 0);

    if (n == 0)
      return OK;  // 对方已经关闭连接

    if (n < 0) {
      if (errno == EAGAIN)
          return AGAIN;
      return ERROR;
    }
    buf->end += n;
  }
  return AGAIN; // buffer已满
}

int buffer_send(buffer_t* buf, int fd) {
  while (buffer_size(buf) > 0) {
    int len = send(fd, buf->start, buffer_size(buf), 0);
    if (len == -1) {
        if (errno == EAGAIN)
            return AGAIN;
        return ERROR;
    }
    buf->start += len;
  };
  buffer_init(buf);
  return OK;
}

int buffer_append(buffer_t *buf, char *str) {
  int rest = buffer_rest(buf);
  int len = min(rest, strlen(str));
  memcpy(buf->end, str, len);
  buf->end += len;
  return len;
}

void buffer_print(buffer_t *buf) {
  for (char *c = buf->start; c != buf->end; c++) {
    printf("%c", *c);
    fflush(stdout);
  }
  printf("\n");
}
