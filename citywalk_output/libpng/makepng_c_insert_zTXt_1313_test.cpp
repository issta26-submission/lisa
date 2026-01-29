/*
Unit test suite for the focal method: insert_zTXt in makepng.c
Goal: verify correct integration with libpng text handling without using GTest.
No private methods are accessed; we rely on libpng public API to inspect results.

Notes:
- The tests rely on libpng being available (header <png.h> and linkage to -lpng).
- We test numeric/string aspects using png_get_text to fetch added text chunks.
- We exercise true/false style expectations via custom macros (non-terminating assertions).

Key Candidate Keywords (Step 1): check_param_count, clear_text, text, compression, set_text, png_text, png_get_text, zTXt, insert_zTXt.

This file provides two tests:
- test_insert_zTXt_basic: adds a single zTXt entry and verifies key, value, and compression.
- test_insert_zTXt_multiple_entries: adds two zTXt entries and verifies presence and correctness of both.

All tests are invoked from main(), with a lightweight test harness that reports PASS/FAIL without terminating on first failure.
*/

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


// Declaration of the focal C function (C linkage)
extern "C" void insert_zTXt(png_structp png_ptr, png_infop info_ptr, int nparams, png_charpp params);

// Simple non-terminating assertion helpers
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (static_cast<bool>(cond)) { \
        ++g_tests_passed; \
        std::cout << "[PASS] " << msg << "\n"; \
    } else { \
        ++g_tests_failed; \
        std::cout << "[FAIL] " << msg << "\n"; \
    } \
} while(0)

static bool init_png_structs(png_structp &png_ptr, png_infop &info_ptr) {
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "Failed to create png write struct\n";
        return false;
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cerr << "Failed to create png info struct\n";
        png_destroy_write_struct(&png_ptr, NULL);
        return false;
    }
    return true;
}

// Cleanup helper to destroy PNG structures
static void cleanup_png(png_structp &png_ptr, png_infop &info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else {
        // still attempt to destroy if only partial was created
        if (info_ptr) png_destroy_info_struct(png_ptr, &info_ptr);
    }
    png_ptr = NULL;
    info_ptr = NULL;
}

// Test 1: Basic insertion into PNG via insert_zTXt and verify keyword/text and compression
static void test_insert_zTXt_basic() {
    std::cout << "Running test_insert_zTXt_basic...\n";

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    if (!init_png_structs(png_ptr, info_ptr)) {
        EXPECT_TRUE(false, "test_insert_zTXt_basic: PNG struct initialization failed");
        return;
    }

    // Prepare parameters: [keyword, text]
    const char *kw = "Comment";
    const char *val = "HelloWorld";

    png_charpp params = new png_charp[2];
    // Allocate modifiable buffers (libpng expects char*)
    char *kw_copy = strdup(kw);
    char *val_copy = strdup(val);
    params[0] = kw_copy;
    params[1] = val_copy;

    // Call the focal function
    insert_zTXt(png_ptr, info_ptr, 2, params);

    // Retrieve text chunks via libpng API
    png_textp text_ptr = NULL;
    int num_text = 0;
    // png_get_text signature assumed:
    // int png_get_text(png_structp, png_infop, png_textp *, int *);
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);

    bool found = false;
    if (text_ptr && num_text > 0) {
        for (int i = 0; i < num_text; ++i) {
            if (text_ptr[i].key != nullptr && std::strcmp(text_ptr[i].key, kw) == 0) {
                found = true;
                // Verify content length and content (best-effort, depends on libpng behavior)
                if (text_ptr[i].text != nullptr) {
                    size_t len = static_cast<size_t>(text_ptr[i].text_length);
                    if (len == std::strlen(val)) {
                        if (std::strncmp(text_ptr[i].text, val, len) == 0) {
                            // Content matches
                            EXPECT_TRUE(true, "test_insert_zTXt_basic: keyword/value match and length");
                        } else {
                            EXPECT_TRUE(false, "test_insert_zTXt_basic: keyword matches but text content differs");
                        }
                    } else {
                        EXPECT_TRUE(false, "test_insert_zTXt_basic: keyword text length mismatch");
                    }
                } else {
                    EXPECT_TRUE(false, "test_insert_zTXt_basic: keyword found but text is null");
                }
                // Check compression flag (should be 0 for zTXt in this API)
                if (text_ptr[i].compression == 0) {
                    EXPECT_TRUE(true, "test_insert_zTXt_basic: compression == 0 as expected");
                } else {
                    EXPECT_TRUE(false, "test_insert_zTXt_basic: compression != 0");
                }
                break;
            }
        }
        if (!found) {
            EXPECT_TRUE(false, "test_insert_zTXt_basic: keyword not found in text chunks");
        }
    } else {
        EXPECT_TRUE(false, "test_insert_zTXt_basic: no text chunks retrieved");
    }

    // Cleanup
    delete[] params;
    free(kw_copy);
    free(val_copy);
    // libpng does not require explicit free of text_ptr; it is managed internally.

    cleanup_png(png_ptr, info_ptr);
}

// Test 2: Insert two zTXt entries and verify both exist
static void test_insert_zTXt_multiple_entries() {
    std::cout << "Running test_insert_zTXt_multiple_entries...\n";

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    if (!init_png_structs(png_ptr, info_ptr)) {
        EXPECT_TRUE(false, "test_insert_zTXt_multiple_entries: PNG struct initialization failed");
        return;
    }

    // First entry
    const char *kw1 = "Comment";
    const char *val1 = "FirstEntry";

    // Second entry
    const char *kw2 = "Author";
    const char *val2 = "UnitTester";

    png_charpp params = new png_charp[2];
    char *kw1_copy = strdup(kw1);
    char *val1_copy = strdup(val1);
    params[0] = kw1_copy;
    params[1] = val1_copy;

    insert_zTXt(png_ptr, info_ptr, 2, params);

    // Second call
    // Update params for second entry
    free(kw1_copy);
    free(val1_copy);
    kw1_copy = strdup(kw2);
    val1_copy = strdup(val2);
    // Reuse array for second insertion by reallocating
    png_charpp params2 = new png_charp[2];
    params2[0] = kw1_copy;
    params2[1] = val1_copy;

    insert_zTXt(png_ptr, info_ptr, 2, params2);

    // Retrieve text chunks
    png_textp text_ptr = NULL;
    int num_text = 0;
    png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);

    bool found_kw1 = false;
    bool found_kw2 = false;
    if (text_ptr && num_text > 0) {
        for (int i = 0; i < num_text; ++i) {
            if (text_ptr[i].key != nullptr) {
                if (std::strcmp(text_ptr[i].key, kw1) == 0) {
                    found_kw1 = true;
                    // Optional: verify content
                    if (text_ptr[i].text != nullptr && text_ptr[i].text_length == std::strlen(val1)) {
                        if (std::strncmp(text_ptr[i].text, val1, text_ptr[i].text_length) != 0) {
                            EXPECT_TRUE(false, "test_insert_zTXt_multiple_entries: kw1 value mismatch");
                        }
                    }
                    if (text_ptr[i].compression != 0) {
                        EXPECT_TRUE(false, "test_insert_zTXt_multiple_entries: kw1 compression not 0");
                    }
                } else if (std::strcmp(text_ptr[i].key, kw2) == 0) {
                    found_kw2 = true;
                    if (text_ptr[i].text != nullptr && text_ptr[i].text_length == std::strlen(val2)) {
                        if (std::strncmp(text_ptr[i].text, val2, text_ptr[i].text_length) != 0) {
                            EXPECT_TRUE(false, "test_insert_zTXt_multiple_entries: kw2 value mismatch");
                        }
                    }
                    if (text_ptr[i].compression != 0) {
                        EXPECT_TRUE(false, "test_insert_zTXt_multiple_entries: kw2 compression not 0");
                    }
                }
            }
        }
    }

    EXPECT_TRUE(found_kw1 && found_kw2, "test_insert_zTXt_multiple_entries: both keywords found");

    // Cleanup
    delete[] params;
    delete[] params2;
    free(kw1_copy); free(val1_copy);
    free((void*)kw2);
    free((void*)val2);

    cleanup_png(png_ptr, info_ptr);
}

// Main runner
int main() {
    std::cout << "Starting insert_zTXt unit tests (no GTest, manual harness)..." << std::endl;

    test_insert_zTXt_basic();
    test_insert_zTXt_multiple_entries();

    std::cout << "\nTest results: "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total.\n";

    return (g_tests_failed > 0) ? 1 : 0;
} 

/* End of test suite for insert_zTXt */