// Unit test suite for the focal method: testPrintError
// This test suite is written for C++11, does not rely on Google Test,
// and uses a lightweight, self-contained test harness.
// It captures stderr output by temporarily redirecting the file descriptor.

#include <functional>
#include <string.h>
#include <lsmtest.h>
#include <vector>
#include <sys/types.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


extern "C" void testPrintError(const char* zFormat, ...); // Focal function under test (C linkage)

// Capture the output that testPrintError writes to stderr by redirecting STDERR to a pipe.
// This version uses variadic templates (C++11) to forward arbitrary arguments to testPrintError.
template<typename... Args>
static std::string captureOutput(const char* fmt, Args... args) {
  int pipefd[2];
  if (pipe(pipefd) != 0) {
    return ""; // On failure, return empty
  }

  // Save current stderr
  int savedErr = dup(STDERR_FILENO);
  // Ensure any pending I/O is flushed
  fflush(stderr);
  // Redirect stderr to the write end of the pipe
  dup2(pipefd[1], STDERR_FILENO);
  // Close the write end as it's now duplicated to STDERR
  close(pipefd[1]);

  // Invoke the focal function with forwarded arguments
  testPrintError(fmt, args...);

  // Flush and restore stderr
  fflush(stderr);
  dup2(savedErr, STDERR_FILENO);
  close(savedErr);

  // Read the captured output from the pipe
  std::string output;
  char buffer[4096];
  ssize_t r;
  while ((r = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
    output.append(buffer, static_cast<size_t>(r));
  }
  close(pipefd[0]);
  return output;
}

// Lightweight test harness (non-terminating assertions) to maximize code coverage.
static int g_total = 0;
static int g_failed = 0;

#define TEST_CASE(name, expected, producer)                                    \
  do {                                                                         \
    ++g_total;                                                                 \
    std::string got = (producer)();                                            \
    if (got == (expected)) {                                                 \
      /* Test passed: output matches expected */                              \
      std::cout << "[PASS] " << (name) << std::endl;                          \
    } else {                                                                   \
      ++g_failed;                                                              \
      std::cerr << "[FAIL] " << (name) << "\n"                                \
                << "  Expected: \"" << (expected) << "\"\n"                   \
                << "  Got:      \"" << got << "\"\n";                        \
    }                                                                          \
  } while (0)

int main() {
  // Candidate Keywords extracted from Step 1:
  // testPrintError, vfprintf, stderr, variadic arguments, format strings
  // We create tests to exercise various format specifiers and edge cases.

  // Test 1: Print a simple string with no additional arguments.
  // Expected: exactly "hello"
  TEST_CASE("testPrintError simple string", "hello", []() {
    return captureOutput("hello");
  });

  // Test 2: Print with a string argument using %s.
  // Expected: "hello world"
  TEST_CASE("testPrintError with string argument", "hello world", []() {
    return captureOutput("hello %s", "world");
  });

  // Test 3: Print with integer and float formatting.
  // Expected: "num=42, flt=3.140000" (default printf formatting)
  TEST_CASE("testPrintError with int and float", "num=42, flt=3.140000", []() {
    return captureOutput("num=%d, flt=%f", 42, 3.14);
  });

  // Test 4: Print a percent literal using %%.
  // Expected: "special: %"
  TEST_CASE("testPrintError percent literal", "special: %", []() {
    return captureOutput("special: %%");
  });

  // Test 5: Print an empty string using %s with an empty argument.
  // Expected: ""
  TEST_CASE("testPrintError empty string argument", "", []() {
    return captureOutput("%s", "");
  });

  // Summary
  std::cout << "Total tests: " << g_total << ", Passed: " << (g_total - g_failed)
            << ", Failed: " << g_failed << std::endl;

  return (g_failed == 0) ? 0 : 1;
}