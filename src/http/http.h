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

typedef struct
{
  char type[16];
  char resource[256];
  size_t size;

} request;

typedef struct
{
  char header[1024];
  size_t header_size;
  void *content;
  size_t content_size;
} response;

EXTERN void parse_get_request (const char *working_dir, request *req,
                               void *buffer);
EXTERN void create_ok_default_header (response *res);
EXTERN void create_error_default_header (response *res);
#undef EXTERN
#endif
