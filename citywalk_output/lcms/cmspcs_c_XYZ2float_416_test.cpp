/*
Minimal C++11 test suite for the focal method XYZ2float in cmspcs.c
- Uses a lightweight, non-terminating assertion approach (PASS/FAIL logs)
- Does not rely on GTest; test runner is invoked from main
- Imports necessary CMS types and function prototype
- Verifies the behavior of XYZ2float by comparing against a deterministic
  expected value derived from the fixed-point conversion semantics
*/

#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Import the internal CMS types and the focal function prototype.
// Assumes that lcms2_internal.h provides cmsUInt16Number and cmsFloat64Number

// Declare the focal function (C linkage) to ensure correct linkage in C++ test code.
extern "C" cmsFloat64Number XYZ2float(cmsUInt16Number v);

// Simple global counter to track failures (non-terminating assertions)
static int g_failures = 0;

// Basic assertion helper: compares two doubles within a tolerance.
// Logs PASS/FAIL and continues execution.
static void EXPECT_DOUBLE_EQ(const double actual, const double expected, const double tol, const std::string& msg)
{
    if (std::fabs(actual - expected) <= tol) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cerr << "[FAIL] " << msg
                  << " | Expected: " << std::setprecision(15) << std::fixed << expected
                  << " | Actual: " << actual
                  << " | Tolerance: " << tol << "\n";
        ++g_failures;
    }
}

// Convenience wrapper to run a single XYZ2float test case, given a v value.
// It computes the expected value using the known fixed-point interpretation:
// fix32 = v << 1 (cmsS15Fixed16Number), then to double => expected = fix32 / 65536.0
static void test_xyz2float_case(const cmsUInt16Number v)
{
    // Build a descriptive test name for logging
    char test_name[64];
    std::snprintf(test_name, sizeof(test_name), "XYZ2float(v=%u)", static_cast<unsigned int>(v));

    // Call the focal function
    cmsFloat64Number actual = XYZ2float(v);

    // Compute expected value using fixed-point semantics (v << 1) / 65536
    // Cast to signed 32-bit to reflect cmsS15Fixed16Number behavior
    int32_t fix32 = static_cast<int32_t>(static_cast<uint32_t>(v)) << 1;
    double expected = static_cast<double>(fix32) / 65536.0;

    // Use a tight tolerance; for exact fixed-point to double conversion this should be exact
    const double tol = 1e-12;

    // Perform the assertion
    EXPECT_DOUBLE_EQ(static_cast<double>(actual), expected, tol, std::string(test_name));
}

// Entry point for the test suite
int main() {
    // Test 1: v = 0 -> fix32 = 0; expected = 0.0
    // Verifies the lower bound
    test_xyz2float_case(0);

    // Test 2: v = 1 -> fix32 = 2; expected = 2 / 65536 = 0.000030517578125
    test_xyz2float_case(1);

    // Test 3: v = 32767 -> fix32 = 65534; expected ≈ 0.999969482421875
    test_xyz2float_case(32767);

    // Test 4: v = 32768 -> fix32 = 65536; expected = 1.0
    test_xyz2float_case(32768);

    // Test 5: v = 65535 -> fix32 = 131070; expected ≈ 1.999969482421875
    test_xyz2float_case(65535);

    // Test 6: Additional representative value to increase coverage
    // v = 12345 -> fix32 = 24690; expected = 24690 / 65536 ≈ 0.37664794921875
    test_xyz2float_case(12345);

    // Summary
    if (g_failures == 0) {
        std::cout << "\nALL TESTS PASSED (" << 6 << " cases evaluated).\n";
        return 0;
    } else {
        std::cerr << "\nTESTS FAILED: " << g_failures << " case(s) failed out of 6.\n";
        return 1;
    }
}