extern "C" {
#include "../main/session.h"
#include "../protocol/protocol.h"
#include "session.h"
}
#include "session.hpp"

int session_request(DbXmlSessionData* session, DbXmlRequest* req, DbXmlResponse* res) {
  try {
    DbXmlSession* s = (DbXmlSession*)(session->Session.priv_data);
    s->command(req, res);
    return 0;
  } catch (...) {
    return -1;
  }
}
