#include "io.h"

int initial_handshake(DbXmlSessionData* session) {

  WRITEDATAVARS;
  READLINEVARS;

  // first thing to expect is: 
  // SESSION $envname VERSION $version\n
  READLINE;
  if (sscanf(line, "SESSION %255s VERSION %f",
             session->Session.env_name,
             &(session->Session.client_version)) != 2) {
    LOG_INFO("Client sent bad data (%s)\n", line);
    return -1;
  }
  
  // now we send the server version
  WRITEDATA(session, "SERVER VERSION 0.001\n", 21);
  return 0;

}

int receive_credentials(DbXmlSessionData* session, DbXmlCredentials* cred) {

  READLINEVARS;
  PARSEHEADERVARS;

  // now we need to receive the credentials.
  memset(cred, 0, sizeof(DbXmlCredentials));
  int header_count = 0;
  for (;;) {
    READLINE;
    // after the last credential, we receive a blank line
    if (line[0] == 0)
      break;

    // refuse more then MAX_HEADERS headers.
    if (header_count == MAX_HEADERS) {
      LOG_ERROR("received too many credentials");
      return -1;
    }

    // receive the credentials.
    DbXmlHeader* header = malloc(sizeof(DbXmlHeader));
    if (header == NULL) {
      LOG_ERROR("Failed to allocate header memory.\n");
      return -1;
    }
      
    PARSEHEADER;
    
    cred->headers[header_count++] = header;
  }
  return 0;
}

int check_authentication(DbXmlSessionData* session, DbXmlCredentials* cred) {
  // TODO: check authentication
  return 0;
}

int free_credentials(DbXmlSessionData* session, DbXmlCredentials* cred) {
  // We can get rid of the credentials now, the authentication module
  // should not keep a copy of them anyway.
  int header_count = MAX_HEADERS;
  while (header_count) {
    header_count--;
    if (cred->headers[header_count])
      free(cred->headers[header_count]);
  }
  return 0;
}

int initialize_session(DbXmlSessionData* session) {
  return 0;
}

int session_response(DbXmlSessionData* session, int res) {

  WRITEDATAVARS;
  // initialize session
  if (res) {
    WRITEDATA(session, "ACCESS DENIED\n", 14);
  } else {
    WRITEDATA(session, "SESSION STARTING\n", 17);
  }
  return res;
}

int receive_session_options(DbXmlSessionData* session, DbXmlSessionOptions* options) {

  READLINEVARS;
  PARSEHEADERVARS;

  // session negotiation
  // now we need to receive the credentials.
  int header_count = 0;
  for (;;) {
    READLINE;
    // after the last option, we receive a blank line
    if (line[0] == 0)
      break;
    // refuse more then MAX_HEADERS headers.
    if (header_count == MAX_HEADERS) {
      LOG_ERROR("received too many options");
      return -1;
    }

    // receive the credentials.
    DbXmlHeader* header = malloc(sizeof(DbXmlHeader));
    if (header == NULL) {
      LOG_ERROR("Failed to allocate header memory.\n");
      return -1;
    }
    PARSEHEADER;
    
    options->headers[header_count++] = header;
  }
  return 0;
}

int initialize_session_options(DbXmlSessionData* session, DbXmlSessionOptions* options, DbXmlSessionOptions** ret) {
  // TODO
  return 0;
}

int send_session_options(DbXmlSessionData* session, DbXmlSessionOptions* options) {
  // TODO
  return 0;
}

int free_session_options(DbXmlSessionData* session, DbXmlSessionOptions* options) {
  int header_count = MAX_HEADERS;
  while (header_count) {
    header_count--;
    if (options->headers[header_count])
      free(options->headers[header_count]);
  }
  return 0;
}

int request_response_mode(DbXmlSessionData* session) {
  // TODO
  return 0;
}

int free_session(DbXmlSessionData* session) {
  // TODO
  return 0;
}

void protocol_start_session(DbXmlSessionData *session) {

  int r;
  DbXmlCredentials cred;
  DbXmlSessionOptions options;
  DbXmlSessionOptions *options_set;

  if (initial_handshake(session) != 0)
    return;

  r = receive_credentials(session, &cred);

  if (r == 0)
    r = check_authentication(session, &cred);
  
  free_credentials(session, &cred);

  if (r == 0)
    r = initialize_session(session);

  if (session_response(session, r) != 0)
    return;

  r = receive_session_options(session, &options);

  options_set = NULL;
  if (r == 0)
    r = initialize_session_options(session, &options, &options_set);
  
  if (r == 0)
    r = send_session_options(session, options_set);

  free_session_options(session, &options);

  if (r == 0)
    request_response_mode(session);
  
  free_session(session);
  
}

