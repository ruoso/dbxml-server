#ifndef MAIN_OPTIONS_H
#define MAIN_OPTIONS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define LOG(type, args...) fprintf(stderr, "[%s] ", type); fprintf(stderr, ## args);
#define LOG_DEBUG(args...) LOG("DEBUG", ## args);
#define LOG_INFO(args...)  LOG("INFO", ## args);
#define LOG_ERROR(args...) LOG("ERROR", ## args);

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
