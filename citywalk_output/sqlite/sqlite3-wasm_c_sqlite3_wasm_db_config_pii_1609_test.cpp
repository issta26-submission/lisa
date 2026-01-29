// Lightweight C++11 unit tests for sqlite3_wasm_db_config_pii (no GTest)
// This test suite provides minimal EXPECT-like assertions and a small
// test harness to validate the focal method behavior against a stubbed
// sqlite3_db_config implementation.

// The tests are self-contained: they re-implement the minimal surrounding
// API (sqlite3, SQLITE_DBCONFIG_LOOKASIDE, SQLITE_MISUSE) and the focal
// function sqlite3_wasm_db_config_pii. A real project would link against
// the actual sqlite3 library; here we simulate behavior to achieve high
// coverage of the focal method logic.

#include <emscripten/wasmfs.h>
#include <assert.h>
#include <cstddef>
#include <iostream>
#include <cstdint>


// Domain constants (as used by sqlite3_wasm_db_config_pii)
#define SQLITE_DBCONFIG_LOOKASIDE 1
#define SQLITE_MISUSE -1

// Minimal sqlite3 structure (stub)
struct sqlite3 {
  int dummy;
};

// Forward declaration of the focal method under test (re-implemented locally)
int sqlite3_wasm_db_config_pii(sqlite3 *pDb, int op, void * pArg1, int arg2, int arg3);

// Globals to capture calls to the stubbed sqlite3_db_config
static sqlite3* g_last_db_config_db = nullptr;
static int      g_last_db_config_op = 0;
static void*    g_last_db_config_arg1 = nullptr;
static int      g_last_db_config_arg2 = 0;
static int      g_last_db_config_arg3 = 0;

// Helper to reset captured call data
static void reset_last_db_config_call() {
  g_last_db_config_db = nullptr;
  g_last_db_config_op = 0;
  g_last_db_config_arg1 = nullptr;
  g_last_db_config_arg2 = 0;
  g_last_db_config_arg3 = 0;
}

// Stubbed sqlite3_db_config that records inputs for verification
extern "C" int sqlite3_db_config(sqlite3 *db, int op, void *pArg1, int arg2, int arg3) {
  g_last_db_config_db   = db;
  g_last_db_config_op   = op;
  g_last_db_config_arg1 = pArg1;
  g_last_db_config_arg2 = arg2;
  g_last_db_config_arg3 = arg3;
  // Return a recognizable value to verify propagation
  return 42;
}

// Lightweight test assertions (non-terminating)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ_INT(actual, expected, desc) do {                               \
  if ((actual) != (expected)) {                                                  \
    std::cerr << "TEST FAIL: " << (desc)                                               \
              << " | expected " << (expected)                                   \
              << ", got " << (actual) << "\n";                                 \
    ++g_tests_failed;                                                            \
  }                                                                                \
} while(0)

#define EXPECT_EQ_PTR(actual, expected, desc) do {                                 \
  if ((void*)(actual) != (void*)(expected)) {                                      \
    std::cerr << "TEST FAIL: " << (desc)                                               \
              << " | expected pointer " << expected                             \
              << ", got " << actual << "\n";                                    \
    ++g_tests_failed;                                                            \
  }                                                                                \
} while(0)

#define EXPECT_TRUE(cond, desc) do {                                                \
  if (!(cond)) {                                                                   \
    std::cerr << "TEST FAIL: " << (desc)                                               \
              << " | condition failed\n";                                        \
    ++g_tests_failed;                                                            \
  }                                                                                \
} while(0)

// Re-implementation of the focal function under test
int sqlite3_wasm_db_config_pii(sqlite3 *pDb, int op, void * pArg1, int arg2, int arg3){
  switch(op){
    case SQLITE_DBCONFIG_LOOKASIDE:
      return sqlite3_db_config(pDb, op, pArg1, arg2, arg3);
    default: return SQLITE_MISUSE;
  }
}

// Helper to run a test case and report progress
#define RUN_TEST(TEST_FUNC) do { \
  g_tests_run++;                  \
  TEST_FUNC();                      \
} while(0)

// Test 1: When op == SQLITE_DBCONFIG_LOOKASIDE, the function should forward the call
// to sqlite3_db_config with exactly the same parameters and return its value.
static void test_lookaside_forwards_to_db_config() {
  reset_last_db_config_call();
  sqlite3 db;
  void* arg1 = (void*)0x12345678;
  int arg2 = 7;
  int arg3 = 9;

  int ret = sqlite3_wasm_db_config_pii(&db, SQLITE_DBCONFIG_LOOKASIDE, arg1, arg2, arg3);

  EXPECT_EQ_INT(ret, 42, "sqlite3_wasm_db_config_pii should return value from sqlite3_db_config for LOOKASIDE");
  EXPECT_EQ_PTR(g_last_db_config_db, &db, "sqlite3_db_config should receive correct db pointer");
  EXPECT_EQ_INT(g_last_db_config_op, SQLITE_DBCONFIG_LOOKASIDE, "sqlite3_db_config should receive correct op");
  EXPECT_EQ_PTR(g_last_db_config_arg1, arg1, "sqlite3_db_config should receive correct arg1");
  EXPECT_EQ_INT(g_last_db_config_arg2, arg2, "sqlite3_db_config should receive correct arg2");
  EXPECT_EQ_INT(g_last_db_config_arg3, arg3, "sqlite3_db_config should receive correct arg3");
}

// Test 2: For an unsupported op, the focal method should return SQLITE_MISUSE
// and should not invoke sqlite3_db_config.
static void test_unsupported_op_returns_misuse_no_call() {
  reset_last_db_config_call();
  sqlite3 db;
  void* arg1 = (void*)0xDEADBEEF;
  int arg2 = 123;
  int arg3 = -5;

  int ret = sqlite3_wasm_db_config_pii(&db, 9999, arg1, arg2, arg3);

  EXPECT_EQ_INT(ret, SQLITE_MISUSE, "Unsupported op should return SQLITE_MISUSE");
  // Ensure no call happened
  EXPECT_EQ_PTR(g_last_db_config_db, nullptr, "sqlite3_db_config should not be called for unsupported op");
  EXPECT_EQ_INT(g_last_db_config_op, 0, "sqlite3_db_config op should remain default when not called");
}

// Test 3: Null database pointer should still forward to sqlite3_db_config and return value.
// This tests that pDb is passed through as-is.
static void test_null_db_pointer_handled() {
  reset_last_db_config_call();
  void* arg1 = (void*)0xCAFEBABE;
  int arg2 = 0;
  int arg3 = 0;

  int ret = sqlite3_wasm_db_config_pii(nullptr, SQLITE_DBCONFIG_LOOKASIDE, arg1, arg2, arg3);

  EXPECT_EQ_INT(ret, 42, "LOOKASIDE with null db should still return value from sqlite3_db_config");
  EXPECT_EQ_PTR(g_last_db_config_db, (sqlite3*)nullptr, "sqlite3_db_config should receive null db pointer");
  EXPECT_EQ_INT(g_last_db_config_op, SQLITE_DBCONFIG_LOOKASIDE, "sqlite3_db_config should receive correct op for null db");
  EXPECT_EQ_PTR(g_last_db_config_arg1, arg1, "sqlite3_db_config should receive correct arg1 for null db case");
  EXPECT_EQ_INT(g_last_db_config_arg2, arg2, "sqlite3_db_config should receive correct arg2 for null db case");
  EXPECT_EQ_INT(g_last_db_config_arg3, arg3, "sqlite3_db_config should receive correct arg3 for null db case");
}

// Test 4: Additional coverage for different argument values.
// Ensures that varied inputs still forward through correctly.
static void test_lookaside_varied_args() {
  reset_last_db_config_call();
  sqlite3 db;
  void* arg1 = (void*)0x0ABC;
  int arg2 = -42;
  int arg3 = 2147483646;

  int ret = sqlite3_wasm_db_config_pii(&db, SQLITE_DBCONFIG_LOOKASIDE, arg1, arg2, arg3);

  EXPECT_EQ_INT(ret, 42, "LOOKASIDE with varied args should forward correctly");
  EXPECT_EQ_PTR(g_last_db_config_db, &db, "db pointer should be forwarded unchanged");
  EXPECT_EQ_INT(g_last_db_config_op, SQLITE_DBCONFIG_LOOKASIDE, "op should be LOOKASIDE");
  EXPECT_EQ_PTR(g_last_db_config_arg1, arg1, "arg1 should match varied input");
  EXPECT_EQ_INT(g_last_db_config_arg2, arg2, "arg2 should match varied input");
  EXPECT_EQ_INT(g_last_db_config_arg3, arg3, "arg3 should match varied input");
}

// Main: run all tests and report summary
int main() {
  std::cout << "Running sqlite3_wasm_db_config_pii unit tests (no GTest)..." << std::endl;

  test_lookaside_forwards_to_db_config();     // Test 1
  test_unsupported_op_returns_misuse_no_call(); // Test 2
  test_null_db_pointer_handled();             // Test 3
  test_lookaside_varied_args();               // Test 4

  std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;

  // Return non-zero if any test failed
  return g_tests_failed ? 1 : 0;
}