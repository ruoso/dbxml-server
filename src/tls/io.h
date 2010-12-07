#ifndef TLS_IO_H
#define TLS_IO_H

int tls_read(DbXmlSessionData *session, char* buffer, int buffer_size);
int tls_write(DbXmlSessionData *session, char* buffer, int length);

#endif
