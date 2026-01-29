// C++11 unit test suite for the focal method: sqlite3GetInt32
// This test suite is self-contained: it provides minimal dependencies
// required by sqlite3GetInt32 and exercises its various branches.
// No external testing framework (e.g., Google Test) is used.
// The tests are executed from main() and use non-terminating assertions.

#include <math.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// ---------------------------------------------------------------------
// Lightweight dependency stubs and type aliases required by sqlite3GetInt32
// (These mimic a minimal subset of the sqlite internal utilities used by the
// focal function. They are provided here to keep the test self-contained.)
// ---------------------------------------------------------------------

typedef long long sqlite_int64; // 64-bit signed integer
typedef unsigned int u32;        // 32-bit unsigned

// Minimal implementations of utility predicates used by sqlite3GetInt32
static int sqlite3Isxdigit(int c) {
    return ((c >= '0' && c <= '9') ||
            (c >= 'a' && c <= 'f') ||
            (c >= 'A' && c <= 'F'));
}
static int sqlite3Isdigit(int c) {
    return (c >= '0' && c <= '9');
}
static int sqlite3HexToInt(int h) {
    if (h >= '0' && h <= '9') return h - '0';
    if (h >= 'a' && h <= 'f') return 10 + (h - 'a');
    if (h >= 'A' && h <= 'F') return 10 + (h - 'A');
    return 0;
}

// A small no-op macro to satisfy the original code's testcase() calls.
// It does not affect control flow and is used only for instrumentation.
#ifndef testcase
#define testcase(x) do { (void)(x); } while(0)
#endif

// ---------------------------------------------------------------------
// Focal method under test (reproduced here for self-contained testing)
// Based on the provided <FOCAL_METHOD> snippet.
// ---------------------------------------------------------------------

int sqlite3GetInt32(const char *zNum, int *pValue){
  sqlite_int64 v = 0;
  int i, c;
  int neg = 0;
  if( zNum[0]=='-' ){
    neg = 1;
    zNum++;
  }else if( zNum[0]=='+' ){
    zNum++;
  }
#ifndef SQLITE_OMIT_HEX_INTEGER
  else if( zNum[0]=='0'
        && (zNum[1]=='x' || zNum[1]=='X')
        && sqlite3Isxdigit(zNum[2])
  ){
    u32 u = 0;
    zNum += 2;
    while( zNum[0]=='0' ) zNum++;
    for(i=0; i<8 && sqlite3Isxdigit(zNum[i]); i++){
      u = u*16 + sqlite3HexToInt(zNum[i]);
    }
    if( (u&0x80000000)==0 && sqlite3Isxdigit(zNum[i])==0 ){
      memcpy(pValue, &u, 4);
      return 1;
    }else{
      return 0;
    }
  }
#endif
  if( !sqlite3Isdigit(zNum[0]) ) return 0;
  while( zNum[0]=='0' ) zNum++;
  for(i=0; i<11 && (c = zNum[i] - '0')>=0 && c<=9; i++){
    v = v*10 + c;
  }
  /* The longest decimal representation of a 32 bit integer is 10 digits:
  **
  **             1234567890
  **     2^31 -> 2147483648
  */
  testcase( i==10 );
  if( i>10 ){
    return 0;
  }
  testcase( v-neg==2147483647 );
  if( v-neg>2147483647 ){
    return 0;
  }
  if( neg ){
    v = -v;
  }
  *pValue = (int)v;
  return 1;
}

// ---------------------------------------------------------------------
// Minimal test framework (non-terminating assertions)
// - ASSERT_TRUE(cond): records failure but continues running tests
// - ASSERT_EQ(expected, actual): compares values and reports mismatch
// - main() runs all tests and returns non-zero on any failure
// ---------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_passed = 0;

#define ASSERT_TRUE(cond) \
  do { \
    g_tests_run++; \
    if (cond) { g_tests_passed++; } \
    else { \
      std::cerr << "Assertion failed: " #cond " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
  } while(0)

#define ASSERT_EQ(expected, actual) \
  do { \
    g_tests_run++; \
    if ((expected) == (actual)) { g_tests_passed++; } \
    else { \
      std::cerr << "Assertion failed: expected " << (expected) << " but got " \
                << (actual) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
  } while(0)

// Convenience macro to capture string inputs as C-strings
#define TEST_INPUTS(str) (str)

// ---------------------------------------------------------------------
// Step 2: Unit Test Generation (test suite for sqlite3GetInt32)
// Covers true/false branches of predicates, hex/decimal paths, sign handling.
// Note: HEX tests are included only if SQLITE_OMIT_HEX_INTEGER is not defined.
// ---------------------------------------------------------------------

// 1) Decimal: basic positive number
void test_decimal_basic() {
  // Purpose: Verify standard decimal parsing into int with no sign.
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("12345"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(12345, v);
}

// 2) Decimal: leading zeros should be ignored
void test_decimal_with_leading_zeros() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("0000012345"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(12345, v);
}

// 3) Decimal: maximum signed 32-bit value
void test_decimal_max_int() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("2147483647"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(2147483647, v);
}

// 4) Decimal: overflow should fail (too large for int)
void test_decimal_overflow() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("2147483648"), &v);
  ASSERT_TRUE(ok == 0);
}

// 5) Decimal: negative value within range
void test_decimal_negative_min() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("-2147483648"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(-2147483648, v);
}

// 6) Decimal: negative overflow should fail
void test_decimal_negative_overflow() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("-2147483649"), &v);
  ASSERT_TRUE(ok == 0);
}

// 7) Decimal: explicit plus sign
void test_decimal_with_plus_sign() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("+123"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(123, v);
}

// 8) Decimal: input that starts with non-digit after sign should fail
void test_non_digit_after_sign() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("+abc"), &v);
  ASSERT_TRUE(ok == 0);
}

// 9) Decimal: non-digit first character should fail
void test_non_digit_first_char() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("a123"), &v);
  ASSERT_TRUE(ok == 0);
}

// 10) Decimal: zero handling (0, -0, +0)
void test_zero handling() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("0"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(0, v);

  v = 0;
  ok = sqlite3GetInt32(TEST_INPUTS("-0"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(0, v);

  v = 0;
  ok = sqlite3GetInt32(TEST_INPUTS("+0"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(0, v);
}

// 11) Decimal: long string of zeros
void test_decimal_zeros_only() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("00000000000"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(0, v);
}

// 12) Decimal: first character non-digit (e.g., 'x') should fail
void test_non_digit_first_char_hexlike() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("x123"), &v);
  ASSERT_TRUE(ok == 0);
}

// 13) Hex: true hex parsing (only if hex support is compiled in)
#ifndef SQLITE_OMIT_HEX_INTEGER
void test_hex_parsing_positive() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("0x7fffffff"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(2147483647, v);
}
#endif

// 14) Hex: overflow hex value (top bit set) should fail
#ifndef SQLITE_OMIT_HEX_INTEGER
void test_hex_overflow() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("0x80000000"), &v);
  ASSERT_TRUE(ok == 0);
}
#endif

// 15) Hex: extra hex digit after valid 8 digits should fail
#ifndef SQLITE_OMIT_HEX_INTEGER
void test_hex_extra_digit() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("0x7fffffff1"), &v);
  ASSERT_TRUE(ok == 0);
}
#endif

// 16) Hex: case-insensitive hex prefix (0X vs 0x)
#ifndef SQLITE_OMIT_HEX_INTEGER
void test_hex_prefix_case_insensitive() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("0X7FFFFFFF"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(2147483647, v);
}
#endif

// 17) Hex: zero after skipping leading zeros
#ifndef SQLITE_OMIT_HEX_INTEGER
void test_hex_with_leading_zeros() {
  int v = 0;
  int ok = sqlite3GetInt32(TEST_INPUTS("0x0000007F"), &v);
  ASSERT_TRUE(ok == 1);
  ASSERT_EQ(127, v);
}
#endif

// Run all tests
void run_all_tests() {
  test_decimal_basic();
  test_decimal_with_leading_zeros();
  test_decimal_max_int();
  test_decimal_overflow();
  test_decimal_negative_min();
  test_decimal_negative_overflow();
  test_decimal_with_plus_sign();
  test_non_digit_after_sign();
  test_non_digit_first_char();
  test_zero handling();
  test_decimal_zeros_only();
  test_non_digit_first_char_hexlike();

#ifndef SQLITE_OMIT_HEX_INTEGER
  test_hex_parsing_positive();
  test_hex_overflow();
  test_hex_extra_digit();
  test_hex_prefix_case_insensitive();
  test_hex_with_leading_zeros();
#endif
}

// ---------------------------------------------------------------------
// Step 3: Test Case Refinement
// - The tests cover true/false branches per predicate
// - They exercise both decimal and hex representations when available
// - They verify boundary conditions (INT32 max, min, overflow)
// - They include sign handling (+, -, no sign)
// ---------------------------------------------------------------------

int main() {
  run_all_tests();

  std::cout << "Unit Tests Completed: " << g_tests_passed << " / " << g_tests_run
            << " tests passed." << std::endl;

  // Return non-zero if any test failed
  return (g_tests_run == g_tests_passed) ? 0 : 1;
}