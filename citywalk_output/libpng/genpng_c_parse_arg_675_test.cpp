// Test suite for the focal method parse_arg in genpng.c
// This test is written for C++11 (no GTest). It uses direct function calls
// to the C code and inspects the resulting struct arg fields.
// The tests are designed to be non-terminating (they run multiple test cases
// and report results rather than aborting on first failure).

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <iomanip>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>


// Step 1: Declare the C interfaces and the exact struct layout used by parse_arg.
// We mirror the struct arg layout as per the provided <FOCAL_CLASS_DEP> snippet.
// This ensures we can access the same fields after parse_arg is invoked.
extern "C" {
    struct arg {
        int color;
        int width;
        void *inside_fn;
        void *check_fn;
        double x1;
        double y1;
        double x2;
        double y2;
    };

    // Dependency functions used by parse_arg. We rely on their public signatures.
    int color_of(const char *arg);
    int width_of(const char *arg);
    double coordinate_of(const char *arg);
    void *shape_of(const char *arg, double width, int f);

    // The function under test.
    void parse_arg(struct arg *arg, const char **argv);
}

// Helper: compare doubles with tolerance to handle floating-point inaccuracies.
static bool almost_equal(double a, double b, double eps = 1e-9) {
    return std::fabs(a - b) <= eps;
}

// Test 1: Basic parsing sanity
// - Provide typical 7-argument argv (color, width, shape, x1, y1, x2, y2).
// - Verify arg fields match the results from color_of/width_of/coordinate_of.
// - Ensure inside_fn and check_fn are non-null after parsing.
static bool test_parse_arg_basic() {
    // Arrange
    const char *argv[] = { "red", "2", "line", "1.0", "2.0", "3.0", "4.0" };

    // Act
    struct arg parsed;
    parse_arg(&parsed, argv);

    // Compute expected values using the same public helpers (to validate passthrough behavior)
    int expected_color = color_of(argv[0]);
    int expected_width = width_of(argv[1]);
    double expected_x1 = coordinate_of(argv[3]);
    double expected_y1 = coordinate_of(argv[4]);
    double expected_x2 = coordinate_of(argv[5]);
    double expected_y2 = coordinate_of(argv[6]);

    // Assert
    bool ok = true;

    if (parsed.color != expected_color) {
        std::cerr << "[test_parse_arg_basic] color mismatch: got " << parsed.color
                  << ", expected " << expected_color << "\n";
        ok = false;
    }
    if (parsed.width != expected_width) {
        std::cerr << "[test_parse_arg_basic] width mismatch: got " << parsed.width
                  << ", expected " << expected_width << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.x1, expected_x1)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_basic] x1 mismatch: got " << parsed.x1
                  << ", expected " << expected_x1 << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.y1, expected_y1)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_basic] y1 mismatch: got " << parsed.y1
                  << ", expected " << expected_y1 << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.x2, expected_x2)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_basic] x2 mismatch: got " << parsed.x2
                  << ", expected " << expected_x2 << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.y2, expected_y2)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_basic] y2 mismatch: got " << parsed.y2
                  << ", expected " << expected_y2 << "\n";
        ok = false;
    }

    if (parsed.inside_fn == nullptr) {
        std::cerr << "[test_parse_arg_basic] inside_fn pointer is NULL\n";
        ok = false;
    }
    if (parsed.check_fn == nullptr) {
        std::cerr << "[test_parse_arg_basic] check_fn pointer is NULL\n";
        ok = false;
    }

    return ok;
}

// Test 2: Negative and non-trivial numeric values
// - Validate that parse_arg correctly handles negative/decimal inputs.
// - Again verify color/width and coordinates via the helper functions.
static bool test_parse_arg_negative_values() {
    // Arrange
    const char *argv[] = { "blue", "-7", "circle", "-1.5", "-2.25", "-3.75", "-4.125" };

    // Act
    struct arg parsed;
    parse_arg(&parsed, argv);

    // Expected values derived from helpers
    int expected_color = color_of(argv[0]);
    int expected_width = width_of(argv[1]);
    double expected_x1 = coordinate_of(argv[3]);
    double expected_y1 = coordinate_of(argv[4]);
    double expected_x2 = coordinate_of(argv[5]);
    double expected_y2 = coordinate_of(argv[6]);

    // Assert
    bool ok = true;

    if (parsed.color != expected_color) {
        std::cerr << "[test_parse_arg_negative_values] color mismatch: got " << parsed.color
                  << ", expected " << expected_color << "\n";
        ok = false;
    }
    if (parsed.width != expected_width) {
        std::cerr << "[test_parse_arg_negative_values] width mismatch: got " << parsed.width
                  << ", expected " << expected_width << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.x1, expected_x1)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_negative_values] x1 mismatch: got " << parsed.x1
                  << ", expected " << expected_x1 << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.y1, expected_y1)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_negative_values] y1 mismatch: got " << parsed.y1
                  << ", expected " << expected_y1 << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.x2, expected_x2)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_negative_values] x2 mismatch: got " << parsed.x2
                  << ", expected " << expected_x2 << "\n";
        ok = false;
    }
    if (!almost_equal(parsed.y2, expected_y2)) {
        std::cerr << std::fixed << std::setprecision(12)
                  << "[test_parse_arg_negative_values] y2 mismatch: got " << parsed.y2
                  << ", expected " << expected_y2 << "\n";
        ok = false;
    }

    if (parsed.inside_fn == nullptr || parsed.check_fn == nullptr) {
        std::cerr << "[test_parse_arg_negative_values] function pointers are NULL\n";
        ok = false;
    }

    return ok;
}

// Entry point: run all tests and report results.
// This follows the instruction to "call test methods from the main function" and
// not rely on a predefined test framework.
int main() {
    int failures = 0;

    std::cout << "Running test_parse_arg_basic...\n";
    bool r1 = test_parse_arg_basic();
    if (r1) {
        std::cout << "test_parse_arg_basic: PASSED\n";
    } else {
        std::cout << "test_parse_arg_basic: FAILED\n";
        ++failures;
    }

    std::cout << "Running test_parse_arg_negative_values...\n";
    bool r2 = test_parse_arg_negative_values();
    if (r2) {
        std::cout << "test_parse_arg_negative_values: PASSED\n";
    } else {
        std::cout << "test_parse_arg_negative_values: FAILED\n";
        ++failures;
    }

    if (failures > 0) {
        std::cout << failures << " test(s) failed.\n";
        return 1;
    } else {
        std::cout << "All tests passed.\n";
        return 0;
    }
}