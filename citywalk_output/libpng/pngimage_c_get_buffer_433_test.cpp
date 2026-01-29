// C++11 test suite for the focal method get_buffer(pp) in pngimage.c
// This test uses libpng (png.h) to exercise the actual get_buffer behavior:
// get_buffer(pp) returns the current buffer pointer by casting the value returned
// by png_get_io_ptr(pp). We verify that the io_ptr supplied via png_set_read_fn
// is retrieved unchanged by get_buffer.

// Notes:
// - We avoid terminating tests on failure to maximize coverage (non-terminating assertions).
// - We rely on libpng to provide the actual png_get_io_ptr behavior.
// - We only compare addresses (void*) to avoid dependency on the exact struct buffer layout.
// - The struct buffer type is forward-declared to avoid needing the private layout.

#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <png.h>


// Forward declaration of the buffer type as used by pngimage.c.
// We do not need its internal layout for address comparison.
struct buffer;

// Declaration of the focal function under test with C linkage.
extern "C" buffer* get_buffer(png_structp pp);

// Forward declaration of libpng read callback (signature as used by png_set_read_fn).
extern "C" void test_png_read_fn(png_structp, png_bytep, png_size_t);

// Simple non-terminating test framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define ASSERT_TRUE(cond, message) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::fprintf(stderr, "TEST FAILURE: %s\n", message); \
            ++g_failed_tests; \
        } \
    } while (false)

static void report_summary() {
    std::printf("Tests run: %d, Passed: %d, Failed: %d\n",
                g_total_tests, g_total_tests - g_failed_tests, g_failed_tests);
}

// Test 1: Basic retrieval - when io_ptr is set via png_set_read_fn, get_buffer(pp)
// should return exactly that pointer (cast to struct buffer*).
// This exercises the true path of get_buffer and ensures pointer equality.
void test_get_buffer_basic() {
    // Allocate a dummy IO buffer to be used as the io_ptr for libpng.
    // We only need its address; contents are irrelevant.
    void* io_ptr = std::malloc(64);
    if (io_ptr == nullptr) {
        ASSERT_TRUE(false, "Failed to allocate io_ptr buffer for test_get_buffer_basic");
        return;
    }

    // Create a minimal png_struct for reading. This does not require a real PNG source;
    // we are only interested in the io_ptr value retrieved via png_get_io_ptr inside get_buffer.
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == nullptr) {
        std::fprintf(stderr, "Failed to create png read struct in test_get_buffer_basic\n");
        std::free(io_ptr);
        ASSERT_TRUE(false, "png_create_read_struct returned NULL");
        return;
    }

    // Set the read function and pass our io_ptr as the IO pointer for libpng.
    // The read function does not need to be called for get_buffer.
    png_set_read_fn(png_ptr, io_ptr, test_png_read_fn);

    // Call the focal function under test
    buffer* result = get_buffer(png_ptr);

    // The result should be the same as the io_ptr (cast to struct buffer*).
    ASSERT_TRUE(result != nullptr, "get_buffer returned NULL when io_ptr was set");
    ASSERT_TRUE(static_cast<void*>(result) == io_ptr, "get_buffer did not return the original io_ptr");

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    std::free(io_ptr);
}

// Test 2: Null io_ptr - when io_ptr is NULL, get_buffer(pp) should return NULL.
void test_get_buffer_null_io_ptr() {
    // Create a png_struct for the test
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == nullptr) {
        std::fprintf(stderr, "Failed to create png read struct in test_get_buffer_null_io_ptr\n");
        ASSERT_TRUE(false, "png_create_read_struct returned NULL");
        return;
    }

    // Pass a NULL io_ptr
    png_set_read_fn(png_ptr, NULL, test_png_read_fn);

    buffer* result = get_buffer(png_ptr);

    // Expect NULL
    ASSERT_TRUE(result == nullptr, "get_buffer should return NULL when io_ptr is NULL");

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Test 3: Multiple buffers - ensure distinct io_ptr pointers map to distinct buffers.
// This checks that changing io_ptr between different png_ptr instances yields correct associations.
void test_get_buffer_multiple_buffers() {
    // First buffer
    void* io_ptr1 = std::malloc(32);
    // Second buffer
    void* io_ptr2 = std::malloc(32);
    if (io_ptr1 == nullptr || io_ptr2 == nullptr) {
        std::fprintf(stderr, "Failed to allocate buffers for test_get_buffer_multiple_buffers\n");
        if (io_ptr1) std::free(io_ptr1);
        if (io_ptr2) std::free(io_ptr2);
        ASSERT_TRUE(false, "Allocation failure in test_get_buffer_multiple_buffers");
        return;
    }

    // First png_ptr
    png_structp png_ptr1 = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    // Second png_ptr
    png_structp png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (png_ptr1 == nullptr || png_ptr2 == nullptr) {
        std::fprintf(stderr, "Failed to create png read structs in test_get_buffer_multiple_buffers\n");
        if (png_ptr1) png_destroy_read_struct(&png_ptr1, NULL, NULL);
        if (png_ptr2) png_destroy_read_struct(&png_ptr2, NULL, NULL);
        std::free(io_ptr1);
        std::free(io_ptr2);
        ASSERT_TRUE(false, "png_create_read_struct failed in test_get_buffer_multiple_buffers");
        return;
    }

    // Assign first and second io_ptrs
    png_set_read_fn(png_ptr1, io_ptr1, test_png_read_fn);
    png_set_read_fn(png_ptr2, io_ptr2, test_png_read_fn);

    buffer* res1 = get_buffer(png_ptr1);
    buffer* res2 = get_buffer(png_ptr2);

    ASSERT_TRUE(static_cast<void*>(res1) == io_ptr1, "get_buffer for first png_ptr did not return io_ptr1");
    ASSERT_TRUE(static_cast<void*>(res2) == io_ptr2, "get_buffer for second png_ptr did not return io_ptr2");
    ASSERT_TRUE(static_cast<void*>(res1) != static_cast<void*>(res2),
                "get_buffer should map different io_ptrs to different buffers");

    // Cleanup
    png_destroy_read_struct(&png_ptr1, NULL, NULL);
    png_destroy_read_struct(&png_ptr2, NULL, NULL);
    std::free(io_ptr1);
    std::free(io_ptr2);
}

// Dummy read function required by libpng API (signature: void png_rw_ptr(png_structp, png_bytep, png_size_t))
// It is unused by the tests but necessary to satisfy png_set_read_fn.
extern "C" void test_png_read_fn(png_structp /*pp*/, png_bytep /*data*/, png_size_t /*length*/) {
    // Intentionally empty: tests do not perform actual IO.
}

// Entry point: run all tests and report summary.
// Note: We intentionally do not exit on first failure to maximize code coverage.
int main() {
    // Run tests
    test_get_buffer_basic();
    test_get_buffer_null_io_ptr();
    test_get_buffer_multiple_buffers();

    // Report summary
    report_summary();

    // Return 0 to indicate the test runner itself completed.
    return 0;
}