#ifndef RUNTIME_TESTS_H
#define RUNTIME_TESTS_H

// A simple unit testing framework.

int run_tests();

#define TEST_PASS return 0
#define TEST_FAIL return 1

#define TEST(name, description) \
  int test_##name()

#define TEST_GROUP(name) \
  void test_group_##name(int* fail_count)

#define RUN_TEST(name) \
  if (test_##name()) (*fail_count)++;

#define RUN_GROUP(name) \
  test_group_##name(&fail_count);

#define CHECK(expr) \
  if (!(expr)) { \
    printf("CHECK: \"%s\" was false.\n", #expr); \
    TEST_FAIL; \
  } \

#endif
