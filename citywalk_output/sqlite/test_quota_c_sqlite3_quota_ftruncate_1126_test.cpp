/*
  Unit test suite for sqlite3_quota_ftruncate (focal method)
  - Implemented in C code (test_quota.c)
  - Tests are written in C++11 (no GTest) and call the C function via extern "C"
  - Uses real file I/O to exercise ftruncate path on UNIX-like systems
  - Exercises three scenarios:
    1) Early return (-1) when quota file size is smaller than target truncation size and pFile is present
    2) Successful ftruncate and quota size updates when pFile exists and iSize >= szNew
    3) ftruncate path when there is no quota file bound to the FILE (pFile == nullptr)
  - Relies on test_quota.h to provide quota_FILE, quotaGroup, quotaFile structures and related fields
  - Provides explanatory comments for each test case
  - All tests are run from main with simple assertions and non-terminating error reporting
*/

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <test_quota.h>
#include <os_setup.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <cstdlib>


extern "C" {
  // Forward declaration of the focal function (C linkage)
  // The actual type sqlite3_int64 is defined in the test_quota.h header
  // We declare with extern "C" to avoid name mangling in C++
  struct quota_FILE;
  typedef long long sqlite3_int64;

  int sqlite3_quota_ftruncate(struct quota_FILE *p, sqlite3_int64 szNew);
}

// Include the quota test dependencies (structures like quota_FILE, quotaGroup, quotaFile)

// Simple non-terminating assertion macro: logs failures but does not abort
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "ASSERT FAILED: " << (msg) << " (Line " << __LINE__ << ")" << std::endl; \
    tests_failed++; \
  } else { \
    tests_passed++; \
  } \
} while(0)

static int tests_passed = 0;
static int tests_failed = 0;

/* RAII helper to manage a temporary file backing a quota_FILE in tests */
class TempQuotaFile {
public:
  std::string path;
  FILE *fp;

  TempQuotaFile(size_t initialSize) : path(""), fp(nullptr) {
    // Create a unique temporary file
    char tmpl[] = "/tmp/quota_test_XXXXXX";
    int fd = mkstemp(tmpl);
    if(fd == -1) {
      std::cerr << "Failed to create temp file" << std::endl;
      std::exit(1);
    }
    // Unlink immediately to avoid leaving a stray file if program terminates unexpectedly
    // But we still keep the path for inspection; unlinking ensures cleanup
    // NOTE: We keep the file until program exit for easier debugging; we'll remove explicitly in destructor
    path = tmpl;
    // Set the desired initial size
    if(ftruncate(fd, initialSize) != 0) {
      std::cerr << "Failed to set initial temp file size" << std::endl;
      close(fd);
      std::exit(1);
    }
    fp = fdopen(fd, "r+b");
    if(fp == nullptr) {
      std::cerr << "Failed to fdopen temp file" << std::endl;
      close(fd);
      std::exit(1);
    }
  }

  ~TempQuotaFile() {
    if(fp) fclose(fp);
    if(!path.empty()) {
      // Ensure file cleanup
      remove(path.c_str());
    }
  }

  // Disable copy to avoid double-close of FILE*
  TempQuotaFile(const TempQuotaFile&) = delete;
  TempQuotaFile& operator=(const TempQuotaFile&) = delete;

  // Allow move semantics if needed (not strictly required here)
  TempQuotaFile(TempQuotaFile&& other) noexcept : path(std::move(other.path)), fp(other.fp) {
    other.fp = nullptr;
  }
  TempQuotaFile& operator=(TempQuotaFile&& other) noexcept {
    if(this != &other) {
      if(fp) fclose(fp);
      path = std::move(other.path);
      fp = other.fp;
      other.fp = nullptr;
    }
    return *this;
  }
};

// Helper to stat a file size
static off_t file_size_on_disk(const char *path) {
  struct stat st;
  if(stat(path, &st) != 0) return -1;
  return st.st_size;
}

/*
  Test Case 1: Early return (-1) when p->pFile exists and pFile->iSize < szNew
  This path should short-circuit with -1 without performing any ftruncate
*/
static void test_early_return_when_file_size_smaller_than_target() {
  // Create a temp backing file of 40 bytes
  TempQuotaFile tfile(40);

  // Prepare quota structures
  static quotaGroup group;
  static quotaFile qfile;
  // Group holds an arbitrary initial iSize for a realistic scenario
  group.iSize = 100;
  group.pNext = nullptr;
  group.pFiles = &qfile;

  // quotaFile bound to the group with iSize smaller than target szNew
  qfile.iSize = 10;           // less than szNew to trigger early return
  qfile.pGroup = &group;

  // quota_FILE instance: f points to temp file, pFile points to qfile
  static quota_FILE q;
  q.f = tfile.fp;
  q.pFile = &qfile;

  // Execute focal method with szNew greater than current iSize
  sqlite3_int64 szNew = 20;
  int rc = sqlite3_quota_ftruncate(&q, szNew);

  // Assertions
  TEST_ASSERT(rc == -1, "Early return should produce rc = -1 when iSize < szNew and pFile non-null");
  // Sanity: disk size should remain unchanged
  off_t diskSize = file_size_on_disk(tfile.path.c_str());
  TEST_ASSERT(diskSize == 40, "Disk file size should remain unchanged after early return");
  // Sanity: quota in-memory sizes should remain unchanged
  TEST_ASSERT(qfile.iSize == 10, "quotaFile.iSize should remain unchanged after early return");
  TEST_ASSERT(group.iSize == 100, "quotaGroup.iSize should remain unchanged after early return");
}

/*
  Test Case 2: Successful ftruncate when pFile exists and iSize >= szNew
  Expect rc == 0, pFile->iSize updated to szNew, and pGroup->iSize adjusted accordingly
*/
static void test_ftruncate_and_update_sizes_when_allowed() {
  // Create a temp backing file of 80 bytes (will be truncated to 40 in this test)
  TempQuotaFile tfile(80);

  // Prepare quota structures
  static quotaGroup group;
  static quotaFile qfile;
  group.iSize = 100;
  group.pNext = nullptr;
  group.pFiles = &qfile;

  // Bind a quotaFile with iSize >= szNew so that ftruncate path is exercised
  qfile.iSize = 60;             // >= szNew
  qfile.pGroup = &group;

  static quota_FILE q;
  q.f = tfile.fp;
  q.pFile = &qfile;

  // szNew is smaller than current iSize to trigger actual truncation
  sqlite3_int64 szNew = 40;

  int rc = sqlite3_quota_ftruncate(&q, szNew);

  // Assertions
  TEST_ASSERT(rc == 0, "ftruncate should return 0 on successful truncation");
  // After operation, quotaFile iSize should be szNew
  TEST_ASSERT(qfile.iSize == szNew, "quotaFile.iSize should be updated to szNew after truncation");
  // Group iSize should reflect delta (szNew - old iSize)
  // Initial group.iSize = 100, delta = -20 => new iSize = 80
  TEST_ASSERT(group.iSize == 80, "quotaGroup.iSize should be adjusted by (szNew - old_iSize) -> 80");
  // Disk file size should be szNew
  off_t diskSize = file_size_on_disk(tfile.path.c_str());
  TEST_ASSERT(diskSize == (off_t)szNew, "Disk file size should be truncated to szNew (40)");
}

/*
  Test Case 3: Behavior when there is no quotaFile bound to the FILE (pFile == nullptr)
  Expect rc to reflect OS-level truncation and no quotaGroup modifications
*/
static void test_no_quota_bound_to_file() {
  // Create a temp backing file of 70 bytes
  TempQuotaFile tfile(70);

  // Prepare quota structures
  static quotaGroup group;
  static quotaFile qfile;
  group.iSize = 120;
  group.pNext = nullptr;
  group.pFiles = &qfile;

  // No pFile bound to FILE
  static quota_FILE q;
  q.f = tfile.fp;
  q.pFile = nullptr;

  int rc = sqlite3_quota_ftruncate(&q, 30);

  // Assertions
  TEST_ASSERT(rc == 0, "ftruncate should return 0 when truncating via OS call with no quotaFile bound");
  // quotaGroup should not be updated since pFile is null
  TEST_ASSERT(group.iSize == 120, "quotaGroup.iSize should remain unchanged when pFile is null");
  // quotaFile should remain unaffected (not accessible since pFile is null)
  // Disk file size should be 30
  off_t diskSize = file_size_on_disk(tfile.path.c_str());
  TEST_ASSERT(diskSize == 30, "Disk file size should be truncated to szNew even when pFile is null");
}

/* Entry point to run all tests */
int main(void) {
  std::cout << "Starting sqlite3_quota_ftruncate unit tests (C++11) ..." << std::endl;

  test_early_return_when_file_size_smaller_than_target();
  test_ftruncate_and_update_sizes_when_allowed();
  test_no_quota_bound_to_file();

  std::cout << "Tests completed." << std::endl;
  std::cout << "Passed: " << tests_passed << ", Failed: " << tests_failed << std::endl;

  return (tests_failed == 0) ? 0 : 1;
}