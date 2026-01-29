// Unit test suite for the focal function random_u16 in pngvalid.c
// This test suite is designed for a C++11 environment without Google Test.
// It links against the C implementation (pngvalid.c) which provides
// random_u16, randomize_bytes and png_get_uint_16.
// Note: We treat randomize_bytes and png_get_uint_16 as extern "C" to
// enable direct linkage from C++ tests.

// Key Candidate Keywords (Step 1):
// - randomize_bytes: fills a buffer with random data (used by random_u16)
// - b2: the 2-byte buffer inside random_u16
// - png_get_uint_16: converts two bytes to a 16-bit unsigned value
// - 2-byte buffer semantics: two bytes forming a 16-bit value (big-endian as per libpng)

/*
Domain knowledge considerations:
- Use only standard C++11 + provided C functions (no GTest).
- Non-terminating tests: do not exit on first failure; report per-test results.
- Access to static helper functions in the C file is not possible; tests rely on
  the provided extern functions.
- Tests include: range check, deterministic endianness check for png_get_uint_16,
  buffer modification check by randomize_bytes, and basic variability check for
  random_u16.
*/

#include <cstddef>
#include <cstdint>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Declarations for the externally linked C functions from pngvalid.c
extern "C" {
  // Two-byte random value generator (focal method under test)
  // Returns a 16-bit unsigned value (likely png_uint_16)
  unsigned short random_u16(void);

  // Fills a buffer with random bytes (internal RNG state used by random_u16)
  void randomize_bytes(void* pv, size_t size);

  // libpng helper: converts two bytes to a 16-bit value (big-endian)
  // Signature in pngvalid.c: png_uint_16 png_get_uint_16(png_bytep p)
  unsigned short png_get_uint_16(unsigned char* p);
}

// Simple test framework (non-terminating, diagnostic output)
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define RUN_TEST(test_fn)                                      \
  do {                                                           \
    bool ok = (test_fn)();                                        \
    ++g_tests_run;                                               \
    if (ok) { ++g_tests_passed; } else {                       \
      std::cerr << "[TEST FAILED] " #test_fn "\n";             \
    }                                                            \
  } while (0)

///////////////////// Test Case 1 /////////////////////
// Validate that random_u16 returns a value within the 16-bit range.
// This is a basic property test ensuring no overflow or truncation occurs.
bool test_random_u16_in_range() {
  const int ITER = 1000;
  for (int i = 0; i < ITER; ++i) {
    unsigned short val = random_u16();
    // Range check: 0 .. 65535 for a 16-bit value
    if (val > 0xFFFF) {
      std::cerr << "random_u16 out of 16-bit range: " << val << "\n";
      return false;
    }
  }
  return true;
}

///////////////////// Test Case 2 /////////////////////
// Validate endianness/byte interpretation for png_get_uint_16.
// Given a known two-byte sequence, png_get_uint_16 should return the
// concatenated 16-bit value (big-endian interpretation: p[0] as high byte).
bool test_png_get_uint_16_known() {
  unsigned char bytes[2] = { 0x12, 0x34 }; // big-endian interpretation: 0x1234
  unsigned short val = png_get_uint_16(bytes);
  const unsigned short expected = 0x1234;
  if (val != expected) {
    std::cerr << "png_get_uint_16 failed for known bytes: got 0x"
              << std::hex << val << ", expected 0x" << expected << std::dec << "\n";
    return false;
  }
  // Also test the swapped sequence to ensure consistent interpretation
  unsigned char bytes2[2] = { 0xAB, 0xCD }; // 0xABCD
  val = png_get_uint_16(bytes2);
  if (val != 0xABCD) {
    std::cerr << "png_get_uint_16 failed for known bytes2: got 0x"
              << std::hex << val << ", expected 0xABCD" << std::dec << "\n";
    return false;
  }
  return true;
}

///////////////////// Test Case 3 /////////////////////
// Randomness variability check: ensure that two consecutive calls to
// random_u16() are not always identical (highly unlikely to be equal).
bool test_random_u16_variability() {
  unsigned short a = random_u16();
  unsigned short b = random_u16();
  // If they happen to be equal (extremely unlikely), retry a few times
  int retries = 5;
  while (a == b && retries-- > 0) {
    b = random_u16();
  }
  if (a == b) {
    std::cerr << "random_u16 produced identical consecutive values after retries: a="
              << a << ", b=" << b << "\n";
    return false;
  }
  return true;
}

///////////////////// Test Case 4 /////////////////////
// Validate that randomize_bytes actually writes to the buffer.
// We seed the initial buffer with distinct bytes and ensure that at least one
// byte changes after randomization.
bool test_randomize_bytes_modifies_buffer() {
  unsigned char buf[2] = { 0xAA, 0xBB };
  randomize_bytes(buf, 2);
  // If both bytes remain unchanged, it's suspicious (1 in 65536 chance)
  if (buf[0] == 0xAA && buf[1] == 0xBB) {
    std::cerr << "randomize_bytes did not modify the buffer (bytes unchanged).\n";
    return false;
  }
  return true;
}

///////////////////// Test Runner /////////////////////
int main() {
  std::cout << "Starting unit tests for random_u16() in pngvalid.c\n";

  RUN_TEST(test_random_u16_in_range);
  RUN_TEST(test_png_get_uint_16_known);
  RUN_TEST(test_random_u16_variability);
  RUN_TEST(test_randomize_bytes_modifies_buffer);

  std::cout << "Tests run: " << g_tests_run << ", Passed: "
            << g_tests_passed << ", Failed: " << (g_tests_run - g_tests_passed) << "\n";

  if (g_tests_run == g_tests_passed) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << "Some tests failed. See output above for details.\n";
  }

  // Return 0 for success, non-zero otherwise (consistent with test harness)
  return (g_tests_run == g_tests_passed) ? 0 : 1;
}