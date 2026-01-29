// This test suite targets the focal method: png_set_write_status_fn
// It is implemented in a minimal, self-contained manner to be compiled
// as a standalone C++11 test (no GTest required).

// Candidate Keywords (Step 1 recap):
// - png_struct_def / png_structrp (mocked struct and pointer type mirroring the real API)
// - png_write_status_ptr (function pointer type assigned to png_ptr->write_row_fn)
// - png_ptr->write_row_fn (field to be set by the focal method)
// - png_debug (internal logging call within the function, always invoked)
// - NULL branch (png_ptr == NULL) behavior
// - write_row_fn assignment (behavior when non-NULL)


#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>


// <FOCAL_CLASS_DEP> Mocked dependencies to enable testing without the real libpng.
// We recreate only what's necessary for the focal method to compile and be testable.

typedef void (*png_write_status_ptr)(struct png_struct_def*, unsigned int);

struct png_struct_def {
    png_write_status_ptr write_row_fn;
};

// Alias to mimic the naming in the original code (png_structrp is a pointer to the struct)
typedef png_struct_def* png_structrp;

// Minimal mock of the internal debug logger used by the focal method.
static bool g_png_debug_called = false;
void png_debug(int, const char*) {
    // Record that the debug function was invoked. The parameters are ignored for testing.
    g_png_debug_called = true;
}

// The focal method under test (duplicated here to allow isolated unit testing without
// requiring the full libpng build environment). The logic mirrors the provided snippet.
void png_set_write_status_fn(png_structrp png_ptr, png_write_status_ptr write_row_fn)
{
{
   png_debug(1, "in png_set_write_status_fn");
   if (png_ptr == NULL)
      return;
   png_ptr->write_row_fn = write_row_fn;
}
}

// Dummy status callback implementations used as distinct function pointers for testing.
void dummy_statusA(struct png_struct_def* /*ptr*/, unsigned int /*val*/) { /* no-op */ }
void dummy_statusB(struct png_struct_def* /*ptr*/, unsigned int /*val*/) { /* no-op */ }

// Simple non-GTest assertion helpers (non-terminating, suitable for a lightweight harness)
static int g_total_assertions = 0;
static int g_failed_assertions = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_total_assertions; \
    if (!(cond)) { \
        ++g_failed_assertions; \
        std::cerr << "Test fail: " << (msg) << std::endl; \
    } \
} while (0)

#define ASSERT_EQ_PTR(a, b, msg) do { \
    ++g_total_assertions; \
    if ((void*)(a) != (void*)(b)) { \
        ++g_failed_assertions; \
        std::cerr << "Test fail: " << (msg) \
                  << " (expected pointer " << static_cast<const void*>(a) \
                  << " to equal " << static_cast<const void*>(b) << ")" << std::endl; \
    } \
} while (0)

#define ASSERT_EQ_INT(a, b, msg) do { \
    ++g_total_assertions; \
    if ((a) != (b)) { \
        ++g_failed_assertions; \
        std::cerr << "Test fail: " << (msg) \
                  << " (expected " << (a) << " == " << (b) << ")" << std::endl; \
    } \
} while (0)

// Test 1: When png_ptr is NULL, the function should not crash and should still call png_debug.
void test_null_ptr_branches_are_safe() {
    g_png_debug_called = false;
    png_ptr = nullptr; // intentionally using NULL-like state via C++ nullptr

    // Call the function with a NULL pointer and a dummy status function
    png_set_write_status_fn(nullptr, dummy_statusA);

    // Expect that png_debug was invoked regardless of png_ptr being NULL
    ASSERT_TRUE(g_png_debug_called, "png_debug should be called even when png_ptr is NULL");

    // No write_row_fn assignment occurs, but since png_ptr is NULL there is nothing to verify further.
}

// Test 2: When png_ptr is non-NULL, the write_row_fn should be assigned correctly.
void test_write_status_fn_sets_pointer_when_ptr_non_null() {
    g_png_debug_called = false;

    png_struct_def s;
    s.write_row_fn = nullptr;

    // Call with a valid pointer and a specific callback
    png_set_write_status_fn(&s, dummy_statusA);

    // Verify that the callback was stored
    ASSERT_EQ_PTR(s.write_row_fn, dummy_statusA, "write_row_fn should be set to the provided function pointer");

    // Also verify that the debug function was invoked
    ASSERT_TRUE(g_png_debug_called, "png_debug should be called when png_ptr is non-NULL");
}

// Test 3: Changing from one function pointer to another should update correctly.
void test_write_status_fn_updates_existing_pointer() {
    g_png_debug_called = false;

    png_struct_def s;
    s.write_row_fn = dummy_statusB; // initial pointer

    // Change to a different function pointer
    png_set_write_status_fn(&s, dummy_statusA);

    ASSERT_EQ_PTR(s.write_row_fn, dummy_statusA, "write_row_fn should update to the new function pointer");
    ASSERT_TRUE(g_png_debug_called, "png_debug should be called when updating write_row_fn");
}

// Helper to run all tests
void run_all_tests() {
    std::cout << "Running tests for png_set_write_status_fn (standalone test harness)" << std::endl;
    test_null_ptr_branches_are_safe();
    test_write_status_fn_sets_pointer_when_ptr_non_null();
    test_write_status_fn_updates_existing_pointer();

    int total = g_total_assertions;
    int failed = g_failed_assertions;
    std::cout << "Tests completed. Total: " << total << ", Failed: " << failed << std::endl;
}

int main() {
    run_all_tests();

    // Return non-zero if any test failed, for a conventional exit code signaling
    return (g_failed_assertions != 0) ? 1 : 0;
}