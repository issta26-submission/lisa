/*
 This test suite targets the focal function:
   png_get_gAMA_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
                     png_fixed_point *file_gamma)

 It exercises:
 - Null-pointer handling (both NULL, one NULL)
 - Info struct without PNG_INFO_gAMA
 - GAMA present with a known fixed-point gamma value
 - GAMA present but file_gamma pointer is NULL

 It uses libpng public API to construct valid png_ptr and info_ptr objects
 and to manipulate the gAMA state via png_set_gAMA.
 The tests are written without GoogleTest; a small, non-terminating assertion
 framework is embedded to accumulate failures and continue execution.
*/

#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <cstdio>
#include <png.h>


// Simple non-terminating test assertions (non-GTest style)
// Collect failures but continue execution to improve coverage
static int g_failure_count = 0;

#define EXPECT_EQ(a, b, descr) do { \
    if ((a) != (b)) { \
        std::cerr << "[FAIL] " << (descr) << " -> Expected: " << (b) << "  Got: " << (a) << "\n"; \
        ++g_failure_count; \
    } \
} while(0)

#define EXPECT_TRUE(cond, descr) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (descr) << "\n"; \
        ++g_failure_count; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr, descr) do { \
    if ((ptr) == NULL) { \
        std::cerr << "[FAIL] " << (descr) << "\n"; \
        ++g_failure_count; \
    } \
} while(0)

/*
 Test 1: Null-pointer handling
   - png_get_gAMA_fixed(NULL, NULL, &gamma) -> 0
   - png_get_gAMA_fixed(non-null png_ptr, NULL, &gamma) -> 0
   - png_get_gAMA_fixed(NULL, non-null info_ptr, &gamma) -> 0
*/
static void test_null_ptr_handling() {
    // Case 1: both pointers NULL
    {
        png_fixed_point gamma_tmp = 0;
        int ret = png_get_gAMA_fixed(NULL, NULL, &gamma_tmp);
        EXPECT_EQ(ret, 0, "png_get_gAMA_fixed(NULL, NULL, &gamma) should return 0");
    }

    // Case 2: png_ptr non-null, info_ptr NULL
    {
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        EXPECT_NOT_NULL(png_ptr, "png_create_read_struct failed in test_null_ptr_handling Case 2");

        int ret = png_get_gAMA_fixed(png_ptr, NULL, NULL);
        EXPECT_EQ(ret, 0, "png_get_gAMA_fixed(png_ptr, NULL, NULL) should return 0");

        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    // Case 3: png_ptr NULL, info_ptr non-null
    {
        // Have to create a valid info_ptr in order to pass a non-null pointer
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        EXPECT_NOT_NULL(png_ptr, "png_create_read_struct failed in test_null_ptr_handling Case 3");

        png_infop info_ptr = png_create_info_struct(png_ptr);
        EXPECT_NOT_NULL(info_ptr, "png_create_info_struct failed in test_null_ptr_handling Case 3");

        int ret = png_get_gAMA_fixed(NULL, info_ptr, NULL);
        EXPECT_EQ(ret, 0, "png_get_gAMA_fixed(NULL, info_ptr, NULL) should return 0");

        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }
}

/*
 Test 2: Info structure without gAMA
   - Ensure function returns 0 when PNG_INFO_gAMA not present in info_ptr->valid
*/
static void test_no_gAMA_bit_set() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    EXPECT_NOT_NULL(png_ptr, "png_create_read_struct failed in test_no_gAMA_bit_set");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    EXPECT_NOT_NULL(info_ptr, "png_create_info_struct failed in test_no_gAMA_bit_set");

    // Do not set gAMA; info_ptr->valid should not have PNG_INFO_gAMA bit
    png_fixed_point gamma_out = 0;
    int ret = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_out);
    EXPECT_EQ(ret, 0, "png_get_gAMA_fixed without gAMA should return 0");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

/*
 Test 3: GAMA present with known gamma, and a valid output pointer
   - gamma_out should be equal to the set gamma
   - return value should indicate PNG_INFO_gAMA
*/
static void test_gAMA_present_with_output() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    EXPECT_NOT_NULL(png_ptr, "png_create_read_struct failed in test_gAMA_present_with_output");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    EXPECT_NOT_NULL(info_ptr, "png_create_info_struct failed in test_gAMA_present_with_output");

    // Set a known gamma value using libpng API
    const png_fixed_point gamma_value = 45455; // typical 0.45455 in fixed-point
    png_set_gAMA(png_ptr, info_ptr, gamma_value);

    png_fixed_point gamma_out = 0;
    int ret = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_out);

    EXPECT_EQ(ret, PNG_INFO_gAMA, "png_get_gAMA_fixed should return PNG_INFO_gAMA when gAMA exists");
    EXPECT_EQ(gamma_out, gamma_value, "png_get_gAMA_fixed should return the exact gamma value set by png_set_gAMA");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

/*
 Test 4: GAMA present but file_gamma pointer is NULL
   - Function should return PNG_INFO_gAMA and not crash
*/
static void test_gAMA_present_null_output_ptr() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    EXPECT_NOT_NULL(png_ptr, "png_create_read_struct failed in test_gAMA_present_null_output_ptr");

    png_infop info_ptr = png_create_info_struct(png_ptr);
    EXPECT_NOT_NULL(info_ptr, "png_create_info_struct failed in test_gAMA_present_null_output_ptr");

    const png_fixed_point gamma_value = 50000; // 0.5 in fixed-point
    png_set_gAMA(png_ptr, info_ptr, gamma_value);

    int ret = png_get_gAMA_fixed(png_ptr, info_ptr, NULL);

    EXPECT_EQ(ret, PNG_INFO_gAMA, "png_get_gAMA_fixed should return PNG_INFO_gAMA even when file_gamma is NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

int main() {
    // Run tests
    test_null_ptr_handling();
    test_no_gAMA_bit_set();
    test_gAMA_present_with_output();
    test_gAMA_present_null_output_ptr();

    if (g_failure_count == 0) {
        std::cout << "[TEST] All tests passed for png_get_gAMA_fixed." << std::endl;
        return EXIT_SUCCESS;
    } else {
        std::cerr << "[TEST] " << g_failure_count
                  << " test(s) failed for png_get_gAMA_fixed." << std::endl;
        return EXIT_FAILURE;
    }
}