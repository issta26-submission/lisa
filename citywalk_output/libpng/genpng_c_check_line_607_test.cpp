// Auto-generated lightweight C++11 test suite for the C function
// check_line from genpng.c. This test harness uses plain C linkage
// and a minimal in-file Arg structure that matches the usage in
// check_line. No GoogleTest is used; tests are run from main() with
// simple assertions that print diagnostic information and continue
// execution to maximize coverage.
//
// Notes:
// - This test focuses on exercising the branching structure of check_line:
//   1) When the first line_check() returns true and the width is small,
//      the function is expected to return 0 (inside-capability not guaranteed
//      to require extra filtering).
//   2) When the width is large enough, and the line is near the point, check_line
//      should produce a non-zero result indicating INSIDE (or equivalent non-zero
//      code).
// - We do not rely on private methods or private state.
// - We do not mock; we call the real function from the linked genpng.c.
//
// How to compile (example):
// g++ -std=c++11 -I. test_genpng_check_line.cpp genpng.c -o test_genpng_check_line
//
// Then run: ./test_genpng_check_line
//
// The test prints per-test results and a final summary.

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <math.h>


// Ensure C linkage for the focal function and its dependencies.
extern "C" {

// Minimal matcher for the C struct arg as used in genpng.c
// This structure mirrors the fields accessed by check_line:
//   arg->x1, arg->y1, arg->x2, arg->y2, arg->width
struct arg {
    double x1;
    double y1;
    double x2;
    double y2;
    double width;
};

// Focal function under test (defined in genpng.c)
int check_line(const struct arg *arg, double x, double y);

// Optional helper from genpng.c (not used directly here, but available if needed for deeper debugging)
// double line_check(double x, double y, double x1, double y1, double x2, double y2,
//                   double w, double expand);
}

// Simple assertion macros that do not terminate on failure and allow continued execution.
// They print a clear diagnostic message and accumulate a global count of failures.
static int g_failures = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[OK]   " << (msg) << std::endl; \
    } \
} while (0)

#define ASSERT_EQ(actual, expected, msg) do { \
    if (!((actual) == (expected))) { \
        std::cerr << "[FAIL] " << (msg) << " Expected: " << (expected \
                  << "  Got: " << (actual)) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[OK]   " << (msg) << std::endl; \
    } \
} while (0)

#define ASSERT_NEQ(actual, not_expected, msg) do { \
    if ((actual) == (not_expected)) { \
        std::cerr << "[FAIL] " << (msg) << " Value should differ from: " \
                  << (not_expected) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[OK]   " << (msg) << std::endl; \
    } \
} while (0)

// Helper to present test name headers
#define TESTCASE(name) std::cout << "\n=== Test: " << (name) << " ===\n";

int main(int argc, char **argv) {

    (void)argc; (void)argv;

    std::cout << "Starting test suite for genpng.c check_line" << std::endl;

    // Test 1: Small width scenario should yield a non-inside outcome.
    // Rationale: With a small width, the inner check (second line_check)
    // may not even be evaluated, leading to a 0 result when the point lies
    // near the line for the tested geometry.
    TESTCASE("Small width yields zero (edge case before inner check)");
    {
        struct arg a;
        a.x1 = 0.0; a.y1 = 0.0;
        a.x2 = 100.0; a.y2 = 0.0;
        // Choose a small width; since FILTER_WIDTH is a compile-time constant
        // in genpng.c, setting a modest value should satisfy the "width/2" path
        // without triggering the second condition.
        a.width = 1.0;

        // Point located on the line (x along the line, y=0)
        double x = 50.0;
        double y = 0.0;

        int r = check_line(&a, x, y);

        // We expect 0 in this specific construction as per the method's
        // early return when inside-branch is not fully satisfied.
        // If r is not 0 on some environments, this test will fail to keep coverage.
        ASSERT_EQ(r, 0, "check_line should return 0 for small width near line");
    }

    // Test 2: Large width scenario should allow an INSIDE outcome for a point near the line.
    // Rationale: With a sufficiently large width, the inner condition (second line_check)
    // becomes relevant and should yield a non-zero result indicating INSIDE (or equivalent).
    TESTCASE("Large width near line yields non-zero (inside)");
    {
        struct arg a;
        a.x1 = -10.0; a.y1 = -5.0;
        a.x2 =  10.0; a.y2 =  5.0;
        a.width = 200.0; // Large width to ensure width > 2*FILTER_WIDTH

        // Point near the line segment defined by (x1,y1)-(x2,y2)
        double x = 0.0;
        double y = 0.0;

        int r = check_line(&a, x, y);

        // Expect a non-zero value (INSIDE or equivalent) for this geometry.
        ASSERT_NEQ(r, 0, "check_line should return non-zero for large width inside near the line");
    }

    // Test 3: Outside scenario (far from the line) should yield a non-inside result
    // (depending on implementation, this may be OUTSIDE or 0 if on edge). We attempt
    // to exercise the outside branch as best as possible with a distant point.
    TESTCASE("Far outside point yields non-zero (outside)");
    {
        struct arg a;
        a.x1 = 0.0; a.y1 = 0.0;
        a.x2 = 20.0; a.y2 = 0.0;
        a.width = 1.0;

        // Point clearly far from the line (y significantly off the line)
        double x = 10.0;
        double y = 50.0;

        int r = check_line(&a, x, y);

        // We expect a non-zero value representing that the point is outside
        // or not inside. If the library defines OUTSIDE as a specific non-zero
        // code, this assertion will validate non-insideness.
        ASSERT_NEQ(r, 0, "check_line should return non-zero for far outside point");
    }

    // Summary
    std::cout << "\nTest suite finished. Failures: " << g_failures << std::endl;

    // Return non-zero if any test failed to aid CI tooling
    return g_failures == 0 ? 0 : 1;
}