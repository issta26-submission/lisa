// This test suite targets the focal function: png_get_pixel_aspect_ratio
// from the provided focal method in pngget.c.
// It exercises true/false branches of the internal predicate and returns
// the correct float value or 0.0f when conditions are not met.
// Notes:
// - We rely on libpng's public API to construct valid png_ptr and info_ptr.
// - We cover both scenarios when PNG_READ_pHYs_SUPPORTED is defined and when
//   it's not defined (the latter is exercised via a compile-time #else branch).
// - Tests are implemented in plain C++11 without Google Test. They use a small
//   custom assertion mechanism to avoid terminating the test run on failure.
// - All tests are wrapped in individual functions with explanatory comments.

#include <cmath>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>



// Domain knowledge alignment:
// - Candidate Keywords: pHYs flag, PNG_INFO_pHYs, x/y_pixels_per_unit, valid bits,
//   ratio calculation, NULL checks, macro-driven branches (PNG_READ_pHYs_SUPPORTED),
//   type-safe comparisons, libpng initialization helpers, resource cleanup.

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertion helpers
static void log_fail(const char* test_name, const char* message)
{
    ++g_failed_tests;
    std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
}

static void expect_float_near(const char* test_name, float actual, float expected, float eps = 1e-6f)
{
    if (std::fabs(actual - expected) > eps) {
        char buf[256];
        std::snprintf(buf, sizeof(buf), "expected %f, got %f (eps %f)", expected, actual, eps);
        log_fail(test_name, buf);
    }
}

static void expect_true(const char* test_name, bool cond, const char* message)
{
    if (!cond) {
        log_fail(test_name, message);
    }
}

// Test 1: PNG_READ_pHYs_SUPPORTED path - true ratio calculation when x != 0
static void test_png_get_pixel_aspect_ratio_basic_ratio()
{
    const char* test_name = "test_png_get_pixel_aspect_ratio_basic_ratio";

    // Create a valid libpng read struct and info struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_fail(test_name, "Failed to create png_struct (png_create_read_struct)");
        return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_fail(test_name, "Failed to create info_struct (png_create_info_struct)");
        return;
    }

    // Initialize info_ptr fields to simulate a pHYs chunk present with non-zero pixels per unit
    memset(info_ptr, 0, sizeof(png_info)); // zero all fields safely
    info_ptr->valid = PNG_INFO_pHYs;
    info_ptr->x_pixels_per_unit = 2; // non-zero
    info_ptr->y_pixels_per_unit = 6; // ratio = 6/2 = 3.0

    // Ensure png_ptr is non-NULL (required by the function to evaluate conditions)
    // Use a dummy but valid png_ptr from libpng
    // Note: We already created png_ptr above; it's valid for this test.

    // Call the focal function
    float ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);

    // Validate result
    expect_float_near(test_name, ratio, 3.0f);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 2: PNG_READ_pHYs_SUPPORTED path - x_pixels_per_unit is zero
static void test_png_get_pixel_aspect_ratio_zero_x()
{
    const char* test_name = "test_png_get_pixel_aspect_ratio_zero_x";

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_fail(test_name, "Failed to create png_struct");
        return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_fail(test_name, "Failed to create info_struct");
        return;
    }

    memset(info_ptr, 0, sizeof(png_info));
    info_ptr->valid = PNG_INFO_pHYs;
    info_ptr->x_pixels_per_unit = 0; // zero triggers 0.0 return
    info_ptr->y_pixels_per_unit = 123; // arbitrary

    float ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);

    expect_float_near(test_name, ratio, 0.0f);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 3: PNG_READ_pHYs_SUPPORTED path - valid flag not set
static void test_png_get_pixel_aspect_ratio_invalid_flag()
{
    const char* test_name = "test_png_get_pixel_aspect_ratio_invalid_flag";

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_fail(test_name, "Failed to create png_struct");
        return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_fail(test_name, "Failed to create info_struct");
        return;
    }

    memset(info_ptr, 0, sizeof(png_info));
    // Do not set PNG_INFO_pHYs in valid bits
    info_ptr->valid = 0;
    info_ptr->x_pixels_per_unit = 4;
    info_ptr->y_pixels_per_unit = 8;

    float ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);

    expect_float_near(test_name, ratio, 0.0f);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 4: PNG_READ_pHYs_SUPPORTED path - NULL pointer handling
static void test_png_get_pixel_aspect_ratio_null_pointers()
{
    const char* test_name = "test_png_get_pixel_aspect_ratio_null_pointers";

    // NULL png_ptr with valid info_ptr should yield 0.0
    {
        png_structp dummy_png_ptr = nullptr;
        png_infop info_ptr = (png_infop)png_create_info_struct(reinterpret_cast<png_structp>(dummy_png_ptr));
        // If allocation failed, early exit
        if (info_ptr == NULL) {
            log_fail(test_name, "Failed to create dummy info_struct for NULL-ptr test");
            return;
        }
        // Ensure we have a valid info_ptr; even though png_ptr is NULL, function should return 0.0
        info_ptr->valid = PNG_INFO_pHYs;
        info_ptr->x_pixels_per_unit = 2;
        info_ptr->y_pixels_per_unit = 6;

        float ratio = png_get_pixel_aspect_ratio(nullptr, info_ptr);
        expect_float_near(test_name, ratio, 0.0f);
        png_destroy_read_struct(&dummy_png_ptr, &info_ptr, NULL);
    }

    // NULL info_ptr with non-NULL png_ptr should yield 0.0
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            log_fail(test_name, "Failed to create png_ptr for NULL-info test");
            return;
        }
        float ratio = png_get_pixel_aspect_ratio(png_ptr, nullptr);
        expect_float_near(test_name, ratio, 0.0f);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }
}

// Test 5: When PNG_READ_pHYs_SUPPORTED is not defined, the function returns 0.0
#ifdef PNG_READ_pHYs_SUPPORTED
// If this macro is defined, tests above cover the branch. No-op here.
#else
static void test_png_get_pixel_aspect_ratio_macro_not_defined()
{
    const char* test_name = "test_png_get_pixel_aspect_ratio_macro_not_defined";

    // We simulate a scenario where pHYs is ignored due to macro not defined.
    // Construct a valid info_ptr with pHYs fields, but the function should return 0.0.

    // Initialize dummy libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_fail(test_name, "Failed to create png_ptr");
        return;
    }
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_fail(test_name, "Failed to create info_ptr");
        return;
    }

    memset(info_ptr, 0, sizeof(png_info));
    info_ptr->valid = PNG_INFO_pHYs;
    info_ptr->x_pixels_per_unit = 2;
    info_ptr->y_pixels_per_unit = 6;

    float ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);

    // In this mode, function should be 0.0
    expect_float_near(test_name, ratio, 0.0f);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}
#endif

static void run_all_tests()
{
    ++g_total_tests;
    test_png_get_pixel_aspect_ratio_basic_ratio();
    ++g_total_tests;
    test_png_get_pixel_aspect_ratio_zero_x();
    ++g_total_tests;
    test_png_get_pixel_aspect_ratio_invalid_flag();
    ++g_total_tests;
    test_png_get_pixel_aspect_ratio_null_pointers();

#ifdef PNG_READ_pHYs_SUPPORTED
    // If supported, ensure not compiled as a no-op for this test.
#else
    ++g_total_tests;
    test_png_get_pixel_aspect_ratio_macro_not_defined();
#endif
}

int main()
{
    run_all_tests();

    std::cout << "Test results: " << g_total_tests << " run, "
              << g_failed_tests << " failed." << std::endl;

    if (g_failed_tests != 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}