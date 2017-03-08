#ifndef _HPOT_PARSE_H_
#define _HPOT_PARSE_H_

#include "request.h"

int parse_request_line(request_t *r);
int parse_request_header(request_t *r);
int parse_request_body(request_t *r);
int parse_request_method(char *start, char * end);
int parse_request_uri(request_t *r, char *start, char *end);
#endif
