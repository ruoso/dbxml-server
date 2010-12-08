#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include "../main/session.h"
#include "protocol.h"
#include "../tls/io.h"

#define MAX_BUF 1023

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
  
#define WRITEDATA(session,buffer,size)                          \
  p = size;                                                     \
  while ((p > 0) &&                                             \
         (r = tls_write(session,buffer+(size-p),p))) {          \
    if (r == 0) {                                               \
      /* client closed connection */                            \
      return;                                                   \
    } else if (r < 0) {                                         \
      /* corrupt data */                                        \
      LOG_ERROR("*** error sending data(%d)\n", r);             \
      return;                                                   \
    }                                                           \
    p -= r;                                                     \
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
  }                                                             \
  scanf_stop = strchr(buffer, '\n');                            \
  remaining = bufpos - (scanf_stop - buffer);                   \
  memset(&line, 0, MAX_BUF + 1);                                \
  memcpy(&line, buffer, bufpos - remaining);                    \
  memset(&auxbuffer, 0, MAX_BUF + 1);                           \
  memcpy(&auxbuffer, scanf_stop + 1, remaining);                \
  memset(&buffer, 0, MAX_BUF + 1);                              \
  linesize = bufpos - remaining;                                \
  LOG_DEBUG("LINE: (%d) !!!%s!!!\n", linesize, line);           \
  bufpos = 0;

#define PARSEHEADER                                             \
  memset(header, 0, sizeof(DbXmlHeader));                       \
  scanf_stop = strchr(line, '=');                               \
  if (scanf_stop == NULL) {                                     \
    LOG_ERROR("received corrupt header: !!!%s!!!", line);       \
    return;                                                     \
  }                                                             \
  int keysize = scanf_stop - line;                              \
  int valsize = linesize - keysize - 1;                         \
  if (keysize >= MAX_TOKEN_SIZE ||                              \
      valsize >= MAX_TOKEN_SIZE) {                              \
    LOG_ERROR("header key or value too long.");                 \
    return;                                                     \
  }                                                             \
  LOG_DEBUG("HEADER SIZES: %d = 1 + %d + %d\n",                 \
            linesize, keysize, valsize)                         \
  memcpy(&(header->key), line, keysize);                        \
  LOG_DEBUG("HEADER: KEY ``%s''\n",                             \
            header->key);                                       \
  memcpy(&(header->val), line + keysize + 1, valsize);          \
  LOG_DEBUG("HEADER: VAL ``%s''\n",                             \
            header->val);

void protocol_start_session(DbXmlSessionData *session) {
  int r, p;

  // we need to leave the last byte for the NULL char
  // noone will set more then MAX_BUF on the string, so we
  // know for sure the string is 0 terminated.
  char line[MAX_BUF + 1];
  char buffer[MAX_BUF + 1];
  char auxbuffer[MAX_BUF + 1];
  int bufpos = 0;
  int linesize = 0;
  int remaining = 0;
  int header_count = 0;
  char* scanf_stop = NULL;
  memset(&line, 0, MAX_BUF + 1);
  memset(&buffer, 0, MAX_BUF + 1);
  memset(&auxbuffer, 0, MAX_BUF + 1);

  // first thing to expect is: 
  // SESSION $envname VERSION $version\n
  READLINE;
  if (sscanf(line, "SESSION %255s VERSION %f",
             session->Session.env_name,
             &(session->Session.client_version)) != 2) {
    LOG_INFO("Client sent bad data (%s)\n", line);
    return;
  }
  
  // now we send the server version
  WRITEDATA(session, "SERVER VERSION 0.001\n", 21);

  // now we need to receive the credentials.
  DbXmlCredentials* cred = malloc(sizeof(DbXmlCredentials));
  memset(cred, 0, sizeof(DbXmlCredentials));
  header_count = 0;
  for (;;) {
    READLINE;
    // after the last credential, we receive a blank line
    if (line[0] == 0)
      break;

    // refuse more then MAX_HEADERS headers.
    if (header_count == MAX_HEADERS) {
      LOG_ERROR("received too many credentials");
      return;
    }

    // receive the credentials.
    DbXmlHeader* header = malloc(sizeof(DbXmlHeader));
    if (header == NULL) {
      LOG_ERROR("Failed to allocate header memory.\n");
      return;
    }
      
    PARSEHEADER;
    
    cred->headers[header_count++] = header;
  }

  // TODO: check if this environment exists and authenticate
  WRITEDATA(session, "SESSION STARTING\n", 17);

  // expect for commands
  for (;;) {
    READLINE;
    
    
  }

}
