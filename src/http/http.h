/**
 * @file
 * @author Elias Rammos <rammoselias@gmail.com>
 * @version $Date$
 */

#ifndef HTTP_H
#define HTTP_H

#include <unistd.h>
#ifdef http_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

typedef struct request_s {
  char type[16];
  char resource[256];
  size_t size;

} request_t;

typedef struct response_s {
  char header[1024];
  size_t header_size;
  void *content;
  size_t content_size;
} response_t;

EXTERN void parse_get_request(request_t *req, void *buffer);
EXTERN void create_default_header(response_t *res);
#undef EXTERN
#endif
