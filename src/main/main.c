#include "main.h"
#include "options.h"
#include "../conn/bind.h"

int main(int argc, char* argv[]) {

  // first thing is parsing the options
  DbXmlServerOptions options;
  parse_options(argc, argv, &options);

  // TODO: initialize authentication

  // delegate to the connection module
  bind_server(&options);

  return 1;
}

