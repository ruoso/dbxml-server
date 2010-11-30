#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../main/options.h"
#include "bind.h"

#define MAX_CHARS_ADDR 1025

void bind_server(DbXmlServerOptions *options) {
  int bound = 0;
  struct addrinfo *addrinfo = options->Connection.addrinfo;
  while (!bound && addrinfo) {

    // create the socket
    int socket_fd;
    socket_fd = socket(addrinfo->ai_family, addrinfo->ai_socktype, 0);
    if (socket_fd == -1) {
      fprintf(stderr, "Error creating socket (%d, %d): %s",
              addrinfo->ai_family, addrinfo->ai_socktype,
              strerror(errno));
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
      fprintf(stderr, "Error binding to address %s:%s - %s",
              printable_host, printable_port,
              strerror(errno));
      continue;
    } else {
      fprintf(stderr, "Binding to %s:%s\n", printable_host, printable_port);
    }


    addrinfo = addrinfo->ai_next;
  }
}
