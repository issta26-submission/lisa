/*
Unit test suite for png_get_cHRM_XYZ_fixed (C++11, no Google Test)

Notes:
- Uses actual libpng API to exercise real integration paths.
- Focuses on true/false branches of conditional checks inside png_get_cHRM_XYZ_fixed:
  - NULL pointers
  - info_ptr validity (PNG_INFO_cHRM flag)
  - successful path with full and partial output pointers
- Test structure avoids terminating assertions; uses a lightweight EXPECT-style macros.
- Test entry point is main() and tests are invoked in sequence.
- All tests are self-contained and rely only on standard library and libpng.
*/

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <png.h>



static int g_test_failures = 0;

// Lightweight non-terminating test assertions
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        ++g_test_failures; \
        std::cerr << "Expectation failed: " << __FILE__ << ":" << __LINE__ \
                  << " (" << #cond << ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    auto _a = (a); auto _b = (b); \
    if(!(_a == _b)) { \
        ++g_test_failures; \
        std::cerr << "Expectation failed: " << __FILE__ << ":" << __LINE__ \
                  << " (" << #a << " == " << #b << ") " \
                  << " => " << _a << " != " << _b << std::endl; \
    } \
} while(0)

// Error/warning handlers for libpng (avoid terminating on errors)
extern "C" void png_user_error(png_structp, png_const_charp);
extern "C" void png_user_warning(png_structp, png_const_charp);

extern "C" void png_user_error(png_structp, png_const_charp) {
    std::cerr << "libpng error encountered during test." << std::endl;
    std::exit(1);
}
extern "C" void png_user_warning(png_structp, png_const_charp) {
    // Treat warnings as non-fatal; log and continue
    std::cerr << "libpng warning encountered during test." << std::endl;
}

/////////////////////////////////////////////////////////////////
// Test 1: Null pointers should short-circuit and return 0
/////////////////////////////////////////////////////////////////
static void test_png_get_cHRM_XYZ_fixed_null_ptrs() {
    // All pointers NULL
    png_fixed_point xr, yr, zr, xg, yg, zg, xb, yb, zb;
    png_uint_32 res = png_get_cHRM_XYZ_fixed(nullptr, nullptr,
                          &xr, &yr, &zr,
                          &xg, &yg, &zg,
                          &xb, &yb, &zb);
    EXPECT_EQ(res, 0u);

    // png_ptr non-null, info_ptr NULL
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, png_user_error, png_user_warning);
    png_infop info_ptr = nullptr;
    res = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                          &xr, &yr, &zr,
                          &xg, &yg, &zg,
                          &xb, &yb, &zb);
    EXPECT_EQ(res, 0u);

    // Cleanup
    if (png_ptr != nullptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp)nullptr, (png_infopp)nullptr);
    }
}

/////////////////////////////////////////////////////////////////
// Test 2: Full success path - all nine fixed outputs should be filled
/////////////////////////////////////////////////////////////////
static void test_png_get_cHRM_XYZ_fixed_full() {
    // Initialize libpng read struct and info struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, png_user_error, png_user_warning);
    EXPECT_TRUE(png_ptr != nullptr);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    EXPECT_TRUE(info_ptr != nullptr);

    // Set cHRM using known xy chromaticity values
    // Values chosen to be within valid range [0,1], typical sRGB-like primaries
    const double white_x = 0.3127, white_y = 0.3290;
    const double red_x   = 0.6400, red_y   = 0.3300;
    const double green_x = 0.3000, green_y = 0.6000;
    const double blue_x  = 0.1500, blue_y  = 0.0600;

    png_set_cHRM(png_ptr, info_ptr,
                 white_x, white_y,
                 red_x, red_y,
                 green_x, green_y,
                 blue_x, blue_y);

    // Retrieve double XYZ values for expected comparison
    double rx, ry, rz, gx, gy, gz, bx, by, bz;
    png_get_cHRM_XYZ(png_ptr, info_ptr,
                     &rx, &ry, &rz,
                     &gx, &gy, &gz,
                     &bx, &by, &bz);

    // Helper to convert double to fixed-point (scaled by 100000) with rounding
    auto to_fixed = [](double v) -> png_fixed_point {
        return static_cast<png_fixed_point>(llround(v * 100000.0));
    };

    const png_fixed_point exp_rx = to_fixed(rx);
    const png_fixed_point exp_ry = to_fixed(ry);
    const png_fixed_point exp_rz = to_fixed(rz);

    const png_fixed_point exp_gx = to_fixed(gx);
    const png_fixed_point exp_gy = to_fixed(gy);
    const png_fixed_point exp_gz = to_fixed(gz);

    const png_fixed_point exp_bx = to_fixed(bx);
    const png_fixed_point exp_by = to_fixed(by);
    const png_fixed_point exp_bz = to_fixed(bz);

    // Call the fixed XYZ extraction
    png_fixed_point fx_rX, fx_rY, fx_rZ;
    png_fixed_point fx_gX, fx_gY, fx_gZ;
    png_fixed_point fx_bX, fx_bY, fx_bZ;

    png_uint_32 info_ret = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                              &fx_rX, &fx_rY, &fx_rZ,
                              &fx_gX, &fx_gY, &fx_gZ,
                              &fx_bX, &fx_bY, &fx_bZ);

    // Validate success path and exact fixed-point values
    EXPECT_TRUE(info_ret != 0);

    EXPECT_EQ(fx_rX, exp_rx);
    EXPECT_EQ(fx_rY, exp_ry);
    EXPECT_EQ(fx_rZ, exp_rz);

    EXPECT_EQ(fx_gX, exp_gx);
    EXPECT_EQ(fx_gY, exp_gy);
    EXPECT_EQ(fx_gZ, exp_gz);

    EXPECT_EQ(fx_bX, exp_bx);
    EXPECT_EQ(fx_bY, exp_by);
    EXPECT_EQ(fx_bZ, exp_bz);

    // Cleanup
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infptr)nullptr);
    }
}

/////////////////////////////////////////////////////////////////
// Test 3: Partial pointer coverage - ensure non-NULL outputs can be selectively filled
/////////////////////////////////////////////////////////////////
static void test_png_get_cHRM_XYZ_fixed_partial_ptrs() {
    // Initialize
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, png_user_error, png_user_warning);
    EXPECT_TRUE(png_ptr != nullptr);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    EXPECT_TRUE(info_ptr != nullptr);

    const double white_x = 0.3127, white_y = 0.3290;
    const double red_x   = 0.6400, red_y   = 0.3300;
    const double green_x = 0.3000, green_y = 0.6000;
    const double blue_x  = 0.1500, blue_y  = 0.0600;

    png_set_cHRM(png_ptr, info_ptr,
                 white_x, white_y,
                 red_x, red_y,
                 green_x, green_y,
                 blue_x, blue_y);

    // Retrieve doubles for expected conversion
    double rx, ry, rz, gx, gy, gz, bx, by, bz;
    png_get_cHRM_XYZ(png_ptr, info_ptr,
                     &rx, &ry, &rz,
                     &gx, &gy, &gz,
                     &bx, &by, &bz);
    auto to_fixed = [](double v) -> png_fixed_point { return static_cast<png_fixed_point>(llround(v * 100000.0)); };

    const png_fixed_point exp_rx = to_fixed(rx);
    const png_fixed_point exp_ry = to_fixed(ry);
    const png_fixed_point exp_rz = to_fixed(rz);
    const png_fixed_point exp_gy = to_fixed(gy);
    const png_fixed_point exp_bx = to_fixed(bx);
    const png_fixed_point exp_by = to_fixed(by);
    const png_fixed_point exp_bz = to_fixed(bz);

    // Partial: only red_Y, green_Y, blue_Y outputs requested
    png_fixed_point prY, pgY, pbY;

    // Pass NULL for non-requested outputs
    png_uint_32 info_ret = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                               nullptr, &prY, nullptr,
                               nullptr, &pgY, nullptr,
                               nullptr, &pbY, nullptr);

    EXPECT_TRUE(info_ret != 0);
    // Validate the three provided fields
    EXPECT_EQ(prY, exp_ry);  // red_Y
    EXPECT_EQ(pgY, exp_gy);  // green_Y
    EXPECT_EQ(pbY, exp_by);  // blue_Y

    // Cleanup
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infptr)nullptr);
    }
}

/////////////////////////////////////////////////////////////////
// Test 4: Failure when PNG_INFO_cHRM bit is not set (no cHRM data)
/////////////////////////////////////////////////////////////////
static void test_png_get_cHRM_XYZ_fixed_no_cHRM_flag() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, png_user_error, png_user_warning);
    EXPECT_TRUE(png_ptr != nullptr);

    // Create info struct but do not set PNG_INFO_cHRM
    png_infop info_ptr = png_create_info_struct(png_ptr);
    EXPECT_TRUE(info_ptr != nullptr);

    // Do not set cHRM data; do not call png_set_cHRM

    png_fixed_point rx, ry, rz, gx, gy, gz, bx, by, bz;
    png_uint_32 res = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                          &rx, &ry, &rz,
                          &gx, &gy, &gz,
                          &bx, &by, &bz);
    EXPECT_EQ(res, 0u);

    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infptr)nullptr);
    }
}

/////////////////////////////////////////////////////////////////
// Test 5: Null info_ptr parameter case
/////////////////////////////////////////////////////////////////
static void test_png_get_cHRM_XYZ_fixed_null_info_ptr() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, png_user_error, png_user_warning);
    EXPECT_TRUE(png_ptr != nullptr);

    // Call with NULL info_ptr should return 0
    png_fixed_point rx, ry, rz, gx, gy, gz, bx, by, bz;
    png_uint_32 res = png_get_cHRM_XYZ_fixed(png_ptr, nullptr,
                          &rx, &ry, &rz,
                          &gx, &gy, &gz,
                          &bx, &by, &bz);
    EXPECT_EQ(res, 0u);

    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, (png_inf_ptr)nullptr, (png_inf_ptr)nullptr);
    }
}

/////////////////////////////////////////////////////////////////
// Test 6: Null png_ptr parameter case
/////////////////////////////////////////////////////////////////
static void test_png_get_cHRM_XYZ_fixed_null_png_ptr() {
    // info_ptr can be non-null if needed for the test; here we pass NULL png_ptr
    png_infop info_ptr = nullptr;

    png_fixed_point rx, ry, rz, gx, gy, gz, bx, by, bz;
    png_uint_32 res = png_get_cHRM_XYZ_fixed(nullptr, info_ptr,
                          &rx, &ry, &rz,
                          &gx, &gy, &gz,
                          &bx, &by, &bz);
    EXPECT_EQ(res, 0u);
}

/////////////////////////////////////////////////////////////////
// Utility: Run all tests
/////////////////////////////////////////////////////////////////
int main() {
    struct Test {
        const char* name;
        void (*func)();
    };

    Test tests[] = {
        {"Null pointers should short-circuit", test_png_get_cHRM_XYZ_fixed_null_ptrs},
        {"Full success path with all outputs filled", test_png_get_cHRM_XYZ_fixed_full},
        {"Partial pointer coverage (some outputs)", test_png_get_cHRM_XYZ_fixed_partial_ptrs},
        {"Failure when PNG_INFO_cHRM flag not set", test_png_get_cHRM_XYZ_fixed_no_cHRM_flag},
        {"Null info_ptr parameter", test_png_get_cHRM_XYZ_fixed_null_info_ptr},
        {"Null png_ptr parameter", test_png_get_cHRM_XYZ_fixed_null_png_ptr},
    };

    const size_t nTests = sizeof(tests) / sizeof(tests[0]);
    int total_failed = 0;

    std::cout << "Starting tests for png_get_cHRM_XYZ_fixed..." << std::endl;

    for (size_t i = 0; i < nTests; ++i) {
        // Reset per-test failure indicator by using a local snapshot
        int failures_before = g_test_failures;
        tests[i].func();
        int failures_after = g_test_failures;
        if (failures_after == failures_before) {
            std::cout << "[PASS] " << tests[i].name << std::endl;
        } else {
            std::cout << "[FAIL] " << tests[i].name << std::endl;
            total_failed++;
        }
    }

    if (total_failed > 0) {
        std::cout << total_failed << " test(s) FAILED." << std::endl;
    } else {
        std::cout << "All tests PASSED." << std::endl;
    }

    // Ensure libpng resources are fully cleaned up if any test left
    // (Tests already destroy_structs; nothing else needed here)

    return total_failed ? 1 : 0;
}