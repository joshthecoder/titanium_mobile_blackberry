#include <runtime.h>

#include "tests.h"

static int fail_count = 0;

void start_tests() {
  fail_count = run_tests();
  if (fail_count) {
    printf("--- %i tests failed ---\n", fail_count);
  } else {
    printf("--- All tests passed ---\n");
  }
}

int main(int argc, char* argv[]) {
  rt_set_run_loop(start_tests);
  return rt_start(argc, argv) || fail_count > 0;
}

