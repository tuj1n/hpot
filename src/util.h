#ifndef _HPOT_UTIL_H_
#define _HPOT_UTIL_H_

#include <stdio.h>
#include <stdbool.h>

#define OK 0
#define AGAIN 1
#define ERROR -1
#define ERR_REQUEST -2
#define ERR_METHOD -3
#define BUFFER_FULL -4

#define CRLF "\r\n"

#define SERVER_STRING "hpot/0.1.0"

#define log_info(msg, ...) fprintf(stdout, "%s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_err(msg, ...) fprintf(stderr, "ERROR %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log(msg, ...) fprintf(stderr, msg "\n", ##__VA_ARGS__)

typedef struct string {
  char *str;
  int len;
} string_t;

static inline int min(int x, int y) {
  return x < y ? x : y;
}

static inline int max(int x, int y) {
  return x > y ? x : y;
}

bool issame(char *str1, const char *str2, int len);

void string_init(string_t *s);
#endif
