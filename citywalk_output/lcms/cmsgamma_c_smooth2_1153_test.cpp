// Lightweight C++11 unit tests for the focal function: smooth2
// This test suite is designed to work without Google Test.
// It uses a small, non-terminating assertion mechanism and prints a summary.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <lcms2_internal.h>
#include <cmath>


// ---------------------------------------------------------------------------
// Minimal type and function declarations to bind with the focal function.
// We provide fallbacks in case cms types are not available via public headers.
// This section is deliberately lightweight and aims to be compatible with
// typical CMS-LittleCMS style environments.
// ---------------------------------------------------------------------------

// Fallback typedefs to represent core CMS types (aligns with common LittleCMS types).
typedef void* cmsContext;
typedef float cmsFloat32Number;
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

// Prototype for the focal method under test.
// Ensure C linkage to match the library's symbol (smooth2 is a C function).
extern "C" cmsBool smooth2(cmsContext ContextID,
                         cmsFloat32Number w[],
                         cmsFloat32Number y[],
                         cmsFloat32Number z[],
                         cmsFloat32Number lambda,
                         int m);

// ---------------------------------------------------------------------------
// Minimal test harness (non-terminating assertions to improve coverage).
// ---------------------------------------------------------------------------

static int g_totalTests = 0;
static int g_failedTests = 0;

// Log a failure message without terminating the test suite.
static void logFailure(const char* msg) {
    std::cerr << "Test Failure: " << msg << std::endl;
}

// Expect condition to be true; count the test regardless, but log on failure.
static void EXPECT_TRUE(bool cond, const char* msg) {
    ++g_totalTests;
    if (!cond) {
        ++g_failedTests;
        logFailure(msg);
    }
}

// Helper: safe isfinite check for cmsFloat32Number
static bool isFiniteFloat(cmsFloat32Number v) {
    return std::isfinite(static_cast<double>(v));
}

// Summary printer
static void printSummary() {
    std::cout << "Total tests: " << g_totalTests
              << ", Failures: " << g_failedTests
              << std::endl;
}

// ---------------------------------------------------------------------------
// Test Cases for smooth2
// Notes on test design:
// - We test the early exit path when m < 4 (should return FALSE).
// - We test a normal path by providing m >= 4 with a large lambda to avoid
//   the early lambda < MATRIX_DET_TOLERANCE path. We then verify that the
//   output z[] contains finite numbers.
// - We test the lambda < tolerance branch by providing a negative lambda, which
//   should cause an immediate FALSE return due to the condition lambda < tolerance.
// - We intentionally avoid asserting exact z values due to their numerical
//   dependence on internal solver behavior; instead we verify finiteness.
// - ContextID is passed as NULL to keep the test simple; the function should
//   gracefully handle a null context for these controlled inputs.
// ---------------------------------------------------------------------------

void test_smooth2_early_exit_m_too_small() {
    // m < 4 should trigger the initial early exit and return FALSE.
    cmsFloat32Number w[12];
    cmsFloat32Number y[12];
    cmsFloat32Number z[12];
    // Initialize some non-zero values; not used due to early return, but kept for realism.
    for (int i = 0; i < 10; ++i) {
        w[i] = 1.0f;
        y[i] = 1.0f;
        z[i] = 0.0f;
    }
    int m = 3;
    cmsFloat32Number lambda = 1.0f;
    cmsBool res = smooth2((cmsContext)nullptr, w, y, z, lambda, m);
    EXPECT_TRUE(res == FALSE, "smooth2 should return FALSE when m < 4");
}

void test_smooth2_normal_case_large_lambda() {
    // Normal path: m >= 4 and lambda large enough to avoid "lambda < tol" branch.
    // We verify that the function returns TRUE and that z[] becomes finite.
    const int m = 4;
    cmsFloat32Number w[m + 1];
    cmsFloat32Number y[m + 1];
    cmsFloat32Number z[m + 1];

    // Prepare inputs: use 1.0 for w and y to keep arithmetic simple.
    for (int i = 0; i <= m; ++i) {
        w[i] = 0.0f;
        y[i] = 0.0f;
        z[i] = 0.0f;
    }
    for (int i = 1; i <= m; ++i) {
        w[i] = 1.0f;
        y[i] = 1.0f;
    }
    // Use a very large lambda to ensure lambda >= MATRIX_DET_TOLERANCE
    cmsFloat32Number lambda = 1e6f;

    cmsBool res = smooth2((cmsContext)nullptr, w, y, z, lambda, m);
    EXPECT_TRUE(res == TRUE, "smooth2 should return TRUE for m>=4 and large lambda");

    // Validate that the resulting z[] values are finite numbers
    for (int i = 1; i <= m; ++i) {
        char msg[80];
        if (!isFiniteFloat(z[i])) {
            std::snprintf(msg, sizeof(msg),
                          "z[%d] should be finite after smoothing, got non-finite value", i);
            EXPECT_TRUE(false, msg);
        } else {
            // Optional trace for successful finite values (commented out to reduce noise)
            // std::cout << "z[" << i << "] = " << z[i] << std::endl;
        }
    }
}

void test_smooth2_negative_lambda_exits() {
    // Lambda below tolerance should trigger the early exit and return FALSE.
    const int m = 4;
    cmsFloat32Number w[m + 1];
    cmsFloat32Number y[m + 1];
    cmsFloat32Number z[m + 1];

    for (int i = 0; i <= m; ++i) {
        w[i] = 1.0f;
        y[i] = 1.0f;
        z[i] = 0.0f;
    }

    cmsFloat32Number lambda = -0.5f; // negative lambda should trigger the lambda < tol path
    cmsBool res = smooth2((cmsContext)nullptr, w, y, z, lambda, m);
    EXPECT_TRUE(res == FALSE, "smooth2 should return FALSE when lambda < tolerance");
}

// ---------------------------------------------------------------------------
// Main function
// ---------------------------------------------------------------------------

int main() {
    // Run tests
    test_smooth2_early_exit_m_too_small();
    test_smooth2_negative_lambda_exits();
    test_smooth2_normal_case_large_lambda();

    // Print summary
    printSummary();
    // Return non-zero if any test failed
    if (g_failedTests > 0) {
        return 1;
    }
    return 0;
}