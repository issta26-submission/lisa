// High-quality C++11 unit tests for the focal method: png_get_rgb_to_gray_status
// This test suite is designed to be run without GoogleTest, using a simple
// in-house harness. It relies on libpng internal headers (pngpriv.h) to access
// the internal struct layout and member rgb_to_gray_status.

#include <png.h>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


// Include C headers with C linkage for libpng
extern "C" {
}

// Simple internal test harness
static int g_total = 0;
static int g_passed = 0;

static void report_test(const char* name, bool passed)
{
    ++g_total;
    if (passed) {
        ++g_passed;
        std::cout << "PASS: " << name << std::endl;
    } else {
        std::cerr << "FAIL: " << name << std::endl;
    }
}

// Test 1: When the input pointer is NULL, the function should return 0
// This covers the false branch of the conditional operator when png_ptr is null.
static void test_png_get_rgb_to_gray_status_null_ptr()
{
    // NULL input - should return 0
    png_byte status = png_get_rgb_to_gray_status(nullptr);
    bool ok = (status == 0);
    report_test("png_get_rgb_to_gray_status(NULL) == 0", ok);
}

// Test 2: Non-null png_ptr with default rgb_to_gray_status == 0
// This covers the true branch with a non-null pointer and rgb_to_gray_status = 0.
static void test_png_get_rgb_to_gray_status_non_null_default_zero()
{
    // Create a minimal read struct. This allocates a valid png_ptr with internal layout.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        report_test("png_get_rgb_to_gray_status(non-null, default 0) allocation", false);
        return;
    }

    // Ensure the internal field exists and default is 0
    // The internal structure defines rgb_to_gray_status; initialize to 0 explicitly.
    png_ptr->rgb_to_gray_status = 0;

    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    bool ok = (status == 0);
    report_test("png_get_rgb_to_gray_status(non-null, default 0) == 0", ok);

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
}

// Test 3: Non-null png_ptr with a non-zero rgb_to_gray_status
// This validates the true branch and correct value propagation through the function.
static void test_png_get_rgb_to_gray_status_non_null_nonzero()
{
    // Create a minimal read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        report_test("png_get_rgb_to_gray_status(non-null, non-zero) allocation", false);
        return;
    }

    // Set a non-zero status and verify
    const png_byte expected = 123;
    png_ptr->rgb_to_gray_status = expected;

    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    bool ok = (status == expected);
    report_test("png_get_rgb_to_gray_status(non-null, non-zero) == expected", ok);

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
}

// Test 4: Non-null png_ptr with the maximum possible byte value
// Ensures correct handling at upper bound (255).
static void test_png_get_rgb_to_gray_status_non_null_max_value()
{
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        report_test("png_get_rgb_to_gray_status(non-null, 255) allocation", false);
        return;
    }

    const png_byte expected = 255;
    png_ptr->rgb_to_gray_status = expected;

    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    bool ok = (status == expected);
    report_test("png_get_rgb_to_gray_status(non-null, 255) == 255", ok);

    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
}

// Entry point
int main()
{
    // Run tests
    test_png_get_rgb_to_gray_status_null_ptr();
    test_png_get_rgb_to_gray_status_non_null_default_zero();
    test_png_get_rgb_to_gray_status_non_null_nonzero();
    test_png_get_rgb_to_gray_status_non_null_max_value();

    // Summary
    std::cout << "\nTest Summary: " << g_passed << " / " << g_total << " tests passed.\n";

    // Return non-zero if any test failed
    return (g_passed == g_total) ? 0 : 1;
}