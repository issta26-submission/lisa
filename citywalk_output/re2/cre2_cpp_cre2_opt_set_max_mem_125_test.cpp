/*
  Unit test suite for cre2_opt_set_max_mem in cre2.cpp (cre2_options_t related API)

  Notes:
  - The tests exercise the focal method by using the public API:
      - cre2_opt_new()
      - cre2_opt_set_max_mem(cre2_options_t*, int64_t)
      - cre2_opt_max_mem(cre2_options_t*)
      - cre2_opt_delete(cre2_options_t*)
  - The test harness is a lightweight C++11 test runner (no GTest) that uses non-terminating
    assertions to maximize code execution and coverage.
  - This test intentionally avoids private members and relies solely on the public API.
  - The tests include multiple scenarios:
      - Setting and retrieving zero, positive, and negative values.
      - Independent state for two separate options objects.
  - The code assumes creatable via cre2.h declarations (part of the project).
*/

#include <cre2.h>
#include <cerrno>
#include <re2/re2.h>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <cstdio>
#include <re2/set.h>


// Lightweight test harness
static int g_total_asserts = 0;
static int g_failed_asserts = 0;

#define ASSERT_TRUE(cond) \
  do { \
    ++g_total_asserts; \
    if (!(cond)) { \
      fprintf(stderr, "[FAIL] %s:%d: Assertion failed: %s\n", __FILE__, __LINE__, #cond); \
      ++g_failed_asserts; \
    } \
  } while (0)

#define ASSERT_EQ(expected, actual) \
  do { \
    ++g_total_asserts; \
    if (static_cast<int64_t>(-(0LL) + (expected)) != static_cast<int64_t>(-(0LL) + (actual))) { \
      fprintf(stderr, "[FAIL] %s:%d: Assertion failed: Expected %lld, got %lld (for %s)\n", __FILE__, __LINE__, (long long)(expected), (long long)(actual), #actual); \
      ++g_failed_asserts; \
    } \
  } while (0)

#define ASSERT_PTR_NOT_NULL(ptr) \
  do { \
    ++g_total_asserts; \
    if ((ptr) == nullptr) { \
      fprintf(stderr, "[FAIL] %s:%d: Assertion failed: %s is NULL\n", __FILE__, __LINE__, #ptr); \
      ++g_failed_asserts; \
    } \
  } while (0)


// Test 1: Basic setting and getting of max_mem on a single cre2_options_t
// Verifies that after setting a value, querying max_mem returns the same value.
void test_cre2_opt_set_max_mem_basic() {
  // Create a new options object
  cre2_options_t *opt = cre2_opt_new();
  ASSERT_PTR_NOT_NULL(opt);

  // Case 1: Set zero and verify
  cre2_opt_set_max_mem(opt, 0);
  int64_t v0 = cre2_opt_max_mem(opt);
  ASSERT_EQ((int64_t)0, v0);

  // Case 2: Set a positive value and verify
  const int64_t pos = 1234567890123LL;
  cre2_opt_set_max_mem(opt, pos);
  int64_t v1 = cre2_opt_max_mem(opt);
  ASSERT_EQ(pos, v1);

  // Case 3: Set a negative value and verify
  cre2_opt_set_max_mem(opt, -1);
  int64_t v2 = cre2_opt_max_mem(opt);
  ASSERT_EQ((int64_t)-1, v2);

  // Clean up
  cre2_opt_delete(opt);
}

// Test 2: Independence between two distinct cre2_options_t instances
// Ensures that setting max_mem on one option does not affect the other.
void test_cre2_opt_set_max_mem_independence() {
  cre2_options_t *opt_a = cre2_opt_new();
  cre2_options_t *opt_b = cre2_opt_new();

  ASSERT_PTR_NOT_NULL(opt_a);
  ASSERT_PTR_NOT_NULL(opt_b);

  // Set different values
  cre2_opt_set_max_mem(opt_a, 10);
  cre2_opt_set_max_mem(opt_b, 99999);

  int64_t a_val = cre2_opt_max_mem(opt_a);
  int64_t b_val = cre2_opt_max_mem(opt_b);

  ASSERT_EQ((int64_t)10, a_val);
  ASSERT_EQ((int64_t)99999, b_val);

  // Cleanup
  cre2_opt_delete(opt_a);
  cre2_opt_delete(opt_b);
}

// Test 3: Boundary and edge-case values
// Verifies behavior with extreme int64_t values (where safe and meaningful in the API).
void test_cre2_opt_set_max_mem_extremes() {
  cre2_options_t *opt = cre2_opt_new();
  ASSERT_PTR_NOT_NULL(opt);

  // INT64_MIN
  cre2_opt_set_max_mem(opt, (int64_t)INT64_MIN);
  int64_t minVal = cre2_opt_max_mem(opt);
  ASSERT_EQ((int64_t)INT64_MIN, minVal);

  // INT64_MAX (be cautious if the API has any overflow constraints; we assume it's accepted)
  cre2_opt_set_max_mem(opt, (int64_t)INT64_MAX);
  int64_t maxVal = cre2_opt_max_mem(opt);
  ASSERT_EQ((int64_t)INT64_MAX, maxVal);

  cre2_opt_delete(opt);
}

// Entry-point for running tests
int main() {
  fprintf(stderr, "Starting tests for cre2_opt_set_max_mem...\n");

  // Run tests
  test_cre2_opt_set_max_mem_basic();
  test_cre2_opt_set_max_mem_independence();
  test_cre2_opt_set_max_mem_extremes();

  // Summary
  if (g_failed_asserts == 0) {
    printf("ALL TESTS PASSED. Assertions: %d\n", g_total_asserts);
    return 0;
  } else {
    fprintf(stderr, "TESTS COMPLETED with FAILURES. Passed: %d, Failed: %d, Total: %d\n",
            g_total_asserts - g_failed_asserts, g_failed_asserts, g_total_asserts);
    return 1;
  }
}