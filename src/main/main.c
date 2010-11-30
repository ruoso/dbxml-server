#include "main.h"
#include "options.h"

int main(int argc, char* argv[]) {

  // first thing is parsing the options
  DbXmlServerOptions options;
  parse_options(argc, argv, &options);

  // TODO: initialize authentication

  

  return 1;
}

