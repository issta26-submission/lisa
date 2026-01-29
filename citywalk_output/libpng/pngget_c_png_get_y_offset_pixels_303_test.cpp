// Test suite for png_get_y_offset_pixels using libpng public API.
// This harness exercises true/false branches of the internal predicate
// by configuring oFFs (offset) via public API functions.
// Compile with: g++ -std=c++11 -I<path_to_libpng_include> test_png_get_y_offset_pixels.cpp -lpng -lpng16 -lm
// Ensure libpng is available in the linking environment.

#include <cstdint>
#include <setjmp.h>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Include libpng headers (public API)

// Global failure counter to allow non-terminating test execution
static int g_failures = 0;

// Simple non-terminating assertion macro for test logging.
// On failure, logs the message and increments the global failure counter.
#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "TEST FAILURE: " << msg << " (in " << __FUNCTION__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

// Helper to safely create and destroy libpng structures for test isolation
static bool prepare_png_structs(png_structp &png_ptr, png_infop &info_ptr) {
    // Create the read struct. libpng ver string macro is provided by libpng.
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "Failed to create png_struct." << std::endl;
        return false;
    }

    // Create the info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cerr << "Failed to create png_info_struct." << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    // Set up libpng error handling with setjmp/longjmp to avoid crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        // If libpng triggers an error, clean up and fail gracefully
        std::cerr << "libpng error during test setup." << std::endl;
        png_destroy_info_struct(png_ptr, &info_ptr);
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    return true;
}

// Test 1: Pixel-offset Y value is returned when oFFs is set with pixel unit.
// This exercises the true branch where:
 // - png_ptr != NULL
 // - info_ptr != NULL
 // - (info_ptr->valid & PNG_INFO_oFFs) != 0
 // - info_ptr->offset_unit_type == PNG_OFFSET_PIXEL
static void test_png_get_y_offset_pixels_true_pixel_case() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!prepare_png_structs(png_ptr, info_ptr)) {
        TEST_ASSERT(false, "Failed to prepare libpng structs for true-pixel-case");
        return;
    }

    // Configure oFFs with pixel units and a specific offset_y
    const png_uint_32 expected_y = 12345;
    png_set_oFFs(info_ptr, 0, static_cast<png_int_32>(expected_y), PNG_OFFSET_PIXEL);

    // Invoke the function under test
    png_uint_32 actual = png_get_y_offset_pixels(png_ptr, info_ptr);

    TEST_ASSERT(actual == expected_y,
                "png_get_y_offset_pixels returned incorrect y_offset for pixel unit");
    
    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 2: Non-pixel unit should yield 0, exercising the false branch of unit_type check.
// oFFs set with a non-pixel unit (e.g., PNG_OFFSET_MICROMETERS)
static void test_png_get_y_offset_pixels_false_non_pixel_unit() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!prepare_png_structs(png_ptr, info_ptr)) {
        TEST_ASSERT(false, "Failed to prepare libpng structs for non-pixel-unit-case");
        return;
    }

    // Configure oFFs with non-pixel unit and a non-zero offset_y
    const png_uint_32 test_y = 54321;
    png_set_oFFs(info_ptr, 0, static_cast<png_int_32>(test_y), PNG_OFFSET_MICROMETERS);

    // Invoke the function under test
    png_uint_32 actual = png_get_y_offset_pixels(png_ptr, info_ptr);

    TEST_ASSERT(actual == 0,
                "png_get_y_offset_pixels should return 0 when oFFs unit != PIXEL");

    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 3: No oFFs information set should yield 0.
// This covers the case where the valid flag isn't set for oFFs.
static void test_png_get_y_offset_pixels_false_no_offs_set() {
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!prepare_png_structs(png_ptr, info_ptr)) {
        TEST_ASSERT(false, "Failed to prepare libpng structs for no-offs-case");
        return;
    }

    // Do not call png_set_oFFs; ensure that y_offset is not considered
    // (libpng should treat as not having oFFs information set).

    png_uint_32 actual = png_get_y_offset_pixels(png_ptr, info_ptr);

    TEST_ASSERT(actual == 0,
                "png_get_y_offset_pixels should return 0 when oFFs is not set");

    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 4: Null png_ptr should yield 0 (safety against null pointer dereference).
static void test_png_get_y_offset_pixels_null_png_ptr() {
    // Prepare a valid info_ptr using a temporary png_ptr to satisfy call shape
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!prepare_png_structs(png_ptr, info_ptr)) {
        TEST_ASSERT(false, "Failed to prepare libpng structs for null-ptr-test");
        return;
    }

    // Call with null png_ptr and valid info_ptr
    png_uint_32 actual = png_get_y_offset_pixels(nullptr, info_ptr);

    TEST_ASSERT(actual == 0,
                "png_get_y_offset_pixels should return 0 when png_ptr is NULL");

    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 5: Null info_ptr should yield 0 (safety against null pointer dereference).
static void test_png_get_y_offset_pixels_null_info_ptr() {
    // Prepare a valid png_ptr
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    if (!prepare_png_structs(png_ptr, info_ptr)) {
        TEST_ASSERT(false, "Failed to prepare libpng structs for null-info-ptr-test");
        return;
    }

    // Call with valid png_ptr and null info_ptr
    png_uint_32 actual = png_get_y_offset_pixels(png_ptr, nullptr);

    TEST_ASSERT(actual == 0,
                "png_get_y_offset_pixels should return 0 when info_ptr is NULL");

    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Entry point running all tests.
// This program does not rely on a specific test framework and uses libpng public API.
// Link against libpng when compiling.
int main() {
    std::cout << "Running tests for png_get_y_offset_pixels...\n";

    test_png_get_y_offset_pixels_true_pixel_case();
    test_png_get_y_offset_pixels_false_non_pixel_unit();
    test_png_get_y_offset_pixels_false_no_offs_set();
    test_png_get_y_offset_pixels_null_png_ptr();
    test_png_get_y_offset_pixels_null_info_ptr();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}