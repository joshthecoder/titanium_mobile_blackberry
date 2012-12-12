#include "zygote.h"

#include <stdio.h>

using namespace v8;

// print(message)
static Handle<Value> print(const Arguments& args) {
  HandleScope handleScope;

  if (args.Length() < 1) {
    Local<Value> e = Exception::Error(
        String::New("print(message)\n    'message' parameter is required."));
    return ThrowException(e);
  }

  printf("%s", *String::Utf8Value(args[0]));
  return Undefined();
}

Handle<Object> zygote_create() {
  HandleScope handleScope;

  Local<Object> zygote = Object::New();

  zygote->Set(String::NewSymbol("print"), FunctionTemplate::New(print)->GetFunction());

  return handleScope.Close(zygote);
}

