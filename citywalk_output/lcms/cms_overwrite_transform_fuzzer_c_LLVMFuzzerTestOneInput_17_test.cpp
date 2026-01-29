/*
Unit test suite for the focal method LLVMFuzzerTestOneInput (as defined in cms_overwrite_transform_fuzzer.c).

Notes on approach:
- We cannot rely on a specific internal behavior of liblcms2 (lcms2) for profile parsing/transform creation. 
- Our tests focus on exercising the control flow present in LLVMFuzzerTestOneInput:
  - Branch: if (size < 2) return 0; (true/false coverage)
  - Branch: if (hTransform) cmsDeleteTransform(hTransform); (whether a valid transform was created or not is library-dependent)
- We use only the C++11 standard library and a minimal harness (no GTest) per the instructions.
- We do not modify the focal code; we only call it with various inputs to maximize coverage while remaining robust against library behavior.
- For safety, tests avoid assuming that a real transform can be created from arbitrary memory. We still exercise both branches by providing inputs of varying sizes and contents.
- The tests use a simple non-terminating assertion/logging scheme to report failures while allowing all tests to run.

Below is the test harness code. It declares the focal function (extern "C" to match the C linkage) and implements five test cases with explanatory comments. A small test runner executes these tests from main and prints a summary.
*/

#include <cstring>
#include <vector>
#include <cstddef>
#include <iostream>
#include <stdint.h>
#include <lcms2.h>
#include <cstdint>


// Declaration of the focal function (extern "C" to ensure correct linkage with C object)
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size);

// Simple non-terminating assertion/logging macro for test reporting
#define TEST_ASSERT(cond, name, msg)                                      \
  do {                                                                     \
    if (!(cond)) {                                                         \
      std::cerr << "[TEST_FAIL] " << name << ": " << (msg) << std::endl;  \
      ++g_failures;                                                       \
    } else {                                                               \
      std::cout << "[TEST_PASS] " << name << std::endl;                   \
    }                                                                      \
  } while (0)

// Global counter to summarize failures across tests
static int g_failures = 0;

/*
  Test Case 1: size = 0 (empty input)
  - Purpose: Cover the branch where size < 2 triggers an early return.
  - Expected: The function should return 0 and not crash, with no reliance on lcms2 behavior.
*/
void test_case_size_zero() {
  const int ret = LLVMFuzzerTestOneInput(nullptr, 0);
  TEST_ASSERT(ret == 0, "test_case_size_zero", "Expected 0 return for size 0");
}

/*
  Test Case 2: size = 1 (single-byte input)
  - Purpose: Another path where size < 2 triggers early return.
  - Expected: The function should return 0 and not crash.
*/
void test_case_size_one() {
  uint8_t buf = 0;
  const int ret = LLVMFuzzerTestOneInput(&buf, 1);
  TEST_ASSERT(ret == 0, "test_case_size_one", "Expected 0 return for size 1");
}

/*
  Test Case 3: size = 2 (minimal non-trivial input)
  - Purpose: Exercise the code path just after the size check.
  - Note: The content is unlikely to produce valid ICC profiles; depends on the library's handling.
  - Expected: Should not crash; return value is 0 according to the focal method.
*/
void test_case_size_two_minimal() {
  uint8_t buf[2] = { 0x00, 0x00 };
  const int ret = LLVMFuzzerTestOneInput(buf, 2);
  TEST_ASSERT(ret == 0, "test_case_size_two_minimal", "Expected 0 return for size 2 input");
}

/*
  Test Case 4: Large input with deterministic pattern (all zeros)
  - Purpose: Exercise the non-trivial path with size >= 2 and provide a sizeable payload.
  - This helps explore behavior when profiles cannot be parsed successfully and hTransform is NULL.
  - Expected: Should not crash; best effort to ensure return value 0 and no exceptions.
*/
void test_case_large_all_zero_input() {
  const size_t N = 1024; // sizeable input
  std::vector<uint8_t> data(N, 0x00); // all zeros
  const int ret = LLVMFuzzerTestOneInput(data.data(), data.size());
  TEST_ASSERT(ret == 0, "test_case_large_all_zero_input", "Expected 0 return for large zero-filled input");
}

/*
  Test Case 5: Multiple random-like inputs
  - Purpose: Increase coverage by feeding several varied inputs that are not guaranteed to yield valid profiles.
  - Approach: Use simple deterministic pseudo-random sequences to avoid relying on randomness in tests.
  - Expected: Function should always return 0; at least exercise various internal code paths without crashing.
*/
void test_case_multiple_random_inputs() {
  const size_t trials = 4;
  for (size_t t = 0; t < trials; ++t) {
    const size_t N = 256 + t * 64; // vary size
    std::vector<uint8_t> data(N);
    // Simple deterministic PRNG (LCG-like, no std::random) for reproducibility
    uint32_t seed = 1234567u + static_cast<uint32_t>(t * 131);
    for (size_t i = 0; i < N; ++i) {
      seed = (1103515245u * seed + 12345u) & 0x7fffffff;
      data[i] = static_cast<uint8_t>(seed & 0xFF);
    }
    const int ret = LLVMFuzzerTestOneInput(data.data(), data.size());
    TEST_ASSERT(ret == 0, "test_case_multiple_random_inputs", "Expected 0 return for randomized input case " + std::to_string(t));
  }
}

// Simple test runner to execute all tests from main
void runAllTests() {
  std::cout << "Starting unit tests for LLVMFuzzerTestOneInput...\n";

  test_case_size_zero();
  test_case_size_one();
  test_case_size_two_minimal();
  test_case_large_all_zero_input();
  test_case_multiple_random_inputs();

  std::cout << "Unit tests completed. Total failures: " << g_failures << "\n";
}

int main() {
  runAllTests();
  return g_failures; // Non-zero indicates test failures
}