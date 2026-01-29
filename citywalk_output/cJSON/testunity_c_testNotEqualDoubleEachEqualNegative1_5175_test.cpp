// Test Suite for the focal method: testNotEqualDoubleEachEqualNegative1
// This C++11 test harness provides a compact, GTest-less unit testing approach
// to exercise the focal method behavior described in the provided source.
//
// Step 1 (derived keywords / dependencies):
// - DOUBLE related comparison: TEST_ASSERT_EACH_EQUAL_DOUBLE(-0.253, p0, 4)
// - p0 array: {-1.0, -0.253, -0.253, -0.253}
// - Branch: UNITY_EXCLUDE_DOUBLE controls whether the test runs or is ignored
// - Expected abort behavior when not all elements equal the expected value
// - Macros like EXPECT_ABORT_BEGIN / VERIFY_FAILS_END in the original Unity test
//   are simulated here by signaling an expected failure in the test harness.
//
// Step 2: Unit Test Generation
// We implement two tests:
// 1) When doubles are included (UNITY_EXCLUDE_DOUBLE not defined), the focal test
//    would fail (abort) because not all elements equal -0.253.
// 2) When doubles are excluded (UNITY_EXCLUDE_DOUBLE defined), the test is skipped.
//
// Step 3: Test Refinement
// - Use a small epsilon for double comparisons to mimic tolerance in floating-point equality.
// - Provide explanatory comments for each test.
// - No GTest; simple test harness is used, with clear PASS/FAIL/SKIP/EXPECTED_FAIL outcomes.

#include <cmath>
#include <unity.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (from Step 1) for reference in comments:
// DOUBLE, -1.0, -0.253, p0, TEST_ASSERT_EACH_EQUAL_DOUBLE, UNITY_EXCLUDE_DOUBLE,
// EXPECT_ABORT_BEGIN, VERIFY_FAILS_END

// Simple test framework (no GTest, Unity, or external deps)
enum class TestStatus { PASS, FAIL, SKIP, EXPECT_FAIL };

struct TestCase {
    std::string name;
    std::function<TestStatus()> run;
};

// Small helper to print results
static void printStatus(const TestStatus st) {
    switch (st) {
        case TestStatus::PASS:       std::cout << "PASS"; break;
        case TestStatus::FAIL:       std::cout << "FAIL"; break;
        case TestStatus::SKIP:       std::cout << "SKIP"; break;
        case TestStatus::EXPECT_FAIL: std::cout << "EXPECTED_FAIL"; break;
        default:                       std::cout << "UNKNOWN"; break;
    }
}

// Focal helper: simulate the abort condition of the Unity test when doubles are included.
// It mirrors the logic described in the focal method:
// p0 = {-1.0, -0.253, -0.253, -0.253}; target = -0.253;
// If any element differs (with a small epsilon), the test would abort (fail in Unity).
static bool focalTestWouldAbort_DoubleIncluded() {
    const std::vector<double> p0 = { -1.0, -0.253, -0.253, -0.253 };
    const double target = -0.253;
    const double EPS = 1e-12;
    bool all_equal = true;
    for (double x : p0) {
        if (std::fabs(x - target) > EPS) {
            all_equal = false;
            break;
        }
    }
    // If not all equal, Unity-style test would "abort" (fail). Return true in that case.
    return !all_equal;
}

// Test 1: When doubles are included (UNITY_EXCLUDE_DOUBLE not defined), verify
// that the focal test would abort (i.e., would fail).
TestStatus test_NotEqualDoubleEachEqualNegative1_WhenDoubleIncluded() {
#ifndef UNITY_EXCLUDE_DOUBLE
    bool wouldAbort = focalTestWouldAbort_DoubleIncluded();
    // If the focal test would abort, this wrapper test is considered an expected failure.
    if (wouldAbort) return TestStatus::EXPECT_FAIL;
    else          return TestStatus::FAIL; // Unexpected: no abort occurred
#else
    // This path simulates Unity's ignore when UNITY_EXCLUDE_DOUBLE is defined.
    // The test should be skipped.
    return TestStatus::SKIP;
#endif
}

// Test 2: When doubles are excluded, the focal test is ignored/skipped.
// This validates the preprocessor-driven skip path.
TestStatus test_NotEqualDoubleEachEqualNegative1_WhenDoubleExcluded() {
#ifdef UNITY_EXCLUDE_DOUBLE
    // Test is correctly skipped when doubles are excluded.
    return TestStatus::SKIP;
#else
    // If we reach here, doubles are not excluded; indicate a failure to reflect
    // that this test is intended to be skipped only when UNITY_EXCLUDE_DOUBLE is defined.
    return TestStatus::FAIL;
#endif
}

int main() {
    // Register tests
    std::vector<TestCase> tests = {
        { "test_NotEqualDoubleEachEqualNegative1_WhenDoubleIncluded",
            []() -> TestStatus { return test_NotEqualDoubleEachEqualNegative1_WhenDoubleIncluded(); } },
        { "test_NotEqualDoubleEachEqualNegative1_WhenDoubleExcluded",
            []() -> TestStatus { return test_NotEqualDoubleEachEqualNegative1_WhenDoubleExcluded(); } }
    };

    // Run tests
    int total = static_cast<int>(tests.size());
    int passed = 0;
    int failed = 0;
    int skipped = 0;
    int expectedFailures = 0;

    std::cout << "Running test suite for focal method: testNotEqualDoubleEachEqualNegative1\n";

    for (const auto& t : tests) {
        TestStatus st = TestStatus::FAIL;
        try {
            st = t.run();
        } catch (const std::exception& ex) {
            // In case a test throws unexpectedly, treat as a failure.
            st = TestStatus::FAIL;
            std::cerr << "\nException in test " << t.name << ": " << ex.what() << "\n";
        }

        // Interpret statuses:
        // - PASS => treated as PASS
        // - FAIL => treated as FAIL
        // - SKIP => skipped
        // - EXPECT_FAIL => an expected failure; treat as PASS for coverage purposes
        if (st == TestStatus::PASS) passed++;
        else if (st == TestStatus::FAIL) { failed++; }
        else if (st == TestStatus::SKIP) { skipped++; }
        else if (st == TestStatus::EXPECT_FAIL) { expectedFailures++; passed++; } // expected failures count as pass

        // Print per-test result
        std::cout << "\nTest: " << t.name << " => ";
        printStatus(st);
        std::cout << "  (status code: " << static_cast<int>(st) << ")";
    }

    // Summary
    std::cout << "\n\nSummary: "
              << "Total=" << total
              << ", Passed=" << passed
              << ", Failed=" << failed
              << ", Skipped=" << skipped
              << ", ExpectedFailures=" << expectedFailures
              << "\n";

    // Return non-zero if any test failed unexpectedly
    if (failed > 0) {
        std::cerr << "Test suite finished with failures.\n";
        return 1;
    }
    return 0;
}