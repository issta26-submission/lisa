// Unit test suite for the focal function init_gamma_errors(png_modifier *pm)
// This test suite is written in C++11 (no Google Test) and relies on the
// project under test providing the original libpng context (i.e., the
// png_modifier type and the init_gamma_errors function).
// The tests use a lightweight, non-terminating assertion scheme to maximize
// code coverage without aborting the test run on a single failure.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// Import the C APIs and type definitions from the project.
// We assume the project provides a libpng-compatible header that defines
// the png_modifier type and declares init_gamma_errors.
extern "C" {
// Declaration of the focal method under test. The real implementation lives in pngvalid.c.
void init_gamma_errors(png_modifier *pm);
}

// Simple non-terminating assertion utilities
static int g_failures = 0;

// Macro to print a readable failure message and increment the failure counter.
// It uses operator<< to print values; for non-printable types, ensure appropriate
// overloads exist in the involved types (e.g., double, int, etc.).
#define EXPECT_EQ(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (!(_a == _b)) { \
            std::cerr << "EXPECT_EQ failed: " << #a << " == " << #b \
                      << " (actual: " << _a << ", expected: " << _b \
                      << ") in " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: condition is false at " \
                      << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define RUN_TEST(test) \
    do { \
        std::cout << "Running " << #test << "..." << std::endl; \
        test(); \
    } while (0)


// Test 1: validate that init_gamma_errors resets the specific gamma error fields
// to -1.0 for all the target fields: error_gray_2, error_gray_4, error_gray_8,
// error_color_8, error_indexed, error_gray_16, and error_color_16.
// This ensures that the function implements the exact expected behavior of
// resetting these error accumulators.
void test_init_gamma_errors_resets_target_fields()
{
    // Create a png_modifier instance. The real type is provided by libpng headers.
    // Zero-initialize to simulate a clean slate prior to the function call.
    png_modifier pm;
    std::memset(&pm, 0, sizeof(pm));

    // Assign non-default, non--1 values to verify that the function actually overwrites them.
    pm.error_gray_2   = 1.1;
    pm.error_gray_4   = 2.2;
    pm.error_gray_8   = 3.3;
    pm.error_color_8  = 4.4;
    pm.error_indexed  = 5.5;
    pm.error_gray_16  = 6.6;
    pm.error_color_16 = 7.7;

    // Call the focal method under test
    init_gamma_errors(&pm);

    // Verify that all targeted fields have been reset to -1.0
    EXPECT_EQ(pm.error_gray_2, -1.0);
    EXPECT_EQ(pm.error_gray_4, -1.0);
    EXPECT_EQ(pm.error_gray_8, -1.0);

    EXPECT_EQ(pm.error_color_8, -1.0);

    EXPECT_EQ(pm.error_indexed, -1.0);

    EXPECT_EQ(pm.error_gray_16, -1.0);
    EXPECT_EQ(pm.error_color_16, -1.0);
}


// Test 2: ensure that after calling init_gamma_errors, previously set
// non-default values are overwritten for all the target fields in a single pass.
// This complements Test 1 by re-affirming that no stale values remain.
void test_init_gamma_errors_overwrites_all_fields_consistently()
{
    png_modifier pm;
    std::memset(&pm, 0, sizeof(pm));

    // Set a distinct pattern of values to ensure each field is overwritten
    pm.error_gray_2    = -9.9;
    pm.error_gray_4    = -8.8;
    pm.error_gray_8    = -7.7;
    pm.error_color_8   = -6.6;
    pm.error_indexed   = -5.5;
    pm.error_gray_16   = -4.4;
    pm.error_color_16  = -3.3;

    init_gamma_errors(&pm);

    // All targets should be -1.0 regardless of initial values
    EXPECT_EQ(pm.error_gray_2, -1.0);
    EXPECT_EQ(pm.error_gray_4, -1.0);
    EXPECT_EQ(pm.error_gray_8, -1.0);

    EXPECT_EQ(pm.error_color_8, -1.0);

    EXPECT_EQ(pm.error_indexed, -1.0);

    EXPECT_EQ(pm.error_gray_16, -1.0);
    EXPECT_EQ(pm.error_color_16, -1.0);
}


// Entry point for the test executable
int main(void)
{
    std::cout << "Starting test suite for init_gamma_errors" << std::endl;

    RUN_TEST(test_init_gamma_errors_resets_target_fields);
    RUN_TEST(test_init_gamma_errors_overwrites_all_fields_consistently);

    if (g_failures == 0) {
        std::cout << "All tests passed successfully." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}