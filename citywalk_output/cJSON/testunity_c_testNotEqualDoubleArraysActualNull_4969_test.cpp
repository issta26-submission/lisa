// This is a standalone C++11 test harness that focuses on exercising the focal behavior
// of the testNotEqualDoubleArraysActualNull scenario from the Unity-based test file.
// Since we are not relying on the Unity test runner, this file provides a minimal, self-contained
// verification of the core logic: when the "actual" double array is NULL, the equality-check
// between an expected non-NULL array and a NULL array should fail.
// The test mirrors the intent of the Unity test while remaining executable in a C++11 environment.

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <cstdio>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (for test generation understanding):
// - UNITY_EXCLUDE_DOUBLE
// - double arrays
// - TEST_ASSERT_EQUAL_DOUBLE_ARRAY
// - actual NULL vs expected non-NULL
// - abort / failure signaling
// - test harness / non-GTest approach
// - BEGIN/END abort blocks (mirrored by failure signaling in this harness)

// Lightweight, self-contained test harness state
static bool CurrentTestFailed = false;
static const char* CurrentTestName = "UnknownTest";

// Reset test state before running a test
static void resetTestState(const char* testName)
{
    CurrentTestFailed = false;
    CurrentTestName = testName;
}

// Minimal assertion that mirrors Unity's TEST_ASSERT_EQUAL_DOUBLE_ARRAY behavior
// Returns true if arrays are equal (or comparison could proceed), false if a failure occurs.
// Special-case: if actual is NULL, consider it a failure (as per the focal test behavior).
static bool TEST_ASSERT_EQUAL_DOUBLE_ARRAY(const double* expected, const double* actual, std::size_t length)
{
    if (actual == nullptr)
    {
        // Simulate Unity's abort/fail when actual is NULL
        CurrentTestFailed = true;
        std::cerr << CurrentTestName
                  << ": Assertion failed - actual pointer is NULL (expected non-NULL array of length "
                  << length << ")." << std::endl;
        return false;
    }

    for (std::size_t i = 0; i < length; ++i)
    {
        if (expected[i] != actual[i])
        {
            CurrentTestFailed = true;
            std::cerr << CurrentTestName
                      << ": Assertion failed - mismatch at index " << i
                      << " (expected " << expected[i] << ", actual " << actual[i] << ")." << std::endl;
            return false;
        }
    }
    return true;
}

// Focal test adapted to a standalone harness
// Purpose: Validate that when the "actual" array is NULL, the equality assertion detects a failure.
// This mirrors the Unity testNotEqualDoubleArraysActualNull scenario from testunity.c.
/*
Original Unity test (pseudocode):
void testNotEqualDoubleArraysActualNull(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    TEST_IGNORE();
#else
    double p0[] = {1.0, 8.0, 25.4, 0.253};
    double* p1 = NULL;
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);
    VERIFY_FAILS_END
#endif
}
*/

// Note: This harness uses a macro-like control via UNITY_EXCLUDE_DOUBLE to simulate the ignored path.
static void testNotEqualDoubleArraysActualNull(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // Simulate Unity's TEST_IGNORE() by reporting an ignored test path and returning.
    std::cout << "testNotEqualDoubleArraysActualNull: IGNORED (UNITY_EXCLUDE_DOUBLE is defined)" << std::endl;
#else
    // Domain: non-NULL expected array, NULL actual pointer -> should fail the assertion
    constexpr double p0_arr[] = {1.0, 8.0, 25.4, 0.253};
    const double* p0 = p0_arr;
    const double* p1 = nullptr; // Actual is NULL

    // Reset and execute the assertion, capturing whether it fails
    resetTestState("testNotEqualDoubleArraysActualNull");
    bool ok = TEST_ASSERT_EQUAL_DOUBLE_ARRAY(p0, p1, 4);

    // Interpret result: it is a failure if the assertion returns false
    if (ok)
    {
        // If the assertion unexpectedly passed, report failure of the test
        std::cerr << "testNotEqualDoubleArraysActualNull: FAILED - expected failure when actual is NULL, but assertion passed." << std::endl;
    }
    else
    {
        // Expected path: assertion failed due to NULL actual
        std::cout << "testNotEqualDoubleArraysActualNull: PASSED - detected NULL actual as expected." << std::endl;
    }

    // Optional: ensure that the harness sees the failure flag as well
    if (CurrentTestFailed)
    {
        std::cout << "testNotEqualDoubleArraysActualNull: Harness correctly marks test as failed." << std::endl;
    }
    else
    {
        std::cout << "testNotEqualDoubleArraysActualNull: Harness did not mark test as failed (unexpected)." << std::endl;
    }
#endif
}

// Entry point
int main()
{
    // Run the focal test
    testNotEqualDoubleArraysActualNull();

    // Summary
    if (CurrentTestFailed)
    {
        std::cout << "Overall result: FAILURE" << std::endl;
        return 1;
    }
    else
    {
        // If UNITY_EXCLUDE_DOUBLE is defined, the test might be ignored. We still report outcome.
        std::cout << "Overall result: SUCCESS (note: if test was ignored, that's intentional under UNITY_EXCLUDE_DOUBLE)." << std::endl;
        return 0;
    }
}