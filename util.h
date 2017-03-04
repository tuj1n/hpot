#ifndef _HPOT_UTIL_H_
#define _HPOT_UTIL_H_

#include <stdio.h>
#define log_err(msg, ...) fprintf(stderr, "%s:%d: " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif
