/*
  Unit test suite for the focal method:
  i64 lsmCheckpointLogOffset(u32 *aCkpt)

  Notes:
  - This test does not rely on GTest. It uses a tiny in-house test harness
    that accumulates failures and prints results to stdout.
  - The method computes a 64-bit offset from two 32-bit words in a checkpoint
    header array. Specifically, it returns:
      (i64)aCkpt[CKPT_HDR_LO_MSW] << 32 | (i64)aCkpt[CKPT_HDR_LO_LSW]
  - We verify normal, boundary, and edge values using the same constants/macros
    defined in the project (CKPT_HDR_LO_MSW, CKPT_HDR_LO_LSW).
  - We import project dependencies (lsmInt.h) to obtain type aliases i64, u32
    and the required macros. The function under test is declared with extern "C"
    to ensure linkage against the C implementation.
*/

#include <vector>
#include <lsmInt.h>
#include <string>
#include <iostream>
#include <cstdint>


// Include project dependencies that define i64, u32 and CKPT_HDR_LO_MSW/CKPT_HDR_LO_LSW

extern "C" {
  // Prototype for the focal method (C linkage)
  i64 lsmCheckpointLogOffset(u32 *aCkpt);
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

static void test_assert_equal_i64(const std::string &name, i64 expected, i64 actual) {
  ++g_tests_run;
  if (expected != actual) {
    ++g_tests_failed;
    std::cerr << "[FAIL] " << name
              << " | expected: " << expected
              << ", actual: " << actual << std::endl;
  } else {
    std::cout << "[PASS] " << name << std::endl;
  }
}

/*
  Test Case 1: Basic non-zero values
  Set LO_MSW = 1, LO_LSW = 2. Expect (1 << 32) + 2
*/
static void test_case_basic_values() {
  // Ensure the array is large enough to contain both indices
  int maxIndex = CKPT_HDR_LO_MSW > CKPT_HDR_LO_LSW ? CKPT_HDR_LO_MSW : CKPT_HDR_LO_LSW;
  std::vector<u32> aCkpt(static_cast<size_t>(maxIndex) + 1, 0);

  aCkpt[CKPT_HDR_LO_MSW] = 0x00000001;
  aCkpt[CKPT_HDR_LO_LSW] = 0x00000002;

  i64 expected = ((i64)aCkpt[CKPT_HDR_LO_MSW] << 32) + (i64)aCkpt[CKPT_HDR_LO_LSW];
  i64 actual = lsmCheckpointLogOffset(aCkpt.data());

  test_assert_equal_i64("test_case_basic_values", expected, actual);
}

/*
  Test Case 2: Both components zero
  LO_MSW = 0, LO_LSW = 0 -> expected 0
*/
static void test_case_zero_values() {
  int maxIndex = CKPT_HDR_LO_MSW > CKPT_HDR_LO_LSW ? CKPT_HDR_LO_MSW : CKPT_HDR_LO_LSW;
  std::vector<u32> aCkpt(static_cast<size_t>(maxIndex) + 1, 0);

  aCkpt[CKPT_HDR_LO_MSW] = 0;
  aCkpt[CKPT_HDR_LO_LSW] = 0;

  i64 expected = 0;
  i64 actual = lsmCheckpointLogOffset(aCkpt.data());

  test_assert_equal_i64("test_case_zero_values", expected, actual);
}

/*
  Test Case 3: Max 32-bit components
  LO_MSW = 0xFFFFFFFF, LO_LSW = 0xFFFFFFFF -> expected -1 (0xFFFFFFFFFFFFFFFF)
*/
static void test_case_max_values() {
  int maxIndex = CKPT_HDR_LO_MSW > CKPT_HDR_LO_LSW ? CKPT_HDR_LO_MSW : CKPT_HDR_LO_LSW;
  std::vector<u32> aCkpt(static_cast<size_t>(maxIndex) + 1, 0);

  aCkpt[CKPT_HDR_LO_MSW] = 0xFFFFFFFF;
  aCkpt[CKPT_HDR_LO_LSW] = 0xFFFFFFFF;

  i64 expected = ( (i64)0xFFFFFFFF << 32 ) + (i64)0xFFFFFFFF; // 0xFFFFFFFFFFFFFFFF
  i64 actual = lsmCheckpointLogOffset(aCkpt.data());

  test_assert_equal_i64("test_case_max_values", expected, actual);
}

/*
  Test Case 4: Mixed upper and lower edges
  LO_MSW = 0x0, LO_LSW = 0xFFFFFFFF -> expected 0xFFFFFFFF
*/
static void test_case_mixed_edges() {
  int maxIndex = CKPT_HDR_LO_MSW > CKPT_HDR_LO_LSW ? CKPT_HDR_LO_MSW : CKPT_HDR_LO_LSW;
  std::vector<u32> aCkpt(static_cast<size_t>(maxIndex) + 1, 0);

  aCkpt[CKPT_HDR_LO_MSW] = 0x0;
  aCkpt[CKPT_HDR_LO_LSW] = 0xFFFFFFFF;

  i64 expected = ( (i64)0x0 << 32 ) + (i64)0xFFFFFFFF;
  i64 actual = lsmCheckpointLogOffset(aCkpt.data());

  test_assert_equal_i64("test_case_mixed_edges", expected, actual);
}

/*
  Test Case 5: Random values to ensure consistency
  LO_MSW = 0x12345678, LO_LSW = 0x9ABCDEF0
*/
static void test_case_random_values() {
  int maxIndex = CKPT_HDR_LO_MSW > CKPT_HDR_LO_LSW ? CKPT_HDR_LO_MSW : CKPT_HDR_LO_LSW;
  std::vector<u32> aCkpt(static_cast<size_t>(maxIndex) + 1, 0);

  aCkpt[CKPT_HDR_LO_MSW] = 0x12345678;
  aCkpt[CKPT_HDR_LO_LSW] = 0x9ABCDEF0;

  i64 expected = ((i64)0x12345678 << 32) + (i64)0x9ABCDEF0;
  i64 actual = lsmCheckpointLogOffset(aCkpt.data());

  test_assert_equal_i64("test_case_random_values", expected, actual);
}

int main() {
  // Run all test cases
  test_case_basic_values();
  test_case_zero_values();
  test_case_max_values();
  test_case_mixed_edges();
  test_case_random_values();

  // Summary
  std::cout << "\nTotal tests run: " << g_tests_run
            << ", Failures: " << g_tests_failed << std::endl;

  // Return non-zero if any test failed
  return (g_tests_failed == 0) ? 0 : 1;
}