// Minimal C++11 unit test suite for the focal method XYZ2Fix
// This test does not depend on any external testing framework (e.g., GTest).
// It uses a lightweight, non-terminating assertion mechanism to maximize coverage.

#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge alignment:
//  - The focal method under test:
//      cmsUInt16Number XYZ2Fix(cmsFloat64Number d)
//      returns _cmsQuickSaturateWord(d * 32768.0);
//  - Our tests exercise the saturation/clamping behavior for a variety of inputs.
//  - We rely on the exact signature (types) used by the focal code.
//  - The test is designed to be compilable with C++11 and link with the C implementation.

typedef unsigned short cmsUInt16Number; // Mirror of the project's typedef
typedef double cmsFloat64Number;        // Mirror of the project's typedef

// Function under test: declare with C linkage to avoid name mangling when linking with C file.
extern "C" cmsUInt16Number XYZ2Fix(cmsFloat64Number d);

// Lightweight non-terminating assertion helper
static int g_failures = 0;

static void logFailure(const std::string& testName,
                       cmsUInt16Number actual,
                       cmsUInt16Number expected,
                       const std::string& detail)
{
    std::cerr << "[FAIL] " << testName << ": " << detail
              << " | actual=" << actual << ", expected=" << expected << "\n";
    ++g_failures;
}

// Expected value computation based on the saturating behavior of _cmsQuickSaturateWord.
// This helper mirrors the (likely) internal implementation: clamp to [0, 65535]
// after converting the product to an unsigned 16-bit value (truncation toward zero).
static cmsUInt16Number expectedXYZ2Fix(double d)
{
    double v = d * 32768.0;
    if (v <= 0.0) return 0;
    if (v >= 65535.0) return 65535;
    return static_cast<cmsUInt16Number>(v); // truncation toward zero
}

// Test 1: Basic non-negative inputs that map to distinct, exact saturations.
// Rationale: Verify that standard small positive values produce expected, non-error results.
static void test_XYZ2Fix_BasicNonNegative()
{
    // True representative values that yield exact integer products with 32768
    // to avoid ambiguity introduced by floating-point rounding.
    std::vector<double> inputs = {0.0, 0.25, 0.5, 0.75, 1.0, 2.0, 0.0}; // repeats allowed
    // Descriptive test name
    const std::string testName = "test_XYZ2Fix_BasicNonNegative";

    for (size_t i = 0; i < inputs.size(); ++i)
    {
        double d = inputs[i];
        cmsUInt16Number actual = XYZ2Fix(d);
        cmsUInt16Number expected = expectedXYZ2Fix(d);
        if (actual != expected) {
            logFailure(testName, actual, expected,
                       "XYZ2Fix(" + std::to_string(d) + ") mismatch");
        } // else, test passes for this input
    }
}

// Test 2: Fractional inputs that produce exact integer product after multiplication.
// Rationale: Ensure handling for fractional d that maps to known fixed values.
static void test_XYZ2Fix_FractalsExactMultiples()
{
    const std::string testName = "test_XYZ2Fix_FractalsExactMultiples";
    std::vector<double> inputs = {0.25, 0.5, 0.75, 1.0, 0.125, 1.5, 1.9993828125}; // subset including exact multiples
    for (double d : inputs)
    {
        cmsUInt16Number actual = XYZ2Fix(d);
        cmsUInt16Number expected = expectedXYZ2Fix(d);
        if (actual != expected) {
            logFailure(testName, actual, expected,
                       "XYZ2Fix(" + std::to_string(d) + ") mismatch");
        }
    }
}

// Test 3: Negative inputs should saturate to 0.
// Rationale: Validate lower-bound clamping behavior.
static void test_XYZ2Fix_NegativeInputs_SaturateToZero()
{
    const std::string testName = "test_XYZ2Fix_NegativeInputs_SaturateToZero";
    std::vector<double> inputs = {-0.0001, -0.5, -1.0, -2.5};
    for (double d : inputs)
    {
        cmsUInt16Number actual = XYZ2Fix(d);
        cmsUInt16Number expected = expectedXYZ2Fix(d);
        if (actual != expected) {
            logFailure(testName, actual, expected,
                       "XYZ2Fix(" + std::to_string(d) + ") mismatch");
        }
    }
}

// Test 4: Large positive inputs saturate to the maximum allowed 65535.
// Rationale: Ensure proper upper-bound clamping when the product exceeds 65535.
static void test_XYZ2Fix_LargePositiveInputs_SaturateToMax()
{
    const std::string testName = "test_XYZ2Fix_LargePositiveInputs_SaturateToMax";
    std::vector<double> inputs = {2.0, 3.0, 10.0};
    for (double d : inputs)
    {
        cmsUInt16Number actual = XYZ2Fix(d);
        cmsUInt16Number expected = expectedXYZ2Fix(d);
        if (actual != expected) {
            logFailure(testName, actual, expected,
                       "XYZ2Fix(" + std::to_string(d) + ") mismatch");
        }
    }
}

// Test 5: Edge cases around zero to ensure stable behavior near zero.
// Rationale: Confirm that very small positives map correctly (to non-zero if > 0).
static void test_XYZ2Fix_ZeroEdgeCases()
{
    const std::string testName = "test_XYZ2Fix_ZeroEdgeCases";
    std::vector<double> inputs = {0.0, 0.0000001, -0.0000001};
    for (double d : inputs)
    {
        cmsUInt16Number actual = XYZ2Fix(d);
        cmsUInt16Number expected = expectedXYZ2Fix(d);
        if (actual != expected) {
            logFailure(testName, actual, expected,
                       "XYZ2Fix(" + std::to_string(d) + ") mismatch");
        }
    }
}

// Main test runner
int main()
{
    // Run all test scenarios
    test_XYZ2Fix_BasicNonNegative();
    test_XYZ2Fix_FractalsExactMultiples();
    test_XYZ2Fix_NegativeInputs_SaturateToZero();
    test_XYZ2Fix_LargePositiveInputs_SaturateToMax();
    test_XYZ2Fix_ZeroEdgeCases();

    // Summary
    if (g_failures == 0) {
        std::cout << "[PASS] All XYZ2Fix tests passed.\n";
        return 0;
    } else {
        std::cerr << "[SUMMARY] XYZ2Fix tests finished with " << g_failures << " failure(s).\n";
        return g_failures;
    }
}