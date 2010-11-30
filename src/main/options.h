#ifndef MAIN_OPTIONS_H
#define MAIN_OPTIONS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct DbXmlServerOptions {
  struct {
    char* server_home;
  } Main;
  struct {
    char* module;
  } Authentication;
  struct {
    struct addrinfo *addrinfo;
  } Connection;
} DbXmlServerOptions;

void parse_options(int argc, char* argv[], DbXmlServerOptions* ret);

#endif
