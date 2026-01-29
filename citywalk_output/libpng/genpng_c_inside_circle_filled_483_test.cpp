/*
Unit Test Suite for inside_circle_filled (genpng.c)

Step 1 - Program Understanding (Keywords)
- Core function under test: inside_circle_filled(const struct arg *arg, double x, double y)
- Dependencies: circle_check(x, y, arg->x1, arg->y1, arg->x2, arg->y2)
- Dependent struct: struct arg with members x1, y1, x2, y2 (likely defined in png.h)
- Behavior: inside_circle_filled delegates to circle_check to determine if (x, y) lies inside a circle defined by coordinates (x1,y1) and (x2,y2)
- Target: Cover true/false outcomes by varying the bounding rectangle that defines the circle

Step 2 - Unit Test Generation (Plan)
- Create tests that exercise inside_circle_filled for:
  - Points clearly inside the circle
  - Points clearly outside the circle
  - Boundary points on the circle (to verify inclusive/exclusive behavior)
  - Different rectangle configurations (positive, negative coordinates, and swapped coordinates)
- Use the actual struct arg type from the project (png.h) to ensure proper memory layout
- Use a lightweight, non-terminating assertion mechanism (no GTest) to maximize code execution
- Access static/global logic only via public APIs (no private internals)
- Compile with C++11 and link against genpng.c (or its object) to satisfy C linkage

Step 3 - Test Case Refinement (Domain Knowledge)
- Ensure test points are chosen such that the circle is well-defined by the rectangle corners
- Use boundary points to verify inclusive handling
- Keep tests isolated and independent, reinitializing arg for each test
- Maintain clear commentary for each test

Code: C++11 test harness (no GTest), using a lightweight EXPECT-style macros and a C linkage to the focal function
- Assumes png.h defines struct arg and exposes inside_circle_filled; if not, adjust include path accordingly.
- The test uses memset to safely initialize the struct to zero before setting known fields to avoid layout assumptions about other members.

Compile guidance (not part of code): 
- Example (adjust paths as needed):
  g++ -std=c++11 -O2 -Wall test_inside_circle_filled.cpp genpng.c -o test_inside_circle_filled
- Ensure the include path to png.h matches your project layout.

*/

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Bring in the project definitions (struct arg and function prototype)
// This header is expected to define struct arg with at least: double x1, y1, x2, y2
extern "C" {
// The focal function under test is defined in genpng.c
int inside_circle_filled(const struct arg *arg, double x, double y);
}

// Lightweight test framework (non-terminating assertions)
static int g_test_total = 0;
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_test_total; \
    if(!(cond)) { \
        std::cerr << "Test failed at line " << __LINE__ << ": " << msg << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    ++g_test_total; \
    if(cond) { \
        std::cerr << "Test failed at line " << __LINE__ << ": " << msg << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

static void run_test_case_inside_circle_filled_basic_inside_outside() {
    // Test 1: Basic rectangle [-1, -1] to [1, 1], center at (0,0), radius sqrt(2)
    struct arg a;
    std::memset(&a, 0, sizeof(a));
    a.x1 = -1.0; a.y1 = -1.0; a.x2 = 1.0; a.y2 = 1.0;

    // Point at center should be inside
    int res_center = inside_circle_filled(&a, 0.0, 0.0);
    EXPECT_TRUE(res_center != 0, "Center (0,0) should be inside the circle");

    // Point clearly inside along x-axis
    int res_inside = inside_circle_filled(&a, 0.5, 0.0);
    EXPECT_TRUE(res_inside != 0, "(0.5,0.0) should be inside the circle");

    // Point clearly outside
    int res_out = inside_circle_filled(&a, 2.0, 0.0);
    EXPECT_FALSE(res_out != 0, "(2.0,0.0) should be outside the circle");
}

static void run_test_case_inside_circle_filled_boundary() {
    // Test 2: Boundary condition for the same rectangle
    struct arg a;
    std::memset(&a, 0, sizeof(a));
    a.x1 = -1.0; a.y1 = -1.0; a.x2 = 1.0; a.y2 = 1.0;

    // Point on the circle boundary (1,1) should be inside if boundary is inclusive
    int res_boundary = inside_circle_filled(&a, 1.0, 1.0);
    EXPECT_TRUE(res_boundary != 0, "(1.0,1.0) on boundary should be inside the circle (inclusive)");
}

static void run_test_case_inside_circle_filled_negative_coords() {
    // Test 3: Rectangle with negative coordinates
    struct arg a;
    std::memset(&a, 0, sizeof(a));
    a.x1 = -2.0; a.y1 = -2.0; a.x2 = 0.0; a.y2 = 0.0;

    // Center at (-1,-1)
    // Point inside: (-1, -0.5) should be inside
    int r_inside = inside_circle_filled(&a, -1.0, -0.5);
    EXPECT_TRUE(r_inside != 0, "(-1.0, -0.5) should be inside the circle");

    // Point outside: (1.0, 1.0) should be outside
    int r_out = inside_circle_filled(&a, 1.0, 1.0);
    EXPECT_FALSE(r_out != 0, "(1.0, 1.0) should be outside the circle");
}

static void run_test_case_inside_circle_filled_swapped_coords() {
    // Test 4: Swapped rectangle corners
    struct arg a;
    std::memset(&a, 0, sizeof(a));
    // corners provided in reverse order
    a.x1 = 1.0; a.y1 = 1.0; a.x2 = -1.0; a.y2 = -1.0;

    // Center at (0,0) despite swapped corners
    int res_center = inside_circle_filled(&a, 0.0, 0.0);
    EXPECT_TRUE(res_center != 0, "Center (0,0) should be inside even with swapped coordinates");

    // Point outside: (2,0) should be outside
    int res_out = inside_circle_filled(&a, 2.0, 0.0);
    EXPECT_FALSE(res_out != 0, "(2.0,0.0) should be outside the circle even with swapped coords");
}

int main() {
    // Run test cases
    run_test_case_inside_circle_filled_basic_inside_outside();
    run_test_case_inside_circle_filled_boundary();
    run_test_case_inside_circle_filled_negative_coords();
    run_test_case_inside_circle_filled_swapped_coords();

    // Summary
    std::cout << "Total tests run: " << g_test_total
              << ", Failures: " << g_test_failures
              << std::endl;

    return g_test_failures ? 1 : 0;
}