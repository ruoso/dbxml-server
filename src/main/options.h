#ifndef MAIN_OPTIONS_H
#define MAIN_OPTIONS_H

#include <gnutls/gnutls.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>

#define LOG(type, args...) fprintf(stderr, "[%s] ", type); fprintf(stderr, ## args);
#define LOG_DEBUG(args...) LOG("DEBUG", ## args);
#define LOG_INFO(args...)  LOG("INFO", ## args);
#define LOG_ERROR(args...) LOG("ERROR", ## args);

#define MAX_FILENAME_SIZE 255

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
  struct {
    char keyfile[MAX_FILENAME_SIZE];
    char certfile[MAX_FILENAME_SIZE];
    char cafile[MAX_FILENAME_SIZE];
    char crlfile[MAX_FILENAME_SIZE];
    gnutls_certificate_credentials_t x509_cred;
    gnutls_priority_t priority_cache;
  } Encryption;
} DbXmlServerOptions;

void parse_options(int argc, char* argv[], DbXmlServerOptions* ret);

#endif
