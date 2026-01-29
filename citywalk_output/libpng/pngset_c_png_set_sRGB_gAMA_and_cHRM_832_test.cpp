// Minimal C++11 unit test suite for libpng based function
// Focused on testing png_set_sRGB_gAMA_and_cHRM(png_const_structrp png_ptr, png_inforp info_ptr, int srgb_intent)
// This test does not rely on GTest; it uses lightweight ASSERT-like macros and a small harness.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstdio>
#include <png.h>


extern "C" {
}

// Lightweight, non-terminating assertion macro.
// On failure, increments global counter and prints diagnostic message.
static int g_test_failures = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            ++g_test_failures; \
            std::cerr << "ASSERT FAILED: " << (msg) \
                      << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        } \
    } while (0)


// Helper to convert potentially missing libpng features into test-skip decisions.
static bool is_gAMA_supported() {
#ifdef PNG_gAMA_SUPPORTED
    return true;
#else
    return false;
#endif
}

static bool is_cHRM_supported() {
#ifdef PNG_cHRM_SUPPORTED
    return true;
#else
    return false;
#endif
}

// Test 1: Null branch protection.
// The focal function should return early when either png_ptr or info_ptr is NULL.
// We create a valid png_ptr and a NULL info_ptr to exercise the early return path.
static void test_png_set_sRGB_gAMA_and_cHRM_null_ptr_handling() {
    // Create a real png_ptr to exercise the early return path safely.
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "WARNING: Could not create png_ptr for null-pointer test; skipping test_png_set_sRGB_gAMA_and_cHRM_null_ptr_handling." << std::endl;
        return;
    }

    // Provide a NULL info_ptr to trigger the early return branch.
    png_inforp info_ptr = NULL;

    // Call with NULL info_ptr; function should return without crashing.
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, 0);

    // Clean up
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

    // If we reached here, the null-pointer branch did not crash.
    ASSERT_TRUE(true, "png_set_sRGB_gAMA_and_cHRM gracefully handles NULL info_ptr without crashing");
}


// Test 2: Full storage function behavior for sRGB gamma and cHRM/cHRM XYZ (when supported).
// Steps:
// - Create valid png_ptr and info_ptr via libpng API.
// - Call png_set_sRGB_gAMA_and_cHRM with srgb_intent = 0 (and then 1 to exercise diversity if desired).
// - Verify that sRGB intent is stored via png_get_sRGB.
// - If PNG_gAMA_SUPPORTED is defined, verify gAMA is set to PNG_GAMMA_sRGB_INVERSE via png_get_gAMA_fixed.
// - If PNG_cHRM_SUPPORTED is defined, verify fixed cHRM values via png_get_cHRM_fixed.
// This test covers true branches of each predicate in the focal function.
static void test_png_set_sRGB_gAMA_and_cHRM_basic_behavior() {
    // First attempt with srgb_intent = 0
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr == NULL) {
            std::cerr << "ERROR: Failed to create png_ptr for test_png_set_sRGB_gAMA_and_cHRM_basic_behavior." << std::endl;
            ++g_test_failures;
            return;
        }
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            std::cerr << "ERROR: Failed to create info_ptr for test_png_set_sRGB_gAMA_and_cHRM_basic_behavior." << std::endl;
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            ++g_test_failures;
            return;
        }

        // Execute the focal function
        const int srgb_intent_0 = 0;
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent_0);

        // Retrieve and verify sRGB intent
        int got_intent = -1;
        png_get_sRGB(png_ptr, info_ptr, &got_intent);
        ASSERT_TRUE(got_intent == srgb_intent_0,
                    "sRGB intent should be stored as 0 after first call");

        // gAMA check (only if supported)
#ifdef PNG_gAMA_SUPPORTED
        {
            png_fixed_point gamma = 0;
            png_get_gAMA_fixed(png_ptr, info_ptr, &gamma);
            ASSERT_TRUE(gamma == PNG_GAMMA_sRGB_INVERSE,
                        "gAMA should be PNG_GAMMA_sRGB_INVERSE for sRGB in test");
        }
#else
        // If not supported, skip with a warning-like message
        (void)0;
#endif

        // cHRM check (only if supported)
#ifdef PNG_cHRM_SUPPORTED
        {
            png_fixed_point w_x = 0, w_y = 0;
            png_fixed_point r_x = 0, r_y = 0;
            png_fixed_point g_x = 0, g_y = 0;
            png_fixed_point b_x = 0, b_y = 0;
            png_get_cHRM_fixed(png_ptr, info_ptr,
                               &w_x, &w_y, &r_x, &r_y,
                               &g_x, &g_y, &b_x, &b_y);
            ASSERT_TRUE(w_x == 31270 && w_y == 32900 &&
                        r_x == 64000 && r_y == 33000 &&
                        g_x == 30000 && g_y == 60000 &&
                        b_x == 15000 && b_y == 6000,
                        "cHRM fixed values should match the expected sRGB constants");
        }
#else
        (void)0;
#endif

        // Cleanup
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    // Second attempt with srgb_intent = 1 (if supported by the library)
    {
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (png_ptr == NULL) {
            std::cerr << "WARNING: Could not create png_ptr for second-subtest in test_png_set_sRGB_gAMA_and_cHRM_basic_behavior." << std::endl;
            return;
        }
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == NULL) {
            std::cerr << "WARNING: Could not create info_ptr for second-subtest in test_png_set_sRGB_gAMA_and_cHRM_basic_behavior." << std::endl;
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
            return;
        }

        const int srgb_intent_1 = 1;
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent_1);

        int got_intent = -1;
        png_get_sRGB(png_ptr, info_ptr, &got_intent);
        ASSERT_TRUE(got_intent == srgb_intent_1,
                    "sRGB intent should be stored as 1 after second call");

        // Gamma and cHRM checks (if supported)
#ifdef PNG_gAMA_SUPPORTED
        {
            png_fixed_point gamma = 0;
            png_get_gAMA_fixed(png_ptr, info_ptr, &gamma);
            ASSERT_TRUE(gamma == PNG_GAMMA_sRGB_INVERSE,
                        "gAMA should be PNG_GAMMA_sRGB_INVERSE for sRGB in second-subtest");
        }
#else
        (void)0;
#endif

#ifdef PNG_cHRM_SUPPORTED
        {
            png_fixed_point w_x = 0, w_y = 0;
            png_fixed_point r_x = 0, r_y = 0;
            png_fixed_point g_x = 0, g_y = 0;
            png_fixed_point b_x = 0, b_y = 0;
            png_get_cHRM_fixed(png_ptr, info_ptr,
                               &w_x, &w_y, &r_x, &r_y,
                               &g_x, &g_y, &b_x, &b_y);
            // The expected values are identical to the ones used in the focal method.
            ASSERT_TRUE(w_x == 31270 && w_y == 32900 &&
                        r_x == 64000 && r_y == 33000 &&
                        g_x == 30000 && g_y == 60000 &&
                        b_x == 15000 && b_y == 6000,
                        "cHRM fixed values should match the expected sRGB constants for second subtest");
        }
#else
        (void)0;
#endif

        // Cleanup
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}


// Main harness: runs tests and reports summary.
// The design follows the non-terminating assertion requirement: tests run to completion even if failures occur.
int main() {
    std::cout << "Starting tests for png_set_sRGB_gAMA_and_cHRM (libpng integration)" << std::endl;

    test_png_set_sRGB_gAMA_and_cHRM_null_ptr_handling();
    test_png_set_sRGB_gAMA_and_cHRM_basic_behavior();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_test_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}