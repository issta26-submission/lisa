// C++11 test suite for the focal method: store_image_row
// This test does not use Google Test. It uses a lightweight custom harness.
// It relies on libpng's error handling mechanism to verify error branches.

#include <cstdint>
#include <cassert>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Forward declaration of the function under test.
// The actual implementation resides in pngvalid.c and uses the struct png_store.
struct png_store {
    unsigned char* image;
    unsigned int image_h;
    unsigned int cb_row;
    unsigned int cb_image;
    // other fields exist in the real type, but are not needed for the tests
};

// The focal method under test, declared with C linkage to match the library's symbol.
// Note: In the real project, store_image_row is defined in pngvalid.c.
// We declare it here to link against the production implementation.
extern "C" unsigned char* store_image_row(const png_store* ps, png_const_structp pp, int nImage,
                                         png_uint_32 y);

// Global jump buffer and last error message to capture PNG errors during tests.
static jmp_buf g_png_jump_buffer;
static const char* g_last_png_error_message = nullptr;

// Error handler used by libpng during tests to intercept errors.
// This matches the signature expected by png_set_error_fn.
static void test_error_handler(png_structp png_ptr, png_const_charp error_msg)
{
    (void)png_ptr; // unused in tests
    g_last_png_error_message = error_msg;
    longjmp(g_png_jump_buffer, 1);
}

// Basic test framework (very lightweight).
static bool run_all_tests();

// Test 1: Success path - ensure correct pointer arithmetic when image data exists and fits.
static bool test_store_image_row_success()
{
    // Arrange: small, predictable setup
    png_store ps;
    // image size chosen so that coffset = 2 for nImage=0, y=0 with cb_row=4, image_h=1
    ps.image = new unsigned char[9]; // need at least coffset + cb_row + 3 = 9
    for (size_t i = 0; i < 9; ++i)
        ps.image[i] = static_cast<unsigned char>(i);
    ps.image_h = 1;
    ps.cb_row = 4;
    ps.cb_image = 9; // ensure any overrun check passes

    // Use a valid PNG struct pointer; we don't trigger errors in this test.
    png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (pp == nullptr) {
        delete[] ps.image;
        std::cerr << "[test_store_image_row_success] Failed to create png_structp\n";
        return false;
    }

    int nImage = 0;
    png_uint_32 y = 0; // coffset calculation will be (0*1 + 0)*(4+5) + 2 = 2

    // Act
    unsigned char* result = store_image_row(&ps, pp, nImage, y);

    // Assert
    bool passed = (result == ps.image + 2);

    // Cleanup
    png_destroy_write_struct(&pp, NULL);
    delete[] ps.image;

    if (!passed) {
        std::cout << "[test_store_image_row_success] FAILED: Returned pointer did not match expected offset.\n";
    } else {
        std::cout << "[test_store_image_row_success] PASSED\n";
    }
    return passed;
}

// Test 2: Error path - "no allocated image" when ps.image is NULL.
static bool test_store_image_row_error_no_allocated()
{
    // Arrange
    png_store ps;
    ps.image = nullptr;       // trigger first error branch
    ps.image_h = 1;
    ps.cb_row = 4;
    ps.cb_image = 100;          // value irrelevant for this path

    png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (pp == nullptr) {
        std::cerr << "[test_store_image_row_error_no_allocated] Failed to create png_structp\n";
        return false;
    }

    g_last_png_error_message = nullptr;
    // Prepare jump buffer for error handling
    int jig = setjmp(g_png_jump_buffer);
    if (jig == 0) {
        // Install our error handler before invoking the function
        png_set_error_fn(pp, NULL, test_error_handler, NULL);
        // Act: this should trigger the "no allocated image" error and longjmp
        unsigned char* res = store_image_row(&ps, pp, 0, 0);
        // If we return here, the error did not occur as expected
        png_destroy_write_struct(&pp, NULL);
        std::cout << "[test_store_image_row_error_no_allocated] FAILED: Expected error did not occur. Returned pointer: "
                  << static_cast<void*>(res) << "\n";
        return false;
    } else {
        // Assert: error should have been reported with that exact message
        bool ok = (g_last_png_error_message != nullptr) &&
                  (std::strcmp(g_last_png_error_message, "no allocated image") == 0);
        png_destroy_write_struct(&pp, NULL);
        if (!ok) {
            std::cout << "[test_store_image_row_error_no_allocated] FAILED: Unexpected error message or none reported.\n";
        } else {
            std::cout << "[test_store_image_row_error_no_allocated] PASSED\n";
        }
        return ok;
    }
}

// Test 3: Error path - "image too small" when the image buffer is too small to hold the requested row.
static bool test_store_image_row_error_image_too_small()
{
    // Arrange
    png_store ps;
    // image non-null but too small for the requested coffset and cb_row
    // coffset for nImage=0, y=0, cb_row=4, image_h=1 is 2; need at least 2 + 4 + 3 = 9
    ps.image = new unsigned char[8]; // deliberately too small
    ps.image_h = 1;
    ps.cb_row = 4;
    ps.cb_image = 8; // 8 < 9 -> should trigger "image too small"

    png_structp pp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (pp == nullptr) {
        delete[] ps.image;
        std::cerr << "[test_store_image_row_error_image_too_small] Failed to create png_structp\n";
        return false;
    }

    g_last_png_error_message = nullptr;
    int jig = setjmp(g_png_jump_buffer);
    if (jig == 0) {
        png_set_error_fn(pp, NULL, test_error_handler, NULL);
        unsigned char* res = store_image_row(&ps, pp, 0, 0);
        // If returns, error didn't occur
        png_destroy_write_struct(&pp, NULL);
        delete[] ps.image;
        std::cout << "[test_store_image_row_error_image_too_small] FAILED: Expected error did not occur. Returned pointer: "
                  << static_cast<void*>(res) << "\n";
        return false;
    } else {
        bool ok = (g_last_png_error_message != nullptr) &&
                  (std::strcmp(g_last_png_error_message, "image too small") == 0);
        png_destroy_write_struct(&pp, NULL);
        delete[] ps.image;
        if (!ok) {
            std::cout << "[test_store_image_row_error_image_too_small] FAILED: Unexpected error message or none reported.\n";
        } else {
            std::cout << "[test_store_image_row_error_image_too_small] PASSED\n";
        }
        return ok;
    }
}

// Main test orchestrator
static bool run_all_tests()
{
    bool all_passed = true;

    std::cout << "Running tests for store_image_row (pngvalid.c) using libpng error handling...\n";

    if (!test_store_image_row_success()) {
        all_passed = false;
    }
    if (!test_store_image_row_error_no_allocated()) {
        all_passed = false;
    }
    if (!test_store_image_row_error_image_too_small()) {
        all_passed = false;
    }

    return all_passed;
}

// Entry point
int main(void)
{
    bool all_ok = run_all_tests();
    if (all_ok) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}