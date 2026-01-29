// Minimal C++11 test suite for sqlite3Utf8CharLen
// This test suite provides a self-contained reproduction of the focal
// function along with lightweight tests to validate its behavior.
//
// Notes:
// - We re-implement a small subset of the dependencies (u8, SQLITE_SKIP_UTF8)
//   locally to make the tests self-contained without requiring SQLite headers.
// - The tests cover ASCII, multibyte UTF-8 sequences, boundary behavior (nByte),
//   and the nByte < 0 (count until string terminator).
// - No external testing framework is used; a tiny in-file assertion helper logs
//   results to stdout/stderr.

#include <assert.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Minimal type alias used by the focal method
using u8 = unsigned char;

// Macro to advance z by the length of the current UTF-8 character.
// This mirrors SQLite's internal behavior for skipping a UTF-8 code point.
#define SQLITE_SKIP_UTF8(z) do {                                       \
  unsigned char c = *(const unsigned char*)(z);                        \
  if (c < 0x80) { (z) = (z) + 1; }                                    \
  else if ((c & 0xE0) == 0xC0) { (z) = (z) + 2; }                       \
  else if ((c & 0xF0) == 0xE0) { (z) = (z) + 3; }                       \
  else if ((c & 0xF8) == 0xF0) { (z) = (z) + 4; }                       \
  else { (z) = (z) + 1; }                                              \
} while(0)

// Focal method under test (reproduced in this test file for self-containment)
int sqlite3Utf8CharLen(const char *zIn, int nByte){
  int r = 0;
  const u8 *z = (const u8*)zIn;
  const u8 *zTerm;
  if( nByte>=0 ){
    zTerm = &z[nByte];
  }else{
    zTerm = (const u8*)(-1);
  }
  assert( z<=zTerm );
  while( *z!=0 && z<zTerm ){
    SQLITE_SKIP_UTF8(z);
    r++;
  }
  return r;
}

// Tiny assertion helper to avoid terminating tests on first failure
static bool expect_eq_int(const char* testName, int got, int expected) {
  if (got == expected) {
    std::cout << "[OK] " << testName << ": got " << got << ", expected " << expected << "\n";
    return true;
  } else {
    std::cerr << "[FAIL] " << testName << ": got " << got << ", expected " << expected << "\n";
    return false;
  }
}

// Test 1: ASCII string within boundary nByte == length
// Expect to count all characters up to nByte (3).
static bool test_ascii_within_boundary() {
  // "abc" ASCII
  const char s[] = "abc";
  int r = sqlite3Utf8CharLen(s, 3);
  return expect_eq_int("test_ascii_within_boundary", r, 3);
}

// Test 2: ASCII string with nByte beyond end
// Expect to count the entire string length (3), boundary beyond end should not affect.
static bool test_ascii_beyond_end_boundary() {
  const char s[] = "abc";
  int r = sqlite3Utf8CharLen(s, 5);
  return expect_eq_int("test_ascii_beyond_end_boundary", r, 3);
}

// Test 3: Multibyte UTF-8 character (é) length 2 bytes
// String: é (C3 A9)
static bool test_multibyte_single_boundary_hit() {
  const char s[] = "\xC3\xA9"; // "é"
  int r0 = sqlite3Utf8CharLen(s, 0); // boundary before any byte
  int r1 = sqlite3Utf8CharLen(s, 1); // boundary after first byte (inside first char)
  int r2 = sqlite3Utf8CharLen(s, 2); // boundary after full first char (end of string)
  bool ok = expect_eq_int("test_multibyte_single_boundary_hit_r0", r0, 0) &&
            expect_eq_int("test_multibyte_single_boundary_hit_r1", r1, 1) &&
            expect_eq_int("test_multibyte_single_boundary_hit_r2", r2, 1);
  return ok;
}

// Test 4: Multibyte string with two characters: éé (C3 A9 C3 A9)
static bool test_multibyte_two_chars_boundaries() {
  const char s[] = "\xC3\xA9\xC3\xA9"; // "éé"
  int r_at_2 = sqlite3Utf8CharLen(s, 2); // should count 1 (first code point)
  int r_at_3 = sqlite3Utf8CharLen(s, 3); // boundary after first 1.5 codepoints -> should count 2
  int r_at_neg1 = sqlite3Utf8CharLen(s, -1); // full string; should count 2
  bool ok = expect_eq_int("test_multibyte_two_chars_boundaries_r_at_2", r_at_2, 1) &&
            expect_eq_int("test_multibyte_two_chars_boundaries_r_at_3", r_at_3, 2) &&
            expect_eq_int("test_multibyte_two_chars_boundaries_r_at_neg1", r_at_neg1, 2);
  return ok;
}

// Test 5: Negative nByte (-1) counts entire string regardless of length
static bool test_negative_nByte_counts_entire_string() {
  const char s[] = "\xC3\xA9\xC3\xA9"; // "éé"
  int r = sqlite3Utf8CharLen(s, -1);
  return expect_eq_int("test_negative_nByte_counts_entire_string", r, 2);
}

// Test 6: Empty string should return 0 regardless of nByte
static bool test_empty_string_returns_zero() {
  const char s[] = "";
  int r1 = sqlite3Utf8CharLen(s, 0);
  int r2 = sqlite3Utf8CharLen(s, -1);
  bool ok = expect_eq_int("test_empty_string_returns_zero_r1", r1, 0) &&
            expect_eq_int("test_empty_string_returns_zero_r2", r2, 0);
  return ok;
}

// Test 7: Mixed ASCII and multibyte sequence with boundary in the middle
// String: a é b  -> bytes: 'a'(61) 0xC3 0xA9 'b'
static bool test_mixed_ascii_multibyte_boundary() {
  const char s[] = "a\xC3\xA9" "b"; // "aé" + "b" => 4 bytes + terminator
  int r_at_1 = sqlite3Utf8CharLen(s, 1); // boundary after 'a' (ASCII) => 1
  int r_at_2 = sqlite3Utf8CharLen(s, 2); // boundary into first multibyte char (should count 1)
  int r_at_3 = sqlite3Utf8CharLen(s, 3); // after first multibyte; should count 2 (é counted)
  bool ok = expect_eq_int("test_mixed_ascii_multibyte_boundary_r_at_1", r_at_1, 1) &&
            expect_eq_int("test_mixed_ascii_multibyte_boundary_r_at_2", r_at_2, 1) &&
            expect_eq_int("test_mixed_ascii_multibyte_boundary_r_at_3", r_at_3, 2);
  return ok;
}

// Main: run all tests
int main() {
  bool all_ok = true;
  all_ok &= test_ascii_within_boundary();
  all_ok &= test_ascii_beyond_end_boundary();
  all_ok &= test_multibyte_single_boundary_hit();
  all_ok &= test_multibyte_two_chars_boundaries();
  all_ok &= test_negative_nByte_counts_entire_string();
  all_ok &= test_empty_string_returns_zero();
  all_ok &= test_mixed_ascii_multibyte_boundary();

  if (all_ok) {
    std::cout << "ALL TESTS PASSED\n";
    return 0;
  } else {
    std::cerr << "SOME TESTS FAILED\n";
    return 1;
  }
}