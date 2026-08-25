/******************************************************************************
 *                           TESTING FRAMEWORK                                *
 *                                                                            *
 *        A header-only minimalist testing framework written in C.            *
 *                                                                            *
 *  Include the implementation either by compiling this header file with      *
 *      cc -xc -DTEST_FRAMEWORK_IMPLEMENTATION framework.h                    *
 *  or by putting the following definition before including this file         *
 *      #define TEST_FRAMEWORK_IMPLEMENTATION                                 *
 *      #include "framework.h"                                                *
 ******************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * A dynamic array of tests (`struct Test`).
 */
struct TestArray {
  size_t capacity;
  size_t size;
	struct Test *buf;
};

struct Test {
	[[nodiscard]]
	bool (*run)();
	char *name;
};

[[nodiscard]]
int run_tests(struct TestArray ta);

/* Dynamic Array Push - pushes a new element into a dynamic array */
#define DA_PUSH(arr, x)                                                        \
  do {                                                                         \
    if ((arr).capacity == 0) {                                                 \
      (arr).capacity = 256;                                                    \
      (arr).buf = calloc((arr).capacity, sizeof((arr).buf[0]));                \
    }                                                                          \
    if ((arr).size == (arr).capacity) {                                        \
      (arr).capacity *= 2;                                                     \
      (arr).buf = realloc((arr).buf, (arr).capacity * sizeof((arr).buf[0]));   \
    }                                                                          \
    (arr).buf[(arr).size++] = (x);                                             \
  } while (0)

#define M_COMMA ,
#define ADD_TEST(arr, run, name) \
	DA_PUSH((arr), (struct Test){(run) M_COMMA (name)})

#define ASSERT(cond)                                                           \
  do {                                                                         \
    if (!(cond)) {                                                             \
      fprintf(stderr, "ASSERT(%s) failed.\n", #cond);                          \
      return false;                                                            \
    }                                                                          \
  } while (0)

// #define TEST_FRAMEWORK_IMPLEMENTATION

#ifdef TEST_FRAMEWORK_IMPLEMENTATION

int run_tests(struct TestArray ta) {
	size_t failing = 0;

  for (size_t i = 0; i < ta.size; i++) {
    printf("Running test %ld/%ld: '%s'\n", i + 1, ta.size, ta.buf[i].name);
    bool ok = ta.buf[i].run();
		if (!ok) {
			failing++;
			printf("Test '%s' failed.\n", ta.buf[i].name);
		}
  }
  printf("\n");
	if (failing != 0) {
		printf("%ld test(s) failing.\n", failing);
		return 1;
	}

	return 0;
}

#endif /* Implementation */
