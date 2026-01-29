// SQLite-like DecOrHex to I64 unit test suite (C++11, no GoogleTest)
// This test targets the focal method:
//   int sqlite3DecOrHexToI64(const char *z, i64 *pOut)
// The test suite is implemented in C++11, uses manual test harness (no GTest),
// and provides minimal C stubs for dependent helpers to ensure hex/decimal paths
// are exercised. It links against util.c (the focal implementation).

#include <math.h>
#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Domain-specific typedefs to mirror sqlite's types
typedef long long i64;
typedef unsigned long long u64;
typedef unsigned char u8;

// Forward declaration of the focal function (to be linked with util.c)
extern "C" int sqlite3DecOrHexToI64(const char *z, i64 *pOut);

// Provide working implementations for dependent helpers that the focal
// function relies on when compiled with the given class dependencies.
// We purposefully override weak/dummy definitions from the testable file
// to ensure correct behavior during tests.

extern "C" {

// Determine if a character is a hexadecimal digit (0-9, a-f, A-F)
int sqlite3Isxdigit(int c) {
  unsigned char ch = (unsigned char)c;
  if (ch >= '0' && ch <= '9') return 1;
  if (ch >= 'a' && ch <= 'f') return 1;
  if (ch >= 'A' && ch <= 'F') return 1;
  return 0;
}

// Convert a hex digit character to its numeric value (0-15)
int sqlite3HexToInt(int h) {
  unsigned char ch = (unsigned char)h;
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
  if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
  return 0;
}

// Atoi64 helper used by the decimal path in sqlite3DecOrHexToI64.
// Implement a calibrated decimal parser that respects the length parameter.
// For the purposes of unit testing, we implement a robust yet straightforward parser.
int sqlite3Atoi64(const char *zNum, i64 *pNum, int length, u8 enc) {
  // Basic, safe decimal parsing with optional sign, up to 'length' characters
  // We tolerate leading whitespace as the decimal path allowed in the original code
  int idx = 0;
  // Build a small working buffer from the input range
  int end = length;
  // Skip leading whitespace
  while (idx < end && (zNum[idx] == ' ' || zNum[idx] == '\n' || zNum[idx] == '\t' || zNum[idx] == '\r'))
    idx++;

  int sign = 1;
  if (idx < end && (zNum[idx] == '+' || zNum[idx] == '-')) {
    if (zNum[idx] == '-') sign = -1;
    idx++;
  }

  i64 val = 0;
  bool anyDigit = false;
  while (idx < end && zNum[idx] >= '0' && zNum[idx] <= '9') {
    anyDigit = true;
    int d = zNum[idx] - '0';
    val = val * 10 + d;
    idx++;
  }

  if (!anyDigit) {
    // No digits were found; mimic failure behavior
    return 1;
  }

  *pNum = sign * val;
  return 0;
}

} // extern "C"

// Simple test harness
struct TestCase {
  std::string name;
  const char* z;
  i64 expectedOut;
  int expectedRet;
  bool checkOut; // whether to verify expectedOut
};

// Helper macro to print test result details
#define LOG_TEST_FAIL(name, details) \
  std::cerr << "[FAIL] " << name << ": " << details << "\n";

// Main function to run tests
int main() {
  std::vector<TestCase> tests = {
    // Hex tests
    // 1) Simple hex: 0x1A -> 26, expect success (ret 0)
    {"Hex Simple 0x1A", "0x1A", 26, 0, true},
    // 2) Hex with leading zeros: 0x0000 -> 0, expect success
    {"Hex LeadingZeros 0x0000", "0x0000", 0, 0, true},
    // 3) Hex with invalid trailing hex digit (non-hex after digits): 0x1G -> return 1
    {"Hex TrailingInvalid 0x1G", "0x1G", 0, 1, false},
    // 4) Hex with more than 16 hex digits after 0x triggers overflow code 2
    {"Hex Overflow 0x123456789ABCDEF0123", "0x123456789ABCDEF0123", 0, 2, false},
    // 5) Hex with just 0x and no digits: "0x" -> treated as success with 0
    {"Hex Empty 0x", "0x", 0, 0, true},

    // Decimal tests
    // 6) Decimal simple: 123 -> 123, ret 0
    {"Decimal Simple 123", "123", 123, 0, true},
    // 7) Decimal with sign: -456 -> -456, ret 0
    {"Decimal Negative -456", "-456", -456, 0, true},
    // 8) Decimal with leading whitespace and plus: " +789" -> 789, ret 0
    {"Decimal WithSpace +789", " +789", 789, 0, true},
    // 9) Decimal with leading spaces and no trailing digits but valid end: " 0" -> 0, ret 0
    {"Decimal Zero WithSpace  0", " 0", 0, 0, true},

    // 10) Decimal path with non-digit after digits within length limit: "123a" -> should parse 123
    {"Decimal Mixed 123a", "123a", 123, 0, true}
  };

  int total = 0;
  int passed = 0;

  // Run tests
  for (const auto &tc : tests) {
    total++;
    i64 outVal = 0;
    int ret = sqlite3DecOrHexToI64(tc.z, &outVal);
    bool retOk = (ret == tc.expectedRet);
    bool outOk = true;
    if (tc.checkOut) {
      outOk = (outVal == tc.expectedOut);
    } else {
      // If not verifying output (due to hex overflow or undefined pOut after certain return codes),
      // we skip output verification.
      outOk = true;
    }

    if (retOk && outOk) {
      passed++;
    } else {
      std::string details;
      details += "ret=" + std::to_string(ret) + " (expected " + std::to_string(tc.expectedRet) + ")";
      if (tc.checkOut) {
        details += "; out=" + std::to_string(outVal) + " (expected " + std::to_string(tc.expectedOut) + ")";
      } else {
        details += "; out=" + std::to_string(outVal);
      }
      LOG_TEST_FAIL(tc.name, details);
    }
  }

  // Summary
  std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";

  // Return non-zero if any test failed
  return (passed == total) ? 0 : 1;
}

// Explanatory notes (for maintainers):
// - Hex tests cover: valid hex, leading zeros, trailing non-hex, overflow (>16 hex digits),
//   and empty hex ("0x") scenarios.
// - Decimal tests cover: simple decimal, signed decimal, whitespace handling, and a mixed
//   alpha character following digits to ensure decimal parsing stops correctly.
// - We provide minimal C stubs (sqlite3Isxdigit, sqlite3HexToInt, sqlite3Atoi64) to ensure
//   the focal function can compile and run in isolation within this test harness.
// - The binary is intended to be linked with the implementation file util.c (which defines
//   sqlite3DecOrHexToI64). The macro SQLITE_OMIT_HEX_INTEGER is not defined here to ensure
//   the hex code path is exercised. If needed, you can introduce corresponding macro guards
//   in the build to isolate specific paths.