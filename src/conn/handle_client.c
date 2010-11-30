#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "../main/options.h"
#include "handle_client.h"

void handle_client(int client_fd, struct sockaddr *peer_addr, socklen_t len, DbXmlServerOptions *options) {
  const char* message = "handling client...\n";
  int to_write = strlen(message);
  while (to_write) {
    to_write -= write(client_fd, message, to_write);
  }
  close(client_fd);
}
