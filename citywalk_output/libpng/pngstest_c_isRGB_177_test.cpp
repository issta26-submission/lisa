// Test suite for the focal method isRGB(int fixed_linear) from pngstest.c
// This test is written for C++11 and linked against the C implementation.
// Build guidance (to ensure compatibility): compile pngstest.c with
// -DPNG_SIMPLIFIED_READ_SUPPORTED (to avoid the C file providing its own main), and
// compile this test file as a separate executable, linking against the object
// of pngstest.c. Then run the produced executable.
// This test uses a minimal, self-contained harness (no GoogleTest•GMock).

#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declarations of the focal function and its dependency from the C file under test.
// We declare them with C linkage to avoid name mangling when linking with C++.
double isRGB(int fixed_linear);
double sRGB(double linear);
#ifdef __cplusplus
}
#endif

// Simple, domain-appropriate test harness
static int failed_tests = 0;

static void log_pass(const char* name) {
    std::cout << "[PASS] " << name << std::endl;
}
static void log_fail(const char* name, const char* details) {
    std::cerr << "[FAIL] " << name << " - " << details << std::endl;
    ++failed_tests;
}
static void report_result(const char* name, bool passed, const char* details) {
    if (passed) log_pass(name);
    else log_fail(name, details);
}

// Helper: approximate equality for floating point comparisons
static bool approx_equal(double a, double b, double tol = 1e-9) {
    // Handle NaN and Inf gracefully
    if (!std::isfinite(a) || !std::isfinite(b)) return false;
    return std::fabs(a - b) <= tol;
}

// Test 1: isRGB(0) should map to sRGB(0.0) as per the wrapper logic.
static void test_isRGB_zero() {
    const char* test_name = "isRGB(0) == sRGB(0.0)";
    int input = 0;
    double expected = sRGB(0.0);
    double actual = isRGB(input);
    bool pass = approx_equal(actual, expected);
    if (!pass) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
                      "actual=%f, expected=%f (input=%d)", actual, expected, input);
        report_result(test_name, false, buf);
    } else {
        report_result(test_name, true, "values are approximately equal.");
    }
}

// Test 2: isRGB(65535) should map to sRGB(1.0).
static void test_isRGB_max() {
    const char* test_name = "isRGB(65535) == sRGB(1.0)";
    int input = 65535;
    double expected = sRGB(1.0);
    double actual = isRGB(input);
    bool pass = approx_equal(actual, expected);
    if (!pass) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
                      "actual=%f, expected=%f (input=%d)", actual, expected, input);
        report_result(test_name, false, buf);
    } else {
        report_result(test_name, true, "values are approximately equal.");
    }
}

// Test 3: isRGB with a mid-range value to exercise the division and the sRGB path.
// We choose a non-edge value and verify against direct sRGB computation.
static void test_isRGB_mid() {
    const char* test_name = "isRGB(32767) == sRGB(32767/65535.0)";
    int input = 32767;
    double normalized = static_cast<double>(input) / 65535.0;
    double expected = sRGB(normalized);
    double actual = isRGB(input);
    bool pass = approx_equal(actual, expected);
    if (!pass) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
                      "actual=%f, expected=%f (input=%d, norm=%f)", actual, expected, input, normalized);
        report_result(test_name, false, buf);
    } else {
        report_result(test_name, true, "values are approximately equal.");
    }
}

// Test 4: Ensure isRGB returns a finite double and lies in a plausible range [0,1] for typical inputs.
// This validates basic behavioral expectations without relying on specific sRGB output.
static void test_isRGB_bounds() {
    const char* test_name = "isRGB outputs finite value within [0,1] for typical inputs";
    int inputs[] = {0, 12345, 40000, 65535};
    bool all_in_range = true;
    bool all_finite = true;
    for (int i = 0; i < 4; ++i) {
        int in = inputs[i];
        double val = isRGB(in);
        if (!std::isfinite(val)) {
            all_finite = false;
            break;
        }
        if (val < 0.0 - 1e-9 || val > 1.0 + 1e-9) {
            all_in_range = false;
            // Do not break to report all issues comprehensively
        }
    }
    if (all_finite && all_in_range) {
        log_pass(test_name);
    } else {
        char details[256];
        std::snprintf(details, sizeof(details),
                      "finite=%d, in_range=%d", all_finite ? 1 : 0, all_in_range ? 1 : 0);
        log_fail(test_name, details);
        ++failed_tests;
    }
}

// Entry point: run all tests and report summary
int main(void) {
    // Run tests in a deterministic order
    test_isRGB_zero();
    test_isRGB_max();
    test_isRGB_mid();
    test_isRGB_bounds();

    if (failed_tests > 0) {
        std::cerr << "Total failed tests: " << failed_tests << std::endl;
        return EXIT_FAILURE;
    } else {
        std::cout << "All tests passed." << std::endl;
        return EXIT_SUCCESS;
    }
}