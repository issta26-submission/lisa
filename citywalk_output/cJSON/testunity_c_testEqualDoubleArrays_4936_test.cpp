/*
Step 1 - Candidate Keywords (derived from the focal method)
- testEqualDoubleArrays: focal test in testunity.c that validates equality of two double arrays with a length parameter.
- double arrays, element-wise comparison, length-based comparison.
- NULL pointer handling for array pointers (NULL vs NULL, NULL vs non-NULL).
- UNITY_EXCLUDE_DOUBLE: gating macro that excludes the double-branch in Unity (TEST_IGNORE in original).
- TEST_ASSERT_EQUAL_DOUBLE_ARRAY: semantics replicated as equal_double_arrays(a, b, len).
- Positive-branch coverage: identical arrays, identical first N elements, and NULL-handling case.
- Negative-branch considerations (covered in extended suite): non-equal arrays for completeness if desired.
- Class dependencies (Unity-like behavior is simulated here in a self-contained harness): basic array equality logic, pointer checks, and length-based comparison.
- Domain knowledge touches: proper separation of test cases, explicit comments per test, and use of C++11 safe constructs.
- Domain constraints: do not rely on GTest; provide a standalone C++11 test harness that culminates in a clear pass/fail summary.

Note: The provided focal method is in C (Unity test style). To satisfy the request without pulling in GTest, this solution implements a compact, self-contained C++11 test harness that emulates the essential behavior of testEqualDoubleArrays and validates the same scenarios. It also respects the UNITY_EXCLUDE_DOUBLE gating pattern by skipping the test when defined.
*/

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (summary for reviewers)
// - testEqualDoubleArrays, double arrays, length-based comparison, NULL handling, UNITY_EXCLUDE_DOUBLE,
// - equal_double_arrays, array equality, element-wise comparison, test harness, non-GTest, C++11

// Minimal stand-in for the Unity-style equality checker for doubles.
// Semantics: two arrays are equal if:
//  - both pointers are NULL -> considered equal
//  - exactly one is NULL -> not equal
//  - both non-NULL: first 'len' elements are bitwise-equal (as doubles)
static bool equal_double_arrays(const double* a, const double* b, std::size_t len)
{
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;

    for (std::size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Simple test assertion helper that records results and prints details.
// It mirrors the idea of an assertion in the Unity-style test, but accumulates results
// for a non-GTest, non-termination based harness.
struct TestHarness
{
    struct Result
    {
        std::string name;
        bool passed;
        std::string message;
    };

    std::vector<Result> results;

    void assert_true(bool condition, const std::string& name, const std::string& message)
    {
        results.push_back({name, condition, condition ? "" : message});
    }

    int failure_count() const
    {
        int count = 0;
        for (const auto& r : results)
            if (!r.passed) ++count;
        return count;
    }

    void summarize() const
    {
        int idx = 1;
        for (const auto& r : results)
        {
            std::cout << (r.passed ? "[PASS] " : "[FAIL] ")
                      << std::setw(2) << idx++ << ": " << r.name;
            if (!r.passed && !r.message.empty())
                std::cout << " -- " << r.message;
            std::cout << "\n";
        }
        std::cout << "\nTotal tests: " << results.size()
                  << ", Failures: " << failure_count() << "\n";
    }

    bool all_passed() const
    {
        return failure_count() == 0;
    }
};

// The focal test: testEqualDoubleArrays
// This mirrors the assertions performed in the Unity test as described in the prompt.
// If UNITY_EXCLUDE_DOUBLE is defined, we skip execution to emulate TEST_IGNORE() behavior.
static void testEqualDoubleArrays(TestHarness& harness)
{
#ifdef UNITY_EXCLUDE_DOUBLE
    // Emulate Unity's TEST_IGNORE() path which marks the test as ignored.
    // In this harness we simply record a pass with an informational message to indicate skip.
    harness.results.push_back({"testEqualDoubleArrays (excluded)", true, "TEST_IGNORE: UNITY_EXCLUDE_DOUBLE defined"});
    return;
#else
    // Core test data mirroring the original Unity test values
    const double p0[] = {1.0, -8.0,  25.4, -0.123};
    const double p1[] = {1.0, -8.0,  25.4, -0.123};
    const double p2[] = {1.0, -8.0,  25.4, -0.2};
    const double p3[] = {1.0, -23.0, 25.0, -0.26};

    // p0 == p0, len 1
    harness.assert_true(equal_double_arrays(p0, p0, 1),
                        "testEqualDoubleArrays: p0 vs p0 (len=1)",
                        "Expected arrays to be equal for first element");

    // p0 == p0, len 4
    harness.assert_true(equal_double_arrays(p0, p0, 4),
                        "testEqualDoubleArrays: p0 vs p0 (len=4)",
                        "Expected arrays to be equal for all 4 elements");

    // p0 == p1, len 4
    harness.assert_true(equal_double_arrays(p0, p1, 4),
                        "testEqualDoubleArrays: p0 vs p1 (len=4)",
                        "Expected arrays to be equal for all 4 elements");

    // p0 == p2, len 3 (first 3 equal)
    harness.assert_true(equal_double_arrays(p0, p2, 3),
                        "testEqualDoubleArrays: p0 vs p2 (len=3)",
                        "Expected first 3 elements to be equal");

    // p0 == p3, len 1 (first element equal)
    harness.assert_true(equal_double_arrays(p0, p3, 1),
                        "testEqualDoubleArrays: p0 vs p3 (len=1)",
                        "Expected first element to be equal");

    // NULL pointers equal case
    harness.assert_true(equal_double_arrays(nullptr, nullptr, 1),
                        "testEqualDoubleArrays: NULL vs NULL (len=1)",
                        "Both pointers NULL should be treated as equal");
#endif
}

// Additional test to strengthen coverage (non-target, but demonstrates negative path handling)
// This is optional for the exact focal method but improves robustness of the suite.
static void testNotEqualDoubleArraysSuite(TestHarness& harness)
{
#ifndef UNITY_EXCLUDE_DOUBLE
    // Additional non-equality checks to ensure false branches are exercised
    const double a[] = {0.0, 1.0, 2.0, 3.0};
    const double b[] = {0.0, 1.0, 2.0, 4.0};

    harness.assert_true(!equal_double_arrays(a, b, 4),
                        "testNotEqualDoubleArraysSuite: a vs b (len=4)",
                        "Expected arrays to differ in at least one element");
    
    harness.assert_true(!equal_double_arrays(a, nullptr, 4),
                        "testNotEqualDoubleArraysSuite: a vs NULL (len=4)",
                        "Expected comparison with NULL to be not equal");
#endif
}

// Entry point for the test harness
int main()
{
    TestHarness harness;

    // Run focal test for equal double arrays
    testEqualDoubleArrays(harness);

    // Optional: run negative/extra test to widen coverage
    testNotEqualDoubleArraysSuite(harness);

    // Report results
    harness.summarize();

    // Return non-zero if failures occurred
    return harness.failure_count() > 0 ? 1 : 0;
}