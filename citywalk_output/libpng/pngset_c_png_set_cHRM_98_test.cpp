// Lightweight C++11 unit test harness for the focal function png_set_cHRM
// This test runs in a standalone manner (no GoogleTest). It uses simple
// assertion macros and prints results to stdout.
// The test mocks the minimal dependencies required by the focal function
// (conversion to fixed point and the XYZ path) so we can observe the
// interactions without relying on the full external libpng suite.
//
// Note:
// - This test is designed to be compiled alongside the project that provides
//   the actual focal method (png_set_cHRM) in pngset.c.
// - We provide C-compatible mocks for the dependent functions used by png_set_cHRM
//   to capture the inputs passed to those helper routines (png_fixed, png_set_cHRM_fixed,
//   png_set_cHRM_XYZ).
// - We avoid terminating tests on failure (non-terminating assertions) to maximize
//   code coverage.

#include <cstdint>
#include <cmath>
#include <pngpriv.h>
#include <iostream>
#include <cstdio>
#include <cstring>


// ---------------------------
// Mocked/libpng-like types
// ---------------------------

// Define lightweight placeholder type aliases to mimic libpng types in tests.
// We purposefully avoid pulling in real libpng headers to keep the test self-contained.
typedef void* png_ptr;
typedef void* png_inforp;
typedef const void* png_const_structrp;
typedef int32_t png_fixed_point;

// ---------------------------
// Global state to capture function interactions
// ---------------------------

struct FixedArgs {
    png_fixed_point white_x;
    png_fixed_point white_y;
    png_fixed_point red_x;
    png_fixed_point red_y;
    png_fixed_point green_x;
    png_fixed_point green_y;
    png_fixed_point blue_x;
    png_fixed_point blue_y;
};

struct XYZArgs {
    double red_X;
    double red_Y;
    double red_Z;
    double green_X;
    double green_Y;
    double green_Z;
    double blue_X;
    double blue_Y;
    double blue_Z;
};

// Captured calls
static FixedArgs g_lastFixed;
static XYZArgs g_lastXYZ;
static bool g_xyz_called = false;

// Utility: reset capture state between tests
static void reset_capture_state() {
    std::memset(&g_lastFixed, 0, sizeof(g_lastFixed));
    std::memset(&g_lastXYZ, 0, sizeof(g_lastXYZ));
    g_xyz_called = false;
}

// Utility: fixed-point conversion helper (mirrors the test's mock behavior)
static inline int32_t to_fixed(double v) {
    // Use a fixed-scale factor matching the test's mock expectations.
    // Chosen to be precise yet stable for typical test values.
    return static_cast<int32_t>(llround(v * 1000000.0));
}

// ---------------------------
// Mocks for dependencies used by png_set_cHRM
// ---------------------------

// Prototype declarations matching the focal method's usage (extern "C" to resemble C linkage)
extern "C" void png_set_cHRM(png_const_structrp png_ptr, png_inforp info_ptr,
                             double white_x, double white_y,
                             double red_x, double red_y,
                             double green_x, double green_y,
                             double blue_x, double blue_y);

extern "C" void png_set_cHRM_fixed(png_ptr, png_inforp,
                                 png_fixed_point white_x, png_fixed_point white_y,
                                 png_fixed_point red_x, png_fixed_point red_y,
                                 png_fixed_point green_x, png_fixed_point green_y,
                                 png_fixed_point blue_x, png_fixed_point blue_y);

extern "C" void png_set_cHRM_XYZ(png_ptr, png_inforp,
                                double red_X, double red_Y, double red_Z,
                                double green_X, double green_Y, double green_Z,
                                double blue_X, double blue_Y, double blue_Z);

// The real code would define png_fixed(), which converts double to fixed-point.
// We implement a test mock that records the converted fixed values.
extern "C" png_fixed_point png_fixed(png_ptr, double v, const char* /*msg*/) {
    // Record conversion for the test by returning a fixed-point value
    // and also provide a reasonable side-effect via static mapping (not needed here beyond value).
    (void) v;
    return static_cast<png_fixed_point>(to_fixed(v));
}

// The real code would define additional utilities (e.g., ITU fixed handling).
// We provide minimal stubs if needed by the linker; the test focuses on the cHRM call path.

// Mock implementation: record the fixed-point values passed to png_set_cHRM_fixed
extern "C" void png_set_cHRM_fixed(png_ptr /*png_ptr*/, png_inforp /*info_ptr*/,
                                 png_fixed_point white_x, png_fixed_point white_y,
                                 png_fixed_point red_x, png_fixed_point red_y,
                                 png_fixed_point green_x, png_fixed_point green_y,
                                 png_fixed_point blue_x, png_fixed_point blue_y) {
    (void) 0; // suppress unused warnings for non-test builds
    g_lastFixed.white_x = white_x;
    g_lastFixed.white_y = white_y;
    g_lastFixed.red_x = red_x;
    g_lastFixed.red_y = red_y;
    g_lastFixed.green_x = green_x;
    g_lastFixed.green_y = green_y;
    g_lastFixed.blue_x = blue_x;
    g_lastFixed.blue_y = blue_y;
}

// Mock implementation: record the XYZ values passed to png_set_cHRM_XYZ
extern "C" void png_set_cHRM_XYZ(png_ptr /*png_ptr*/, png_inforp /*info_ptr*/,
                               double red_X, double red_Y, double red_Z,
                               double green_X, double green_Y, double green_Z,
                               double blue_X, double blue_Y, double blue_Z) {
    (void) 0; // suppress unused warnings for non-test builds
    g_lastXYZ.red_X = red_X;
    g_lastXYZ.red_Y = red_Y;
    g_lastXYZ.red_Z = red_Z;
    g_lastXYZ.green_X = green_X;
    g_lastXYZ.green_Y = green_Y;
    g_lastXYZ.green_Z = green_Z;
    g_lastXYZ.blue_X = blue_X;
    g_lastXYZ.blue_Y = blue_Y;
    g_lastXYZ.blue_Z = blue_Z;
    g_xyz_called = true;
}

// ---------------------------
// Minimal test framework
// ---------------------------

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAILED] " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_test_failures; \
    } else { \
        std::cout << "[PASSED] " << (msg) << "\n"; \
    } \
} while (0)

static int g_test_failures = 0;

// Expose a helper to print a short summary
static void print_summary() {
    if (g_test_failures == 0) {
        std::cout << "\nALL TESTS PASSED\n";
    } else {
        std::cout << "\nTOTAL FAILURES: " << g_test_failures << "\n";
    }
}

// ---------------------------
// Unit tests for png_set_cHRM
// ---------------------------

// Test 1: Verify fixed-point conversion path passes correct fixed-point values
// This test exercises the conversion from double to fixed-point via png_fixed
// and ensures that the values fed to png_set_cHRM_fixed match the expected
// fixed-point representation derived from the inputs.
static void test_png_set_cHRM_fixed_conversion() {
    reset_capture_state();

    // Inputs chosen to exercise typical values
    double white_x = 0.3127;
    double white_y = 0.3290;
    double red_x   = 0.64;
    double red_y   = 0.33;
    double green_x = 0.3;
    double green_y = 0.6;
    double blue_x  = 0.15;
    double blue_y  = 0.06;

    // Call the focal method (will indirectly call mocks)
    png_set_cHRM(nullptr, nullptr,
                 white_x, white_y,
                 red_x, red_y,
                 green_x, green_y,
                 blue_x, blue_y);

    // Expected fixed-point results (scale = 1e6 as in to_fixed)
    int32_t exp_white_x = to_fixed(white_x);
    int32_t exp_white_y = to_fixed(white_y);
    int32_t exp_red_x   = to_fixed(red_x);
    int32_t exp_red_y   = to_fixed(red_y);
    int32_t exp_green_x = to_fixed(green_x);
    int32_t exp_green_y = to_fixed(green_y);
    int32_t exp_blue_x  = to_fixed(blue_x);
    int32_t exp_blue_y  = to_fixed(blue_y);

    TEST_ASSERT(g_lastFixed.white_x == exp_white_x, "png_set_cHRM_fixed: White X fixed-point matches expected");
    TEST_ASSERT(g_lastFixed.white_y == exp_white_y, "png_set_cHRM_fixed: White Y fixed-point matches expected");
    TEST_ASSERT(g_lastFixed.red_x   == exp_red_x,   "png_set_cHRM_fixed: Red X fixed-point matches expected");
    TEST_ASSERT(g_lastFixed.red_y   == exp_red_y,   "png_set_cHRM_fixed: Red Y fixed-point matches expected");
    TEST_ASSERT(g_lastFixed.green_x == exp_green_x, "png_set_cHRM_fixed: Green X fixed-point matches expected");
    TEST_ASSERT(g_lastFixed.green_y == exp_green_y, "png_set_cHRM_fixed: Green Y fixed-point matches expected");
    TEST_ASSERT(g_lastFixed.blue_x  == exp_blue_x,  "png_set_cHRM_fixed: Blue X fixed-point matches expected");
    TEST_ASSERT(g_lastFixed.blue_y  == exp_blue_y,  "png_set_cHRM_fixed: Blue Y fixed-point matches expected");
}

// Test 2: Verify that the XYZ path is invoked with the expected non-fixed inputs
// This test ensures that the code makes the XYZ path call with non-fixed (double) values.
// We don't assert exact XYZ numbers (as they depend on internal computations), but we do
// assert that the XYZ function was indeed called and that the inputs passed are those we supplied
// to the focal method for at least the first color component (Red X).
static void test_png_set_cHRM_xyz_path_invoked() {
    reset_capture_state();

    double white_x = 0.2;
    double white_y = 0.25;
    double red_x   = 0.7;
    double red_y   = 0.2;
    double green_x = 0.15;
    double green_y = 0.8;
    double blue_x  = 0.1;
    double blue_y  = 0.05;

    png_set_cHRM(nullptr, nullptr,
                 white_x, white_y,
                 red_x, red_y,
                 green_x, green_y,
                 blue_x, blue_y);

    // Confirm that XYZ pathway was invoked
    TEST_ASSERT(g_xyz_called, "png_set_cHRM_XYZ path was invoked");

    // Validate that the first XYZ inputs (Red X) match the provided red_x
    // and similarly for Red Y (as a basic sanity check).
    TEST_ASSERT(g_lastXYZ.red_X == red_x, "png_set_cHRM_XYZ: Red X matches input");
    TEST_ASSERT(g_lastXYZ.red_Y == red_y, "png_set_cHRM_XYZ: Red Y matches input");
}

// Test 3: Multiple invocations ensure state resets between tests
static void test_png_set_cHRM_multiple_calls_consistency() {
    reset_capture_state();

    // First set
    png_set_cHRM(nullptr, nullptr,
                 0.1, 0.2,
                 0.3, 0.4,
                 0.5, 0.6,
                 0.7, 0.8);

    int32_t exp_wx1 = to_fixed(0.1);
    int32_t exp_wy1 = to_fixed(0.2);
    TEST_ASSERT(g_lastFixed.white_x == exp_wx1, "First call: White X fixed-point check");
    TEST_ASSERT(g_lastFixed.white_y == exp_wy1, "First call: White Y fixed-point check");

    // Reset and second set
    reset_capture_state();
    png_set_cHRM(nullptr, nullptr,
                 0.11, 0.22,
                 0.31, 0.41,
                 0.51, 0.61,
                 0.71, 0.81);

    int32_t exp_wx2 = to_fixed(0.11);
    int32_t exp_wy2 = to_fixed(0.22);
    TEST_ASSERT(g_lastFixed.white_x == exp_wx2, "Second call: White X fixed-point check");
    TEST_ASSERT(g_lastFixed.white_y == exp_wy2, "Second call: White Y fixed-point check");
}

// ---------------------------
// Main entry
// ---------------------------

int main() {
    std::cout << "Starting tests for png_set_cHRM (mocked environment)\n";

    test_png_set_cHRM_fixed_conversion();
    test_png_set_cHRM_xyz_path_invoked();
    test_png_set_cHRM_multiple_calls_consistency();

    print_summary();
    return g_test_failures ? 1 : 0;
}

// End of test harness

/************************************************************
Notes and rationale for test design:

- The tests focus on the key behavior of png_set_cHRM as exercised in
  the provided focal code:
  - It calls png_set_cHRM_fixed() with fixed-point representations of
    the input doubles white_x, white_y, red_x, red_y, green_x, green_y,
    blue_x, blue_y. We validate this mapping using a deterministic
    mock of png_fixed (via to_fixed with a 1e6 scale).

  - It subsequently calls png_set_cHRM_XYZ() with 9 double values (red_X,
    red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z).
    We verify that the XYZ pathway is invoked and that the first XYZ
    inputs reflect the doubles passed to the focal method (e.g., red_X).

- Test coverage strategy:
  - Test 1 asserts the fixed-point conversion pathway produces expected results.
  - Test 2 asserts the XYZ path is invoked and captures input values.
  - Test 3 exercises repeated calls and ensures state resets do not leak across
    invocations (consistency of behavior).

- Domain knowledge application:
  - Assertions are non-terminating; failures only log but do not exit.
  - We use only standard C++11 and standard I/O; no GTest or external mocking
    frameworks.
  - Static dependencies in this test are emulated via global state (g_lastFixed,
    g_lastXYZ) to reflect captured arguments from the focal method's internal calls.

- Limitations:
  - The actual XYZ conversion from XY to XYZ beyond the input doubles is opaque
    without the full libpng implementation. The test therefore asserts that the
    XYZ path is invoked and that the initial red_X/red_Y inputs are preserved,
    which is a reasonable proxy given the test harness constraints.
 ************************************************************/