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
  char* bind_address = "127.0.0.1";
  char* bind_port = "47156";

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
      fprintf(stderr, "Parsing of configuration files not yet implemented\n");
    default: /* '?' */
      fprintf(stderr, "Usage: %s [-h bindaddress][-p bindport][-d dir][-c config]\n",
              argv[0]);
      exit(-1);
    }
  }

  // TODO: parse config file.

  // resolve bind args to addrinfo
  int s = getaddrinfo(bind_address, bind_port, NULL, &(ret->Connection.addrinfo));
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(EXIT_FAILURE);
  }

  
}
