extern "C" {
#include "../main/session.h"
#include "../protocol/protocol.h"
#include "session.h"
}
#include "session.hpp"

int session_request(DbXmlSessionData* session, DbXmlRequest* req, DbXmlResponse* res) {
  try {
    DbXmlSession* s = (DbXmlSession*)(session->Session.priv_data);
    return s->command(req, res);
  } catch (...) {
    LOG_ERROR("Failed to run the command.\n");
    return -1;
  }
}
