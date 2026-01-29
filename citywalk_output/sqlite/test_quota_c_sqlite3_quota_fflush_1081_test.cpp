/*
  Lightweight C++ test suite for sqlite3_quota_fflush (Step 2/3: test generation)
  - No GoogleTest; uses a tiny, non-terminating assertion style.
  - Tests rely on the public entry points declared for the quota subsystem:
      - quota_FILE is an opaque type; we declare it for function signatures.
      - sqlite3_quota_fopen, sqlite3_quota_fflush, sqlite3_quota_fclose
  - We simulate fsync behavior by overriding the C symbol fsync via a small
    extern "C" function to control success/failure of the OS-specific path.
  - Tests cover:
      1) No fsync path (doFsync = 0): ensure fflush path succeeds
      2) Fsync path with failure: simulate fsync failure and verify non-zero return
      3) Fsync path with success: simulate fsync success and verify zero return
  - All tests are executable C++11 code and print results rather than terminating on failure.
*/

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>


// Forward-declare the quota_FILE type so we can call the C entry points from C++
extern "C" {
  // Forward declaration compatible with the library's definition
  typedef struct quota_FILE quota_FILE;

  // Public API expected to exist in the focal module
  quota_FILE* sqlite3_quota_fopen(const char *zFilename, const char *zMode);
  int sqlite3_quota_fflush(quota_FILE *p, int doFsync);
  int sqlite3_quota_fclose(quota_FILE *p);
}

// ---------------------------------------------------------------------------------
// OS-path override mechanism for testing fsync behavior
// We override the POSIX fsync() to simulate success or failure deterministically.
// This relies on dynamic interposition: the executable provides an fsync() symbol
// which will be used by the code under test when linked in a normal manner.
// ---------------------------------------------------------------------------------

// Global switch to control fsync return value for tests
static int g_fsync_should_fail = 0;

// C linkage to override the standard library fsync behavior for test purposes
extern "C" int fsync(int fd) {
  // When g_fsync_should_fail is non-zero, simulate a failure (-1)
  if (g_fsync_should_fail) {
    return -1;
  }
  // Simulate success (0)
  return 0;
}

// ---------------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// ---------------------------------------------------------------------------------
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(a, b, msg) do {                                \
  ++g_total_tests;                                                 \
  if ((a) != (b)) {                                              \
    std::cerr << "[FAIL] " << (msg) << " | Expected: " << (b)     \
              << ", Got: " << (a) << std::endl;                   \
    ++g_failed_tests;                                            \
  } else {                                                       \
    /* Passed */                                                 \
  }                                                              \
} while(0)

static void run_and_report(const char* name, void (*fn)()){
  std::cout << "Running test: " << name << std::endl;
  fn();
}

// ---------------------------------------------------------------------------------
// Test helpers
// ---------------------------------------------------------------------------------

// Test 1: fflush path without fsync (doFsync = 0) should succeed (return 0)
static void test_fflush_no_fsync_success(){
  // Create a temporary file for quota_file
  char tmpNameTemplate[] = "/tmp/sqlquota_test_XXXXXX.txt";
  int fd = mkstemps(tmpNameTemplate, 4); // create unique file; ".txt" tail included
  if (fd == -1) {
    std::cerr << "[WARN] Unable to create temporary file for test_fflush_no_fsync_success" << std::endl;
    // Skip test gracefully
    return;
  }
  // Close the fd to ensure sqlite3_quota_fopen can open/own its own FILE stream
  close(fd);

  quota_FILE *p = sqlite3_quota_fopen(tmpNameTemplate, "r+");
  if (p == nullptr) {
    std::cerr << "[WARN] sqlite3_quota_fopen failed in test_fflush_no_fsync_success" << std::endl;
    std::remove(tmpNameTemplate);
    return;
  }

  int rc = sqlite3_quota_fflush(p, 0); // doFsync = 0; should not invoke fsync
  EXPECT_EQ(rc, 0, "sqlite3_quota_fflush with doFsync=0 should return 0 (success)");

  int close_rc = sqlite3_quota_fclose(p);
  // Ignore close return, focus on flush outcome
  (void)close_rc;

  std::remove(tmpNameTemplate);
}

// Test 2: fflush path with doFsync=1; simulate fsync failure -> expect non-zero return
static void test_fflush_with_fsync_failure(){
  // Reset the override to force fsync failure path
  g_fsync_should_fail = 1;

  char tmpNameTemplate[] = "/tmp/sqlquota_testfsyncfail_XXXXXX.txt";
  int fd = mkstemps(tmpNameTemplate, 4);
  if (fd == -1) {
    std::cerr << "[WARN] Unable to create temporary file for test_fflush_with_fsync_failure" << std::endl;
    g_fsync_should_fail = 0;
    return;
  }
  close(fd);

  quota_FILE *p = sqlite3_quota_fopen(tmpNameTemplate, "w+");
  if (p == nullptr) {
    std::cerr << "[WARN] sqlite3_quota_fopen failed in test_fflush_with_fsync_failure" << std::endl;
    std::remove(tmpNameTemplate);
    g_fsync_should_fail = 0;
    return;
  }

  int rc = sqlite3_quota_fflush(p, 1); // doFsync = 1; fsync will be forced to fail
  EXPECT_EQ(rc, 1, "sqlite3_quota_fflush with doFsync=1 and fsync failure should return non-zero");

  sqlite3_quota_fclose(p);
  std::remove(tmpNameTemplate);

  // Reset to default for subsequent tests
  g_fsync_should_fail = 0;
}

// Test 3: fflush path with doFsync=1; simulate fsync success -> expect zero return
static void test_fflush_with_fsync_success(){
  // Ensure fsync reports success
  g_fsync_should_fail = 0;

  char tmpNameTemplate[] = "/tmp/sqlquota_testfsyncsucc_XXXXXX.txt";
  int fd = mkstemps(tmpNameTemplate, 4);
  if (fd == -1) {
    std::cerr << "[WARN] Unable to create temporary file for test_fflush_with_fsync_success" << std::endl;
    return;
  }
  close(fd);

  quota_FILE *p = sqlite3_quota_fopen(tmpNameTemplate, "w+");
  if (p == nullptr) {
    std::cerr << "[WARN] sqlite3_quota_fopen failed in test_fflush_with_fsync_success" << std::endl;
    std::remove(tmpNameTemplate);
    return;
  }

  int rc = sqlite3_quota_fflush(p, 1); // doFsync = 1; fsync will report success via override
  EXPECT_EQ(rc, 0, "sqlite3_quota_fflush with doFsync=1 and fsync success should return 0");

  sqlite3_quota_fclose(p);
  std::remove(tmpNameTemplate);
}

// ---------------------------------------------------------------------------------
// Main: run all tests
// ---------------------------------------------------------------------------------
int main(){
  std::cout << "Starting sqlite3_quota_fflush unit tests (non-GTest, lightweight harness)" << std::endl;

  run_and_report("test_fflush_no_fsync_success", test_fflush_no_fsync_success);
  run_and_report("test_fflush_with_fsync_failure", test_fflush_with_fsync_failure);
  run_and_report("test_fflush_with_fsync_success", test_fflush_with_fsync_success);

  std::cout << "Tests completed. Total: " << g_total_tests
            << "  Failures: " << g_failed_tests << std::endl;
  // Return non-zero if any test failed
  return g_failed_tests != 0 ? 1 : 0;
}