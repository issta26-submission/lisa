/*
Unit tests for cmsFloat64Number xpow10(int n) in cmscgats.c

Step 1 (Program Understanding) highlights:
- Core operation: compute 10^n using pow(10, (cmsFloat64Number)n).
- cmsFloat64Number is a floating-point type (likely double).
- Behavior includes handling negative exponents and boundary/overflow cases.

Candidate Keywords (core dependencies):
- pow, 10, cmsFloat64Number, xpow10
- double precision numeric behavior, infinities, NaNs
- integer exponent n domain

Step 2 (Unit Test Generation):
- Validate:
  - n = 0 -> 1.0
  - n = 1 -> 10.0
  - n = -1 -> 0.1
  - n = 2, 3 and their negative counterparts
  - large exponent within range (e.g., 308 -> ~1e308)
  - exponent that overflows to infinity (e.g., 309 -> inf)
  - negative large exponent (e.g., -308 -> ~1e-308)

Step 3 (Test Case Refinement):
- Use a small inline test framework (no GTest) with non-terminating assertions (continue on failure).
- Use a tolerant floating-point comparison (approxEqual) for non-infinite results.
- Handle infinities explicitly when overflow occurs.
- Test file is self-contained and relies only on standard C++11 library.

Notes:
- This test code declares the C function xpow10 with C linkage to link against the CMS ICC code in cmscgats.c.
- If cmsFloat64Number is indeed a typedef for double in the real project, this test will link correctly. If not, adjust the typedef accordingly to match the actual type.

Compile instructions (example, not part of code):
- g++ -std=c++11 -o test_xpow10 test_xpow10.cpp cmscgats.c -lm

Code:
*/

#include <limits>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <lcms2_internal.h>
#include <cmath>


// Declaration of the focal method from cmscgats.c
extern "C" double xpow10(int n);

/* Lightweight test harness (non-terminating assertions)
   - Keeps track of passed/failed tests and prints per-test results
*/
static int g_total = 0;
static int g_pass = 0;
static int g_fail = 0;

static bool approxEqual(double a, double b, double tol = 1e-12) {
    if (std::isnan(a) || std::isnan(b)) return false;
    if (std::isinf(a) || std::isinf(b)) return a == b;
    double abd = std::fabs(a - b);
    double scale = std::max(1.0, std::max(std::fabs(a), std::fabs(b)));
    return abd <= tol * scale;
}

static void logPass(const char* msg) {
    ++g_pass;
    std::cout << "[PASS] " << msg << std::endl;
}
static void logFail(const char* msg) {
    ++g_fail;
    std::cout << "[FAIL] " << msg << std::endl;
}
static void logInfo(const char* msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

/* Test case 1: n = 0 => 1.0
   Reason: 10^0 = 1
*/
static void test_case_n0() {
    ++g_total;
    double result = xpow10(0);
    double expected = 1.0;
    if (approxEqual(result, expected)) {
        logPass("xpow10(0) == 1.0");
    } else {
        logFail("xpow10(0) != 1.0");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected approx: " << expected << std::endl;
    }
}

/* Test case 2: n = 1 => 10.0
   Reason: 10^1 = 10
*/
static void test_case_n1() {
    ++g_total;
    double result = xpow10(1);
    double expected = 10.0;
    if (approxEqual(result, expected)) {
        logPass("xpow10(1) == 10.0");
    } else {
        logFail("xpow10(1) != 10.0");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected: " << expected << std::endl;
    }
}

/* Test case 3: n = -1 => 0.1
   Reason: 10^-1 = 0.1
*/
static void test_case_n_minus1() {
    ++g_total;
    double result = xpow10(-1);
    double expected = 0.1;
    if (approxEqual(result, expected)) {
        logPass("xpow10(-1) == 0.1");
    } else {
        logFail("xpow10(-1) != 0.1");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected: " << expected << std::endl;
    }
}

/* Test case 4: n = 2 => 100
   Reason: 10^2 = 100
*/
static void test_case_n2() {
    ++g_total;
    double result = xpow10(2);
    double expected = 100.0;
    if (approxEqual(result, expected)) {
        logPass("xpow10(2) == 100.0");
    } else {
        logFail("xpow10(2) != 100.0");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected: " << expected << std::endl;
    }
}

/* Test case 5: n = -2 => 0.01
   Reason: 10^-2 = 0.01
*/
static void test_case_n_minus2() {
    ++g_total;
    double result = xpow10(-2);
    double expected = 0.01;
    if (approxEqual(result, expected)) {
        logPass("xpow10(-2) == 0.01");
    } else {
        logFail("xpow10(-2) != 0.01");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected: " << expected << std::endl;
    }
}

/* Test case 6: n = 3 => 1000
   Reason: 10^3 = 1000
*/
static void test_case_n3() {
    ++g_total;
    double result = xpow10(3);
    double expected = 1000.0;
    if (approxEqual(result, expected)) {
        logPass("xpow10(3) == 1000.0");
    } else {
        logFail("xpow10(3) != 1000.0");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected: " << expected << std::endl;
    }
}

/* Test case 7: n = -3 => 0.001
   Reason: 10^-3 = 0.001
*/
static void test_case_n_minus3() {
    ++g_total;
    double result = xpow10(-3);
    double expected = 0.001;
    if (approxEqual(result, expected)) {
        logPass("xpow10(-3) == 0.001");
    } else {
        logFail("xpow10(-3) != 0.001");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected: " << expected << std::endl;
    }
}

/* Test case 8: n = 308 => ~1e308
   Reason: Large exponent within double range
*/
static void test_case_n308() {
    ++g_total;
    double result = xpow10(308);
    double expected = 1e308; // within double range
    if (approxEqual(result, expected)) {
        logPass("xpow10(308) ≈ 1e308");
    } else {
        logFail("xpow10(308) != ~1e308");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected ≈ " << expected << std::endl;
    }
}

/* Test case 9: n = 309 => overflow to +inf
   Reason: 10^309 exceeds DBL_MAX -> inf
*/
static void test_case_n309_overflow() {
    ++g_total;
    double result = xpow10(309);
    if (std::isinf(result) && result > 0) {
        logPass("xpow10(309) correctly overflows to +inf");
    } else {
        logFail("xpow10(309) did not overflow to +inf as expected");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", isinf = " << (std::isinf(result) ? "true" : "false") << std::endl;
    }
}

/* Test case 10: n = -308 => ~1e-308
   Reason: Very small positive exponent
*/
static void test_case_n_minus308() {
    ++g_total;
    double result = xpow10(-308);
    double expected = 1e-308;
    if (approxEqual(result, expected)) {
        logPass("xpow10(-308) == ~1e-308");
    } else {
        logFail("xpow10(-308) != ~1e-308");
        std::cout << "  Actual: " << std::setprecision(15) << result
                  << ", Expected: " << expected << std::endl;
    }
}

int main() {
    // Run all test cases
    test_case_n0();
    test_case_n1();
    test_case_n_minus1();
    test_case_n2();
    test_case_n_minus2();
    test_case_n3();
    test_case_n_minus3();
    test_case_n308();
    test_case_n309_overflow();
    test_case_n_minus308();

    // Summary
    std::cout << "\nTest Summary: " << g_pass << " passed, " << g_fail << " failed, "
              << g_total << " total tests." << std::endl;

    // Exit with non-zero if any test failed
    return (g_fail == 0) ? 0 : 1;
}