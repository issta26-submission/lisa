/*
Step 1 - Program Understanding (Candidate Keywords)
- sqlite3_value_int64: focal method under test
- sqlite3VdbeIntValue: internal helper invoked by sqlite3_value_int64
- Mem: underlying type cast from sqlite3_value*
- sqlite3_value: opaque value type (test uses as a wrapper for Mem)
- sqlite_int64: 64-bit signed integer return type
- vdbeapi.c / vdbeInt handling context (represented in test via local stubs)
This test harness provides a minimal, self-contained environment to exercise
sqlite3_value_int64 semantics by steering through sqlite3VdbeIntValue and Mem.
*/

#include <opcodes.h>
#include <cstddef>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Domain knowledge: keep tests isolated in a single TU, no GTest, use a lightweight harness.

// Public types/aliases used by the focal method (mocked/stubbed for testing)
typedef long long sqlite_int64;

// Opaque sqlite3_value type (contents irrelevant for this unit test)
struct sqlite3_value {};

// Minimal Mem structure to back the test value storage
struct Mem {
  sqlite_int64 i; // the 64-bit integer payload used by sqlite3VdbeIntValue
};

// Global flag to verify that the internal helper is invoked by the focal method
static int g_vdbe_intvalue_calls = 0;

// Mocked internal function that the focal method delegates to.
// In the real SQLite, this would extract the integer value from a Mem object.
// For testing, we implement a simple, predictable behavior and count invocations.
static sqlite_int64 sqlite3VdbeIntValue(Mem* p){
  g_vdbe_intvalue_calls++;
  if (p == nullptr) return 0;
  return p->i;
}

// Focal method under test (replicated here for a self-contained test environment).
// In the real project, this would live in vdbeapi.c and be linked from the SQLite sources.
// The function signature must match the original to exercise its behavior.
extern "C" sqlite_int64 sqlite3_value_int64(sqlite3_value *pVal){
  return sqlite3VdbeIntValue((Mem*)pVal);
}

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT_EQUAL(func_result, expected, test_disc) do { \
  ++g_total_tests; \
  if ((func_result) != (expected)) { \
    std::cerr << "Test FAILED: " << test_disc \
              << " Expected: " << (expected) \
              << ", Got: " << (func_result) << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

#define TEST_ASSERT_TRUE(cond, test_disc) do { \
  ++g_total_tests; \
  if (!(cond)) { \
    std::cerr << "Test FAILED: " << test_disc \
              << " Condition evaluated to false." << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

// Helper to reset the internal invocation counter between tests
static void resetInvocationCounter() { g_vdbe_intvalue_calls = 0; }

// Test 1: Basic positive value retrieval
// - Verifies that a standard positive 64-bit value is returned correctly
// - Verifies that sqlite3VdbeIntValue is invoked exactly once
static void test_value_int64_basic_positive() {
  Mem m;
  m.i = 1234567890123456789LL; // within 64-bit signed range
  sqlite3_value *pVal = (sqlite3_value*)&m;

  resetInvocationCounter();
  sqlite_int64 res = sqlite3_value_int64(pVal);

  TEST_ASSERT_EQUAL(res, m.i, "Basic positive value retrieval");
  TEST_ASSERT_EQUAL((sqlite_int64)g_vdbe_intvalue_calls, 1, "Invocation count for positive value");
}

// Test 2: Basic negative value retrieval
// - Ensures that negative 64-bit values are preserved/transmitted correctly
static void test_value_int64_basic_negative() {
  Mem m;
  m.i = -987654321098765432LL;
  sqlite3_value *pVal = (sqlite3_value*)&m;

  resetInvocationCounter();
  sqlite_int64 res = sqlite3_value_int64(pVal);

  TEST_ASSERT_EQUAL(res, m.i, "Basic negative value retrieval");
  TEST_ASSERT_EQUAL((sqlite_int64)g_vdbe_intvalue_calls, 1, "Invocation count for negative value");
}

// Test 3: Maximum 64-bit value
// - Checks handling of the largest positive 64-bit integer
static void test_value_int64_maximum() {
  Mem m;
  m.i = 9223372036854775807LL; // LLONG_MAX
  sqlite3_value *pVal = (sqlite3_value*)&m;

  resetInvocationCounter();
  sqlite_int64 res = sqlite3_value_int64(pVal);

  TEST_ASSERT_EQUAL(res, m.i, "Maximum int64 value retrieval");
  TEST_ASSERT_EQUAL((sqlite_int64)g_vdbe_intvalue_calls, 1, "Invocation count for max int64");
}

// Test 4: Zero value retrieval
// - Verifies zero handling and that invocation still occurs
static void test_value_int64_zero() {
  Mem m;
  m.i = 0;
  sqlite3_value *pVal = (sqlite3_value*)&m;

  resetInvocationCounter();
  sqlite_int64 res = sqlite3_value_int64(pVal);

  TEST_ASSERT_EQUAL(res, (sqlite_int64)0, "Zero value retrieval");
  TEST_ASSERT_EQUAL((sqlite_int64)g_vdbe_intvalue_calls, 1, "Invocation count for zero value");
}

// Test 5: Multiple sequential calls to ensure isolated invocation counts
static void test_value_int64_sequential_calls() {
  Mem m1; m1.i = 1111;
  Mem m2; m2.i = -2222;
  sqlite3_value *pVal1 = (sqlite3_value*)&m1;
  sqlite3_value *pVal2 = (sqlite3_value*)&m2;

  // First call
  resetInvocationCounter();
  sqlite_int64 r1 = sqlite3_value_int64(pVal1);
  TEST_ASSERT_EQUAL(r1, m1.i, "Sequential call 1");
  TEST_ASSERT_EQUAL((sqlite_int64)g_vdbe_intvalue_calls, 1, "Invocation count after first call");

  // Second call
  sqlite_int64 r2 = sqlite3_value_int64(pVal2);
  TEST_ASSERT_EQUAL(r2, m2.i, "Sequential call 2");
  TEST_ASSERT_EQUAL((sqlite_int64)g_vdbe_intvalue_calls, 1 + 1, "Invocation count after second call");
}

// Runner to execute all tests and report summary
static void run_all_tests() {
  test_value_int64_basic_positive();
  test_value_int64_basic_negative();
  test_value_int64_maximum();
  test_value_int64_zero();
  test_value_int64_sequential_calls();

  if (g_failed_tests == 0) {
    std::cout << "ALL TESTS PASSED: " << g_total_tests << " tests." << std::endl;
  } else {
    std::cerr << g_failed_tests << " tests FAILED out of " << g_total_tests << "." << std::endl;
  }
}

// Main entry point
int main() {
  run_all_tests();
  return g_failed_tests == 0 ? 0 : 1;
}