#ifndef TI_RUNTIME_H
#define TI_RUNTIME_H

#include <v8.h>

int runtime_start(int argc, char* argv[]);

// This version is incremented whenever API
// compatibility breaks with extensions. Runtime
// will refuse to load any extension with a non-matching
// API version.
#define RUNTIME_API_VERSION 0x0001

struct runtime_extension {
  // The runtime API version this extension is compatible with.
  // If the version does not match up with the runtime's current
  // version it will fail to load.
  int api_version;

  // Called once when the extension has been loaded.
  // This will allow the extension to perform one time
  // initialization tasks before the first export call.
  int (*init_func)(void);

  // Returns an exports object containing the extension's public APIs.
  // This should cause no side effects and may be called multiple times.
  v8::Handle<v8::Object> (*export_func)(void);
};

#define RUNTIME_EXTENSION(name, init_func, export_func) \
  struct runtime_extension extension = { \
    RUNTIME_API_VERSION,                 \
    init_func,                           \
    export_func                          \
  };

#endif
