// Test suite for square_check_line in genpng.c
// This suite uses a lightweight, non-terminating assertion mechanism
// (no Google Test). It calls the focal function via C linkage and
// validates its behavior across representative scenarios.
//
// Notes based on the focal method analysis:
// - The function square_check_line returns a positive value for INSIDE,
//   0 for "inside the lines themselves" (boundary case), and a negative
//   value for OUTSIDE. We validate by checking the sign of the return value
//   to avoid depending on the exact numeric constants of INSIDE/OUTSIDE.
// - The dependent struct arg is assumed to contain fields x1, y1, x2, y2
//   describing a line segment from (x1,y1) to (x2,y2).
// - The tests focus on coverage of true/false branches and boundary cases.
// - We assume the project can be linked with the C source genpng.c providing
//   the implementation of square_check_line (and its dependencies like square_check).

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <cmath>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <math.h>


// Declare the external C interface used by the focal method.
// We mirror the struct arg layout (x1, y1, x2, y2) to match the source.
// The actual implementation resides in genpng.c (or linked object), with C linkage.
extern "C" {

struct arg {
    double x1;
    double y1;
    double x2;
    double y2;
};

// Focal function under test.
int square_check_line(const struct arg *arg, double x, double y, double w);

} // extern "C"

// Lightweight test harness (non-terminating assertions)
static int tests_run = 0;
static int tests_failed = 0;

static void log_pass(const char* name) {
    std::cout << "[PASS] " << name << "\n";
}
static void log_fail(const char* name, const char* detail) {
    std::cerr << "[FAIL] " << name << " - " << detail << "\n";
}

// Helper: compute simple sign for comparison.
// >0 => positive (INSIDE), 0 => boundary, <0 => outside.
static int sign_of(int v) {
    if (v > 0) return 1;
    if (v < 0) return -1;
    return 0;
}

// Helper: classify return value by sign without depending on INSIDE/OUTSIDE macros.
static int classify_result(int r) {
    // r > 0 => inside
    // r == 0 => on/within boundary lines
    // r < 0 => outside
    return sign_of(r);
}

// Test 1: Point strictly inside the diagonal line region
// Arg: diagonal from (0,0) to (2,2); small width w=0.5
// Expected: INSIDE (positive result)
static void test_inside_diagonal_point() {
    const char* test_name = "test_inside_diagonal_point";
    struct arg a;
    a.x1 = 0.0; a.y1 = 0.0;
    a.x2 = 2.0; a.y2 = 2.0;

    double px = 1.0;
    double py = 1.0;
    double w  = 0.5;

    int r = square_check_line(&a, px, py, w);
    int s = classify_result(r);

    if (s == 1) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "expected INSIDE (>0) for point (1,1) with w=0.5 on diagonal (0,0)-(2,2)");
        ++tests_failed;
    }
    ++tests_run;
}

// Test 2: Point inside expanded boundary but inside inner boundary as well (INSIDE)
// Arg: same as Test 1, point at center (1,1) should be inside inner region when w is small
static void test_inside_center_point() {
    const char* test_name = "test_inside_center_point";
    struct arg a;
    a.x1 = 0.0; a.y1 = 0.0;
    a.x2 = 2.0; a.y2 = 2.0;

    double px = 0.75; // inside inner square [0.5, 0.5]..[1.5,1.5]
    double py = 0.75;
    double w  = 0.25;

    int r = square_check_line(&a, px, py, w);
    int s = classify_result(r);

    if (s == 1) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "expected INSIDE (>0) for point (0.75,0.75) with w=0.25 on diagonal (0,0)-(2,2)");
        ++tests_failed;
    }
    ++tests_run;
}

// Test 3: Point inside expanded boundary but outside inner (boundary line region)
// Arg: diagonal from (0,0) to (2,2); w=0.5
// Point chosen to be inside expanded rectangle but outside inner square
static void test_inside_expanded_outside_inner_boundary() {
    const char* test_name = "test_inside_expanded_outside_inner_boundary";
    struct arg a;
    a.x1 = 0.0; a.y1 = 0.0;
    a.x2 = 2.0; a.y2 = 2.0;

    double px = 0.0;
    double py = 0.0;
    double w  = 0.5;

    int r = square_check_line(&a, px, py, w);
    int s = classify_result(r);

    // Expect boundary-like region (0) because inner area is not reached for this w
    if (s == 0) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "expected boundary (0) for point (0,0) with w=0.5 on diagonal (0,0)-(2,2)");
        ++tests_failed;
    }
    ++tests_run;
}

// Test 4: Point outside the expanded boundary
// Arg: diagonal from (0,0) to (2,2); w=0.5
static void test_outside_region() {
    const char* test_name = "test_outside_region";
    struct arg a;
    a.x1 = 0.0; a.y1 = 0.0;
    a.x2 = 2.0; a.y2 = 2.0;

    double px = 5.0;
    double py = 5.0;
    double w  = 0.5;

    int r = square_check_line(&a, px, py, w);
    int s = classify_result(r);

    if (s == -1) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "expected OUTSIDE (<0) for point (5,5) with w=0.5 on diagonal (0,0)-(2,2)");
        ++tests_failed;
    }
    ++tests_run;
}

// Test 5: Large width that eliminates the inner positive region
// Arg: diagonal from (0,0) to (2,2); w=2.0
// Expect no INSIDE result; inside area yields 0 (boundary-like)
static void test_large_w_no_inner_inside() {
    const char* test_name = "test_large_w_no_inner_inside";
    struct arg a;
    a.x1 = 0.0; a.y1 = 0.0;
    a.x2 = 2.0; a.y2 = 2.0;

    double px = 1.0;
    double py = 1.0;
    double w  = 2.0;

    int r = square_check_line(&a, px, py, w);
    int s = classify_result(r);

    // When w >= wx or wy, inner region doesn't exist; result should be boundary-like or outside if not in outer
    // We'll accept boundary-like (0) for a point inside expanded region.
    if (s == 0) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "expected boundary (0) for point (1,1) with w=2.0 on diagonal (0,0)-(2,2)");
        ++tests_failed;
    }
    ++tests_run;
}

// Test 6: Reversed direction of the line (2,2) -> (0,0) to ensure symmetry
static void test_reverse_direction_inside() {
    const char* test_name = "test_reverse_direction_inside";
    struct arg a;
    a.x1 = 2.0; a.y1 = 2.0;
    a.x2 = 0.0; a.y2 = 0.0;

    double px = 1.0;
    double py = 1.0;
    double w  = 0.5;

    int r = square_check_line(&a, px, py, w);
    int s = classify_result(r);

    if (s == 1) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "expected INSIDE (>0) for point (1,1) with w=0.5 on reversed diagonal (2,2)-(0,0)");
        ++tests_failed;
    }
    ++tests_run;
}

// Main runner
int main() {
    std::cout << "Starting unit tests for square_check_line...\n";

    test_inside_diagonal_point();
    test_inside_center_point();
    test_inside_expanded_outside_inner_boundary();
    test_outside_region();
    test_large_w_no_inner_inside();
    test_reverse_direction_inside();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";

    if (tests_failed > 0) {
        std::cerr << "Some tests FAILED. See details above.\n";
        return EXIT_FAILURE;
    }
    std::cout << "All tests PASSED.\n";
    return EXIT_SUCCESS;
}