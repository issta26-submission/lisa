// Test-suite for the focal method testNotEqualDoubleEachEqual2 (Unity style) translated to a lightweight
// C++11 based test harness. The goal is high coverage for the same logical scenario without GTest.
// This file provides a minimal, self-contained test runner suitable for C++11 compilation.

#include <cmath>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Step 1: Candidate Keywords (representing core components of the focal method and its test flow)
// - DOUBLE: floating point path used by testNotEqualDoubleEachEqual2
// - testNotEqualDoubleEachEqual2: focal test function in testunity.c
// - TEST_ASSERT_EACH_EQUAL_DOUBLE: core assertion being emulated (checks all elements equal to a value)
// - EXPECT_ABORT_BEGIN / VERIFY_FAILS_END: error handling / abort flow (simulated here via return values)
// - p0: sample double array used in the test
// - UNITY_EXCLUDE_DOUBLE: conditional branch toggle (not exercised in this lightweight harness, but noted)
// - 8.0, 0.253: representative values used to demonstrate a failing and passing scenario

// Step 2: Lightweight, non-GTest test harness to emulate the core behavior of the focal test
// We implement a small predicate that mirrors the essence of TEST_ASSERT_EACH_EQUAL_DOUBLE(8.0, p0, 4):
// "Are all elements in p0 equal to 8.0?" We treat a false answer as a successful test of the NOT_EQUAL case
// (i.e., the Unity test would fail on that assertion, which we translate into a passed test in our harness).

// Core helper: checks whether all elements of the array are equal to target
static bool notAllEqualDouble(const std::vector<double>& arr, double target)
{
    for (double v : arr)
    {
        if (v != target) return false;
    }
    return true;
}

// Focal test reimagined in C++: simulates the NOT_EQUAL scenario of testNotEqualDoubleEachEqual2
// Expected Unity semantics: the assertion TEST_ASSERT_EACH_EQUAL_DOUBLE(8.0, p0, 4) would fail because
// the last element (0.253) is not 8.0. Our test asserts that the NOT-ALL-EQUAL condition is true
// (i.e., the array is NOT entirely equal to 8.0), which corresponds to a "test should fail" in Unity.
// We translate that into returning true to indicate the test logic behaved as expected.
static bool test_NotEqualDoubleEachEqual2_WithDoubleIncluded()
{
    // This mirrors: double p0[] = {8.0, 8.0, 8.0, 0.253};
    std::vector<double> p0 = {8.0, 8.0, 8.0, 0.253};

    bool allEqualToTarget = notAllEqualDouble(p0, 8.0); // should be false because last element differs

    // In Unity, this would cause the assertion to fail. We reflect that by expecting "not all equal" to be true.
    // Return true to indicate the test case behaved as expected (i.e., NOT all elements equal triggers failure path).
    return (allEqualToTarget == false);
}

// Additional test: All elements equal to target -> NOT_EQUAL assertion should not fail (i.e., it would pass)
static bool test_NotEqualDoubleEachEqual2_AllElementsEqual()
{
    // double p0[] = {8.0, 8.0, 8.0, 8.0};
    std::vector<double> p0 = {8.0, 8.0, 8.0, 8.0};

    bool allEqualToTarget = notAllEqualDouble(p0, 8.0); // should be true

    // If all elements are equal, the Unity-style TEST_ASSERT_EACH_EQUAL_DOUBLE would pass.
    // Our test returns true to indicate the behavior is as expected for the "equal" path.
    return allEqualToTarget;
}

// Test metadata and harness
struct TestCase
{
    std::string name;
    std::function<bool()> func;
};

static void runTest(const TestCase& tc, int& passed, int& failed)
{
    bool result = true;
    try
    {
        result = tc.func();
    }
    catch (...)
    {
        result = false;
    }

    if (result)
    {
        std::cout << "[PASS] " << tc.name << "\n";
        ++passed;
    }
    else
    {
        std::cout << "[FAIL] " << tc.name << "\n";
        ++failed;
    }
}

int main()
{
    // Register tests
    std::vector<TestCase> tests = {
        { "test_NotEqualDoubleEachEqual2_WithDoubleIncluded", test_NotEqualDoubleEachEqual2_WithDoubleIncluded },
        { "test_NotEqualDoubleEachEqual2_AllElementsEqual", test_NotEqualDoubleEachEqual2_AllElementsEqual }
    };

    int passed = 0;
    int failed = 0;

    // Execute tests
    for (const auto& t : tests)
        runTest(t, passed, failed);

    // Summary
    std::cout << "\nTests run: " << tests.size()
              << "  Passed: " << passed
              << "  Failed: " << failed << "\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}

// Notes:
// - This harness provides non-terminating test behavior (tests return booleans rather than exiting).
// - We cover both branches of the core predicate: the NOT-EQUAL scenario (false all-equal) and the ALL-EQUAL scenario (true all-equal).
// - The approach mirrors the intent of testNotEqualDoubleEachEqual2 (i.e., a failing Unity assertion) while remaining compatible with a simple C++11 test runner.
// - If additional coverage is desired, more test cases can be added following the same pattern (e.g., other arrays with different tail values or different lengths).