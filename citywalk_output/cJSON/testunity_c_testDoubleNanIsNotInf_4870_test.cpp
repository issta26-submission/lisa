// This file provides a minimal C++11 unit-test harness to exercise the focal method
// testDoubleNanIsNotInf from testunity.c without relying on Unity/GTest frameworks.
// It mirrors the intent of the Unity test: NaN should not be treated as an Inf value.
// The test suite is deliberately lightweight and self-contained to be compatible with
// a C++11 toolchain.
//
// Notes on Candidate Keywords (Step 1):
// - UNITY_EXCLUDE_DOUBLE, UNITY_EXCLUDE_FLOAT
// - d_zero, f_zero
// - TEST_ASSERT_DOUBLE_IS_INF
// - NaN (Not-a-Number), Inf (Infinity)
// - 0.0 / d_zero (used to generate NaN in the focal test)
// - isinf, isnan (std equivalents in <cmath>)
// - startPutcharSpy, endPutcharSpy (Unity I/O spying concepts -> not used here)
// - EXPECT_ABORT_BEGIN, VERIFY_FAILS_END (Unity error-assertion scaffolding -> emulated)
// - Static data members: not applicable in this minimal harness, but noted in test plan
// - Non-GTest approach: this file uses a tiny internal test runner and plain asserts via exceptions

#include <cmath>
#include <unity.h>
#include <limits>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight test framework (no external testing library)
class SimpleTestFramework {
public:
    struct TestCase {
        std::string name;
        std::function<void()> func;
        bool skipped;
    };

    void add(const std::string& name, std::function<void()> f, bool skipped = false) {
        tests.push_back({name, f, skipped});
    }

    int run() {
        int failures = 0;
        for (auto &t : tests) {
            if (t.skipped) {
                std::cout << "[IGNORED] " << t.name << "\n";
                continue;
            }
            std::cout << "[RUN  ] " << t.name << "\n";
            try {
                t.func();
                std::cout << "         PASS" << "\n";
            } catch (const std::exception& ex) {
                ++failures;
                std::cout << "         FAIL: " << ex.what() << "\n";
            } catch (...) {
                ++failures;
                std::cout << "         FAIL: unknown exception\n";
            }
        }
        return failures;
    }

private:
    std::vector<TestCase> tests;
};

#ifndef UNITY_EXCLUDE_DOUBLE
// Core test: verifies that a NaN value is not treated as Inf.
// This mirrors the Unity test's intent where 0.0 / d_zero yields NaN and should
// not satisfy TEST_ASSERT_DOUBLE_IS_INF (which would expect Inf).
static void testDoubleNanIsNotInf_Normal(void)
{
    // Generate NaN in a way that mimics the focal method: NaN = 0.0 / 0.0
    double nanValue = 0.0 / 0.0;

    // Assert semantics: NaN should not be Inf.
    if (std::isinf(nanValue)) {
        throw std::runtime_error("NaN unexpectedly detected as Inf");
    }

    // Additional sanity: value should indeed be NaN
    if (!std::isnan(nanValue)) {
        throw std::runtime_error("Result is not NaN as expected");
    }
}
#else
// If UNITY_EXCLUDE_DOUBLE is defined, this test is ignored (simulating Unity skip)
static void testDoubleNanIsNotInf_Skipped(void) { /* intentionally left blank to simulate skip */ }
#endif

#ifndef UNITY_EXCLUDE_DOUBLE
// Extra coverage: another NaN-related check to improve coverage of NaN behavior.
// It ensures that NaN is not equal to itself, a well-known IEEE-754 property.
static void testDoubleNanIsNotEqualToItself(void)
{
    double nanValue = 0.0 / 0.0;
    if (!(nanValue != nanValue)) {
        throw std::runtime_error("NaN unexpectedly equals itself (IEEE-754 violation)");
    }
}
#else
static void testDoubleNanIsNotEqualToItself_Skipped(void) { /* intentionally left blank */ }
#endif

int main()
{
    SimpleTestFramework tf;

#ifndef UNITY_EXCLUDE_DOUBLE
    // Register the normal test path (NaN should not be Inf)
    tf.add("testDoubleNanIsNotInf_Normal", testDoubleNanIsNotInf_Normal);
    // Additional NaN coverage test
    tf.add("testDoubleNanIsNotEqualToItself", testDoubleNanIsNotEqualToItself);
#else
    // Register the skip/ignore path when UNITY_EXCLUDE_DOUBLE is defined
    tf.add("testDoubleNanIsNotInf_Normal", testDoubleNanIsNotInf_Skipped, true);
    tf.add("testDoubleNanIsNotEqualToItself", testDoubleNanIsNotEqualToItself_Skipped, true);
#endif

    int failures = tf.run();
    // Return non-zero if any failures occurred
    return failures ? 1 : 0;
}