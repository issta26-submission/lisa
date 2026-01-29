// Comprehensive C++11 unit tests for sqlite3OsFullPathname
// This test suite uses a lightweight, homegrown test harness (no GTest) and mocks
// the sqlite3_vfs.xFullPathname behavior to validate sqlite3OsFullPathname.

#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <algorithm>


// Simple non-terminating test assertions (EXPECT_* style)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { std::cerr << "EXPECT_TRUE FAILED: " << (msg) << std::endl; ++g_failures; } } while(0)

#define EXPECT_EQ(a, b, msg) \
    do { if((a) != (b)) { std::cerr << "EXPECT_EQ FAILED: " << (a) << " != " << (b) << "; " << (msg) << std::endl; ++g_failures; } } while(0)

#define EXPECT_STR_EQ(expected, actual) \
    do { if(std::strcmp((actual), (expected)) != 0) { std::cerr << "EXPECT_STR_EQ FAILED: expected '" << (expected) << "', got '" << (actual) << "'\n"; ++g_failures; } } while(0)

#define EXPECT_STRNEQ(expected, actual) \
    do { if(std::strcmp((actual), (expected)) == 0) { std::cerr << "EXPECT_STRNEQ FAILED: strings are equal to '" << (expected) << "'\n"; ++g_failures; } } while(0)

// Forward declare the mock type so the xFullPathname function can cast to the derived type.
struct MockVfsContainer;

// The mock xFullPathname handler that simulates a real vfs's path resolution.
// It prefixes the input path with "FULL:" to produce a deterministic, testable output.
static int mock_xFullPathname(sqlite3_vfs* pVfs, const char* zPath, int nPathOut, char* zPathOut);

// Lightweight mock vfs container that embeds the real sqlite3_vfs as its first member.
// This allows sqlite3OsFullPathname to treat it as a valid sqlite3_vfs* and our mock
// function to cast back to MockVfsContainer* for state inspection.
struct MockVfsContainer {
  sqlite3_vfs base;       // Must be first to allow safe cast from sqlite3_vfs*
  int ret;                  // Return code to mimic xFullPathname's behavior
  std::string lastInput;    // Optional: remember last input path for validation
};

// Implementation of the mock xFullPathname
static int mock_xFullPathname(sqlite3_vfs* pVfs, const char* zPath, int nPathOut, char* zPathOut) {
  // Retrieve the derived object from the base pointer (base is the first field)
  MockVfsContainer* self = reinterpret_cast<MockVfsContainer*>(pVfs);
  const char* input = zPath ? zPath : "";
  self->lastInput = input;

  // Produce a deterministic full path representation
  std::string produced = std::string("FULL:") + self->lastInput;

  // Copy to zPathOut with respect to the provided buffer size
  int copyLen = static_cast<int>(produced.size());
  if (nPathOut <= 0) return self->ret;
  if (copyLen >= nPathOut) copyLen = nPathOut - 1;
  std::memcpy(zPathOut, produced.c_str(), static_cast<size_t>(copyLen));
  zPathOut[copyLen] = '\0';

  // Return the configured status
  return self->ret;
}

// Convenience helper to initialize a MockVfsContainer with sane defaults
static MockVfsContainer make_mock_vfs(int retCode) {
  MockVfsContainer v;
  std::memset(&v, 0, sizeof(v));         // Zero initialize fields
  v.ret = retCode;
  v.base.iVersion = 1;                   // Typical minimal vfs version
  v.base.xFullPathname = mock_xFullPathname;
  return v;
}

// Test 1: Normal path with ample output buffer
// Ensures sqlite3OsFullPathname delegates to xFullPathname and copies the expected result.
static void test_fullpath_normal() {
  // Domain: normal operation
  MockVfsContainer vfs = make_mock_vfs(0);
  char outBuf[256] = {0};

  const char* testPath = "/tmp/testfile.txt";
  int rc = sqlite3OsFullPathname((sqlite3_vfs*)&vfs, testPath, sizeof(outBuf), outBuf);

  // Assertions
  EXPECT_EQ(rc, 0, "Return code should be 0 for success");
  EXPECT_STR_EQ("FULL:/tmp/testfile.txt", outBuf);
  // Additional check: ensure the output is not empty and matches expected prefix
  EXPECT_TRUE(outBuf[0] == 'F', "Output should start with 'F' after full path computation");
  EXPECT_TRUE(outBuf[5] == ':', "Output should contain ':' after 'FULL' prefix");
}

// Test 2: Buffer too small to hold the full path
// Verifies truncation behavior and that no buffer overruns occur.
static void test_fullpath_small_buffer() {
  MockVfsContainer vfs = make_mock_vfs(0);
  // Intentionally small buffer: nPathOut = 6 -> can hold 5 characters + NUL
  char outBuf[6] = {0};

  int rc = sqlite3OsFullPathname((sqlite3_vfs*)&vfs, "abcdef", sizeof(outBuf), outBuf);

  // The real produced string would be "FULL:abcdef" which is longer than 5 chars.
  // The test buffer can hold 5 chars + NUL -> expect "FULL:" (first 5 chars)
  EXPECT_EQ(rc, 0, "Return code should be 0 even on small buffers");
  EXPECT_STR_EQ("FULL:", outBuf);
}

// Test 3: xFullPathname returns non-zero (error) but still ensures output buffer handling
// Verifies that sqlite3OsFullPathname propagates the return code correctly.
static void test_fullpath_nonzero_return() {
  MockVfsContainer vfs = make_mock_vfs(1); // Simulate an error code from xFullPathname
  char outBuf[256] = {0};

  int rc = sqlite3OsFullPathname((sqlite3_vfs*)&vfs, "/path/to/file", sizeof(outBuf), outBuf);

  // Expect the same non-zero return code; the output buffer still receives data from xFullPathname
  EXPECT_EQ(rc, 1, "Return code should propagate the value set in the vfs mock");
  EXPECT_STR_EQ("FULL:/path/to/file", outBuf);
}

// Test 4: Empty path handling
// Ensures that an empty input path yields a predictable full path string.
static void test_fullpath_empty_input() {
  MockVfsContainer vfs = make_mock_vfs(0);
  char outBuf[256] = {0};

  int rc = sqlite3OsFullPathname((sqlite3_vfs*)&vfs, "", sizeof(outBuf), outBuf);

  EXPECT_EQ(rc, 0, "Return code should be 0 for empty path");
  EXPECT_STR_EQ("FULL:", outBuf); // When input is empty, output should be "FULL:"
}

// Main test runner
int main() {
  std::cout << "Starting sqlite3OsFullPathname unit tests (C++11, no GTest)..." << std::endl;

  test_fullpath_normal();
  test_fullpath_small_buffer();
  test_fullpath_nonzero_return();
  test_fullpath_empty_input();

  if (g_failures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << g_failures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}