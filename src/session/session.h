#ifndef SESSION_H
#define SESSION_H

int initialize_session(DbXmlSessionData* session, DbXmlSessionOptions* options, DbXmlSessionOptions** ret);
int free_session(DbXmlSessionData* session);

#endif
