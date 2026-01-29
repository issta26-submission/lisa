/*
  This test suite targets the focal method:
    int lsmVarintLen32(int n)

  Context:
  - The function is a thin wrapper around lsmVarintPut32, calling it with a local
    9-byte buffer and returning its length (number of bytes written).
  - We cannot rely on the exact encoding details of the varint implementation
    (as the actual lsmVarintPut32 is in the C dependency). Therefore, the tests
    validate consistency between the wrapper and the underlying Put32 function.

  Test strategy (aligned with the provided instructions):
  - Step 2: Generate tests that compare lsmVarintLen32(n) against a direct call
    to lsmVarintPut32 for the same input n.
  - Step 3: Refine tests to maximize code coverage and execution (non-terminating
    assertions). Multiple test cases cover typical, boundary, and negative values.
  - We implement a lightweight test harness (no GTest) and run tests from main(),
  ensuring static functions (if any) remain unmocked and private details are not
  accessed.

  Notes:
  - We use extern "C" to declare the C functions so the C++ test can link against
    the C implementation (lsm_varint.c).
  - The test uses only the C++ standard library.
  - Failures are reported but do not terminate execution to maximize coverage.
  - This test suite focuses on functional correctness of the wrapper behavior.
*/

#include <lsmInt.h>
#include <iostream>
#include <cstdio>
#include <cstdint>


// Prototypes of the C functions (assuming typical typedefs map to unsigned char).
// We declare them with C linkage to avoid name mangling when linking with the C source file.
extern "C" {
  int lsmVarintLen32(int n);
  int lsmVarintPut32(uint8_t *aData, int iVal);
}

// Simple non-terminating test harness
static int g_totalTests = 0;
static int g_failures = 0;

// Helper macro for equality assertion without aborting on failure
#define TEST_EQ(a, b, desc) do { \
  ++g_totalTests; \
  if (!((a) == (b))) { \
    ++g_failures; \
    std::cerr << "FAIL: " << (desc) << " | Expected: " << (b) << "  Actual: " << (a) \
              << "  (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
  } \
} while(0)

// Helper macro for simple truth assertion
#define TEST_TRUE(cond, desc) do { \
  ++g_totalTests; \
  if (!(cond)) { \
    ++g_failures; \
    std::cerr << "FAIL: " << (desc) << " | Condition is false at " \
              << __FILE__ << ":" << __LINE__ << std::endl; \
  } \
} while(0)

/*
  Test Case 1: Basic correctness
  - For a representative set of integers (including boundary and negative values),
    verify that lsmVarintLen32(n) returns the same value as a direct call to
    lsmVarintPut32 for the same input.
  - This asserts that the wrapper simply delegates to the underlying Put32 and
    preserves the encoding length.
*/
static void test_LsmVarintLen32_matches_Put32_basic_values() {
  int testValues[] = {
      0,
      1,
      127,
      128,
      255,
      256,
      1024,
      32767,
      32768,
      65535,
      65536,
      2147483647,
      -1,
      -2,
      -128,
      -32768,
      -2147483648
  };
  const int N = sizeof(testValues) / sizeof(testValues[0]);

  for (int i = 0; i < N; ++i) {
    int n = testValues[i];
    unsigned char bufPut[9]; // 9-byte buffer as per original function usage
    int lenPut = lsmVarintPut32(bufPut, n);
    int lenLen32 = lsmVarintLen32(n);

    // Assert equality of lengths as the wrapper should mirrorPut32's output length.
    char desc[128];
    std::snprintf(desc, sizeof(desc),
                  "lsmVarintLen32(n) should equal lsmVarintPut32(n) for n=%d",
                  n);
    TEST_EQ(lenLen32, lenPut, desc);

    // Basic sanity: length should be positive
    TEST_TRUE(lenLen32 > 0, "Varint length should be positive");
  }
}

/*
  Test Case 2: Consistency across repeated calls
  - Ensure that multiple invocations of lsmVarintLen32 with the same input yield
    identical results (no nondeterminism in encoding length).
  - Demonstrates stability of the wrapper across the same input domain.
*/
static void test_LsmVarintLen32_consistency_repeated_calls() {
  // Deterministic sequence
  const int testValues[] = {0, 1, 2, 63, 64, 127, 128, 255, -1, -12345, 12345, 2147483647};
  const int N = sizeof(testValues) / sizeof(testValues[0]);

  for (int i = 0; i < N; ++i) {
    int n = testValues[i];
    int first = lsmVarintLen32(n);
    int second = lsmVarintLen32(n);

    char desc[128];
    std::snprintf(desc, sizeof(desc),
                  "Repeated calls should yield same length for n=%d", n);
    TEST_EQ(first, second, desc);
  }
}

/*
  Test Case 3: Non-trivial sanity checks
  - Ensure that the wrapper returns a reasonable value (positive) for a variety of
    inputs, including large positive and negative numbers.
  - This provides extra coverage for control flow on the wrapper level (even though
    there are no branches inside lsmVarintLen32 itself).
*/
static void test_LsmVarintLen32_sanity_checks() {
  int sanityInputs[] = {0, 1, 128, 256, 100000, -1, -999999, 2147483647, -2147483648};
  const int N = sizeof(sanityInputs) / sizeof(sanityInputs[0]);

  for (int i = 0; i < N; ++i) {
    int n = sanityInputs[i];
    int len = lsmVarintLen32(n);
    // Basic sanity: length should be at least 1 and finite
    TEST_TRUE(len >= 1, "Sanity: length should be >= 1");
    TEST_TRUE(len < 10, "Sanity: length should be < 10 (varint should not exceed 9 bytes in most implementations)");
  }
}

int main() {
  // Run all test cases
  test_LsmVarintLen32_matches_Put32_basic_values();
  test_LsmVarintLen32_consistency_repeated_calls();
  test_LsmVarintLen32_sanity_checks();

  // Summary
  std::cerr << "Total tests run: " << g_totalTests << std::endl;
  std::cerr << "Total failures: " << g_failures << std::endl;

  // Return non-zero if any test failed
  return (g_failures == 0) ? 0 : 1;
}