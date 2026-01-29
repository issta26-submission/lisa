/*
  Lightweight C++11 test suite to exercise the focal method scenario:
  testNotEqualDoubleEachEqualNegative3 (as described in testunity.c)

  Note:
  - This is a self-contained test harness not using GTest or Unity.
  - It mirrors the intent of the focal test: verify that the "each equal" check fails
    when not all elements in a double array are equal to the expected value (-8.0).
  - The harness provides minimal coverage by including:
      • Negative test: where not all elements are equal (should detect mismatch).
      • Positive test: where all elements are equal (should pass).
  - Static members and complex Unity-specific behavior are not reproduced here; instead
    we focus on functional equivalence of the core predicate: "all elements equal to value".
  - This file compiles under C++11 as a standalone executable.

  Candidate Keywords (derived from the focal method context):
  - NotEqualDouble, Each, Equal, Negative3, testNotEqualDoubleEachEqualNegative3,
    TEST_ASSERT_EACH_EQUAL_DOUBLE, double, array, p0, -8.0, -0.253
*/

#include <cmath>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight predicate: checks if all elements in arr (length n) are exactly equal to value val.
static bool all_equal_double(double val, const double* arr, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (arr[i] != val) {
            return false;
        }
    }
    return true;
}

// Lightweight test framework (minimal): a test is a function returning bool (pass=true).
struct TestCase {
    std::string name;
    std::function<bool(void)> func;
    std::string description;
};

// FOCAL TEST 1: Negative case from testNotEqualDoubleEachEqualNegative3
// Description: Verifies that the "each equal" check fails when the last element is not -8.0.
// Mirrors the Unity test's intent to abort on mismatch (here we simply return failure).
static bool testNotEqualDoubleEachEqualNegative3_impl() {
    // Setup identical to focal test: a 4-element array with last value different
    double p0[] = { -8.0, -8.0, -8.0, -0.253 };
    const size_t len = sizeof(p0) / sizeof(p0[0]);
    // Expectation: not all elements are equal to -8.0
    bool result = !all_equal_double(-8.0, p0, len);
    // The test passes if the predicate detects not-all-equal (i.e., result == true)
    return result;
}

// FOCAL TEST 2: Positive path (not the original focal negative test, but valuable for coverage)
// Description: Verifies that the "each equal" check passes when all elements are -8.0.
static bool testAllEqualDoubleShouldPass_impl() {
    double p0[] = { -8.0, -8.0, -8.0, -8.0 };
    const size_t len = sizeof(p0) / sizeof(p0[0]);
    bool result = all_equal_double(-8.0, p0, len);
    // Should pass
    return result;
}

// Simple test runner
int main() {
    std::vector<TestCase> tests;

    tests.push_back({
        "testNotEqualDoubleEachEqualNegative3",
        testNotEqualDoubleEachEqualNegative3_impl,
        "Negative test: not all elements equal to -8.0 in a 4-element double array."
    });

    tests.push_back({
        "testAllEqualDoubleShouldPass",
        testAllEqualDoubleShouldPass_impl,
        "Positive test: all elements equal to -8.0 in a 4-element double array."
    });

    int pass_count = 0;
    int fail_count = 0;

    std::cout << std::boolalpha;
    std::cout << "Running " << tests.size() << " test(s) for focal method scenario: testNotEqualDoubleEachEqualNegative3\n";

    for (const auto& t : tests) {
        bool ok = false;
        try {
            ok = t.func();
        } catch (...) {
            // If an exception occurs, consider it a failure for this simplistic harness
            ok = false;
        }
        std::cout << std::setw(40) << std::left << t.name
                  << " : " << (ok ? "PASS" : "FAIL")
                  << "  - " << t.description << "\n";
        if (ok) ++pass_count;
        else ++fail_count;
    }

    std::cout << "\nSummary: " << pass_count << " passed, " << fail_count << " failed out of "
              << tests.size() << " test(s).\n";

    // Return non-zero if any test failed
    return (fail_count == 0) ? 0 : 1;
}