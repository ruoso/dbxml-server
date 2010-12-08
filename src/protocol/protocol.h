#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_TOKEN_SIZE 255
#define MAX_HEADERS 50

typedef struct DbXmlHeader {
  char key[MAX_TOKEN_SIZE];
  char val[MAX_TOKEN_SIZE];
} DbXmlHeader;

typedef struct DbXmlCredentials {
  DbXmlHeader* headers[50];
} DbXmlCredentials;

typedef struct DbXmlRequest {
  char method[MAX_TOKEN_SIZE];
  char command[MAX_TOKEN_SIZE];
  DbXmlHeader* headers[MAX_HEADERS];
  int body_size;
} DbXmlRequest;

typedef struct DbXmlResponse {
  int code;
  char message[MAX_TOKEN_SIZE];
  DbXmlHeader* headers[MAX_HEADERS];
  int body_size;
} DbXmlResponse;

void protocol_start_session(DbXmlSessionData *session);

#endif
