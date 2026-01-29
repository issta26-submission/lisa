// Test suite for the focal method inside_circle in genpng.c
// This test is written for C++11, does not use GTest, and calls the C function
// inside_circle via C linkage. It uses a minimal test harness with non-terminating
// assertions to exercise true/false branches.
//
// Important: This test relies on the public behavior of inside_circle which uses
// arg->width to derive the circle radius (arg->width/2) and circle_check_line(arg, x, y, w).
// The tests aim to exercise points clearly inside and outside the circle boundary.

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>


// Bring in C linkage for the focal function and its arg struct.
// We declare a minimal struct arg with a width member because inside_circle uses arg->width.
extern "C" {
    // The actual implementation is in genpng.c; this struct definition is provided here
    // to allow construction of test arguments matching the expected layout.
    struct arg {
        int width;
        // Other fields may exist in the real project; tests only rely on width.
    };

    // Focal method under test
    int inside_circle(const struct arg *arg, double x, double y);
}

// Simple non-terminating test assertion helpers
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if (!(cond)) { \
        ++tests_failed; \
        std::cerr << "TEST_FAIL: " << msg << std::endl; \
    } else { \
        /* Optional: can print successes if needed for debugging */ \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    ++tests_run; \
    if (cond) { \
        ++tests_failed; \
        std::cerr << "TEST_FAIL: " << msg << std::endl; \
    } else { \
        /* pass silently */ \
    } \
} while(0)


// Test 1: Center point should be inside the circle for a typical width.
// Given width = 100, radius = width/2 = 50, center is at (50, 50).
// Point (50, 50) is exactly at the center, should yield inside_circle == 1 (true).
void test_inside_circle_center_true() {
    // Arrange
    struct arg a;
    a.width = 100;

    // Act
    int res = inside_circle(&a, a.width / 2.0, a.width / 2.0); // (50.0, 50.0)

    // Assert
    EXPECT_TRUE(res != 0, "inside_circle should be true for the center point (width=100).");
}

// Test 2: Far outside the circle should yield false.
// For width = 100, center (50,50), point (0,0) is outside the circle of radius 50.
void test_inside_circle_center_outside_false() {
    // Arrange
    struct arg a;
    a.width = 100;

    // Act
    int res = inside_circle(&a, 0.0, 0.0);

    // Assert
    EXPECT_FALSE(res != 0, "inside_circle should be false for a far outside point (0,0) with width=100.");
}

// Test 3: Small width boundary scenario with radius = width/2 using integer division.
// width = 3 => radius = 1 (since 3/2 == 1 in integer division). Center is at (1.5, 1.5).
// Point (1,1) is inside since distance ~0.707 < 1.
void test_inside_circle_small_width_inside_true() {
    // Arrange
    struct arg a;
    a.width = 3;

    // Act
    int res = inside_circle(&a, 1.0, 1.0);

    // Assert
    EXPECT_TRUE(res != 0, "inside_circle should be true for point (1,1) with width=3 (radius=1).");
}

// Test 4: Small width outside scenario for width=3.
// Point (0,0) is outside the circle centered at (1.5,1.5) with radius 1.
void test_inside_circle_small_width_outside_false() {
    // Arrange
    struct arg a;
    a.width = 3;

    // Act
    int res = inside_circle(&a, 0.0, 0.0);

    // Assert
    EXPECT_FALSE(res != 0, "inside_circle should be false for point (0,0) with width=3.");
}

// Run all tests and report summary
void run_all_tests() {
    test_inside_circle_center_true();
    test_inside_circle_center_outside_false();
    test_inside_circle_small_width_inside_true();
    test_inside_circle_small_width_outside_false();

    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
}

int main() {
    run_all_tests();
    return (tests_failed == 0) ? 0 : 1;
}