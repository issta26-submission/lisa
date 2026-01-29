/*
  Test Suite: Validation of the focal method testFloatIsNegInf1 behavior
  Context:
  - The focal method under test is testFloatIsNegInf1 from testunity.c.
  - It uses UNITY_EXCLUDE_FLOAT to optionally skip the test and relies on the macro
    TEST_ASSERT_FLOAT_IS_NEG_INF(-3.0f / f_zero) when float support is enabled.
  - The core candidate components (from Step 1) include: UNITY_EXCLUDE_FLOAT, f_zero,
    and the assertion that a float expression evaluates to negative infinity.
  - The suite below is written in C++11 (no GTest), with a lightweight, non-terminating
    test harness to maximize coverage and allow static branching via preprocessor symbols.
  - This file should compile as C++11. No external test framework is required.
  - For completeness, we also exercise the alternative branch (negative denominator zero sign)
    and a compile-time guarded ignore path to reflect UNITY_EXCLUDE_FLOAT behavior.

  Note: This test suite does not modify the original testunity.c; instead, it simulates
  the behavior of the focal method in a standalone, self-contained manner suitable for
  C++11 environments.
*/

#include <cmath>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


/* Candidate Keywords alignment (for reference/documentation purposes):
   - testFloatIsNegInf1
   - UNITY_EXCLUDE_FLOAT
   - f_zero
   - TEST_ASSERT_FLOAT_IS_NEG_INF
   - -3.0f / f_zero
   - negative infinity (IEEE-754)
*/

// Lightweight test harness (non-terminating) to maximize code coverage
// Each test returns true if it passes, false if it fails.
namespace FocalNegInfTests {

// Helper: print a simple test result line
static void printResultLine(const std::string& testName, bool passed) {
    std::cout << (passed ? "[OK] " : "[FAIL] ") << testName << "\n";
}

// Test 1 (normal path): Verify that -3.0f / 0.0f yields negative infinity
// This directly mirrors the focal behavior when UNITY_EXCLUDE_FLOAT is not defined.
// Uses IEEE-754 semantics (std::isinf and std::signbit) to confirm sign and infinity.
static bool test_float_is_neg_inf_normal_path() {
    // Simulate the focal file's static constant
    const float f_zero = 0.0f;

    // Compute the value that should be negative infinity per IEEE-754 rules
    float value = -3.0f / f_zero;  // May yield -INFINITY on compliant platforms

    // Check: value is infinite and negative
    bool isInf = std::isinf(value);
    bool isNegInf = std::signbit(value); // true iff value is negative
    bool result = (isInf && isNegInf);

    if (!result) {
        std::cerr << "Failure in test_float_is_neg_inf_normal_path: expected -inf, got "
                  << (std::isinf(value) ? (std::signbit(value) ? "-inf (neg)" : "+inf (pos)") : "NaN/finite")
                  << "\n";
    }
    printResultLine("test_float_is_neg_inf_normal_path", result);
    return result;
}

// Test 2 (alternative zero-sign behavior): Validate -3.0f / -0.0f yields +infinity
// This ensures we understand sign propagation with zero-denominator signedness.
static bool test_float_is_neg_inf_with_negative_zero() {
    const float neg_zero = -0.0f;
    float value = -3.0f / neg_zero; // Expected to be +INFINITY

    bool isInf = std::isinf(value);
    bool isNegInf = std::signbit(value); // For +inf, signbit should be false

    bool result = (isInf && !isNegInf);

    if (!result) {
        std::cerr << "Failure in test_float_is_neg_inf_with_negative_zero: expected +inf, got "
                  << (std::isinf(value) ? (std::signbit(value) ? "-inf" : "+inf") : "NaN/finite")
                  << "\n";
    }
    printResultLine("test_float_is_neg_inf_with_negative_zero", result);
    return result;
}

// Test 3 (ignore path): Simulate UNITY_EXCLUDE_FLOAT branch
// If UNITY_EXCLUDE_FLOAT is defined, the focal test should be ignored.
// We emulate "ignore" by marking the test as skipped (but still returning success to
// avoid counting as a failure in this standalone harness).
static bool test_float_is_neg_inf_ignored_branch() {
#ifdef UNITY_EXCLUDE_FLOAT
    // Simulated skip: do not perform the actual assertion
    std::cout << "[SKIPPED] test_float_is_neg_inf_ignored_branch (UNITY_EXCLUDE_FLOAT defined)\n";
    // Returning true to indicate the test suite considers skip as non-failing
    return true;
#else
    // If not excluded, run the normal path as a fallback
    return test_float_is_neg_inf_normal_path();
#endif
}

} // namespace FocalNegInfTests

int main() {
    using namespace FocalNegInfTests;

    // Collect tests
    std::vector<std::pair<std::string, std::function<bool()>>> tests = {
        {"test_float_is_neg_inf_normal_path", test_float_is_neg_inf_normal_path},
        {"test_float_is_neg_inf_with_negative_zero", test_float_is_neg_inf_with_negative_zero},
        {"test_float_is_neg_inf_ignored_branch", test_float_is_neg_inf_ignored_branch}
    };

    // Run tests
    int passed = 0;
    int failed = 0;
    for (const auto& t : tests) {
        bool ok = t.second();
        if (ok) ++passed;
        else ++failed;
    }

    // Summary
    std::cout << "\nTest Summary: " << passed << " passed, "
              << failed << " failed, "
              << ((failed == 0) ? "ALL GOOD" : "SOME FAILURES") << "\n";

    // Return non-zero on failure to integrate with simple CI expectations
    return (failed == 0) ? 0 : 1;
}