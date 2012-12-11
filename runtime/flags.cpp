#include "flags.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace titanium {

Flags Flags::flags;

static void printHelp() {
  printf("Usage: %s [flags] main.js\n\n", Flags::current().processName());

  printf("Options:\n"
         "  --v8-flags        print v8 command line options\n"
         "  --v8-flags=flags  set v8 flags\n"
         "\n");
}

void Flags::setFromCommandLine(int argc, char* argv[]) {
  flags.processName_ = argv[0];

  for (int i = 1; i < argc; i++) {
    const char* arg = argv[i];
    if (strcmp(arg, "--help") == 0) {
      printHelp();
      exit(0);
    } else if (strstr(arg, "--v8-flags") == arg) {
      const char* p = strchr(arg, '=');
      if (p == NULL) {
        // Have V8 print help text if no flags are provided.
        flags.v8Flags_ = const_cast<char*>("--help");
        continue;
      }
      flags.v8Flags_ = 1 + p;
    } else if (arg[0] != '-') {
      // First non-flag argument is the path of the main script.
      flags.mainScriptPath_ = arg;
    }
  }
}

void Flags::resetAll() {
  flags = Flags();
}

} // namespace titanium

