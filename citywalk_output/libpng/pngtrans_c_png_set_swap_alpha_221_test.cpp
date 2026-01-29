// Test suite for png_set_swap_alpha function from pngtrans.c
// This test is designed to be self-contained and executable without Google Test.
// It uses libpng (include <png.h>) to create a real png_struct and verify
// that png_set_swap_alpha correctly ORs the PNG_SWAP_ALPHA flag into
// the transformations field. It also tests NULL handling and preservation
// of existing flags.
//
// Notes:
// - Tests assume libpng is available in the environment where this is compiled.
// - Non-terminating assertions are used: failures are recorded but do not
//   abort the test suite, enabling multiple scenarios to run in one go.

#include <cstdint>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <png.h>


// libpng headers (C API accessed from C++). Ensure C linkage for C headers.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_test_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        printf("[TEST PASS] %s\n", test_name);
    } else {
        ++g_failed_tests;
        printf("[TEST FAIL] %s\n", test_name);
    }
}

// Test 1: NULL pointer should be handled gracefully (no crash, no modification expected).
// Rationale: png_set_swap_alpha checks for NULL and returns early; no changes should occur.
static void test_png_set_swap_alpha_null_ptr() {
    const char* test_name = "png_set_swap_alpha_null_ptr";
    // Call with NULL; if the function dereferenced the pointer, this would crash.
    png_set_swap_alpha(nullptr);
    // If we reach here, either the function handled NULL or did not crash.
    log_test_result(test_name, true);
}

// Test 2: Non-NULL png_ptr should have PNG_SWAP_ALPHA bit set after call.
// Rationale: The function ORs the PNG_SWAP_ALPHA flag into the transformations field.
static void test_png_set_swap_alpha_sets_flag() {
    const char* test_name = "png_set_swap_alpha_sets_flag";

    // Create a png struct using libpng's API. This avoids constructing the
    // internal struct manually and mirrors typical usage.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        printf("[TEST WARN] %s: unable to create png_struct (png_create_read_struct failed)\n", test_name);
        log_test_result(test_name, false);
        return;
    }

    // Ensure a known initial state
    png_ptr->transformations = 0u;

    // Invoke the function under test
    png_set_swap_alpha(png_ptr);

    // Validate that the flag is set
    bool passed = (png_ptr->transformations & PNG_SWAP_ALPHA) != 0;

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    log_test_result(test_name, passed);
}

// Test 3: Existing transformations should be preserved and OR-ed with PNG_SWAP_ALPHA.
// Rationale: The function should not clobber other flags; it should perform bitwise OR.
static void test_png_set_swap_alpha_preserves_other_flags() {
    const char* test_name = "png_set_swap_alpha_preserves_other_flags";

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        printf("[TEST WARN] %s: unable to create png_struct (png_create_read_struct failed)\n", test_name);
        log_test_result(test_name, false);
        return;
    }

    // Set an arbitrary non-zero initial transformation value with multiple bits.
    // Use a concrete constant to avoid depending on runtime values.
    const uint32_t initial_flags = 0xABCDEu;
    png_ptr->transformations = initial_flags;

    // Call the function under test
    png_set_swap_alpha(png_ptr);

    // Expected: initial_flags OR PNG_SWAP_ALPHA
    uint32_t expected = initial_flags | PNG_SWAP_ALPHA;
    bool passed = (png_ptr->transformations == expected);

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    log_test_result(test_name, passed);
}

// Entry point: run all tests and report summary
int main() {
    printf("Running tests for png_set_swap_alpha...\n\n");

    test_png_set_swap_alpha_null_ptr();
    test_png_set_swap_alpha_sets_flag();
    test_png_set_swap_alpha_preserves_other_flags();

    printf("\nTest summary: %d total, %d failed\n",
           g_total_tests, g_failed_tests);

    // Non-zero exit code if any test failed to integrate with build systems.
    return g_failed_tests == 0 ? 0 : 1;
}