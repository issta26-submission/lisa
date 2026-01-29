/*
  Unit Test Suite for focal method: test_flush
  - Based on the provided focal method and class dependencies, this test suite verifies
    the behavior of test_flush under different conditions and inputs.
  - The tests are written in C++11 (without Google Test) and use a lightweight, non-terminating
    assertion approach to maximize code coverage.
  - The test suite expects to be linked with the existing project that provides the actual
    implementation of test_flush (and its dependent environment, including zlib and
    the data/definitions it relies on, e.g., data, dataLen, zalloc, zfree, CHECK_ERR).
  - The focus is on exercising core paths indicated by the focal code, including:
    - Successful deflate initialization and flush path
    - An intentional modification to the compressed output to force an error in the first block
    - The subsequent deflate finish path and error handling
    - End of deflate sequence and result returning
  - To maximize portability, the tests use extern "C" for the focal function prototype,
    and rely on zlib.h for types like z_size_t.

  Candidate Keywords (Step 1): deflateInit, deflate, deflateEnd, deflate (Z_FULL_FLUSH), deflate (Z_FINISH),
  Z_STREAM_END, Z_BUF_ERROR, Z_DEFAULT_COMPRESSION, Z_FULL_FLUSH, Z_FINISH, NEXT_IN/NEXT_OUT, avail_in,
  avail_out, total_out, compr, comprLen, data, dataLen, zalloc, zfree, CHECK_ERR macro.
  - These keywords map to key control flow and dependencies inside test_flush and its environment.
*/

#include <cstdio>
#include <cstring>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Include zlib for type definitions (z_size_t) and interaction with deflate API.

extern "C" {
  // Forward declaration for the focal function under test.
  // We assume the actual implementation is provided in the project under test.
  // Use C linkage to avoid C++ name mangling mismatches.
  int test_flush(unsigned char *compr, z_size_t *comprLen);
}

// Lightweight, non-terminating test harness (logs failures but continues execution).
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_FAILED(msg) do { \
    std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) << std::endl; \
    ++g_failed_tests; \
  } while (0)

#define TEST_PASSED() do { \
    std::cout << "[OK] " << __FUNCTION__ << std::endl; \
  } while (0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { TEST_FAILED(msg); } else { /* no-op when true */ } \
  } while (0)

#define EXPECT_FALSE(cond, msg) do { \
    ++g_total_tests; \
    if ((cond)) { TEST_FAILED(msg); } else { /* no-op when false */ } \
  } while (0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if (!((a) == (b))) { TEST_FAILED(msg); } \
  } while (0)

#define EXPECT_NE(a, b, msg) do { \
    ++g_total_tests; \
    if (!((a) != (b))) { TEST_FAILED(msg); } \
  } while (0)


// Test 1: Large output buffer path
// - Rationale: Exercise the normal compress/flush path with enough space for the entire output.
// - Expected: test_flush completes without fatal errors; comprLen is set to a positive value.
bool test_flush_large_buffer() {
  // Large buffer to accommodate the compressed data.
  const size_t BUF = 4096;
  unsigned char compr[BUF];
  z_size_t comprLen = BUF;

  // Call the focal method under test.
  int ret = test_flush(compr, &comprLen);

  // Validate: function returns 0 and produced some compressed data length.
  if (ret != 0) {
    TEST_FAILED("test_flush returned non-zero status on large buffer");
    return false;
  }
  EXPECT_TRUE(comprLen > 0, "test_flush did not produce any compressed output length on large buffer");
  TEST_PASSED();
  return true;
}

// Test 2: Small output buffer path
// - Rationale: Force a tighter output buffer to exercise the code path where the output space may
//   be insufficient, which can cause Z_BUF_ERROR / non-Z_STREAM_END scenarios.
// - Expected: test_flush handles limited space gracefully, and comprLen is updated.
bool test_flush_small_buffer() {
  // Small buffer to trigger potential buffer-related edge cases.
  const size_t BUF = 16;
  unsigned char compr[BUF];
  z_size_t comprLen = BUF;

  int ret = test_flush(compr, &comprLen);

  // Validate: function should still return 0 and set comprLen to some value.
  if (ret != 0) {
    TEST_FAILED("test_flush returned non-zero status on small buffer");
    return false;
  }
  EXPECT_TRUE(comprLen > 0, "test_flush did not update comprLen on small buffer");
  TEST_PASSED();
  return true;
}

// Test 3: Multiple sequential runs to ensure stability across invocations
// - Rationale: Reuse the same environment to verify no unexpected state persists
//   between calls that would corrupt subsequent runs.
// - Expected: Consecutive calls do not crash and each produce a non-zero comprLen.
bool test_flush_multiple_runs() {
  const size_t BUF1 = 1024;
  unsigned char compr1[BUF1];
  z_size_t comprLen1 = BUF1;

  const size_t BUF2 = 2048;
  unsigned char compr2[BUF2];
  z_size_t comprLen2 = BUF2;

  int ret1 = test_flush(compr1, &comprLen1);
  int ret2 = test_flush(compr2, &comprLen2);

  if (ret1 != 0 || ret2 != 0) {
    TEST_FAILED("test_flush failed on one or both sequential runs");
    return false;
  }

  EXPECT_TRUE(comprLen1 > 0, "First run produced no output length");
  EXPECT_TRUE(comprLen2 > 0, "Second run produced no output length");
  TEST_PASSED();
  return true;
}

// Test 4: Edge case with zero-sized data handled by the focal implementation
// - Rationale: If the underlying data length is zero (or effectively minimal), ensure
//   no crash occurs and comprLen is updated legitimately.
// - Note: We rely on the focal implementation's handling; this test focuses on stability.
bool test_flush_zero_data_path() {
  // We cannot directly set the internal data to zero without modifying the focal code,
  // but we still verify behavior with a small buffer; this acts as a stability check.
  const size_t BUF = 256;
  unsigned char compr[BUF];
  z_size_t comprLen = BUF;

  int ret = test_flush(compr, &comprLen);

  if (ret != 0) {
    TEST_FAILED("test_flush failed on zero-like data path (indirect)");
    return false;
  }
  EXPECT_TRUE(comprLen > 0, "test_flush produced no output length on zero-like data path");
  TEST_PASSED();
  return true;
}

// Step 3: Test Case Refinement
// - We included a mix of tests to cover true/false condition branches implicit in the
//   focal code path (deflate initialization success, deflate flush path, forced error via
//   compr[3]++, deflate end, and total_out handling). We rely on the non-terminating
//   assertions (non-fatal logging) to maximize execution coverage without aborting on
//   a single failure.
int main() {
  std::cout << "Starting test suite for focal method: test_flush" << std::endl;

  // Run individual tests
  bool ok1 = test_flush_large_buffer();
  bool ok2 = test_flush_small_buffer();
  bool ok3 = test_flush_multiple_runs();
  bool ok4 = test_flush_zero_data_path();

  // Aggregate results
  int total = g_total_tests;
  int failures = g_failed_tests;

  std::cout << "\nTest summary: " << (total - failures) << "/" << total << " passed, "
            << failures << " failed." << std::endl;

  // Return non-zero if any test failed to signal abnormal exit in environments that require it.
  return (failures == 0) ? 0 : 1;
}