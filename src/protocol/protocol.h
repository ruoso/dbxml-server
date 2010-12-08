#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MAX_TOKEN_SIZE 255
#define MAX_HEADERS 50

typedef struct DbXmlHeader {
  char key[MAX_TOKEN_SIZE];
  char val[MAX_TOKEN_SIZE];
} DbXmlHeader;

typedef struct DbXmlCredentials {
  DbXmlHeader* headers[MAX_HEADERS];
} DbXmlCredentials;

typedef struct DbXmlSessionOptions {
  DbXmlHeader* headers[MAX_HEADERS];
} DbXmlSessionOptions;

typedef struct DbXmlRequest {
  char method[MAX_TOKEN_SIZE];
  char command[MAX_TOKEN_SIZE];
  DbXmlHeader* headers[MAX_HEADERS];
  int body_size;
  int body_read;
} DbXmlRequest;

struct DbXmlResponse; typedef struct DbXmlResponse DbXmlResponse;
struct DbXmlResponse {
  int code;
  char message[MAX_TOKEN_SIZE];
  DbXmlHeader* headers[MAX_HEADERS];
  int body_size;
  void* private_data;
  int (*body_producer)(DbXmlSessionData* session,
                       DbXmlResponse* res,
                       char* buffer, int buf_limit);
  int (*finish)(DbXmlSessionData* session,
                DbXmlResponse* res);
};

void protocol_start_session(DbXmlSessionData *session);
void protocol_request_response(DbXmlSessionData *session);
int  protocol_request_read_body(DbXmlSessionData *session,
                                DbXmlRequest *req,
                                char* buffer, int buf_limit);

#endif
