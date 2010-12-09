#include "io.h"
#include "../session/session.h"

int receive_command(DbXmlSessionData* session, DbXmlRequest *req) {
  READLINEVARS;

  READLINE;

  if (strncmp(line, "QUIT", 4) == 0) {
    return -1;
  }

  if (sscanf(line, "%10[A-Z] %s", req->method, req->command) != 2) {
    LOG_ERROR("client sent bad data (%s)\n", line);
    return -1;
  }

  if (strcmp(req->method, "POST") == 0) {
    READLINE;
    if (sscanf(line, "Content-length: %d", &(req->body_size)) != 1) {
      LOG_ERROR("POST method requires immediate length header.\n");
      return -1;
    }
  } else if (strcmp(req->method, "GET") == 0) {
    req->body_size = 0;
  } else {
    LOG_ERROR("Uknown method received: %s", req->method);
    return -1;
  }
    
  return 0;
}

int receive_headers(DbXmlSessionData *session, DbXmlRequest *req) {
  READLINEVARS;
  PARSEHEADERVARS;

  int header_count = 0;
  for (;;) {
    READLINE;
    if (line[0] == 0)
      break;
    if (header_count == MAX_HEADERS) {
      LOG_ERROR("received too many headers");
      return -1;
    }
    // receive the credentials.
    DbXmlHeader* header = malloc(sizeof(DbXmlHeader));
    if (header == NULL) {
      LOG_ERROR("Failed to allocate header memory.\n");
      return -1;
    }
    PARSEHEADER;
    req->headers[header_count++] = header;
  }

  return 0;
}

int consume_remaining_req_body(DbXmlSessionData* session, DbXmlRequest *req) {
  READDATAVARS;
  // if the body wasn't entirely consumed during the request
  // processing, then we need to make sure we consume it now.
  while (req->body_size > req->body_read) {
    char buffer[MAX_BUF + 1];
    int remain = req->body_size - req->body_read;
    READDATA(session, buffer, remain > MAX_BUF ? MAX_BUF : remain);
    req->body_read += rd;
  }
  return 0;
}

int output_response_line(DbXmlSessionData* session, DbXmlResponse* res) {
  WRITEDATAVARS;
  // Now we should produce the response.
  char response_line[512] = "";
  sprintf(response_line, "%d %s\n", res->code, res->message);
  int response_size = strlen(response_line);
  WRITEDATA(session, response_line, response_size);
  sprintf(response_line, "Content-length: %d\n", res->body_size);
  response_size = strlen(response_line);
  WRITEDATA(session, response_line, response_size);
  return 0;
}

int output_response_headers(DbXmlSessionData* session, DbXmlResponse* res) {
  WRITEDATAVARS;
  // output the headers...
  int header_count = 0;
  while (header_count < MAX_HEADERS &&
         res->headers[header_count] != NULL) {
    WRITEDATA(session,
              res->headers[header_count]->key,
              strlen(res->headers[header_count]->key));
    WRITEDATA(session, "=", 1);
    WRITEDATA(session,
              res->headers[header_count]->val,
              strlen(res->headers[header_count]->val));
    WRITEDATA(session, "\n", 1);
    header_count++;
  }
  WRITEDATA(session, "\n", 1);
  return 0;
}

int output_response_body(DbXmlSessionData* session, DbXmlResponse* res) {
  WRITEDATAVARS;
  char buffer[MAX_BUF];
  int remaining = res->body_size;
  while (remaining &&
         res->body_producer != NULL &&
         (r = res->body_producer(session, res, buffer, remaining > MAX_BUF ? MAX_BUF : remaining))) {
    WRITEDATA(session, buffer, r);
    remaining -= r;
  }
  if (remaining) {
    LOG_ERROR("body_producer didn't produce the entire specified size.\n");
    return -1;
  }
  return 0;
}

int finish_res(DbXmlSessionData* session, DbXmlResponse* res) {
  if (res->finish != NULL)
    return res->finish(session, res);
  else
    return 0;
}

int free_req_headers(DbXmlSessionData* session, DbXmlRequest* req) {
  int header_count = 0;
  while (header_count < MAX_HEADERS &&
         req->headers[header_count] != NULL) {
    free(req->headers[header_count]);
    req->headers[header_count] = NULL;
    header_count++;
  }
  return 0;
}

int free_res_headers(DbXmlSessionData* session, DbXmlResponse* res) {
  int header_count = 0;
  while (header_count < MAX_HEADERS &&
         res->headers[header_count] != NULL) {
    free(res->headers[header_count]);
    res->headers[header_count] = NULL;
    header_count++;
  }
  return 0;
}

void protocol_request_response(DbXmlSessionData* session) {
  // request_response is a loop.
  for (;;) {
    DbXmlRequest req;
    DbXmlResponse res;
    memset(&req, 0, sizeof(DbXmlRequest));
    memset(&res, 0, sizeof(DbXmlResponse));

    int r;

    r = receive_command(session, &req);

    if (r == 0)
      r = receive_headers(session, &req);
    
    if (r == 0)
      session_request(session, &req, &res);

    if (r == 0)
      r = consume_remaining_req_body(session, &req);
    
    if (r == 0)
      r = output_response_line(session, &res);

    if (r == 0)
      r = output_response_headers(session, &res);

    if (r == 0)
      r = output_response_body(session, &res);

    finish_res(session, &res);
    free_req_headers(session, &req);
    free_res_headers(session, &res);

    if (r != 0)
      break;
  }
}

int protocol_request_read_body(DbXmlSessionData *session,
                               DbXmlRequest *req,
                               char* buffer, int buf_limit) {
  int remaining = req->body_size - req->body_read;
  int r = tls_read(session, buffer,
                   remaining > buf_limit ? buf_limit : remaining);
  if (r > 0)
    req->body_read += r;
  return r;
}


