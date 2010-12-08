#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef struct DbXmlHeader {
  char key[255];
  char val[255];
} DbXmlHeader;

typedef struct DbXmlCredentials {
  int headers_count;
  DbXmlHeader** headers;
} DbXmlCredentials;

typedef struct DbXmlRequest {
  char method[255];
  char command[255];
  int headers_count;
  DbXmlHeader** headers;
  int body_size;
} DbXmlRequest;

typedef struct DbXmlResponse {
  int code;
  char message[255];
  int headers_count;
  DbXmlHeader** headers;
  int body_size;
} DbXmlResponse;

void protocol_start_session(DbXmlSessionData *session);

#endif
