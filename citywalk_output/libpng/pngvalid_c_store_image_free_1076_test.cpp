// Minimal C++11 test harness for the focal method: store_image_free
// This test suite is self-contained and does not rely on external test frameworks (no GTest).
// It provides a lightweight assertion mechanism and mocks for the required dependencies
// to exercise all branches of the focal method as described in the prompt.

// The test suite purpose:
// - Verify behavior when ps->image is NULL (no-op)
// - Verify behavior when image has correct sentinel values (overwrite would not be triggered)
// - Verify behavior when sentinel checks fail and pp is non-NULL (png_error is invoked)
// - Verify behavior when sentinel checks fail and pp is NULL (store_log is invoked)

#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <math.h>


// ------------------------------------------------------------------------------------
// Minimal type and structure definitions to mirror the focal function's usage.

typedef unsigned char* png_bytep;
typedef const void*    png_const_structp;

// Lightweight internal representation of png_store used for testing.
// We only require the members accessed by the focal method.
struct png_store {
    png_bytep image;
    size_t cb_image;
};

// ------------------------------------------------------------------------------------
// Mocks for dependencies invoked by the focal method.

// Global state to capture calls made by the focal method under test.
static bool error_called = false;
static std::string last_error_message;

static std::string last_log_message;

// Mock implementation of png_error (C linkage to resemble real usage)
extern "C" void png_error(png_const_structp pp, const char* message)
{
    (void)pp; // unused in tests
    error_called = true;
    last_error_message = message;
}

// Mock implementation of store_log (C linkage to resemble real usage)
extern "C" void store_log(png_store *ps, png_const_structp pp, const char *message, int is_error)
{
    (void)ps;
    (void)pp;
    (void)is_error;
    last_log_message = message;
}

// ------------------------------------------------------------------------------------
// Re-implementation of the focal method (copied here to enable isolated unit testing
// without requiring the full libpng internal test harness).
// The implementation mirrors the logic exactly as provided in the prompt.

void store_image_free(png_store *ps, png_const_structp pp)
{
{
   if (ps->image != NULL)
   {
      png_bytep image = ps->image;
      if (image[-1] != 0xed || image[ps->cb_image] != 0xfe)
      {
         if (pp != NULL)
            png_error(pp, "png_store image overwrite (1)");
         else
            store_log(ps, NULL, "png_store image overwrite (2)", 1);
      }
      ps->image = NULL;
      ps->cb_image = 0;
      --image;
      free(image);
   }
}
}

// ------------------------------------------------------------------------------------
// Lightweight, self-contained test harness

// Simple test assertion helpers
static int tests_passed = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "Expectation failed: " #cond " (" __FILE__ ":" << __LINE__ << ")\n"; \
            ++tests_failed; \
        } else { \
            ++tests_passed; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        if (!((a) == (b))) { \
            std::cerr << "Expectation failed: " #a " == " #b " (" __FILE__ ":" << __LINE__ << ")\n"; \
            std::cerr << "  actual: " << (a) << "  expected: " << (b) << "\n"; \
            ++tests_failed; \
        } else { \
            ++tests_passed; \
        } \
    } while (0)

#define EXPECT_STR_EQ(a, b) \
    do { \
        if (std::string((a)) != std::string((b))) { \
            std::cerr << "Expectation failed: strings differ (" __FILE__ ":" << __LINE__ << ")\n"; \
            std::cerr << "  actual: " << (a) << "  expected: " << (b) << "\n"; \
            ++tests_failed; \
        } else { \
            ++tests_passed; \
        } \
    } while (0)

// Helper to create a memory block with the necessary sentinel layout:
// base[0] -> sentinel for image[-1]
// base[1] -> image data's first byte (content not important for tests)
// base[2] -> used as image[cb_image] index when cb_image == 1 (as per test plan)
static png_bytep create_image_block(bool sentinel_ok, size_t cb_image, png_store &ps)
{
    // We allocate 3 bytes to satisfy image[-1] (base[0]), image[cb_image] (base[cb_index])
    // and the image data area starting at base[1].
    png_bytep base = (png_bytep)std::malloc(3);
    if (base == NULL) return NULL;

    if (sentinel_ok) {
        base[0] = 0xed;
        // base[1] can be any content
        base[2] = 0xfe;
    } else {
        // Mismatch sentinel values to trigger overwrite checks
        base[0] = sentinel_ok ? 0xed : 0x00; // explicitly non-0xed when test requires
        base[2] = 0x00;
    }

    ps.image = base + 1; // image points just after the sentinel at base[0]
    ps.cb_image = cb_image; // will read image[cb_image] (base[cb_image + 1])
    return base;
}

// Test 1: image is NULL -> function should be a no-op and not crash
static bool test_no_op_when_image_null()
{
    // Reset diagnostics
    error_called = false;
    last_error_message.clear();
    last_log_message.clear();

    png_store ps;
    ps.image = NULL;
    ps.cb_image = 0;

    store_image_free(&ps, NULL);

    // Assertions
    EXPECT_TRUE(ps.image == NULL);
    EXPECT_EQ(ps.cb_image, 0u);
    EXPECT_TRUE(error_called == false);
    EXPECT_STR_EQ(last_log_message.c_str(), "");
    return true;
}

// Test 2: image present with correct sentinel values -> no error or log, memory freed
static bool test_no_overwrite_logged_when_sentinel_ok()
{
    error_called = false;
    last_error_message.clear();
    last_log_message.clear();

    png_store ps;
    png_bytep base = create_image_block(true, 1, ps); // sentinel_ok = true, cb_image = 1
    if (base == NULL) return false;

    // Call focal method
    store_image_free(&ps, NULL);

    // Assertions
    EXPECT_TRUE(ps.image == NULL);
    EXPECT_EQ(ps.cb_image, 0u);
    EXPECT_TRUE(error_called == false);
    EXPECT_STR_EQ(last_error_message.c_str(), "");
    EXPECT_STR_EQ(last_log_message.c_str(), ""); // no log should be emitted
    // Free the base memory if not already freed by the focal method
    // (The focal method frees the memory block: image-1 == base)
    // However, we still need to avoid double-free; since focal frees, base is freed.
    // We cannot free base here safely; rely on focal to have freed it.

    return true;
}

// Test 3a: sentinel mismatch and non-NULL pp -> png_error should be invoked
static bool test_overwrite_error_called_when_sentinal_mismatch_and_pp_non_null()
{
    error_called = false;
    last_error_message.clear();
    last_log_message.clear();

    png_store ps;
    // sentinel mismatch: image[-1] != 0xed or image[cb_image] != 0xfe
    png_bytep base = create_image_block(false, 1, ps);
    if (base == NULL) return false;

    // Pass a non-NULL pp to trigger png_error path
    store_image_free(&ps, (png_const_structp)0x1);

    // Assertions
    EXPECT_TRUE(ps.image == NULL);
    EXPECT_EQ(ps.cb_image, 0u);
    EXPECT_TRUE(error_called == true);
    EXPECT_STR_EQ(last_error_message.c_str(), "png_store image overwrite (1)");
    EXPECT_STR_EQ(last_log_message.c_str(), ""); // no log should be emitted
    // Memory freed by focal
    return true;
}

// Test 3b: sentinel mismatch and NULL pp -> store_log should be invoked
static bool test_overwrite_log_called_when_sentinal_mismatch_and_pp_null()
{
    error_called = false;
    last_error_message.clear();
    last_log_message.clear();

    png_store ps;
    png_bytep base = create_image_block(false, 1, ps);
    if (base == NULL) return false;

    // Pass NULL pp to trigger store_log path
    store_image_free(&ps, NULL);

    // Assertions
    EXPECT_TRUE(ps.image == NULL);
    EXPECT_EQ(ps.cb_image, 0u);
    EXPECT_TRUE(error_called == false);
    EXPECT_STR_EQ(last_error_message.c_str(), "");
    EXPECT_STR_EQ(last_log_message.c_str(), "png_store image overwrite (2)");
    return true;
}

// Helper to free memory blocks created for tests that rely on dynamic allocation.
// The focal function performs a free on image-1; base points to that memory.
static void cleanup_memory(png_store &ps)
{
    if (ps.image != NULL) {
        // image was allocated as base+1; image-1 points to base
        png_bytep base = ps.image - 1;
        std::free(base);
        ps.image = NULL;
        ps.cb_image = 0;
    }
}

// Main entry: run all tests
int main(void)
{
    std::cout << "Starting store_image_free unit tests (self-contained harness)\n";

    bool t1 = test_no_op_when_image_null();
    cleanup_memory(*(new png_store{NULL, 0})); // safe no-op; memory not allocated

    bool t2 = test_no_overwrite_logged_when_sentinel_ok();
    // Clean up any residual memory if test allocated one
    // The test frees base inside focal; nothing else to free here.

    bool t3a = test_overwrite_error_called_when_sentinal_mismatch_and_pp_non_null();
    bool t3b = test_overwrite_log_called_when_sentinal_mismatch_and_pp_null();

    // Summary
    int total = 4;
    int passed = tests_passed;
    int failed = tests_failed;

    std::cout << "Tests passed: " << passed << "/" << total << "\n";
    if (failed > 0) {
        std::cout << "Some tests failed. See error messages above.\n";
        return 1;
    }
    return 0;
}