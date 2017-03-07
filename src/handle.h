#ifndef _HPOT_HANDLE_H_
#define _HPOT_HANDLE_H_

#include "request.h"

int handle_request(request_t *r);
int handle_response(request_t *r);
int handle_request_line(request_t *r);
int handle_request_uri(request_t *r);
int handle_request_header(request_t *r);
int handle_request_body(request_t *r);
#endif
