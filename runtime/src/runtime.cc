#include "runtime.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <v8.h>

#include "flags.h"
#include "zygote.h"

using namespace titanium;
using namespace v8;

static Local<Script> load_script(const char* path) {
  HandleScope handleScope;

  int fd = open(path, O_RDONLY);
  if (fd == -1) {
    return Local<Script>();
  }

  struct stat fileStats;
  fstat(fd, &fileStats);

  int length = fileStats.st_size;
  char* data = new char[length];
  read(fd, data, length);

  close(fd);

  Local<String> source = String::New(data, length);
  return handleScope.Close(Script::Compile(source));
}

static rt_run_loop run_loop = NULL;

void rt_set_run_loop(rt_run_loop loop) {
  run_loop = loop;
}

int rt_start(int argc, char* argv[]) {
  Flags::setFromCommandLine(argc, argv);

  // Set any V8 flags from command line before initializing.
  const char* flags = Flags::current().v8Flags();
  if (flags != 0) {
    V8::SetFlagsFromString(flags, strlen(flags));
  }
  V8::Initialize();

  // Create the main context and handle scope
  // before creating any V8 objects.
  Persistent<Context> context = Context::New();
  Context::Scope context_scope(context);
  HandleScope handle_scope;

  Local<Script> bootScript = load_script("zygote.js");
  if (bootScript.IsEmpty()) {
    printf("Unable to load boot script.\n");
    exit(1);
  }

  Local<Function> bootFunc = Local<Function>::Cast(bootScript->Run());
  Handle<Value> args[] = { zygote_create() };
  context->Global()->Set(String::NewSymbol("global"), context->Global());
  bootFunc->Call(context->Global(), 1, args);

  if (run_loop) {
    run_loop();
  }

  return 0;
}

