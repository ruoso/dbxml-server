#include <string.h>
#include "main.h"
#include "options.h"
#include "../conn/bind.h"
#include "../tls/init.h"

int main(int argc, char* argv[]) {

  // first thing is parsing the options
  DbXmlServerOptions options;
  memset(&options, 0, sizeof(DbXmlServerOptions));
  parse_options(argc, argv, &options);

  // TODO: initialize authentication

  // Initialize TLS.
  tls_init(&options);

  // delegate to the connection module
  bind_server(&options);

  tls_deinit(&options);

  free_options(&options);

  return 0;
}

