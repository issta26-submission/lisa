// Test suite for libpng's png_get_sRGB using a lightweight custom test harness.
// This test uses the actual libpng API (requires linking against -lpng).
// It verifies true/false branches of the sRGB info retrieval logic.
// The tests are designed to be non-terminating: failures are reported but do not abort execution.

#include <iostream>
#include <string>
#include <png.h>
#include <pngpriv.h>


// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// EXPECT_TRUE: non-terminating assertion with a message
#define EXPECT_TRUE(cond, msg) do {                 \
    ++g_total_tests;                                  \
    if (!(cond)) {                                    \
        ++g_failed_tests;                               \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << std::endl; \
    }                                                 \
} while (0)

// Helper to create and initialize a png read struct and its info struct
static bool create_png_structs(png_structp &png_ptr, png_infop &info_ptr) {
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "Failed to create png_struct." << std::endl;
        return false;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        std::cerr << "Failed to create png_info struct." << std::endl;
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }
    return true;
}

// Test 1: Valid info_ptr with PNG_INFO_sRGB flag set; file_srgb_intent is non-NULL
// Expectation: returns PNG_INFO_sRGB and stores rendering_intent into file_srgb_intent.
static void test_png_get_sRGB_valid_intent_with_output() {
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    if (!create_png_structs(png_ptr, info_ptr)) {
        EXPECT_TRUE(false, "Failed to initialize libpng structures for test 1");
        return;
    }

    // Set up sRGB info in info_ptr
    info_ptr->valid |= PNG_INFO_sRGB;  // indicate sRGB data is valid
    info_ptr->rendering_intent = 42;    // arbitrary rendering intent

    int file_srgb_intent = -1;
    int ret = png_get_sRGB(reinterpret_cast<png_const_structrp>(png_ptr),
                           reinterpret_cast<png_const_inforp>(info_ptr),
                           &file_srgb_intent);

    EXPECT_TRUE(ret == PNG_INFO_sRGB, "png_get_sRGB should return PNG_INFO_sRGB when sRGB is valid");
    EXPECT_TRUE(file_srgb_intent == 42, "png_get_sRGB should propagate rendering_intent to output");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 2: Valid info_ptr with PNG_INFO_sRGB flag set; file_srgb_intent is NULL
// Expectation: returns PNG_INFO_sRGB; should not crash or dereference NULL.
static void test_png_get_sRGB_valid_intent_with_null_output() {
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    if (!create_png_structs(png_ptr, info_ptr)) {
        EXPECT_TRUE(false, "Failed to initialize libpng structures for test 2");
        return;
    }

    info_ptr->valid |= PNG_INFO_sRGB;
    info_ptr->rendering_intent = 7;

    int ret = png_get_sRGB(reinterpret_cast<png_const_structrp>(png_ptr),
                           reinterpret_cast<png_const_inforp>(info_ptr),
                           NULL);

    EXPECT_TRUE(ret == PNG_INFO_sRGB, "png_get_sRGB should return PNG_INFO_sRGB even when output pointer is NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 3: info_ptr valid flag does not include PNG_INFO_sRGB
// Expectation: returns 0; output pointer (if any) should not be modified.
static void test_png_get_sRGB_missing_flag() {
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    if (!create_png_structs(png_ptr, info_ptr)) {
        EXPECT_TRUE(false, "Failed to initialize libpng structures for test 3");
        return;
    }

    // Do NOT set PNG_INFO_sRGB flag
    info_ptr->valid &= ~PNG_INFO_sRGB;
    info_ptr->rendering_intent = 99;

    int file_srgb_intent = -123;
    int ret = png_get_sRGB(reinterpret_cast<png_const_structrp>(png_ptr),
                           reinterpret_cast<png_const_inforp>(info_ptr),
                           &file_srgb_intent);

    EXPECT_TRUE(ret == 0, "png_get_sRGB should return 0 when PNG_INFO_sRGB flag is not set");
    EXPECT_TRUE(file_srgb_intent == -123, "Output value should remain unchanged when sRGB flag missing");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 4: Null png_ptr
// Expectation: returns 0 regardless of info_ptr state
static void test_png_get_sRGB_null_png_ptr() {
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    if (!create_png_structs(png_ptr, info_ptr)) {
        EXPECT_TRUE(false, "Failed to initialize libpng structures for test 4");
        return;
    }

    info_ptr->valid |= PNG_INFO_sRGB;
    info_ptr->rendering_intent = 5;

    int file_srgb_intent = 5;
    int ret = png_get_sRGB(nullptr, reinterpret_cast<png_const_inforp>(info_ptr),
                           &file_srgb_intent);

    EXPECT_TRUE(ret == 0, "png_get_sRGB should return 0 if png_ptr is NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 5: Null info_ptr
// Expectation: returns 0 when info_ptr is NULL
static void test_png_get_sRGB_null_info_ptr() {
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    if (!create_png_structs(png_ptr, info_ptr)) {
        EXPECT_TRUE(false, "Failed to initialize libpng structures for test 5");
        return;
    }

    info_ptr->valid |= PNG_INFO_sRGB;

    int file_srgb_intent = 10;
    int ret = png_get_sRGB(reinterpret_cast<png_const_structrp>(png_ptr),
                           nullptr,
                           &file_srgb_intent);

    EXPECT_TRUE(ret == 0, "png_get_sRGB should return 0 if info_ptr is NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

int main() {
    // Run tests
    test_png_get_sRGB_valid_intent_with_output();
    test_png_get_sRGB_valid_intent_with_null_output();
    test_png_get_sRGB_missing_flag();
    test_png_get_sRGB_null_png_ptr();
    test_png_get_sRGB_null_info_ptr();

    // Summary
    std::cout << "Total tests: " << g_total_tests << std::endl;
    std::cout << "Failed tests: " << g_failed_tests << std::endl;
    if (g_failed_tests > 0) {
        std::cout << "Some tests failed. Please inspect the above messages." << std::endl;
        return 1;
    }
    else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}