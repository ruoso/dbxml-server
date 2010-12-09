#ifndef MAIN_SESSION_H
#define MAIN_SESSION_H

#include "../main/options.h"

typedef struct DbXmlSessionData {
  DbXmlServerOptions *options;
  struct {
    struct sockaddr peer_addr;
    socklen_t peer_addr_len;
    int client_fd;
  } Connection;
  struct {
    gnutls_session_t session;
  } Encryption;
  struct {
    char env_name[256];
    float client_version;
    void* priv_data;
  } Session;
} DbXmlSessionData;


#endif
