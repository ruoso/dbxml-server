#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include "../main/session.h"
#include "protocol.h"
#include "../tls/io.h"

#define MAX_BUF 1023

#define WRITEDATAVARS                                           \
  int r, p;                                                     \
  // needed by WRITEDATA  

#define WRITEDATA(session,buffer,size)                          \
  p = size;                                                     \
  while ((p > 0) &&                                             \
         (r = tls_write(session,buffer+(size-p),p))) {          \
    if (r == 0) {                                               \
      /* client closed connection */                            \
      return -1;                                                \
    } else if (r < 0) {                                         \
      /* corrupt data */                                        \
      LOG_ERROR("*** error sending data(%d)\n", r);             \
      return -1;                                                \
    }                                                           \
    p -= r;                                                     \
  }

#define READDATAVARS                                            \
  int rd;

#define READDATA(session,buffer,size)                           \
  rd = tls_read(session,buffer,size);                           \
  if (rd == 0) {                                                \
    /* client closed connection */                              \
    return -1;                                                  \
  } else if (rd < 0) {                                          \
    /* corrupt data */                                          \
    LOG_ERROR("*** Received corrupted data(%d)\n", rd);         \
    return -1;                                                  \
  }

#define READLINEVARS                                            \
  READDATAVARS;                                                 \
  char buffer[MAX_BUF + 1];                                     \
  char line[MAX_BUF + 1];                                       \
  char auxbuffer[MAX_BUF + 1];                                  \
  int bufpos = 0;                                               \
  int linesize = 0;                                             \
  int remaining = 0;                                            \
  char* scanf_stop = NULL;                                      \
  memset(&line, 0, MAX_BUF + 1);                                \
  memset(&buffer, 0, MAX_BUF + 1);                              \
  memset(&auxbuffer, 0, MAX_BUF + 1);

#define READLINE                                                \
  while (strchr(buffer, '\n') == NULL) {                        \
    READDATA(session, auxbuffer, MAX_BUF);                      \
    if ((bufpos + rd) < MAX_BUF) {                              \
      LOG_DEBUG("Read %d,%d bytes.\n", rd, bufpos);             \
      memcpy((char*)((int)&buffer + bufpos), auxbuffer, rd);    \
      LOG_DEBUG("BUFFER: !!!%s!!!\n", buffer);                  \
      bufpos += rd;                                             \
    } else {                                                    \
      LOG_ERROR("Exceeded buffer size. Giving up.\n");          \
      return -1;                                                \
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

#define PARSEHEADERVARS                                         \
  int keysize;                                                  \
  int valsize;

#define PARSEHEADER                                             \
  memset(header, 0, sizeof(DbXmlHeader));                       \
  scanf_stop = strchr(line, '=');                               \
  if (scanf_stop == NULL) {                                     \
    LOG_ERROR("received corrupt header: !!!%s!!!", line);       \
    return -1;                                                  \
  }                                                             \
  keysize = scanf_stop - line;                                  \
  valsize = linesize - keysize - 1;                             \
  if (keysize >= MAX_TOKEN_SIZE ||                              \
      valsize >= MAX_TOKEN_SIZE) {                              \
    LOG_ERROR("header key or value too long.");                 \
    return -1;                                                  \
  }                                                             \
  LOG_DEBUG("HEADER SIZES: %d = 1 + %d + %d\n",                 \
            linesize, keysize, valsize)                         \
  memcpy(&(header->key), line, keysize);                        \
  LOG_DEBUG("HEADER: KEY ``%s''\n",                             \
            header->key);                                       \
  memcpy(&(header->val), line + keysize + 1, valsize);          \
  LOG_DEBUG("HEADER: VAL ``%s''\n",                             \
            header->val);
