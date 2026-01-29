// LSMDB test suite for the focal method: testDeleteLsmdb
// This test suite is designed to be compiled as C++11 source without GTest.
// It relies on the external C functions provided by the project under test:
// - testDeleteLsmdb(const char *zFile)
// - testMallocPrintf(const char *fmt, ...)
// - testFree(void *ptr)
// These are declared with C linkage to ensure proper linking.
// The tests create and remove files to verify correct behavior of testDeleteLsmdb.

#include <lsmtest.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <sys/stat.h>
#include <cstdlib>


#ifdef _WIN32
  #include <io.h>
  #define unlink _unlink
#else
  #include <unistd.h>
#endif

// External C functions from the project under test
extern "C" {
  void testDeleteLsmdb(const char *zFile);
  char* testMallocPrintf(const char *fmt, ...);
  void testFree(void *ptr);
}

// Simple lightweight test harness (non-terminating assertions)
static int g_failures = 0;

static void check(bool condition, const char* message) {
  if (condition) {
    printf("[PASS] %s\n", message);
  } else {
    printf("[FAIL] %s\n", message);
    ++g_failures;
  }
}

// Helper: Create an empty file at given path
static void createEmptyFile(const std::string& path) {
  FILE *f = fopen(path.c_str(), "wb");
  if (f) {
    fclose(f);
  }
}

// Helper: Check if a file exists at given path
static bool fileExists(const std::string& path) {
  struct stat st;
  return (stat(path.c_str(), &st) == 0);
}

// Candidate Keywords (Step 1)
//
// - testDeleteLsmdb: focal method under test
// - testMallocPrintf / testFree: memory management utilities used to allocate
//   and free auxiliary strings (zLog, zShm)
// - unlink: POSIX file removal used by the focal method
// - zFile / zFile-log / zFile-shm: derived filenames that must be removed
// - zFile base path: used to construct dependent file names
//
// These keywords reflect the core dependencies and behaviors we must cover.


// Test 1: When all three files exist, testDeleteLsmdb should remove them.
// This verifies both zFile and the derived files are unlinked as expected.
static void test_delete_lsmdb_removes_all_files() {
  // Construct a unique base path in /tmp
  pid_t pid = getpid();
  std::string base = "/tmp/lsmtest_del_" + std::to_string(pid) + "_1";

  std::string zFile = base;
  std::string zLog  = zFile + "-log";
  std::string zShm  = zFile + "-shm";

  // Create all three files with some content (the content is irrelevant)
  createEmptyFile(zFile);
  createEmptyFile(zLog);
  createEmptyFile(zShm);

  // Pre-conditions: files exist
  check(fileExists(zFile), "precondition: zFile exists");
  check(fileExists(zLog),  "precondition: zLog exists");
  check(fileExists(zShm),  "precondition: zShm exists");

  // Call the focal function
  testDeleteLsmdb(zFile.c_str());

  // Post-conditions: all three files should be removed
  check(!fileExists(zFile), "postcondition: zFile removed");
  check(!fileExists(zLog),  "postcondition: zLog removed");
  check(!fileExists(zShm),  "postcondition: zShm removed");
}

// Test 2: When files do not exist, testDeleteLsmdb should handle gracefully
// without crashing or leaking resources.
static void test_delete_lsmdb_handles_missing_files_gracefully() {
  // Use a base path that is almost certainly non-existent
  pid_t pid = getpid();
  std::string base = "/tmp/lsmtest_del_" + std::to_string(pid) + "_2_nonexistent";

  std::string zFile = base;
  std::string zLog  = zFile + "-log";
  std::string zShm  = zFile + "-shm";

  // Ensure none of the files exist before invocation
  if (fileExists(zFile)) { unlink(zFile.c_str()); }
  if (fileExists(zLog))  { unlink(zLog.c_str()); }
  if (fileExists(zShm))  { unlink(zShm.c_str()); }

  check(!fileExists(zFile), "precondition: zFile does not exist");
  check(!fileExists(zLog),  "precondition: zLog does not exist");
  check(!fileExists(zShm),  "precondition: zShm does not exist");

  // Call the focal function (files do not exist; unlink should simply fail silently)
  testDeleteLsmdb(zFile.c_str());

  // Post-conditions: still no such files exist
  check(!fileExists(zFile), "postcondition: zFile still does not exist");
  check(!fileExists(zLog),  "postcondition: zLog still does not exist");
  check(!fileExists(zShm),  "postcondition: zShm still does not exist");
}

// Entry point for test execution (Step 3: Test Case Refinement is embedded in tests)
int main() {
  printf("Starting test suite for testDeleteLsmdb (C++11, no GTest)\n");

  // Run tests
  test_delete_lsmdb_removes_all_files();
  test_delete_lsmdb_handles_missing_files_gracefully();

  // Summary
  if (g_failures == 0) {
    printf("All tests passed.\n");
  } else {
    printf("Total failures: %d\n", g_failures);
  }

  // Return non-zero if any test failed
  return (g_failures == 0) ? 0 : 1;
}