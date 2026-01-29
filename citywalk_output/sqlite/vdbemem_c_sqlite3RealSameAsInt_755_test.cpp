// C++11 test suite for sqlite3RealSameAsInt in vdbemem.c
// This test uses a lightweight, non-terminating assertion approach
// since Google Test is not allowed. It directly calls the C function
// with a C linkage prototype and verifies all important branches.
// The tests cover true/false branches, boundary conditions, and special
// IEEE-754 values (e.g., 0.0 and -0.0).

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <string>


// Domain knowledge note:
// - sqlite3RealSameAsInt takes a double r1 and a 64-bit integer i
// - It returns true if r1 == 0.0 (short-circuit) OR
//   if bitwise representation of r1 equals that of (double)i
//   AND i is within [-2^51, 2^51).
// - We assume sqlite3_int64 is a 64-bit signed integer type.
// - All tests are designed to exercise both true and false branches.

extern "C" {
  // Forward declaration of the focal function with C linkage.
  // We assume sqlite3_int64 is a 64-bit signed integer type.
  typedef long long sqlite3_int64;
  int sqlite3RealSameAsInt(double r1, sqlite3_int64 i);
}

// Lightweight non-terminating assertion helper
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "[EXPECTED-FAIL] " << msg \
                << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")\n"; \
      ++g_failures; \
    } \
  } while(false)

static void run_all_tests() {
  // Test 1: If r1 == 0.0, should return true regardless of i
  {
    double r1 = 0.0;
    sqlite3_int64 i = 123456789012345LL;
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res != 0, "Case 1: r1 == 0.0 should yield true irrespective of i");
  }

  // Test 2: Exact bitwise match via i's conversion when r1 == (double)i
  // Here pick r1 = 1.0 and i = 1
  {
    double r1 = 1.0;
    sqlite3_int64 i = 1;
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res != 0, "Case 2: r1 == (double)i for r1=1.0, i=1 should yield true");
  }

  // Test 3: Bitwise mismatch with within-range i should yield false
  // r1 = 1.0, i = -1 -> r2 = -1.0, memcmp differs
  {
    double r1 = 1.0;
    sqlite3_int64 i = -1;
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res == 0, "Case 3: r1=1.0 and i=-1 should yield false due to bitwise mismatch");
  }

  // Test 4: Non-matching non-zero r1 with a small i not preserving bit pattern
  {
    double r1 = 2.5;
    sqlite3_int64 i = 2; // r2=2.0, not equal to 2.5 in bit pattern
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res == 0, "Case 4: r1=2.5, i=2 should yield false (bit pattern differs)");
  }

  // Test 5: Boundary-case where r1 and i have exact bit pattern and i is within range
  // Use i = -2^51 (min boundary)
  {
    sqlite3_int64 i = -2251799813685248LL; // -2^51
    double r1 = static_cast<double>(i);
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res != 0, "Case 5: r1 equals (double)i at lower boundary should yield true");
  }

  // Test 6: Boundary-case near the upper limit where i equals max-1
  {
    sqlite3_int64 i = 2251799813685247LL; // 2^51 - 1
    double r1 = static_cast<double>(i);
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res != 0, "Case 6: r1 equals (double)i at upper boundary-1 should yield true");
  }

  // Test 7: i just outside the allowed range should yield false even if bit patterns match
  // i = 2^51  (outside upper bound)
  {
    sqlite3_int64 i = 2251799813685248LL; // 2^51, out of range (exclusive upper bound)
    double r1 = static_cast<double>(i);
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res == 0, "Case 7: i outside range should yield false even if bit pattern matches");
  }

  // Test 8: -0.0 should be treated as zero; r1 == 0.0 check already handles it
  // Here, use r1 = -0.0 and ensure the function returns true
  {
    double r1 = -0.0; // negative zero
    sqlite3_int64 i = 123456789;
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res != 0, "Case 8: r1 == -0.0 should yield true due to r1==0.0 condition");
  }

  // Test 9: Large non-matching r1 with large i outside range should still be false
  {
    double r1 = 1e308; // huge non-zero
    sqlite3_int64 i = 999999999999999LL;
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res == 0, "Case 9: large non-matching r1 with large i should yield false");
  }

  // Test 10: Zero i with non-zero r1 that matches within range due to bit pattern
  // Choose r1 such that (double)i equals r1 bitwise; for i=0, r2=0.0, so need r1==0.0
  // This is covered by Case 1, but add an additional check: r1 is -0.0 which is still zero by equality.
  {
    double r1 = -0.0; // represents zero in numeric comparison
    sqlite3_int64 i = 0;
    int res = sqlite3RealSameAsInt(r1, i);
    EXPECT_TRUE(res != 0, "Case 10: r1 is -0.0 and i=0 should yield true due to r1==0.0");
  }
}

int main() {
  // Run all tests
  run_all_tests();

  // Summary
  int total = 10;
  if (g_failures == 0) {
    std::cout << "[TEST-PASS] All " << total << " tests passed.\n";
    return 0;
  } else {
    std::cout << "[TEST-FAIL] " << g_failures << " of " << total
              << " tests failed. See details above.\n";
    return 1;
  }
}