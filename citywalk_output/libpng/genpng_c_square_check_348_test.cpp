// This test suite targets the focal method: square_check(double x, double y, double x1, double y1, double x2, double y2)
// It follows the instructions to produce a non-GTest unit test suite in C++11 that can be compiled alongside genpng.c.
// The implementation relies on a small, self-contained test harness with non-terminating assertions to maximize coverage.
//
// Domain and behavioral notes (mapped to Candidate Keywords):
// - Inputs: (x, y) - the point to test; (x1, y1), (x2, y2) - define the square/rectangle region. The region is effectively [x1, x2) x [y1, y2) in the
//   sense of the inner predicate using strict less-than for the comparisons, combined with XOR and bitwise AND.
// - Predicates used inside square_check: (x < x1) XOR (x < x2) and (y < y1) XOR (y < y2)
// - Output: 1.0 if inside (true), 0.0 otherwise (false). Because the two sub-expressions yield 0/1 integers, the final result is 0 or 1 before
//   being converted to double.
// - Edge handling: boundary on the left/bottom edges is considered inside (e.g., x == x1 or y == y1 yields true for that axis).
//
// Test approach:
// - Cover true and false branches for various relative positions of (x, y) with respect to the rectangle.
// - Include interior points, left/bottom edges, and points outside (to the left, to the right, above, below, etc.).
// - Use decimals to ensure behavior with non-integer inputs.
// - Use a lightweight non-terminating assertion mechanism that continues executing after failures.

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <cstdlib>
#include <stdio.h>
#include <math.h>


// Import the focal function from the C source file (genpng.c).
// The function is C linkage; use extern "C" for C++ compatibility.
extern "C" double square_check(double x, double y, double x1, double y1, double x2, double y2);

// Simple non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

static bool almost_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) <= tol;
}

#define EXPECT_DOUBLE_EQ(val, expected, desc) do {                    \
    ++g_total_tests;                                                  \
    if (!almost_equal((val), (expected))) {                          \
        ++g_failed_tests;                                             \
        std::cerr << "FAIL: " << (desc)                                   \
                  << " | expected: " << (expected)                   \
                  << ", got: " << (val) << std::endl;              \
    } else {                                                          \
        std::cout << "PASS: " << (desc) << std::endl;               \
    }                                                                 \
} while (0)

#define TEST_CASE(name) void name()

// Candidate Keywords (for traceability in test descriptions):
// - Point: (x, y)
// - Square: defined by (x1, y1) and (x2, y2)
// - Predicates: (x < x1), (x < x2), (y < y1), (y < y2)
// - Logical operations: XOR (^ for ints, due to bool results), AND (&)
// - Behavior: inside iff ((x<x1) ^ (x<x2)) & ((y<y1) ^ (y<y2)) yields 1.0, else 0.0
// - Boundary: x == x1 or y == y1 on the left/bottom edges considered inside for that axis.


// Test 1: Point inside the center of the square (0,0)-(10,10) -> expect 1.0
TEST_CASE(test_square_center_inside) {
    double res = square_check(5.0, 5.0, 0.0, 0.0, 10.0, 10.0);
    EXPECT_DOUBLE_EQ(res, 1.0, "center point should be inside the square");
}

// Test 2: Point on the left edge (x == x1) should be considered inside horizontally
TEST_CASE(test_square_left_edge_inside) {
    double res = square_check(0.0, 5.0, 0.0, 0.0, 10.0, 10.0);
    EXPECT_DOUBLE_EQ(res, 1.0, "point on left edge should be inside horizontally");
}

// Test 3: Point on the bottom edge (y == y1) should be considered inside vertically
TEST_CASE(test_square_bottom_edge_inside) {
    double res = square_check(5.0, 0.0, 0.0, 0.0, 10.0, 10.0);
    EXPECT_DOUBLE_EQ(res, 1.0, "point on bottom edge should be inside vertically");
}

// Test 4: Point outside to the left (x < x1) should be outside
TEST_CASE(test_square_left_outside) {
    double res = square_check(-1.0, 5.0, 0.0, 0.0, 10.0, 10.0);
    EXPECT_DOUBLE_EQ(res, 0.0, "point to the left of the square should be outside");
}

// Test 5: Point outside to the right (x >= x2) should be outside
TEST_CASE(test_square_right_outside) {
    double res = square_check(11.0, 5.0, 0.0, 0.0, 10.0, 10.0);
    EXPECT_DOUBLE_EQ(res, 0.0, "point to the right of the square should be outside");
}

// Test 6: Point outside above (y >= y2) should be outside
TEST_CASE(test_square_above_outside) {
    double res = square_check(5.0, 11.0, 0.0, 0.0, 10.0, 10.0);
    EXPECT_DOUBLE_EQ(res, 0.0, "point above the square should be outside");
}

// Test 7: Non-integer boundaries with interior point (0.5,0.5)-(1.5,1.5), inside at (1.0,1.0)
TEST_CASE(test_square_non_integer_boundaries_inside) {
    double res = square_check(1.0, 1.0, 0.5, 0.5, 1.5, 1.5);
    EXPECT_DOUBLE_EQ(res, 1.0, "interior with non-integer boundaries should be inside");
}

// Test 8: Non-integer boundaries on left edge (x == x1) with non-integer bounds -> inside
TEST_CASE(test_square_non_integer_left_edge_inside) {
    double res = square_check(0.5, 0.5, 0.5, 0.5, 1.5, 1.5);
    // x == x1 makes (x < x1) false, (x < x2) true -> 0 ^ 1 = 1; similarly for y -> inside
    EXPECT_DOUBLE_EQ(res, 1.0, "point on non-integer left edge should be inside");
}

// Test runner
int main() {
    // Run tests
    test_square_center_inside();
    test_square_left_edge_inside();
    test_square_bottom_edge_inside();
    test_square_left_outside();
    test_square_right_outside();
    test_square_above_outside();
    test_square_non_integer_boundaries_inside();
    test_square_non_integer_left_edge_inside();

    std::cout << "Total tests run: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << std::endl;

    return g_failed_tests != 0 ? 1 : 0;
}