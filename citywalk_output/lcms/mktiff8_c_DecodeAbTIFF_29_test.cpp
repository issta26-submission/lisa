/*
Unit test suite for DecodeAbTIFF(double ab)

Step 1 - Candidate Keywords (from understanding the focal method)
- DecodeAbTIFF: the function under test
- Threshold: 128.0 (input <= 128 triggers the "add 127" branch)
- Offset: 127.0 (value added when ab <= 128, value subtracted when ab > 128)
- Branch coverage: test inputs on both sides of 128 to exercise both branches
- Data type: double (floating-point behavior, precise comparisons with tolerance)

Note: The tested function DecodeAbTIFF is defined in C code (mktiff8.c). This test declares it with C linkage to link against the C implementation from C++11.

This test suite uses a lightweight, non-terminating assertion approach (manual EXPECT-like checks) to maximize code execution and coverage without relying on GTest.
*/

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <lcms2.h>
#include <cmath>


extern "C" double DecodeAbTIFF(double ab); // Declaration for the C function under test

// Simple non-terminating assertion helper: prints failure details but does not exit
static void assert_close(double observed, double expected, double tol, const char* description) {
    if (std::fabs(observed - expected) <= tol) {
        std::cout << "[PASS] " << description << " (observed=" << observed << ", expected=" << expected << ", tol=" << tol << ")\n";
    } else {
        std::cerr << "[FAIL] " << description
                  << " (observed=" << observed
                  << ", expected=" << expected
                  << ", tol=" << tol << ")\n";
    }
}

// Wrapper to run a single test case
static void run_case(double input, double expected, const char* description, double tol = 1e-9) {
    double result = DecodeAbTIFF(input);
    assert_close(result, expected, tol, description);
}

int main() {
    // Summary-oriented test execution without aborting on first failure
    // Branch coverage:
    // - ab <= 128.0 should trigger addition by 127.0
    // - ab > 128.0 should trigger subtraction by 127.0

    // Test 1: ab = 0.0 (<= 128): expect 0.0 + 127.0 = 127.0
    run_case(0.0, 127.0, "DecodeAbTIFF: input 0.0 should add 127 (<=128 branch)");

    // Test 2: ab = 128.0 (boundary <= 128): expect 128.0 + 127.0 = 255.0
    run_case(128.0, 255.0, "DecodeAbTIFF: input 128.0 should add 127 (boundary case)");

    // Test 3: ab = 128.001 (just above 128): expect 128.001 - 127.0 = 1.001
    run_case(128.001, 1.001, "DecodeAbTIFF: input 128.001 should subtract 127 (just above 128)");

    // Test 4: ab = 129.0 ( > 128): expect 129.0 - 127.0 = 2.0
    run_case(129.0, 2.0, "DecodeAbTIFF: input 129.0 should subtract 127 (greater than 128)");

    // Test 5: ab = -50.0 (<= 128): expect -50.0 + 127.0 = 77.0
    run_case(-50.0, 77.0, "DecodeAbTIFF: negative input should still use add-127 path (<=128)");

    // Test 6: ab = 300.0 ( > 128): expect 300.0 - 127.0 = 173.0
    run_case(300.0, 173.0, "DecodeAbTIFF: input 300.0 should subtract 127 (large >128)");

    // Test 7: ab = 0.5 (<= 128): expect 0.5 + 127.0 = 127.5
    run_case(0.5, 127.5, "DecodeAbTIFF: input 0.5 should add 127 (fractional <=128)");

    // Summary: total number of tests = 7
    std::cout << "Test suite completed.\n";
    return 0;
}