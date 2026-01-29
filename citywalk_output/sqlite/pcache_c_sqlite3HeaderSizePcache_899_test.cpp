/*
  Unit test suite for sqlite3HeaderSizePcache() located in pcache.c

  This test suite is written in C++11 (no GoogleTest) and uses a minimal
  assertion framework to maximize code coverage while allowing test cases
  to continue on failures (non-terminating assertions).

  Notes:
  - We rely on the real SQLite internals to be available in the build (i.e.,
    sqliteInt.h must define PgHdr and ROUND8 as in the production pcache.c).
  - The tests focus on validating that sqlite3HeaderSizePcache() returns
    ROUND8(sizeof(PgHdr)) by comparing against a runtime calculation of the
    expected rounded size.
  - Static helper functions within pcache.c are not accessible; we test only
    the focal function as required.
  - Test output includes explicit comments for maintainability and traceability.
*/

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


extern "C" {
  // Import the necessary SQLite types and declarations. This header provides
  // PgHdr and the ROUND8 macro used by sqlite3HeaderSizePcache().
  #include "sqliteInt.h"
}

// Simple non-terminating test assertion macros
static int g_test_failures = 0;

// Log a test failure without aborting test execution
#define TEST_EXPECT_EQ(a, b) \
  do { \
    auto _a = (a); \
    auto _b = (b); \
    if (static_cast<long long>(_a) != static_cast<long long>(_b)) { \
      std::fprintf(stderr, "TEST_FAIL: %s != %s (actual: %lld, expected: %lld) at %s:%d\n", \
                   #a, #b, (long long)_a, (long long)_b, __FILE__, __LINE__); \
      ++g_test_failures; \
    } \
  } while (0)

#define TEST_EXPECT_TRUE(x) TEST_EXPECT_EQ((x), true)

// Candidate Keywords (Step 1 context)
// - PgHdr, ROUND8, sizeof(PgHdr), sqlite3HeaderSizePcache, pcache, PCache
// - The test focuses on the behavior of sqlite3HeaderSizePcache() which computes
//   ROUND8(sizeof(PgHdr)) for the header size of a cache page header.


// Test 1: Verify sqlite3HeaderSizePcache() equals ROUND8(sizeof(PgHdr))
// This asserts the core contract of the focal method using a runtime calculation
// of the expected rounded size.
static void test_sqlite3HeaderSizePcache_matches_round8() {
  int actual = sqlite3HeaderSizePcache();
  // Expected: align sizeof(PgHdr) up to the next multiple of 8
  // Using a portable bitwise rounding: (size + 7) & ~7
  int expected_round8 = ( ((int)sizeof(PgHdr) + 7) & ~7 );
  TEST_EXPECT_EQ(actual, expected_round8);
  // Also ensure the result is a multiple of 8 (sanity check)
  TEST_EXPECT_TRUE((actual % 8) == 0);
  // Local explanatory note:
  // If the implementation of ROUND8 changes, this test still captures the
  // observable contract: the returned value must equal the size of PgHdr
  // paginated to an 8-byte boundary.
}

// Test 2: Repeated calls produce a consistent result (idempotence for the size formula)
static void test_sqlite3HeaderSizePcache_consistent_across_calls() {
  int first  = sqlite3HeaderSizePcache();
  int second = sqlite3HeaderSizePcache();
  TEST_EXPECT_EQ(first, second);
}

// Test 3: The header size should be a valid multiple of 8 (sanity check)
static void test_sqlite3HeaderSizePcache_is_aligned() {
  int size = sqlite3HeaderSizePcache();
  TEST_EXPECT_EQ((size % 8), 0);
}

// Simple test runner
static void run_all_tests() {
  std::fprintf(stdout, "Running tests for sqlite3HeaderSizePcache...\n");

  test_sqlite3HeaderSizePcache_matches_round8();
  test_sqlite3HeaderSizePcache_consistent_across_calls();
  test_sqlite3HeaderSizePcache_is_aligned();

  if (g_test_failures == 0) {
    std::fprintf(stdout, "ALL TESTS PASSED\n");
  } else {
    std::fprintf(stderr, "TOTAL FAILURES: %d\n", g_test_failures);
  }
}

// Entry point (C++ program)
int main() {
  run_all_tests();
  // Return non-zero if any test failed to signal issues to build systems.
  return g_test_failures;
}