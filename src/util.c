#include "util.h"

bool issame(char *str1, const char *str2, int len) {
  int i;
  for (i = 0; i != len; i++)
    if (str1[i] != str2[i])
      return false;

  return true;
}

void string_init(string_t *s) {
  s->str = NULL;
  s->len = 0;
}
