// SPDX-License-Identifier: MIT
// Unit tests for gzfilebuf::underflow() without using Google Test.
// This test suite uses a lightweight, non-terminating assertion style.
// It focuses on key branches in gzfilebuf::underflow() as described in the prompt.

#include <cstring>
#include <cstdio>
#include <zfstream.h>
#include <iostream>


// Include the focal class header

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;

#define TEST_OK(cond, msg) do {                                 \
    ++g_total_tests;                                              \
    if (static_cast<bool>(cond)) {                                \
        ++g_passed_tests;                                         \
        std::cout << "[OK] " << msg << std::endl;               \
    } else {                                                      \
        std::cout << "[FAIL] " << msg << std::endl;             \
    }                                                             \
} while (0)

// Test harness: extend gzfilebuf to expose protected API for testing.
class gzfilebuf_Test : public gzfilebuf {
public:
  // Public wrapper to invoke the protected underflow()
  int call_underflow() { return underflow(); }

  // Expose a helper to set the get area (begin, current, end)
  void set_get_area(char* beg, char* cur, char* end) {
    // setg is a protected member of std::streambuf; derived class can call it.
    setg(beg, cur, end);
  }
};

// Test 1: When there is already data in the get area (gptr() != null and gptr() < egptr()),
// underflow should return the next character from the get area without touching the file.
void test_underflow_uses_existing_get_area() {
  gzfilebuf_Test t;
  // Prepare a tiny buffer with a known content.
  char buf[4] = { 'A', 'B', 'C', '\0' };

  // Set get area so that gptr() is non-null and gptr() < egptr()
  // Beginning at buf, current position at buf, end at buf + 1 (only one byte available)
  t.set_get_area(buf, buf, buf + 1);

  int ret = t.call_underflow();

  // Expect to get the first character ('A') without performing any gzread.
  TEST_OK(ret == (int)('A'), "underflow uses existing get area: returns first available character ('A')");
}

// Test 2: When the file hasn't been opened for reading (not open or mode not in),
// underflow should return EOF. We simulate "not open" by closing the streambuf
// before invoking underflow.
void test_underflow_not_open_returns_eof() {
  gzfilebuf_Test t;
  // Explicitly close to simulate not-open state (is_open() should become false).
  t.close();

  int ret = t.call_underflow();

  int eof_expected = gzfilebuf::traits_type::eof();
  TEST_OK(ret == eof_expected, "underflow returns EOF when file is not open for reading");
}

// Main function to run tests
int main() {
  std::cout << "Starting gzfilebuf::underflow() unit tests (C++11, no gtest)..." << std::endl;

  test_underflow_uses_existing_get_area();
  test_underflow_not_open_returns_eof();

  std::cout << "Tests completed. Passed " << g_passed_tests << " of " << g_total_tests << " tests." << std::endl;

  // Return non-zero if any test failed (optional, keeps compatibility with simple test runners)
  return (g_total_tests == g_passed_tests) ? 0 : 1;
}