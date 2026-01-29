// This file provides a small, self-contained C++11 test harness to validate
// the INT16 within-delta behavior used by testINT16sWithinDelta in the
// provided focal Unity-based file. It purposefully avoids GTest and uses a
// lightweight non-terminating assertion framework so that all test cases run
// and report their outcomes, maximizing coverage.
//
// Step 1 – Understanding (Candidate Keywords)
// - Core concept: within-delta assertion for 16-bit integers.
// - Key components: delta (tolerance), expected value, actual value, abs difference.
// - Dependent elements: 16-bit integers (int16_t), non-aborting assertions, test name
//   strings, and a small static helper to simulate static member usage.
//
// Step 2 – Unit Test Generation
// - Create test cases mirroring the focal method's scenarios:
//   1) Within delta true: (delta=1, 5000 vs 5001)
//   2) Within delta true: (delta=5, 2 vs -2)
//   3) Within delta true: (delta=5, -2 vs 2)
// - Additional coverage (non-focal but useful): boundary/false cases and static member usage.
// Step 3 – Test Case Refinement
// - Use non-terminating assertions to keep the test running through failures.
// - Collect and report per-test-case results, including a summary.
//
// Notes:
// - The test harness is standalone and does not rely on Unity during execution.
// - It simulates static member behavior via a simple static function to illustrate
//   test coverage of static-like aspects per the domain knowledge.

#include <cmath>
#include <cstdint>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (for reference in comments)
// - TEST_ASSERT_INT16_WITHIN equivalent: withinInt16
// - Delta / tolerance: int16_t delta
// - Values: 5000, 5001, 2, -2
// - Data type: int16_t
// - Static-like behavior: static helper function
// - Non-terminating assertions

// Namespace encapsulation to avoid global symbol collisions in larger suites
namespace SimpleCppTestFramework {

    // Lightweight non-aborting within-delta check for 16-bit integers.
    // We cast to int32_t for the difference to avoid overflow during subtraction.
    static inline bool withinInt16(int16_t expected, int16_t actual, int16_t delta) {
        int32_t diff = static_cast<int32_t>(expected) - static_cast<int32_t>(actual);
        if (diff < 0) diff = -diff;
        return diff <= static_cast<int32_t>(delta);
    }

    // Simple test assertion helpers
    class SimpleTestRunner;

    struct Assertion {
        std::string description;
        bool passed;
    };

    class SimpleTestRunner {
    public:
        SimpleTestRunner() : totalAssertions(0), failedAssertions(0) {}

        // Assert that two int16_t values are within a given delta
        void assertWithin(const std::string& desc, int16_t delta, int16_t expected, int16_t actual) {
            bool ok = withinInt16(expected, actual, delta);
            assertions.push_back({desc, ok});
            ++totalAssertions;
            if (!ok) ++failedAssertions;
        }

        // Simple equality assertion for int16_t (to cover static-like behavior)
        void assertEquals(const std::string& desc, int16_t expected, int16_t actual) {
            bool ok = (expected == actual);
            assertions.push_back({desc, ok});
            ++totalAssertions;
            if (!ok) ++failedAssertions;
        }

        // Accessor for summary
        int getTotal() const { return totalAssertions; }
        int getFailed() const { return failedAssertions; }

        // Produce a concise, readable summary of test results
        void summary() const {
            std::cout << std::boolalpha;
            std::cout << "Total assertions: " << totalAssertions << "\n";
            std::cout << "Failed assertions: " << failedAssertions << "\n";
            if (assertions.empty()) {
                std::cout << "(No assertions recorded)\n";
                return;
            }
            for (size_t i = 0; i < assertions.size(); ++i) {
                const auto& a = assertions[i];
                std::cout << std::setw(2) << i+1 << ". "
                          << (a.passed ? "PASS" : "FAIL") << " - " << a.description << "\n";
            }
        }

        // Convenience to reset the current run (not strictly needed here)
        void reset() {
            assertions.clear();
            totalAssertions = 0;
            failedAssertions = 0;
        }

    private:
        std::vector<Assertion> assertions;
        int totalAssertions;
        int failedAssertions;
    };

    // Static helper to emulate static member behavior in tests
    // This helps cover scenarios where static state affects test outcomes.
    static int StaticCounter() {
        static int s = 0;
        ++s;
        return s;
    }

} // namespace SimpleCppTestFramework

using namespace SimpleCppTestFramework;

// FOCAL_TESTS
// The following tests are designed to mirror the intent of testINT16sWithinDelta
// found in the focal testunity.c, but implemented in a lightweight C++11 harness.

namespace {

// Test 1: Basic within-delta checks for INT16
// Mirrors:
//   TEST_ASSERT_INT16_WITHIN(1, 5000, 5001);
//   TEST_ASSERT_INT16_WITHIN(5, 2, -2);
//   TEST_ASSERT_INT16_WITHIN(5, -2, 2);
void testINT16sWithinDelta_Basic(SimpleTestRunner& runner) {
    // delta=1: 5000 vs 5001 differ by 1 -> within
    runner.assertWithin("INT16 within delta - delta=1: 5000 vs 5001", 1, 5000, 5001);
    // delta=5: 2 vs -2 differ by 4 -> within
    runner.assertWithin("INT16 within delta - delta=5: 2 vs -2", 5, 2, -2);
    // delta=5: -2 vs 2 differ by 4 -> within
    runner.assertWithin("INT16 within delta - delta=5: -2 vs 2", 5, -2, 2);
}

// Test 2: Additional coverage to ensure non-false positives/negatives are handled
// - Within delta true: boundary case
// - Within delta false: slightly outside boundary
void testINT16sWithinDelta_EdgeCases(SimpleTestRunner& runner) {
    // Boundary true: diff = 1, delta=1
    runner.assertWithin("INT16 within delta boundary true: 0 vs 1, delta=1", 1, 0, 1);

    // Outside boundary: diff = 2, delta=1
    runner.assertWithin("INT16 within delta boundary false: 0 vs 2, delta=1", 1, 0, 2);
}

// Test 3: Static-like behavior coverage to exercise static members in tests
// - Validates that a static counter increments across calls, and can be asserted
//   for deterministic behavior.
void testINT16sWithinDelta_StaticCounter(SimpleTestRunner& runner) {
    int a = StaticCounter(); // first call -> 1
    int b = StaticCounter(); // second call -> 2
    // Check that static counter behaved deterministically
    runner.assertEquals("StaticCounter first call returns 1", 1, static_cast<int>(a));
    runner.assertEquals("StaticCounter second call returns 2", 2, static_cast<int>(b));
}

// Wrapper function to invoke all focal tests for this file
void runAllFocalTests(SimpleTestRunner& runner) {
    testINT16sWithinDelta_Basic(runner);
    testINT16sWithinDelta_EdgeCases(runner);
    testINT16sWithinDelta_StaticCounter(runner);
}

// A tiny helper to simulate the focus file's single-test invocation style.
// This is not a Unity test harness; it is a focused, self-contained test suite.
} // anonymous namespace

int main() {
    SimpleTestRunner runner;

    // Run all focal tests
    runAllFocalTests(runner);

    // Print a detailed summary
    runner.summary();

    // Return non-zero if any test failed, to signal issues to the caller
    return (runner.getFailed() > 0) ? 1 : 0;
}