#ifndef _HPOT_RESPONSE_H_
#define _HPOT_RESPONSE_H_

#include "request.h"

void build_response_line(request_t *r);
int build_response_header(request_t *r);
int build_response(request_t *r);
int build_response_err(request_t *r);

char *build_status_msg(int status);
#endif
