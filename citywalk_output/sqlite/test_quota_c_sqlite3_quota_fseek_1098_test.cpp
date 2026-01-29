// Test suite for sqlite3_quota_fseek
// This test targets the focal method:
//   int sqlite3_quota_fseek(quota_FILE *p, long offset, int whence)
// It relies on the quota_FILE wrapper around a standard FILE* and delegates
// to the underlying fseek. We exercise true/false outcomes by moving the
// file pointer with SEEK_SET, SEEK_CUR, and SEEK_END and verify with
// sqlite3_quota_ftell.
// Notes:
// - We compile as C++11, but call C-style APIs (extern "C").
// - No GTest is used; a lightweight test harness is implemented.
// - We avoid accessing private/static internals and rely on public quota API.

// Domain knowledge and dependencies are reflected in the approach:
// - Use sqlite3_quota_fopen to obtain a quota_FILE* backed by a real FILE*.
// - Use sqlite3_quota_fseek to reposition and sqlite3_quota_ftell to verify.
// - Ensure proper cleanup (fclose) and temporary file removal.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cstdlib>


// Forward declaration of quota_FILE to avoid dependence on its internal layout.
// We only use the public API functions; the internal structure is opaque here.
struct quota_FILE;

// C declarations for the quota API.
// We assume the following functions are provided by the project under test.
extern "C" {
  // Open quota-wrapped file (returns NULL on failure)
  quota_FILE *sqlite3_quota_fopen(const char *zFilename, const char *zMode);

  // Seek wrapper around the underlying FILE*
  // Returns 0 on success, non-zero on error (consistent with fseek)
  int sqlite3_quota_fseek(quota_FILE *p, long offset, int whence);

  // Tell the current position in the quota-wrapped file
  long sqlite3_quota_ftell(quota_FILE *p);

  // Close the quota-wrapped file
  int sqlite3_quota_fclose(quota_FILE *p);
}

// Simple test framework helpers (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_REQUIRE(cond, msg)                    \
  do {                                             \
    ++g_total_tests;                               \
    if (!(cond)) {                                 \
      std::cerr << "Test failed: " << (msg) << "\n"; \
      ++g_failed_tests;                            \
    }                                              \
  } while (0)


// Test 1: Basic forward seeks using SEEK_SET and SEEK_END with a non-empty file.
// Steps:
// - Create a temp file with known content (10 bytes).
// - Open via sqlite3_quota_fopen.
// - Seek to 0 (SEEK_SET) and verify pos == 0.
// - Seek to 5 (SEEK_SET) and verify pos == 5.
// - Seek to -1 with SEEK_SET and verify failure (non-zero return).
// - Seek to end (SEEK_END) and verify pos == file size (10).
static bool test_quota_fseek_basic_sequence() {
  const char *tmpPath = "quota_test_basic.bin";

  // Create a 10-byte test file
  FILE *f = std::fopen(tmpPath, "wb");
  if (!f) {
    std::cerr << "Unable to create temp file for test_quota_fseek_basic_sequence\n";
    return false;
  }
  const char testData[10] = "0123456789";
  std::fwrite(testData, 1, 10, f);
  std::fclose(f);

  // Open quota-wrapped file
  quota_FILE *qp = sqlite3_quota_fopen(tmpPath, "r+");
  if (!qp) {
    std::cerr << "sqlite3_quota_fopen failed in test_quota_fseek_basic_sequence\n";
    std::remove(tmpPath);
    return false;
  }

  // Seek to 0 from start
  int rc = sqlite3_quota_fseek(qp, 0, SEEK_SET);
  TEST_REQUIRE(rc == 0, "sqlite3_quota_fseek(..., 0, SEEK_SET) should succeed");
  long pos = sqlite3_quota_ftell(qp);
  TEST_REQUIRE(pos == 0, "Position after SEEK_SET 0 should be 0");

  // Seek to 5 from start
  rc = sqlite3_quota_fseek(qp, 5, SEEK_SET);
  TEST_REQUIRE(rc == 0, "sqlite3_quota_fseek(..., 5, SEEK_SET) should succeed");
  pos = sqlite3_quota_ftell(qp);
  TEST_REQUIRE(pos == 5, "Position after SEEK_SET to 5 should be 5");

  // Negative offset should fail
  rc = sqlite3_quota_fseek(qp, -1, SEEK_SET);
  TEST_REQUIRE(rc != 0, "sqlite3_quota_fseek(..., -1, SEEK_SET) should fail");

  // Seek to end and verify size (10)
  rc = sqlite3_quota_fseek(qp, 0, SEEK_END);
  TEST_REQUIRE(rc == 0, "sqlite3_quota_fseek(..., 0, SEEK_END) should succeed");
  pos = sqlite3_quota_ftell(qp);
  TEST_REQUIRE(pos == 10, "Position after SEEK_END should be file size (10)");

  // Additional: move back by 3 from current using SEEK_CUR and verify
  rc = sqlite3_quota_fseek(qp, -3, SEEK_CUR);
  TEST_REQUIRE(rc == 0, "sqlite3_quota_fseek(..., -3, SEEK_CUR) should succeed");
  pos = sqlite3_quota_ftell(qp);
  TEST_REQUIRE(pos == 7, "Position after SEEK_CUR -3 from end should be 7");

  // Cleanup
  sqlite3_quota_fclose(qp);
  std::remove(tmpPath);
  return g_failed_tests == 0;
}

// Test 2: Negative tests for invalid whence should be rejected by caller handling.
// Note: The current sqlite3_quota_fseek implementation does not validate whence
// beyond passing to fseek, so this test focuses on a valid whence path.
// We intentionally reuse a valid file to ensure that only the whence path in this test remains valid.
// This test ensures behavior remains deterministic for valid whence values and does not crash.
static bool test_quota_fseek_whence_validation() {
  const char *tmpPath = "quota_test_whence.bin";

  // Create a 2-byte test file
  FILE *f = std::fopen(tmpPath, "wb");
  if (!f) {
    std::cerr << "Unable to create temp file for test_quota_fseek_whence_validation\n";
    return false;
  }
  const char data[2] = "AB";
  std::fwrite(data, 1, 2, f);
  std::fclose(f);

  // Open quota-wrapped file
  quota_FILE *qp = sqlite3_quota_fopen(tmpPath, "r+");
  if (!qp) {
    std::cerr << "sqlite3_quota_fopen failed in test_quota_fseek_whence_validation\n";
    std::remove(tmpPath);
    return false;
  }

  // Valid whence values should behave like standard fseek
  int rc = sqlite3_quota_fseek(qp, 1, SEEK_SET);
  TEST_REQUIRE(rc == 0, "SEEK_SET with offset 1 should succeed");
  long pos = sqlite3_quota_ftell(qp);
  TEST_REQUIRE(pos == 1, "Position after SEEK_SET 1 should be 1");

  rc = sqlite3_quota_fseek(qp, 0, SEEK_END);
  TEST_REQUIRE(rc == 0, "SEEK_END from current position should succeed");
  pos = sqlite3_quota_ftell(qp);
  TEST_REQUIRE(pos == 2, "Position after SEEK_END should be 2 (end of file)");

  // Cleanup
  sqlite3_quota_fclose(qp);
  std::remove(tmpPath);
  return g_failed_tests == 0;
}


// Main harness: run tests and report summary
int main() {
  std::cout << "Running sqlite3_quota_fseek unit tests (C++11 harness)\n";

  // Run tests
  bool t1 = test_quota_fseek_basic_sequence();
  // The first test prints its own diagnostics; we still count as passed if true.
  if (t1) {
    std::cout << "TEST PASSED: test_quota_fseek_basic_sequence\n";
  } else {
    std::cout << "TEST FAILED: test_quota_fseek_basic_sequence\n";
  }

  bool t2 = test_quota_fseek_whence_validation();
  if (t2) {
    std::cout << "TEST PASSED: test_quota_fseek_whence_validation\n";
  } else {
    std::cout << "TEST FAILED: test_quota_fseek_whence_validation\n";
  }

  int total = g_total_tests;
  int failed = g_failed_tests;
  std::cout << "Summary: total=" << total << " failed=" << failed << "\n";

  // Exit with non-zero if any test failed
  return failed ? 1 : 0;
}