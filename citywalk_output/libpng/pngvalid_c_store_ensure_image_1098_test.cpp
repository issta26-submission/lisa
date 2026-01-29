// This test suite targets the focal method: store_ensure_image
// and its class dependency store_store-related fields used within the pngvalid.c file.
// The tests are written in C++11 (without GoogleTest) and exercise the most
// critical execution paths of store_ensure_image by providing a minimal testing
// harness that mimics the necessary dependencies of the original C code.
// Note: The test harness stubs only the required interactions; it does not rely on
// libpng internals. It focuses on ensuring the allocation path and the reuse path
// of image buffers behave as documented in the focal method.
//
// Structure:
// - Lightweight C-like scaffolding to emulate the environment required by store_ensure_image
// - Minimal implementations for store_image_free and store_image_row with the same semantics
// - A small assertion framework to collect failures without terminating test execution
// - Two test cases: allocation path (buffer not large enough) and reuse path (buffer already large)
// - A main() that runs the tests and reports results
//
// Important: We intentionally keep the test focused on the logic of store_ensure_image
// and its dependent components, as per instructions. We do not attempt to mock every
// global or static function from libpng; only the necessary behavior is reproduced.

#include <cstdint>
#include <cassert>
#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Domain-oriented assumptions for the test harness
// We provide typedefs and a minimal png_store struct compatible with the focal code.

typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef unsigned int png_uint_32;
typedef void* png_structp;
typedef const void* png_const_structp;

// Forward declaration of store_ensure_image with C linkage (as in the original code)
extern "C" {
    struct png_store;
    typedef struct png_store png_store;
    // Prototype of the focal function (signature must match)
    void store_ensure_image(png_store *ps, png_const_structp pp, int nImages,
                            size_t cbRow, png_uint_32 cRows);

    // The following helper functions are used by store_ensure_image in the focal code.
    // We'll provide minimal C-linkage implementations below (see helpers section).
    void store_image_free(png_store *ps, png_const_structp pp);
    png_bytep store_image_row(const png_store* ps, png_const_structp pp,
                              int nImage, png_uint_32 y);
}

// Compatibility macro used by the focal method to cast malloc() results.
// We provide a simple inline macro to avoid C++-isms in the C-style code path.
#define voidcast(type, expr) ((type)(expr))

// Minimal, test-specific definitions to emulate the png_store layout expected by the focal code.
// This layout matches the usage within store_ensure_image (cb_image, image, cb_row, image_h).
struct png_store {
    size_t cb_image;     // allocated image buffer size (in bytes)
    png_bytep image;      // pointer to the image buffer, offset by 1 due to misalignment
    size_t cb_row;       // number of bytes per row including extra 2 bytes at start and 3 at end
    png_uint_32 image_h;   // number of rows per image
};

// Globals used by our test stubs to validate behavior
static int g_store_image_free_called = 0;
static int g_png_error_called = 0;
static const char* g_last_png_error_msg = nullptr;

// ---- Test Helpers (C linkage) ----

extern "C" {

// Minimal implementation of store_image_free for testing
// It must mirror the semantics: if ps->image != NULL, free the image block starting at (ps->image - 1),
// then reset the fields to indicate no image is allocated.
void store_image_free(png_store *ps, png_const_structp pp)
{
    (void)pp; // not used in test harness
    if (ps == nullptr) return;
    if (ps->image != NULL) {
        // In store_ensure_image, the memory was allocated as malloc(cb+2) then ++image,
        // so the base pointer is image - 1.
        free(ps->image - 1);
        ps->image = NULL;
        ps->cb_image = 0;
    }
    // Mark that free was called
    g_store_image_free_called++;
}

// Minimal implementation of store_image_row to compute the address of a row
// within the allocated image buffer. The layout assumed by the focal code is:
// - Each row occupies (cbRow + 5) bytes: 2 bytes before the row data, cbRow data bytes,
//   and 3 bytes after the row data.
// - row points to the first data byte for the given (nImage, y) pair, so
//   row[-2] and row[-1] are valid (two preceding bytes), and
//   row[cbRow], row[cbRow+1], row[cbRow+2] are valid (trailing bytes).
png_bytep store_image_row(const png_store* ps, png_const_structp pp,
                          int nImage, png_uint_32 y)
{
    (void)pp; // not used in test harness
    if (ps == nullptr || ps->image == NULL) return NULL;
    size_t cb = ps->cb_row + 5; // per-row footprint
    size_t offset = ((size_t)nImage * ps->image_h + (size_t)y) * cb + 2;
    return ps->image + offset;
}

// The real library defines sql-level png_error(pp, msg). We mock it to capture errors.
void png_error(png_const_structp ppIn, const char *message)
{
    (void)ppIn;
    (void)message;
    g_png_error_called++;
    // In real code this would longjmp or terminate; for test we just record the event.
    // We also store the message for inspection if needed.
    // Note: Not allocating memory to store the message to avoid leaks in test harness.
    g_last_png_error_msg = message;
}

// A tiny wrapper to make the call explicit if needed in some tests
void store_ensure_image_setup(png_store *ps, png_const_structp pp, int nImages,
                              size_t cbRow, png_uint_32 cRows)
{
    store_ensure_image(ps, pp, nImages, cbRow, cRows);
}

} // extern "C"

// ---- Test Platform (Assertions) ----

static int g_total_tests_passed = 0;
static int g_total_tests = 0;

#define TEST_ASSERT(cond, msg) do {                     \
    ++g_total_tests;                                      \
    if (cond) {                                           \
        ++g_total_tests_passed;                           \
    } else {                                              \
        fprintf(stderr, "Test failed: %s\n", msg);      \
    }                                                     \
} while (0)

// Utility to print a summary
static void print_summary() {
    fprintf(stderr, "Tests: %d, Passed: %d\n", g_total_tests, g_total_tests_passed);
}

// ---- The Focal Tests ----

// Test 1: Allocation path when ps->cb_image < cb
// - Expect a new buffer allocated of size 'cb'
// - Image pointer is adjusted (ps->image[-1] == 0xed, ps->image[cb] == 0xfe)
// - The whole image region (cb bytes) is initialized to 0xb2 (178)
// - Row markers are set for all rows of all images
static void test_allocate_and_markers_path()
{
    // Reset state
    g_store_image_free_called = 0;
    g_png_error_called = 0;
    g_last_png_error_msg = nullptr;

    png_store ps;
    ps.cb_image = 0;     // force allocation
    ps.image = NULL;
    ps.cb_row = 0;
    ps.image_h = 0;

    // Parameters: nImages=2, cbRow=10, cRows=3
    const int nImages = 2;
    const size_t cbRow = 10;
    const png_uint_32 cRows = 3;

    // Call focal function
    store_ensure_image_setup(&ps, NULL, nImages, cbRow, cRows);

    // Expected cb
    const size_t cb = (size_t)nImages * cRows * (cbRow + 5); // 2 * 3 * 15 = 90

    // Assertions
    TEST_ASSERT(ps.cb_image == cb, "Allocation size mismatch (cb_image)");
    TEST_ASSERT(ps.image != NULL, "Image pointer should be allocated");
    TEST_ASSERT((unsigned char)ps.image[-1] == 0xed, "Marker before image data (image[-1]) should be 0xed");
    TEST_ASSERT((unsigned char)ps.image[cb] == 0xfe, "Marker after image data (image[cb]) should be 0xfe");

    // All bytes in the allocated image data should be initialized to 178 (0xb2)
    for (size_t i = 0; i < cb; ++i) {
        if (ps.image[i] != 178) {
            fprintf(stderr, "Byte at offset %zu is 0x%02x, expected 0xb2 (0x%02x)\n", i, ps.image[i], 178);
        }
        TEST_ASSERT(ps.image[i] == (png_bytep)178 ? true : false, "Image data initialized to 178");
        // The above line uses a trick due to types; ensure it's evaluated.
        // Real assertion is the value equality.
        if (ps.image[i] != 178) break;
    }

    // Validate per-row markers
    for (int img = 0; img < nImages; ++img) {
        for (png_uint_32 y = 0; y < cRows; ++y) {
            png_bytep row = store_image_row(&ps, NULL, img, y);
            TEST_ASSERT(row != NULL, "store_image_row returned NULL");
            TEST_ASSERT((int)row[-2] == 190, "Row marker -2 should be 190");
            TEST_ASSERT((int)row[-1] == 239, "Row marker -1 should be 239");
            TEST_ASSERT((int)row[(size_t)cbRow] == 222, "Row marker cbRow should be 222");
            TEST_ASSERT((int)row[(size_t)cbRow + 1] == 173, "Row marker cbRow+1 should be 173");
            TEST_ASSERT((int)row[(size_t)cbRow + 2] == 17, "Row marker cbRow+2 should be 17");
        }
    }

    // Cleanup
    store_image_free(&ps, NULL);

    // Summary
    // The assertions themselves update the counters
}

// Test 2: Reuse path when existing buffer is sufficient (ps->cb_image >= cb)
// - No allocation or reallocation should occur
// - The existing image buffer should be reused and partially overwritten with 178
// - Markers should still be applied to all rows
static void test_reuse_buffer_path()
{
    // Reset state
    g_store_image_free_called = 0;
    g_png_error_called = 0;
    g_last_png_error_msg = nullptr;

    png_store ps;
    ps.cb_image = 90;       // pre-allocated buffer large enough
    // Pre-allocate a buffer large enough to mimic prior use
    ps.image = (png_bytep)malloc(ps.cb_image);
    // Initialize with a known value different from 178 to ensure we overwrite
    if (ps.image) {
        memset(ps.image, 0xAA, ps.cb_image);
    }
    ps.cb_row = 0;
    ps.image_h = 0;

    // Parameters: nImages=1, cbRow=2, cRows=2
    const int nImages = 1;
    const size_t cbRow = 2;
    const png_uint_32 cRows = 2;

    // Call focal function
    store_ensure_image_setup(&ps, NULL, nImages, cbRow, cRows);

    // CB for this path
    const size_t cb = (size_t)nImages * cRows * (cbRow + 5); // 1 * 2 * 7 = 14

    // Since we reused the buffer, no allocation should have occurred
    TEST_ASSERT(g_store_image_free_called == 0, "store_image_free should not be called in reuse path");
    TEST_ASSERT(ps.cb_image == 90, "cb_image should remain unchanged in reuse path");
    TEST_ASSERT(ps.image != NULL, "Image pointer should still be allocated");

    // The first 'cb' bytes should be set to 178
    for (size_t i = 0; i < cb; ++i) {
        TEST_ASSERT(ps.image[i] == (png_bytep)178 ? true : false, "Image data should be overwritten with 178 in reuse path");
        if (ps.image[i] != 178) break;
    }

    // Validate row markers for the single image
    for (png_uint_32 y = 0; y < cRows; ++y) {
        png_bytep row = store_image_row(&ps, NULL, 0, y);
        TEST_ASSERT(row != NULL, "store_image_row should not return NULL in reuse path");
        TEST_ASSERT((int)row[-2] == 190, "Row marker -2 should be 190 in reuse path");
        TEST_ASSERT((int)row[-1] == 239, "Row marker -1 should be 239 in reuse path");
        TEST_ASSERT((int)row[(size_t)cbRow] == 222, "Row marker cbRow should be 222 in reuse path");
        TEST_ASSERT((int)row[(size_t)cbRow + 1] == 173, "Row marker cbRow+1 should be 173 in reuse path");
        TEST_ASSERT((int)row[(size_t)cbRow + 2] == 17, "Row marker cbRow+2 should be 17 in reuse path");
    }

    // Cleanup
    free(ps.image);
    ps.image = NULL;
    ps.cb_image = 0;
    ps.cb_row = 0;
    ps.image_h = 0;
}

// Main test runner
int main(void)
{
    fprintf(stderr, "Running unit tests for store_ensure_image (pngvalid.c focal method)\n");

    test_allocate_and_markers_path();
    test_reuse_buffer_path();

    // Print summary
    print_summary();

    // Return non-zero if any test failed
    if (g_total_tests_passed != g_total_tests) {
        fprintf(stderr, "Some tests failed.\n");
        return 1;
    }

    fprintf(stderr, "All tests passed.\n");
    return 0;
}