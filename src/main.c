#include "http/http.h"
#include "server.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int
main (int argc, const char **argv)
{
  openlog (NULL, 0, LOG_USER);

  server_info *s = server_init ("8080", argv[1]);

  struct sockaddr connected_client;
  socklen_t addrlen = sizeof (connected_client);
  while (true)
    {

      connection *connection = server_accept_connection (s);
      request req = { 0 };
      size_t bytes = server_http_receive (connection, &req);
      page_t p = server_load_page (req.resource);
      response res = { 0 };
      res.content = p.data;
      res.content_size = p.size;
     printf("wtf is this %s\n",(char *) res.content);
      if (res.content)
        {
          create_ok_default_header (&res);
        }
      else
        create_error_default_header (&res);

      server_http_send (connection, &res);
      server_dispose_page (&p);
      server_close_connection (connection);
    }
  server_deinit (s);
  return 0;
}
