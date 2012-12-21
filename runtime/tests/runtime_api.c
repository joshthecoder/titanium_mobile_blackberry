#include "tests.h"

#include <runtime.h>

// Runtime API tests

TEST(create_object, "Creating an object") {
  rt_object obj = rt_object_new();
  CHECK(obj != NULL);
  TEST_PASS;
}

TEST(property_access, "Accessing an object's properties") {
  rt_object obj = rt_object_new();

  // Verify when requesting a property that
  // is not defined returns the Undefined object.
  rt_value undefined = rt_object_get(obj, "divideByZero");
  CHECK(undefined && rt_value_strict_equals(undefined, rt_undefined()));

  // Verify setting and getting property values works.
  rt_value val_set, val_get;
  val_set = rt_string_new_terminated("bar");
  CHECK(rt_object_set(obj, "foo", val_set));      // set "foo" -> "bar"
  CHECK(val_get = rt_object_get(obj, "foo"));      // get "foo"
  CHECK(rt_value_strict_equals(val_set, val_get)); // and verify it's the same

  TEST_PASS;
}

TEST(has_property, "Testing if an object has a property with key") {
  rt_object obj = rt_object_new();
  rt_object_set(obj, "here", rt_object_new());
  CHECK(!rt_object_has(obj, "shouldnotbehere"));
  CHECK(rt_object_has(obj, "here"));
  TEST_PASS;
}

TEST(delete_property, "Deleting an object property") {
  rt_object obj = rt_object_new();
  rt_object_set(obj, "target", rt_object_new());
  CHECK(rt_object_delete(obj, "target"));
  CHECK(!rt_object_has(obj, "target"));
  TEST_PASS;
}

TEST_GROUP(api) {
  RUN_TEST(create_object);
  RUN_TEST(property_access);
  RUN_TEST(has_property);
  RUN_TEST(delete_property);
}

