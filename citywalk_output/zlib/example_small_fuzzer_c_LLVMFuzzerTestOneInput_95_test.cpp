// Auto-generated C++11 unit test suite for LLVMFuzzerTestOneInput
// Based on the provided focal method and class dependencies.
// Notes:
// - Tests are written without Google Test; a lightweight in-file test harness is used.
// - Focus is on exercising input size validation, global data/state handling, and
//   memory allocation/cleanup paths within LLVMFuzzerTestOneInput.
// - Explanatory comments accompany each unit test for clarity and maintainability.

#include <cstring>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Include zlib for compressBound (as used by the focal method)

// Focal-class-like globals and stubs (as seen in <FOCAL_CLASS_DEP> excerpt)
static const uint8_t *data = nullptr; // mirrors the "data" global used by focal method
static size_t dataLen = 0;            // mirrors the "dataLen" global used by focal method

// Stubs for test_deflate/test_inflate as per the focal dependencies
// These are intentionally minimal; the real project would provide proper behavior.
// They are provided here to ensure the focal method compiles and can be exercised.
int test_deflate(unsigned char *compr, size_t comprLen) {
  // Minimal stub: pretend to write something small to compr if possible
  if (compr && comprLen > 0) {
    compr[0] = 0; // deterministic placeholder
  }
  return 0;
}

int test_inflate(unsigned char *compr, size_t comprLen, unsigned char *uncompr, size_t uncomprLen) {
  // Minimal stub: pretend to "inflate" by touching memory
  if (compr && comprLen > 0 && uncompr && uncomprLen > 0) {
    uncompr[0] = 0;
  }
  return 0;
}

// Focal method under test replicated in this harness for direct invocation.
// Signature matches the original: int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size) {
  size_t comprLen = compressBound(size);
  size_t uncomprLen = size;
  uint8_t *compr, *uncompr;

  // Discard inputs larger than 1Mb.
  static size_t kMaxSize = 1024 * 1024;
  if (size < 1 || size > kMaxSize)
    return 0;

  data = d;
  dataLen = size;

  compr = (uint8_t *)calloc(1, comprLen);
  uncompr = (uint8_t *)calloc(1, uncomprLen);

  test_deflate(compr, comprLen);
  test_inflate(compr, comprLen, uncompr, uncomprLen);

  free(compr);
  free(uncompr);

  /* This function must return 0. */
  return 0;
}

// Lightweight in-file test harness.
// Provides non-terminating assertions: errors are reported but do not abort tests.
static int g_totalTests = 0;
static int g_failedTests = 0;

#define T_PASSED(msg)  do { std::cout << "[OK] " << msg << "\n"; } while (0)
#define T_FAILED(msg)  do { std::cerr << "[FAIL] " << msg << "\n"; g_failedTests++; } while (0)
#define T_CHECK(cond, name) do { \
  ++g_totalTests; \
  if (!(cond)) { T_FAILED(std::string("Test '").append(name).append("' failed.").c_str()); } \
  else { T_PASSED(std::string("Test '").append(name).append("' passed.").c_str()); } \
} while (0)

// Helper to run a test case that expects a return value of 0
static void run_test_case_valid_input(const std::vector<uint8_t>& input, const std::string& testName) {
  // Before call: capture expected data pointer
  const uint8_t* expected_ptr = input.data();
  size_t expected_len = input.size();

  // Call the focal function
  int ret = LLVMFuzzerTestOneInput(expected_ptr, input.size());

  // After call: verify return code and internal state consistency
  bool ret_ok = (ret == 0);
  bool data_ptr_ok = (data == expected_ptr);
  bool data_len_ok = (dataLen == expected_len);

  // Aggregate results using non-terminating assertions
  // inc total tests and log pass/fail for each assertion to maximize coverage
  T_CHECK(ret_ok, testName + " - return == 0");
  T_CHECK(data_ptr_ok, testName + " - data pointer preserved");
  T_CHECK(data_len_ok, testName + " - dataLen reflects input size");
}

// Test 0: Edge case - size = 0 with null data pointer
// Expected behavior: immediate return 0, no dereferencing of data
static void test_edge_zero_size() {
  const char* testName = "EdgeCase_Size0_NullData";
  // Initialize globals to known state
  data = nullptr;
  dataLen = 0;

  int ret = LLVMFuzzerTestOneInput(nullptr, 0);

  // Validate behavior
  bool ret_ok = (ret == 0);
  bool data_ptr_ok = (data == nullptr);
  bool data_len_ok = (dataLen == 0);

  T_CHECK(ret_ok, testName + " - return == 0");
  T_CHECK(data_ptr_ok, testName + " - data remains nullptr");
  T_CHECK(data_len_ok, testName + " - dataLen remains 0");
}

// Test 1: Valid small input (size = 1)
static void test_small_input_size1() {
  std::vector<uint8_t> input(1, 0xAB);
  test_small_input_size1_name:
  {
    const std::string testName = "ValidInput_Size1";
    run_test_case_valid_input(input, testName);
  }
}

// Test 2: Valid large input (size = 1 MB)
static void test_large_input_size1MB() {
  // Allocate 1 MB of data
  std::vector<uint8_t> input(1024 * 1024, 0x5A);
  const std::string testName = "ValidInput_Size1MB";
  run_test_case_valid_input(input, testName);
}

// Test 3: Input larger than maximum allowed (size > 1MB) should be rejected early
// This test passes a nullptr for data to ensure no dereferencing occurs.
static void test_over_max_size() {
  const size_t maxSize = 1024 * 1024;
  const size_t tooBig = maxSize + 1;
  // Reset global state to a known value
  data = nullptr;
  dataLen = 0;

  int ret = LLVMFuzzerTestOneInput(nullptr, tooBig);

  bool ret_ok = (ret == 0);
  // Since size > kMaxSize, the function should return early without touching global data
  bool data_ptr_ok = (data == nullptr);
  bool data_len_ok = (dataLen == 0);

  T_CHECK(ret_ok, "OverflowTest_Size_gtMax");
  T_CHECK(data_ptr_ok, "OverflowTest_NoDataPointerAfterOverflow");
  T_CHECK(data_len_ok, "OverflowTest_DataLen_unchanged");
}

// Main: orchestrates all tests and prints a summary.
// Per instructions: if gtest is not allowed, call test methods from main.
int main() {
  std::cout << "Starting LLVMFuzzerTestOneInput unit test suite (no GTest).\n";

  // Run individual tests with explanatory comments
  // 0) Edge case: size=0 with null data
  test_edge_zero_size();

  // 1) Small valid input
  test_small_input_size1();

  // 2) Large valid input (1MB)
  test_large_input_size1MB();

  // 3) Input exceeding maximum size
  test_over_max_size();

  // Summary
  std::cout << "Test suite finished. Total cases: " << g_totalTests
            << ", Failures: " << g_failedTests << ".\n";

  // Return non-zero if any test failed to aid integration with CI systems
  return g_failedTests == 0 ? 0 : 1;
}