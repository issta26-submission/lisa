// Unit test suite for the focal method gamma_end (as implemented in pngvalid.c)
// This test harness is a self-contained C++11 program that provides minimal
// stubs/mocks for the dependent types and functions used by gamma_end.
// It verifies both branches of the conditional inside gamma_end without using GTest.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Step 1: Program Understanding (Candidate Keywords)
// - gamma_end: tests depending on progressive_ptr and gamma_display
// - gamma_display: contains a nested this with fields: speed, ps (pointer to a struct with validated)
// - png_get_progressive_ptr(pp): returns the progressive object (here mocked to return a dp pointer)
// - gamma_image_validate(dp, pp, pi): called when !dp->this.speed
// - dp->this.ps->validated: set to 1 when speed is non-zero
// - voidcast(type, expr): macro used to cast the progressive pointer (mocked here)

// Step 2: Unit Test Generation
// - Create a minimal environment that mimics the dependencies needed by gamma_end
// - Test both branches of the if (!dp->this.speed) condition
// - Ensure static/global state is reset between tests

// Step 3: Test Case Refinement
// - Use a lightweight test framework implemented in this file (no GTest)
// - Provide non-terminating assertions (do not abort on failure)
// - Use only C++ standard library
// - Access static-like state via global/test-local variables (as appropriate for this mock)


// Domain Knowledge Helpers
// Type aliases to mimic the libpng types used by gamma_end
typedef void* png_structp;
typedef void* png_infop;
typedef const void* png_const_structp;

// Mocked gamma related types (as used in the focal method)
typedef struct gamma_ps {
    int validated;
} gamma_ps;

typedef struct gamma_this {
    int speed;
    gamma_ps *ps; // pointer to gamma_ps
} gamma_this;

typedef struct gamma_display {
    gamma_this this;
} gamma_display;

// Macro used in the focal method
#define voidcast(type, x) ((type)(x))

// Forward declarations of dependencies used by gamma_end (mocked)
static gamma_display* g_prog_ptr = nullptr; // global progressive object pointer

extern "C" void* png_get_progressive_ptr(png_structp pp) {
    // In tests, return the currently configured progressive object
    return static_cast<void*>(g_prog_ptr);
}

// Flag to verify gamma_image_validate was invoked
static int gamma_image_validate_called = 0;

// Mocked gamma_image_validate to verify that the call path is exercised
extern "C" void gamma_image_validate(gamma_display *dp, png_structp pp, png_infop pi) {
    (void)dp; (void)pp; (void)pi;
    gamma_image_validate_called = 1;
}

// The focal method under test (recreated here to run in the test binary).
// It mirrors the logic exactly as provided in <FOCAL_METHOD>.
extern "C" void gamma_end(png_structp ppIn, png_infop pi) {
    // Local C->C++-friendly aliasing to match the original code
    png_const_structp pp = ppIn; // mimic: png_const_structp pp = ppIn;
    gamma_display *dp = voidcast(gamma_display*, png_get_progressive_ptr(pp));
    if (!dp->this.speed)
        gamma_image_validate(dp, pp, pi);
    else
        dp->this.ps->validated = 1;
}

// Test framework (lightweight, non-terminating assertions)
static int g_total_tests = 0;
static int g_total_passed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (cond) { \
        ++g_total_passed; \
    } else { \
        std::cerr << "Test failed: " << (msg) \
                  << " (function: " << __func__ << ")" \
                  << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_total_tests; \
    if ((a) == (b)) { \
        ++g_total_passed; \
    } else { \
        std::cerr << "Test failed: " << (msg) \
                  << " | expected: " << (a) \
                  << ", actual: " << (b) \
                  << " (function: " << __func__ << ")" \
                  << std::endl; \
    } \
} while (0)

static void summary() {
    std::cout << "Test results: " << g_total_passed << " / " << g_total_tests << " tests passed." << std::endl;
}

// Test 1: dp->this.speed == 0 should trigger gamma_image_validate(dp, pp, pi)
void test_gamma_end_calls_gamma_image_validate_when_speed_zero() {
    // Arrange
    gamma_ps ps;
    ps.validated = 0;
    gamma_display dp;
    dp.this.speed = 0;
    dp.this.ps = &ps;

    // Prepare progressive_ptr to return our dp
    g_prog_ptr = &dp;
    gamma_image_validate_called = 0;

    // Act
    gamma_end(nullptr, nullptr);

    // Assert
    EXPECT_TRUE(gamma_image_validate_called == 1, "gamma_end should call gamma_image_validate when speed == 0");
    // Sanity: ps should remain unchanged (not modified by gamma_image_validate in this branch)
    EXPECT_EQ(ps.validated, 0, "ps.validated should remain unchanged when speed == 0");
}

// Test 2: dp->this.speed != 0 should set dp->this.ps->validated to 1
void test_gamma_end_sets_validated_when_speed_nonzero() {
    // Arrange
    gamma_ps ps;
    ps.validated = 0;
    gamma_display dp;
    gamma_ps* pps = &ps;
    dp.this.speed = 1;
    dp.this.ps = pps;

    // Prepare progressive_ptr to return our dp
    g_prog_ptr = &dp;

    // Act
    gamma_end(nullptr, nullptr);

    // Assert
    EXPECT_TRUE(ps.validated == 1, "dp->this.ps->validated should be set to 1 when speed != 0");
}

// Entry point
int main() {
    // Display what this test suite is about
    std::cout << "Starting gamma_end unit tests (mocked environment)" << std::endl;

    // Run tests
    test_gamma_end_calls_gamma_image_validate_when_speed_zero();
    test_gamma_end_sets_validated_when_speed_nonzero();

    // Summary
    summary();
    return (g_total_tests == g_total_passed) ? 0 : 1;
}