// Test suite for the get_valid(display *d, png_infop info_ptr) function
// This test is crafted to work with the provided FOCAL_METHOD implementation
// without depending on GTest. It uses a lightweight, non-terminating assertion
// style and a small internal test harness to simulate libpng interactions.
//
// Key points implemented:
// - Mocked libpng interface (png_get_valid, png_get_text) via a local png.h header.
// - A fake "display" struct layout compatible with the expected usage in get_valid.
// - Non-terminating EXPECT-style assertions.
// - Test cases covering true/false branches for text presence and compression values.
// - Handling of the unknown compression branch via longjmp-based exit interception.

#include <cstdint>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>


// -------------------- Mocked libpng header (png.h) --------------------
// Provide minimal declarations needed by the focal code under test.

#ifndef PNG_H
#define PNG_H

typedef unsigned int png_uint_32;
typedef void* png_structp;
typedef void* png_infop;
typedef struct png_text png_text;
typedef png_text* png_textp;

// Bit flags used by get_valid (as assumed by the focal method)
#define PNG_INFO_tEXt (1 << 0)
#define PNG_INFO_zTXt (1 << 1)
#define PNG_INFO_iTXt (1 << 2)

// Simple png_text struct used by tests
struct png_text {
    int compression; // Only 'compression' field is used by get_valid
};

// Pointer type for arrays of png_text
typedef png_text* png_textp;

// The actual functions are provided by the focal C file (pngunknown.c).
// We provide test-time overrides (mocks) for these symbols below.
extern "C" {
    png_uint_32 png_get_valid(png_structp, png_infop, png_uint_32);
    png_uint_32 png_get_text(png_structp, png_infop, png_textp*, void*);
}

// Forward declaration of the actual FOCAL_TYPE "display" used by get_valid.
// We declare together with an incomplete layout to allow our test to construct
// an instance that matches the presumed layout used by the focal method.
typedef struct display display;

// The actual layout in the focal file is expected to be:
// typedef struct display {
//     void *png_ptr;
//     const char *file;
//     const char *test;
// } display;
struct display {
    void* png_ptr;
    const char* file;
    const char* test;
};

// Forward declaration of the focal method (C linkage)
extern "C" unsigned int get_valid(display *d, png_infop info_ptr);

#endif // PNG_H

// -------------------- Test harness and mocks --------------------

static jmp_buf test_jmp;        // Jump buffer for intercepting exit path
static int g_test_exit_seen = 0;  // Flag indicating exit path was triggered

// Global state used by the mocked png_get_valid and png_get_text
static png_uint_32 g_base_flags = 0;
static int g_ntext = 0;
static png_text g_text_buf[32];

// Helpers for test harness
static void set_png_base_flags(png_uint_32 v) {
    g_base_flags = v;
}
static void set_texts(const std::vector<int>& compressions) {
    g_ntext = (int)compressions.size();
    for (size_t i = 0; i < compressions.size() && i < (size_t) (sizeof(g_text_buf)/sizeof(g_text_buf[0])); ++i) {
        g_text_buf[i].compression = compressions[i];
    }
}

// Overrides/mocks for libpng functions used by get_valid

extern "C" {

png_uint_32 png_get_valid(png_structp /*png_ptr*/, png_infop /*info_ptr*/, png_uint_32 /*mask*/) {
    // Return the configured base flags for the test scenario
    return g_base_flags;
}

png_uint_32 png_get_text(png_structp /*png_ptr*/, png_infop /*info_ptr*/, png_textp* text_ptr, void* /*text_ptr_mask*/) {
    // Provide the currently configured text array and count to the focal code
    *text_ptr = g_text_buf;
    return (png_uint_32)g_ntext;
}

// Intercept the exit path invoked for unknown text compression
void display_exit(display *d) {
    // Indicate we hit the "unknown compression" path and longjmp back to test driver
    (void)d; // suppress unused parameter warning if not actually used
    g_test_exit_seen = 1;
    longjmp(test_jmp, 1);
}

} // extern "C"

// The test driver with non-terminating, descriptive assertions

static int g_test_total = 0;
static int g_test_passed = 0;

#define EXPECT_EQ(actual, expected, desc) do { \
    ++g_test_total; \
    if ((actual) == (expected)) { \
        ++g_test_passed; \
        std::cout << "[PASS] " << desc << "\n"; \
    } else { \
        std::cerr << "[FAIL] " << desc << " - got " << (actual) << ", expected " << (expected) << "\n"; \
    } \
} while(0)

static void test_no_text_case() {
    // DESC: No text chunks present; ensure flags are returned unchanged
    set_png_base_flags(0x10); // Base flags from png_get_valid
    set_texts({});            // No text chunks

    display d;
    d.png_ptr = nullptr;
    d.file = "test_file";
    d.test = "no_text";

    png_uint_32 result = get_valid(&d, nullptr);

    EXPECT_EQ(result, 0x10u, "get_valid should return base flags when no text chunks exist");
}

static void test_text_tEXt_case() {
    // DESC: One text chunk with compression -1 should set PNG_INFO_tEXt
    set_png_base_flags(0);        // Base flags start at 0
    set_texts({-1});               // One text chunk with compression -1

    display d;
    d.png_ptr = nullptr;
    d.file = "test_file";
    d.test = "tEXt_case";

    png_uint_32 result = get_valid(&d, nullptr);

    EXPECT_EQ(result, (png_uint_32)PNG_INFO_tEXt, "get_valid should map -1 compression to tEXt");
}

static void test_text_zTXt_case() {
    // DESC: One text chunk with compression 0 should set PNG_INFO_zTXt
    set_png_base_flags(0);
    set_texts({0});

    display d;
    d.png_ptr = nullptr;
    d.file = "test_file";
    d.test = "zTXt_case";

    png_uint_32 result = get_valid(&d, nullptr);

    EXPECT_EQ(result, (png_uint_32)PNG_INFO_zTXt, "get_valid should map 0 compression to zTXt");
}

static void test_text_iTXt_case() {
    // DESC: One text chunk with compression 2 should map to iTXt
    set_png_base_flags(0);
    set_texts({2});

    display d;
    d.png_ptr = nullptr;
    d.file = "test_file";
    d.test = "iTXt_case";

    png_uint_32 result = get_valid(&d, nullptr);

    EXPECT_EQ(result, (png_uint_32)PNG_INFO_iTXt, "get_valid should map 2 compression to iTXt");
}

static void test_unknown_compression_case() {
    // DESC: Unknown compression value should trigger exit path (display_exit)
    // Expect longjmp back to test harness
    set_png_base_flags(0);
    set_texts({3}); // Unknown compression (not -1, 0, 1, 2)

    display d;
    d.png_ptr = nullptr;
    d.file = "test_file";
    d.test = "unknown_case";

    if (setjmp(test_jmp) == 0) {
        g_test_exit_seen = 0;
        // If the exit path is correctly triggered, this call will longjmp
        get_valid(&d, nullptr);
        // If we reach here, the exit did not trigger as expected
        ++g_test_total;
        std::cerr << "[FAIL] unknown_case - expected exit via display_exit, but get_valid returned\n";
    } else {
        // We returned here via longjmp from display_exit, path is correct
        ++g_test_passed;
        std::cout << "[PASS] unknown_case - exit path triggered as expected\n";
        ++g_test_total;
    }
}

// -------------------- Main runner --------------------

int main() {
    // Run all tests
    test_no_text_case();
    test_text_tEXt_case();
    test_text_zTXt_case();
    test_text_iTXt_case();
    test_unknown_compression_case();

    // Summary
    std::cout << "\nTest summary: " << g_test_passed << " passed, " << g_test_total - g_test_passed
              << " failed, out of " << g_test_total << " tests.\n";

    // Return non-zero if any test failed
    if (g_test_passed == g_test_total) {
        return 0;
    } else {
        return 1;
    }
}