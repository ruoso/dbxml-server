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
} DbXmlSessionData;


#endif
