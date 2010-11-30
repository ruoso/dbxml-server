#ifndef CONN_HANDLE_CLIENT_H
#define CONN_HANDLE_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>

void handle_client(int client_fd, struct sockaddr *peer_addr, socklen_t len, DbXmlServerOptions *options);

#endif
