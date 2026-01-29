/*
Unit test suite for the focal methodLLVMFuzzerTestOneInput (and its class dependencies)
- Target: int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size)
- Approach: Create a lightweight C++11 test harness (no GTest) that exercises
  boundary conditions and typical inputs to ensure coverage of both branches
  and the overall contract (must return 0).
- Key dependencies (as seen in the provided sources):
  - test_large_deflate and test_large_inflate (stubs in the provided class dep)
  - global inputs: data, dataLen (assigned inside the fuzzer)
  - memory handling via compr/uncompr buffers
- Strategy: Validate that the function returns 0 for:
  - size < 1 (0 in test)
  - size within range (typical inputs)
  - size == kMaxSize (512*1024)
  - size > kMaxSize (oversize path)
  Additionally, we ensure the function handles multiple input sizes without crashing.
*/

#include <stddef.h>
#include <string.h>
#include <iomanip>
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


// Declaration of the focal function to be tested (C linkage)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *d, size_t size);

/*
Minimal non-terminating test harness:
- Uses simple PASS/FAIL messages to stdout/stderr
- Never terminates on a failed expectation; records failures and reports at the end
- Covers several representative inputs to maximize coverage
*/

// Global counters for test reporting
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Run a test with a given input buffer and its size
static void runTestWithSize(const std::vector<uint8_t>& data, size_t size, const char* testName) {
  const uint8_t* ptr = data.empty() ? nullptr : data.data();
  int ret = LLVMFuzzerTestOneInput(ptr, size);
  ++g_total_tests;
  if (ret != 0) {
    std::cerr << "[FAIL] " << testName
              << " -> Return code " << ret << " (expected 0)" << std::endl;
    ++g_failed_tests;
  } else {
    std::cout << "[PASS] " << testName << std::endl;
  }
}

int main() {
  // Comment on Candidate Keywords (Step 1 outputs) to guide reviewers:
  // - Candidate Keywords: data, dataLen, compr, uncompr, comprLen, uncomprLen,
  //   test_large_deflate, test_large_inflate, kMaxSize, 512*1024, calloc/free

  // Test 0: size = 0 (boundary of allowed inputs, should return 0)
  {
    std::vector<uint8_t> empty;
    runTestWithSize(empty, 0, "LLVMFuzzerTestOneInput with size 0 (boundary: <=0)");
  }

  // Test 1: size = 1 (minimal non-empty input)
  {
    std::vector<uint8_t> one = { 0xAB };
    runTestWithSize(one, 1, "LLVMFuzzerTestOneInput with size 1");
  }

  // Test 2: size = kMaxSize (boundary of allowed inputs)
  {
    // Data content is irrelevant for this test since the function should process the size properly
    std::vector<uint8_t> dummy = { 0x01 };
    size_t maxSize = 512u * 1024u; // 512 * 1024
    runTestWithSize(dummy, maxSize, "LLVMFuzzerTestOneInput with size kMaxSize (512*1024)");
  }

  // Test 3: size = 10 (small but non-trivial input)
  {
    std::vector<uint8_t> ten(10);
    for (size_t i = 0; i < 10; ++i) ten[i] = static_cast<uint8_t>(i * 3);
    runTestWithSize(ten, ten.size(), "LLVMFuzzerTestOneInput with size 10");
  }

  // Test 4: size > kMaxSize to exercise the early-return path
  {
    // Use a tiny data buffer; the function should return 0 without allocating large buffers
    std::vector<uint8_t> small = { 0x00 };
    size_t oversize = 512u * 1024u + 1u; // 524289
    runTestWithSize(small, oversize, "LLVMFuzzerTestOneInput with size oversize (> kMaxSize)");
  }

  // Summary
  std::cerr << std::endl << "Test Summary: " << g_total_tests
            << " tests, " << g_failed_tests << " failures." << std::endl;

  // Return non-zero if any test failed to aid CI systems
  return (g_failed_tests == 0) ? 0 : 1;
}