// Simplified C++11 unit test suite for the libpng function
// png_set_text_compression_window_bits
// This test suite does not rely on Google Test. It uses a small in-house
// test harness with non-terminating assertions to maximize coverage.
// The tests exercise observable behaviors (warnings) via libpng's error/warning
// handler mechanism and observable side-effects when possible.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Include libpng headers. The tests rely on the official API where possible.

// Global storage for captured warnings from the library's warning function.
static std::string g_last_warning;

// Custom warning function to capture warnings emitted by libpng.
// This mirrors the signature used by libpng for warning handlers.
static void my_png_warning(png_structp /*ptr*/, png_const_charp message)
{
    if (message != nullptr)
        g_last_warning = message;
    else
        g_last_warning.clear();
}

// Minimal test harness (non-terminating assertions) inspired by EXPECT_* style.
// Keeps track of total tests and failed assertions to summarize results.
static int g_test_total = 0;
static int g_test_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_test_total; \
    if(!(cond)) { \
        ++g_test_failed; \
        std::fprintf(stderr, "TEST FAILURE: %s | Condition not met: %s\n", __FILE__, msg); \
    } \
} while(0)

#define EXPECT_NO_WARNING() do { \
    ++g_test_total; \
    if(!g_last_warning.empty()) { \
        ++g_test_failed; \
        std::fprintf(stderr, "TEST FAILURE: Warning emitted unexpectedly: %s\n", g_last_warning.c_str()); \
        g_last_warning.clear(); \
    } \
} while(0)

#define EXPECT_WARNING_CONTAINS(substring) do { \
    ++g_test_total; \
    if(g_last_warning.find(substring) == std::string::npos) { \
        ++g_test_failed; \
        std::fprintf(stderr, "TEST FAILURE: Expected warning containing '%s' not found. Got: '%s'\n", substring, g_last_warning.c_str()); \
    } else { \
        /* Reset after a successful match to isolate tests */ \
        g_last_warning.clear(); \
    } \
} while(0)

// Test 1: Null png_ptr should cause no crash or warning.
// The function should return immediately when png_ptr is NULL.
void test_png_set_text_compression_window_bits_null_ptr()
{
    // Reset captured warnings
    g_last_warning.clear();

    // This should be a no-op and should not crash.
    png_set_text_compression_window_bits(nullptr, 20);

    // Expect no warning to have been emitted.
    EXPECT_NO_WARNING();
}

// Test 2: Upper bound clamping and warning when window_bits > 15.
// The library should clamp to 15 and emit a warning with the known message.
void test_png_set_text_compression_window_bits_upper_bound_warning()
{
    // Allocate a png write struct using libpng's API as a realistic test driver.
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == nullptr) {
        std::fprintf(stderr, "Failed to create png_ptr for upper-bound test.\n");
        // Mark as a failed test since setup failed.
        ++g_test_total;
        ++g_test_failed;
        return;
    }

    // Install a warning handler to capture messages.
    g_last_warning.clear();
    // Some libpng versions provide png_set_error_fn to install custom handlers.
    // We use it here to ensure we intercept warnings from png_warning.
    // If the exact API signature differs, this call may need adaptation in the real project.
    png_set_error_fn(png_ptr, nullptr, nullptr, my_png_warning);

    // Call with a value greater than the allowed maximum (16 should trigger clamp to 15).
    png_set_text_compression_window_bits(png_ptr, 20);

    // Expect a warning containing the specific message substring.
    EXPECT_WARNING_CONTAINS("Only compression windows <= 32k supported by PNG");

    // Cleanup
    png_destroy_write_struct(&png_ptr, nullptr);
}

// Test 3: Lower bound clamping and warning when window_bits < 8.
// The library should clamp to 8 and emit a warning with the known message.
void test_png_set_text_compression_window_bits_lower_bound_warning()
{
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == nullptr) {
        std::fprintf(stderr, "Failed to create png_ptr for lower-bound test.\n");
        ++g_test_total;
        ++g_test_failed;
        return;
    }

    g_last_warning.clear();
    png_set_error_fn(png_ptr, nullptr, nullptr, my_png_warning);

    // Call with a value below the allowed minimum (7 should trigger clamp to 8).
    png_set_text_compression_window_bits(png_ptr, 7);

    EXPECT_WARNING_CONTAINS("Only compression windows >= 256 supported by PNG");

    png_destroy_write_struct(&png_ptr, nullptr);
}

// Test 4: In-range values should produce no warnings.
// Values within [8, 15] are accepted without emitting warnings.
void test_png_set_text_compression_window_bits_in_range_no_warning()
{
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == nullptr) {
        std::fprintf(stderr, "Failed to create png_ptr for in-range test.\n");
        ++g_test_total;
        ++g_test_failed;
        return;
    }

    g_last_warning.clear();
    png_set_error_fn(png_ptr, nullptr, nullptr, my_png_warning);

    // In-range value: 12 should not emit a warning.
    png_set_text_compression_window_bits(png_ptr, 12);
    EXPECT_NO_WARNING();

    // Boundary values also should not emit warnings.
    g_last_warning.clear();
    png_set_text_compression_window_bits(png_ptr, 8);
    EXPECT_NO_WARNING();

    g_last_warning.clear();
    png_set_text_compression_window_bits(png_ptr, 15);
    EXPECT_NO_WARNING();

    png_destroy_write_struct(&png_ptr, nullptr);
}

// Helper to print summary after tests
void print_summary()
{
    std::printf("Test summary: total=%d, failed=%d, passed=%d\n",
                g_test_total,
                g_test_failed,
                g_test_total - g_test_failed);
}

// Main entry: run all tests
int main()
{
    // Informational header
    std::printf("Starting tests for png_set_text_compression_window_bits...\n");

    test_png_set_text_compression_window_bits_null_ptr();
    test_png_set_text_compression_window_bits_upper_bound_warning();
    test_png_set_text_compression_window_bits_lower_bound_warning();
    test_png_set_text_compression_window_bits_in_range_no_warning();

    print_summary();

    // Return non-zero on failure to signaling test framework
    return (g_test_failed == 0) ? 0 : 1;
}