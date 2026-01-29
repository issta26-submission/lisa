// Test suite for LLVMFuzzerTestOneInput in example_dict_fuzzer.c
// This suite exercises the focal function via its C interface, without relying on
// a specific unit testing framework. It uses simple, non-terminating assertions
// (logging failures but continuing execution) to maximize code coverage.
//
// Prerequisites: The test runner assumes the existence of the following C symbols
// from the project under test:
// - extern "C" int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size);
// - extern "C" int test_dict_deflate(unsigned char **compr, size_t *comprLen);
// - extern "C" int test_dict_inflate(unsigned char *compr, size_t comprLen);
//
// The test harness does not redefine these functions; it only calls into them.

#include <cstring>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cstdint>
#include <cstddef>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Prototypes of the focal function and its dependencies (C linkage)
extern "C" {
  int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size);
  int test_dict_deflate(unsigned char **compr, size_t *comprLen);
  int test_dict_inflate(unsigned char *compr, size_t comprLen);
}

// Simple non-terminating assertion mechanism: log failures but do not abort.
static int g_total = 0;
static int g_failed = 0;

#define NTN_ASSERT(cond, msg) do { \
  ++g_total; \
  if (!(cond)) { \
    std::cerr << "[ASSERT_FAIL] " << msg << " (case " << g_total << ")" << std::endl; \
    ++g_failed; \
  } \
} while(0)

// Helper: run a single input through LLVMFuzzerTestOneInput and expect 0 return.
static void run_case(const std::string &name, const std::vector<uint8_t> &input) {
  int ret = LLVMFuzzerTestOneInput(input.data(), input.size());
  NTN_ASSERT(ret == 0, "Expected 0 return from LLVMFuzzerTestOneInput for case: " + name);
}

//
// Test Case 1: Empty size (size == 0)
// Rationale: Triggers the early return path (size < 1) to ensure no dereferencing occurs.
// Expected: returns 0, no crash.
//
static void test_case_empty_size() {
  // Pass a null data pointer with size 0; function should immediately return 0.
  std::vector<uint8_t> input; // size 0
  run_case("empty_size", input);
}

//
// Test Case 2: Too large size (size > kMaxSize)
// Rationale: Triggers the upper-bound early return path.
// Expected: returns 0, no crash.
//
static void test_case_too_large_size() {
  // 100 * 1024 = 102400 is the kMaxSize inside the function; use one above.
  std::vector<uint8_t> input(102400 + 1, 0x01);
  run_case("too_large_size", input);
}

//
// Test Case 3: Normal input with dictionaryLen <= dataLen
// Rationale: Exercises the main path where dictionaryLen is clamped to dataLen if needed.
// Expected: returns 0.
//
static void test_case_normal_dictionary_len_ok() {
  // size 64, first byte = 32 => dictionaryLen = 32 <= 64
  std::vector<uint8_t> input(64, 0);
  input[0] = 32;
  // fill rest with a pattern
  for (size_t i = 1; i < input.size(); ++i) input[i] = static_cast<uint8_t>(i & 0xFF);
  run_case("normal_dictionary_len_ok", input);
}

//
// Test Case 4: dictionaryLen > dataLen (dictionaryLen will be clamped to dataLen)
// Rationale: Forces the branch where dictionaryLen would otherwise exceed dataLen.
// Expected: returns 0.
//
static void test_case_dictionary_len_exceeds_data_len() {
  // size 16, first byte 32 => dictionaryLen would be > dataLen, clamp to 16
  std::vector<uint8_t> input(16, 0);
  input[0] = 32;
  for (size_t i = 1; i < input.size(); ++i) input[i] = static_cast<uint8_t>((i * 3) & 0xFF);
  run_case("dictionary_len_exceeds_data_len", input);
}

//
// Test Case 5: Boundary condition at maximum allowed size (size == kMaxSize)
// Rationale: Exercises the upper edge of the allowed input size.
// Expected: returns 0.
//
static void test_case_boundary_max() {
  // Build a vector exactly 100 * 1024 bytes long
  static const size_t MAX_SIZE = 100 * 1024;
  std::vector<uint8_t> input(MAX_SIZE, 0xAA);
  // Set first byte to a non-zero value to exercise dictionaryLen usage
  input[0] = 0x7F;
  run_case("boundary_max_size", input);
}

//
// Test Case 6: Regular multi-byte input to exercise additional code paths
// Rationale: Ensures multiple bytes are processed and memory from deflate/inflate can be allocated.
// Expected: returns 0.
//
static void test_case_multi_byte_input() {
  std::vector<uint8_t> input(128, 0x55);
  input[0] = 64; // dictionaryLen <= dataLen
  for (size_t i = 1; i < input.size(); ++i) input[i] = static_cast<uint8_t>(i);
  run_case("multi_byte_input", input);
}

int main() {
  std::cout << "Starting LLVMFuzzerTestOneInput test suite (no GTest)..." << std::endl;

  test_case_empty_size();
  test_case_too_large_size();
  test_case_normal_dictionary_len_ok();
  test_case_dictionary_len_exceeds_data_len();
  test_case_boundary_max();
  test_case_multi_byte_input();

  std::cout << "Test suite completed. Total checks: " << g_total
            << ", Failures: " << g_failed << std::endl;

  // Return code indicates success if no failures were recorded.
  return (g_failed == 0) ? 0 : 1;
}