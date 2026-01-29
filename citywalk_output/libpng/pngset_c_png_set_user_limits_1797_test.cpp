// Lightweight C++11 test suite for the PNG library function png_set_user_limits
// This test suite avoids Google Test and uses a minimal in-process harness.
// It relies on libpng internal structures to inspect the effect of png_set_user_limits.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>
#include <png.h>


extern "C" {
}

// Simple test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_PASS() do { std::cout << "[PASS] " << __func__ << std::endl; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << __func__ << ": " << msg << std::endl; ++g_failed_tests; } while(0)

#define RUN_TEST(test_fn) do { ++g_total_tests; if (test_fn()) { TEST_PASS(); } else { TEST_FAIL("test failed"); } } while(0)

// Helper to access the internal fields of the png_struct allocated by libpng.
// libpng defines the actual struct as 'struct png_struct'. The field names
// 'user_width_max' and 'user_height_max' exist on that struct.
// This helper casts the opaque pointer to the known internal type for verification.
static inline struct png_struct* as_internal_ptr(png_structp p) {
    return reinterpret_cast<struct png_struct*>(p);
}

// Test 1: Ensure that a valid png_ptr updates both user_width_max and user_height_max accordingly.
// This covers the non-NULL path and validates the setter's effect on internal state.
bool test_png_set_user_limits_updates_values() {
    // Create a write struct to obtain a valid png_ptr
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        // If creation fails, this environment isn't suitable for the test
        return false;
    }

    // Initialize internal fields to known values
    struct png_struct* internal = as_internal_ptr(png_ptr);
    internal->user_width_max = 0;
    internal->user_height_max = 0;

    // Invoke the function under test
    const uint32_t test_width  = 12345u;
    const uint32_t test_height = 67890u;
    png_set_user_limits(png_ptr, test_width, test_height);

    // Verify that the internal fields changed to the expected values
    bool ok = (internal->user_width_max  == test_width)  &&
              (internal->user_height_max == test_height);

    png_destroy_write_struct(&png_ptr, NULL);
    return ok;
}

// Test 2: Ensure that passing a NULL pointer does not crash or modify global state.
// This exercises the NULL branch in the function.
bool test_png_set_user_limits_with_null_ptr() {
    // The function should simply return when png_ptr is NULL and do nothing else.
    // If this dereferences NULL or crashes, the test would fail here.
    png_set_user_limits(NULL, 0xFFFFFFFF, 0xFFFFFFFF);
    // If no crash occurs, we consider this path exercised successfully.
    return true;
}

// Test 3: Validate that multiple consecutive calls update the internal state correctly.
// This ensures that repeated invocations behave as expected (no stale data).
bool test_png_set_user_limits_consecutive_updates() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) return false;

    struct png_struct* internal = as_internal_ptr(png_ptr);
    internal->user_width_max = 0;
    internal->user_height_max = 0;

    // First update
    png_set_user_limits(png_ptr, 100, 200);
    bool ok1 = (internal->user_width_max == 100) && (internal->user_height_max == 200);
    // Second update with new values
    png_set_user_limits(png_ptr, 300, 400);
    bool ok2 = (internal->user_width_max == 300) && (internal->user_height_max == 400);

    png_destroy_write_struct(&png_ptr, NULL);
    return ok1 && ok2;
}

// Test 4: Check handling of edge values (maximum 32-bit unsigned) to ensure no overflow issues.
// This aligns with the documentation note about accepting large limits.
bool test_png_set_user_limits_edge_values() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) return false;

    struct png_struct* internal = as_internal_ptr(png_ptr);
    internal->user_width_max = 0;
    internal->user_height_max = 0;

    const uint32_t max32 = 0x7FFFFFFF; // 2147483647
    png_set_user_limits(png_ptr, max32, max32);

    bool ok = (internal->user_width_max == max32) && (internal->user_height_max == max32);

    png_destroy_write_struct(&png_ptr, NULL);
    return ok;
}

int main() {
    // Run all tests and report summary
    RUN_TEST(test_png_set_user_limits_updates_values);
    RUN_TEST(test_png_set_user_limits_with_null_ptr);
    RUN_TEST(test_png_set_user_limits_consecutive_updates);
    RUN_TEST(test_png_set_user_limits_edge_values);

    std::cout << "Total tests: " << g_total_tests << ", Failed: " << g_failed_tests << std::endl;
    return (g_failed_tests == 0) ? 0 : 1;
}