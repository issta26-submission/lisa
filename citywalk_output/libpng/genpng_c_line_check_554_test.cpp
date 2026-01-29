// Test suite for the focal method: line_check (in genpng.c)
// 
// Notes:
// - The function signature is:
//     double line_check(double x, double y, double x1, double y1, double x2, double y2,
//                       double w, double expand);
// - It returns 0.0 for outside, and 1.0 (as a double) for inside/within the filter.
// - This test uses a minimal in-house test harness (no Google Test) suitable for C++11.
// - The C implementation is assumed to be linked (genpng.c) and exposed with C linkage.

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <math.h>


extern "C" double line_check(double x, double y, double x1, double y1, double x2, double y2,
                             double w, double expand);

// Simple non-terminating test harness macros
static int gTestFailed = 0;

#define TEST_PASS(msg) \
    do { std::cout << "[PASS] " << msg << std::endl; } while (0)

#define TEST_FAIL(msg) \
    do { std::cerr << "[FAIL] " << msg << std::endl; gTestFailed = 1; } while (0)

// Helper to assert boolean results from line_check
static void assert_line_check_eq(double x, double y,
                                 double x1, double y1, double x2, double y2,
                                 double w, double expand,
                                 double expected,
                                 const std::string &testName)
{
    double res = line_check(x, y, x1, y1, x2, y2, w, expand);
    bool ok = (static_cast<int>(res) == static_cast<int>(expected));
    if (ok) {
        TEST_PASS(testName + " -> expected " + std::to_string((int)expected) + ", got " + std::to_string((int)res));
    } else {
        TEST_FAIL(testName + " -> expected " + std::to_string((int)expected) +
                  ", got " + std::to_string((int)res) +
                  " (res=" + std::to_string(res) + ")");
    }
}

// Test suite for line_check
static void run_line_check_tests()
{
    // Test 1: Outside due to distance from the line (point well above the line)
    // Line: (0,0) -> (10,0). Point: (0, 15) should be outside for typical w/expand.
    assert_line_check_eq(0.0, 15.0, 0.0, 0.0, 10.0, 0.0, 1.0, 0.0,
                         0.0, "Line outside by distance (above line)");

    // Test 2: Inside case (point on the line within segment bounds)
    // Line: (0,0) -> (10,0). Point: (5,0). w=1, expand=0 -> inside
    assert_line_check_eq(5.0, 0.0, 0.0, 0.0, 10.0, 0.0, 1.0, 0.0,
                         1.0, "Point on line inside segment (midpoint)");
    
    // Test 3: Outside case (point beyond segment end along the line)
    // Line: (0,0) -> (10,0). Point: (11,0) should be outside
    assert_line_check_eq(11.0, 0.0, 0.0, 0.0, 10.0, 0.0, 1.0, 0.0,
                         0.0, "Point beyond end of segment along line");

    // Test 4: Inside due to non-zero expand (start point with positive expand)
    // Line: (0,0) -> (10,0). Point: (0,0). w=1, expand=1 -> inside
    assert_line_check_eq(0.0, 0.0, 0.0, 0.0, 10.0, 0.0, 1.0, 1.0,
                         1.0, "Start point with positive expand should be inside");

    // Test 5: End boundary with positive expand (endpoint included with expand)
    // Line: (0,0) -> (10,0). Point: (10,0). w=1, expand=1 -> inside
    assert_line_check_eq(10.0, 0.0, 0.0, 0.0, 10.0, 0.0, 1.0, 1.0,
                         1.0, "End point within expand width should be inside");

    // Test 6: Boundary equality for cross-test (outside due to exact equality)
    // For line (0,0)->(10,0), w=1, expand=0, point (5,1) yields cross^2 == (w+expand)^2 * len2
    // This should be treated as outside (return 0)
    assert_line_check_eq(5.0, 1.0, 0.0, 0.0, 10.0, 0.0, 1.0, 0.0,
                         0.0, "Cross-boundary equality should be outside");

    // Test 7: Degenerate line (start and end are the same point)
    // Line is a point; any x,y should be considered outside
    assert_line_check_eq(3.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                         0.0, "Degenerate line (zero length) should be outside");
}

// Entry point
int main(int argc, const char **argv)
{
    (void)argc; (void)argv;

    std::cout << "Running line_check unit tests (no GoogleTest framework)." << std::endl;

    run_line_check_tests();

    if (gTestFailed) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}