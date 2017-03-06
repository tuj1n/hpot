#ifndef _HPOT_UTIL_H_
#define _HPOT_UTIL_H_

#include <stdio.h>

#define OK 0
#define ERROR -1
#define AGAIN 1

#define log_info(msg, ...) fprintf(stdout, "%s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log_err(msg, ...) fprintf(stderr, "ERROR %s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define log(msg, ...) fprintf(stderr, msg "\n", ##__VA_ARGS__)
#endif
