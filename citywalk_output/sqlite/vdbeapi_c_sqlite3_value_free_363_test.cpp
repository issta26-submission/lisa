/*
 Lightweight C++11 unit tests for the focal method:
   void sqlite3_value_free(sqlite3_value *pOld){
     sqlite3ValueFree(pOld);
   }

Notes:
- We provide a minimal mock of the internal symbol sqlite3ValueFree to verify
  that sqlite3_value_free delegates to it correctly.
- We avoid any dependencies on external test frameworks (GTest not used).
- Tests cover non-null and null pointer behavior, ensuring wrapper correctness.
- The test uses a small, self-contained test harness with simple ASSERT-like macros.
*/

#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Minimal representation of the sqlite3_value type for testing purposes.
struct sqlite3_value {
  int dummy;
};

// Forward declaration of the internal function that sqlite3_value_free should call.
// In the real SQLite codebase, this is an internal API function.
extern "C" void sqlite3ValueFree(sqlite3_value *pVal);

// The focal method under test, reproduced here to exercise and validate its behavior.
// It should call the internal sqlite3ValueFree with the same pointer.
extern "C" void sqlite3_value_free(sqlite3_value *pOld){
  sqlite3ValueFree(pOld);
}

// Global counters to verify that sqlite3ValueFree is invoked correctly by the wrapper.
static int g_valueFreeCalls = 0;        // Counts non-null pointers passed to sqlite3ValueFree
static int g_valueFreeNullCalls = 0;    // Counts NULL-pointer invocations
static sqlite3_value *g_lastPointer = nullptr;

// Mock implementation of sqlite3ValueFree to observe wrapper behavior.
// This function is intentionally minimal and C-linkage is used to match the real symbol.
extern "C" void sqlite3ValueFree(sqlite3_value *pVal){
  if(pVal == nullptr){
    g_valueFreeNullCalls++;
  } else {
    g_valueFreeCalls++;
    g_lastPointer = pVal;
  }
}

// Simple test harness with non-terminating assertions (so all tests run).
static int totalAssertions = 0;
static int failedAssertions = 0;

#define ASSERT_TRUE(cond, msg) do { \
  totalAssertions++; \
  if(!(cond)) { \
    failedAssertions++; \
    std::cerr << "ASSERT FAILED: " << (msg) \
              << "  (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
  } \
} while(0)

//
// Test 1: Non-null pointer should cause sqlite3ValueFree to be called exactly once
//           and the wrapper should pass the original pointer through.
//
static void test_wrapper_calls_internal_on_non_null_pointer() {
  // Reset counters
  g_valueFreeCalls = 0;
  g_valueFreeNullCalls = 0;
  g_lastPointer = nullptr;

  // Create a dummy sqlite3_value instance on the stack
  sqlite3_value v;
  v.dummy = 42;

  // Call the wrapper
  sqlite3_value_free(&v);

  // Assertions
  ASSERT_TRUE(g_valueFreeCalls == 1, "sqlite3_value_free should call sqlite3ValueFree exactly once for non-null input");
  ASSERT_TRUE(g_valueFreeNullCalls == 0, "sqlite3ValueFree should not be invoked with NULL for non-null input");
  ASSERT_TRUE(g_lastPointer == &v, "sqlite3ValueFree should receive the original pointer (pass-through)");
}

//
// Test 2: NULL pointer should be handled gracefully and not crash.
//           We expect sqlite3ValueFree to be called with NULL (or gracefully ignored by the mock).
//
static void test_wrapper_handles_null_pointer_without_crash() {
  // Reset counters
  g_valueFreeCalls = 0;
  g_valueFreeNullCalls = 0;
  g_lastPointer = nullptr;

  // Call the wrapper with NULL
  sqlite3_value_free(nullptr);

  // Assertions
  // The mock tracks NULL invocations separately; non-null count should stay 0.
  ASSERT_TRUE(g_valueFreeCalls == 0, "sqlite3_value_free with NULL should not call sqlite3ValueFree with non-null pointer");
  ASSERT_TRUE(g_valueFreeNullCalls == 1, "sqlite3ValueFree should be invoked with NULL (as observed by the mock)");
  ASSERT_TRUE(g_lastPointer == nullptr, "No non-null pointer should be processed when input is NULL");
}

//
// Optional additional test: Multiple calls to ensure state is preserved across calls.
//
static void test_wrapper_multiple_calls_non_null() {
  // Reset counters
  g_valueFreeCalls = 0;
  g_valueFreeNullCalls = 0;
  g_lastPointer = nullptr;

  sqlite3_value a, b;
  a.dummy = 1;
  b.dummy = 2;

  sqlite3_value_free(&a);
  sqlite3_value_free(&b);
  sqlite3_value_free(nullptr);

  // Assertions
  ASSERT_TRUE(g_valueFreeCalls == 2, "Two non-null calls should increment non-null counter");
  ASSERT_TRUE(g_valueFreeNullCalls == 1, "One null call should increment null-call counter");
  ASSERT_TRUE(g_lastPointer == &b, "Last non-null pointer should be the last non-null argument passed");
}

int main() {
  std::cout << "Running unit tests for sqlite3_value_free wrapper...\n";

  test_wrapper_calls_internal_on_non_null_pointer();
  test_wrapper_handles_null_pointer_without_crash();
  test_wrapper_multiple_calls_non_null();

  std::cout << "Tests completed. Assertions: " << totalAssertions
            << ", Failures: " << failedAssertions << "\n";

  if(failedAssertions == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED\n";
    return 1;
  }
}