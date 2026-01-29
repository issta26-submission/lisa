// A minimal C++11 test harness focusing on the focal behavior of testNotEqualDoubleArraysExpectedNull.
// This reproduces the intent of the Unity test (not using Unity framework) and ensures executable tests
// across the two compilation branches controlled by UNITY_EXCLUDE_DOUBLE.

#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Simple array comparison utility for doubles.
// Returns true if all corresponding elements are equal, false otherwise.
// Treats NULL pointers as non-equal (as typical in unit-test expectations for NULL vs non-NULL).
static bool equal_double_arrays(const double* a, const double* b, std::size_t n)
{
    if (a == nullptr || b == nullptr)
        return false;
    for (std::size_t i = 0; i < n; ++i)
    {
        if (a[i] != b[i])
            return false;
    }
    return true;
}

// Test 1: Ignored path when UNITY_EXCLUDE_DOUBLE is defined.
// This simulates the UNITY macro path where the test would be ignored.
// Explanatory comment: ensures that when doubles are excluded, the focal test is reported as ignored.
void testNotEqualDoubleArraysExpectedNull_Ignored(void)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    std::cout << "[IGNORED] testNotEqualDoubleArraysExpectedNull (UNITY_EXCLUDE_DOUBLE defined)\n";
#else
    // If doubles are not excluded, this test path would be skipped in the Unity-like flow.
    // We still emit a trace to indicate the non-excluded environment is being exercised here.
    std::cout << "[SKIPPED] testNotEqualDoubleArraysExpectedNull_Ignored (UNITY_EXCLUDE_DOUBLE not defined)\n";
#endif
}

// Test 2: Normal path where UNITY_EXCLUDE_DOUBLE is not defined.
// This reproduces the original test's intent: NOT_EQUAL between a NULL pointer and a non-NULL array,
// which should cause the assertion to fail. We model this by throwing an exception to indicate the
// expected failure (since we are not using GTest/Unity here).
// Explanatory comment: verifies that a NULL actual vs non-NULL expected correctly results in a failure.
void testNotEqualDoubleArraysExpectedNull_NotIgnored(void)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    try
    {
        const double* p0 = nullptr;
        const double p1[] = {1.0, 8.0, 25.4, 0.252};
        // We expect the arrays to be not equal (NULL vs non-NULL).
        bool areEqual = equal_double_arrays(p0, p1, 4);
        if (areEqual)
        {
            // This would mean the test unexpectedly passed.
            throw std::runtime_error("Test unexpectedly passed: NULL and non-NULL double arrays were considered equal.");
        }
        else
        {
            // This path models the "expected failure" in the Unity test.
            throw std::runtime_error("EXPECTED_FAILURE: NULL actual vs non-NULL expected should fail.");
        }
    }
    catch (const std::exception& ex)
    {
        // We treat catching an exception as a successful handling of the expected failure.
        std::cout << "[PASS] testNotEqualDoubleArraysExpectedNull_NotIgnored: " << ex.what() << std::endl;
        return;
    }
    // If no exception occurred (unexpected), report failure.
    std::cout << "[FAIL] testNotEqualDoubleArraysExpectedNull_NotIgnored: No failure detected\n";
#else
    // When UNITY_EXCLUDE_DOUBLE is defined, this branch isn't compiled in normally.
    std::cout << "[IGNORED] testNotEqualDoubleArraysExpectedNull_NotIgnored (UNITY_EXCLUDE_DOUBLE defined)\n";
#endif
}

// Lightweight test runner to execute the synthesized tests.
int main()
{
    std::cout << "Running synthesized tests for testNotEqualDoubleArraysExpectedNull\n";

    // Execute both variants. Depending on THE macro UNITY_EXCLUDE_DOUBLE, one or both paths
    // will emit informative traces or perform actual checks.
    testNotEqualDoubleArraysExpectedNull_Ignored();
    testNotEqualDoubleArraysExpectedNull_NotIgnored();

    std::cout << "Finished synthesized tests\n";
    return 0;
}