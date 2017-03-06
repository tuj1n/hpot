#ifndef _HPOT_BUFFER_H_
#define _HPOT_BUFFER_H_

#include <stdbool.h>

#define MAX_BUFSIZE 40960

typedef struct {
  char *start;
  char *end;
  char *limit;
  char data[MAX_BUFSIZE];
} buffer_t;

static inline void buffer_init(buffer_t *buf) {
  buf->start = buf->data;
  buf->end = buf->data;
  buf->limit = buf->data + MAX_BUFSIZE - 1;
}

static inline int buffer_size(buffer_t *buf) {
  return buf->end - buf->start; //  +1?
}

static inline bool buffer_full(buffer_t *buf) {
  return buf->end >= buf->limit;
}

static inline int buffer_rest(buffer_t *buf) {
  return buf->limit - buf->end;
}

int buffer_recv(buffer_t *buf, int fd);
int buffer_send(buffer_t* buf, int fd);
void buffer_print(buffer_t *buf);
#endif
