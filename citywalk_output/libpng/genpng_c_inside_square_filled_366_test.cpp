// Simple C++11 unit tests for the function inside_square_filled in genpng.c
// This test suite is designed to be self-contained (no Google Test) and
// to exercise true/false paths of the focal method.
// The tests rely on a compatible representation of 'struct arg' that matches
// the fields used by inside_square_filled (x1, y1, x2, y2).
// The test harness mimics a minimal assertion framework and reports results
// without terminating on the first failure.

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Step 1: Provide C linkage for the focal function.
// We declare the struct arg with the fields used by inside_square_filled.
// Note: This layout is chosen to be compatible with the production code's
// usage of arg->x1, arg->y1, arg->x2, arg->y2 inside square-related checks.
extern "C" {
    struct arg {
        double x1;
        double y1;
        double x2;
        double y2;
    };

    // Focal method under test
    int inside_square_filled(const struct arg *arg, double x, double y);
}

// Lightweight test harness (non-terminating assertions)
static int total_tests = 0;
static int failed_tests = 0;

#define LOG_PASS(msg) do { std::cout << "[PASS] " << (msg) << std::endl; } while(0)
#define LOG_FAIL(msg) do { std::cout << "[FAIL] " << (msg) << std::endl; } while(0)
#define ASSERT(cond, msg) do { \
    total_tests++; \
    if (cond) { LOG_PASS(msg); } else { LOG_FAIL(msg); failed_tests++; } \
} while(0)

// Step 2 & 3: Test cases for inside_square_filled
// Candidate Keywords considered for test reasoning:
// - square_check: algorithm to determine inside vs outside a defined square.
// - arg.x1, arg.y1, arg.x2, arg.y2: square bounds.
// - Boundary inclusion (edges considered inside or not) may affect results.
// - Typical scenarios: inside, on boundary, outside, negative coordinates.
// - The function is pure with respect to its inputs; no external state to mock.
// - We avoid private/internal methods; we only exercise the public focal method.

bool test_inside_true_basic() {
    // Description: Point strictly inside a standard square [0,0] to [10,10]
    arg a;
    a.x1 = 0.0;
    a.y1 = 0.0;
    a.x2 = 10.0;
    a.y2 = 10.0;

    double x = 5.0;
    double y = 5.0;

    int res = inside_square_filled(&a, x, y);
    return (res != 0);
}

bool test_on_boundary_true() {
    // Description: Point on the lower-left boundary (corner) should be inside.
    arg a;
    a.x1 = 0.0;
    a.y1 = 0.0;
    a.x2 = 10.0;
    a.y2 = 10.0;

    double x = 0.0;
    double y = 0.0;

    int res = inside_square_filled(&a, x, y);
    // Depending on the implementation, boundary may be considered inside.
    // We treat boundary as inside here and expect non-zero.
    return (res != 0);
}

bool test_outside_true_false() {
    // Description: Point clearly outside the square.
    arg a;
    a.x1 = -3.0;
    a.y1 = -3.0;
    a.x2 = 2.0;
    a.y2 = 2.0;

    double x = 5.0;
    double y = 5.0;

    int res = inside_square_filled(&a, x, y);
    return (res == 0);
}

bool test_inside_negative_coordinates() {
    // Description: Degenerated to a negative coordinate square and inside point.
    // Square from [-10, -10] to [-1, -1], point at [-5,-5] should be inside.
    arg a;
    a.x1 = -10.0;
    a.y1 = -10.0;
    a.x2 = -1.0;
    a.y2 = -1.0;

    double x = -5.0;
    double y = -5.0;

    int res = inside_square_filled(&a, x, y);
    return (res != 0);
}

bool test_outside_negative_to_positive() {
    // Description: Point outside a square that spans from negative to positive space.
    // Square from [-5, -5] to [0, 0], point at [4,4] should be outside.
    arg a;
    a.x1 = -5.0;
    a.y1 = -5.0;
    a.x2 = 0.0;
    a.y2 = 0.0;

    double x = 4.0;
    double y = 4.0;

    int res = inside_square_filled(&a, x, y);
    return (res == 0);
}

int main() {
    std::cout << "Running inside_square_filled unit tests (C++ harness)" << std::endl;

    // Execute tests with explanatory comments
    // Test 1: Inside
    ASSERT(test_inside_true_basic(), "inside_square_filled should return non-zero for a point strictly inside the square [0,0]-[10,10]");

    // Test 2: Boundary
    ASSERT(test_on_boundary_true(), "inside_square_filled should return non-zero for a point on the square boundary (0,0)");

    // Test 3: Outside
    ASSERT(test_outside_true_false(), "inside_square_filled should return zero for a point clearly outside the square");

    // Test 4: Negative coordinates inside
    ASSERT(test_inside_negative_coordinates(), "inside_square_filled should return non-zero for a point inside a negative-coordinate square");

    // Test 5: Outside boundary case with negative-to-positive range
    ASSERT(test_outside_negative_to_positive(), "inside_square_filled should return zero for a point outside a negative-to-zero square");

    std::cout << "Test summary: " << total_tests << " tests, "
              << failed_tests << " failures." << std::endl;

    // Non-terminating test harness: return non-zero if any failure occurred
    return (failed_tests > 0) ? 1 : 0;
}