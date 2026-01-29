// Unit test suite for the focal method: png_get_progressive_ptr
// Target: verify correct behavior for NULL input and for a valid png_ptr with a progressive pointer
// Notes:
// - This test uses the public libpng API to create a png_struct and set a progressive read function.
// - We avoid terminating assertions; failures are logged and counted.
// - Compile with: g++ -std=c++11 -I<path-to-libpng-include> test_file.cpp -lpng -lpng16 (adjust as needed)

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <png.h>


// Bring in C headers with C linkage for libpng
extern "C" {
}

// Global counter for test failures to allow non-terminating test execution
static int g_test_failures = 0;

// Dummy callback implementations required by png_set_progressive_read_fn.
// They are no-ops since we only test the progressive pointer plumbing.
static void info_fn(png_structp /*png_ptr*/, png_infop /*info_ptr*/) {
    // No operation
}
static void row_fn(png_structp /*png_ptr*/, png_bytep /*row*/, png_uint_32 /*width*/) {
    // No operation
}
static void end_fn(png_structp /*png_ptr*/, png_infop /*info_ptr*/) {
    // No operation
}

/*
 * Helper macro to report pointer equality in a non-terminating manner.
 * Prints a message on mismatch and increments the global failure counter.
 */
#define REPORT_PTR_EQ(actual, expected, msg) do { \
    if ((void*)(actual) != (void*)(expected)) { \
        std::cerr << "TEST FAIL: " << msg \
                  << " | Expected: " << (void*)(expected) \
                  << " | Actual: " << (void*)(actual) << std::endl; \
        ++g_test_failures; \
    } \
} while (0)

/*
 * Test 1: Null input should return NULL
 * - Call png_get_progressive_ptr with a NULL pointer and verify NULL is returned.
 */
static void test_png_get_progressive_ptr_null_input() {
    // Arrange
    png_const_structrp null_ptr = NULL; // explicit NULL input
    // Act
    png_voidp result = png_get_progressive_ptr(null_ptr);
    // Assert
    REPORT_PTR_EQ(result, NULL, "png_get_progressive_ptr(NULL) should return NULL");
}

/*
 * Test 2: Valid png_ptr with a progressive pointer set via png_set_progressive_read_fn
 * - Create a png read struct, set a progressive read function with a known progressive_ptr,
 *   then verify png_get_progressive_ptr returns the same pointer.
 */
static void test_png_get_progressive_ptr_after_setting() {
    // Arrange: Create a png read struct
    const char* ver = PNG_LIBPNG_VER_STRING;
    png_structp png_ptr = png_create_read_struct(ver, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "TEST FAIL: Failed to create png read struct." << std::endl;
        ++g_test_failures;
        return;
    }

    // Progressive pointer we want to track
    static int test_token = 0;
    png_voidp expected_ptr = static_cast<png_voidp>(&test_token);

    // Act: Set progressive read function with the test token as progressive_ptr
    png_set_progressive_read_fn(png_ptr, expected_ptr, info_fn, row_fn, end_fn);

    // Retrieve the progressive pointer via the focal method
    png_voidp actual_ptr = png_get_progressive_ptr((png_const_structrp)png_ptr);

    // Assert
    REPORT_PTR_EQ(actual_ptr, expected_ptr, "png_get_progressive_ptr should return the progressive_ptr set by png_set_progressive_read_fn");

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

/*
 * Test 3: Change progressive_ptr to a different object and verify update
 * - After initial set, update to a new pointer and ensure the getter reflects the new value.
 */
static void test_png_get_progressive_ptr_updates() {
    // Arrange: Create a png read struct
    const char* ver = PNG_LIBPNG_VER_STRING;
    png_structp png_ptr = png_create_read_struct(ver, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "TEST FAIL: Failed to create png read struct for update test." << std::endl;
        ++g_test_failures;
        return;
    }

    // Initial progressive_ptr
    static int token1 = 0;
    png_voidp ptr1 = static_cast<png_voidp>(&token1);
    png_set_progressive_read_fn(png_ptr, ptr1, info_fn, row_fn, end_fn);

    // Verify initial value
    png_voidp actual1 = png_get_progressive_ptr((png_const_structrp)png_ptr);
    REPORT_PTR_EQ(actual1, ptr1, "Initial progressive_ptr should match ptr1");

    // Update progressive_ptr to a new object
    static int token2 = 0;
    png_voidp ptr2 = static_cast<png_voidp>(&token2);
    png_set_progressive_read_fn(png_ptr, ptr2, info_fn, row_fn, end_fn);

    // Verify updated value
    png_voidp actual2 = png_get_progressive_ptr((png_const_structrp)png_ptr);
    REPORT_PTR_EQ(actual2, ptr2, "Updated progressive_ptr should match ptr2");

    // Cleanup
    PNG_UNUSED(actual1); // suppress unused warnings if compiled with certain flags
    PNG_UNUSED(actual2);
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Entry point to run tests
int main() {
    // Run tests
    test_png_get_progressive_ptr_null_input();
    test_png_get_progressive_ptr_after_setting();
    test_png_get_progressive_ptr_updates();

    // Report result
    if (g_test_failures == 0) {
        std::cout << "[ALL TESTS PASSED] png_get_progressive_ptr behavior is correct." << std::endl;
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] Total failures: " << g_test_failures << std::endl;
        return 1;
    }
}