// File: pngimage_test_helpers.h
#ifndef PNGIMAGE_TEST_HELPERS_H
#define PNGIMAGE_TEST_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

// Minimal representation of the display struct used by the focal method.
// This mirrors the fields accessed by display_clean in pngimage.c.
// Note: This is a test-only definition to allow probing the behavior
// of display_clean from C++ test code without exposing private
// implementation details of the original C file.
struct display {
    int original_rowbytes;
    void *original_rows;
    int chunks;
    void *original_pp;
    void *original_ip;
    int results;
    const char *filename;
};

// Focal method under test (C linkage)
void display_clean(struct display *dp);

#ifdef __cplusplus
}
#endif

#endif // PNGIMAGE_TEST_HELPERS_H



// File: test_display_clean_no_define.cpp
// Purpose: Test display_clean behavior when PNG_WRITE_PNG_SUPPORTED is NOT defined.
// This exercises the non-write-path and validates that fields are reset correctly.

#include <stdarg.h>
#include <string.h>
#include <pngimage_test_helpers.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>


// Simple non-GTest/test framework (non-terminating assertions)
#define EXPECT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT FAILED: " << (msg) << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

static int g_failures = 0;

// Test: display_clean should reset internal state fields and not touch the filename.
// We assume that when PNG_WRITE_PNG_SUPPORTED is not defined, the write path is absent
// and only the read/cleanup path is executed.
static void test_display_clean_resets_fields_no_define()
{
    struct display dp;
    dp.original_rowbytes = 1234;
    dp.original_rows = (void*)0xDEADBEEF;
    dp.chunks = 5;
    dp.original_pp = (void*)0x1111;
    dp.original_ip = (void*)0x2222;
    dp.results = 7;
    dp.filename = "err.png";

    // Call the focal method
    display_clean(&dp);

    // Validate that fields have been reset as per implementation
    EXPECT(dp.original_rowbytes == 0, "original_rowbytes should be reset to 0");
    EXPECT(dp.original_rows == nullptr, "original_rows should be reset to NULL");
    EXPECT(dp.chunks == 0, "chunks should be reset to 0");
    // png_destroy_read_struct is expected to NULL out the libpng pointers.
    // The exact behavior is library-dependent, but standard usage nulls the struct pointers.
    // We check for NULL as a robust expectation for this unit under test.
    EXPECT(dp.original_pp == nullptr, "original_pp should be NULL after cleanup");
    EXPECT(dp.original_ip == nullptr, "original_ip should be NULL after cleanup");
    EXPECT(dp.results == 0, "results should be reset to 0");
    // Filename should be preserved for error detection
    EXPECT(std::strcmp(dp.filename, "err.png") == 0, "filename should remain unchanged");
}

// Entry point
int main()
{
    test_display_clean_resets_fields_no_define();

    if(g_failures > 0)
        std::cout << "TEST SUMMARY: " << g_failures << " failure(s) detected." << std::endl;
    else
        std::cout << "TEST SUMMARY: All tests passed." << std::endl;

    return g_failures ? 1 : 0;
}



// File: test_display_clean_with_define.cpp
// Purpose: Test display_clean behavior when PNG_WRITE_PNG_SUPPORTED is defined.
// This exercises the write-path integration (though the write function is a stub
// in the provided focal-class dependency, the branch is exercised for coverage).

#define PNG_WRITE_PNG_SUPPORTED 1


// Simple non-GTest/test framework (non-terminating assertions)
#define EXPECT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT FAILED: " << (msg) << " (in " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

static int g_failures = 0;

// Test: display_clean should reset internal state fields and not crash, even with write-path enabled.
static void test_display_clean_resets_fields_with_define()
{
    struct display dp;
    dp.original_rowbytes = 5678;
    dp.original_rows = (void*)0xA1A2A3A4;
    dp.chunks = 9;
    dp.original_pp = (void*)0x1A2B3C4D;
    dp.original_ip = (void*)0x4D3C2B1A;
    dp.results = 42;
    dp.filename = "image_with_define.png";

    // Call the focal method
    display_clean(&dp);

    // Validate that fields have been reset as per implementation
    EXPECT(dp.original_rowbytes == 0, "original_rowbytes should be reset to 0 (define path)");
    EXPECT(dp.original_rows == nullptr, "original_rows should be reset to NULL (define path)");
    EXPECT(dp.chunks == 0, "chunks should be reset to 0 (define path)");
    // After png_destroy_read_struct called with &dp->original_pp and &dp->original_ip,
    // typical behavior is to NULL out these pointers.
    EXPECT(dp.original_pp == nullptr, "original_pp should be NULL after cleanup (define path)");
    EXPECT(dp.original_ip == nullptr, "original_ip should be NULL after cleanup (define path)");
    EXPECT(dp.results == 0, "results should be reset to 0 (define path)");
    // Filename should be preserved for error detection
    EXPECT(std::strcmp(dp.filename, "image_with_define.png") == 0, "filename should remain unchanged (define path)");
}

// Entry point
int main()
{
    test_display_clean_resets_fields_with_define();

    if(g_failures > 0)
        std::cout << "TEST SUMMARY (DEFINE PATH): " << g_failures << " failure(s) detected." << std::endl;
    else
        std::cout << "TEST SUMMARY (DEFINE PATH): All tests passed." << std::endl;

    return g_failures ? 1 : 0;
}