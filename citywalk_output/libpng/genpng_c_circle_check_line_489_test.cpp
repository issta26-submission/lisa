// Test suite for circle_check_line located in genpng.c
// This test harness uses a minimal C++11 test framework (no GTest).
// It relies on the project headers to provide the actual implementations
// of struct arg and the constants INSIDE and OUTSIDE.
// The test cases cover:
 // - Case A: Point well inside outer boundary -> expect INSIDE (between boundary lines)
// - Case B: Point inside outer boundary but when inner check is not satisfied due to large w -> expect 0
// - Case C: Point clearly outside -> expect OUTSIDE

#include <../../png.h>
#include <stddef.h>
#include <string.h>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>


// Import project dependencies and declare the focal function.
// The project uses C code; ensure C linkage for the function prototype.
extern "C" {
// Function signature from focal implementation
int circle_check_line(const struct arg *arg, double x, double y, double w);
}

// Optional: if the header does not provide inline prototypes for the function,
// ensure the prototype is visible to the compiler.
// (The extern "C" block above should suffice in most setups.)

// Simple test harness
static int run_test(const char* name, const std::function<int()> &fn) {
    int ret = fn();
    if (ret == 0) {
        std::cout << "[OK]   " << name << std::endl;
        return 0;
    } else {
        std::cout << "[FAILED] " << name << " (return=" << ret << ")" << std::endl;
        return 1;
    }
}

// The tests assume the following semantics from circle_check_line (as per the focal code):
// - Returns INSIDE when the point is inside the boundary lines between the expanded bounds.
// - Returns 0 when the point is inside the boundary lines themselves (not between lines).
// - Returns OUTSIDE when the point lies outside the boundary lines.
// We rely on the project-provided constants INSIDE and OUTSIDE for assertions.
// The 0-case is explicitly tested to cover the "inside the lines themselves" branch.


int test_circle_check_line_inside_between_boundaries() {
    // Test: Point at the center of a square-like boundary
    // Arg: x1=-1, x2=1, y1=-1, y2=1  -> center at (0,0), widths wx=wy=1
    // w = 0.1 ensures the inner check is performed and should succeed (point at center)
    struct arg a;
    a.x1 = -1.0;
    a.x2 =  1.0;
    a.y1 = -1.0;
    a.y2 =  1.0;

    double x = 0.0;
    double y = 0.0;
    double w = 0.1;

    int ret = circle_check_line(&a, x, y, w);

    // Expect INSIDE (i.e., between the boundary lines)
    if (ret != INSIDE) {
        std::cerr << "Test: inside-between-boundaries - expected INSIDE, got " << ret << std::endl;
        return 1;
    }
    return 0;
}

int test_circle_check_line_inside_lines_only_when_outer_expanded_contains_point() {
    // Test: Same arg as previous, but with a sufficiently large w so that
    // after subtracting w from wx and wy, the inner check is not performed
    // (wx > 0 && wy > 0 fails). We still expect the initial outer check to pass
    // and thus return 0 (inside the lines themselves).
    struct arg a;
    a.x1 = -1.0;
    a.x2 =  1.0;
    a.y1 = -1.0;
    a.y2 =  1.0;

    double x = 0.0;
    double y = 0.0;
    double w = 1.2; // Large enough that wx - w <= 0 and wy - w <= 0

    int ret = circle_check_line(&a, x, y, w);

    // Expect 0 (inside the lines themselves, per the implementation comments)
    if (ret != 0) {
        std::cerr << "Test: inside-lines-only-with-large-w - expected 0, got " << ret << std::endl;
        return 1;
    }
    return 0;
}

int test_circle_check_line_outside_area() {
    // Test: Point far away from the arg's region should be OUTSIDE
    struct arg a;
    a.x1 = -1.0;
    a.x2 =  1.0;
    a.y1 = -1.0;
    a.y2 =  1.0;

    double x = 10.0;
    double y = 10.0;
    double w = 0.1;

    int ret = circle_check_line(&a, x, y, w);

    // Expect OUTSIDE
    if (ret != OUTSIDE) {
        std::cerr << "Test: outside-area - expected OUTSIDE, got " << ret << std::endl;
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    // Run tests and summarize results
    int total = 0;
    int failures = 0;

    total += run_test("circle_check_line_inside_between_boundaries", test_circle_check_line_inside_between_boundaries);
    total += run_test("circle_check_line_inside_lines_only_with_large_w", test_circle_check_line_inside_lines_only_when_outer_expanded_contains_point);
    total += run_test("circle_check_line_outside_area", test_circle_check_line_outside_area);

    for (int i = 0; i < 80; ++i) std::cout << "-";
    std::cout << "\n";

    total = 3; // three tests executed
    // Recompute failures to print final summary
    // Note: failures variable isn't updated in run_test; we infer from printed output.
    // For simplicity, exit code 0 if no failures printed; otherwise 1.
    // In this simple harness, we rely on the per-test return values.
    // Compute failures by checking stdout isn't feasible here; instead
    // we track via individual return values above (already recorded in failures if any).

    // The run_test prints per-test results; accumulate failure count manually by re-running with a diagnostic flag is cumbersome.
    // We'll adopt a straightforward approach: if any test failed, we exit with non-zero status.

    // If any test failed, at least one of the test returns 1 from run_test, but we don't keep a global variable.
    // To ensure correct exit status, we re-run tests with a combined status aggregator.

    // Quick approach: re-run tests with explicit return value capture for final exit.
    int final_status = 0;
    if (test_circle_check_line_inside_between_boundaries()) final_status = 1;
    if (test_circle_check_line_inside_lines_only_when_outer_expanded_contains_point()) final_status = 1;
    if (test_circle_check_line_outside_area()) final_status = 1;

    if (final_status == 0) {
        std::cout << "All tests passed (" << total << " tests)." << std::endl;
    } else {
        std::cout << "Some tests failed." << std::endl;
    }

    return final_status;
}