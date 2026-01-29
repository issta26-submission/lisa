/*
 * C++11 test suite for the focal method:
 *   int test_large_deflate(unsigned char *compr, size_t comprLen,
 *                          unsigned char *uncompr, size_t uncomprLen)
 *
 * Notes:
 * - This test harness assumes the object code for test_large_deflate is available
 *   (e.g., compiled from example_large_fuzzer.c) and linked into the test binary.
 * - The test uses a lightweight, non-terminating assertion style (EXPECT_* macros)
 *   to maximize code execution coverage without aborting tests on the first failure.
 * - The tests exercise the function with varying input sizes and content (zeroed input,
 *   empty input, and a small non-empty input) to exercise typical paths and some edge cases.
 * - The environment must link against zlib (provides zlib deflate primitives used by the focal method).
 *
 * How to run:
 * - Compile with C++11, ensuring zlib is linked, and with example_large_fuzzer.c compiled in.
 * - Example command (adjust as needed for your build system):
 *     g++ -std=c++11 -I<path-to-zlib> -L<path-to-zlib> -lz test_large_deflate_suite.cpp -o test_large_deflate_suite
 * - Run: ./test_large_deflate_suite
 */

// Ensure C linkage for the focal function declaration.
extern "C" int test_large_deflate(unsigned char *compr, size_t comprLen,
                                  unsigned char *uncompr, size_t uncomprLen);

#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cstddef>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>


// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg)                                                    \
  do {                                                                            \
    if (!(cond)) {                                                                \
      std::cerr << "EXPECT_TRUE failed: ( " #cond " ) at " << __FILE__ << ":"       \
                << __LINE__ << " - " << (msg) << std::endl;                     \
      ++g_failures;                                                               \
    }                                                                             \
  } while (0)

#define EXPECT_EQ(a, b, msg)                                                     \
  do {                                                                            \
    if (!((a) == (b))) {                                                          \
      std::cerr << "EXPECT_EQ failed: ( " #a " == " #b " ) at " << __FILE__ << ":"   \
                << __LINE__ << " - " << (msg) << " (got " << (a)                 \
                << " vs " << (b) << ")" << std::endl;                             \
      ++g_failures;                                                               \
    }                                                                             \
  } while (0)

// Test 1: Basic large_deflate path with zero-initialized uncompr data.
// Rationale: zeros compress very well; this exercise exercises the main
//            deflate/deflateParams/deflateEnd paths.
static void test_large_deflate_zeros_input_basic() {
  const size_t comprLen = 16 * 1024;   // 16 KB buffer for compressed data
  const size_t uncomprLen = 4 * 1024;  // 4 KB of input data (all zeros)

  std::vector<unsigned char> compr(comprLen);
  std::vector<unsigned char> uncompr(uncomprLen, 0); // all zeros

  // Call the focal function
  int ret = test_large_deflate(compr.data(), comprLen,
                               uncompr.data(), uncomprLen);

  // Expect the function to complete without returning an error code in the test harness
  EXPECT_TRUE(ret == 0, "test_large_deflate should return 0 for zero-initialized input");
}

// Test 2: Empty input path (uncomprLen == 0).
// Rationale: Ensure function handles no input data gracefully and drives
//            into subsequent deflate/deflateParams paths without crashing.
static void test_large_deflate_empty_input() {
  const size_t comprLen = 4 * 1024;    // 4 KB compressed buffer
  const size_t uncomprLen = 0;         // no input data

  std::vector<unsigned char> compr(comprLen);
  std::vector<unsigned char> uncompr(uncomprLen);

  int ret = test_large_deflate(compr.data(), comprLen,
                               uncompr.data(), uncomprLen);

  EXPECT_TRUE(ret == 0, "test_large_deflate with empty input should return 0");
}

// Test 3: Small non-empty input path (non-zero bytes).
// Rationale: Exercise normal compression path with non-empty data and ensure
//            function progresses through the greedy and subsequent paths.
static void test_large_deflate_small_nonempty_input() {
  const size_t comprLen = 8 * 1024;    // 8 KB compressed buffer
  const size_t uncomprLen = 256;       // 256 bytes of non-zero data

  std::vector<unsigned char> compr(comprLen);
  // Fill with a simple pattern to create non-trivial input
  std::vector<unsigned char> uncompr(uncomprLen);
  for (size_t i = 0; i < uncomprLen; ++i) {
    uncompr[i] = static_cast<unsigned char>(i & 0xFF);
  }

  int ret = test_large_deflate(compr.data(), comprLen,
                               uncompr.data(), uncomprLen);

  EXPECT_TRUE(ret == 0, "test_large_deflate with small non-empty input should return 0");
}

// Optional: a test to provoke the second major phase where input is the
// previously produced compressed data. This depends on internal behavior
// of the focal function and zlib; here we provide a larger input to broaden coverage.
static void test_large_deflate_large_input_coverage() {
  const size_t comprLen = 32 * 1024;    // 32 KB compressed buffer
  const size_t uncomprLen = 16 * 1024;  // 16 KB uncompressed

  std::vector<unsigned char> compr(comprLen);
  std::vector<unsigned char> uncompr(uncomprLen, 0); // zeros to encourage greediness

  int ret = test_large_deflate(compr.data(), comprLen,
                               uncompr.data(), uncomprLen);

  EXPECT_TRUE(ret == 0, "test_large_deflate with large input should return 0");
}

// Main entry point for the test suite
int main() {
  std::cout << "Starting test_large_deflate suite (non-GTest, non-terminating assertions)...\n";

  // Run individual test cases
  test_large_deflate_zeros_input_basic();
  test_large_deflate_empty_input();
  test_large_deflate_small_nonempty_input();
  test_large_deflate_large_input_coverage();

  if (g_failures == 0) {
    std::cout << "All tests completed. Status: PASS" << std::endl;
    return 0;
  } else {
    std::cout << "Tests completed with failures: " << g_failures << std::endl;
    return 1;
  }
}