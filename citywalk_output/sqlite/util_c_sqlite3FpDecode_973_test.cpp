// Unit test suite for the focal method sqlite3FpDecode located in util.c
// This test suite is written in C++11 without any external testing framework.
// It uses a lightweight assertion mechanism to avoid terminating the test run
// on the first failure, allowing broader code-path coverage.
//
// Notes:
// - We assume the library provides a C function with C linkage:
//     void sqlite3FpDecode(void* p, double r, int iRound, int mxRound);
//   We declare it with a void* parameter to minimize strict C++ type coupling
//   and to help avoid potential struct-layout mismatches between translation units.
// - We define a test-local FpDecodeTest struct with fields expected by the
//   implementation (as touched by sqlite3FpDecode). The real layout in the
//   project is assumed to be compatible with this partial layout for testing.
// - Tests exercise: negative numbers, zero, infinities and NaN (special values),
//   and a basic finite number to ensure normal operation paths are reachable.
// - Static members and static helper behaviors from the original project are not
//   relied upon directly in these tests, since we interact with the public
//   FpDecode interaction surface only.
// - All tests are invoked from main() as required by the constraints that GTest is not allowed
//   and tests must be run programmatically without an external framework.

#include <math.h>
#include <cmath>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <limits>
#include <iostream>


// Minimal, test-oriented FpDecode layout used by the tests.
// This must be laid out consistently with the expectations of sqlite3FpDecode
// for the fields accessed by the function. The real layout in the project may differ,
// but this partial struct mirrors the fields touched by sqlite3FpDecode.
// If the actual layout differs, this test may need adjustment to match the real
// FpDecode definition from the sqlite3 headers.
struct FpDecodeTest {
  int isSpecial;
  char sign;
  int n;
  int iDP;
  char *z;
  char zBuf[128];
};

// Declare the focal function with C linkage, using a void* parameter to
// accommodate possible differences in the public type definition across units.
extern "C" void sqlite3FpDecode(void *p, double r, int iRound, int mxRound);

// Lightweight assertion helpers to enable non-terminating test execution.
static int g_failures = 0;

#define TEST_TRUE(cond, msg) \
  do { \
    if(!(cond)){ \
      std::cerr << "TEST FAIL: " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
                << " (condition: " << #cond << ")" << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define TEST_STR_EQ(a, b, msg) \
  do { \
    if(std::strcmp((a), (b)) != 0){ \
      std::cerr << "TEST FAIL: " << __FILE__ << ":" << __LINE__ << " - " \
                << (msg) << " (expected \"" << (b) << "\", got \"" << (a) << "\")" \
                << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define TEST_INT_EQ(a, b, msg) \
  do { \
    if((a) != (b)){ \
      std::cerr << "TEST FAIL: " << __FILE__ << ":" << __LINE__ << " - " \
                << (msg) << " (expected " << (b) << ", got " << (a) << ")" \
                << std::endl; \
      ++g_failures; \
    } \
  } while(0)

// Helper to initialize a fresh FpDecodeTest instance
static void initFpDecodeTest(FpDecodeTest &p){
  std::memset(&p, 0, sizeof(p));
  // Ensure z points to a valid buffer by default
  p.z = p.zBuf;
}

// Test 1: Negative input numbers should set the sign to '-' and still produce a valid decode.
// This exercises the branch: if( r<0.0 ){ p->sign = '-'; r = -r; }
static void test_negative_sign(){
  FpDecodeTest p;
  initFpDecodeTest(p);

  double r = -2.5;
  sqlite3FpDecode((void*)&p, r, 0, 15);

  // Expected: sign is negative and function returns with finite state
  TEST_TRUE(p.sign == '-', "Negative input should set sign to '-'");
  // Additionally, ensure that some representation of the magnitude exists
  TEST_TRUE(p.n >= 1 || p.iDP >= 1, "Negative input should produce some digits (n or iDP > 0)");
}

// Test 2: Zero input should set a deterministic representation "0" with exact metadata.
// This covers the r==0.0 path returning early.
static void test_zero_case(){
  FpDecodeTest p;
  initFpDecodeTest(p);

  double r = 0.0;
  sqlite3FpDecode((void*)&p, r, 0, 15);

  // Zero must yield a literal "0" string and specific metadata
  TEST_TRUE(p.sign == '+', "Zero input should set sign to '+'");
  TEST_INT_EQ(p.n, 1, "Zero input should set n to 1");
  TEST_INT_EQ(p.iDP, 1, "Zero input should set iDP to 1");
  TEST_TRUE(p.z != nullptr, "Zero input should set z to point to a string");
  TEST_TRUE(p.z[0] == '0' && p.z[1] == '\0', "Zero input should produce string '0'");
}

// Test 3: Special values Infinity and NaN should be detected and flagged as special.
// Infinity should set isSpecial to 1; NaN should set isSpecial to 2.
static void test_special_values(){
  FpDecodeTest p;
  initFpDecodeTest(p);

  // Positive Infinity
  double inf = std::numeric_limits<double>::infinity();
  sqlite3FpDecode((void*)&p, inf, 0, 15);
  TEST_INT_EQ(p.isSpecial, 1, "Infinity should set isSpecial to 1");
  TEST_INT_EQ(p.n, 0, "Infinity should have n == 0");
  TEST_INT_EQ(p.iDP, 0, "Infinity should have iDP == 0");
  TEST_TRUE(p.sign == '+', "Infinity should keep sign as '+' (positive)");

  // NaN
  initFpDecodeTest(p);
  double nan = std::numeric_limits<double>::quiet_NaN();
  sqlite3FpDecode((void*)&p, nan, 0, 15);
  TEST_INT_EQ(p.isSpecial, 2, "NaN should set isSpecial to 2");
  TEST_INT_EQ(p.n, 0, "NaN should have n == 0");
  TEST_INT_EQ(p.iDP, 0, "NaN should have iDP == 0");
}

// Test 4: A small finite positive number should produce a non-zero digit sequence.
// This exercises the normal decoding path (non-special, finite numbers).
static void test_finite_positive_basic(){
  FpDecodeTest p;
  initFpDecodeTest(p);

  double r = 123.456; // finite finite
  sqlite3FpDecode((void*)&p, r, 0, 15);

  // Expectation: digits should be produced (n > 0) and z should point to a digit string
  TEST_TRUE(p.n > 0, "Finite positive number should result in n > 0");
  TEST_TRUE(p.z != nullptr, "Finite positive number should set z to a valid string/digits buffer");
}

// Test 5: A simple sanity check with a moderately large finite value to touch the non-long-double path.
// We only verify that a decode occurs and the output fields are sensible.
static void test_large_finite_sanity(){
  FpDecodeTest p;
  initFpDecodeTest(p);

  double r = 1.0e6; // moderately large but finite
  sqlite3FpDecode((void*)&p, r, 0, 15);

  TEST_TRUE(p.n > 0, "Large finite number should produce digits (n > 0)");
  TEST_TRUE(p.z != nullptr, "Large finite number should set z to a valid digits string");
}

// Simple test runner to execute all tests and summarize the results.
static void run_all_tests(){
  test_negative_sign();
  test_zero_case();
  test_special_values();
  test_finite_positive_basic();
  test_large_finite_sanity();
}

int main(){
  run_all_tests();

  if(g_failures == 0){
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  }else{
    std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
    return 1;
  }
}