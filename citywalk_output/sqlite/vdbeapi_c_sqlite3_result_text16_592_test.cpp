// Comprehensive C++11 unit tests for sqlite3_result_text16
// This test suite provides a minimal, self-contained environment to
// exercise the focal method and its dependent behavior without a full
// SQLite build. It uses a tiny test harness with simple assertions and
// prints a summary at the end.

// NOTE on scope:
// - This test focuses on the true-path behavior of sqlite3_result_text16
//   (i.e., with the mutex held). The false-path (assert violation) is
//   not executed here to avoid terminating the test suite.
// - We recreate only the necessary parts of the surrounding SQLite types
//   used by sqlite3_result_text16 for isolation and determinism.

#include <opcodes.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cassert>


// Domain-specific simple types to mirror the usage in sqlite3_result_text16
using u64 = unsigned long long;
using u8  = unsigned char;

// Public constant used by the function under test
#define SQLITE_UTF16NATIVE 1

// Forward declaration of the mutex-check helper (simulated for tests)
extern "C" int sqlite3_mutex_held(void*);

// Minimal dummy mutex type (opaque in this test)
struct SQLiteMutex {
  int dummy;
};

// Lightweight mocks of only the minimal SQLite context structures required
struct sqlite3_db {
  SQLiteMutex* mutex; // mutex protecting the database connection
};

struct sqlite3_out {
  sqlite3_db* db;
};

struct sqlite3_context {
  sqlite3_out* pOut;
};

// Global storage to capture the result produced by sqlite3_result_text16
struct ResultState {
  bool hasResult;
  std::string text;
  int enc;          // encoding parameter passed through (should be SQLITE_UTF16NATIVE)
  void (*xDel)(void*); // destructor passed through (if any)
} g_result = {false, "", 0, nullptr};

// Helper to reset captured result before each test
static void resetResultState() {
  g_result.hasResult = false;
  g_result.text.clear();
  g_result.enc = 0;
  g_result.xDel = nullptr;
}

// Simulated destructor function to validate xDel propagation
static void dummyDestructor(void*){
  // no-op
}

// Simple implementation to mimic setResultStrOrError behavior used by the focal method.
// In a real SQLite environment this would convert and store the result or an error.
// For testing, we only verify that the data flow is correct.
static void setResultStrOrError(
  sqlite3_context *pCtx,
  const void *z,
  int n,
  u8 enc,
  void (*xDel)(void*)
){
  // The original code applies n & ~(u64)1 to strip the least significant bit.
  size_t len = static_cast<size_t>(n & ~(u64)1);
  // Copy the bytes if available; otherwise, produce an empty result
  if (z != nullptr && len > 0) {
    g_result.text.assign(reinterpret_cast<const char*>(z), len);
  } else {
    g_result.text.clear();
  }
  g_result.enc = static_cast<int>(enc);
  g_result.xDel = xDel;
  g_result.hasResult = true;
}

// The focal function under test: sqlite3_result_text16
// Note: We implement a standalone version here to enable testing without the
// full SQLite source tree.
void sqlite3_result_text16(
  sqlite3_context *pCtx,
  const void *z,
  int n,
  void (*xDel)(void *)
){
  // In the real code this asserts that the mutex is held by the current thread.
  // Our test harness simulates the condition by returning true from sqlite3_mutex_held.
  assert( sqlite3_mutex_held(pCtx->pOut->db->mutex) );
  // Core behavior: apply the length mask and delegate to the helper
  setResultStrOrError(pCtx, z, n & ~(u64)1, SQLITE_UTF16NATIVE, xDel);
}

// Implementation of sqlite3_mutex_held for the test (always true to exercise the path)
extern "C" int sqlite3_mutex_held(void*){
  return 1;
}

// TEST HARNESSSS: A tiny framework to run and report unit tests
static int gTestsPassed = 0;
static int gTotalTests  = 0;

#define RUN_TEST(t) do {                          \
    ++gTotalTests;                                   \
    t();                                             \
    std::cout << "[TEST] " #t " passed\n";          \
    ++gTestsPassed;                                   \
} while(0)

#define ASSERT_EQ(a, b) do {                          \
  if (!((a) == (b))) {                                 \
    std::cerr << "ASSERTION FAILED: " #a " == " #b " (" << (a) << " != " << (b) << ")\n"; \
    std::exit(1);                                        \
  }                                                     \
} while(0)

#define ASSERT_TRUE(cond) do { if(!(cond)){ \
  std::cerr << "ASSERTION FAILED: " #cond "\n"; std::exit(1); } } while(0)

// Test fixture helper to build a minimal sqlite3_context with nested pOut/db/mutex
struct TestContext {
  sqlite3_db  db;
  SQLiteMutex mutexObj;
  sqlite3_out  out;
  sqlite3_context ctx;

  TestContext() {
    mutexObj.dummy = 0;
    db.mutex = &mutexObj;
    out.db = &db;
    ctx.pOut = &out;
  }
};

// Test 1: Verify that an even-length n copies exactly n bytes from z
static void test_even_length_copy() {
  resetResultState();
  TestContext tc;

  const char *data = "abcdef"; // 6 bytes
  sqlite3_result_text16(&tc.ctx, (const void*)data, 6, nullptr);

  ASSERT_TRUE(g_result.hasResult);
  ASSERT_EQ(g_result.text, std::string("abcdef"));
  ASSERT_EQ(g_result.enc, SQLITE_UTF16NATIVE);
  ASSERT_TRUE(g_result.xDel == nullptr);
}

// Test 2: Verify that an odd-length n truncates the last byte (n & ~1 == 4 for n=5)
static void test_odd_length_truncation() {
  resetResultState();
  TestContext tc;

  const char *data = "12345"; // 5 bytes
  sqlite3_result_text16(&tc.ctx, (const void*)data, 5, nullptr);

  ASSERT_TRUE(g_result.hasResult);
  ASSERT_EQ(g_result.text, std::string("1234")); // should copy 4 bytes
  ASSERT_EQ(g_result.enc, SQLITE_UTF16NATIVE);
}

// Test 3: Verify behavior with a null z pointer results in an empty string
static void test_null_input_results_empty() {
  resetResultState();
  TestContext tc;

  sqlite3_result_text16(&tc.ctx, nullptr, 4, nullptr);

  ASSERT_TRUE(g_result.hasResult);
  ASSERT_EQ(g_result.text, std::string(""));
  ASSERT_EQ(g_result.enc, SQLITE_UTF16NATIVE);
}

// Test 4: Verify that the destructor callback is propagated to setResultStrOrError
static void test_destructor_propagated() {
  resetResultState();
  TestContext tc;

  const char *data = "xyz";
  sqlite3_result_text16(&tc.ctx, (const void*)data, 3, &dummyDestructor);

  ASSERT_TRUE(g_result.hasResult);
  // Because length is 3 (odd), 3 & ~1 == 2, so expect 2 bytes copied
  // In this test, we pass 3 to verify xDel propagation rather than text correctness
  // We'll instead check that the destructor pointer is preserved
  ASSERT_TRUE(g_result.xDel == &dummyDestructor);

  // Optional: also verify the textual content corresponds to first 2 bytes
  // which would be "xy" for 2 bytes
  ASSERT_EQ(g_result.text, std::string("xy"));
}

// Main entry point to run tests
int main() {
  std::cout << "sqlite3_result_text16 unit tests started\n";

  RUN_TEST(test_even_length_copy);
  RUN_TEST(test_odd_length_truncation);
  RUN_TEST(test_null_input_results_empty);
  RUN_TEST(test_destructor_propagated);

  std::cout << "All tests completed. Passed " << gTestsPassed << " / " << gTotalTests << " tests.\n";
  return 0;
}