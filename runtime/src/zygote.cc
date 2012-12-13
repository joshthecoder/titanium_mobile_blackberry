#include "zygote.h"

#include <stdio.h>
#include <stdlib.h>

#include "os.h"

using namespace v8;

// $.print([message])
//
// Prints a message to stdout and appends a newline.
static Handle<Value> print(const Arguments& args) {
  HandleScope handleScope;
  puts(args.Length() > 0 ? *String::Utf8Value(args[0]) : "");
  return Undefined();
}

// $.runScript(source, [filename])
//
// Compile and run the script source in the current
// context returning the resulting value.
static Handle<Value> run_script(const Arguments& args) {
  HandleScope handleScope;

  if (args.Length() < 1) {
    return ThrowException(Exception::Error(String::NewSymbol("No source code provided.")));
  }

  Local<String> source, filename;
  source = args[0]->ToString(),
  filename = args.Length() > 1 ? args[1]->ToString() : String::New("<script>");

  // Catch any exceptions thrown while running script.
  TryCatch try_catch;

  Local<Script> script = Script::Compile(source, filename);
  if (script.IsEmpty()) {
    return try_catch.ReThrow();
  }

  Local<Value> result = script->Run();
  if (result.IsEmpty()) {
    return try_catch.ReThrow();
  }

  return handleScope.Close(result);
}

// $.readFile(path)
//
// Reads in the entire contents of a file.
static Handle<Value> read_file(const Arguments& args) {
  HandleScope handle_scope;

  if (args.Length() < 1) {
    return ThrowException(Exception::Error(String::New("Missing file path.")));
  }

  String::Utf8Value path(args[0]);

  int fd = os_fs_open(*path, OS_FS_RDONLY);
  if (fd == -1) {
    return ThrowException(Exception::Error(String::New("Failed to open file.")));
  }

  struct os_fs_info file_info;
  if (os_fs_get_info(fd, &file_info)) {
    return ThrowException(Exception::Error(String::New("Failed to get file info.")));
  }

  printf("file size is %i\n", file_info.size);
  if (file_info.size <= 0) {
    // If file has no content, return an empty string.
    return handle_scope.Close(String::New(""));
  }

  void* data = malloc(file_info.size);
  int length = os_fs_read(fd, data, 0, file_info.size);
  os_fs_close(fd);
  if (length == -1) {
    return ThrowException(Exception::Error(String::New("Error reading file content.")));
  }

  // TODO(josh): return a proper Buffer type to better handle binary data.
  Local<String> buffer = String::New((char*)data, length);
  free(data);

  return handle_scope.Close(buffer);
}

Handle<Object> zygote_create() {
  HandleScope handleScope;

  Local<Object> zygote = Object::New();

  zygote->Set(String::NewSymbol("print"), FunctionTemplate::New(print)->GetFunction());
  zygote->Set(String::NewSymbol("runScript"), FunctionTemplate::New(run_script)->GetFunction());
  zygote->Set(String::NewSymbol("readFile"), FunctionTemplate::New(read_file)->GetFunction());

  return handleScope.Close(zygote);
}

