#include "runtime.h"

#include <v8.h>

using namespace v8;

int rt_value_strict_equals(rt_value a, rt_value b) {
  return ((Value*) a)->StrictEquals(Handle<Value>((Value*) b));
}

rt_value rt_undefined() {
  return (rt_value) *Undefined();
}

rt_object rt_object_new() {
  return (rt_object) *Object::New();
}

int rt_object_set(rt_object obj, const char* key, rt_value val) {
  HandleScope handle_scope;
  Local<String> key_symbol = String::NewSymbol(key);
  Handle<Value> val_handle((Value*) val);
  return ((Object*) obj)->Set(key_symbol, val_handle);
}

rt_value rt_object_get(rt_object obj, const char* key) {
  HandleScope handle_scope;
  Local<String> key_symbol = String::NewSymbol(key);
  Local<Value> val = ((Object*) obj)->Get(key_symbol);
  return (rt_value) *handle_scope.Close(val);
}

int rt_object_has(rt_object obj, const char* key) {
  HandleScope handle_scope;
  return ((Object*) obj)->Has(String::NewSymbol(key));
}

int rt_object_delete(rt_object obj, const char* key) {
  HandleScope handle_scope;
  return ((Object*) obj)->Delete(String::NewSymbol(key));
}

rt_string rt_string_new(const char* data, int length) {
  return (rt_string) *String::New(data, length);
}

rt_string rt_string_new_terminated(const char* str) {
  return (rt_string) *String::New(str);
}

int rt_string_length(rt_string str) {
  return ((String*) str)->Utf8Length();
}

int rt_string_write_utf8(rt_string str, char* buffer, int buffer_size) {
  return ((String*) str)->WriteUtf8(buffer, buffer_size);
}

rt_number rt_number_new(double val) {
  return (rt_number)*Number::New(val);
}

static Handle<Value> invocation_cb(const Arguments& args) {
  HandleScope handle_scope;
  rt_function_cb cb = (rt_function_cb) External::Unwrap(args.Data());
  rt_function_info info = {
    (rt_object) *args.This(),
    args.Length(),
    const_cast<Arguments*>(&args)
  };
  Handle<Value> result((Value*) cb(&info));
  return handle_scope.Close(result);
}

rt_function rt_function_new(rt_function_cb cb) {
  HandleScope handle_scope;
  Local<FunctionTemplate> func_template = FunctionTemplate::New(
    invocation_cb,
    External::Wrap((void*)cb));
  return (rt_function) *handle_scope.Close(func_template->GetFunction());
}

