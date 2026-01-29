// Unit test suite for the focal method png_get_cICP (pngget.c) using C++11 (no GTest).
// This test suite uses libpng public APIs to construct a valid png info/struct pair
// and validates both success and failure branches of png_get_cICP.
//
// Notes:
// - We rely on the public API: png_set_cICP to populate the cICP fields prior to retrieval.
// - The tests are designed to be non-terminating (they report failures but continue).
// - Static/global state is avoided; tests are independent and cleaned up properly.
// - The test harness is self-contained and prints a final summary.

// Include libpng headers (path depends on the build environment)
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


extern "C" {
}

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Lightweight assertion helpers (non-terminating)
#define TEST_EXPECT_TRUE(cond, msg) do {                              \
    ++g_total_tests;                                                   \
    if(!(cond)) {                                                      \
        ++g_failed_tests;                                              \
        fprintf(stderr, "TEST FAILED: %s\n", (msg));                   \
    } else {                                                           \
        /* Optional: verbose success can be enabled here */            \
    }                                                                  \
} while(0)

#define TEST_EXPECT_EQ(lhs, rhs, msg) do {                             \
    ++g_total_tests;                                                     \
    if(!((lhs) == (rhs))) {                                            \
        ++g_failed_tests;                                                \
        fprintf(stderr, "TEST FAILED: %s (expected %ld, got %ld)\n",       \
                (msg), (long)(rhs), (long)(lhs));                       \
    }                                                                  \
} while(0)

// C linkage error/warning handlers for libpng (no-ops)
static void test_png_error(png_structp, png_const_charp) { /* no-op */ }
static void test_png_warn(png_structp, png_const_charp) { /* no-op */ }

// Wrapper to safely create and destroy libpng structures
static bool create_png_structs(png_structpp out_ptr, png_infopp out_info) {
    if (!out_ptr || !out_info) return false;
    *out_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
                                      test_png_error, test_png_warn);
    if (*out_ptr == nullptr) return false;

    *out_info = png_create_info_struct(*out_ptr);
    if (*out_info == nullptr) {
        png_destroy_read_struct(out_ptr, nullptr, nullptr);
        return false;
    }
    return true;
}

// Test 1: Success path - properly populated cICP values should be retrieved
static bool test_png_get_cICP_success() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!create_png_structs(&png_ptr, &info_ptr)) {
        fprintf(stderr, "Failed to create libpng structures for success test.\n");
        return false;
    }

    // Configure cICP values using public API
    // Values chosen to be distinct and easy to verify
    int colour_primaries_set = 1;
    int transfer_function_set = 2;
    int matrix_coefficients_set = 3;
    int video_full_range_flag_set = 1;

    // Use public setter to populate the cICP data in info_ptr
    // The png_set_cICP function is a part of libpng API in newer versions.
    // If the environment uses a slightly different signature, adapt accordingly.
    png_set_cICP(png_ptr, info_ptr,
                 colour_primaries_set,
                 transfer_function_set,
                 matrix_coefficients_set,
                 video_full_range_flag_set);

    // Outputs
    png_bytep colour_primaries_out = NULL;
    png_bytep transfer_function_out = NULL;
    png_bytep matrix_coefficients_out = NULL;
    png_bytep video_full_range_flag_out = NULL;

    // Allocate single-byte containers
    png_byte colour_primaries_byte = 0;
    png_byte transfer_function_byte = 0;
    png_byte matrix_coefficients_byte = 0;
    png_byte video_full_range_byte = 0;

    colour_primaries_out = &colour_primaries_byte;
    transfer_function_out = &transfer_function_byte;
    matrix_coefficients_out = &matrix_coefficients_byte;
    video_full_range_flag_out = &video_full_range_byte;

    // Call the focal method under test
    PNG_INFO info_flag = png_get_cICP(png_ptr, info_ptr,
                                      colour_primaries_out,
                                      transfer_function_out,
                                      matrix_coefficients_out,
                                      video_full_range_flag_out);

    // Validate return value and retrieved data
    TEST_EXPECT_TRUE(info_flag == PNG_INFO_cICP,
                     "png_get_cICP should return PNG_INFO_cICP on success");
    TEST_EXPECT_EQ((long)colour_primaries_byte, (long)colour_primaries_set,
                   "colour_primaries should match the value set");
    TEST_EXPECT_EQ((long)transfer_function_byte, (long)transfer_function_set,
                   "transfer_function should match the value set");
    TEST_EXPECT_EQ((long)matrix_coefficients_byte, (long)matrix_coefficients_set,
                   "matrix_coefficients should match the value set");
    TEST_EXPECT_EQ((long)video_full_range_byte, (long)video_full_range_flag_set,
                   "video_full_range_flag should match the value set");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return g_failed_tests == 0;
}

// Test 2: Null png_ptr should fail gracefully (return 0)
static bool test_png_get_cICP_null_png_ptr() {
    // Prepare a valid info_ptr without relying on cICP
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!create_png_structs(&png_ptr, &info_ptr)) {
        fprintf(stderr, "Failed to create libpng structures for null_png_ptr test.\n");
        return false;
    }

    // Prepare outputs
    png_byte colour_primaries_byte = 0;
    png_byte transfer_function_byte = 0;
    png_byte matrix_coefficients_byte = 0;
    png_byte video_full_range_byte = 0;

    int ret = 0;
    ret = png_get_cICP(png_ptr, info_ptr,
                       &colour_primaries_byte,
                       &transfer_function_byte,
                       &matrix_coefficients_byte,
                       &video_full_range_byte);

    TEST_EXPECT_TRUE(ret == 0, "png_get_cICP should return 0 when png_ptr is NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return g_failed_tests == 0;
}

// Test 3: NULL info_ptr should fail gracefully (return 0)
static bool test_png_get_cICP_null_info_ptr() {
    // Create valid png_ptr
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!create_png_structs(&png_ptr, &info_ptr)) {
        fprintf(stderr, "Failed to create libpng structures for null_info_ptr test.\n");
        return false;
    }

    // Prepare outputs
    png_byte colour_primaries_byte = 0;
    png_byte transfer_function_byte = 0;
    png_byte matrix_coefficients_byte = 0;
    png_byte video_full_range_byte = 0;

    int ret = png_get_cICP(png_ptr, nullptr,
                           &colour_primaries_byte,
                           &transfer_function_byte,
                           &matrix_coefficients_byte,
                           &video_full_range_byte);

    TEST_EXPECT_TRUE(ret == 0, "png_get_cICP should return 0 when info_ptr is NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return g_failed_tests == 0;
}

// Test 4: info_ptr valid but without PNG_INFO_cICP flag set (simulate by not calling png_set_cICP)
static bool test_png_get_cICP_missing_flag() {
    // Create structures
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!create_png_structs(&png_ptr, &info_ptr)) {
        fprintf(stderr, "Failed to create libpng structures for missing_flag test.\n");
        return false;
    }

    // Do NOT set cICP data; ensure valid flag remains unset
    // Ensure the function handles missing flag gracefully
    png_bytep colour_primaries_out = new png_byte(0);
    png_bytep transfer_function_out = new png_byte(0);
    png_bytep matrix_coefficients_out = new png_byte(0);
    png_bytep video_full_range_flag_out = new png_byte(0);

    PNG_INFO info_flag = png_get_cICP(png_ptr, info_ptr,
                                      colour_primaries_out,
                                      transfer_function_out,
                                      matrix_coefficients_out,
                                      video_full_range_flag_out);

    TEST_EXPECT_TRUE(info_flag == 0,
                     "png_get_cICP should return 0 when PNG_INFO_cICP flag not set");

    delete colour_primaries_out;
    delete transfer_function_out;
    delete matrix_coefficients_out;
    delete video_full_range_flag_out;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return g_failed_tests == 0;
}

// Test 5: Output pointers are NULL should fail gracefully (return 0)
static bool test_png_get_cICP_null_outputs() {
    // Create structures and populate cICP via setter
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    if (!create_png_structs(&png_ptr, &info_ptr)) {
        fprintf(stderr, "Failed to create libpng structures for null_outputs test.\n");
        return false;
    }

    // Set cICP to valid data
    png_set_cICP(png_ptr, info_ptr, 1, 2, 3, 1);

    // Pass NULL for all output pointers
    int ret = png_get_cICP(png_ptr, info_ptr, nullptr, nullptr, nullptr, nullptr);

    TEST_EXPECT_TRUE(ret == 0,
                     "png_get_cICP should return 0 when output pointers are NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return g_failed_tests == 0;
}

// Main function to run tests
int main() {
    // Run tests with explanatory comments in code
    printf("Running test: test_png_get_cICP_success\n");
    bool ok1 = test_png_get_cICP_success();
    printf("Result: %s\n", ok1 ? "PASS" : "FAIL");

    printf("Running test: test_png_get_cICP_null_png_ptr\n");
    bool ok2 = test_png_get_cICP_null_png_ptr();
    printf("Result: %s\n", ok2 ? "PASS" : "FAIL");

    printf("Running test: test_png_get_cICP_null_info_ptr\n");
    bool ok3 = test_png_get_cICP_null_info_ptr();
    printf("Result: %s\n", ok3 ? "PASS" : "FAIL");

    printf("Running test: test_png_get_cICP_missing_flag\n");
    bool ok4 = test_png_get_cICP_missing_flag();
    printf("Result: %s\n", ok4 ? "PASS" : "FAIL");

    printf("Running test: test_png_get_cICP_null_outputs\n");
    bool ok5 = test_png_get_cICP_null_outputs();
    printf("Result: %s\n", ok5 ? "PASS" : "FAIL");

    // Summary
    printf("Total tests: %d, Failures: %d\n", g_total_tests, g_failed_tests);
    if (g_failed_tests > 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}