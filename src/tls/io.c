#include <gnutls/gnutls.h>
#include "../main/session.h"
#include "io.h"

int tls_read(DbXmlSessionData *session, char* buffer, int buffer_size) {
  return gnutls_record_recv(session->Encryption.session, buffer, buffer_size);
}

int tls_write(DbXmlSessionData *session, char* buffer, int length) {
  return gnutls_record_send(session->Encryption.session, buffer, length);
}
