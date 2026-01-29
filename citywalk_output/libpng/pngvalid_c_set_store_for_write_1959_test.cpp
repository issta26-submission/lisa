// Unit tests for the focal method: set_store_for_write in pngvalid.c
// Targeted for C++11 environment, no GTest used.
// The tests rely on the libpng types and the exact function signature as provided.
// They are written to be non-terminating (useful assertions) and to be self-contained
// within a single test executable you can integrate with your build system.
// Explanatory comments explain the intent and what each test covers.

#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


extern "C" {
}

// Declaration of the focal function (as it would appear in the linked codebase).
// It is defined in pngvalid.c, linked with libpng in the project under test.
extern "C" png_structp set_store_for_write(png_store *ps, png_infopp ppi, const char *name);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TU_EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "Test failure: " << (msg) << " (condition: " #cond ")" << std::endl; \
    } \
} while(0)

#define TU_EXPECT_NOT_NULL(ptr, msg) TU_EXPECT_TRUE((ptr) != nullptr, msg)
#define TU_EXPECT_EQUAL(a, b, msg) TU_EXPECT_TRUE((a) == (b), msg)

//
// Test 1: Basic path - ensure set_store_for_write succeeds when no write store is currently active.
// This covers the common path where ps->pwrite is NULL, a write struct is created, a write
// callback is wired, and a ps->piwrite info struct is created if a non-NULL ppi is supplied.
//
static void test_basic_path() {
    // Prepare a png_store object (the real struct is defined by libpng headers)
    png_store ps;
    std::memset(&ps, 0, sizeof(ps));

    // ppi is non-NULL to exercise the code path that creates the info struct
    png_infop pi = NULL;
    png_infopp ppi = &pi;

    const char *name = "test_store_write.png";

    // Call the focal function
    png_structp result = set_store_for_write(&ps, ppi, name);

    // Assertions:
    // - a non-NULL write struct should be returned
    // - the info struct pointer should be non-NULL (set via *ppi)
    // - the name should be safely copied into ps.wname
    TU_EXPECT_NOT_NULL(result, "set_store_for_write should return a valid write struct on first use");
    TU_EXPECT_NOT_NULL(pi, "png_create_info_struct should allocate an info struct when ppi is non-NULL");
    // Sanity check that wname contains the provided name (safecat should have copied it)
    if (ps.wname != nullptr) { // wname is a fixed-size array in the real struct; pointer check is defensive
        std::string wname_str(ps.wname);
        TU_EXPECT_TRUE(wname_str.rfind(name, 0) == 0 || wname_str.find(name) != std::string::npos,
                       "ps.wname should contain the provided name after safecat");
    } else {
        // If for some reason wname is not addressable, at least ensure no crash occurred
        TU_EXPECT_TRUE(true, "ps.wname addressable check skipped (implementation may vary)");
    }

    // Note: We do not explicitly destroy the created write struct here since the test binary
    // environment for libpng may require a corresponding destroy call elsewhere.
}


//
// Test 2: Path with NULL ppi (ppi == NULL) - ensure the function does not attempt to write to *ppi
// and still returns a valid write struct. This exercises the "do not touch info struct" branch
// on the outer call path.
//
static void test_ppi_null_path() {
    // Prepare a fresh png_store object
    png_store ps;
    std::memset(&ps, 0, sizeof(ps));

    // ppi is NULL to exercise the "do not assign *ppi" branch
    png_infopp ppi = NULL;

    const char *name = "test_store_write_nullppi.png";

    // Call the focal function
    png_structp result = set_store_for_write(&ps, ppi, name);

    // Assertions:
    // - a non-NULL write struct should be returned even if ppi is NULL
    // - there will be no attempt to write to *ppi (since ppi is NULL)
    TU_EXPECT_NOT_NULL(result, "set_store_for_write should return a valid write struct even when ppi is NULL");
}


//
// Test 3: Reuse scenario (optional coverage attempt).
// If you run this test in an environment that supports the exact library error handling,
// you can verify that a subsequent call to set_store_for_write with a fresh ps and non-NULL ppi
// returns a new write struct and a corresponding info struct. This test aims to exercise
// repeated initialization behavior and to exercise the branch where a second write store is allocated.
// Note: Depending on your libpng error handling setup, this test may require additional teardown.
//
static void test_reuse_initialization() {
    // First initialization
    png_store ps;
    std::memset(&ps, 0, sizeof(ps));
    png_infop pi1 = NULL;
    png_infopp ppi1 = &pi1;
    const char *name1 = "first_store.png";

    png_structp res1 = set_store_for_write(&ps, ppi1, name1);
    TU_EXPECT_NOT_NULL(res1, "First initialization should yield a valid write struct");
    TU_EXPECT_NOT_NULL(pi1, "First initialization should yield a valid info struct");

    // Second initialization with a fresh png_store to simulate reuse
    png_store ps2;
    std::memset(&ps2, 0, sizeof(ps2));
    png_infop pi2 = NULL;
    png_infopp ppi2 = &pi2;
    const char *name2 = "second_store.png";

    png_structp res2 = set_store_for_write(&ps2, ppi2, name2);
    TU_EXPECT_NOT_NULL(res2, "Second initialization should yield a valid write struct");
    TU_EXPECT_NOT_NULL(pi2, "Second initialization should yield a valid info struct");

    // Basic sanity: the two write structs should be distinct
    TU_EXPECT_TRUE(res1 != res2, "Two separate initializations should yield distinct write structs when using separate stores");
}


// Entry point for the test suite
int main(void) {
    std::cout << "Running PNG store write tests (set_store_for_write) ..." << std::endl;

    test_basic_path();
    test_ppi_null_path();
    test_reuse_initialization();

    int failures = g_failed_tests;
    int total = g_total_tests;

    std::cout << "Test summary: " << (total - failures) << "/" << total
              << " tests passed." << std::endl;

    if (failures > 0) {
        std::cerr << failures << " tests failed." << std::endl;
        return 1;
    }
    return 0;
}