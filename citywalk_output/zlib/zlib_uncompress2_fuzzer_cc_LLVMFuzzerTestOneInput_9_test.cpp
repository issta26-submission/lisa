// Fuzzer Test Suite for LLVMFuzzerTestOneInput (zlib_uncompress2_fuzzer.cc)
// This test suite is designed for C++11 without GoogleTest.
// It calls the focal function LLVMFuzzerTestOneInput and exercises a few representative inputs.
// Explanatory comments are provided for each unit test.

// Step 1: Candidate Keywords (core dependencies)
// - uncompress2: the decompress function invoked by the focal method
// - buffer: pre-allocated destination buffer used by uncompress2
// - data, size: input compressed data and its length
// - buffer_length, buf_size: sizes used for uncompress2 call
// - The test links against zlib (for uncompress2) and relies on an externally defined 'buffer'.
// This test ensures compilation against the focal interface without modifying production code.

#include <cstdio>
#include <cstring>
#include <stddef.h>
#include <string.h>
#include <cstdint>
#include <cstddef>
#include <stdint.h>
#include <vector>
#include <zlib.h>


// Declaration of the focal function under test (extern "C" to match original linkage)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

// The original implementation references a global 'buffer'. We declare it here
// as an extern to bind to the production symbol defined in the object under test.
// The actual size is determined within the production file; an incomplete type is okay for linkage.
extern unsigned char buffer[];

// Lightweight, non-terminating test framework (no GTest required).
static int g_total = 0;
static int g_failed = 0;

// Simple assertion helper: logs failure but does not abort execution
#define TEST_ASSERT(cond, msg) do { \
  ++g_total; \
  if (!(cond)) { \
    ++g_failed; \
    std::printf("ASSERT_FAIL: %s (in %s:%d)\n", (msg), __FILE__, __LINE__); \
  } else { \
    std::printf("ASSERT_PASS: %s\n", (msg)); \
  } \
} while (0)

// Utility to safely obtain a data pointer for possibly empty vectors
static const uint8_t* data_ptr(const std::vector<uint8_t>& v) {
  return v.empty() ? nullptr : v.data();
}

// Step 2: Unit Test Generation
// Producing a small but representative suite to exercise LLVMFuzzerTestOneInput.
// The test focuses on calling the focal method with various input sizes and patterns.
// Rationale: The function does not expose return values beyond 0, and its behavior is
// driven by the underlying uncompress2 call. We still ensure the call path is exercised
// without crashing, and that the function returns 0 as documented by the production code.

// Test 1: Basic small input
// - Purpose: Ensure that a short data payload does not crash and returns 0.
// - Coverage: validates normal path with a few bytes of input.
static void test_basic_small_input() {
  std::printf("Running test_basic_small_input...\n");
  std::vector<uint8_t> input = {0x01, 0x02, 0x03, 0x04};
  int ret = LLVMFuzzerTestOneInput(data_ptr(input), input.size());
  TEST_ASSERT(ret == 0, "LLVMFuzzerTestOneInput returns 0 for small input");
}

// Test 2: Empty input
// - Purpose: Validate behavior when size == 0 (no data provided).
// - Coverage: branch where input length is zero; ensures no crash and returns 0.
static void test_empty_input() {
  std::printf("Running test_empty_input...\n");
  std::vector<uint8_t> input; // empty
  int ret = LLVMFuzzerTestOneInput(data_ptr(input), input.size());
  TEST_ASSERT(ret == 0, "LLVMFuzzerTestOneInput returns 0 for empty input");
}

// Test 3: Moderate input (boundary around typical buffer size)
// - Purpose: Exercise the decompression path with a reasonable input size.
// - Coverage: checks that function handles non-trivial input without error (return 0).
static void test_moderate_input() {
  std::printf("Running test_moderate_input...\n");
  // Size chosen to be around the likely destination buffer (e.g., 512 bytes)
  std::vector<uint8_t> input(512);
  for (size_t i = 0; i < input.size(); ++i) {
    input[i] = static_cast<uint8_t>(i & 0xFF);
  }
  int ret = LLVMFuzzerTestOneInput(data_ptr(input), input.size());
  TEST_ASSERT(ret == 0, "LLVMFuzzerTestOneInput returns 0 for moderate input (512 bytes)");
}

// Test 4: Large input (larger than typical buffer to test robustness)
// - Purpose: Ensure stability when large compressed data is provided.
// - Coverage: increases exploration of internal uncompress2 behavior.
static void test_large_input() {
  std::printf("Running test_large_input...\n");
  std::vector<uint8_t> input(1024);
  for (size_t i = 0; i < input.size(); ++i) {
    input[i] = static_cast<uint8_t>((i * 7) & 0xFF);
  }
  int ret = LLVMFuzzerTestOneInput(data_ptr(input), input.size());
  TEST_ASSERT(ret == 0, "LLVMFuzzerTestOneInput returns 0 for large input (1024 bytes)");
}

// Test 5: Very large input (stress test beyond typical usage)
// - Purpose: Push the decompression path with maximum test size in this suite.
// - Coverage: robustness check; ensure no crash and consistent return value.
static void test_very_large_input() {
  std::printf("Running test_very_large_input...\n");
  std::vector<uint8_t> input(2048);
  for (size_t i = 0; i < input.size(); ++i) {
    input[i] = static_cast<uint8_t>((i * 13) & 0xFF);
  }
  int ret = LLVMFuzzerTestOneInput(data_ptr(input), input.size());
  TEST_ASSERT(ret == 0, "LLVMFuzzerTestOneInput returns 0 for very large input (2048 bytes)");
}

// Step 3: Test Case Refinement
// The following main() function orchestrates the tests and reports the outcome.
// This avoids dependency on any test framework and adheres to the constraint of not using GTest.
// All tests are designed to be non-terminating: failures are logged but do not abort execution.

int main() {
  // Informational header
  std::printf("Starting zlib_uncompress2_fuzzer.cc unit tests (C++11, no GTest)\n");

  // Execute test cases
  test_basic_small_input();
  test_empty_input();
  test_moderate_input();
  test_large_input();
  test_very_large_input();

  // Summary
  std::printf("TEST SUMMARY: total=%d, failed=%d\n", g_total, g_failed);

  // Return non-zero if any test failed
  return (g_failed == 0) ? 0 : 1;
}