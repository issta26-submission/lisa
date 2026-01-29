// Unit test suite for the focal method: sqlite3MulInt64
// This test is designed to run under C++11 without Google Test.
// It provides a self-contained implementation of sqlite3MulInt64 (as it appears in the provided snippet)
// and exercises multiple branches of the overflow-detection logic (else path).
// Notes:
// - The test uses the else-branch implementation (GCC_VERSION < 5004000) to cover the
//   predicate branches for positive, negative, and boundary values.
// - We purposely compile with GCC_VERSION set below the threshold to exercise non-Builtin path.
// - The tests avoid terminating assertions; failures are reported but execution continues to
//   maximize coverage across all test cases.

#include <math.h>
#include <vector>
#include <climits>
#include <string>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// Domain-specific typedefs/macros (as used by sqlite3MulInt64)
using i64 = long long;
#define LARGEST_INT64 LLONG_MAX
#define SMALLEST_INT64 LLONG_MIN

// Compile-time macro to force the non-GCC builtin path (testing the classic branch logic)
#define GCC_VERSION 4000000

// Focal method replicated (as in the provided code snippet)
int sqlite3MulInt64(i64 *pA, i64 iB){
#if GCC_VERSION>=5004000 && !defined(__INTEL_COMPILER)
  // Builtin path (not exercised in this test binary due to GCC_VERSION value)
  return __builtin_mul_overflow(*pA, iB, pA);
#else
  i64 iA = *pA;
  if( iB>0 ){
    if( iA>LARGEST_INT64/iB ) return 1;
    if( iA<SMALLEST_INT64/iB ) return 1;
  }else if( iB<0 ){
    if( iA>0 ){
      if( iB<SMALLEST_INT64/iA ) return 1;
    }else if( iA<0 ){
      if( iB==SMALLEST_INT64 ) return 1;
      if( iA==SMALLEST_INT64 ) return 1;
      if( -iA>LARGEST_INT64/-iB ) return 1;
    }
  }
  *pA = iA*iB;
  return 0;
#endif
}

// Simple non-terminating assertion helper
static int g_fail_count = 0;
#define ASSERT_TRUE(cond, msg) \
  do { if(!(cond)) { std::cerr << "[FAIL] " << msg << "\n"; ++g_fail_count; } } while(0)
#define ASSERT_EQ(actual, expected, msg) \
  do { if(!((actual) == (expected))) { std::cerr << "[FAIL] " << msg \
      << " | Expected: " << (expected) << ", Got: " << (actual) << "\n"; ++g_fail_count; } } while(0)

// Test harness helpers
struct TestCase {
  const char* name;
  i64 initialA;
  i64 multiplier;
  int expectedRet;
  i64 expectedA;
};

// Entry point
int main() {
  // Define a suite of test cases to exercise the else-branch logic thoroughly.
  // Case A: No overflow with small values
  //   pA = 3, iB = 4  => 12, no overflow
  //   Expect ret = 0, pA = 12
  // Case B: Overflow when the left operand is the largest possible value
  //   pA = LARGEST_INT64, iB = 2  => overflow (ret = 1), pA unchanged
  TestCase t1 = {"NoOverflow_SmallValues", 3, 4, 0, 12};
  TestCase t2 = {"Overflow_LargeLeft", LARGEST_INT64, 2, 1, LARGEST_INT64};
  // Case C: Negative left with positive multiplier, no overflow
  //   pA = -1, iB = 2  => -2, ret = 0
  TestCase t3 = {"NegLeft_Positive_NoOverflow", -1, 2, 0, -2};
  // Case D: Negative multiplier, positive left value that does not overflow
  //   pA = 2, iB = -2  => -4, ret = 0
  TestCase t4 = {"NegativeMultiplier_NoOverflow", 2, -2, 0, -4};
  // Case E: Overflow case when iB is SMALLEST_INT64
  //   pA = -5, iB = SMALLEST_INT64 => overflow (ret = 1), pA unchanged
  TestCase t5 = {"Overflow_Boundary_SMALLEST_INT64", -5, SMALLEST_INT64, 1, -5};
  // Case F: Negative operands with negative multiplier, no overflow
  //   pA = -2, iB = -2 => 4, ret = 0
  TestCase t6 = {"NegNeg_NoOverflow", -2, -2, 0, 4};
  // Case G: Zero multiplier
  //   pA = 123, iB = 0 => 0, ret = 0
  TestCase t7 = {"ZeroMultiplier", 123, 0, 0, 0};

  std::vector<TestCase> tests = {t1, t2, t3, t4, t5, t6, t7};

  int passed = 0;
  int total = (int)tests.size();

  for(const auto& tc : tests) {
    i64 a = tc.initialA;
    int ret = sqlite3MulInt64(&a, tc.multiplier);
    bool ok = (ret == tc.expectedRet) && (a == tc.expectedA);
    if(ok) {
      std::cout << "[OK] " << tc.name << "\n";
      ++passed;
    } else {
      std::cerr << "[ERR] " << tc.name
                << " | initialA=" << tc.initialA
                << ", multiplier=" << tc.multiplier
                << " | expectedRet=" << tc.expectedRet
                << ", expectedA=" << tc.expectedA
                << " | gotRet=" << ret
                << ", gotA=" << a << "\n";
    }
  }

  if(passed != total) {
    std::cerr << "Test suite completed with failures: " << (total - passed) << " of " << total << "\n";
  } else {
    std::cout << "All tests passed: " << total << "\n";
  }

  // Return non-zero if there were any failures to indicate test result to the caller
  return (passed == total) ? 0 : 1;
}