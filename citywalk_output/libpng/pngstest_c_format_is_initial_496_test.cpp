// This test suite targets the focal function: format_is_initial
// Source context (from the focal file):
//   int format_is_initial(format_list *pf)
//   {
//     int i;
//     for (i=0; i<FORMAT_SET_COUNT; ++i)
//        if (pf->bits[i] != 0)
//           return 0;
//     return 1;
//   }
//
// Candidate Keywords (Step 1): format_list, bits, FORMAT_SET_COUNT, pf->bits[i], non-zero check, initial state.
//
// Notes:
// - We assume that the project's png.h (or equivalent) provides the definition of format_list
//   including the member: png_uint_32 bits[FORMAT_SET_COUNT] (or equivalent).
// - The test uses non-terminating checks by recording failures and continuing (no aborting).
// - We compile this test as a separate translation unit and link against the existing pngstest.c
//   (which implements format_is_initial and the related types).

#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


// Bring in PNG-specific types and the function under test.
// We rely on the project to expose format_list and FORMAT_SET_COUNT via png.h
extern "C" {
int format_is_initial(format_list *pf);
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void record_result(bool pass, const char* test_name, const char* detail) {
    ++g_total_tests;
    if (!pass) {
        ++g_failed_tests;
        std::cerr << "[FAILED] " << test_name;
        if (detail) std::cerr << ": " << detail;
        std::cerr << std::endl;
    } else {
        std::cout << "[PASSED] " << test_name;
        if (detail) std::cout << " -- " << detail;
        std::cout << std::endl;
    }
}

// Test 1: All bits are zero -> format_is_initial should return 1 (true)
static void test_format_is_initial_all_zero()
{
    format_list pf;
    // Initialize entire structure to zeros to ensure a clean initial state
    std::memset(&pf, 0, sizeof(pf));

    int result = format_is_initial(&pf);
    bool pass = (result == 1);
    record_result(pass, "test_format_is_initial_all_zero", "expected 1 when all bits are zero");
}

// Test 2: First bit non-zero -> format_is_initial should return 0 (false)
static void test_format_is_initial_first_bit_nonzero()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    // Set the first bit to a non-zero value to simulate an active format
    pf.bits[0] = 1;

    int result = format_is_initial(&pf);
    bool pass = (result == 0);
    record_result(pass, "test_format_is_initial_first_bit_nonzero", "expected 0 when pf.bits[0] != 0");
}

// Test 3: Last bit non-zero -> format_is_initial should return 0 (false)
static void test_format_is_initial_last_bit_nonzero()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    // Set the last valid bit index to non-zero
    pf.bits[FORMAT_SET_COUNT - 1] = 1;

    int result = format_is_initial(&pf);
    bool pass = (result == 0);
    record_result(pass, "test_format_is_initial_last_bit_nonzero", "expected 0 when pf.bits[FORMAT_SET_COUNT-1] != 0");
}

// Test 4: Multiple non-zero bits -> format_is_initial should return 0 (false)
static void test_format_is_initial_multiple_nonzero_bits()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    // Set multiple non-zero entries
    pf.bits[0] = 2;
    pf.bits[FORMAT_SET_COUNT/2] = 3;

    int result = format_is_initial(&pf);
    bool pass = (result == 0);
    record_result(pass, "test_format_is_initial_multiple_nonzero_bits", "expected 0 when any pf.bits[i] != 0");
}

// Optional: Test that after resetting to zero, the function returns 1 again
static void test_format_is_initial_reset_to_zero()
{
    format_list pf;
    std::memset(&pf, 0, sizeof(pf));

    pf.bits[0] = 4;
    int r1 = format_is_initial(&pf);
    bool first_nonzero = (r1 == 0);

    // Reset to zero
    pf.bits[0] = 0;
    int r2 = format_is_initial(&pf);
    bool second_zero = (r2 == 1);

    record_result(!first_nonzero && second_zero, "test_format_is_initial_reset_to_zero", "expected: first non-zero yields 0, after reset all-zero yields 1");
}

// Main function to run tests (Step 3: Test Case Refinement)
int main() {
    // Run tests to cover true/false branches of the predicate pf->bits[i] != 0
    test_format_is_initial_all_zero();
    test_format_is_initial_first_bit_nonzero();
    test_format_is_initial_last_bit_nonzero();
    test_format_is_initial_multiple_nonzero_bits();
    test_format_is_initial_reset_to_zero();

    // Summary
    std::cout << "\nTest Summary: ";
    std::cout << g_total_tests << " run, ";
    std::cout << g_failed_tests << " failed." << std::endl;

    // Return non-zero if any test failed (helps CI detect failures)
    return (g_failed_tests != 0) ? 1 : 0;
}

// End of test suite for format_is_initial
// Notes for maintainers:
// - This suite assumes the project exposes format_list and FORMAT_SET_COUNT via png.h.
// - If the project uses different include paths, adjust the <png.h> include accordingly.
// - The tests use non-terminating assertions to maximize code execution coverage, as requested.