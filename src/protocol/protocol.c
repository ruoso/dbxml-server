#include <strings.h>
#include <string.h>
#include "../main/session.h"
#include "protocol.h"
#include "../tls/io.h"

#define MAX_BUF 1024

#define READDATA(session,buffer,size)                           \
  r = tls_read(session,buffer,size);                            \
  if (r == 0) {                                                 \
    /* client closed connection */                              \
    return;                                                     \
  } else if (r < 0) {                                           \
    /* corrupt data */                                          \
    LOG_ERROR("*** Received corrupted data(%d)\n", r);          \
    return;                                                     \
  }
  
#define WRITEDATA(session,buffer,size)                                \
  p = size;                                                           \
  while ((p > 0) && (r = tls_write(session,buffer+(size-p),p))) {     \
    if (r == 0) {                                                     \
      /* client closed connection */                                  \
      return;                                                         \
    } else if (r < 0) {                                               \
      /* corrupt data */                                              \
      LOG_ERROR("*** error sending data(%d)\n", r);                   \
      return;                                                         \
    }                                                                 \
    p -= r;                                                           \
  }

#define READLINE                                                \
  while (strchr(buffer, '\n') == NULL) {                        \
    READDATA(session, auxbuffer, MAX_BUF);                      \
    if ((bufpos + r) < MAX_BUF) {                               \
      LOG_DEBUG("Read %d,%d bytes.\n", r,bufpos);               \
      memcpy((char*)((int)&buffer + bufpos), auxbuffer, r);     \
      LOG_DEBUG("BUFFER: !!!%s!!!\n", buffer);                  \
      bufpos += r;                                              \
    } else {                                                    \
      LOG_ERROR("Exceeded buffer size. Giving up.\n");          \
      return;                                                   \
    }                                                           \
  }
 
#define CONSUME_LINE                                    \
  scanf_stop = strchr(buffer, '\n');                    \
  remaining = bufpos - (scanf_stop - buffer - 1);       \
  memset(&auxbuffer, 0, MAX_BUF + 1);                   \
  memcpy(&auxbuffer, scanf_stop + 1, remaining);        \
  memset(&buffer, 0, MAX_BUF + 1);                      \
  memcpy(&buffer, auxbuffer, remaining);                \
  bufpos = remaining - 2;                               \
  // 1 for the newline, and 1 for the NULL in the end.

void protocol_start_session(DbXmlSessionData *session) {
  int r, p;

  // we need to leave the last byte for the NULL char
  // noone will set more then MAX_BUF on the string, so we
  // know for sure the string is 0 terminated.
  char buffer[MAX_BUF + 1];
  char auxbuffer[MAX_BUF + 1];
  int bufpos = 0;
  int remaining = 0;
  char* scanf_stop = NULL;
  memset(&buffer, 0, MAX_BUF + 1);
  memset(&auxbuffer, 0, MAX_BUF + 1);

  // first thing to expect is: 
  // SESSION $envname VERSION $version\n
  READLINE;

  if (sscanf(buffer, "SESSION %255s VERSION %f\n",
             session->Session.env_name,
             &(session->Session.client_version)) != 2) {
    LOG_INFO("Client sent bad data (%s)", buffer);
    return;
  }
  
  CONSUME_LINE;
  
  // now we send the server version
  WRITEDATA(session, "SERVER VERSION 0.001\n", 21);

  // now we need to receive the credentials.
  for (;;) {
    READLINE;

    // TODO: receive the credentials.
    
    // after the last credential, we receive a blank line
    if (buffer[0] == '\n') {
      break;
    }

    CONSUME_LINE;
  }
  
}


void protocol_session(DbXmlSessionData *session) {

}
