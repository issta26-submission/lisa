// C++11 test suite for the focal method: png_data_freer
// This test suite validates the behavior of png_data_freer as implemented in png.c
// It uses real libpng types and functions to exercise the conditional branches.
//
// Key behaviors tested:
// - Early return when either png_ptr or info_ptr is NULL
// - FREER = PNG_DESTROY_WILL_FREE_DATA -> sets the mask in info_ptr->free_me (bitwise OR)
// - FREER = PNG_USER_WILL_FREE_DATA    -> clears the mask in info_ptr->free_me (bitwise AND with NOT)
// - Unknown freer parameter triggers the libpng error handler (via a custom error function and setjmp/longjmp)

#include <cassert>
#include <setjmp.h>
#include <iostream>
#include <pngpriv.h>
#include <png.h>


// Global hook to detect if a libpng error path was invoked during a test
static int g_error_seen = 0;

// Custom error function to be installed for test 4 (unknown freer param)
static void custom_png_error(png_structp, png_const_charp)
{
    g_error_seen = 1;
}

// Helper macro for non-terminating test assertions
static int g_tests_run = 0;
static int g_tests_failed = 0;
#define EXPECT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "Test failed: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
    } \
} while (0)

// Test 1: Null pointers should cause an early return without modifying any state.
// This ensures the function handles NULL inputs safely.
static void test_null_pointers_return_no_action()
{
    // Call with both pointers NULL; should return immediately without dereferencing any member
    png_data_freer(nullptr, nullptr, 0, 0);

    // Additionally, verify that a non-NULL png_ptr but NULL info_ptr also returns
    // Create a minimal png_ptr to pass through the check, but keep info_ptr NULL
    png_structp test_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    // If creation failed, report and skip
    if (test_png_ptr == nullptr) {
        std::cerr << "Warning: Failed to create a dummy png_ptr for test_null_pointers_return_no_action; skipping this part." << std::endl;
        return;
    }

    png_inforp test_info_ptr = nullptr; // intentionally NULL
    png_data_freer((png_const_structrp)test_png_ptr, test_info_ptr, 0, 0);

    // Cleanup
    png_destroy_read_struct(&test_png_ptr, &test_info_ptr, nullptr);

    // If we reached here, the NULL info_ptr path did not crash
    EXPECT(true, "test_null_pointers_return_no_action completed safely");
}

// Test 2: When freer == PNG_DESTROY_WILL_FREE_DATA, the mask should be OR'ed into info_ptr->free_me
// This validates the true-branch of the first conditional.
static void test_destroy_will_free_data_sets_mask()
{
    // Create valid libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        std::cerr << "Warning: Failed to create png_ptr in test_destroy_will_free_data_sets_mask; skipping." << std::endl;
        EXPECT(false, "png_ptr creation failed");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        std::cerr << "Warning: Failed to create info_ptr in test_destroy_will_free_data_sets_mask; skipping." << std::endl;
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        EXPECT(false, "info_ptr creation failed");
        return;
    }

    // Initialize free_me to 0 for deterministic testing
    info_ptr->free_me = 0;

    const png_uint_32 mask = 0x01;
    png_data_freer((png_const_structrp)png_ptr, (png_inforp)info_ptr, PNG_DESTROY_WILL_FREE_DATA, mask);

    EXPECT((info_ptr->free_me & mask) != 0, "free_me bit should be set when using PNG_DESTROY_WILL_FREE_DATA");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

// Test 3: When freer == PNG_USER_WILL_FREE_DATA, the mask should be cleared from info_ptr->free_me
// This validates the false-branch of the first conditional and the mask-clearing logic.
static void test_user_will_free_data_clears_mask()
{
    // Create valid libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        std::cerr << "Warning: Failed to create png_ptr in test_user_will_free_data_clears_mask; skipping." << std::endl;
        EXPECT(false, "png_ptr creation failed");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        std::cerr << "Warning: Failed to create info_ptr in test_user_will_free_data_clears_mask; skipping." << std::endl;
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        EXPECT(false, "info_ptr creation failed");
        return;
    }

    // Initialize free_me to a known non-zero value with multiple bits set
    info_ptr->free_me = 0xFF;

    const png_uint_32 mask = 0x0F; // clear only the lower 4 bits
    png_data_freer((png_const_structrp)png_ptr, (png_inforp)info_ptr, PNG_USER_WILL_FREE_DATA, mask);

    EXPECT((info_ptr->free_me & mask) == 0, "free_me lower 4 bits should be cleared after PNG_USER_WILL_FREE_DATA");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

// Test 4: Unknown freer parameter should trigger the libpng error handling mechanism.
// We install a custom error function and use setjmp/longjmp to detect the error path.
// This ensures the else branch (unknown parameter) is exercised.
static void test_unknown_freer_param_triggers_error()
{
    // Prepare a png_ptr with a custom error function
    g_error_seen = 0;
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, custom_png_error, nullptr);
    if (png_ptr == nullptr) {
        std::cerr << "Warning: Failed to create png_ptr in test_unknown_freer_param_triggers_error; skipping." << std::endl;
        EXPECT(false, "png_ptr creation failed");
        return;
    }

    // Create an info struct; must be non-NULL to test the error path
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        std::cerr << "Warning: Failed to create info_ptr in test_unknown_freer_param_triggers_error; skipping." << std::endl;
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        EXPECT(false, "info_ptr creation failed");
        return;
    }

    // Use setjmp/longjmp style to catch the libpng error handler invocation
    int jmp_result = setjmp(png_jmpbuf(png_ptr));
    bool error_triggered = false;
    if (jmp_result != 0) {
        // We got a longjmp back from libpng due to error
        error_triggered = true;
    } else {
        // Trigger the error path by providing an unknown freer parameter
        png_data_freer((png_const_structrp)png_ptr, (png_inforp)info_ptr, 99999, 0x02);
        // If the function returns normally, it's not the expected path
        error_triggered = g_error_seen != 0;
    }

    EXPECT(error_triggered, "Unknown freer parameter should trigger libpng error handling (longjmp).");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

// Entry point: run all tests and report summary
int main()
{
    // Run the four unit tests with explanatory commentary
    test_null_pointers_return_no_action();
    test_destroy_will_free_data_sets_mask();
    test_user_will_free_data_clears_mask();
    test_unknown_freer_param_triggers_error();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << " | Passed: " << (g_tests_run - g_tests_failed)
              << " | Failed: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed (optional, but helpful for CI)
    return g_tests_failed ? 1 : 0;
}