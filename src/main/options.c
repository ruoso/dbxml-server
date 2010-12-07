#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "options.h"

void parse_options(int argc, char* argv[], DbXmlServerOptions* ret) {

  // default values
  ret->Authentication.module = "basic";
  ret->Main.server_home = ".";
  char* bind_address = "0";
  char* bind_port = "47156";
  strcpy(ret->Encryption.keyfile, "server.key");
  strcpy(ret->Encryption.certfile, "server.pem");
  strcpy(ret->Encryption.cafile, "");
  strcpy(ret->Encryption.crlfile, "");

  // getopt parsing...
  int opt = 0;
  while ((opt = getopt(argc, argv, "h:p:d:c:")) != -1) {
    switch (opt) {
    case 'h':
      bind_address = optarg;
      break;
    case 'p':
      bind_port = optarg;
      break;
    case 'd':
      ret->Main.server_home = optarg;
      break;
    case 'c':
      LOG_ERROR("Parsing of configuration files not yet implemented\n");
    default: /* '?' */
      LOG_ERROR("Usage: %s [-h bindaddress][-p bindport][-d dir][-c config]\n",
                argv[0]);
      exit(-1);
    }
  }

  // TODO: parse config file.

  // resolve bind args to addrinfo
  struct addrinfo hints;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_socktype = SOCK_STREAM;
  int s = getaddrinfo(bind_address, bind_port, &hints, &(ret->Connection.addrinfo));
  if (s != 0) {
    LOG_ERROR("getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  
}
