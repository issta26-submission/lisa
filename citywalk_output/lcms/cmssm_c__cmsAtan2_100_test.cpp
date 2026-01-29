// cmsAtan2_unit_tests.cpp
// Lightweight, GTest-free test suite for the focal function:
// cmsFloat64Number _cmsAtan2(cmsFloat64Number y, cmsFloat64Number x)
// The tests are implemented in C++11 with a minimal, non-terminating assertion framework.
// Extern "C" is used to link against the C implementation in cmssm.c.

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cmath>


// Domain knowledge alignment:
// - Use only standard library and provided methods.
// - Do not rely on private/internal implementation details beyond the focal function.
// - Provide non-terminating assertions to maximize coverage.
// - Access static-like interfaces via straightforward function calls (no mocks).

// Forward declaration of the focal function from the C source.
// We assume cmsFloat64Number is alias of double in the actual project.
// Provide a C linkage to avoid C++ name mangling issues when linking with cmssm.c.
extern "C" double _cmsAtan2(double y, double x);

// Simple non-terminating assertion framework (no exceptions, no test exit on failure).
static int g_tests_run = 0;
static int g_tests_failed = 0;
static std::vector<std::string> g_failure_messages;

// Helper to record a failure without stopping test execution
static void record_failure(const std::string& msg) {
    ++g_tests_run; // count the failing test as a run as well
    g_tests_failed += 1;
    g_failure_messages.push_back(msg);
}

// Helper to record a passing test
static void record_success() {
    ++g_tests_run;
}

// Helper to perform near-equality check for doubles
static void expect_near(double got, double expected, double tol, const std::string& test_desc) {
    if (std::fabs(got - expected) <= tol) {
        record_success();
    } else {
        std::string msg = "Test FAILED: " + test_desc +
                          " | expected ~" + std::to_string(expected) +
                          ", got " + std::to_string(got);
        record_failure(msg);
    }
}

// Run a single test case and report immediate outcome (non-terminating)
static void run_case_atan2(double y, double x, double expected, const std::string& desc) {
    double result = _cmsAtan2(y, x);
    // Use a small tolerance for floating-point comparisons
    const double tol = 1e-12;
    expect_near(result, expected, tol, desc);
}

int main() {
    // Step 2 / Step 3: Generate a comprehensive suite of test cases for _cmsAtan2.
    // We cover undefined case (0,0), along axes, and representative quadrant angles.

    // 1) Undefined case: (y=0, x=0) -> 0
    // Note: The function is specified to return 0 for the (0,0) undefined case.
    run_case_atan2(0.0, 0.0, 0.0, "cmsAtan2(0,0) should return 0 (undefined case handled)");

    // 2) Along positive X-axis: (y=0, x>0) -> 0
    run_case_atan2(0.0, 1.0, 0.0, "cmsAtan2(0, +x) should be 0 degrees");

    // 3) Along negative X-axis: (y=0, x<0) -> 180
    run_case_atan2(0.0, -1.0, 180.0, "cmsAtan2(0, -x) should be 180 degrees");

    // 4) Along positive Y-axis: (y>0, x=0) -> 90
    run_case_atan2(1.0, 0.0, 90.0, "cmsAtan2(+y, 0) should be 90 degrees");

    // 5) Along negative Y-axis: (y<0, x=0) -> 270
    run_case_atan2(-1.0, 0.0, 270.0, "cmsAtan2(-y, 0) should be 270 degrees");

    // 6) First quadrant (y>0, x>0): (1,1) -> 45
    run_case_atan2(1.0, 1.0, 45.0, "cmsAtan2(1,1) should be 45 degrees");

    // 7) Second quadrant (y<0, x>0): (-1,1) -> 315
    run_case_atan2(-1.0, 1.0, 315.0, "cmsAtan2(-1,1) should be 315 degrees");

    // 8) Second case quadrant (y>0, x<0): (1,-1) -> 135
    run_case_atan2(1.0, -1.0, 135.0, "cmsAtan2(1,-1) should be 135 degrees");

    // 9) Third quadrant (y<0, x<0): (-1,-1) -> 225
    run_case_atan2(-1.0, -1.0, 225.0, "cmsAtan2(-1,-1) should be 225 degrees");

    // 10) Arbitrary angle in first quadrant: (y=2, x=3)
    // atan2(2,3) in radians ~ 0.588002603, degrees ~ 33.69006753
    run_case_atan2(2.0, 3.0, 33.6900675, "cmsAtan2(2,3) should be ~33.6900675 degrees");

    // Summary reporting (non-terminating assertions imply all tests run)
    std::cout << "cmsAtan2 unit tests completed. Run: " << g_tests_run
              << ", Failures: " << g_tests_failed << std::endl;

    // If any test failed, return non-zero to indicate failure to the caller/build system
    if (g_tests_failed > 0) {
        std::cout << "Failure details:\n";
        for (const auto& msg : g_failure_messages) {
            std::cout << " - " << msg << "\n";
        }
        return 1;
    } else {
        return 0;
    }
}