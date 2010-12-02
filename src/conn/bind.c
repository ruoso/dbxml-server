#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../main/options.h"
#include "bind.h"
#include "handle_client.h"

#define MAX_CHARS_ADDR 1025

void bind_server(DbXmlServerOptions *options) {
  int bound = 0;
  struct addrinfo *addrinfo = options->Connection.addrinfo;
  while (!bound && addrinfo) {

    // create the socket
    int socket_fd;
    socket_fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, 0);
    if (socket_fd == -1) {
      LOG_ERROR("Error creating socket (%d, %d): %s\n",
              addrinfo->ai_family, addrinfo->ai_socktype,
              strerror(errno));
      addrinfo = addrinfo->ai_next;
      continue;
    }

    // let's build an user friendly representation of this address in
    // order to print the trace message about the success or failure
    // in the bind operation.
    char printable_host[MAX_CHARS_ADDR] = "";
    char printable_port[MAX_CHARS_ADDR] = "";
    getnameinfo(addrinfo->ai_addr, addrinfo->ai_addrlen,
                printable_host, MAX_CHARS_ADDR-1,
                printable_port, MAX_CHARS_ADDR-1,
                NI_NUMERICHOST | NI_NUMERICSERV);

    // bind to the address
    int res = bind(socket_fd, addrinfo->ai_addr, addrinfo->ai_addrlen);
    if (res != 0) {
      LOG_ERROR("Error binding to address %s:%s - %s\n",
              printable_host, printable_port,
              strerror(errno));
      addrinfo = addrinfo->ai_next;
      close(socket_fd);
      continue;
    }

    // listen on the socket
    if (listen(socket_fd, 10) != 0) {
      LOG_ERROR("Error listening to socket, trying next address.\n");
      addrinfo = addrinfo->ai_next;
      close(socket_fd);
      continue;
    }

    // From this point on, we assume to have a valid connection
    bound = 1;
    LOG_INFO("Listening to %s:%s\n", printable_host, printable_port);

    int client_fd = 0;
    struct sockaddr peer_addr;
    socklen_t peer_addr_size = sizeof(struct sockaddr);
    while ((client_fd = accept(socket_fd, &peer_addr, &peer_addr_size))) {
      if (client_fd >= 0) {
        // as it is understood that this might open a new thread, this function
        // won't close the client_fd. handle_client should do it.
        handle_client(client_fd, &peer_addr, peer_addr_size, options);

      } else {
        LOG_DEBUG("Error in accept: %s\n", strerror(errno));
      }
    }

    // let's close the socket
    close(socket_fd);
  }
  if (!bound) {
    LOG_ERROR("Could not bind to any address.\n");
    exit(-1);
  }
}
