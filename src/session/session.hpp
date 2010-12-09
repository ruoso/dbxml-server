#include "DbXml.hpp"
extern "C" {
#include "../main/session.h"
#include "../protocol/protocol.h"
}

class DbXmlSession {
  DB_ENV* env;
  DbXml::XmlManager* manager;
  DbXmlSessionData* session;
  DbXmlSessionOptions options;
public:
  DbXmlSession(DbXmlSessionData* session,
               DbXmlSessionOptions* options,
               DbXmlSessionOptions** ret);
  ~DbXmlSession();
  int command(DbXmlRequest* req, DbXmlResponse* res);
};
