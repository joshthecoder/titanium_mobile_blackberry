#include "tests.h"

#include <stdio.h>

#include <v8.h>

using namespace v8;

int main(int argc, char* argv[]) {
  // --- Setup V8 ---
  Persistent<Context> ctx = Context::New();
  Context::Scope ctx_scope(ctx);
  HandleScope handle_scope;

  int fail_count = run_tests();
  if (fail_count) {
    printf("--- %i tests failed ---\n", fail_count);
  } else {
    printf("--- All tests passed ---\n");
  }

  return fail_count > 0 ? 1 : 0;
}

