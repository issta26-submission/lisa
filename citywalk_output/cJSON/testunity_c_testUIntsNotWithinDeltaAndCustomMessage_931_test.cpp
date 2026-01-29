/*
  Standalone C++11 test suite for the focal test case:
  testUIntsNotWithinDeltaAndCustomMessage

  Rationale:
  - The original focal method relies on Unity macros to validate that a UINT is NOT within a given delta
    and prints a custom message on failure. Since we are not using GTest and must provide a
    self-contained C++11 test, this file implements a minimal, self-contained test harness that
    reproduces the essential behavior of that specific assertion:
      ASSERT_UINT_WITHIN_MESSAGE(delta, expected, actual, message)
    The test below exercises the "not within delta" branch and also provides additional tests to
    cover the "within delta" path and edge-cases for completeness.

  Notes:
  - The tests are designed to be non-terminating (do not abort the program) so they can all run
    in a single invocation, aligning with the domain knowledge requirement to use non-terminating
    assertions where possible.
  - Static members in the original Unity file are not required for this focused test; this harness
    keeps functionality local to ensure portability in C++11.
  - The test suite exports clear PASS/FAIL messages for each test, and a final summary.
*/

#include <unity.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight, self-contained assertion facility to emulate the essential behavior
// of Unity's TEST_ASSERT_UINT_WITHIN_MESSAGE for unsigned integers.
namespace FocalTestHarness {

static int gTotalTests = 0;   // Total tests executed
static int gFailedTests = 0;  // Tests that failed (i.e., assertion did not behave as expected)

// Emulates the Unity macro TEST_ASSERT_UINT_WITHIN_MESSAGE(delta, expected, actual, message)
static bool ASSERT_UINT_WITHIN_MESSAGE(unsigned delta,
                                      unsigned expected,
                                      unsigned actual,
                                      const char* message)
{
    // Compute absolute difference safely for unsigned values
    unsigned long long diff = (expected > actual) ? static_cast<unsigned long long>(expected - actual)
                                              : static_cast<unsigned long long>(actual - expected);

    bool within = (diff <= delta);
    if (!within) {
        // Print a failure message that includes the custom message to mimic Unity's behavior
        std::cout << "ASSERT FAILED: diff=" << diff
                  << " delta=" << delta
                  << ". Message: " << (message ? message : "")
                  << std::endl;
    }
    return within;
}

// Test 1: The focal test case - expected NOT within delta and a custom message.
// This mirrors the provided focal method:
//   TEST_ASSERT_UINT_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
// The test should NOT be within delta (diff == 2 > delta == 1), hence it should fail.
// However, for non-terminating tests, we treat the expected failure as a passed test case
// in this harness.
void testUIntsNotWithinDeltaAndCustomMessage(void)
{
    ++gTotalTests;
    bool within = ASSERT_UINT_WITHIN_MESSAGE(1, 2147483647u, 2147483649u, "Custom Message.");
    if (within) {
        std::cout << "[FAIL] testUIntsNotWithinDeltaAndCustomMessage: "
                  << "Expected NOT within delta, but value is within delta." << std::endl;
        ++gFailedTests;
    } else {
        std::cout << "[PASS] testUIntsNotWithinDeltaAndCustomMessage" << std::endl;
    }
}

// Test 2: Within delta with a custom message (to exercise true-branch of the predicate)
void testUIntsWithinDeltaAndCustomMessage(void)
{
    ++gTotalTests;
    // diff = |1000 - 1001| = 1, delta = 1 => within
    bool within = ASSERT_UINT_WITHIN_MESSAGE(1, 1000u, 1001u, "Custom Message.");
    if (within) {
        std::cout << "[PASS] testUIntsWithinDeltaAndCustomMessage" << std::endl;
    } else {
        std::cout << "[FAIL] testUIntsWithinDeltaAndCustomMessage: Expected within delta." << std::endl;
        ++gFailedTests;
    }
}

// Test 3: Edge-case where delta is zero and values are equal (should be within)
void testUIntsWithinDeltaZeroDeltaEdgeCase(void)
{
    ++gTotalTests;
    bool within = ASSERT_UINT_WITHIN_MESSAGE(0u, 12345u, 12345u, "Zero delta edge case.");
    if (within) {
        std::cout << "[PASS] testUIntsWithinDeltaZeroDeltaEdgeCase" << std::endl;
    } else {
        std::cout << "[FAIL] testUIntsWithinDeltaZeroDeltaEdgeCase: Expected within delta (zero delta, equal values)." << std::endl;
        ++gFailedTests;
    }
}

// Test 4: Edge-case where values differ by more than delta (not within)
void testUIntsNotWithinDeltaEdgeCase(void)
{
    ++gTotalTests;
    bool within = ASSERT_UINT_WITHIN_MESSAGE(1, 0u, 2u, "Edge case not within delta.");
    if (within) {
        std::cout << "[FAIL] testUIntsNotWithinDeltaEdgeCase: Expected not within delta." << std::endl;
        ++gFailedTests;
    } else {
        std::cout << "[PASS] testUIntsNotWithinDeltaEdgeCase" << std::endl;
    }
}

// Expose a simple test runner interface
void runAllTests(void)
{
    testUIntsNotWithinDeltaAndCustomMessage();
    testUIntsWithinDeltaAndCustomMessage();
    testUIntsWithinDeltaZeroDeltaEdgeCase();
    testUIntsNotWithinDeltaEdgeCase();
}

} // namespace FocalTestHarness

int main()
{
    // Run the self-contained test suite
    FocalTestHarness::runAllTests();

    // Summary
    std::cout << "\nTest Summary: "
              << FocalTestHarness::gTotalTests << " run, "
              << FocalTestHarness::gFailedTests << " failed." << std::endl;

    // Return non-zero if any test failed
    return (FocalTestHarness::gFailedTests > 0) ? 1 : 0;
}