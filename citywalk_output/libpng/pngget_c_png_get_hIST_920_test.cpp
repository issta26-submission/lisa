// Minimal C++11 test suite for the focal function png_get_hIST from pngget.c
// This test avoids Google Test/GMock and uses a small, non-terminating assertion framework.
// It relies on libpng headers (png.h, pngpriv.h) being available in the project include path.

#include <vector>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <png.h>


// Include libpng public headers for the API and internal structures access.
// The internal structure access (png_info.valid, png_info.hist, etc.) may require pngpriv.h.

// Simple non-terminating test assertion helpers
static int g_failures = 0;

static void log_failure(const char* file, int line, const char* cond, const char* msg) {
    std::cerr << "Test failure at " << file << ":" << line << " - "
              << (cond ? cond : "COND") << " - " << (msg ? msg : "") << "\n";
}

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { log_failure(__FILE__, __LINE__, #cond, msg); ++g_failures; } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    auto _a = (a); auto _b = (b); \
    if(!(_a == _b)) { \
        std::cerr << "Test failure at " << __FILE__ << ":" << __LINE__ \
                  << " - Expected: " << #a << " == " << #b \
                  << " (actual: " << _a << " vs " << _b << "). " \
                  << (msg ? msg : "") << "\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { \
    if((void*)(a) != (void*)(b)) { \
        std::cerr << "Test failure at " << __FILE__ << ":" << __LINE__ \
                  << " - Pointers not equal: " << (void*)(a) << " != " << (void*)(b) \
                  << ". " << (msg ? msg : "") << "\n"; \
        ++g_failures; \
    } \
} while(0)

#ifdef PNG_INFO_hIST
#ifndef PNG_INFO_hIST // guard if macro not defined in some environments
#define PNG_INFO_hIST PNG_INFO_hIST
#endif
#endif

// Test 1: Success path where all prerequisites are satisfied.
// - png_ptr non-null
// - info_ptr non-null and has PNG_INFO_hIST bit set in valid
// - hist output pointer is non-null
// - info_ptr->hist points to a valid 256-element array
static void test_hIST_success_case() {
    // Create a png read structure and info structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Prepare histogram data
    png_uint_16 hist_values[256];
    for (int i = 0; i < 256; ++i) hist_values[i] = (png_uint_16)i; // fill with distinct values

    // Mark hIST as valid and point to hist_values
    info_ptr->valid |= PNG_INFO_hIST;
    info_ptr->hist = hist_values;

    // Output parameter (pointer-to-pointer)
    png_uint_16p hist_out = NULL;
    png_uint_16p *hist_out_ptr = &hist_out;

    // Call the focal function
    png_uint_32 result = png_get_hIST((png_const_structrp)png_ptr,
                                      (png_inforp)info_ptr,
                                      hist_out_ptr);

    // Validate the result and the out parameter
    EXPECT_EQ(result, PNG_INFO_hIST, "png_get_hIST should return PNG_INFO_hIST on success");
    EXPECT_PTR_EQ(hist_out, hist_values, "Output hist pointer should match info_ptr->hist pointer");
    if (hist_out != hist_values) {
        // Show actual pointer values for debugging
        std::cerr << "Actual hist_out: " << (void*)hist_out
                  << " Expected: " << (void*)hist_values << "\n";
    }

    // Optional safety: ensure content is preserved
    if (hist_out == hist_values) {
        EXPECT_EQ(hist_out[0], hist_values[0], "First histogram value should propagate correctly");
        EXPECT_EQ(hist_out[100], hist_values[100], "Histogram value at index 100 should propagate correctly");
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 2: NULL png_ptr should yield 0 (failure case)
static void test_hIST_null_png_ptr() {
    // Prepare a dummy info_ptr with hist pointer valid
    png_structp dummy_png_ptr = NULL;
    png_infop info_ptr = NULL;
    // Reusing libpng to create a real info_ptr to keep structure valid
    // In this test we only need info_ptr non-null
    dummy_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    info_ptr = png_create_info_struct(dummy_png_ptr);
    info_ptr->valid |= PNG_INFO_hIST;
    png_uint_16 hist_values[256];
    info_ptr->hist = hist_values;

    png_uint_16p hist_out = NULL;
    png_uint_32 result = png_get_hIST((png_const_structrp)NULL,
                                      (png_inforp)info_ptr,
                                      &hist_out);

    EXPECT_EQ(result, 0u, "png_get_hIST should return 0 when png_ptr is NULL");
    png_destroy_read_struct(&dummy_png_ptr, &info_ptr, NULL);
}

// Test 3: NULL hist output parameter should yield 0
static void test_hIST_null_hist_output() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Set up valid info_ptr with hist pointer
    png_uint_16 hist_values[256];
    info_ptr->valid |= PNG_INFO_hIST;
    info_ptr->hist = hist_values;

    // Call with NULL hist output
    png_uint_32 result = png_get_hIST((png_const_structrp)png_ptr,
                                      (png_inforp)info_ptr,
                                      NULL);

    EXPECT_EQ(result, 0u, "png_get_hIST should return 0 when hist output parameter is NULL");
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 4: info_ptr valid flag not set should yield 0
static void test_hIST_missing_valid_flag() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);

    // Do NOT set the hIST valid flag
    png_uint_16 hist_values[256];
    info_ptr->hist = hist_values; // point to something, but not marked valid

    png_uint_16p hist_out = NULL;
    png_uint_32 result = png_get_hIST((png_const_structrp)png_ptr,
                                      (png_inforp)info_ptr,
                                      &hist_out);

    EXPECT_EQ(result, 0u, "png_get_hIST should return 0 when PNG_INFO_hIST not marked valid");
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 5: NULL info_ptr should yield 0 (additional safety)
static void test_hIST_null_info_ptr() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_uint_16 hist_values[256];
    png_uint_16p hist_out = NULL;

    png_uint_32 result = png_get_hIST((png_const_structrp)png_ptr,
                                      (png_inforp)NULL,
                                      &hist_out);

    EXPECT_EQ(result, 0u, "png_get_hIST should return 0 when info_ptr is NULL");
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Main test runner
int main() {
    std::cout << "Starting tests for png_get_hIST...\n";

    test_hIST_success_case();
    test_hIST_null_png_ptr();
    test_hIST_null_hist_output();
    test_hIST_missing_valid_flag();
    test_hIST_null_info_ptr();

    if (g_failures == 0) {
        std::cout << "All tests passed for png_get_hIST.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed for png_get_hIST.\n";
        return 1;
    }
}