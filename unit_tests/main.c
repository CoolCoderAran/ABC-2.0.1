#define TEST_FRAMEWORK_IMPLEMENTATION
#include "framework.h"

#include "b.h"
/* Add rest of the includes here: */
#include "i1btr.h"

static void add_all_tests(struct TestArray *ta);

int main() {
  struct TestArray ta = {0};

  add_all_tests(&ta);

  return run_tests(ta);
}

/*
 * All unit tests must have `bool f()` signature.
 *
 * They return true when they pass, false otherwise.
 * Use the ASSERT macro to check a condition, then return false if it fails.
 */
static bool test_case1() {
  ASSERT(42 == 42);
  return true;
}
static bool test_case2() {
  ASSERT(42 == 67);
  return true;
}

static void add_all_tests(struct TestArray *ta) {
  ADD_TEST(*ta, test_case1, "Foo");
  ADD_TEST(*ta, test_case2, "Bar");
}
