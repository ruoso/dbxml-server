extern "C" {
#include "../main/session.h"
#include "../protocol/protocol.h"
#include "session.h"
}
#include "session.hpp"

DbXmlSession::DbXmlSession(DbXmlSessionData* session,
                           DbXmlSessionOptions* options,
                           DbXmlSessionOptions** ret) {
}

int DbXmlSession::command(DbXmlRequest* req, DbXmlResponse* res) {
  return 0;
}
