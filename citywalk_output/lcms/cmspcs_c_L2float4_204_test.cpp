/*
  Test Suite for the focal method:
  cmsFloat64Number L2float4(cmsUInt16Number v)

  Context and design notes (Step 1-3):
  - Core behavior: L2float4 converts a 16-bit unsigned input to a double by dividing by 655.35,
    effectively mapping 0..65535 -> 0..~100.0.
  - Candidate Keywords identified from the focal class/file dependencies:
    L2float4, L2float2, L2Fix2/4, Clamp_L_doubleV*, ComputeLBFD, XYZ2float, etc.
    The tests focus on L2float4's transformation and its numeric boundaries.
  - The test suite uses a lightweight, non-terminating assertion approach (no GTest).
  - Tests are written in C++11 style, using only standard library facilities and the
    provided L2float4 function from the library (extern "C" linkage assumed).
  - The tests assume cmspcs.c (or equivalent) is part of the build and L2float4 is
    linked in; the prototype is declared here with C linkage.
  - Output is designed for straightforward inclusion in a CI-like workflow without
    requiring Google Test. Each test includes explanatory comments.
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cmath>
#include <cstdlib>


// Import CMS internal types; path may vary in your project.
// This header is required to obtain the typedefs like cmsFloat64Number and cmsUInt16Number.
extern "C" {

// Focal method under test: L2float4
// Signature assumed from the provided snippet.
// The actual implementation is expected to be in cmspcs.c (or linked in the project).
cmsFloat64Number L2float4(cmsUInt16Number v);
}

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_passed = 0;

// Utility: check near-equality with a tolerance; records results and prints on failure.
static bool check_near(const char* test_name, double actual, double expected, double tol)
{
    ++g_total;
    double diff = std::fabs(actual - expected);
    if (diff <= tol) {
        ++g_passed;
        return true;
    } else {
        // Non-terminating: report and continue
        std::fprintf(stderr,
                     "FAIL: %s | actual=%0.12f, expected=%0.12f, diff=%0.12f, tol=%0.12f\n",
                     test_name, actual, expected, diff, tol);
        return false;
    }
}

// Helper: run all tests and print a final summary
static void print_summary()
{
    std::printf("L2float4 Test Summary: %d passed, %d total\n", g_passed, g_total);
    // Non-fatal behavior: exit code reflects success status
    if (g_total == g_passed) {
        std::printf("ALL TESTS PASSED\n");
    } else {
        std::printf("SOME TESTS FAILED\n");
    }
}

// Test 1: Zero input -> expect 0.0
// Verifies the lower boundary behavior of the conversion.
static void test_L2float4_zero()
{
    cmsUInt16Number v = 0;
    cmsFloat64Number res = L2float4(v);
    double expected = static_cast<double>(v) / 655.35; // 0 / 655.35 == 0.0
    check_near("L2float4(0) -> 0.0", static_cast<double>(res), expected, 1e-12);
}

/*
  Test 2: Maximum input (65535) -> expect 100.0
  This exercises the upper boundary and confirms the exact mapping:
  65535 / 655.35 = 100.0 exactly in ideal arithmetic.
*/
static void test_L2float4_max()
{
    cmsUInt16Number v = 65535;
    cmsFloat64Number res = L2float4(v);
    double expected = static_cast<double>(v) / 655.35;
    check_near("L2float4(65535) -> ~100.0", static_cast<double>(res), expected, 1e-12);
}

// Test 3: Mid-range value (approx 50.0)
static void test_L2float4_mid()
{
    cmsUInt16Number v = 32767; // close to half of 65535
    cmsFloat64Number res = L2float4(v);
    double expected = static_cast<double>(v) / 655.35;
    check_near("L2float4(32767) -> ~50.0", static_cast<double>(res), expected, 1e-12);
}

// Test 4: Small non-zero value to exercise precision near 0
static void test_L2float4_small()
{
    cmsUInt16Number v = 1;
    cmsFloat64Number res = L2float4(v);
    double expected = static_cast<double>(v) / 655.35;
    check_near("L2float4(1) -> ~1.525e-3", static_cast<double>(res), expected, 1e-12);
}

// Test 5: Random representative sample to increase coverage
static void test_L2float4_random_sample()
{
    // A few representative values spanning the range
    const cmsUInt16Number samples[] = { 0, 5, 123, 1000, 5000, 12345, 40000, 65534 };
    const int N = sizeof(samples) / sizeof(samples[0]);
    for (int i = 0; i < N; ++i) {
        cmsUInt16Number v = samples[i];
        cmsFloat64Number res = L2float4(v);
        double expected = static_cast<double>(v) / 655.35;
        char test_name[64];
        std::snprintf(test_name, sizeof(test_name), "L2float4(%u)", static_cast<unsigned>(v));
        check_near(test_name, static_cast<double>(res), expected, 1e-12);
    }
}

// Main entry: execute all tests and print summary
int main()
{
    // Execute tests
    test_L2float4_zero();
    test_L2float4_max();
    test_L2float4_mid();
    test_L2float4_small();
    test_L2float4_random_sample();

    // Final summary
    print_summary();

    // Return non-zero if any test failed
    return (g_total == g_passed) ? 0 : 1;
}