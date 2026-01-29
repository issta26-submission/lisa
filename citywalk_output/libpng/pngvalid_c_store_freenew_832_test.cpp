// C++11 test suite for the focal method store_freenew in pngvalid.c
// This test suite uses the real libpng structures declared in <png.h>
// and verifies the behavior of store_freenew with respect to the internal
// fields and palette management.
// Note: This test is designed to compile against the existing codebase without
// introducing GTest. It uses lightweight checks and prints results to stdout.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// We assume that the libpng headers expose the following relevant types and
// function prototype for store_freenew:
//   void store_freenew(png_store *ps);
#ifdef __cplusplus
extern "C" {
#endif
// Declaration moved to ensure linkage with the C implementation if not already
// included via <png.h>. If <png.h> already declares it, this is a harmless
// duplicate declaration due to the extern "C" wrapper.
void store_freenew(png_store *ps);
#ifdef __cplusplus
}
#endif

// Lightweight test harness helpers
static bool g_all_passed = true;
#define CHECK(cond, msg) do {                                        \
    if (!(cond)) {                                                     \
        std::cerr << "CHECK FAILED: " << (msg) << "\n";               \
        g_all_passed = false;                                         \
    }                                                                 \
} while (0)


// Test 1: Palette pointer is NULL initially (palette == NULL should skip free path)
// Purpose: Ensure that when ps.palette is NULL, store_freenew still resets
// the scalar fields to the expected values and leaves palette-related fields as NULL.
static bool test_store_freenew_palette_null_case() {
    std::cout << "Running test_store_freenew_palette_null_case...\n";

    // Zero-initialize the png_store to simulate a clean, default state.
    png_store ps;
    std::memset(&ps, 0, sizeof(ps));

    // Call the focal method
    store_freenew(&ps);

    // Validate scalar field resets
    CHECK(ps.writepos == 0, "writepos should be reset to 0");
    CHECK(ps.chunkpos == 8, "chunkpos should be reset to 8");
    CHECK(ps.chunktype == 0, "chunktype should be reset to 0");
    CHECK(ps.chunklen == 16, "chunklen should be reset to 16");
    CHECK(ps.IDAT_size == 0, "IDAT_size should be reset to 0");
    CHECK(ps.IDAT_bits == 0, "IDAT_bits should be reset to 0");

    // Validate palette fields: should remain NULL/0 since initial palette was NULL
    CHECK(ps.palette == NULL, "palette should remain NULL when initially NULL");
    CHECK(ps.npalette == 0, "npalette should remain 0 when initially NULL");

    std::cout << "test_store_freenew_palette_null_case completed.\n";
    return g_all_passed;
}

// Test 2: Palette pointer is non-NULL (path inside if will execute)
// Purpose: Ensure that when ps.palette is non-NULL, store_freenew frees the palette
// memory and resets palette fields as well as the same scalar fields as in Test 1.
// We allocate a small memory block and assign it to ps.palette to trigger the free path.
static bool test_store_freenew_palette_nonnull_case() {
    std::cout << "Running test_store_freenew_palette_nonnull_case...\n";

    // Zero-initialize and then allocate a small chunk for palette
    png_store ps;
    std::memset(&ps, 0, sizeof(ps));

    // Allocate a small palette block and assign to ps.palette
    // We cast through the expected palette pointer type to avoid compile-time
    // type mismatch while still exercising the free path.
    ps.palette = (png_colorp)std::malloc(1);
    if (ps.palette == NULL) {
        std::cerr << "Memory allocation for palette failed in test setup.\n";
        g_all_passed = false;
        return g_all_passed;
    }
    ps.npalette = 1; // indicate one palette entry

    // Call the focal method
    store_freenew(&ps);

    // Validate that the palette was freed and pointers reset
    CHECK(ps.palette == NULL, "palette should be NULL after store_freenew when previously non-NULL");
    CHECK(ps.npalette == 0, "npalette should be 0 after store_freenew when previously non-NULL");

    // Validate scalar field resets (same as Test 1)
    CHECK(ps.writepos == 0, "writepos should be reset to 0");
    CHECK(ps.chunkpos == 8, "chunkpos should be reset to 8");
    CHECK(ps.chunktype == 0, "chunktype should be reset to 0");
    CHECK(ps.chunklen == 16, "chunklen should be reset to 16");
    CHECK(ps.IDAT_size == 0, "IDAT_size should be reset to 0");
    CHECK(ps.IDAT_bits == 0, "IDAT_bits should be reset to 0");

    std::cout << "test_store_freenew_palette_nonnull_case completed.\n";
    return g_all_passed;
}

// Main function to execute tests
int main() {
    std::cout << "Starting unit tests for store_freenew...\n";

    bool ok = true;
    ok = test_store_freenew_palette_null_case() && ok;
    ok = test_store_freenew_palette_nonnull_case() && ok;

    if (ok && g_all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}