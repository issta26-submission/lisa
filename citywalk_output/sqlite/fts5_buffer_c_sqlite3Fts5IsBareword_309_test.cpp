// Test suite for sqlite3Fts5IsBareword
// This file provides a small, self-contained test harness (no GTest) suitable for C++11.
// It exercises the focal function sqlite3Fts5IsBareword(char t) using a variety of inputs
// to cover true/false branches in the internal predicate.
// The tests rely on the function being linked from fts5_buffer.c (as provided in the project).

#include <iostream>
#include <string>
#include <fts5Int.h>


// Step 1: Interface to the focal function (C linkage)
extern "C" int sqlite3Fts5IsBareword(char t);

// Global counters for simple test reporting
static int gTestCount = 0;
static int gFailures = 0;

// Simple test logger
static void logTest(const std::string& name, bool ok) {
  ++gTestCount;
  if (!ok) {
    ++gFailures;
    std::cerr << "[FAILED] " << name << std::endl;
  } else {
    std::cout << "[OK]     " << name << std::endl;
  }
}

// Domain-driven tests for sqlite3Fts5IsBareword

// Test 1: High-bit set should always yield true due to (t & 0x80)
void test_MsbSet_true() {
  // 0x80
  int r1 = sqlite3Fts5IsBareword((char)0x80);
  logTest("sqlite3Fts5IsBareword(0x80) == true", r1 != 0);

  // 0xFF
  int r2 = sqlite3Fts5IsBareword((char)0xFF);
  logTest("sqlite3Fts5IsBareword(0xFF) == true", r2 != 0);

  // (char)0xA0 to exercise negative-char path in C/C++
  int r3 = sqlite3Fts5IsBareword((char)0xA0);
  logTest("sqlite3Fts5IsBareword((char)0xA0) == true", r3 != 0);
}

// Test 2: Digit/letter-based borewords from ASCII range (0x30..0x39 are digits)
void test_ASCII_30_true() {
  int r = sqlite3Fts5IsBareword((char)0x30); // '0'
  logTest("sqlite3Fts5IsBareword('0') == true", r != 0);
}

// Test 3: Known false in 0x00 .. 0x0F region (explicit 0x00)
void test_ASCII_0x00_false() {
  int r = sqlite3Fts5IsBareword((char)0x00);
  logTest("sqlite3Fts5IsBareword(0x00) == false", r == 0);
}

// Test 4: 0x11 is false according to the aBareword table segment 0x10..0x1F
void test_ASCII_0x11_false() {
  int r = sqlite3Fts5IsBareword((char)0x11);
  logTest("sqlite3Fts5IsBareword(0x11) == false", r == 0);
}

// Test 5: 0x12 is true (aBareword[0x12] == 1)
void test_ASCII_0x12_true() {
  int r = sqlite3Fts5IsBareword((char)0x12);
  logTest("sqlite3Fts5IsBareword(0x12) == true", r != 0);
}

// Test 6: 0x3C (less-than sign) is marked as bareword in 0x30..0x3F block
void test_ASCII_0x3C_true() {
  int r = sqlite3Fts5IsBareword((char)0x3C);
  logTest("sqlite3Fts5IsBareword(0x3C) == true", r != 0);
}

// Test 7: 0x3E (question mark) is not a bareword (false) in 0x30..0x3F block
void test_ASCII_0x3E_false() {
  int r = sqlite3Fts5IsBareword((char)0x3E);
  logTest("sqlite3Fts5IsBareword(0x3E) == false", r == 0);
}

// Test 8: 0x5B is false in 0x50..0x5F block (per table row)
void test_ASCII_0x5B_false() {
  int r = sqlite3Fts5IsBareword((char)0x5B);
  logTest("sqlite3Fts5IsBareword(0x5B) == false", r == 0);
}

// Test 9: 0x5F is true (last entry in 0x50..0x5F block)
void test_ASCII_0x5F_true() {
  int r = sqlite3Fts5IsBareword((char)0x5F);
  logTest("sqlite3Fts5IsBareword(0x5F) == true", r != 0);
}

// Test 10: 0x61 is true (ASCII 'a' region under 0x60..0x6F)
void test_ASCII_0x61_true() {
  int r = sqlite3Fts5IsBareword((char)0x61);
  logTest("sqlite3Fts5IsBareword(0x61) == true", r != 0);
}

// Test 11: 0x7B is false (0x70..0x7F region ends with several false values)
void test_ASCII_0x7B_false() {
  int r = sqlite3Fts5IsBareword((char)0x7B);
  logTest("sqlite3Fts5IsBareword(0x7B) == false", r == 0);
}

// Test 12: 0x7F is false in the final block
void test_ASCII_0x7F_false() {
  int r = sqlite3Fts5IsBareword((char)0x7F);
  logTest("sqlite3Fts5IsBareword(0x7F) == false", r == 0);
}

// Test 13: Additional test for explicit negative-valued char to ensure MSB path
void test_negative_char_explict_true() {
  char c = static_cast<char>(0xFF);
  int r = sqlite3Fts5IsBareword(c);
  logTest("sqlite3Fts5IsBareword((char)0xFF) == true", r != 0);
}

// Test runner
void runAllTests() {
  // The tests are designed to cover true/false branches of the predicate.
  test_MsbSet_true();
  test_ASCII_30_true();
  test_ASCII_0x00_false();
  test_ASCII_0x11_false();
  test_ASCII_0x12_true();
  test_ASCII_0x3C_true();
  test_ASCII_0x3E_false();
  test_ASCII_0x5B_false();
  test_ASCII_0x5F_true();
  test_ASCII_0x61_true();
  test_ASCII_0x7B_false();
  test_ASCII_0x7F_false();
  test_negative_char_explict_true();
}

// Main: run the tests and report summary
int main() {
  std::cout << "Running sqlite3Fts5IsBareword test suite (C++11, no GTest)..." << std::endl;
  runAllTests();
  std::cout << "Total tests: " << gTestCount << ", Failures: " << gFailures << std::endl;
  // Return non-zero if any test failed to aid integration with build systems
  return gFailures;
}