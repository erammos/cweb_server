#include "http/http.h"
#include "server.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, const char **argv) {
  openlog(NULL, 0, LOG_USER);

  server_info_t *s = server_init("8080", "some workspace");

  struct sockaddr connected_client;
  socklen_t addrlen = sizeof(connected_client);
  while (true) {

    connection_t *connection = server_accept_connection(s);

    request_t req = {0};
    server_http_receive(connection, &req);

    page_t p = server_load_page(req.resource);
    response_t res = {0};
    res.content = p.data;
    res.content_size = p.size;

    create_default_header(&res);
    server_http_send(connection, &res);

    server_dispose_page(&p);
    server_close_connection(connection);
  }
  server_deinit(s);
  return 0;
}
