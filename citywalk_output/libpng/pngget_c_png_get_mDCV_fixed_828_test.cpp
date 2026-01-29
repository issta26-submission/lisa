// C++11 test suite for libpng's png_get_mDCV_fixed
// This test uses the public libpng interfaces (png.h) to exercise
// the focal function png_get_mDCV_fixed in a runtime environment.
// The tests do not rely on any testing framework (GTest, etc.) and
// use simple non-terminating assertions to maximize code coverage.

#include <cstdint>
#include <cassert>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <png.h>

extern "C" {
}

// Simple lightweight test harness (non-terminating assertions)
static int g_test_counter = 0;
static int g_failures = 0;
#define TEST_START() ++g_test_counter;
#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "Test " << g_test_counter << " FAILED: " << msg << std::endl; \
            ++g_failures; \
        } \
    } while(false)

// Test 1: png_ptr NULL should return 0
// Rationale: true branch requires both png_ptr and info_ptr to be non-NULL.
void test_png_get_mDCV_fixed_null_png_ptr() {
    TEST_START();
    png_fixed_point white_x = 0, white_y = 0;
    png_fixed_point red_x = 0, red_y = 0;
    png_fixed_point green_x = 0, green_y = 0;
    png_fixed_point blue_x = 0, blue_y = 0;
    png_uint_32 maxDL = 0, minDL = 0;
    int ret = png_get_mDCV_fixed(NULL, NULL,
                                &white_x, &white_y,
                                &red_x, &red_y,
                                &green_x, &green_y,
                                &blue_x, &blue_y,
                                &maxDL, &minDL);
    TEST_ASSERT(ret == 0, "Expected 0 return when png_ptr is NULL");
}

// Test 2: info_ptr NULL should return 0
void test_png_get_mDCV_fixed_null_info_ptr() {
    TEST_START();
    // Create a minimal png_ptr to pass the first non-NULL requirement
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    TEST_ASSERT(png_ptr != NULL, "Failed to create png_ptr for test 2");
    png_fixed_point white_x = 0;
    int ret = png_get_mDCV_fixed(png_ptr, NULL,
                                &white_x, NULL,
                                NULL, NULL,
                                NULL, NULL,
                                NULL, NULL,
                                NULL, NULL);
    TEST_ASSERT(ret == 0, "Expected 0 return when info_ptr is NULL");
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 3: info_ptr valid flag not set should return 0
void test_png_get_mDCV_fixed_info_ptr_without_flag() {
    TEST_START();
    // Prepare png_ptr and info_ptr
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    TEST_ASSERT(png_ptr != NULL, "Failed to create png_ptr for test 3");
    png_infop info_ptr = png_create_info_struct(png_ptr);
    TEST_ASSERT(info_ptr != NULL, "Failed to create info_ptr for test 3");

    // Ensure PNG_INFO_mDCV flag is not set
    ((png_info*)info_ptr)->valid = 0;

    png_fixed_point w = 0;
    png_fixed_point h = 0;
    int ret = png_get_mDCV_fixed(png_ptr, info_ptr, &w, &h,
                                NULL, NULL,
                                NULL, NULL,
                                NULL, NULL,
                                NULL, NULL);
    TEST_ASSERT(ret == 0, "Expected 0 return when PNG_INFO_mDCV not set in info_ptr");

    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 4: Full retrieval when all fields are present
void test_png_get_mDCV_fixed_all_fields() {
    TEST_START();
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    TEST_ASSERT(png_ptr != NULL, "Failed to create png_ptr for test 4");
    png_infop info_ptr = png_create_info_struct(png_ptr);
    TEST_ASSERT(info_ptr != NULL, "Failed to create info_ptr for test 4");

    // Enable mDCV retrieval
    ((png_info*)info_ptr)->valid |= PNG_INFO_mDCV;

    // Populate master display values (these are in fixed-point representation)
    // The function multiplies by 2, so expected outputs are 2x these values.
    ((png_info*)info_ptr)->mastering_white_x = 1;
    ((png_info*)info_ptr)->mastering_white_y = 2;
    ((png_info*)info_ptr)->mastering_red_x   = 3;
    ((png_info*)info_ptr)->mastering_red_y   = 4;
    ((png_info*)info_ptr)->mastering_green_x = 5;
    ((png_info*)info_ptr)->mastering_green_y = 6;
    ((png_info*)info_ptr)->mastering_blue_x  = 7;
    ((png_info*)info_ptr)->mastering_blue_y  = 8;
    ((png_info*)info_ptr)->mastering_maxDL     = 1000;
    ((png_info*)info_ptr)->mastering_minDL     = 100;

    png_fixed_point white_x = 0, white_y = 0;
    png_fixed_point red_x  = 0, red_y   = 0;
    png_fixed_point green_x= 0, green_y = 0;
    png_fixed_point blue_x = 0, blue_y  = 0;
    png_uint_32 maxDL = 0, minDL = 0;

    int ret = png_get_mDCV_fixed(png_ptr, info_ptr,
                                &white_x, &white_y,
                                &red_x, &red_y,
                                &green_x, &green_y,
                                &blue_x, &blue_y,
                                &maxDL, &minDL);

    TEST_ASSERT(ret == PNG_INFO_mDCV, "Return should indicate PNG_INFO_mDCV is set");
    TEST_ASSERT(white_x == (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_white_x),
                "white_x should be twice mastering_white_x");
    TEST_ASSERT(white_y == (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_white_y),
                "white_y should be twice mastering_white_y");
    TEST_ASSERT(red_x  == (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_red_x),
                "red_x should be twice mastering_red_x");
    TEST_ASSERT(red_y  == (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_red_y),
                "red_y should be twice mastering_red_y");
    TEST_ASSERT(green_x== (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_green_x),
                "green_x should be twice mastering_green_x");
    TEST_ASSERT(green_y== (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_green_y),
                "green_y should be twice mastering_green_y");
    TEST_ASSERT(blue_x == (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_blue_x),
                "blue_x should be twice mastering_blue_x");
    TEST_ASSERT(blue_y == (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_blue_y),
                "blue_y should be twice mastering_blue_y");
    TEST_ASSERT(maxDL == ((png_info*)info_ptr)->mastering_maxDL,
                "mastering_maxDL should match info_ptr value");
    TEST_ASSERT(minDL == ((png_info*)info_ptr)->mastering_minDL,
                "mastering_minDL should match info_ptr value");

    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 5: Partial pointers - only some outputs are requested
void test_png_get_mDCV_fixed_partial_pointers() {
    TEST_START();
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    TEST_ASSERT(png_ptr != NULL, "Failed to create png_ptr for test 5");
    png_infop info_ptr = png_create_info_struct(png_ptr);
    TEST_ASSERT(info_ptr != NULL, "Failed to create info_ptr for test 5");

    // Enable mDCV retrieval
    ((png_info*)info_ptr)->valid |= PNG_INFO_mDCV;

    // Populate fixed master values
    ((png_info*)info_ptr)->mastering_white_x = 2;
    ((png_info*)info_ptr)->mastering_white_y = 4;
    ((png_info*)info_ptr)->mastering_maxDL = 500;
    ((png_info*)info_ptr)->mastering_minDL = 50;

    // Only white_x and mastering_minDL/non-NULL pointers will be updated
    png_fixed_point white_x_out = 0;
    png_uint_32 maxDL_out = 0;
    // Prepare to check that other pointers are not touched by not providing them
    png_fixed_point dummy1 = -1, dummy2 = -2, dummy3 = -3, dummy4 = -4;
    png_fixed_point white_y_out = dummy1;
    png_fixed_point red_x_out = dummy2, red_y_out = dummy3;
    png_fixed_point green_x_out = dummy4, green_y_out = dummy4;
    png_fixed_point blue_x_out = dummy4, blue_y_out = dummy4;

    int ret = png_get_mDCV_fixed(png_ptr, info_ptr,
                                &white_x_out, NULL, // only white_x_out requested
                                NULL, NULL,
                                NULL, NULL,
                                NULL, NULL,
                                &maxDL_out, NULL); // maxDL_out requested
    TEST_ASSERT(ret == PNG_INFO_mDCV, "Return should indicate PNG_INFO_mDCV is set (partial test)");
    TEST_ASSERT(white_x_out == (png_fixed_point)(2 * ((png_info*)info_ptr)->mastering_white_x),
                "white_x_out should be updated to 2 * mastering_white_x");
    TEST_ASSERT(maxDL_out == ((png_info*)info_ptr)->mastering_maxDL,
                "mastering_maxDL_out should match info_ptr value");
    // Other outputs should remain unchanged (sentinel values)
    TEST_ASSERT(white_y_out == dummy1, "white_y_out should remain unchanged when not requested");
    TEST_ASSERT(red_x_out == dummy2 && red_y_out == dummy3 &&
                green_x_out == dummy4 && green_y_out == dummy4 &&
                blue_x_out == dummy4 && blue_y_out == dummy4,
                "All non-requested outputs should remain unchanged");

    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Main runner
int main() {
    std::cout << "Starting png_get_mDCV_fixed test suite (C++11, non-GTest)..." << std::endl;

    // Run tests
    test_png_get_mDCV_fixed_null_png_ptr();
    test_png_get_mDCV_fixed_null_info_ptr();
    test_png_get_mDCV_fixed_info_ptr_without_flag();
    test_png_get_mDCV_fixed_all_fields();
    test_png_get_mDCV_fixed_partial_pointers();

    std::cout << "Tests completed. Total tests: " << g_test_counter
              << ", Failures: " << g_failures << std::endl;

    // If any failure, indicate non-zero exit code for CI friendliness
    return (g_failures == 0) ? 0 : 1;
}