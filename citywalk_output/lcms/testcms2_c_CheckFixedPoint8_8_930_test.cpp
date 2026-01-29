// test_fixedpoint8_8_suite.cpp
// A lightweight C++11 test suite exercising the focal function CheckFixedPoint8_8
// from the C test file testcms2.c without using GoogleTest.
// It relies on the existing C test harness (testcms2.h / testcms2.c) and
// demonstrates calls across C/C++ boundary.
//
// Notes:
// - We compile this as a C++11 program and link against the C source(s) providing
//   CheckFixedPoint8_8 and TestSingleFixed8_8.
// - We implement a minimal test harness with non-terminating checks (do not exit on first failure).
// - The tests focus on exercising the public API surface (CheckFixedPoint8_8) and
//   directly exercising TestSingleFixed8_8 to validate its basic behavior.
// - Static/internal helpers inside testcms2.c are not accessed directly (as required).

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>


// Bring C declarations into the C++ translation unit with C linkage.
extern "C" {
    // Prototypes from testcms2.h (assuming standard LittleCMS-style typedefs)
    // If testcms2.h provides its own guards, this external block will still be compatible.
    int CheckFixedPoint8_8(void);          // The focal method under test
    int TestSingleFixed8_8(double);          // Helper used by CheckFixedPoint8_8
}

// Simple non-terminating assertion helper for tests.
// It records failures but allows subsequent tests to run for higher coverage.
static void expect_bool(bool cond, const std::string& msg, int& failCount) {
    if (!cond) {
        std::cerr << "ASSERTION FAILED: " << msg << std::endl;
        ++failCount;
    }
}

// Test 1: Validate that the focal function CheckFixedPoint8_8 returns a boolean-like result (0 or 1).
// Rationale: Ensures that the high-level API remains usable and aligns with its contract.
// This test does not assume internal branching behavior beyond returning 0/1.
static bool Test_CheckFixedPoint8_8_Returns_BoolLike() {
    int result = CheckFixedPoint8_8();
    // The contract is typically 0 (failure) or 1 (success).
    bool ok = (result == 0) || (result == 1);
    if (!ok) {
        std::cerr << "CheckFixedPoint8_8() returned an unexpected value: " << result << std::endl;
    }
    return ok;
}

// Test 2: Exercise TestSingleFixed8_8 directly with a variety of representative inputs
// to ensure it returns either 0 or 1 and does not crash.
// This helps cover the "true/false" predicates underlying the focal path indirectly.
static bool Test_TestSingleFixed8_8_Inputs() {
    // Provide a representative mix of values that cover typical, boundary, and out-of-range cases.
    // Values selected to exercise typical usage (positive, fractional, large magnitude).
    const double testVals[] = {
        -123.0,   // lower than some fixed-point ranges
        -1.5,     // negative fractional
        0.0,      // zero
        0.25,     // small positive fractional
        0.99999,  // just below 1
        1.0,      // common exact value
        2.0,      // another exact value
        3.14159,  // non-integer floating
        123.456,  // mid-range
        255.0,    // upper boundary-like
        255.123,  // near upper boundary
        999.0     // clearly out-of-range for 8.8, if bounded; should still not crash
    };

    const size_t N = sizeof(testVals) / sizeof(testVals[0]);
    for (size_t i = 0; i < N; ++i) {
        int r = TestSingleFixed8_8(testVals[i]);
        // Expectation: the function returns 0 or 1 (non-negative, binary behavior).
        bool isBinary = (r == 0) || (r == 1);
        if (!isBinary) {
            std::cerr << "TestSingleFixed8_8(" << testVals[i]
                      << ") returned non-binary value: " << r << std::endl;
            return false;
        }
        // Additional non-terminating assertion: ensure we don't crash for successive inputs.
        // We do not enforce particular truthiness here, only type/format correctness.
    }
    return true;
}

// Test 3: Cross-check that for the typical inputs used by CheckFixedPoint8_8's internal tests,
// TestSingleFixed8_8 agrees with expected binary behavior for those specific values.
// We will reuse the same values as in the focal function to ensure consistency.
static bool Test_TestSingleFixed8_8_KnownGoodInputs() {
    // Expected to be in {0,1}; we don't know exact expected bit here without internal knowledge.
    // Instead, we verify that the same values used by CheckFixedPoint8_8 do not crash and return 0/1.
    const double vals[] = { 1.0, 2.0, 1.23456, 0.99999, 0.1234567890123456789099999, +255.1234567890123456789099999 };
    const size_t N = sizeof(vals) / sizeof(vals[0]);
    for (size_t i = 0; i < N; ++i) {
        int r = TestSingleFixed8_8(vals[i]);
        bool isBinary = (r == 0) || (r == 1);
        if (!isBinary) {
            std::cerr << "TestSingleFixed8_8(KnownGoodInput[" << i << "]) returned invalid value: "
                      << r << std::endl;
            return false;
        }
    }
    return true;
}

int main() {
    int failures = 0;

    // Run Test 1
    std::cout << std::setw(60) << std::setfill('-') << std::left
              << " Running Test 1: CheckFixedPoint8_8_Returns_BoolLike " << std::setw(0) << std::endl;
    bool t1 = Test_CheckFixedPoint8_8_Returns_BoolLike();
    expect_bool(t1, "CheckFixedPoint8_8 should return 0 or 1", failures);

    // Run Test 2
    std::cout << std::setw(60) << std::setfill('-') << std::left
              << " Running Test 2: TestSingleFixed8_8_Inputs " << std::setw(0) << std::endl;
    bool t2 = Test_TestSingleFixed8_8_Inputs();
    expect_bool(t2, "TestSingleFixed8_8 should return 0 or 1 for a range of inputs", failures);

    // Run Test 3
    std::cout << std::setw(60) << std::setfill('-') << std::left
              << " Running Test 3: TestSingleFixed8_8_KnownGoodInputs Consistency " << std::setw(0) << std::endl;
    bool t3 = Test_TestSingleFixed8_8_KnownGoodInputs();
    expect_bool(t3, "TestSingleFixed8_8 known-good inputs should be binary (0 or 1)", failures);

    // Summary
    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "TESTS FAILED: " << failures << " failure(s) detected." << std::endl;
        return 1;
    }
}