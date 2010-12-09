extern "C" {
#include "../main/session.h"
#include "../protocol/protocol.h"
#include "session.h"
#include "string.h"
}
#include "session.hpp"

DbXmlSession::DbXmlSession(DbXmlSessionData* s,
                           DbXmlSessionOptions* o,
                           DbXmlSessionOptions** r) {
  session = s;
  memset(&options, 0, sizeof(DbXmlSessionOptions));
  *r = &options;
  
  // first thing we need to do is to build the final env dir.  we will
  // concat the server home dir with the name of the env sent by the
  // client. We should also check if the env name is sane.
  char safeenvdir[255] = "";
  if (sscanf(session->Session.env_name, "%254[A-Za-z]", safeenvdir) != 1) {
    LOG_ERROR("env dir invalid: %s.\n", session->Session.env_name);
    throw "bad env dir";
  }

  char envdir[255] = "";
  snprintf(envdir, 254, "%s/%s",
           session->options->Main.server_home,
           safeenvdir);

  // now we need to parse any options we support.
  // for this moment we don't support any.

  int dberr;
  // initialize the db environment
  dberr = db_env_create(&env, 0);
  if (dberr) {
    LOG_ERROR("Failed to create the env: %s.\n", db_strerror(dberr));
    throw dberr;
  }

  dberr = env->open(env, envdir,
                    DB_INIT_TXN | DB_INIT_MPOOL, 0);
  if (dberr) {
    LOG_ERROR("Failed to open the env %s: %s.\n", envdir, db_strerror(dberr));
    throw dberr;
  }

  // initialize the dbxml manager
  manager = new DbXml::XmlManager(env, DbXml::DBXML_ALLOW_AUTO_OPEN);

}

DbXmlSession::~DbXmlSession() {
  // free session data.
  if (env)
    env->close(env, 0);
  if (manager)
    delete manager;
}

int DbXmlSession::command(DbXmlRequest* req, DbXmlResponse* res) {
  return 0;
}

