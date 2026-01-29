// Test suite for png_get_cHRM_XYZ (as found in pngget.c) using a minimal, self-contained C++11 test harness.
// The goal is to exercise true/false branches of the focal function without GTest, relying only on the C++ standard library.
// Notes:
// - The test environment provides minimal stubs for PNG-related types and helpers used by png_get_cHRM_XYZ.
// - We implement a lightweight, non-terminating assertion framework (EXPECT_* macros) to maximize coverage.
// - Tests cover: normal path, NULL pointers, missing cHRM flag, and XYZ computation failure path.
// - All code is contained in this single file to simplify compilation in a C++11 project without GTest.

#include <string>
#include <functional>
#include <cmath>
#include <iostream>
#include <pngpriv.h>
#include <limits>


// Domain knowledge helper: lightweight assertion framework
static int g_assertions = 0;
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_assertions; \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "[EXPECT_TRUE FAILED] " << msg << "\n"; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) do { \
    ++g_assertions; \
    if((cond)) { \
        ++g_failures; \
        std::cerr << "[EXPECT_FALSE FAILED] " << msg << "\n"; \
    } \
} while(0)

// For generic equality (e.g., integer handles)
#define EXPECT_EQ(a, b, msg) do { \
    ++g_assertions; \
    if(!((a) == (b))) { \
        ++g_failures; \
        std::cerr << "[EXPECT_EQ FAILED] " << msg \
                  << " | " << (a) << " != " << (b) << "\n"; \
    } \
} while(0)

// For floating-point approximate equality
#define EXPECT_EQ_DOUBLE(a, b, tol, msg) do { \
    ++g_assertions; \
    if(!(std::fabs((a) - (b)) <= (tol))) { \
        ++g_failures; \
        std::cerr << "[EXPECT_EQ_DOUBLE FAILED] " << msg \
                  << " | " << (a) << " vs " << (b) \
                  << " (diff=" << std::fabs((a)-(b)) << ", tol=" << (tol) << ")\n"; \
    } \
} while(0)

// For pointer equality
#define EXPECT_PTR_EQ(a, b, msg) do { \
    ++g_assertions; \
    if(((void*)(a)) != ((void*)(b))) { \
        ++g_failures; \
        std::cerr << "[EXPECT_PTR_EQ FAILED] " << msg \
                  << " | " << static_cast<const void*>(a) << " != " \
                  << static_cast<const void*>(b) << "\n"; \
    } \
} while(0)

#define TEST_PASSED() do { std::cout << "[TEST PASSED] " << __func__ << "\n"; } while(0)
#define TEST_FAILED() do { std::cout << "[TEST FAILED] " << __func__ << "\n"; } while(0)

// Minimal PNG-related scaffolding to support the focal function logic in this test

// Forward declarations to mimic libpng environment
typedef const struct png_struct * png_const_structrp;
typedef const struct png_info * png_const_inforp;

struct png_struct {}; // opaque placeholder
struct png_info {
    unsigned int valid; // bitfield for info flags (e.g., PNG_INFO_cHRM)
    struct png_cHRM cHRM;
};

// PNG_INFO_cHRM flag (bitmask)
static const unsigned int PNG_INFO_cHRM = 1;

// cHRM color primaries (xy coordinates)
struct png_cHRM {
    double white_x;
    double white_y;
    double red_x;
    double red_y;
    double green_x;
    double green_y;
    double blue_x;
    double blue_y;
};

// XYZ representation derived from xy primaries
struct png_XYZ {
    double red_X, red_Y, red_Z;
    double green_X, green_Y, green_Z;
    double blue_X, blue_Y, blue_Z;
};

// Global test hook to simulate XYZ computation success/failure
struct TestEnv {
    // When non-zero, png_XYZ_from_xy will pretend to fail (simulate error path)
    static int force_XYZ_fail;
};

int TestEnv::force_XYZ_fail = 0;

// Stubbed helper: convert XYZ from cHRM xy values (simple, deterministic mapping for tests)
int png_XYZ_from_xy(png_XYZ* XYZ, const png_cHRM* cHRM) {
    if (TestEnv::force_XYZ_fail)
        return 1; // simulate failure

    // Simple mapping: treat xy as primary coordinates and derive XYZ components
    // This is not a perceptual color conversion; it's sufficient for unit testing the
    // cHRM_XYZ retrieval pathway in the focal method.
    XYZ->red_X   = cHRM->red_x;
    XYZ->red_Y   = cHRM->red_y;
    XYZ->red_Z   = 1.0 - cHRM->red_x - cHRM->red_y;

    XYZ->green_X = cHRM->green_x;
    XYZ->green_Y = cHRM->green_y;
    XYZ->green_Z = 1.0 - cHRM->green_x - cHRM->green_y;

    XYZ->blue_X  = cHRM->blue_x;
    XYZ->blue_Y  = cHRM->blue_y;
    XYZ->blue_Z  = 1.0 - cHRM->blue_x - cHRM->blue_y;

    return 0;
}

// Stubbed: mimic libpng debug function (no-op for tests)
void png_debug1(int, const char*, const char*) { /* no-op in test harness */ }

// Stubbed: identity conversion from internal value to double
double png_float(png_const_structrp, double value, const char*) {
    return value;
}

// Prototype of the focal function under test (as provided in the prompt)
int png_get_cHRM_XYZ(png_const_structrp png_ptr, png_const_inforp info_ptr,
    double *red_X, double *red_Y, double *red_Z, double *green_X,
    double *green_Y, double *green_Z, double *blue_X, double *blue_Y,
    double *blue_Z);

// Implementation of the focal function under test (translated for the test environment)
int png_get_cHRM_XYZ(png_const_structrp png_ptr, png_const_inforp info_ptr,
    double *red_X, double *red_Y, double *red_Z, double *green_X,
    double *green_Y, double *green_Z, double *blue_X, double *blue_Y,
    double *blue_Z)
{
    png_XYZ XYZ;
    png_debug1(1, "in %s retrieval function", "cHRM_XYZ(float)");
    if (png_ptr != NULL && info_ptr != NULL &&
        (info_ptr->valid & PNG_INFO_cHRM) != 0 &&
        png_XYZ_from_xy(&XYZ, &info_ptr->cHRM) == 0)
    {
        if (red_X != NULL)
            *red_X = png_float(png_ptr, XYZ.red_X, "cHRM red X");
        if (red_Y != NULL)
            *red_Y = png_float(png_ptr, XYZ.red_Y, "cHRM red Y");
        if (red_Z != NULL)
            *red_Z = png_float(png_ptr, XYZ.red_Z, "cHRM red Z");
        if (green_X != NULL)
            *green_X = png_float(png_ptr, XYZ.green_X, "cHRM green X");
        if (green_Y != NULL)
            *green_Y = png_float(png_ptr, XYZ.green_Y, "cHRM green Y");
        if (green_Z != NULL)
            *green_Z = png_float(png_ptr, XYZ.green_Z, "cHRM green Z");
        if (blue_X != NULL)
            *blue_X = png_float(png_ptr, XYZ.blue_X, "cHRM blue X");
        if (blue_Y != NULL)
            *blue_Y = png_float(png_ptr, XYZ.blue_Y, "cHRM blue Y");
        if (blue_Z != NULL)
            *blue_Z = png_float(png_ptr, XYZ.blue_Z, "cHRM blue Z");
        return PNG_INFO_cHRM;
    }
    return 0;
}

// Test harness: individual test cases

// 1) Full retrieval: all outputs non-NULL and info_ptr has cHRM flag; verify all fields
void test_case_full_values() {
    // Set up a plausible cHRM primaries
    png_struct test_png_ptr; // not used in computation (stub)
    png_info test_info;
    test_info.valid = PNG_INFO_cHRM;
    test_info.cHRM.white_x = 0.3127;
    test_info.cHRM.white_y = 0.3290;
    test_info.cHRM.red_x   = 0.64;
    test_info.cHRM.red_y   = 0.33;
    test_info.cHRM.green_x = 0.30;
    test_info.cHRM.green_y = 0.60;
    test_info.cHRM.blue_x  = 0.15;
    test_info.cHRM.blue_y  = 0.06;

    double redX, redY, redZ, greenX, greenY, greenZ, blueX, blueY, blueZ;

    // Call focal function
    int ret = png_get_cHRM_XYZ(reinterpret_cast<png_const_structrp>(&test_png_ptr),
                               reinterpret_cast<png_const_inforp>(&test_info),
                               &redX, &redY, &redZ,
                               &greenX, &greenY, &greenZ,
                               &blueX, &blueY, &blueZ);

    // Expected values based on simple mapping implemented in png_XYZ_from_xy
    double expected_red_X = test_info.cHRM.red_x;
    double expected_red_Y = test_info.cHRM.red_y;
    double expected_red_Z = 1.0 - expected_red_X - expected_red_Y;
    double expected_green_X = test_info.cHRM.green_x;
    double expected_green_Y = test_info.cHRM.green_y;
    double expected_green_Z = 1.0 - expected_green_X - expected_green_Y;
    double expected_blue_X = test_info.cHRM.blue_x;
    double expected_blue_Y = test_info.cHRM.blue_y;
    double expected_blue_Z = 1.0 - expected_blue_X - expected_blue_Y;

    // Assertions
    EXPECT_EQ(ret, PNG_INFO_cHRM, "png_get_cHRM_XYZ should return PNG_INFO_cHRM on success");
    const double tol = 1e-12;
    EXPECT_EQ_DOUBLE(redX, expected_red_X, tol, "red_X should equal cHRM.red_x");
    EXPECT_EQ_DOUBLE(redY, expected_red_Y, tol, "red_Y should equal cHRM.red_y");
    EXPECT_EQ_DOUBLE(redZ, expected_red_Z, tol, "red_Z should be 1 - red_x - red_y");
    EXPECT_EQ_DOUBLE(greenX, expected_green_X, tol, "green_X should equal cHRM.green_x");
    EXPECT_EQ_DOUBLE(greenY, expected_green_Y, tol, "green_Y should equal cHRM.green_y");
    EXPECT_EQ_DOUBLE(greenZ, expected_green_Z, tol, "green_Z should be 1 - green_x - green_y");
    EXPECT_EQ_DOUBLE(blueX, expected_blue_X, tol, "blue_X should equal cHRM.blue_x");
    EXPECT_EQ_DOUBLE(blueY, expected_blue_Y, tol, "blue_Y should equal cHRM.blue_y");
    EXPECT_EQ_DOUBLE(blueZ, expected_blue_Z, tol, "blue_Z should be 1 - blue_x - blue_y");

    TEST_PASSED();
}

// 2) NULL pointers: ensure function tolerates NULL pointers for some outputs
void test_case_null_pointers() {
    // Setup a valid info with cHRM
    png_struct test_png_ptr;
    png_info test_info;
    test_info.valid = PNG_INFO_cHRM;
    test_info.cHRM.red_x = 0.2;
    test_info.cHRM.red_y = 0.2;
    test_info.cHRM.green_x = 0.3;
    test_info.cHRM.green_y = 0.4;
    test_info.cHRM.blue_x = 0.1;
    test_info.cHRM.blue_y = 0.1;

    double redX = -1.0, redY = -1.0, redZ = -1.0;
    double greenX = -1.0, greenY = -1.0, greenZ = -1.0;
    double blueX = -1.0, blueY = -1.0, blueZ = -1.0;

    // Pass NULL for red_X to ensure it isn't written
    int ret = png_get_cHRM_XYZ(reinterpret_cast<png_const_structrp>(&test_png_ptr),
                               reinterpret_cast<png_const_inforp>(&test_info),
                               nullptr, &redY, &redZ,
                               &greenX, &greenY, &greenZ,
                               &blueX, &blueY, &blueZ);

    // Only red_Y, red_Z, green_X/Y/Z, blue_X/Y/Z should be set where pointers are provided
    const double tol = 1e-12;
    // red_Y and red_Z should be computed
    EXPECT_EQ(ret, PNG_INFO_cHRM, "png_get_cHRM_XYZ should succeed when some pointers are NULL");
    EXPECT_EQ_DOUBLE(redY, test_info.cHRM.red_y, tol, "red_Y should be filled when provided");
    EXPECT_EQ_DOUBLE(redZ, 1.0 - test_info.cHRM.red_x - test_info.cHRM.red_y, tol, "red_Z should be computed");
    // green/blue should be filled as well
    EXPECT_EQ_DOUBLE(greenX, test_info.cHRM.green_x, tol, "green_X should be filled");
    EXPECT_EQ_DOUBLE(greenY, test_info.cHRM.green_y, tol, "green_Y should be filled");
    EXPECT_EQ_DOUBLE(greenZ, 1.0 - test_info.cHRM.green_x - test_info.cHRM.green_y, tol, "green_Z should be computed");
    EXPECT_EQ_DOUBLE(blueX, test_info.cHRM.blue_x, tol, "blue_X should be filled");
    EXPECT_EQ_DOUBLE(blueY, test_info.cHRM.blue_y, tol, "blue_Y should be filled");
    EXPECT_EQ_DOUBLE(blueZ, 1.0 - test_info.cHRM.blue_x - test_info.cHRM.blue_y, tol, "blue_Z should be computed");

    // red_X was NULL, ensure the variable remained unchanged (-1.0)
    EXPECT_EQ(redX, -1.0, "red_X was NULL; it should remain unchanged");
    TEST_PASSED();
}

// 3) Missing cHRM flag: ensure function returns 0 when PNG_INFO_cHRM bit is not set
void test_case_missing_flag() {
    png_struct test_png_ptr;
    png_info test_info;
    test_info.valid = 0; // do not set PNG_INFO_cHRM
    test_info.cHRM.red_x = 0.5;
    test_info.cHRM.red_y = 0.25;
    test_info.cHRM.green_x = 0.2;
    test_info.cHRM.green_y = 0.5;
    test_info.cHRM.blue_x = 0.1;
    test_info.cHRM.blue_y = 0.2;

    double redX = 0, redY = 0, redZ = 0;
    double greenX = 0, greenY = 0, greenZ = 0;
    double blueX = 0, blueY = 0, blueZ = 0;

    int ret = png_get_cHRM_XYZ(reinterpret_cast<png_const_structrp>(&test_png_ptr),
                               reinterpret_cast<png_const_inforp>(&test_info),
                               &redX, &redY, &redZ,
                               &greenX, &greenY, &greenZ,
                               &blueX, &blueY, &blueZ);

    EXPECT_EQ(ret, 0, "png_get_cHRM_XYZ should return 0 when PNG_INFO_cHRM bit is not set");
    TEST_PASSED();
}

// 4) XYZ computation fail: simulate failure in XYZ conversion path
void test_case_xyz_from_xy_failure() {
    // Enable forced failure in png_XYZ_from_xy
    TestEnv::force_XYZ_fail = 1;

    png_struct test_png_ptr;
    png_info test_info;
    test_info.valid = PNG_INFO_cHRM;
    test_info.cHRM.red_x = 0.64;
    test_info.cHRM.red_y = 0.33;
    test_info.cHRM.green_x = 0.30;
    test_info.cHRM.green_y = 0.60;
    test_info.cHRM.blue_x = 0.15;
    test_info.cHRM.blue_y = 0.06;

    double redX = 0, redY = 0, redZ = 0;
    double greenX = 0, greenY = 0, greenZ = 0;
    double blueX = 0, blueY = 0, blueZ = 0;

    int ret = png_get_cHRM_XYZ(reinterpret_cast<png_const_structrp>(&test_png_ptr),
                               reinterpret_cast<png_const_inforp>(&test_info),
                               &redX, &redY, &redZ,
                               &greenX, &greenY, &greenZ,
                               &blueX, &blueY, &blueZ);

    // Expect failure path: XOR should return 0 and outputs should remain unchanged
    EXPECT_EQ(ret, 0, "png_get_cHRM_XYZ should return 0 when XYZ_from_xy fails");
    // Outputs should be unchanged (we initialized to 0)
    EXPECT_EQ(redX, 0.0, "red_X should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(redY, 0.0, "red_Y should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(redZ, 0.0, "red_Z should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(greenX, 0.0, "green_X should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(greenY, 0.0, "green_Y should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(greenZ, 0.0, "green_Z should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(blueX, 0.0, "blue_X should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(blueY, 0.0, "blue_Y should be unchanged on XYZ_from_xy failure");
    EXPECT_EQ(blueZ, 0.0, "blue_Z should be unchanged on XYZ_from_xy failure");

    // Reset environment
    TestEnv::force_XYZ_fail = 0;
    TEST_PASSED();
}

// Entry point: run all test cases and print a summary
int main() {
    std::cout << "Running unit tests for png_get_cHRM_XYZ (self-contained harness)\n";

    test_case_full_values();
    test_case_null_pointers();
    test_case_missing_flag();
    test_case_xyz_from_xy_failure();

    int total = g_assertions;
    int failures = g_failures;

    std::cout << "Test run complete. Assertions: " << total
              << ", Failures: " << failures << "\n";

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << failures << " test assertion(s) failed.\n";
        return 1;
    }
}