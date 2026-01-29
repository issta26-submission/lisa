// Lightweight C++11 unit test harness for the focal function png_pass_col_shift
// This test re-creates the essential environment (adam7 table and the function)
// to exercise all code paths without requiring libpng binaries.
// Note: This is a self-contained test that mirrors the logic of the focal method
// and uses explicit test cases to maximize coverage across branches.

#include <cstdint>
#include <vector>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Candidate Keywords gathered from Step 1 (for traceability in test intent):
// - adam7, pass, base, inc, 0xff (error - more than one 'inc' value), 0xfe (error - no row in pass)
// - shifts: inc values 1, 2, 4, 8 corresponding to returns 0, 1, 2, 3
// - default/unknown inc path: (inc << 8) + 0xfd
// - interlace passes across an 8x8 grid (0..7) and the +1 adjustment inside the function

// Global 8x8 adam7 table used by the focal function.
// Tests will modify this matrix to control the flow through png_pass_col_shift.
static int adam7[8][8];

// Focal method under test (reproduced in test harness for determinism).
// Note: This implementation mirrors the provided C code exactly to ensure
// we exercise the same logic paths as in the original project.
int png_pass_col_shift(int pass)
{
{
   int x, y, base=(-1), inc=8;
   ++pass;
   for (x=0; x<8; ++x)
   {
      for (y=0; y<8; ++y)
      {
         if (adam7[y][x] == pass)
         {
            if (base == (-1))
               base = x;
            else if (base == x)
               {}
            else if (inc == x-base)
               base=x;
            else if (inc == 8)
               inc = x-base, base=x;
            else if (inc != x-base)
               return 0xff; /* error - more than one 'inc' value! */
         }
      }
   }
   if (base == (-1)) return 0xfe; /* error - no row in pass! */
   /* The shift is always 1, 2 or 3 - no pass has all the rows! */
   switch (inc)
   {
case 1: return 0; /* pass 7 has all the columns */
case 2: return 1;
case 4: return 2;
case 8: return 3;
default: break;
   }
   /* error - unrecognized 'inc' */
   return (inc << 8) + 0xfd;
}
}

// Helper: reset the adam7 matrix to a known default (all zeros)
static void reset_adam7() {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            adam7[r][c] = 0;
}

// Helper: set a single cell (row, col) to a value (1..N)
static void set_adam7_cell(int row, int col, int value) {
    if (row >= 0 && row < 8 && col >= 0 && col < 8)
        adam7[row][col] = value;
}

// Simple test harness
static int tests_run = 0;
static int tests_passed = 0;

static void log_test_result(const std::string& name, bool ok, const std::string& note = "") {
    ++tests_run;
    if (ok) {
        ++tests_passed;
        std::cout << "[PASS] " << name;
        if (!note.empty()) std::cout << " - " << note;
        std::cout << "\n";
    } else {
        std::cout << "[FAIL] " << name;
        if (!note.empty()) std::cout << " - " << note;
        std::cout << "\n";
    }
}

// Test 1: No matches for given pass -> expect 0xfe
static bool test_no_match_returns_fe() {
    reset_adam7();
    // All cells zero; with pass=0, ++pass = 1; no cell == 1 -> base remains -1
    int res = png_pass_col_shift(0);
    bool ok = (res == 0xfe);
    log_test_result("test_no_match_returns_fe (no row in pass)", ok,
        "expected 0xfe when no adam7[y][x] equals pass+1");
    return ok;
}

// Test 2: Single match -> inc remains 8 -> expect 3
// Setup: exactly one cell at value pass+1 (e.g., pass=2 -> value=3) at some (x,y)
static bool test_single_match_returns_3() {
    reset_adam7();
    set_adam7_cell(0, 2, 3); // one match at (row 0, col 2)
    int res = png_pass_col_shift(2);
    bool ok = (res == 3);
    log_test_result("test_single_match_returns_3 (inc=8 case)", ok,
        "expect 3 when only one matching cell exists (inc=8 -> 3)");
    return ok;
}

// Test 3: Two matches with x-base difference = 1 -> expect 0
// Matches at (1,0) and (2,1) for pass=2 -> base becomes 1, then x-base=1 -> final inc=1
static bool test_two_matches_diff1_returns_0() {
    reset_adam7();
    set_adam7_cell(0, 1, 3);
    set_adam7_cell(1, 2, 3);
    int res = png_pass_col_shift(2);
    bool ok = (res == 0);
    log_test_result("test_two_matches_diff1_returns_0 (inc=1)", ok,
        "expect 0 when two matches with x-diff of 1 occur");
    return ok;
}

// Test 4: Three matches cause 0xff (error - more than one 'inc' value)
static bool test_three_matches_ff() {
    reset_adam7();
    set_adam7_cell(0, 1, 3);
    set_adam7_cell(1, 3, 3);
    set_adam7_cell(2, 7, 3);
    int res = png_pass_col_shift(2);
    bool ok = (res == 0xff);
    log_test_result("test_three_matches_ff (error path 0xff)", ok,
        "expect 0xff when more than one distinct inc value is detected");
    return ok;
}

// Test 5: Two matches with x-base difference = 3 -> expect default path 0x03fd (inc=3)
static bool test_inc3_default_path() {
    reset_adam7();
    set_adam7_cell(0, 0, 3);
    set_adam7_cell(1, 3, 3);
    int res = png_pass_col_shift(2);
    bool ok = (res == 0x03fd);
    log_test_result("test_inc3_default_path (inc=3 -> 0x03fd)", ok,
        "expect (inc<<8)+0xfd when inc=3 final (no further matches)");
    return ok;
}

// Test 6: Two matches with x-base difference = 1 (explicit test for inc=1 path)
static bool test_inc1_path() {
    reset_adam7();
    set_adam7_cell(0, 0, 3);
    set_adam7_cell(1, 1, 3);
    int res = png_pass_col_shift(2);
    bool ok = (res == 0);
    log_test_result("test_inc1_path (inc=1)", ok,
        "expect 0 when inc final = 1");
    return ok;
}

// Test 7: Two matches with x-base difference = 2 -> expect 1
static bool test_inc2_path() {
    reset_adam7();
    set_adam7_cell(0, 0, 3);
    set_adam7_cell(1, 2, 3);
    int res = png_pass_col_shift(2);
    bool ok = (res == 1);
    log_test_result("test_inc2_path (inc=2 -> 1)", ok,
        "expect 1 when inc final = 2");
    return ok;
}

// Test 8: Two matches with x-base difference = 4 -> expect 2
static bool test_inc4_path() {
    reset_adam7();
    set_adam7_cell(0, 0, 3);
    set_adam7_cell(1, 4, 3);
    int res = png_pass_col_shift(2);
    bool ok = (res == 2);
    log_test_result("test_inc4_path (inc=4 -> 2)", ok,
        "expect 2 when inc final = 4");
    return ok;
}

// Test 9: Two matches with same x (inc path should stay at inc=8 -> returns 3)
static bool test_same_x_multiple_matches() {
    reset_adam7();
    set_adam7_cell(0, 5, 3);
    set_adam7_cell(1, 5, 3);
    int res = png_pass_col_shift(2);
    bool ok = (res == 3);
    log_test_result("test_same_x_multiple_matches (same x, inc stays 8 -> 3)", ok,
        "expect 3 when multiple matches share the same x column");
    return ok;
}

// Main: Run all tests and summarize
int main() {
    // Run each test, accumulate results
    test_no_match_returns_fe();
    test_single_match_returns_3();
    test_two_matches_diff1_returns_0();
    test_three_matches_ff();
    test_inc3_default_path();
    test_inc1_path();
    test_inc2_path();
    test_inc4_path();
    test_same_x_multiple_matches();

    // Summary
    std::cout << "----- Test Summary -----\n";
    std::cout << "Total tests run: " << tests_run << "\n";
    std::cout << "Tests passed:  " << tests_passed << "\n";

    // Exit code 0 if all tests passed, otherwise non-zero
    return (tests_run == tests_passed) ? 0 : 1;
}