// Lightweight, framework-free unit tests for the focal function:
// int lsmCheckpointPgsz(u32 *aCkpt);
// The test suite uses a minimal non-terminating assertion style and runs from main.
// It relies on the external C function lsmCheckpointPgsz and the macro CKPT_HDR_PGSZ
// defined in lsmInt.h (included below). We expose the function with C linkage.

#include <vector>
#include <lsmInt.h>
#include <limits>
#include <iostream>
#include <cstdint>


// Import project headers to get types/macros (e.g., CKPT_HDR_PGSZ)

// Provide C linkage for the focal function under test
extern "C" int lsmCheckpointPgsz(uint32_t *aCkpt);

// Simple non-terminating assertion helper macro for test logging
#define TEST_CHECK(cond, msg)                          \
  do {                                                   \
    if (!(cond)) {                                       \
      std::cerr << "Test failure: " << (msg) << "\n";    \
      return false;                                      \
    }                                                    \
  } while (0)

// Test 1: Basic value propagation
// Ensure that the function returns the value stored at aCkpt[CKPT_HDR_PGSZ]
// when a valid array is provided.
static bool test_lsmCheckpointPgsz_basicValue() {
  // Prepare a buffer large enough to contain CKPT_HDR_PGSZ index
  std::vector<uint32_t> a(CKPT_HDR_PGSZ + 1, 0);

  // Set a known value at the checkpoint page size index
  const uint32_t testValue = 42u;
  a[CKPT_HDR_PGSZ] = testValue;

  // Call the focal method
  int result = lsmCheckpointPgsz(a.data());

  // Verify the returned value matches the stored value
  TEST_CHECK(result == static_cast<int>(testValue),
             "Expected result to equal the value stored at CKPT_HDR_PGSZ");
  return true;
}

// Test 2: Zero value propagation
// Ensure that a value of zero is correctly returned.
static bool test_lsmCheckpointPgsz_zeroValue() {
  std::vector<uint32_t> a(CKPT_HDR_PGSZ + 1, 0);
  a[CKPT_HDR_PGSZ] = 0u;

  int result = lsmCheckpointPgsz(a.data());
  TEST_CHECK(result == 0,
             "Expected result to be 0 when CKPT_HDR_PGSZ stores 0");
  return true;
}

// Test 3: Large value within int range
// Ensure values near INT_MAX are handled correctly (within signed int range).
static bool test_lsmCheckpointPgsz_largeIntValue() {
  std::vector<uint32_t> a(CKPT_HDR_PGSZ + 1, 0);
  const uint32_t largeValue = static_cast<uint32_t>(std::numeric_limits<int>::max());
  a[CKPT_HDR_PGSZ] = largeValue;

  int result = lsmCheckpointPgsz(a.data());
  TEST_CHECK(result == static_cast<int>(largeValue),
             "Expected result to equal INT_MAX when CKPT_HDR_PGSZ stores INT_MAX");
  return true;
}

// Test 4: Multiple values around the CKPT_HDR_PGSZ index
// Ensure only the value at CKPT_HDR_PGSZ is read and other indices do not affect outcome.
static bool test_lsmCheckpointPgsz_independence() {
  std::vector<uint32_t> a(CKPT_HDR_PGSZ + 5, 0);
  const uint32_t distinctValue = 1234u;
  // Only set the CKPT_HDR_PGSZ index
  a[CKPT_HDR_PGSZ] = distinctValue;

  int result = lsmCheckpointPgsz(a.data());
  TEST_CHECK(result == static_cast<int>(distinctValue),
             "Expected result to reflect only the CKPT_HDR_PGSZ index value");
  return true;
}

// Entry point to run all tests
int main() {
  int failures = 0;

  std::cout << "Running lsmCheckpointPgsz unit tests...\n";

  if (!test_lsmCheckpointPgsz_basicValue()) {
    ++failures;
  } else {
    std::cout << "PASS: test_lsmCheckpointPgsz_basicValue\n";
  }

  if (!test_lsmCheckpointPgsz_zeroValue()) {
    ++failures;
  } else {
    std::cout << "PASS: test_lsmCheckpointPgsz_zeroValue\n";
  }

  if (!test_lsmCheckpointPgsz_largeIntValue()) {
    ++failures;
  } else {
    std::cout << "PASS: test_lsmCheckpointPgsz_largeIntValue\n";
  }

  if (!test_lsmCheckpointPgsz_independence()) {
    ++failures;
  } else {
    std::cout << "PASS: test_lsmCheckpointPgsz_independence\n";
  }

  if (failures == 0) {
    std::cout << "ALL TESTS PASSED\n";
  } else {
    std::cout << failures << " TEST(S) FAILED\n";
  }

  // Return 0 on success, non-zero on failure to integrate with simple CI scripts
  return failures;
}