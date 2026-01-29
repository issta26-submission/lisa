// C++11 test suite for sqlite3PcacheSetCachesize
// This test does not rely on GTest. It uses a small, self-contained
// test framework with non-terminating assertions to maximize coverage.

#include <iostream>
#include <cstdio>
#include <sqliteInt.h>
#include <cstdint>


// Forward declaration and includes for SQLite internal types.
// We rely on the project's internal headers to expose PCache and related APIs.
// The test is designed to be compiled within the same repository where SQLite's
// internal headers reside.
extern "C" {
  // Include the internal SQLite headers to get PCache, PgHdr, and sqlite3GlobalConfig
  // The exact header path may vary by project layout; adjust if necessary.
  // Typical projects expose internal APIs via sqliteInt.h for test scenarios like this.
  #include "sqliteInt.h"
}

#ifdef __EMSCRIPTEN__
#error This test is intended for native compilation, not WebAssembly.
#endif

// Simple non-terminating test harness
static int g_test_passed = 0;
static int g_test_failed = 0;

#define T_EXPECT_TRUE(cond, msg)                                        \
  do { if (cond) { ++g_test_passed; } else {                          \
      std::cerr << "TEST_FAIL: " << (msg) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
      ++g_test_failed; } } while(0)

#define T_EXPECT_EQ(a, b, msg)                                           \
  do { if ((a) == (b)) { ++g_test_passed; } else {                       \
      std::cerr << "TEST_FAIL: " << (msg) << " (expected: " #a "=" << (a)       \
                << ", actual: " << #b "=" << (b) << ") at "                 \
                << __FILE__ << ":" << __LINE__ << "\n";                 \
      ++g_test_failed; } } while(0)

#define T_EXPECT_NEQ(a, b, msg)                                          \
  do { if ((a) != (b)) { ++g_test_passed; } else {                        \
      std::cerr << "TEST_FAIL: " << (msg) << " (unexpected equal) at "      \
                << __FILE__ << ":" << __LINE__ << "\n";                 \
      ++g_test_failed; } } while(0)

#define TEST_CASE(name) void name()

// Global capture for the xCachesize callback to verify interactions
static void *g_xCachesize_last_pCache = nullptr;
static int   g_xCachesize_last_second = -1;
static bool  g_xCachesize_called = false;

// Test hook: capture first argument to sqlite3GlobalConfig.pcache2.xCachesize
static void test_xCachesize(void *pCache, int iSecond) {
  g_xCachesize_called = true;
  g_xCachesize_last_pCache = pCache;
  g_xCachesize_last_second = iSecond;
}

// Helper: reset capture state
static void reset_xCachesize_capture() {
  g_xCachesize_last_pCache = nullptr;
  g_xCachesize_last_second = -1;
  g_xCachesize_called = false;
}

// Test 1: Basic behavior - ensures szCache is set and xCachesize is invoked with the correct first argument
TEST_CASE(test_sqlite3PcacheSetCachesize_basic) {
  reset_xCachesize_capture();

  // Configure the global config to intercept xCachesize calls
  // sqlite3GlobalConfig.pcache2.xCachesize(void*, int)
  sqlite3GlobalConfig.pcache2.xCachesize = test_xCachesize;

  // Create/initialize a PCache instance using the public internal API.
  // We rely on sqlite3PcacheOpen to allocate/initialize the PCache structure cleanly.
  PCache pcache_instance;
  // The exact parameters of sqlite3PcacheOpen:
  // (szPage, szExtra, bPurgeable, xStress, pStress, PCache *p)
  // We pick conservative values; xStress is NULL in tests.
  int rc = sqlite3PcacheOpen(1024, 0, 0, NULL, NULL, &pcache_instance);
  T_EXPECT_EQ(rc, 0, "sqlite3PcacheOpen should return 0 (OK)");

  // Clear the flag and perform the operation
  reset_xCachesize_capture();
  const int mxPage = 42;
  sqlite3PcacheSetCachesize(&pcache_instance, mxPage);

  // Verify szCache updated
  T_EXPECT_EQ(pcache_instance.szCache, mxPage, "szCache should be updated to mxPage");

  // Verify xCachesize was called and that first parameter matches pcache_instance.pCache
  T_EXPECT_TRUE(g_xCachesize_called, "xCachesize should be called");
  T_EXPECT_TRUE(g_xCachesize_last_pCache == pcache_instance.pCache,
                "xCachesize first argument should be pcache_instance.pCache");

  // Cleanup
  sqlite3PcacheClose(&pcache_instance);
  // Do not reset global state to avoid side effects on other tests
}

// Test 2: Different mxPage values (including 0) should still set szCache and invoke xCachesize
TEST_CASE(test_sqlite3PcacheSetCachesize_variousSizes) {
  sqlite3GlobalConfig.pcache2.xCachesize = test_xCachesize;

  PCache pcache_instance;
  int rc = sqlite3PcacheOpen(1024, 0, 0, NULL, NULL, &pcache_instance);
  T_EXPECT_EQ(rc, 0, "sqlite3PcacheOpen should return 0 (OK) for second test");

  // Test with a zero size
  reset_xCachesize_capture();
  sqlite3PcacheSetCachesize(&pcache_instance, 0);
  T_EXPECT_EQ(pcache_instance.szCache, 0, "szCache should be updated to 0");
  T_EXPECT_TRUE(g_xCachesize_called, "xCachesize should be called for mxPage=0");

  sqlite3PcacheClose(&pcache_instance);

  // Test with a larger size
  rc = sqlite3PcacheOpen(2048, 0, 0, NULL, NULL, &pcache_instance);
  T_EXPECT_EQ(rc, 0, "sqlite3PcacheOpen should return 0 (OK) for second part");

  reset_xCachesize_capture();
  sqlite3PcacheSetCachesize(&pcache_instance, 12345);
  T_EXPECT_EQ(pcache_instance.szCache, 12345, "szCache should be updated to 12345");
  T_EXPECT_TRUE(g_xCachesize_called, "xCachesize should be called for mxPage=12345");

  sqlite3PcacheClose(&pcache_instance);
}

// Main runner
int main() {
  // Run tests
  test_sqlite3PcacheSetCachesize_basic();
  test_sqlite3PcacheSetCachesize_variousSizes();

  // Summary
  std::cout << "Tests run: " << (g_test_passed + g_test_failed) << "\n";
  std::cout << "Passed: " << g_test_passed << "\n";
  std::cout << "Failed: " << g_test_failed << "\n";

  // Return non-zero if failures occurred
  return g_test_failed != 0;
}