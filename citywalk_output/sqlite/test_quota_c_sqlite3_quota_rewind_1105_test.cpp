// C++11 test suite for the focal method: sqlite3_quota_rewind
// This test harness uses plain C++ without any testing framework.
// It exercises sqlite3_quota_rewind by creating quota_FILE objects via the
// provided C API (sqlite3_quota_fopen / sqlite3_quota_fread / sqlite3_quota_rewind / sqlite3_quota_fclose)
// and verifies that after rewinding, reads start from the beginning again.
//
// Important: The quota_FILE type is opaque to the C++ test code. We declare it as an
// incomplete type to interoperate with the C API. The C implementation (test_quota.c)
// uses the actual internal layout (including the member 'f') to implement rewind.
// We only pass pointers to quota_FILE to the C API; we do not dereference it in C++.
//

#include <string.h>
#include <chrono>
#include <assert.h>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cstdlib>


extern "C" {
  // Forward declaration to match the C API without exposing internal layout.
  // The C implementation defines the actual structure of quota_FILE.
  struct quota_FILE;
  struct quota_FILE; // opaque placeholder

  // Prototypes of the quota API functions provided by the project under test.
  struct quota_FILE* sqlite3_quota_fopen(const char *zFilename, const char *zMode);
  size_t sqlite3_quota_fread(void *pBuf, size_t size, size_t nmemb, struct quota_FILE *p);
  int sqlite3_quota_fclose(struct quota_FILE *p);
  void sqlite3_quota_rewind(struct quota_FILE *p);
  // Additional primitives (not strictly needed for these tests) could be declared if used:
  // size_t sqlite3_quota_fwrite(...);
  // int sqlite3_quota_ftruncate(...);
}

// Simple non-terminating test assertion mechanism.
// It logs failures but continues execution to maximize coverage.
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE FAILED: " << (msg) << std::endl; \
    ++g_failures; \
  } \
} while(0)


// Helper: Create a temporary file with given content and return its path.
// The file is removed by the caller after tests finish.
static std::string create_temp_file(const std::string &content) {
  // Generate a reasonably unique path in /tmp
  auto now = std::chrono::system_clock::now().time_since_epoch().count();
  std::string path = "/tmp/qtquota_test_" + std::to_string(now) + ".dat";

  std::ofstream ofs(path, std::ios::binary);
  if(!ofs) return ""; // signal failure by returning empty path
  ofs.write(content.data(), (std::streamsize)content.size());
  ofs.close();
  return path;
}

static void cleanup_file(const std::string &path) {
  if(!path.empty()) {
    std::remove(path.c_str());
  }
}

// Test 1: Basic read, then rewind, then read again should yield the same data.
static void test_quota_rewind_basic_read_after_rewind() {
  std::cout << "Test 1: Basic read then rewind should reset to beginning\n";

  const std::string data = "HelloWorld"; // 10 bytes
  std::string path = create_temp_file(data);
  EXPECT_TRUE(!path.empty(), "Failed to create temporary file for Test 1");

  struct quota_FILE *p = sqlite3_quota_fopen(path.c_str(), "rb");
  EXPECT_TRUE(p != nullptr, "sqlite3_quota_fopen returned null in Test 1");

  char buf1[6];
  memset(buf1, 0, sizeof(buf1));
  size_t got1 = sqlite3_quota_fread(buf1, 1, 5, p);
  buf1[5] = '\0';
  EXPECT_TRUE(got1 == 5, "Test 1: first read should return 5 bytes");
  EXPECT_TRUE(std::string(buf1) == "Hello", "Test 1: first read content should be 'Hello'");

  sqlite3_quota_rewind(p);

  char buf2[6];
  memset(buf2, 0, sizeof(buf2));
  size_t got2 = sqlite3_quota_fread(buf2, 1, 5, p);
  buf2[5] = '\0';
  EXPECT_TRUE(got2 == 5, "Test 1: second read after rewind should return 5 bytes");
  EXPECT_TRUE(std::string(buf2) == "Hello", "Test 1: second read content after rewind should be 'Hello'");

  int rc = sqlite3_quota_fclose(p);
  EXPECT_TRUE(rc == 0, "Test 1: sqlite3_quota_fclose should return 0 on success");

  cleanup_file(path);
  std::cout << "Test 1 completed.\n";
}

// Test 2: Read to end, then rewind, then read again from start.
static void test_quota_rewind_after_end_of_file() {
  std::cout << "Test 2: Rewind after reaching EOF should reset to start\n";

  const std::string data = "abcdef"; // 6 bytes
  std::string path = create_temp_file(data);
  EXPECT_TRUE(!path.empty(), "Failed to create temporary file for Test 2");

  struct quota_FILE *p = sqlite3_quota_fopen(path.c_str(), "rb");
  EXPECT_TRUE(p != nullptr, "sqlite3_quota_fopen returned null in Test 2");

  char buf1[7];
  memset(buf1, 0, sizeof(buf1));
  size_t got1 = sqlite3_quota_fread(buf1, 1, 6, p);
  buf1[6] = '\0';
  EXPECT_TRUE(got1 == 6, "Test 2: read to end should return 6 bytes");
  EXPECT_TRUE(std::string(buf1) == "abcdef", "Test 2: content read should be 'abcdef'");

  // Attempt to read again at EOF; should return 0
  char buf_rest[2];
  memset(buf_rest, 0, sizeof(buf_rest));
  size_t got_rest = sqlite3_quota_fread(buf_rest, 1, 1, p);
  EXPECT_TRUE(got_rest == 0, "Test 2: read after EOF should return 0");

  sqlite3_quota_rewind(p);

  char buf2[7];
  memset(buf2, 0, sizeof(buf2));
  size_t got2 = sqlite3_quota_fread(buf2, 1, 6, p);
  buf2[6] = '\0';
  EXPECT_TRUE(got2 == 6, "Test 2: after rewind, read should return 6 bytes from start");
  EXPECT_TRUE(std::string(buf2) == "abcdef", "Test 2: content after rewind should be 'abcdef'");

  int rc = sqlite3_quota_fclose(p);
  EXPECT_TRUE(rc == 0, "Test 2: sqlite3_quota_fclose should return 0 on success after rewind");

  cleanup_file(path);
  std::cout << "Test 2 completed.\n";
}

// Test 3: Empty file should result in zero bytes read, and rewind should be safe.
static void test_quota_rewind_empty_file() {
  std::cout << "Test 3: Rewind on empty file should be safe and yield zero bytes\n";

  const std::string data = ""; // 0 bytes
  std::string path = create_temp_file(data);
  // Empty content creates a 0-length file; an empty temp file path should still be valid.
  if(path.empty()) {
    // If we fail to create, still attempt a minimal path approach
    // as a fallback, but mark test as failed.
    g_failures++;
    std::cerr << "Test 3: Failed to create temporary empty file\n";
    return;
  }

  struct quota_FILE *p = sqlite3_quota_fopen(path.c_str(), "rb");
  EXPECT_TRUE(p != nullptr, "sqlite3_quota_fopen returned null in Test 3");

  char buf[2];
  memset(buf, 0, sizeof(buf));
  size_t got = sqlite3_quota_fread(buf, 1, 1, p);
  EXPECT_TRUE(got == 0, "Test 3: read from empty file should return 0 bytes");

  sqlite3_quota_rewind(p);

  size_t got2 = sqlite3_quota_fread(buf, 1, 1, p);
  EXPECT_TRUE(got2 == 0, "Test 3: after rewind on empty file, read should still return 0 bytes");

  int rc = sqlite3_quota_fclose(p);
  EXPECT_TRUE(rc == 0, "Test 3: sqlite3_quota_fclose should return 0 on success");

  cleanup_file(path);
  std::cout << "Test 3 completed.\n";
}

int main() {
  // Run tests
  test_quota_rewind_basic_read_after_rewind();
  test_quota_rewind_after_end_of_file();
  test_quota_rewind_empty_file();

  if(g_failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cerr << g_failures << " TEST(S) FAILED\n";
    return 1;
  }
}