// Test suite for the focal function png_pass_row_shift (reproduced in this test file)
// This test is designed to be compilable with C++11 and without any external test framework.
// It exercises multiple code paths of png_pass_row_shift by providing a controllable
// 8x8 adam7 interlacing matrix and verifying the return values for various scenarios.

#include <cstdint>
#include <vector>
#include <signal.h>
#include <string.h>
#include <utility>
#include <iomanip>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Global 8x8 Adam7 interlace mapping used by the focal function.
// In the real library this matrix maps (row, col) to the PNG pass index (1..7).
// For testing, we craft it per-test to trigger specific branches in png_pass_row_shift.
static int adam7[8][8];

// Reproduced focal function under test (exact logic as provided in the prompt).
int png_pass_row_shift(int pass)
{
{
   int x, y, base=(-1), inc=8;
   ++pass;
   for (y=0; y<8; ++y)
   {
      for (x=0; x<8; ++x)
      {
         if (adam7[y][x] == pass)
         {
            if (base == (-1))
               base = y;
            else if (base == y)
               {}
            else if (inc == y-base)
               base=y;
            else if (inc == 8)
               inc = y-base, base=y;
            else if (inc != y-base)
               return 0xff; /* error - more than one 'inc' value! */
         }
      }
   }
   if (base == (-1)) return 0xfe; /* error - no row in pass! */
   /* The shift is always 1, 2 or 3 - no pass has all the rows! */
   switch (inc)
   {
case 2: return 1;
case 4: return 2;
case 8: return 3;
default: break;
   }
   /* error - unrecognized 'inc' */
   return (inc << 8) + 0xfd;
}
}

// Helper: reset adam7 to all zeros
inline void clear_adam7()
{
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            adam7[y][x] = 0;
}

// Helper: fill certain positions for a given pass (pass index starts at 0)
inline void fill_pass_positions(int pass, const std::vector<std::pair<int,int>>& coords)
{
    for (const auto &c : coords) {
        int y = c.first;
        int x = c.second;
        if (y >= 0 && y < 8 && x >= 0 && x < 8) {
            adam7[y][x] = pass + 1; // store as pass+1, matching the function's check
        }
    }
}

// Simple non-terminating assertion helper to report test results
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define ASSERT_EQ(expected, actual, desc)                                      \
    do {                                                                         \
        ++g_total_tests;                                                         \
        if ((expected) != (actual)) {                                          \
            ++g_failed_tests;                                                  \
            std::cout << "[FAIL] " << desc << " | expected: " << std::hex                 \
                      << std::showbase << (expected)                                      \
                      << ", actual: " << std::showbase << (actual) << std::dec << "\n"; \
        } else {                                                               \
            std::cout << "[PASS] " << desc << "\n";                           \
        }                                                                        \
    } while (0)

// Test Case 1: Single row in the pass -> expect shift 3 (inc == 8 -> case 3)
void test_single_row_shift3()
{
    clear_adam7();
    // Place pass+1 (i.e., 1) only on row 2, all columns.
    fill_pass_positions(0, { {2,0}, {2,1}, {2,2}, {2,3}, {2,4}, {2,5}, {2,6}, {2,7} });
    int res = png_pass_row_shift(0); // pass=0 -> internal pass=1
    ASSERT_EQ(3, res, "single-row-pass should return shift code 3 (inc=8)");
}

// Test Case 2: Pass appears on two rows with constant gap 2 -> expect shift 1
void test_two_rows_inc2()
{
    clear_adam7();
    // Rows 1 and 3 have pass
    fill_pass_positions(0, { {1,0}, {1,1} ,{1,2}, {1,3},{1,4},{1,5},{1,6},{1,7} });
    fill_pass_positions(0, { {3,0}, {3,1} ,{3,2}, {3,3},{3,4},{3,5},{3,6},{3,7} });
    int res = png_pass_row_shift(0);
    ASSERT_EQ(1, res, "two-rows with inc=2 should return shift code 1");
}

// Test Case 3: Pass appears on two rows with gap 4 -> expect shift 2
void test_two_rows_inc4()
{
    clear_adam7();
    // Rows 0 and 4 have pass
    fill_pass_positions(0, { {0,0}, {0,1},{0,2},{0,3},{0,4},{0,5},{0,6},{0,7} });
    fill_pass_positions(0, { {4,0}, {4,1} ,{4,2}, {4,3},{4,4},{4,5},{4,6},{4,7} });
    int res = png_pass_row_shift(0);
    ASSERT_EQ(2, res, "two-rows with inc=4 should return shift code 2");
}

// Test Case 4: No row contains the pass -> expect 0xfe
void test_no_base()
{
    clear_adam7();
    // No cell contains pass 1 (pass+1 = 1)
    int res = png_pass_row_shift(0);
    ASSERT_EQ(0xfe, res, "no row in pass should return 0xfe");
}

// Test Case 5: Pass creates inc=6 scenario -> expect 0x06fd
void test_inc_six()
{
    clear_adam7();
    // Rows 0 and 6 have pass
    fill_pass_positions(0, { {0,0} });
    fill_pass_positions(0, { {6,0} });
    int res = png_pass_row_shift(0);
    ASSERT_EQ(0x06fd, res, "inc=6 should return 0x06fd");
}

// Test Case 6: More than one distinct inc value triggers 0xff
void test_multiple_inc_error()
{
    clear_adam7();
    // Occurrences at rows 0, 3, and 7 to create inc values 3 then 4 (conflicting)
    fill_pass_positions(0, { {0,0} });
    fill_pass_positions(0, { {3,0} });
    fill_pass_positions(0, { {7,0} }); // This should cause a conflict and return 0xff
    int res = png_pass_row_shift(0);
    ASSERT_EQ(0xff, res, "multiple inc values should return 0xff");
}

// Test Case 7: Verify that inc=8 (single row) still yields 3 even if more rows exist but not for pass
void test_multiple_rows_with_one_row_only()
{
    clear_adam7();
    // Only one row (row 5) has pass
    fill_pass_positions(0, { {5,0}, {5,1}, {5,2} ,{5,3},{5,4},{5,5},{5,6},{5,7} });
    // Other rows do not have pass
    int res = png_pass_row_shift(0);
    ASSERT_EQ(3, res, "single-row-in-a-block should still yield shift 3");
}

int main(void)
{
    std::cout << "Starting tests for png_pass_row_shift (reproduced in test)..." << std::endl;

    test_single_row_shift3();
    test_two_rows_inc2();
    test_two_rows_inc4();
    test_no_base();
    test_inc_six();
    test_multiple_inc_error();
    test_multiple_rows_with_one_row_only();

    std::cout << "\nTest summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures." << std::endl;

    return (g_failed_tests == 0) ? 0 : 1;
}