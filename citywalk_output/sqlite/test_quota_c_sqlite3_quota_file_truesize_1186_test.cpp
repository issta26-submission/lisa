// Minimal C++11 test suite for sqlite3_quota_file_truesize
// Focus: UNIX path of sqlite3_quota_file_truesize as defined in the provided focal method.
// Notes:
// - We assume the UNIX path is compiled into the tested binary (SQLITE_OS_UNIX defined).
// - quota_FILE is treated as a lightweight struct with the first member 'FILE *f' to
//   align with the usage in sqlite3_quota_file_truesize (p->f).
// - The test uses a named temporary file to determine a known size and validates the
//   function returns that size for rc == 0.
// - A dedicated test harness is provided (no external testing framework required).

#include <string.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cstdlib>


// Domain knowledge hints: Use only standard library and provided interfaces.
// We declare a minimal compatible quota_FILE type and the target function signature.

typedef long long sqlite3_int64; // Provide compatible type alias for the test

// Lightweight local replica of quota_FILE to enable direct testing of the focal function.
// The actual library defines quota_FILE; we only need the first member 'FILE *f' for
// the UNIX path taken by sqlite3_quota_file_truesize.
struct quota_FILE {
  FILE *f;
  // Other members are present in the real struct, but are not accessed by the UNIX path.
};

// Forward declaration of the focal function under test.
// Note: We declare with C linkage to ensure proper name mangling when linking with the
// C-compiled sqlite3_quota_file_truesize implementation.
extern "C" sqlite3_int64 sqlite3_quota_file_truesize(struct quota_FILE *p);

// Helper: create a temporary named file with exactly 'size' bytes of data.
// Returns the filename via 'outName' and leaves the file opened for the caller to use.
// The file is opened in read/write binary mode. Caller is responsible for closing the FILE*.
static FILE* create_temp_file_with_size(size_t size, std::string &outName) {
  // Generate a unique temporary filename
  char tmpName[L_tmpnam] = {0};
  if (std::tmpnam(tmpName) == nullptr) return nullptr;
  outName = std::string(tmpName);

  // Open the file and write deterministic data
  FILE *fp = std::fopen(outName.c_str(), "w+b");
  if (!fp) return nullptr;

  // Write 'A' characters to fill the file to 'size'
  const size_t chunk = 4096;
  size_t remaining = size;
  std::vector<char> buf(chunk, 'A');
  while (remaining > 0) {
    size_t toWrite = (remaining < chunk) ? remaining : chunk;
    size_t wrote = std::fwrite(buf.data(), 1, toWrite, fp);
    if (wrote != toWrite) {
      std::fclose(fp);
      std::remove(outName.c_str());
      return nullptr;
    }
    remaining -= toWrite;
  }
  std::fflush(fp);
  // Do not close the file; the test will call sqlite3_quota_file_truesize while it remains open.
  return fp;
}

// Simple non-terminating assertion macro: logs failure but continues test execution.
static int g_test_failures = 0;
#define CHECK_EQ(expected, actual, msg) \
  do { \
    if ((expected) != (actual)) { \
      std::cerr << "TEST_FAIL: " << (msg) \
                << " | expected: " << (expected) \
                << ", actual: " << (actual) << std::endl; \
      ++g_test_failures; \
    } \
  } while (0)

static void test_quota_file_truesize_unix_nonzero_size() {
  // Test that a known non-empty file reports its size via sqlite3_quota_file_truesize.
  // This exercises the rc == 0 branch on UNIX path.
  std::string name;
  size_t expected_size = 1234; // Known size
  FILE *fp = create_temp_file_with_size(expected_size, name);
  if (!fp) {
    std::cerr << "TEST_SETUP_FAIL: could not create temp file of size " << expected_size << std::endl;
    ++g_test_failures;
    return;
  }

  quota_FILE q;
  q.f = fp;

  // Call the focal function
  sqlite3_int64 result = sqlite3_quota_file_truesize(&q);

  // Validate result equals expected file size
  CHECK_EQ(static_cast<sqlite3_int64>(expected_size), result,
           "sqlite3_quota_file_truesize should return the size of the underlying file when rc==0");

  // Cleanup
  std::fclose(fp);
  std::remove(name.c_str());
}

static void test_quota_file_truesize_unix_zero_size() {
  // Test that an empty file reports size 0.
  std::string name;
  size_t expected_size = 0;
  FILE *fp = create_temp_file_with_size(expected_size, name);
  if (!fp) {
    std::cerr << "TEST_SETUP_FAIL: could not create empty temp file" << std::endl;
    ++g_test_failures;
    return;
  }

  quota_FILE q;
  q.f = fp;

  sqlite3_int64 result = sqlite3_quota_file_truesize(&q);

  CHECK_EQ(static_cast<sqlite3_int64>(expected_size), result,
           "sqlite3_quota_file_truesize should return 0 for empty file when rc==0");

  std::fclose(fp);
  std::remove(name.c_str());
}

// The test suite driver
int main() {
  std::cout << "Starting sqlite3_quota_file_truesize unit tests (UNIX path)..." << std::endl;

  test_quota_file_truesize_unix_nonzero_size();
  test_quota_file_truesize_unix_zero_size();

  if (g_test_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}