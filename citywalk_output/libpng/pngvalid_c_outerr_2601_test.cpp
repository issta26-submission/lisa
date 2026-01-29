// Lightweight C++11 unit test scaffold for the focal method "outerr" from pngvalid.c
// This test suite uses the actual libpng/ pngvalid types via <png.h> header.
// It does not rely on GTest; instead, it uses a minimal, non-terminating assertion style.
// Each unit test is documented with comments explaining the scenario being tested.

#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Forward declare the C function under test with C linkage so the linker finds it.
// We include the libpng header to get the proper definition of png_modifier.
extern "C" {
// The focal function under test, defined in pngvalid.c, is assumed to have C linkage.
double outerr(const png_modifier *pm, int in_depth, int out_depth);
}

// Ensure we use the C linkage when calling the C function from C++
using ::std::cout;
using ::std::cerr;
using ::std::endl;
using ::std::fabs;
using ::std::memset;

// Simple non-terminating assertion helpers
static int g_failures = 0;
static std::vector<std::string> g_failure_descriptions;

static void log_pass(const char* testname) {
    cout << "[PASS] " << testname << "\n";
}
static void log_fail(const char* testname, double got, double expected) {
    cerr << "[FAIL] " << testname << ": got " << got << ", expected " << expected << "\n";
    g_failure_descriptions.push_back(std::string(testname) + ": got " + std::to_string(got) +
                                   ", expected " + std::to_string(expected));
    ++g_failures;
}

// Helper to compare doubles within a tolerance
static void assert_close(const char* testname, double got, double expected, double eps) {
    if (fabs(got - expected) <= eps) {
        log_pass(testname);
    } else {
        log_fail(testname, got, expected);
    }
}

/*
Step-by-step unit tests for outerr:

Test 1: out_depth == 2
- Expect constant 0.73182 - 0.5 = 0.23182
- pm is irrelevant for this branch, but we provide a valid object to satisfy the API.

Test 2: out_depth == 4
- Expect constant 0.90644 - 0.5 = 0.40644

Test 3: 16-bit path when calculations_use_input_precision is true and in_depth == 16
- Expect to return pm->maxout16

Test 4: 16-bit path when calculations_use_input_precision is true and in_depth != 16
- Expect to go to else-if and return pm->maxout8 * 257 (out_depth == 16)

Test 5: General 8-bit path (out_depth == 8)
- Expect to return maxout8 regardless of in_depth, when 2 and 4 bit cases are not taken and 16-bit path not taken.
*/
static void test_outerr_depth2() {
    // Prepare a valid png_modifier object; values are irrelevant for this path.
    png_modifier pm;
    memset(&pm, 0, sizeof(pm));

    double r = outerr(&pm, 0, 2);
    const double expected = 0.23182; // 0.73182 - 0.5
    const double eps = 1e-5;
    assert_close("outerr_depth2", r, expected, eps);
}

static void test_outerr_depth4() {
    png_modifier pm;
    memset(&pm, 0, sizeof(pm));

    double r = outerr(&pm, 0, 4);
    const double expected = 0.40644; // 0.90644 - 0.5
    const double eps = 1e-5;
    assert_close("outerr_depth4", r, expected, eps);
}

static void test_outerr_depth16_in16() {
    // Test the 16-bit path that uses maxout16 when in_depth == 16 and flag is true
    png_modifier pm;
    memset(&pm, 0, sizeof(pm));
    pm.calculations_use_input_precision = 1; // true
    pm.maxout16 = 12345.0;                    // arbitrary non-zero value
    double r = outerr(&pm, 16, 16);
    assert_close("outerr_depth16_in16", r, 12345.0, 1e-5);
}

static void test_outerr_depth16_in8() {
    // Test path where in_depth != 16 and out_depth == 16 with calculations_use_input_precision = true
    // Expect maxout8 * 257
    png_modifier pm;
    memset(&pm, 0, sizeof(pm));
    pm.calculations_use_input_precision = 1; // true
    pm.maxout8 = 100;                         // maxout8
    double r = outerr(&pm, 8, 16);            // in_depth = 8, out_depth = 16, threshold not met for maxout16
    double expected = 100.0 * 257.0;           // 25700
    assert_close("outerr_depth16_in8", r, expected, 1e-5);
}

static void test_outerr_depth8() {
    // Test the general 8-bit path where out_depth == 8
    // Choose in_depth such that (calculations_use_input_precision ? in_depth : out_depth)
    // does not trigger the 16-bit case (set calculations_use_input_precision = true and in_depth != 16)
    png_modifier pm;
    memset(&pm, 0, sizeof(pm));
    pm.calculations_use_input_precision = 1; // true
    pm.maxout8 = 150;                         // arbitrary
    double r = outerr(&pm, 9, 8);             // in_depth = 9, out_depth = 8
    assert_close("outerr_depth8", r, 150.0, 1e-5);
}

static void run_all_tests() {
    test_outerr_depth2();
    test_outerr_depth4();
    test_outerr_depth16_in16();
    test_outerr_depth16_in8();
    test_outerr_depth8();
}

// Entry point
int main(void)
{
    run_all_tests();

    cout << "\nTotal failures: " << g_failures << "\n";
    if (g_failures > 0) {
        cout << "Failure details:\n";
        for (const auto &s : g_failure_descriptions) {
            cout << "  - " << s << "\n";
        }
        return 1;
    }
    return 0;
}