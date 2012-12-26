#ifndef TI_RUNTIME_H
#define TI_RUNTIME_H

// This version is incremented whenever API
// compatibility breaks with extensions. Runtime
// will refuse to load any extension with a non-matching
// API version.
#define RUNTIME_API_VERSION 0x0001

typedef void* rt_value;
typedef rt_value rt_object;
typedef rt_value rt_string;
typedef rt_value rt_number;
typedef rt_object rt_function;

// Called by the embedding application to initialize
// the runtime and evaluate the main entry script.
int rt_start(int argc, char* argv[]);

typedef void (*rt_run_loop)(void);

// Set the run loop function which is called
// once the main entry script has finished evaluating.
void rt_set_run_loop(rt_run_loop loop);

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
  rt_value (*export_func)(void);
};

#define RUNTIME_EXTENSION(name, init_func, export_func) \
  struct runtime_extension extension = { \
    RUNTIME_API_VERSION,                 \
    init_func,                           \
    export_func                          \
  };

// A persistent handle that keeps a runtime object
// alive until the reference count reaches zero.
typedef struct {
  int ref_count;
  void* ptr;
} rt_handle;

void rt_handle_init(rt_handle* handle);

// Check if the two values are strictly equal.
int rt_value_strict_equals(rt_value a, rt_value b);

rt_value rt_undefined();

// Creates a new JavaScript object.
rt_object rt_object_new();

// Sets a property on the object.
int rt_object_set(rt_object obj, const char* key, rt_value val);

// Returns the value of a property on the object.
rt_value rt_object_get(rt_object obj, const char* key);

// Check if the object has a property defined with this key.
int rt_object_has(rt_object obj, const char* key);

// Deletes a property from the object with this key.
int rt_object_delete(rt_object obj, const char* key);

// Creates a new string from either UTF-8 encoded data
// with the length in bytes provided.
rt_string rt_string_new(const char* data, int length);

// Creates a new string from UTF-8 encoded data that is null terminated.
rt_string rt_string_new_terminated(const char* data);

// Returns the number of bytes in the UTF-8 encoded
// representation of the string.
int rt_string_utf8_length(rt_string str);

// Writes the contents of the string encoded in UTF-8
// to an external buffer. Returns the number of bytes written
// including the NULL terminator.
int rt_string_write_utf8(rt_string str, char* buffer, int buffer_size);

// Creates a new JavaScript Number.
rt_number rt_number_new(double val);

typedef struct {
  rt_object self;      // "this" object for the function call.
  int arg_count;       // number of arguments passed to function.

  void* private_data;
} rt_function_info;

typedef rt_value (*rt_function_cb)(rt_function_info* info);

// Creates a new JavaScript function that wraps a C callback function.
rt_function rt_function_new(rt_function_cb cb);

// Returns an argument passed during an invocation of a function.
rt_value rt_function_argv(rt_function_info* info, int index);

#endif
