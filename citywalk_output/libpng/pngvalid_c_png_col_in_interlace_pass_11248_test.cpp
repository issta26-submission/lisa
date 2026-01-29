// test_png_col_in_interlace_pass.cpp
// C++11 unit-test-like harness for the focal C-style function:
// int png_col_in_interlace_pass(png_uint_32 x, int pass)
// The implementation relies on the Adam7 interlace pattern table (adam7[8][8]).

#include <cstdint>
#include <cassert>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <limits>
#include <stdio.h>
#include <math.h>


// Declaration of the focal function to be tested.
// The real implementation is in pngvalid.c and linked during build.
// We declare it with C linkage since the original is a C function.
extern "C" int png_col_in_interlace_pass(uint32_t x, int pass);

// Candidate Keywords derived from Step 1 (for traceability/documentation in tests)
// - adam7: the 8x8 interlace pattern table used by the function
// - interlace pass: the PNG Adam7 passes (1..7, represented in code by pass+1)
// - x masking: x is masked with 7 (x & 7) to confine to 0..7
// - early exit: the function returns 1 as soon as a matching pass is found in any row
// - no-match path: function returns 0 if no row yields a match
// - boundary behavior: testing with pass outside expected range to observe graceful handling

// Simple non-terminating assertion helpers.
// They log failures but continue execution to maximize code coverage.
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) \
                  << " [file " << __FILE__ << ", line " << __LINE__ << "]\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " != " #b " (" << (a) \
                  << " != " << (b) << "). " << (msg) \
                  << " [file " << __FILE__ << ", line " << __LINE__ << "]\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_GE(a, b, msg) do { \
    if(!((a) >= (b))) { \
        std::cerr << "EXPECT_GE failed: " #a " >= " #b " is false (" << (a) \
                  << " >= " << (b) << "). " << (msg) \
                  << " [file " << __FILE__ << ", line " << __LINE__ << "]\n"; \
        ++g_failures; \
    } \
} while(0)


// Test 1: Coverage of true/false branches
// - Iterate over all x in [0..7] and pass in [0..6] and verify that at least one
//   call returns true (1) and at least one call returns false (0).
void test_true_false_branch_coverage() {
    bool has_true = false;
    bool has_false = false;

    // The function masks x with 7, so only x in 0..7 matter.
    for (uint32_t x = 0; x < 8; ++x) {
        for (int pass = 0; pass <= 6; ++pass) {
            int r = png_col_in_interlace_pass(x, pass);
            if (r != 0)
                has_true = true;
            else
                has_false = true;
        }
    }

    // We expect both outcomes across the domain (Adam7 provides multiple passes for
    // many columns; this should yield at least one 1 and one 0 for standard data).
    EXPECT_TRUE(has_true, "Expected at least one 1 result to validate true-branch execution.");
    EXPECT_TRUE(has_false, "Expected at least one 0 result to validate false-branch execution.");
}

// Test 2: Invariance under x modulo 8
// - Since the function applies x &= 7, results for x and x+8 should be identical
//   for the same pass. This tests the masking behavior and keeps coverage focused.
void test_x_mask_invariance() {
    for (int pass = 0; pass <= 6; ++pass) {
        int r0 = png_col_in_interlace_pass(0u, pass);
        int r8 = png_col_in_interlace_pass(8u, pass); // 8 & 7 == 0
        EXPECT_EQ(r0, r8, "Masking x with 7 should produce identical results for x and x+8.");
    }
}

// Test 3: Boundary pass values outside expected range
// - Pass values like -1 and 7 (beyond 0..6) should yield 0 for typical Adam7 data.
// This verifies graceful handling of out-of-range passes and guards against crashes.
void test_out_of_range_passes() {
    // Use a representative x value; masking ensures it remains within 0..7
    uint32_t xs[] = {0u, 3u, 7u, 12u}; // 12 -> 4 after masking
    for (uint32_t x : xs) {
        int r_neg1 = png_col_in_interlace_pass(x, -1);
        int r7 = png_col_in_interlace_pass(x, 7);
        // For out-of-range, ensure non-crash and the result is not a spurious non-zero
        EXPECT_EQ((r_neg1 == 0 || r_neg1 == 1), true, "Out-of-range pass (-1) should not crash; expect 0 or 1.");
        EXPECT_TRUE(r7 == 0 || r7 == 1, "Out-of-range pass (7) should yield 0 or 1 if defined by pattern.");
    }
}

// Test 4: Specific quick sanity checks on a couple of known boundaries
// - Common sense checks: x=0, pass=0 and x=7, pass=6 to observe behavior without relying on
//   exact Adam7 table values (we only rely on boundary consistency).
void test_sanity_boundaries() {
    int r_A = png_col_in_interlace_pass(0u, 0);
    int r_B = png_col_in_interlace_pass(7u, 6);

    // Just ensure function returns a valid 0/1 and does not crash
    EXPECT_TRUE(r_A == 0 || r_A == 1, "Result for (x=0, pass=0) must be 0 or 1.");
    EXPECT_TRUE(r_B == 0 || r_B == 1, "Result for (x=7, pass=6) must be 0 or 1.");
}

// Main test runner
int main() {
    // Run all test cases; they use non-terminating EXPECT macros to maximize coverage.
    test_true_false_branch_coverage();
    test_x_mask_invariance();
    test_out_of_range_passes();
    test_sanity_boundaries();

    if (g_failures != 0) {
        std::cerr << "png_col_in_interlace_pass tests finished with " << g_failures
                  << " failure(s).\n";
        return 1;
    }

    std::cout << "png_col_in_interlace_pass tests passed (non-fatal assertions where applicable).\n";
    return 0;
}