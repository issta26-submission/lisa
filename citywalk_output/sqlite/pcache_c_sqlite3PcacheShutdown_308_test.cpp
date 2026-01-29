// Unit test suite for sqlite3PcacheShutdown (pcache.c) without using GTest.
// This test relies on overriding the sqlite3GlobalConfig.pcache2.xShutdown callback
// to verify that sqlite3PcacheShutdown calls it with the correct argument when non-NULL,
// and does nothing when it is NULL.
//
// Notes:
// - The test uses extern "C" linkage for C symbols to interoperate with the C code in pcache.c.
// - We declare an opaque extern sqlite3GlobalConfig that matches the layout used by pcache.c
//   (pcache2.xShutdown and pcache2.pArg). This is a minimal shim sufficient for testing.
// - We call sqlite3PcacheShutdown() which is implemented in pcache.c.
// - We provide simple non-terminating assertions to maximize code coverage.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Forward declarations to link with the production code (C linkage)
extern "C" {
  // Provide a minimal shim for the global configuration used by sqlite3PcacheShutdown.
  // The real SQLite project defines sqlite3GlobalConfig in a much larger scope,
  // but for the purpose of unit testing the focal function we only need the
  // pcache2.xShutdown callback and its pArg.
  struct {
    struct {
      void (*xShutdown)(void*);
      void *pArg;
    } pcache2;
  } sqlite3GlobalConfig;

  // The function under test (from pcache.c)
  void sqlite3PcacheShutdown(void);

  // Mock callback to verify invocation and parameter passing
  void mock_xShutdown(void* arg);
}

// Test state
static int g_shutdown_call_count = 0;
static void* g_shutdown_last_arg = nullptr;

// Mock implementation of xShutdown used by tests
extern "C" void mock_xShutdown(void* arg) {
  ++g_shutdown_call_count;
  g_shutdown_last_arg = arg;
}

// Helper: reset test state
static void reset_test_state() {
  g_shutdown_call_count = 0;
  g_shutdown_last_arg = nullptr;
}

// Simple non-terminating assertion macros
static int g_test_failed = 0;
#define EXPECT_EQ(actual, expected) do { \
  if ((actual) != (expected)) { \
    std::cerr << "EXPECT_EQ failed: " << (actual) << " != " << (expected) \
              << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    ++g_test_failed; \
  } \
} while(0)

#define EXPECT_PTR_EQ(actual, expected) do { \
  if ((actual) != (expected)) { \
    std::cerr << "EXPECT_PTR_EQ failed: " << actual << " != " << expected \
              << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    ++g_test_failed; \
  } \
} while(0)

// Test 1: When xShutdown is non-NULL, it should be invoked with pArg
static void test_shutdown_calls_when_callback_non_null() {
  reset_test_state();

  // Arrange: provide a non-NULL callback and a known argument
  sqlite3GlobalConfig.pcache2.xShutdown = mock_xShutdown;
  int dummy = 0xABCD;
  sqlite3GlobalConfig.pcache2.pArg = &dummy;

  // Act
  sqlite3PcacheShutdown();

  // Assert: callback was invoked exactly once with the provided argument
  EXPECT_EQ(g_shutdown_call_count, 1);
  EXPECT_PTR_EQ(g_shutdown_last_arg, &dummy);
}

// Test 2: When xShutdown is NULL, sqlite3PcacheShutdown should do nothing
static void test_shutdown_no_call_when_callback_null() {
  reset_test_state();

  // Arrange: NULL callback, non-NULL arg (arg value should be ignored)
  sqlite3GlobalConfig.pcache2.xShutdown = NULL;
  int dummy = 0x1234;
  sqlite3GlobalConfig.pcache2.pArg = &dummy;

  // Act
  sqlite3PcacheShutdown();

  // Assert: no invocation occurred
  EXPECT_EQ(g_shutdown_call_count, 0);
  // Arg should remain unchanged because no call happened
  EXPECT_PTR_EQ(g_shutdown_last_arg, nullptr);
}

// Test 3: When xShutdown is non-NULL and pArg is NULL, the callback should be invoked with NULL
static void test_shutdown_calls_with_null_arg() {
  reset_test_state();

  // Arrange: non-NULL callback, NULL arg
  sqlite3GlobalConfig.pcache2.xShutdown = mock_xShutdown;
  sqlite3GlobalConfig.pcache2.pArg = NULL;

  // Act
  sqlite3PcacheShutdown();

  // Assert: callback invoked with NULL
  EXPECT_EQ(g_shutdown_call_count, 1);
  EXPECT_PTR_EQ(g_shutdown_last_arg, NULL);
}

// Runner
static void run_all_tests() {
  test_shutdown_calls_when_callback_non_null();
  test_shutdown_no_call_when_callback_null();
  test_shutdown_calls_with_null_arg();

  if (g_test_failed) {
    std::cerr << "Unit tests finished with " << g_test_failed << " failure(s).\n";
  } else {
    std::cout << "All unit tests passed.\n";
  }
}

int main() {
  run_all_tests();
  // Return non-zero if any test failed
  return g_test_failed ? 1 : 0;
}