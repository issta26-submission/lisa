/*
  Unit test suite for the focal method:
  void sqlite3Put4byte(unsigned char *p, u32 v)

  Notes:
  - This test is written in C++11 without any GTest or external test framework.
  - It exercises the behavior of sqlite3Put4byte by validating the 4-byte write
    against the compile-time endianness related branches.
  - The test uses a lightweight harness and prints a summary at the end.
  - We rely on the same endianness macros as used by the focal implementation:
    SQLITE_BYTEORDER, GCC_VERSION, MSVC_VERSION (when available in the build).
  - For environments where those macros are not defined, the else branch is exercised.
  - This test focuses on correctness, not performance, and ensures memory is written
    deterministically for representative values.
*/

#include <math.h>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the focal function using C linkage
extern "C" void sqlite3Put4byte(unsigned char *p, uint32_t v);

// Type aliases matching the project's usage in the focal method
using u8 = uint8_t;
using u32 = uint32_t;

/*
  Compute the expected 4-byte sequence for the value 'v' using the same
  macro-driven logic as sqlite3Put4byte. This allows the test to adapt to the
  compile-time macro configuration of SQLITE_BYTEORDER, GCC_VERSION, MSVC_VERSION.
*/
static void expectedBytesForValue(u32 v, unsigned char out[4]) {
#if defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER == 4321)
  // Little-endian host assumption: copy memory representation directly
  std::memcpy(out, &v, 4);
#elif defined(SQLITE_BYTEORDER) && (SQLITE_BYTEORDER == 1234)
  // Big-endian path with GCC >= 4.3.0
  #if defined(GCC_VERSION) && (GCC_VERSION >= 4003000)
  u32 x = __builtin_bswap32(v);
  std::memcpy(out, &x, 4);
  #elif defined(MSVC_VERSION) && (MSVC_VERSION >= 1300)
  u32 x = _byteswap_ulong(v);
  std::memcpy(out, &x, 4);
  #else
  // Fallback if specific compiler versions are not detected
  out[0] = (u8)(v>>24);
  out[1] = (u8)(v>>16);
  out[2] = (u8)(v>>8);
  out[3] = (u8)v;
  #endif
#else
  // Default path (neither 4321 nor 1234 detected)
  out[0] = (u8)(v>>24);
  out[1] = (u8)(v>>16);
  out[2] = (u8)(v>>8);
  out[3] = (u8)v;
#endif
}

/*
  Lightweight test harness
  - Each test function returns true on success, false on failure.
  - The main function runs all tests and prints a summary.
  - Non-terminating assertions: tests do not abort the entire run on failure to
    maximize coverage of subsequent code.
*/

// Helper: print a single test result
static void reportResult(const char* name, bool ok) {
  if (ok) {
    std::cout << "[PASS] " << name << std::endl;
  } else {
    std::cout << "[FAIL] " << name << std::endl;
  }
}

// Test 1: Validate that writing a common value produces the expected 4-byte sequence.
// This relies on the current compile-time endianness macros.
static bool test_put4byte_basic() {
  // Given value with distinct bytes to observe ordering
  const u32 v = 0x11223344;
  unsigned char p[4] = {0x00, 0x00, 0x00, 0x00};

  sqlite3Put4byte(p, v);

  unsigned char expected[4];
  expectedBytesForValue(v, expected);

  bool ok = (std::memcmp(p, expected, 4) == 0);
  if (!ok) {
    std::cout << "  Expected bytes: "
              << int(expected[0]) << " " << int(expected[1]) << " "
              << int(expected[2]) << " " << int(expected[3])
              << " but got: "
              << int(p[0]) << " " << int(p[1]) << " "
              << int(p[2]) << " " << int(p[3]) << std::endl;
  }
  return ok;
}

// Test 2: Ensure zero value is written correctly across all branches.
static bool test_put4byte_zero() {
  const u32 v = 0x00000000;
  unsigned char p[4] = {0xAA, 0xBB, 0xCC, 0xDD}; // pre-fill to ensure overwrite

  sqlite3Put4byte(p, v);

  unsigned char expected[4];
  expectedBytesForValue(v, expected);

  bool ok = (std::memcmp(p, expected, 4) == 0);
  if (!ok) {
    std::cout << "  Zero value test failed. Got: "
              << int(p[0]) << " " << int(p[1]) << " "
              << int(p[2]) << " " << int(p[3])
              << " but expected: "
              << int(expected[0]) << " " << int(expected[1]) << " "
              << int(expected[2]) << " " << int(expected[3]) << std::endl;
  }
  return ok;
}

// Test 3: Ensure writing the maximum unsigned value is handled correctly.
static bool test_put4byte_max() {
  const u32 v = 0xFFFFFFFF;
  unsigned char p[4] = {0x01, 0x02, 0x03, 0x04};

  sqlite3Put4byte(p, v);

  unsigned char expected[4];
  expectedBytesForValue(v, expected);

  bool ok = (std::memcmp(p, expected, 4) == 0);
  if (!ok) {
    std::cout << "  Max value test failed. Got: "
              << int(p[0]) << " " << int(p[1]) << " "
              << int(p[2]) << " " << int(p[3])
              << " but expected: "
              << int(expected[0]) << " " << int(expected[1]) << " "
              << int(expected[2]) << " " << int(expected[3]) << std::endl;
  }
  return ok;
}

// Test 4: Ensure that subsequent writes overwrite the same buffer correctly.
static bool test_put4byte_overwrite() {
  const u32 v1 = 0x01020304;
  const u32 v2 = 0xAABBCCDD;
  unsigned char p[4] = {0x00, 0x00, 0x00, 0x00};

  sqlite3Put4byte(p, v1);
  unsigned char exp1[4];
  expectedBytesForValue(v1, exp1);
  bool ok1 = (std::memcmp(p, exp1, 4) == 0);

  sqlite3Put4byte(p, v2);
  unsigned char exp2[4];
  expectedBytesForValue(v2, exp2);
  bool ok2 = (std::memcmp(p, exp2, 4) == 0);

  if (!ok1) {
    std::cout << "  Overwrite test (step 1) failed." << std::endl;
  }
  if (!ok2) {
    std::cout << "  Overwrite test (step 2) failed." << std::endl;
  }
  return ok1 && ok2;
}

// Main: run all tests and report summary
int main() {
  int passed = 0;
  int failed = 0;

  auto run = [&](const char* name, bool result){
    if (result) {
      ++passed;
      reportResult(name, true);
    } else {
      ++failed;
      reportResult(name, false);
    }
  };

  run("test_put4byte_basic", test_put4byte_basic());
  run("test_put4byte_zero", test_put4byte_zero());
  run("test_put4byte_max", test_put4byte_max());
  run("test_put4byte_overwrite", test_put4byte_overwrite());

  std::cout << "Summary: " << passed << " passed, " << failed << " failed." << std::endl;
  // Return non-zero if any test failed to aid integration in CI environments
  return failed ? 1 : 0;
}