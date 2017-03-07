#include "response.h"
#include "buffer.h"
#include "util.h"
#include <assert.h>

// 响应报文状态行
void build_response_line(request_t *r) {
  buffer_t *sb = &r->send_buffer;
  char *version;

  if (r->version_minor == 1)
    version = "HTTP/1.1 ";
  else
    version = "HTTP/1.0 ";

  buffer_append(sb, version);
  buffer_append(sb, build_status_msg(r->status));
  buffer_append(sb, CRLF);
}

int build_response_header(request_t *r) {
  return 0;
}

int build_response(request_t *r) {
  return 0;
}

int build_response_err(request_t *r) {
  build_response_line(r);
  return 0;
}

char *build_status_msg(int status) {
  char *msg;
  switch(status) {
    case 100: msg = "100 Continue"; break;
    case 101: msg = "101 Switching Protocols"; break;
    case 102: msg = "102 Processing"; break;
    case 200: msg = "200 OK"; break;
    case 201: msg = "201 Created"; break;
    case 202: msg = "202 Accepted"; break;
    case 203: msg = "203 Non-Authoritative Information"; break;
    case 204: msg = "204 No Content"; break;
    case 205: msg = "205 Reset Content"; break;
    case 206: msg = "206 Partial Content"; break;
    case 207: msg = "207 Multi-Status"; break;
    case 300: msg = "300 Multiple Choices"; break;
    case 301: msg = "301 Moved Permanently"; break;
    case 302: msg = "302 Found"; break;
    case 303: msg = "303 See Other"; break;
    case 304: msg = "304 Not Modified"; break;
    case 305: msg = "305 Use Proxy"; break;
    case 306: msg = "306 Switch Proxy"; break;
    case 307: msg = "307 Temporary Redirect"; break;
    case 400: msg = "400 Bad Request"; break;
    case 401: msg = "401 Unauthorized"; break;
    case 402: msg = "402 Payment Required"; break;
    case 403: msg = "403 Forbidden"; break;
    case 404: msg = "404 Not Found"; break;
    case 405: msg = "405 Method Not Allowed"; break;
    case 406: msg = "406 Not Acceptable"; break;
    case 407: msg = "407 Proxy Authentication Required"; break;
    case 408: msg = "408 Request Timeout"; break;
    case 409: msg = "409 Conflict"; break;
    case 410: msg = "410 Gone"; break;
    case 411: msg = "411 Length Required"; break;
    case 412: msg = "412 Precondition Failed"; break;
    case 413: msg = "413 Request Entity Too Large"; break;
    case 414: msg = "414 Request-URI Too Long"; break;
    case 415: msg = "415 Unsupported Media Type"; break;
    case 416: msg = "416 Requested Range Not Satisfiable"; break;
    case 417: msg = "417 Expectation Failed"; break;
    case 418: msg = "418 I'm a teapot"; break;  // 233
    case 421: msg = "421 There are too many connections from your internet address"; break;
    case 422: msg = "422 Unprocessable Entity"; break;
    case 423: msg = "423 Locked"; break;
    case 424: msg = "424 Failed Dependency"; break;
    case 425: msg = "425 Unordered Collection"; break;
    case 426: msg = "426 Upgrade Required"; break;
    case 449: msg = "449 Retry With"; break;
    case 451: msg = "451 Unavailable For Legal Reasons"; break;
    case 500: msg = "500 Internal Server Error"; break;
    case 501: msg = "501 Not Implemented"; break;
    case 502: msg = "502 Bad Gateway"; break;
    case 503: msg = "503 Service Unavailable"; break;
    case 504: msg = "504 Gateway Timeout"; break;
    case 505: msg = "505 HTTP Version Not Supported"; break;
    case 506: msg = "506 Variant Also Negotiates"; break;
    case 507: msg = "507 Insufficient Storage"; break;
    case 509: msg = "509 Bandwidth Limit Exceeded"; break;
    case 510: msg = "510 Not Extended"; break;
    default:
      log_err("unsupported status code");
      assert(false);
  }
  return msg;
}
