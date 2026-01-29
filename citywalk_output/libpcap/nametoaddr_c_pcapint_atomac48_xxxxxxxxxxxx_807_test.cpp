// Comprehensive C++11 unit test suite for pcapint_atomac48_xxxxxxxxxxxx
// This test harness is designed to be used without GoogleTest.
// It uses simple non-terminating assertions and prints PASS/FAIL per test.
// The test suite assumes the presence of the C function:
//   int pcapint_atomac48_xxxxxxxxxxxx(const char *s, uint8_t *addr)
// and the helper function:
//   unsigned int pcapint_xdtoi(const unsigned char c)
// provided by the project (nametoaddr.c and related headers).

#include <gencode.h>
#include <stdio.h>
#include <ethertype.h>
#include <iostream>
#include <errno.h>
#include <nametoaddr.h>
#include <iomanip>
#include <thread-local.h>
#include <cstdint>
#include <string>
#include <llc.h>
#include <pcap/namedb.h>
#include <diag-control.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <os-proto.h>
#include <config.h>
#include <cstring>


extern "C" {
  // Prototypes from the focal code base (C linkage)
  unsigned int pcapint_xdtoi(const unsigned char c);
  int pcapint_atomac48_xxxxxxxxxxxx(const char *s, uint8_t *addr);
}

// Simple non-terminating test harness helpers
static void test_pass(const char* testname) {
  std::cout << "PASS: " << testname << std::endl;
}
static void test_fail(const char* testname, const std::string& reason) {
  std::cout << "FAIL: " << testname << " - " << reason << std::endl;
}
static void test_fail_details(const char* testname, const std::string& reason, const uint8_t* actual, size_t n) {
  std::cout << "FAIL: " << testname << " - " << reason << " [addr=";
  for (size_t i = 0; i < n; ++i) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)actual[i];
  }
  std::cout << std::dec << "]" << std::endl;
}

// Helper to print a 6-byte MAC-like address for debugging
static std::string addr_to_string(const uint8_t addr[6]) {
  std::ostringstream oss;
  for (int i = 0; i < 6; ++i) {
    if (i) oss << ":";
    oss << std::hex << std::setw(2) << std::setfill('0') << (int)addr[i];
  }
  return oss.str();
}

// Test 1: Valid 12 lowercase hex digits -> expect success and exact bytes
static bool test_valid_lowercase() {
  const char s[13] = "0123456789ab"; // exactly 12 hex digits
  uint8_t out[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
  int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);

  // Expected bytes: 0x01, 0x23, 0x45, 0x67, 0x89, 0xab
  const uint8_t expected[6] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab};

  if (ret != 1) {
    test_fail("test_valid_lowercase", "return value != 1 on valid 12-digit input");
    test_fail_details("test_valid_lowercase", "addr", out, 6);
    return false;
  }
  if (std::memcmp(out, expected, sizeof(expected)) != 0) {
    test_fail_details("test_valid_lowercase", "addr bytes do not match expected", out, 6);
    return false;
  }
  test_pass("test_valid_lowercase");
  return true;
}

// Test 2: Valid 12 uppercase hex digits -> expect success and exact bytes
static bool test_valid_uppercase() {
  const char s[13] = "0123456789AB"; // 12 hex digits, uppercase A-F
  uint8_t out[6];
  std::memset(out, 0xAA, sizeof(out));

  int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);
  const uint8_t expected[6] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB};

  if (ret != 1) {
    test_fail("test_valid_uppercase", "return value != 1 on valid 12-digit input");
    test_fail_details("test_valid_uppercase", "addr", out, 6);
    return false;
  }
  if (std::memcmp(out, expected, sizeof(expected)) != 0) {
    test_fail_details("test_valid_uppercase", "addr bytes do not match expected", out, 6);
    return false;
  }
  test_pass("test_valid_uppercase");
  return true;
}

// Test 3: Valid mixed-case hex digits -> expect correct decoding
static bool test_valid_mixed_case() {
  // "a1B2c3D4e5F6" -> bytes: 0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
  const char s[13] = "a1B2c3D4e5F6";
  uint8_t out[6];
  int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);
  const uint8_t expected[6] = {0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6};

  if (ret != 1) {
    test_fail("test_valid_mixed_case", "return value != 1 on valid mixed-case input");
    test_fail_details("test_valid_mixed_case", "addr", out, 6);
    return false;
  }
  if (std::memcmp(out, expected, sizeof(expected)) != 0) {
    test_fail_details("test_valid_mixed_case", "addr bytes do not match expected", out, 6);
    return false;
  }
  test_pass("test_valid_mixed_case");
  return true;
}

// Test 4: Invalid length (11) -> expect failure and no guaranteed addr change
static bool test_invalid_length_no_change() {
  const char s[12] = "0123456789a"; // 11 chars
  uint8_t out[6];
  std::memset(out, 0x11, sizeof(out));

  int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);
  if (ret != 0) {
    test_fail("test_invalid_length_no_change", "expected return 0 for invalid length");
    test_fail_details("test_invalid_length_no_change", "addr", out, 6);
    return false;
  }
  // addr may remain unchanged; we check it remains all 0x11
  const uint8_t expected[6] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11};
  if (std::memcmp(out, expected, sizeof(expected)) != 0) {
    test_fail_details("test_invalid_length_no_change", "addr was modified on invalid input", out, 6);
    return false;
  }
  test_pass("test_invalid_length_no_change");
  return true;
}

// Test 5: Invalid character (non-hex) -> expect failure and no change
static bool test_invalid_char_no_change() {
  const char s[13] = "01234g6789ab"; // 'g' is not a hex digit
  uint8_t out[6];
  std::memset(out, 0x22, sizeof(out));

  int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);
  if (ret != 0) {
    test_fail("test_invalid_char_no_change", "expected return 0 for input with non-hex char");
    test_fail_details("test_invalid_char_no_change", "addr", out, 6);
    return false;
  }
  const uint8_t expected[6] = {0x22, 0x22, 0x22, 0x22, 0x22, 0x22};
  if (std::memcmp(out, expected, sizeof(expected)) != 0) {
    test_fail_details("test_invalid_char_no_change", "addr was modified on invalid input", out, 6);
    return false;
  }
  test_pass("test_invalid_char_no_change");
  return true;
}

// Test 6: All 'f' hex digits -> should decode to 0xFF, 0xFF, ...
static bool test_all_ff() {
  const char s[13] = "ffffffffffff";
  uint8_t out[6];
  int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);
  const uint8_t expected[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  if (ret != 1) {
    test_fail("test_all_ff", "expected return 1 for valid input of all 'f's");
    test_fail_details("test_all_ff", "addr", out, 6);
    return false;
  }
  if (std::memcmp(out, expected, sizeof(expected)) != 0) {
    test_fail_details("test_all_ff", "addr bytes do not match 0xFF", out, 6);
    return false;
  }
  test_pass("test_all_ff");
  return true;
}

// Test 7: Ensure address is unchanged on failure with multiple test variants
static bool test_addr_unchanged_on_failure_variants() {
  // Variant A: invalid length
  {
    const char s[12] = "0123456789a"; // 11 chars
    uint8_t out[6] = {0x10,0x20,0x30,0x40,0x50,0x60};
    int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);
    if (ret != 0) {
      test_fail("test_addr_unchanged_on_failure_variants-A", "expected 0 on invalid length");
      test_fail_details("test_addr_unchanged_on_failure_variants-A", "addr", out, 6);
      return false;
    }
    const uint8_t expected[6] = {0x10,0x20,0x30,0x40,0x50,0x60};
    if (std::memcmp(out, expected, sizeof(expected)) != 0) {
      test_fail_details("test_addr_unchanged_on_failure_variants-A", "addr modified on failure", out, 6);
      return false;
    }
  }
  // Variant B: invalid char
  {
    const char s[13] = "01234g6789ab";
    uint8_t out[6] = {0x01,0x02,0x03,0x04,0x05,0x06};
    int ret = pcapint_atomac48_xxxxxxxxxxxx(s, out);
    if (ret != 0) {
      test_fail("test_addr_unchanged_on_failure_variants-B", "expected 0 on invalid char");
      test_fail_details("test_addr_unchanged_on_failure_variants-B", "addr", out, 6);
      return false;
    }
    const uint8_t expected[6] = {0x01,0x02,0x03,0x04,0x05,0x06};
    if (std::memcmp(out, expected, sizeof(expected)) != 0) {
      test_fail_details("test_addr_unchanged_on_failure_variants-B", "addr modified on failure", out, 6);
      return false;
    }
  }

  test_pass("test_addr_unchanged_on_failure_variants");
  return true;
}

// Main: Run all tests
int main() {
  bool all_passed = true;

  std::cout << "Starting unit tests for pcapint_atomac48_xxxxxxxxxxxx" << std::endl;

  all_passed &= test_valid_lowercase();
  all_passed &= test_valid_uppercase();
  all_passed &= test_valid_mixed_case();
  all_passed &= test_invalid_length_no_change();
  all_passed &= test_invalid_char_no_change();
  all_passed &= test_all_ff();
  all_passed &= test_addr_unchanged_on_failure_variants();

  if (all_passed) {
    std::cout << "ALL TESTS PASSED" << std::endl;
  } else {
    std::cout << "ONE OR MORE TESTS FAILED" << std::endl;
  }
  return all_passed ? 0 : 1;
}