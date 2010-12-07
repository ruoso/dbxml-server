#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../main/session.h"
#include "../tls/start.h"
#include "handle_client.h"

#define MAX_CHARS_ADDR 1025

void* thread_handler(void *a) {
  DbXmlSessionData *args = a;
  
  char printable_host[MAX_CHARS_ADDR] = "";
  char printable_port[MAX_CHARS_ADDR] = "";
  getnameinfo(&(args->Connection.peer_addr), args->Connection.peer_addr_len,
              printable_host, MAX_CHARS_ADDR-1,
              printable_port, MAX_CHARS_ADDR-1,
              NI_NUMERICHOST | NI_NUMERICSERV);
  LOG_INFO("Client %s:%s connected\n", printable_host, printable_port);
  
  handle_session(args);

  LOG_INFO("Client %s:%s disconnected\n", printable_host, printable_port);
  args->options = NULL;
  free(args);
  close(args->Connection.client_fd);
  return NULL;
}

void handle_client(int client_fd, struct sockaddr *peer_addr, socklen_t len, DbXmlServerOptions *options) {
  // will be freed by the thread
  DbXmlSessionData *args = malloc(sizeof(DbXmlSessionData));
  memcpy(&(args->Connection.peer_addr), peer_addr, len);
  args->Connection.peer_addr_len = len;
  args->Connection.client_fd = client_fd;
  args->options = options; // nobody writes to options at this point.

  pthread_t thread;
  pthread_create(&thread, NULL, &thread_handler, args );
}

