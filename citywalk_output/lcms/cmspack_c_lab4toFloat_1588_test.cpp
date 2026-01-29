// Lightweight C++11 test suite for the focal method lab4toFloat
// This test suite is designed to exercise the lab4toFloat conversion
// from Lab4 representation (3 x cmsUInt16Number) to a 3-element
// cmsFloat32Number vector wIn (normalized 0..1 range).
// 
// Notes:
// - The focal function lab4toFloat is assumed to be accessible via
//   external linkage (extern "C") with the signature used below.
// - The tests use non-terminating assertions: failures are reported but
//   do not abort execution, to maximize coverage.
// - This test suite uses only the C++ standard library.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cmath>


// Typedefs matching the CMS types used by the focal function.
// In the real project these would come from the CMS headers.
typedef float cmsFloat32Number;
typedef unsigned short cmsUInt16Number;

// Forward declaration of the focal method under test.
// If you compile this test with the actual cmspack.c (or the library that
// provides lab4toFloat), ensure the linkage is correct.
extern "C" void lab4toFloat(cmsFloat32Number wIn[], cmsUInt16Number lab4[3]);

// Simple non-terminating assertion helper.
// Reports failures but does not stop subsequent tests.
static int g_totalAssertions = 0;
static int g_failedAssertions = 0;

static void expect_close(const std::string& desc,
                         cmsFloat32Number actual,
                         cmsFloat32Number expected,
                         cmsFloat32Number tol = 1e-6f)
{
    ++g_totalAssertions;
    if (std::fabs(actual - expected) > tol) {
        ++g_failedAssertions;
        std::cout << "FAIL  - " << desc
                  << " | actual: " << actual
                  << "   expected: " << expected
                  << "   tol: " << tol << "\n";
    } else {
        std::cout << "PASS  - " << desc
                  << " | actual: " << actual
                  << " (within tol " << tol << ")\n";
    }
}

// Helper: run a single test case by feeding lab4 inputs and validating
// that the outputs wIn[i] equal lab4[i] / 65535.0 for i=0..2.
static void run_test_case(const cmsUInt16Number lab4[3], const std::string& testName)
{
    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};
    lab4toFloat(wIn, lab4);

    cmsFloat32Number exp0 = ((cmsFloat32Number)lab4[0]) / 65535.0f;
    cmsFloat32Number exp1 = ((cmsFloat32Number)lab4[1]) / 65535.0f;
    cmsFloat32Number exp2 = ((cmsFloat32Number)lab4[2]) / 65535.0f;

    expect_close(testName + " - wIn[0] (L component)", wIn[0], exp0);
    expect_close(testName + " - wIn[1] (a component)", wIn[1], exp1);
    expect_close(testName + " - wIn[2] (b component)", wIn[2], exp2);
}

// Test 1: Edge case - all channel values are zero -> expect all outputs to be 0.0
static void test_lab4toFloat_zeros()
{
    cmsUInt16Number lab[3] = {0, 0, 0};
    run_test_case(lab, "Zeros (0,0,0)");
    // Explanatory comment:
    // When lab4 is all zeros, L=0, a=-128, b=-128,
    // after normalization to 0..1 we expect wIn = {0.0, 0.0, 0.0}.
}

// Test 2: Boundary case - all channel values are max -> expect all outputs to be 1.0
static void test_lab4toFloat_max()
{
    cmsUInt16Number lab[3] = {65535, 65535, 65535};
    run_test_case(lab, "Max values (65535,65535,65535)");
    // Explanatory comment:
    // Each component should map to 1.0 after normalization.
}

// Test 3: Midpoint case - all channel values at mid-range
static void test_lab4toFloat_mid()
{
    cmsUInt16Number lab[3] = {32767, 32767, 32767};
    run_test_case(lab, "Midpoint (32767,32767,32767)");
    // Explanatory comment:
    // 32767/65535 ≈ 0.499992..., so expect near 0.5 for all components.
}

// Test 4: Randomized representative case
static void test_lab4toFloat_random()
{
    cmsUInt16Number lab[3] = {1000, 50000, 12345};
    run_test_case(lab, "Random sample (1000, 50000, 12345)");
    // Explanatory comment:
    // Verifies typical non-edge values are correctly normalized.
}

int main()
{
    // Run tests in a deterministic order.
    // Each test uses non-terminating assertions to maximize coverage.

    std::cout << "Starting lab4toFloat tests...\n";

    test_lab4toFloat_zeros();
    test_lab4toFloat_max();
    test_lab4toFloat_mid();
    test_lab4toFloat_random();

    std::cout << "\nTest summary: "
              << "Total assertions: " << g_totalAssertions
              << ", Failures: " << g_failedAssertions << "\n";

    // Return non-zero if any test failed.
    return (g_failedAssertions == 0) ? 0 : 1;
}