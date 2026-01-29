/*
This test suite targets the focal method:
    cmsFloat64Number L2float2(cmsUInt16Number v)

Functionality recap:
- Performs a trivial conversion: cast v to a 64-bit floating point number and divide by 652.800.
- No branching logic inside L2float2; behavior is strictly defined by the numerical operation.

Step 0: Candidate Keywords (core dependent components)
- cmsFloat64Number, cmsUInt16Number, L2float2
- Casting, division, numeric literal 652.800
- Dependency: lcms2_internal.h (types and function availability)
- File under test: cmspcs.c

Step 1: Included dependencies and linkage
- This test uses the exact types and function signature as defined by the project (via lcms2_internal.h).
- L2float2 is declared with C linkage to ensure correct linking with the C implementation.

Step 2: Test plan (mapped to L2float2)
- Validate L2float2(0) -> 0.0
- Validate L2float2(652) -> 652 / 652.800 (approx value)
- Validate L2float2(6528) -> 10.0 (6528 / 652.800 = 10 exactly in real arithmetic; accounting for floating point representation)
- Validate L2float2(65535) -> 65535 / 652.800 (max 16-bit input)
- Validate L2float2(1) -> 1 / 652.800

Notes on test style
- Uses a small, self-contained test harness (no GTest, no Google Mock) compatible with C++11.
- Non-terminating assertions: tests accumulate failures and print per-test results rather than aborting on first failure.
- Uses the library's types via cmsFloat64Number and cmsUInt16Number as defined in lcms2_internal.h.

Now, the test code:

*/

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Step 2: Depend on the same internal header as the FOCAL_CLASS_DEP to ensure type compatibility.
// This header provides cmsFloat64Number and cmsUInt16Number used by L2float2.

// Step 2: Declare the focal function with C linkage to enable linking with the C implementation.
extern "C" cmsFloat64Number L2float2(cmsUInt16Number v);

// Simple non-terminating assertion helper to collect results
static int g_tests = 0;
static int g_failures = 0;

// Check that actual is within tolerable distance of expected.
// testName is used to identify the failing test in output.
static void checkNear(const cmsFloat64Number& expected,
                      const cmsFloat64Number& actual,
                      const cmsFloat64Number& tol,
                      const char* testName)
{
    ++g_tests;
    if (std::fabs(static_cast<double>(expected) - static_cast<double>(actual)) > static_cast<double>(tol)) {
        ++g_failures;
        std::cerr << "[FAIL] " << testName
                  << ": expected " << static_cast<double>(expected)
                  << ", got " << static_cast<double>(actual)
                  << ", tol " << static_cast<double>(tol) << "\n";
    } else {
        std::cout << "[PASS] " << testName << "\n";
    }
}

// Test 1: L2float2(0) should be 0.0
// Rationale: True branch-less validation of zero input; ensures no unintended behavior.
static void test_L2float2_zero()
{
    // Arrange
    cmsUInt16Number v = 0;
    // Act
    cmsFloat64Number actual = L2float2(v);
    // Expected value computed using the same division semantics
    cmsFloat64Number expected = (cmsFloat64Number)0 / (cmsFloat64Number)652.800;
    // Assert
    checkNear(expected, actual, (cmsFloat64Number)1e-12, "L2float2_zero");
}

// Test 2: L2float2 with a small positive value (652) to verify standard division
// Rationale: Ensures correct division and floating-point handling for typical input.
static void test_L2float2_boundary652()
{
    cmsUInt16Number v = 652;
    cmsFloat64Number actual = L2float2(v);
    cmsFloat64Number expected = (cmsFloat64Number)652 / (cmsFloat64Number)652.800;
    checkNear(expected, actual, (cmsFloat64Number)1e-12, "L2float2_boundary652");
}

// Test 3: L2float2 with a value that yields a round number (6528 -> 10.0)
// Rationale: Verifies accuracy when divisor leads to an exact integer in real arithmetic.
static void test_L2float2_round10()
{
    cmsUInt16Number v = 6528; // 652.800 * 10 = 6528
    cmsFloat64Number actual = L2float2(v);
    cmsFloat64Number expected = (cmsFloat64Number)6528 / (cmsFloat64Number)652.800;
    checkNear(expected, actual, (cmsFloat64Number)1e-12, "L2float2_round10");
}

// Test 4: L2float2 with maximum 16-bit input
// Rationale: Exercises scaling to larger outputs to ensure stable behavior across range.
static void test_L2float2_max()
{
    cmsUInt16Number v = 65535;
    cmsFloat64Number actual = L2float2(v);
    cmsFloat64Number expected = (cmsFloat64Number)65535 / (cmsFloat64Number)652.800;
    // Use a slightly looser tolerance due to potential accumulated FP rounding at scale
    checkNear(expected, actual, (cmsFloat64Number)1e-9, "L2float2_max");
}

// Test 5: L2float2 with a very small non-zero value
// Rationale: Ensures small inputs do not underflow and are represented correctly.
static void test_L2float2_one()
{
    cmsUInt16Number v = 1;
    cmsFloat64Number actual = L2float2(v);
    cmsFloat64Number expected = (cmsFloat64Number)1 / (cmsFloat64Number)652.800;
    checkNear(expected, actual, (cmsFloat64Number)1e-12, "L2float2_one");
}

// Step 3: Test runner and summary
int main()
{
    // Run tests
    test_L2float2_zero();
    test_L2float2_boundary652();
    test_L2float2_round10();
    test_L2float2_max();
    test_L2float2_one();

    // Summary
    std::cout << "Tests run: " << g_tests << ", Failures: " << g_failures << "\n";

    // Return non-zero if any test failed, to integrate with simple CI scripts
    return (g_failures == 0) ? 0 : 1;
}