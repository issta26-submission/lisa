// Unit test suite for sqlite3Utf16ByteLen (reproduced within this test file for isolation).
// This test suite is written for C++11, without any external testing framework.
// It covers core behavior of the function as defined in the provided focal method.

// Important notes (mapped to Candidate Keywords and Domain Knowledge guidance):
// - Core concept: UTF-16 length in bytes for given number of characters (code points).
// - Endianness handling: relies on macros SQLITE_UTF16LE and SQLITE_UTF16NATIVE.
// - Surrogate pair handling: recognizes a high surrogate followed by a low surrogate and counts as one character.
// - BOM/offset handling: a minor offset is applied if SQLITE_UTF16NATIVE == SQLITE_UTF16LE.
// - Tests crafted to exercise BMP (2-byte) characters and surrogate pairs, including edge cases (nChar=0).
// - All tests are self-contained and do not rely on private members or external frameworks.

#include <functional>
#include <vector>
#include <assert.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Define macros to simulate the environment in which sqlite3Utf16ByteLen operates.
// We choose LE == NATIVE for most straightforward ASCII/BMP tests, but we also craft a surrogate test
// that still executes correctly under LE == NATIVE by leveraging a memory layout that triggers the surrogate check.

#define SQLITE_UTF16LE 1
#define SQLITE_UTF16BE 0
#define SQLITE_UTF16NATIVE 1  // Simulate native little-endian environment (equal to LE) for the basic tests.

// Focal method replicated locally for test purposes (mirroring the provided <FOCAL_METHOD>).
int sqlite3Utf16ByteLen(const void *zIn, int nChar){
  int c;
  unsigned char const *z = (unsigned char const*)zIn;
  int n = 0;

  if( SQLITE_UTF16NATIVE==SQLITE_UTF16LE ) z++;
  while( n<nChar ){
    c = z[0];
    z += 2;
    if( c>=0xd8 && c<0xdc && z[0]>=0xdc && z[0]<0xe0 ) z += 2;
    n++;
  }
  return (int)(z-(unsigned char const *)zIn) 
              - (SQLITE_UTF16NATIVE==SQLITE_UTF16LE);
}

// Simple internal test harness
static int failures = 0;
static void expect_eq(int actual, int expected, const std::string& testName) {
  if (actual != expected) {
    std::cerr << "[FAIL] " << testName
              << " | Expected: " << expected << ", Got: " << actual << "\n";
    ++failures;
  } else {
    std::cout << "[PASS] " << testName << "\n";
  }
}

static void test_BMP_only_basic() {
  // Purpose: Verify BMP characters without any surrogate involvement.
  // Input: 3 BMP characters -> 6 bytes consumed.
  // Memory layout (little-endian 16-bit units): 0x0041, 0x0042, 0x0043 -> bytes: 41 00 42 00 43 00
  unsigned char z[] = { 0x41,0x00, 0x42,0x00, 0x43,0x00 };
  int nChar = 3;
  int result = sqlite3Utf16ByteLen(z, nChar);
  expect_eq(result, 6, "BMP_only_basic (3 BMP chars) should consume 6 bytes");
}

static void test_zero_length_inputs() {
  // Purpose: Ensure correct handling when nChar == 0.
  // With LE == NATIVE, return should be -1 (offset) since no characters are processed.
  unsigned char z[] = {}; // empty
  int result = sqlite3Utf16ByteLen(z, 0);
  // Expected: 0 - 1 = -1 due to (SQLITE_UTF16NATIVE==SQLITE_UTF16LE)
  expect_eq(result, -1, "zero_length_input_LE_equal_Native should return -1");
}

static void test_BMP_plus_surrogate_LE_equal() {
  // Purpose: Exercise surrogate detection path under LE==NATIVE by crafting a memory layout
  // that misaligns the first 16-bit unit so that the surrogate check still triggers.
  // Memory layout (bytes):
  // [0] = 0x00
  // [1] = 0xD8      -> first surrogate high byte (partial 0xD83D when combined with next byte)
  // [2] = 0x3D
  // [3] = 0xDC      -> second unit's high byte (0xDC..0xDF range triggers surrogate)
  // [4] = 0x00
  // [5] = 0xDE      -> low surrogate byte
  // Interpreted as: first char is surrogate (D83D), second is BMP (DE00 or similar),
  // but due to the neighbor bytes and LE==NATIVE, the surrogate condition should trigger for the first char.
  unsigned char z[] = { 0x00, 0xD8, 0x3D, 0xDC, 0x00, 0xDE };
  int nChar = 2;
  int result = sqlite3Utf16ByteLen(z, nChar);
  // Expected: First surrogate consumes 4 bytes, second BMP consumes 2 bytes -> total 6
  expect_eq(result, 6, "BMP_plus_surrogate_LE_equal should produce 6 bytes for 2 chars");
}

static void test_BMP_only_another_case() {
  // Additional BMP-only case to ensure consistency for a different nChar
  unsigned char z[] = { 0x41,0x00, 0x42,0x00 }; // 2 BMP chars
  int nChar = 2;
  int result = sqlite3Utf16ByteLen(z, nChar);
  expect_eq(result, 4, "BMP_only_case2 (2 BMP chars) should consume 4 bytes");
}

int main() {
  std::cout << "Starting sqlite3Utf16ByteLen unit tests (embedded implementation).\n";

  test_BMP_only_basic();
  test_zero_length_inputs();
  test_BMP_plus_surrogate_LE_equal();
  test_BMP_only_another_case();

  if (failures == 0) {
    std::cout << "[ALL TESTS PASSED] sqlite3Utf16ByteLen implementation appears correct for the covered scenarios.\n";
    return 0;
  } else {
    std::cerr << "[TESTS FAILED] " << failures << " test(s) failed.\n";
    return 1;
  }
}