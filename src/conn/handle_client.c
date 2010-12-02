#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../main/options.h"
#include "handle_client.h"

#define MAX_CHARS_ADDR 1025

typedef struct thread_args {
  struct sockaddr peer_addr;
  socklen_t peer_addr_len;
  int client_fd;
  DbXmlServerOptions *options;
} thread_args;

void* thread_handler(void *a) {
  thread_args *args = (thread_args*)a;
  char printable_host[MAX_CHARS_ADDR] = "";
  char printable_port[MAX_CHARS_ADDR] = "";
  getnameinfo(&(args->peer_addr), args->peer_addr_len,
              printable_host, MAX_CHARS_ADDR-1,
              printable_port, MAX_CHARS_ADDR-1,
              NI_NUMERICHOST | NI_NUMERICSERV);
  LOG_INFO("Client %s:%s connected\n", printable_host, printable_port);
  
  const char* message = "handling connection...\n";
  int to_write = strlen(message);
  while (to_write) {
    sleep(1);
    to_write -= write(args->client_fd, message, to_write);
  }

  LOG_INFO("Client %s:%s disconnected\n", printable_host, printable_port);
  free(args);
  close(args->client_fd);
  return NULL;
}

void handle_client(int client_fd, struct sockaddr *peer_addr, socklen_t len, DbXmlServerOptions *options) {
  // will be freed by the thread
  thread_args *args = malloc(sizeof(thread_args));
  memcpy(&(args->peer_addr), peer_addr, len);
  args->peer_addr_len = len;
  args->client_fd = client_fd;
  args->options = options; // nobody writes to options at this point.

  pthread_t thread;
  pthread_create(&thread, NULL, &thread_handler, args );
}

