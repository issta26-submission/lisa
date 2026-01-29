// Test suite for sqlite3OsSleep(sqlite3_vfs *pVfs, int nMicro)
// This harness assumes the real sqlite3OsSleep is linked from os.c and uses
// the real sqlite3_vfs type defined in the SQLite headers.
// The tests focus on ensuring sqlite3OsSleep correctly forwards the call to
// pVfs->xSleep and returns its value, while also asserting that the arguments
// passed through are preserved.
//
// Candidate Keywords (Step 1 insights):
// - pVfs: the sqlite3_vfs instance provided to sqlite3OsSleep
// - xSleep: the function pointer member of sqlite3_vfs invoked by sqlite3OsSleep
// - nMicro: the input microseconds to sleep, forwarded to xSleep
// - sqlite3_vfs: the VFS abstraction used by SQLite
// - Forwarding behavior: ensure return value and arguments are passed through intact

#include <sqlite3.h>
#include <iostream>
#include <sqliteInt.h>
#include <cassert>


// Non-terminating test assertions (mirroring EXPECT_* style without a framework)
static int g_failCount = 0;

// Simple logging helpers
#define TEST_LOG(msg) std::cerr << "[Test] " << msg << std::endl
#define EXPECT_EQ(a, b) do { \
  if (!((a) == (b))) { \
    ++g_failCount; \
    std::cerr << "EXPECT_EQ failed: " #a " == " #b " (actual: " << (a) \
              << ", expected: " << (b) << ")" \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while (0)

#define EXPECT_PTR_EQ(a, b) do { \
  if ((void*)(a) != (void*)(b)) { \
    ++g_failCount; \
    std::cerr << "EXPECT_PTR_EQ failed: " #a " == " #b \
              << " (actual: " << (void*)(a) << ", expected: " << (void*)(b) \
              << ")" << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while (0)

#define EXPECT_TRUE(cond) do { \
  if (!(cond)) { \
    ++g_failCount; \
    std::cerr << "EXPECT_TRUE failed: " #cond \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while (0)

// Forward declarations of test-specific xSleep hooks.
// These will be registered into sqlite3_vfs.xSleep to intercept calls from sqlite3OsSleep.
extern "C" int test_sleep_xSleep(sqlite3_vfs *pvfs, int nMicro);
extern "C" int test_sleep_xSleep2(sqlite3_vfs *pvfs, int nMicro);

// Thread-local-like globals to capture call details inside the test hooks.
static sqlite3_vfs *g_lastVfsCalled = nullptr;
static int g_lastNMicroCalled = 0;

// Hook implementations
extern "C" int test_sleep_xSleep(sqlite3_vfs *pvfs, int nMicro) {
  g_lastVfsCalled = pvfs;
  g_lastNMicroCalled = nMicro;
  // Return a sentinel value to verify forwarding of return status
  return 12345;
}

extern "C" int test_sleep_xSleep2(sqlite3_vfs *pvfs, int nMicro) {
  g_lastVfsCalled = pvfs;
  g_lastNMicroCalled = nMicro;
  // Return a different sentinel to verify multiple scenarios
  return -7;
}

// Test 1: Verify forwarding of return value and that xSleep is invoked with correct args
// Rationale: sqlite3OsSleep should call pVfs->xSleep(pVfs, nMicro) and return its value.
void test_sqlite3OsSleep_forwards_and_returns_value() {
  TEST_LOG("Test 1: Forwarding and return value propagation");

  sqlite3_vfs vfs = {};
  vfs.xSleep = test_sleep_xSleep; // Hook into forwarding path

  g_lastVfsCalled = nullptr;
  g_lastNMicroCalled = -9999;

  int ret = sqlite3OsSleep(&vfs, 77);

  EXPECT_EQ(ret, 12345);
  EXPECT_PTR_EQ(g_lastVfsCalled, &vfs);
  EXPECT_EQ(g_lastNMicroCalled, 77);
}

// Test 2: Verify that multiple distinct vfs instances forward independently
// Rationale: Ensure that the function does not mix contexts between different VFS instances.
void test_sqlite3OsSleep_for_multiple_vfs_instances() {
  TEST_LOG("Test 2: Forwarding with multiple distinct sqlite3_vfs instances");

  sqlite3_vfs vfs1 = {};
  vfs1.xSleep = test_sleep_xSleep; // First vfs uses sentinel 12345

  sqlite3_vfs vfs2 = {};
  vfs2.xSleep = test_sleep_xSleep2; // Second vfs uses sentinel -7

  // Call with first vfs
  g_lastVfsCalled = nullptr;
  g_lastNMicroCalled = -1;
  int ret1 = sqlite3OsSleep(&vfs1, 0);
  EXPECT_EQ(ret1, 12345);
  EXPECT_PTR_EQ(g_lastVfsCalled, &vfs1);
  EXPECT_EQ(g_lastNMicroCalled, 0);

  // Call with second vfs
  g_lastVfsCalled = nullptr;
  g_lastNMicroCalled = -1;
  int ret2 = sqlite3OsSleep(&vfs2, 1234);
  EXPECT_EQ(ret2, -7);
  EXPECT_PTR_EQ(g_lastVfsCalled, &vfs2);
  EXPECT_EQ(g_lastNMicroCalled, 1234);
}

// Test 3: Verify forwarding for a variety of input values
// Rationale: Ensure correctness across boundary-like values (0, small, large).
void test_sqlite3OsSleep_various_inputs() {
  TEST_LOG("Test 3: Forwarding for a variety of nMicro inputs");

  // Reusable vfs with first hook
  sqlite3_vfs vfs = {};
  vfs.xSleep = test_sleep_xSleep;

  int testValues[] = {0, 1, 9999, 12345678};
  for (int v : testValues) {
    g_lastVfsCalled = nullptr;
    g_lastNMicroCalled = -1;
    int ret = sqlite3OsSleep(&vfs, v);
    // Expect forwarding to return 12345 (as per test_sleep_xSleep)
    EXPECT_EQ(ret, 12345);
    EXPECT_PTR_EQ(g_lastVfsCalled, &vfs);
    EXPECT_EQ(g_lastNMicroCalled, v);
  }
}

// Runner to execute all tests
void run_all_tests() {
  test_sqlite3OsSleep_forwards_and_returns_value();
  test_sqlite3OsSleep_for_multiple_vfs_instances();
  test_sqlite3OsSleep_various_inputs();
}

// Entry point for the test executable
int main() {
  g_failCount = 0;
  TEST_LOG("Starting sqlite3OsSleep unit test suite (C++11, no GTest)");

  run_all_tests();

  if (g_failCount == 0) {
    std::cout << "[ALL TESTS PASSED] sqlite3OsSleep forwarding behavior is correct." << std::endl;
    return 0;
  } else {
    std::cerr << "[TESTS FAILED] " << g_failCount << " test(s) failed." << std::endl;
    return 1;
  }
}