#include "zygote.h"

#include <stdio.h>

using namespace v8;

// zygote.print([message])
//
// Prints a message to stdout and appends a newline.
static Handle<Value> print(const Arguments& args) {
  HandleScope handleScope;
  puts(args.Length() > 0 ? *String::Utf8Value(args[0]) : "");
  return Undefined();
}

// zygote.runScript(source)
//
// Compile and run the script source in the current
// context returning the resulting value.
static Handle<Value> run_script(const Arguments& args) {
  HandleScope handleScope;

  if (args.Length() < 1) {
    return ThrowException(Exception::Error(String::NewSymbol("No source code provided.")));
  }

  // Catch any exceptions thrown while running script.
  TryCatch try_catch;

  Local<Script> script = Script::Compile(args[0]->ToString());
  if (script.IsEmpty()) {
    return try_catch.ReThrow();
  }

  Local<Value> result = script->Run();
  if (result.IsEmpty()) {
    return try_catch.ReThrow();
  }

  return handleScope.Close(result);
}

Handle<Object> zygote_create() {
  HandleScope handleScope;

  Local<Object> zygote = Object::New();

  zygote->Set(String::NewSymbol("print"), FunctionTemplate::New(print)->GetFunction());
  zygote->Set(String::NewSymbol("runScript"), FunctionTemplate::New(run_script)->GetFunction());

  return handleScope.Close(zygote);
}

