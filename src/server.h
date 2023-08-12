/**
 * @file
 * @author Elias Rammos <rammoselias@gmail.com>
 * @version $Date$
 */

#ifndef SERVER_H
#define SERVER_H

#include "http/http.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#ifdef server_IMPORT
#define EXTERN
#else
#define EXTERN extern
#endif

#define MAX_READ_SIZE 1024
#ifdef SYSLOG
#define LOG_ERROR(str, opts...) syslog(LOG_ERR, str, opts);
#else
#define LOG_ERROR(str, opts...) printf(str, ##opts);
#define LOG LOG_ERROR
#endif
typedef struct page_s {
  char *data;
  size_t size;
} page_t;

typedef struct server_s {
  int socket;
  const char *workspace;
} server_info_t;

typedef struct connection_s {
  struct sockaddr address;
  socklen_t len;
  int fd;
  server_info_t *server;
} connection_t;

EXTERN server_info_t *server_init(const char *port, const char *workspace);
EXTERN void server_deinit(server_info_t *s);
EXTERN connection_t *server_accept_connection(server_info_t *s);
EXTERN void server_close_connection(connection_t *connection);
EXTERN size_t server_http_receive(connection_t *connection, request_t *request);
EXTERN size_t server_http_send(connection_t *connection, response_t *response);
page_t server_load_page(const char *filename);
void server_dispose_page(page_t *page);
#undef EXTERN
#endif
