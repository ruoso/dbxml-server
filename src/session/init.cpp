extern "C" {
#include "../main/session.h"
#include "../protocol/protocol.h"
#include "session.h"
}

#include "session.hpp"

int initialize_session(DbXmlSessionData* session,
                       DbXmlSessionOptions* options,
                       DbXmlSessionOptions** ret) {
  try {
    DbXmlSession* s = new DbXmlSession(session, options, ret);
    session->Session.priv_data = s;
    return 0;
  } catch (...) {
    return -1;
  }
}

int free_session(DbXmlSessionData* session) {
  try {
    delete (DbXmlSession*)(session->Session.priv_data);
    return 0;
  } catch (...) {
    return -1;
  }
}

