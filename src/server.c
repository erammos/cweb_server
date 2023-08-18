#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define server_IMPORT
#include "server.h"

page_t
server_load_page (const char *filename)
{
  char *content = NULL;
  size_t content_size = 0;

  FILE *index_file = fopen (filename, "r+");
  if (index_file == NULL)
    {
      LOG_ERROR ("fopen error: %s\n", strerror (errno));
      goto out;
    }
  content = malloc (sizeof (char) * MAX_READ_SIZE);
  content_size = fread (content, sizeof (char), MAX_READ_SIZE, index_file);
  fclose (index_file);

out:
  page_t page = { .data = content, .size = content_size };

  return page;
}

void
server_dispose_page (page_t *page)
{
  page->size = 0;
  free (page->data);
}

connection *
server_accept_connection (server_info *s)
{
  connection *info = malloc (sizeof (connection));

  info->fd = accept (s->socket, &info->address, &info->len);
  if (info->fd == -1)
    {
      exit (1);
    }

  struct sockaddr_in *addr_in = (struct sockaddr_in *)(&info->len);
  LOG ("Accepted connection from  %s\n", inet_ntoa (addr_in->sin_addr));
  info->server = s;
  return info;
}

server_info *
server_init (const char *port, const char *workspace)
{

  int socketfd;
  if ((socketfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
    {
      LOG_ERROR ("Error create socket %d (%s)", errno, strerror (errno))
      exit (1);
    }
  const int enable = 1;
  if (setsockopt (socketfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof (int))
      < 0)
    LOG_ERROR ("setsockopt(SO_REUSEADDR) failed")

  int status;
  struct addrinfo hints;
  struct addrinfo *servinfo;
  struct sockaddr client;

  memset (&hints, 0, sizeof (hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if ((status = getaddrinfo (NULL, port, &hints, &servinfo)) != 0)
    {
      LOG_ERROR ("getaddrinfo error: %s\n", gai_strerror (status));
      exit (1);
    }

  if ((status = bind (socketfd, servinfo->ai_addr, sizeof (struct sockaddr)))
      != 0)
    {
      LOG_ERROR ("bind error: %s\n", gai_strerror (status));
      exit (1);
    }
  freeaddrinfo (servinfo);

  listen (socketfd, 1);
  server_info *info = (server_info *)malloc (sizeof (server_info));
  memset (info, 0, sizeof (*info));
  info->socket = socketfd;
  info->workspace = workspace;

  return info;
}
void
server_deinit (server_info *s)
{
  shutdown (s->socket, 2);
  free (s);
}

void
server_close_connection (connection *connection)
{
  close (connection->fd);
  free (connection);
}

size_t
server_http_receive (connection *connection, request *request)
{

  void *buffer = malloc (sizeof (char) * 1024);
  memset (buffer, 0, 1024);
  size_t bytes;
  bytes = recv (connection->fd, buffer, 1024, 0);
  if (bytes <= 0)
    {
      LOG_ERROR ("Did't read bytes");
      goto out;
    }
  parse_get_request (connection->server->workspace, request, buffer);
out:
  free (buffer);
  return bytes;
}
size_t
server_http_send (connection *connection, response *response)
{
  send (connection->fd, response->header, response->header_size, 0);
  send (connection->fd, response->content, response->content_size, 0);
  return response->header_size + response->content_size;
}
