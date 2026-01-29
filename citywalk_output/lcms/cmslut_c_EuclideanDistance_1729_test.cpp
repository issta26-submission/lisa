// Unit test suite for EuclideanDistance function from cmslut.c
// This test suite is written in plain C++11 (no GoogleTest) and uses a lightweight
// non-terminating assertion approach to maximize code coverage.

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Forward declaration of the focal C function.
// The real code uses cmsFloat32Number (typically float). We provide a compatible alias.
typedef float cmsFloat32Number;
extern "C" cmsFloat32Number EuclideanDistance(cmsFloat32Number a[], cmsFloat32Number b[], int n);

// Simple non-terminating test harness
static int g_total = 0;
static int g_passed = 0;
static std::vector<std::string> g_failures;

// Record a test failure without aborting execution
static void record_failure(const std::string& testName, const std::string& message) {
    g_failures.push_back("Test [" + testName + "] FAILED: " + message);
}

// Convenience: compare two floating-point values with tolerance
static bool almost_equal(float a, float b, float tol = 1e-6f) {
    return std::fabs(a - b) <= tol;
}

/*
Test 1: Distance between identical arrays should be zero.
This covers the true branch when all corresponding elements are equal.
*/
static void test_distance_zero_same_arrays() {
    // GIVEN two identical 3-element arrays
    const int n = 3;
    cmsFloat32Number a[n] = {1.0f, 2.0f, 3.0f};
    cmsFloat32Number b[n] = {1.0f, 2.0f, 3.0f};

    // WHEN computing EuclideanDistance
    cmsFloat32Number d = EuclideanDistance(a, b, n);

    // THEN the distance should be exactly 0 (within tolerance)
    if (!almost_equal(d, 0.0f)) {
        record_failure("test_distance_zero_same_arrays", "Expected 0, got " + std::to_string(d));
    } else {
        ++g_passed;
    }
    ++g_total;
}

/*
Test 2: Classic 3-4-5 case in 2D space.
This validates the standard Pythagorean result where distance is 5.
*/
static void test_distance_known_3_4_5() {
    const int n = 2;
    cmsFloat32Number a[n] = {0.0f, 0.0f};
    cmsFloat32Number b[n] = {3.0f, 4.0f};

    cmsFloat32Number d = EuclideanDistance(a, b, n);
    if (!almost_equal(d, 5.0f)) {
        record_failure("test_distance_known_3_4_5", "Expected 5, got " + std::to_string(d));
    } else {
        ++g_passed;
    }
    ++g_total;
}

/*
Test 3: Distance with negative coordinates.
This checks correctness when diffs are negative and positive.
*/
static void test_distance_negative_values() {
    const int n = 2;
    cmsFloat32Number a[n] = {-1.0f, -2.0f};
    cmsFloat32Number b[n] = {2.0f, 2.0f};

    cmsFloat32Number d = EuclideanDistance(a, b, n);
    const cmsFloat32Number expected = 5.0f; // diffs: 3 and 4
    if (!almost_equal(d, expected)) {
        record_failure("test_distance_negative_values", "Expected 5, got " + std::to_string(d));
    } else {
        ++g_passed;
    }
    ++g_total;
}

/*
Test 4: Edge case with zero length (n == 0).
This ensures the function handles empty inputs without error.
*/
static void test_distance_zero_length() {
    const int n = 0;
    cmsFloat32Number a[0] = {};
    cmsFloat32Number b[0] = {};

    cmsFloat32Number d = EuclideanDistance(a, b, n);
    if (!almost_equal(d, 0.0f)) {
        record_failure("test_distance_zero_length", "Expected 0, got " + std::to_string(d));
    } else {
        ++g_passed;
    }
    ++g_total;
}

/*
Test 5: Four-dimensional case to exercise multiple components.
Distance from origin to (1,2,2,2) -> sqrt(1+4+4+4) = sqrt(13)
*/
static void test_distance_four_dimensions() {
    const int n = 4;
    cmsFloat32Number a[n] = {0.0f, 0.0f, 0.0f, 0.0f};
    cmsFloat32Number b[n] = {1.0f, 2.0f, 2.0f, 2.0f};

    cmsFloat32Number d = EuclideanDistance(a, b, n);
    const cmsFloat32Number expected = std::sqrt(13.0f);
    if (!almost_equal(d, expected)) {
        record_failure("test_distance_four_dimensions",
                       "Expected " + std::to_string(expected) + ", got " + std::to_string(d));
    } else {
        ++g_passed;
    }
    ++g_total;
}

int main() {
    // Run all tests
    test_distance_zero_same_arrays();
    test_distance_known_3_4_5();
    test_distance_negative_values();
    test_distance_zero_length();
    test_distance_four_dimensions();

    // Output results
    std::cout << "EuclideanDistance test results: " << g_passed << " / " << g_total << " tests passed." << std::endl;

    if (!g_failures.empty()) {
        std::cerr << "Failures:\n";
        for (const auto& msg : g_failures) std::cerr << msg << std::endl;
        return 1;
    }

    std::cout << "All tests passed successfully." << std::endl;
    return 0;
}