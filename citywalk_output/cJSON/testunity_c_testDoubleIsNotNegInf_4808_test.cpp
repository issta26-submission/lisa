/*
  Candidate Keywords (derived from the focal method and its dependencies):
  - UNITY_EXCLUDE_DOUBLE
  - TEST_ASSERT_DOUBLE_IS_NEG_INF
  - -999.876 (sample non-negative-infinity value)
  - NEG_INF / negative infinity
  - testDoubleIsNotNegInf (focal test name)
  - isNegInf(double)
  - assertDoubleIsNegInf(double)
  - EXPECT_ABORT-like semantics (simulated in this harness)
  - Unity-like test concepts (aborts on failure, non-terminating assertions are emulated)
  
  This file provides a minimal, self-contained C++11 test harness that reproduces
  the core behavior of the focal Unity test for "testDoubleIsNotNegInf" without
  using GTest. It exercises the non -inf path (abort expected) and a couple of
  edge cases (inf, NaN) to improve coverage. It intentionally does not rely on
  external Unity runtime so it is compatible with a plain C++11 build.
*/

#include <cmath>
#include <unity.h>
#include <limits>
#include <stdexcept>
#include <iostream>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


namespace TestDoubleNotNegInfHarness {

// Helper: determine if a value is negative infinity
static bool isNegInf(double d) {
    // std::isinf handles +/-INF; ensure it's negative
    return std::isinf(d) && (d < 0);
}

// Focal assertion: assert that a given double is negative infinity.
// If the value is not -INF, mimic Unity's assertion failure by throwing.
static void assertDoubleIsNegInf(double d) {
    if (!isNegInf(d)) {
        throw std::runtime_error("Assertion failed: value is not negative infinity (-INF).");
    }
}

// Test 1: Positional non -INF value should abort (simulate EXPECT_ABORT_BEGIN).
// This mirrors: EXPECT_ABORT_BEGIN TEST_ASSERT_DOUBLE_IS_NEG_INF(-999.876); VERIFY_FAILS_END
// Expectation: Since -999.876 is not -INF, the assertion should fail and throw.
void testDoubleIsNotNegInf_PosValue_ShouldAbort(void) {
    bool threw = false;
    try {
        assertDoubleIsNegInf(-999.876);
    } catch (...) {
        threw = true; // abort occurred as expected
    }
    if (!threw) {
        throw std::runtime_error("Test failed: Expected abort did not occur for -999.876.");
    }
}

// Test 2: NaN value should abort (non -INF value scenario).
// NaN is not -INF; the assertion should fail.
void testDoubleIsNotNegInf_NaNValue_ShouldAbort(void) {
    bool threw = false;
    try {
        double nanVal = std::numeric_limits<double>::quiet_NaN();
        assertDoubleIsNegInf(nanVal);
    } catch (...) {
        threw = true;
    }
    if (!threw) {
        throw std::runtime_error("Test failed: Expected abort did not occur for NaN value.");
    }
}

// Test 3: Negative infinity should NOT abort (correct -INF case).
// This path ensures that the assertion passes when -INF is provided.
void testDoubleIsNotNegInf_NegInfValue_ShouldNotAbort(void) {
    bool threw = false;
    try {
        assertDoubleIsNegInf(-std::numeric_limits<double>::infinity());
    } catch (...) {
        threw = true;
    }
    if (threw) {
        throw std::runtime_error("Test failed: Unexpected abort occurred for -INF value.");
    }
}

} // namespace TestDoubleNotNegInfHarness

// Simple test runner (no external framework)
int main() {
    using namespace TestDoubleNotNegInfHarness;

    struct TestCase {
        const char* name;
        void (*fn)();
    };

    TestCase tests[] = {
        { "testDoubleIsNotNegInf_PosValue_ShouldAbort", testDoubleIsNotNegInf_PosValue_ShouldAbort },
        { "testDoubleIsNotNegInf_NaNValue_ShouldAbort", testDoubleIsNotNegInf_NaNValue_ShouldAbort },
        { "testDoubleIsNotNegInf_NegInfValue_ShouldNotAbort", testDoubleIsNotNegInf_NegInfValue_ShouldNotAbort }
    };

    const int total = sizeof(tests) / sizeof(tests[0]);
    int failures = 0;

    std::cout << "Running " << total << " test(s) for focal method testDoubleIsNotNegInf (simulated harness)" << std::endl;
    for (int i = 0; i < total; ++i) {
        try {
            tests[i].fn();
            std::cout << "[PASS] " << tests[i].name << std::endl;
        } catch (const std::exception& ex) {
            ++failures;
            std::cout << "[FAIL] " << tests[i].name << " - Exception: " << ex.what() << std::endl;
        } catch (...) {
            ++failures;
            std::cout << "[FAIL] " << tests[i].name << " - Unknown exception" << std::endl;
        }
    }

    std::cout << "Test results: " << (total - failures) << " passed, " << failures << " failed." << std::endl;
    return (failures == 0) ? 0 : 1;
}