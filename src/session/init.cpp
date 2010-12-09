extern "C" {
#include "../main/session.h"
#include "../protocol/protocol.h"
#include "session.h"
}

extern "C" int initialize_session(DbXmlSessionData* session, DbXmlSessionOptions* options, DbXmlSessionOptions** ret) {
  // TODO: proper initialization. Just echoes for now.
  *ret = options;
  return 0;
}

extern "C" int free_session(DbXmlSessionData* session) {
  // TODO
  return 0;
}

