// C++11 test suite for the focal method ilinear located in pngstest.c
// This test suite is designed to be compiled together with the project
// sources (pngstest.c and friends) and linked into a single executable.
// It uses a lightweight, self-contained assertion framework (no GTest).

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <iomanip>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <ctype.h>


// Forward declarations of focal function and its dependent data from the C sources.
// The actual project provides these symbols with C linkage.
// We declare them with extern "C" to ensure proper linkage from C++.
extern "C" int ilinear(int fixed_srgb);

// Dependent data/functions assumed to exist in the C sources.
// sRGB_to_d is expected to be a 256-element array (mapping sRGB index to double in [0,1]).
extern "C" const double sRGB_to_d[256];

// u16d converts a double to a 16-bit unsigned integer representation.
// We assume it returns a 32-bit integer type for safety in C/C++ interop.
extern "C" unsigned int u16d(double d);

// Lightweight test harness (no external testing framework)
static int tests_run = 0;
static int tests_passed = 0;

#define TEST_PASSED(msg) do { ++tests_run; ++tests_passed; std::cout << "[PASS] " << msg << "\n"; } while (0)
#define TEST_FAILED(msg) do { ++tests_run; std::cout << "[FAIL] " << msg << "\n"; } while (0)

// Helper macro to compare two values and report result
#define ASSERT_EQ(actual, expected, test_desc) \
    do { \
        ++tests_run; \
        if ((actual) == (expected)) { \
            ++tests_passed; \
            std::cout << "[PASS] " << test_desc << " => " \
                      << "actual=" << (actual) << ", expected=" << (expected) << "\n"; \
        } else { \
            std::cout << "[FAIL] " << test_desc << " => " \
                      << "actual=" << (actual) << ", expected=" << (expected) << "\n"; \
        } \
    } while (0)


// Test 1: ilinear(i) matches the composition u16d(65535 * sRGB_to_d[i])
// This validates the core semantics of ilinear against its exact mathematical
// expression using the project-provided dependent functions.
static void test_ilinear_matches_expected_at_indices() {
    std::cout << "Running test_ilinear_matches_expected_at_indices...\n";

    // We'll test a representative subset of indices including boundary cases.
    const int test_indices[] = {0, 1, 2, 5, 10, 31, 64, 100, 128, 200, 255};
    const int n = sizeof(test_indices) / sizeof(test_indices[0]);

    for (int k = 0; k < n; ++k) {
        int idx = test_indices[k];
        int actual = ilinear(idx);

        // Compute expected using the same fundamental operations the focal method uses.
        // If the underlying symbols are correctly implemented in the C sources,
        // this should provide a robust cross-check.
        double scaled = 65535.0 * sRGB_to_d[idx];
        unsigned int expected = u16d(scaled);

        ASSERT_EQ(actual, (int)expected,
                  std::string("ilinear(") + std::to_string(idx) + ") equals u16d(65535 * sRGB_to_d[idx])");
    }

    TEST_PASSED("test_ilinear_matches_expected_at_indices completed");
}

// Test 2: ilinear values are monotonic with respect to fixed_srgb index
// We expect that if i < j, then ilinear(i) <= ilinear(j) given a non-decreasing sRGB_to_d.
// This checks a true/false predicate coverage of the core branch behavior.
static void test_ilinear_monotonic_increasing() {
    std::cout << "Running test_ilinear_monotonic_increasing...\n";

    int prev = ilinear(0);
    bool all_monotonic = true;
    for (int i = 1; i <= 255; ++i) {
        int cur = ilinear(i);
        if (cur < prev) {
            all_monotonic = false;
            std::cout << "[FAIL] ilinear(" << i << ") = " << cur
                      << " is less than ilinear(" << i-1 << ") = " << prev << "\n";
            break;
        }
        prev = cur;
    }

    if (all_monotonic) {
        TEST_PASSED("test_ilinear_monotonic_increasing: ilinear is non-decreasing across 0..255");
    } else {
        TEST_FAILED("test_ilinear_monotonic_increasing: monotonicity violated");
    }
}

// Test 3: ilinear outputs are within 16-bit unsigned range
// Ensures that the function does not produce values outside [0, 65535].
static void test_ilinear_within_16bit_range() {
    std::cout << "Running test_ilinear_within_16bit_range...\n";

    bool in_range = true;
    int min_val = 65536;
    int max_val = -1;
    for (int i = 0; i <= 255; ++i) {
        int v = ilinear(i);
        if (v < 0 || v > 65535) {
            in_range = false;
            std::cout << "[FAIL] ilinear(" << i << ") = " << v
                      << " is out of 0..65535 range\n";
            break;
        }
        if (v < min_val) min_val = v;
        if (v > max_val) max_val = v;
    }

    if (in_range) {
        TEST_PASSED("test_ilinear_within_16bit_range: all ilinear(i) within 0..65535");
    } else {
        TEST_FAILED("test_ilinear_within_16bit_range: found out-of-range value");
    }

    // Optional additional diagnostic: print observed min/max for debugging
    if (in_range) {
        std::cout << "Observed ilinear value range: min=" << min_val << ", max=" << max_val << "\n";
    }
}

// Entry point to run all tests
int main() {
    // Note: We rely on the presence of the symbols ilinear, sRGB_to_d, u16d
    // from the C sources included in the build. The tests are designed to be
    // run as part of the same executable that links against the C sources.

    test_ilinear_matches_expected_at_indices();
    test_ilinear_monotonic_increasing();
    test_ilinear_within_16bit_range();

    // Summary
    std::cout << "\nTest suite finished. "
              << "Total: " << tests_run
              << " tests, Passed: " << tests_passed
              << ", Failed: " << (tests_run - tests_passed)
              << std::endl;

    // Return non-zero if any test failed
    return (tests_run == tests_passed) ? 0 : 1;
}