/*
Unit Test Suite for the focal method: distance
Context:
- The function distance computes the squared Euclidean distance between two RGB points in 3D space.
- Signature assumed from the provided code:
  cmsFloat32Number distance(cmsFloat32Number rgb1[], cmsFloat32Number rgb2[])
  where cmsFloat32Number is a 32-bit floating point type (float).

Step 1: Candidate Keywords (extracted from the focal method and its usage)
- cmsFloat32Number, rgb1, rgb2
- dr, dg, db (differences in red, green, blue channels)
- squared terms: dr*dr, dg*dg, db*db
- return value: dr*dr + dg*dg + db*db
- 3-element array semantics, RGB color space

Step 2: Dependencies
- The focal class dependencies (provided as <FOCAL_CLASS_DEP>) include a number of other functions and helpers; however, the distance function itself is a pure computation on 3-element arrays. No private members or dynamic state are accessed here. The test focuses on the public distance function only.

Step 3: Domain knowledge considerations
- Use C++11, no GTest. Implement a lightweight test harness with non-terminating assertions.
- Access the C-style function distance via extern "C" linkage to ensure correct symbol resolution when compiled with a C source containing the function.
- Cover typical, boundary, and edge-case scenarios: identical inputs (distance 0), known values (verify exact result), negative values, large magnitude values, and NaN behavior.
- Do not rely on private members; test only the public function signature.
- Provide explanatory comments for each test that describe intent and expected outcomes.

Below is the standalone test code (C++11) that can be compiled and linked with the C source providing distance.

*/

#include <fast_float_internal.h>
#include <limits>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Assuming cmsFloat32Number is a typedef for float in the original project.
// Provide a compatible declaration for the test harness.
typedef float cmsFloat32Number;

// Declare the focal function with C linkage to match the original implementation.
// The test harness compiles in C++, so use extern "C" to prevent name mangling.
extern "C" cmsFloat32Number distance(cmsFloat32Number rgb1[], cmsFloat32Number rgb2[]);

// Simple non-terminating assertion helpers
static int g_tests_passed = 0;
static int g_tests_failed = 0;

static void assertion_pass(const char* test_name) {
    std::cout << "[PASSED] " << test_name << std::endl;
    ++g_tests_passed;
}
static void assertion_fail(const char* test_name, const cmsFloat32Number expected, const cmsFloat32Number actual) {
    std::cout << "[FAILED] " << test_name
              << " - expected: " << expected
              << ", actual: " << actual << std::endl;
    ++g_tests_failed;
}

// Helper: compare floats with a tolerance
static bool almost_equal_float(float a, float b, float tol = 1e-5f) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Identical inputs should yield distance 0
void test_distance_identical_inputs() {
    const cmsFloat32Number rgb1[3] = {0.0f, 0.0f, 0.0f};
    const cmsFloat32Number rgb2[3] = {0.0f, 0.0f, 0.0f};

    cmsFloat32Number result = distance(const_cast<cmsFloat32Number*>(rgb1),
                                       const_cast<cmsFloat32Number*>(rgb2));

    const cmsFloat32Number expected = 0.0f;
    if (almost_equal_float(result, expected)) {
        assertion_pass("test_distance_identical_inputs");
    } else {
        assertion_fail("test_distance_identical_inputs", expected, result);
    }
}

// Test 2: Known values (0,0,0) vs (1,2,3) => 1^2 + 2^2 + 3^2 = 14
void test_distance_known_values() {
    const cmsFloat32Number rgb1[3] = {0.0f, 0.0f, 0.0f};
    const cmsFloat32Number rgb2[3] = {1.0f, 2.0f, 3.0f};

    cmsFloat32Number result = distance(const_cast<cmsFloat32Number*>(rgb1),
                                       const_cast<cmsFloat32Number*>(rgb2));

    const cmsFloat32Number expected = 14.0f; // 1^2 + 2^2 + 3^2
    if (almost_equal_float(result, expected)) {
        assertion_pass("test_distance_known_values");
    } else {
        assertion_fail("test_distance_known_values", expected, result);
    }
}

// Test 3: Negative values and mixed signs
// rgb1 = (-1,-2,-3), rgb2 = (1,0,2) => dr=2, dg=2, db=5 -> 4+4+25 = 33
void test_distance_negative_and_positive_values() {
    const cmsFloat32Number rgb1[3] = {-1.0f, -2.0f, -3.0f};
    const cmsFloat32Number rgb2[3] = {1.0f, 0.0f, 2.0f};

    cmsFloat32Number result = distance(const_cast<cmsFloat32Number*>(rgb1),
                                       const_cast<cmsFloat32Number*>(rgb2));

    const cmsFloat32Number expected = 33.0f;
    if (almost_equal_float(result, expected)) {
        assertion_pass("test_distance_negative_and_positive_values");
    } else {
        assertion_fail("test_distance_negative_and_positive_values", expected, result);
    }
}

// Test 4: Large magnitude values to verify numerical stability
// rgb1 = (1000, -2000, 3000), rgb2 = (-4000, 5000, -6000)
// dr = -5000, dg = 7000, db = -9000 -> 25e6 + 49e6 + 81e6 = 155e6
void test_distance_large_values() {
    const cmsFloat32Number rgb1[3] = {1000.0f, -2000.0f, 3000.0f};
    const cmsFloat32Number rgb2[3] = {-4000.0f, 5000.0f, -6000.0f};

    cmsFloat32Number result = distance(const_cast<cmsFloat32Number*>(rgb1),
                                       const_cast<cmsFloat32Number*>(rgb2));

    const cmsFloat32Number expected = 155000000.0f; // 155e6
    if (almost_equal_float(result, expected)) {
        assertion_pass("test_distance_large_values");
    } else {
        assertion_fail("test_distance_large_values", expected, result);
    }
}

// Test 5: NaN propagation
// If any component is NaN, the result should be NaN per IEEE semantics for arithmetic
void test_distance_with_nan_input() {
    const cmsFloat32Number rgb1[3] = {std::numeric_limits<float>::quiet_NaN(), 0.0f, 0.0f};
    const cmsFloat32Number rgb2[3] = {0.0f, 0.0f, 0.0f};

    cmsFloat32Number result = distance(const_cast<cmsFloat32Number*>(rgb1),
                                       const_cast<cmsFloat32Number*>(rgb2));

    if (std::isnan(result)) {
        assertion_pass("test_distance_with_nan_input");
    } else {
        assertion_fail("test_distance_with_nan_input", NAN, result);
    }
}

// Optional: demonstrate non-exceptional termination behavior for robustness
void test_distance_multiple_cases() {
    // Reuse previous tests to ensure the function remains stable in sequential calls
    test_distance_identical_inputs();
    test_distance_known_values();
    test_distance_negative_and_positive_values();
    test_distance_large_values();
    test_distance_with_nan_input();
}

// Main test runner
int main() {
    // Run all defined tests
    test_distance_identical_inputs();
    test_distance_known_values();
    test_distance_negative_and_positive_values();
    test_distance_large_values();
    test_distance_with_nan_input();

    // Optional: run a consolidated test suite
    // test_distance_multiple_cases();

    // Summary
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "Distance function test summary: "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed." << std::endl;
    std::cout << "-----------------------------------------" << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}