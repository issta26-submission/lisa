// Copyright (c) 2026
// Lightweight C++11 test harness for the focal function png_set_sRGB in pngset.c
// This test does not rely on GoogleTest. It uses the real libpng APIs to drive the function.
// The tests are designed to be non-terminating (they report failures but continue execution).

#include <string>
#include <functional>
#include <iostream>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


extern "C" {
}

// Simple error handler used by libpng during test setup
static void pngtest_error_fn(png_structp /*png_ptr*/, png_const_charp /*error_message*/) {
    // Intentionally no-op for test harness; errors will be reported by libpng via setjmp/longjmp in real usage.
}

// Simple non-terminating assertion helper
#define TEST_ASSERT(cond, msg)                                         \
    do {                                                               \
        if (!(cond)) {                                               \
            std::cerr << "TEST FAILURE: " << (msg) << "\n";          \
            return false;                                            \
        }                                                            \
    } while (0)

static bool test_png_set_sRGB_with_valid_non_null_pointers() {
    // Purpose:
    // Verify that when both png_ptr and info_ptr are non-null, the function:
    // - sets info_ptr->rendering_intent to the provided srgb_intent
    // - sets the PNG_INFO_sRGB bit in info_ptr->valid
    //
    // This exercises the true-branch of the predicate.

    // Create a libpng read struct and info struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error_fn, NULL);
    if (png_ptr == NULL) {
        std::cerr << "TEST FAILURE: Unable to create libpng read struct.\n";
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        std::cerr << "TEST FAILURE: Unable to create libpng info struct.\n";
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    // Initialize to known state
    info_ptr->rendering_intent = -1;
    info_ptr->valid = 0;

    // Call focal function with a test intent value
    int test_intent = 12345;
    png_set_sRGB((png_const_structrp)png_ptr, (png_inforp)info_ptr, test_intent);

    // Validate outcomes
    TEST_ASSERT(info_ptr->rendering_intent == test_intent,
                "rendering_intent should be updated to the provided srgb_intent");
    TEST_ASSERT((info_ptr->valid & PNG_INFO_sRGB) != 0,
                "valid flag should have PNG_INFO_sRGB bit set after setting sRGB");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return true;
}

static bool test_png_set_sRGB_with_null_png_ptr_only() {
    // Purpose:
    // Ensure that when png_ptr is NULL but info_ptr is non-null, the function returns early
    // and does not modify info_ptr. This covers the false predicate (png_ptr == NULL) path.

    // Allocate a dummy info struct (simulate a real info_ptr)
    png_inforp info_ptr = (png_inforp)malloc(sizeof(png_info));
    if (info_ptr == NULL) {
        std::cerr << "TEST FAILURE: Allocation of info_ptr failed.\n";
        return false;
    }
    std::memset(info_ptr, 0, sizeof(png_info));

    // Call with NULL png_ptr
    png_set_sRGB((png_const_structrp)NULL, info_ptr, 999);

    // After the call, info_ptr should remain unchanged
    TEST_ASSERT(info_ptr->rendering_intent == 0,
                "rendering_intent should remain unchanged (0) when png_ptr is NULL");
    TEST_ASSERT((info_ptr->valid & PNG_INFO_sRGB) == 0,
                "valid flag should remain unset when png_ptr is NULL");

    free(info_ptr);
    return true;
}

static bool test_png_set_sRGB_with_null_info_ptr_only() {
    // Purpose:
    // Ensure that when info_ptr is NULL but png_ptr is non-null, the function returns early
    // and does not crash. This covers the false predicate (info_ptr == NULL) path.

    // Create a real png_ptr
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error_fn, NULL);
    if (png_ptr == NULL) {
        std::cerr << "TEST FAILURE: Unable to create libpng read struct.\n";
        return false;
    }

    // Call with NULL info_ptr
    png_set_sRGB((png_const_structrp)png_ptr, (png_inforp)NULL, 42);

    // If we reached here, the function returned gracefully.
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    return true;
}

static bool test_png_set_sRGB_overwrites_previous_rendering_intent() {
    // Purpose:
    // Verify that a subsequent call to png_set_sRGB overwrites the previous rendering_intent value
    // and updates the valid flag accordingly.

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error_fn, NULL);
    if (png_ptr == NULL) {
        std::cerr << "TEST FAILURE: Unable to create libpng read struct.\n";
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        std::cerr << "TEST FAILURE: Unable to create libpng info struct.\n";
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return false;
    }

    // First value
    int first_intent = 7;
    png_set_sRGB((png_const_structrp)png_ptr, (png_inforp)info_ptr, first_intent);
    // Overwrite with second value
    int second_intent = 99;
    png_set_sRGB((png_const_structrp)png_ptr, (png_inforp)info_ptr, second_intent);

    TEST_ASSERT(info_ptr->rendering_intent == second_intent,
                "rendering_intent should be overwritten by subsequent png_set_sRGB call");
    TEST_ASSERT((info_ptr->valid & PNG_INFO_sRGB) != 0,
                "PNG_INFO_sRGB flag should remain set after subsequent set_sRGB call");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return true;
}

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const std::string& name, std::function<bool()> f) {
        ++total;
        bool result = false;
        try {
            result = f();
        } catch (...) {
            std::cerr << "TEST EXCEPTION: " << name << "\n";
            result = false;
        }
        if (result) {
            std::cout << "[PASS] " << name << "\n";
            ++passed;
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    run("test_png_set_sRGB_with_valid_non_null_pointers", test_png_set_sRGB_with_valid_non_null_pointers);
    run("test_png_set_sRGB_with_null_png_ptr_only", test_png_set_sRGB_with_null_png_ptr_only);
    run("test_png_set_sRGB_with_null_info_ptr_only", test_png_set_sRGB_with_null_info_ptr_only);
    run("test_png_set_sRGB_overwrites_previous_rendering_intent", test_png_set_sRGB_overwrites_previous_rendering_intent);

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Non-zero exit code on failure to simplify integration with build systems
    return (passed == total) ? 0 : 1;
}