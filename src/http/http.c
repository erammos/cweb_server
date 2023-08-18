#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define http_IMPORT
#include "http.h"

void
parse_get_request (const char *working_path, request *req, void *buffer)
{
  size_t working_size = strlen (working_path);


  printf("%s\n",(char *) buffer);
  const char *s = strchr (buffer, '/');

  const char *e = strchr (s, ' ');
  memcpy (req->resource, working_path, working_size);
  memcpy (req->resource + working_size, s, e - s);
  memcpy (req->type, "GET", sizeof ("GET"));
  printf("%s\n", req->resource);

}

void
create_ok_default_header (response *res)
{
  sprintf (res->header,
           "HTTP/1.1 200 OK\n Connection: Keep-Alive \n Accept-Ranges:bytes\n "
           "Content-Length:%zu\n Content-Type:text/html\n\n",
           res->content_size);
  size_t header_size = strlen (res->header) * sizeof (char);
  res->header_size = header_size;
}

void
create_error_default_header (response *res)
{
  sprintf (res->header, "HTTP/1.1 404 Not Found\n");
  size_t header_size = strlen (res->header) * sizeof (char);
  res->header_size = header_size;
}
