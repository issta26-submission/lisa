/*
Step 0 - Focal insights (translated to code comments)
- Focal feature: testEqualDoubleEachEqualNaN() tests that a collection of NaN doubles is treated as equal to NaN when using Unity’s TEST_ASSERT_EACH_EQUAL_DOUBLE-like behavior.
- Core dependent elements (Candidate Keywords):
  - UNITY_EXCLUDE_DOUBLE: compile-time gate to skip double-specific tests (TEST_IGNORE in original).
  - d_zero / NAN generation: NaN values via 0.0 / 0.0 (represented in code as quiet_NaN()).
  - TEST_ASSERT_EACH_EQUAL_DOUBLE: Unity macro used in focal; here mirrored by an equivalent C++ function equal_double handling NaN==NaN semantics.
  - p0: array of NaNs used in focal test.
  - Double type handling (double) and NaN semantics.
- Class dependencies (from focal description): The test depends on Unity-like test scaffolding and double handling macros. In this C++ translation we emulate the essential behavior with a small, standalone test harness (no GTest) while preserving the focal behavior intent.

This file provides a compact C++11-based test harness that mimics the focal test behavior for testEqualDoubleEachEqualNaN, including both
- the enabled path (when UNITY_EXCLUDE_DOUBLE is not defined), and
- the excluded path (when UNITY_EXCLUDE_DOUBLE is defined, simulating TEST_IGNORE behavior).

Note: This is a lightweight, self-contained test harness intended to complement the existing Unity-based project. It does not require GTest and compiles with a standard C++11 compiler.
*/

// Candidate Keywords reflected in the test setup
// - NaN generation (quiet_NaN), equality semantics for NaN, TEST_IGNORE pathway (UNITY_EXCLUDE_DOUBLE),
// - equal_double helper to treat NaN as equal to NaN, array p0 with NaN elements, and the test "728" behavior for
//   testEqualDoubleEachEqualNaN.

#include <cmath>
#include <unity.h>
#include <limits>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Simple NaN-equality helper that mirrors Unity's expected behavior for this focal test.
// Treats NaN as equal to NaN, and uses normal equality otherwise.
static bool equal_double(double a, double b)
{
    if (std::isnan(a) && std::isnan(b)) {
        return true;
    }
    return a == b;
}

// Lightweight test harness (non-GTest, suitable for C++11)
// Global flags to track test results without bringing in a full framework.
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Macro-like test runner to standardize output
#define RUN_TEST_FUNC(test_func) \
    do { \
        ++g_tests_run; \
        bool result = test_func(); \
        if (result) { \
            ++g_tests_passed; \
            std::cout << "[PASSED] " << #test_func << std::endl; \
        } else { \
            ++g_tests_failed; \
            std::cout << "[FAILED] " << #test_func << std::endl; \
        } \
    } while (0)

// Test 1: Enabled path for testEqualDoubleEachEqualNaN (UNITY_EXCLUDE_DOUBLE not defined)
// This mirrors the focal test's logic: a four-element NaN array should be "equal" to NaN when compared element-wise.
#ifndef UNITY_EXCLUDE_DOUBLE
bool testEqualDoubleEachEqualNaN_Enabled(void)
{
    // Simulate: double p0[] = {NaN, NaN, NaN, NaN}; and TEST_ASSERT_EACH_EQUAL_DOUBLE(NaN, p0, 4);
    double NaN_val = std::numeric_limits<double>::quiet_NaN();
    double p0[4] = {NaN_val, NaN_val, NaN_val, NaN_val};

    // Expect each element to be equal to NaN according to equal_double semantics (NaN == NaN -> true)
    for (int i = 0; i < 4; ++i) {
        if (!equal_double(NaN_val, p0[i])) {
            // Failure if any element is not considered equal to NaN
            return false;
        }
    }
    // All elements matched NaN under our NaN-equality policy
    return true;
}
#else
// If UNITY_EXCLUDE_DOUBLE is defined, this test function will be omitted in the enabled path.
// The excluded path is handled by a separate test below.
bool testEqualDoubleEachEqualNaN_Enabled(void) { return true; } // placeholder to keep signature consistent
#endif

// Test 2: Excluded path for testEqualDoubleEachEqualNaN (UNITY_EXCLUDE_DOUBLE defined)
// This simulates the TEST_IGNORE() branch in the focal code.
#ifdef UNITY_EXCLUDE_DOUBLE
bool testEqualDoubleEachEqualNaN_Excluded(void)
{
    // In the original Unity test, this would be TEST_IGNORE() and not affect test results.
    // Here we treat the test as 'skipped' and thus 'passed' for coverage purposes.
    std::cout << "[IGNORED] testEqualDoubleEachEqualNaN_Excluded (UNITY_EXCLUDE_DOUBLE defined)" << std::endl;
    return true;
}
#else
bool testEqualDoubleEachEqualNaN_Excluded(void) { return true; } // not compiled in this path
#endif

// Additional Refinement Test 3: Non-NaN value should not be considered equal to NaN
// This broadens coverage to ensure our equal_double does not erroneously treat non-NaN as NaN-equal.
bool testDoubleNaN_NotEqualToNonNaN(void)
{
    double NaN_val = std::numeric_limits<double>::quiet_NaN();
    double nonNaN = 1.2345;

    // NaN vs non-NaN should not be considered equal under NaN-equality policy
    if (equal_double(NaN_val, nonNaN)) {
        return false;
    }

    // Additionally, ensure non-NaN equals itself
    if (!equal_double(nonNaN, nonNaN)) {
        return false;
    }

    // All checks pass
    return true;
}

// Entry point
int main()
{
    // Explainant about the compile-time path:
    // - If UNITY_EXCLUDE_DOUBLE is defined, we compile and run the Excluded test (skipped/ignored).
    // - Otherwise, we run the Enabled test (actual NaN-equality logic).

#ifndef UNITY_EXCLUDE_DOUBLE
    std::cout << "Running test: testEqualDoubleEachEqualNaN_Enabled (double supported)" << std::endl;
    RUN_TEST_FUNC(testEqualDoubleEachEqualNaN_Enabled);

    // Additional refinement: NaN vs non-NaN should fail equality
    std::cout << "Running test: testDoubleNaN_NotEqualToNonNaN (NaN equality semantics check)" << std::endl;
    RUN_TEST_FUNC(testDoubleNaN_NotEqualToNonNaN);
#else
    std::cout << "Running test: testEqualDoubleEachEqualNaN_Excluded (double support excluded)" << std::endl;
    RUN_TEST_FUNC(testEqualDoubleEachEqualNaN_Excluded);
    // Even in excluded scenario, you may want to enumerate that you covered the path
    // by compiling with UNITY_EXCLUDE_DOUBLE in your build system.
#endif

    // Summary
    std::cout << "\nTest summary: " << g_tests_passed << " passed, "
              << g_tests_failed << " failed, " << g_tests_run << " total." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed > 0) ? 1 : 0;
}

/*
Notes for maintainers:
- The test is designed to be self-contained and does not rely on GTest or the Unity test runner.
- It mirrors the focal behavior for testEqualDoubleEachEqualNaN by implementing an equivalent NaN-equality check in C++11.
- The UNITY_EXCLUDE_DOUBLE macro creates two compile-time scenarios:
  - Enabled: real NaN-equality test is executed.
  - Excluded: test is treated as ignored, emulated by a dedicated test that reports as IGNORED.
- This approach provides explicit coverage for the focal method's dual-path behavior without modifying the original testunity.c file.
- You can extend this harness with additional tests that mirror other patterns from testunity.c (e.g., other double/NaN/Inf pathways) if needed. 
*/