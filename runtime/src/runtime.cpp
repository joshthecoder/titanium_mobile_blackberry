#include "runtime.h"

#include <string.h>

#include <v8.h>

#include "flags.h"

using namespace v8;

namespace titanium {

static void initializeV8() {
  // Set any V8 flags provided in the general runtime flags.
  // It is important we do this before initializing V8
  // since some flags can only be set before initialization.
  const char* flags = Flags::current().v8Flags();
  if (flags != 0) {
    V8::SetFlagsFromString(flags, strlen(flags));
  }

  V8::Initialize();
}

int Runtime::start(int argc, char* argv[]) {
  // Parse any command line arguments.
  Flags::setFromCommandLine(argc, argv);

  initializeV8();

  return 0;
}

} // namespace titanium

