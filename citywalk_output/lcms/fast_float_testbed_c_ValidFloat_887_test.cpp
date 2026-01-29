// Lightweight C++11 test suite for the focal method ValidFloat
// This test harness does not rely on any external testing framework (e.g., GTest).
// It uses a small in-house assertion mechanism that prints failures but continues
// executing remaining tests to maximize coverage.
//
// Assumptions based on the provided focal code:
// - ValidFloat takes cmsFloat32Number a and cmsFloat32Number b and returns cmsBool.
// - The comparison uses EPSILON_FLOAT_TESTS as the tolerance via fabsf(a-b) < EPSILON_FLOAT_TESTS.
// - The necessary types and EPSILON_FLOAT_TESTS are defined in fast_float_internal.h.
//
// Note: This file should be compiled and linked with the library/object that provides
// the actual implementation of ValidFloat (i.e., the code in fast_float_testbed.c).

#include <memory.h>
#include <stdlib.h>
#include <fast_float_internal.h>
#include <iostream>


// Include the project's internal header to obtain typedefs and EPSILON_FLOAT_TESTS.
// If the real include path differs, adjust accordingly.

extern "C" {
    // Declaration of the focal function from the C codebase.
    // Use C linkage to avoid name mangling when called from C++.
    cmsBool ValidFloat(cmsFloat32Number a, cmsFloat32Number b);
}

// Minimal test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Lightweight EXPECT function: reports failures but does not abort execution.
void expect(bool condition, const char* description) {
    ++g_tests_run;
    if (condition) {
        ++g_tests_passed;
    } else {
        std::cerr << "Test FAILED: " << description << std::endl;
    }
}

int main() {
    // Acquire EPS value from the header for boundary checks.
    // EPSILON_FLOAT_TESTS is defined in the included header as the tolerance.
    const float eps = static_cast<float>(EPSILON_FLOAT_TESTS);

    // Test 1: True case - identical inputs should yield true (difference 0).
    // Rationale: fabsf(0) < EPSILON_FLOAT_TESTS -> true
    {
        cmsFloat32Number a = 0.0f;
        cmsFloat32Number b = 0.0f;
        cmsBool res = ValidFloat(a, b);
        expect(static_cast<bool>(res) == true, "ValidFloat should return true for identical zeros");
    }

    // Test 2: True case - difference well below EPSILON_FLOAT_TESTS
    // Rationale: |a - (a + 0.5*eps)| = 0.5*eps < eps
    {
        cmsFloat32Number a = 1.0f;
        cmsFloat32Number b = a + (0.5f * eps);
        cmsBool res = ValidFloat(a, b);
        expect(static_cast<bool>(res) == true, "ValidFloat should return true for difference below EPS");
    }

    // Test 3: False boundary case - exact EPS difference should be false
    // Rationale: |a - (a + eps)| == eps -> not strictly less than eps
    {
        cmsFloat32Number a = 0.0f;
        cmsFloat32Number b = (cmsFloat32Number)EPSILON_FLOAT_TESTS; // b - a == EPSILON_FLOAT_TESTS
        cmsBool res = ValidFloat(a, b);
        expect(static_cast<bool>(res) == false, "ValidFloat should return false for exact EPS difference");
    }

    // Test 4: False case - large difference beyond EPS
    // Rationale: Difference of 2.0 should clearly be outside the tolerance
    {
        cmsFloat32Number a = 0.0f;
        cmsFloat32Number b = 2.0f;
        cmsBool res = ValidFloat(a, b);
        expect(static_cast<bool>(res) == false, "ValidFloat should return false for large difference");
    }

    // Test 5: True case with negative numbers - small difference around a negative value
    // Rationale: Difference is small and within EPS, irrespective of sign
    {
        cmsFloat32Number a = -1000.0f;
        cmsFloat32Number b = a + (0.5f * eps);
        cmsBool res = ValidFloat(a, b);
        expect(static_cast<bool>(res) == true, "ValidFloat should return true for small negative difference");
    }

    // Test 6: True case - small difference around a non-zero value
    {
        cmsFloat32Number a = 123.456f;
        cmsFloat32Number b = a + (0.1f * eps); // definitely within EPS
        cmsBool res = ValidFloat(a, b);
        expect(static_cast<bool>(res) == true, "ValidFloat should return true for tiny difference within EPS");
    }

    // Test 7: False case - large magnitude difference with opposite signs
    // Rationale: Very large difference should be false
    {
        cmsFloat32Number a = 1e6f;
        cmsFloat32Number b = -1e6f;
        cmsBool res = ValidFloat(a, b);
        expect(static_cast<bool>(res) == false, "ValidFloat should return false for very large opposite-sign difference");
    }

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed)
              << std::endl;

    // Return non-zero if there were any failures to aid integration with simple build scripts.
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}