// Unit test suite for inside_line method in genpng.c
// This test suite is written for C++11 without any test framework (no GTest).
// It uses a small, non-terminating assertion harness and runs from main().
// The tests exercise true/false outcomes of inside_line by supplying various
// arg structures and (x,y) query points.

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>


// Import C dependencies with correct linkage.
// We assume the project provides the following definitions through png.h
extern "C" {
// Prototypes (explicitly declare to ensure linkage in C++ tests)
int line_check(double x, double y,
               double x1, double y1, double x2, double y2,
               double w, double expand);

int inside_line(const struct arg *arg, double x, double y);
}

// Simple test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "TEST FAILED: " << msg \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
        ++g_tests_failed; \
        std::cerr << "TEST FAILED: " << msg \
                  << " | Expected: " << (b) << " Actual: " << (a) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

#define ASSERT_DOUBLE_EQ(a, b, eps, msg) do { \
    ++g_tests_run; \
    if(!((std::fabs((a) - (b)) <= (eps)))) { \
        ++g_tests_failed; \
        std::cerr << "TEST FAILED: " << msg \
                  << " | Expected: " << (b) << " Actual: " << (a) \
                  << " | diff=" << std::fabs((a) - (b)) \
                  << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    } \
} while(0)

// Helper to create a commonly used arg structure (fill only the essential fields)
// We rely on the fields x1, y1, x2, y2, width being present in struct arg as per FOCAL_CLASS_DEP.
static struct arg make_arg(double x1, double y1, double x2, double y2, double width) {
    struct arg a;
    // Initialize fields used by inside_line and line_check.
    // Some projects may have additional fields; we only set the known ones.
    // If some projects have different defaults, the constructor-like approach below
    // still yields valid numeric values for the relevant computations.
    // Note: If struct arg contains padding or other fields, they remain uninitialized
    // but should not affect this test since we only read the known fields.
    a.x1 = x1;
    a.y1 = y1;
    a.x2 = x2;
    a.y2 = y2;
    a.width = width;
    return a;
}

// Test 1: Horizontal line on the line segment, point exactly on the line
static void test_inside_line_horizontal_on_line() {
    // Line from (0,0) to (10,0) with width 4 -> w = 2
    struct arg a = make_arg(0.0, 0.0, 10.0, 0.0, 4.0);

    // Point on the line
    int res = inside_line(&a, 5.0, 0.0);
    ASSERT_TRUE(res != 0, "horizontal line: point on the line should be inside");
}

// Test 2: Horizontal line, point within thickness (inside)
static void test_inside_line_horizontal_within_thickness() {
    struct arg a = make_arg(0.0, 0.0, 10.0, 0.0, 4.0);

    // Point near the line within width/2 distance (y = 1)
    int res = inside_line(&a, 5.0, 1.0);
    ASSERT_TRUE(res != 0, "horizontal line: point within thickness should be inside");
}

// Test 3: Horizontal line, point outside thickness (outside)
static void test_inside_line_horizontal_outside_thickness() {
    struct arg a = make_arg(0.0, 0.0, 10.0, 0.0, 4.0);

    // Point outside the half-width
    int res = inside_line(&a, 5.0, 3.0);
    ASSERT_TRUE(res == 0, "horizontal line: point outside thickness should be outside");
}

// Test 4: Vertical line, point on the line (inside)
static void test_inside_line_vertical_on_line() {
    // Line from (0,0) to (0,10) with width 4 -> w = 2
    struct arg a = make_arg(0.0, 0.0, 0.0, 10.0, 4.0);

    int res = inside_line(&a, 0.0, 5.0);
    ASSERT_TRUE(res != 0, "vertical line: point on the line should be inside");
}

// Test 5: Vertical line, point outside thickness (outside)
static void test_inside_line_vertical_outside_thickness() {
    struct arg a = make_arg(0.0, 0.0, 0.0, 10.0, 4.0);

    int res = inside_line(&a, 3.0, 5.0);
    ASSERT_TRUE(res == 0, "vertical line: point outside thickness should be outside");
}

// Test 6: Diagonal line (y = x), point on the line (inside)
static void test_inside_line_diagonal_on_line() {
    struct arg a = make_arg(0.0, 0.0, 10.0, 10.0, 4.0);

    int res = inside_line(&a, 5.0, 5.0);
    ASSERT_TRUE(res != 0, "diagonal line: point on the line should be inside");
}

// Test 7: Diagonal line, point far from line (outside)
static void test_inside_line_diagonal_outside() {
    struct arg a = make_arg(0.0, 0.0, 10.0, 10.0, 4.0);

    int res = inside_line(&a, 10.0, 0.0);
    ASSERT_TRUE(res == 0, "diagonal line: distant point should be outside");
}

// Test 8: Endpoints inclusive (on endpoint should be inside)
static void test_inside_line_endpoint_inclusive() {
    // Horizontal line with width 4
    struct arg a = make_arg(0.0, 0.0, 10.0, 0.0, 4.0);

    int res_start = inside_line(&a, 0.0, 0.0);
    int res_end   = inside_line(&a, 10.0, 0.0);
    ASSERT_TRUE(res_start != 0, "endpoint on start should be inside");
    ASSERT_TRUE(res_end != 0, "endpoint on end should be inside");
}

// Test 9: Zero width (edge case) – only exact line points should be inside
static void test_inside_line_zero_width() {
    struct arg a = make_arg(0.0, 0.0, 10.0, 0.0, 0.0);

    int res_on_line = inside_line(&a, 5.0, 0.0);     // exactly on the line
    int res_off_line = inside_line(&a, 5.0, 0.01);   // off by 0.01 should be outside if width==0
    ASSERT_TRUE(res_on_line != 0, "zero-width line: point on line should be inside");
    ASSERT_TRUE(res_off_line == 0, "zero-width line: off-line point should be outside");
}

// Test 10: Negative coordinates and negative line segment
static void test_inside_line_negative_coordinates() {
    // Horizontal line from (-5,-5) to (5,-5) with width 4
    struct arg a = make_arg(-5.0, -5.0, 5.0, -5.0, 4.0);

    int res_inside = inside_line(&a, 0.0, -5.0);
    int res_outside = inside_line(&a, 0.0, -3.0);
    ASSERT_TRUE(res_inside != 0, "negative coordinates: point on line should be inside");
    ASSERT_TRUE(res_outside == 0, "negative coordinates: point off line should be outside");
}

// Run all tests
static void run_all_tests() {
    test_inside_line_horizontal_on_line();
    test_inside_line_horizontal_within_thickness();
    test_inside_line_horizontal_outside_thickness();
    test_inside_line_vertical_on_line();
    test_inside_line_vertical_outside_thickness();
    test_inside_line_diagonal_on_line();
    test_inside_line_diagonal_outside();
    test_inside_line_endpoint_inclusive();
    test_inside_line_zero_width();
    test_inside_line_negative_coordinates();

    // Summary
    std::cout << "Total tests run: " << g_tests_run
              << ", failures: " << g_tests_failed << std::endl;
}

int main(int argc, const char **argv) {
    // Start test execution from main, as required when not using a testing framework.
    run_all_tests();
    // Return non-zero if any test failed to indicate unsuccessful suite.
    return (g_tests_failed ? 1 : 0);
}