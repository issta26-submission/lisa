// C++11 unit tests for sqlite3Fts3GetVarint
// This test suite targets the focal method:
//   int sqlite3Fts3GetVarint(const char *pBuf, sqlite_int64 *v)
// which delegates to sqlite3Fts3GetVarintU(U) to decode a 64-bit varint.
// The tests are designed to be non-terminating (they collect failures and report them),
// and do not rely on Google Test. They use a lightweight custom assertion mechanism.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Domain-specific typedefs to mirror the SQLite types used by the focal method.
// We assume a 64-bit width for both signed and unsigned variants.
typedef long long sqlite_int64;
typedef unsigned long long sqlite_uint64;

// Declaration of the focal function with C linkage for proper symbol resolution.
extern "C" int sqlite3Fts3GetVarint(const char *pBuf, sqlite_int64 *v);

// Lightweight non-terminating assertion macro.
// On failure, logs the message and increments a global failure counter, but continues execution.
static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "FAIL: " << msg << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Namespace to encapsulate test cases (avoids polluting global scope).
namespace fts3_unit_tests {

  // Test 1: Decode the smallest varint value (0) using a single-byte encoding.
  // Expected: v == 0, return value == 1 (one byte consumed).
  void test_varint_zero() {
    const unsigned char buf[] = { 0x00 };  // single-byte varint encoding for 0
    sqlite_int64 v = 0;
    int ret = sqlite3Fts3GetVarint(reinterpret_cast<const char*>(buf), &v);

    CHECK(ret == 1, "varint_zero: expected return length 1");
    CHECK(v == 0, "varint_zero: expected decoded value 0");
  }

  // Test 2: Decode a small single-byte value (127).
  // Expected: v == 127, return value == 1.
  void test_varint_127() {
    const unsigned char buf[] = { 0x7F };  // 0x7F = 127, MSB 0 => single-byte
    sqlite_int64 v = 0;
    int ret = sqlite3Fts3GetVarint(reinterpret_cast<const char*>(buf), &v);

    CHECK(ret == 1, "varint_127: expected return length 1");
    CHECK(v == 127, "varint_127: expected decoded value 127");
  }

  // Test 3: Decode a two-byte varint (128).
  // Encoding: 0x80 0x01 represents 128 (first byte continuation bit set, second byte last).
  // Expected: v == 128, return value == 2.
  void test_varint_128_two_bytes() {
    const unsigned char buf[] = { 0x80, 0x01 };
    sqlite_int64 v = 0;
    int ret = sqlite3Fts3GetVarint(reinterpret_cast<const char*>(buf), &v);

    CHECK(ret == 2, "varint_128_two_bytes: expected return length 2");
    CHECK(v == 128, "varint_128_two_bytes: expected decoded value 128");
  }

  // Test 4: Decode a larger two-byte value (300).
  // Encoding: 0xAC 0x02 (0xAC = 0x80 | 0x2C, continuation bit set; 0x02 last byte)
  // Expected: v == 300, return value == 2.
  void test_varint_300_two_bytes() {
    const unsigned char buf[] = { 0xAC, 0x02 };
    sqlite_int64 v = 0;
    int ret = sqlite3Fts3GetVarint(reinterpret_cast<const char*>(buf), &v);

    CHECK(ret == 2, "varint_300_two_bytes: expected return length 2");
    CHECK(v == 300, "varint_300_two_bytes: expected decoded value 300");
  }

  // Run all tests in this namespace.
  void run_all() {
    test_varint_zero();
    test_varint_127();
    test_varint_128_two_bytes();
    test_varint_300_two_bytes();
  }
}

// Entry point for the test executable.
int main() {
  // Run all tests and report results.
  fts3_unit_tests::run_all();

  if(g_failures > 0) {
    std::cerr << "Total failures: " << g_failures << std::endl;
    return 1; // Non-zero exit indicates test failures.
  } else {
    std::cout << "All tests passed." << std::endl;
    return 0;
  }
}