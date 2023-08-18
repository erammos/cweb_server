/**
 * @file server.h
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
#define LOG_ERROR(str, opts...) syslog (LOG_ERR, str, opts);
#else
#define LOG_ERROR(str, opts...) printf (str, ##opts);
#define LOG LOG_ERROR
#endif
typedef struct page_s
{
  char *data;
  size_t size;
} page_t;

typedef struct
{
  int socket;
  const char *workspace;
} server_info;

typedef struct
{
  struct sockaddr address;
  socklen_t len;
  int fd;
  server_info *server;
} connection;

EXTERN server_info *server_init (const char *port, const char *workspace);
EXTERN void server_deinit (server_info *s);
EXTERN connection *server_accept_connection (server_info *s);
EXTERN void server_close_connection (connection *connection);
EXTERN size_t server_http_receive (connection *connection, request *request);
EXTERN size_t server_http_send (connection *connection, response *response);
EXTERN page_t server_load_page (const char *filename);
EXTERN void server_dispose_page (page_t *page);

#undef EXTERN
#endif
