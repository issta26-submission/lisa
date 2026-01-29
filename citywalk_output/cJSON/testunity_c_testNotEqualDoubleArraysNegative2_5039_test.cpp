/*
Step 1 - Program Understanding (Candidate Keywords)
- Core dependencies: double arrays, memory comparison, array length
- Key elements: UNITY_EXCLUDE_DOUBLE gate, TEST_ASSERT_EQUAL_DOUBLE_ARRAY style assertion, negative test (arrays differ)
- Candidate Keywords for test generation: UNITY_EXCLUDE_DOUBLE, DOUBLE arrays, -1.0/-8.0/-25.4/-0.253, -2.0/-8.0/-25.4/-0.253, TEST_ASSERT_EQUAL_DOUBLE_ARRAY, EXPECT_ABORT_BEGIN/VERIFY_FAILS_END analogs

This C++11 test suite emulates the focal Unity test path for the method:
testNotEqualDoubleArraysNegative2 located in testunity.c
It provides a minimal assertion primitive for double arrays and verifies:
- A mismatch in a pair of identical-length double arrays should trigger a "failure" path (i.e., an exception in our harness)
- The opposite case (arrays equal) should not trigger a failure
All tests are executable without GTest and without depending on the Unity runtime.
*/

// Step 2 & 3 - Comprehensive, executable C++11 test suite (no GTest)
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


// Lightweight assertion mimic for double-array equality (exact comparison)
static void assert_double_arrays_equal(const double* a, const double* b, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) {
            // Simulate a test failure path similar to Unity's assertion failure
            throw std::runtime_error("DOUBLE_ARRAYS_NOT_EQUAL: elements differ at index " + std::to_string(i));
        }
    }
}

// Candidate Keywords (Step 1 reflection) used in test comments above
// - UNITY_EXCLUDE_DOUBLE (gate for double tests)
// - TEST_ASSERT_EQUAL_DOUBLE_ARRAY semantics (here emulated by assert_double_arrays_equal)
// - p0, p1 double arrays, length 4
// - Negative test: arrays are not equal -> should trigger "failure" path
// - Positive test: arrays are equal -> should not trigger "failure" path

// Test 1: Reproduce the focal negative test from testNotEqualDoubleArraysNegative2
// This test ensures that when two 4-element double arrays differ (first element differs),
// the assertion path triggers a failure (which we catch and treat as a pass for this test).
static void testNotEqualDoubleArraysNegative2_cpp(void) {
#ifndef UNITY_EXCLUDE_DOUBLE
    double p0[] = {-1.0, -8.0, -25.4, -0.253};
    double p1[] = {-2.0, -8.0, -25.4, -0.253};
    try {
        // This should fail (arrays are not equal)
        assert_double_arrays_equal(p0, p1, 4);
    } catch (const std::exception& /*e*/) {
        // Expected failure path: test passes when a mismatch is detected
        return;
    }
    // If no exception occurred, the test has unexpectedly passed; mark as failure
    throw std::runtime_error("Expected failure did not occur for not-equal double arrays");
#else
    // If double support is excluded, mimic Unity's ignore behavior (pass)
    return;
#endif
}

// Test 2: Additional guard - identical arrays should pass (no failure path)
// This ensures the "no failure" path is also exercised for coverage completeness.
static void testEqualDoubleArraysNoThrow_cpp(void) {
#ifndef UNITY_EXCLUDE_DOUBLE
    double p0[] = {-1.0, -8.0, -25.4, -0.253};
    double p1[] = {-1.0, -8.0, -25.4, -0.253};
    try {
        // Should NOT fail since arrays are equal
        assert_double_arrays_equal(p0, p1, 4);
    } catch (const std::exception& /*e*/) {
        // If an exception is thrown, this is a test failure
        throw std::runtime_error("Unexpected failure for equal double arrays");
    }
#else
    // If double support is excluded, mimic Unity's ignore behavior (pass)
    return;
#endif
}

// Lightweight test harness
class TestSuite {
public:
    using TestFn = std::function<void(void)>;

    void add(const std::string& name, TestFn fn) {
        tests.emplace_back(name, std::move(fn));
    }

    int run() {
        int failures = 0;
        for (const auto& t : tests) {
            const std::string& name = t.first;
            const TestFn& fn = t.second;
            try {
                fn(); // Execute test
                std::cout << "[PASS]  " << name << std::endl;
            } catch (const std::exception& e) {
                ++failures;
                std::cout << "[FAIL]  " << name << " - Exception: " << e.what() << std::endl;
            } catch (...) {
                ++failures;
                std::cout << "[FAIL]  " << name << " - Unknown exception" << std::endl;
            }
        }
        return failures;
    }

private:
    std::vector<std::pair<std::string, TestFn>> tests;
};

// Entry point for the test suite
int main() {
    TestSuite suite;

    // Register tests with explanatory comments
    // Test: Not Equal Double Arrays Negative 2 (focal case)
    // Purpose: Validate that two 4-element double arrays with a single differing element
    // trigger the expected failure path (as per the Unity test scenario).
    suite.add("testNotEqualDoubleArraysNegative2_cpp",
              []() { testNotEqualDoubleArraysNegative2_cpp(); });

    // Test: Equal Double Arrays (coverage for non-failure path)
    // Purpose: Ensure that identical double arrays do not trigger a failure.
    suite.add("testEqualDoubleArraysNoThrow_cpp",
              []() { testEqualDoubleArraysNoThrow_cpp(); });

    // Run all registered tests and report summary
    int failures = suite.run();
    if (failures > 0) {
        std::cout << "TOTAL: " << failures << " test(s) failed." << std::endl;
        return 2;
    } else {
        std::cout << "TOTAL: All tests passed." << std::endl;
        return 0;
    }
}