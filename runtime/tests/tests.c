#include "tests.h"

#include "runtime_api.c"

int run_tests() {
  int fail_count = 0;

  RUN_GROUP(api);

  return fail_count;
}

