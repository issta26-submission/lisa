/*
Step 1 - Program Understanding and Candidate Keywords
- FOCAL_METHOD: Fail(const char* frm, ...)
  - Behavior: Formats a message using va_list into a global buffer ReasonToFailBuffer via vsprintf.
  - Core components: vsprintf, va_list, va_start, va_end, ReasonToFailBuffer, variadic function handling.
- FOCAL_CLASS_DEP (exposed excerpts): Declares a number of test harness utilities, including Die, SubTest, Check, Dot, Say, and importantly Fail.
- Candidate Keywords representing the method's core dependent components:
  - Fail, ReasonToFailBuffer
  - va_list, va_start, va_end, vsprintf
  - frm (format string), variadic arguments
  - Variadic formatting behavior and potential buffer overflow considerations (buffer used without explicit size here)
  - This test suite focuses on verifying that Fail correctly formats input into ReasonToFailBuffer for a few representative cases.

Note: The actual project is C/C code with a C-style variadic function. We create a small, self-contained C++11 test harness that calls Fail via the provided C interface, and inspects the global ReasonToFailBuffer. We do not rely on any testing framework (as requested).

Step 2 - Unit Test Generation Plan
- Target: Fail in testcms2.c (via testcms2.h declaration)
- Access: ReasonToFailBuffer is assumed to be an externally visible global buffer declared in testcms2.h.
- Test cases (basic coverage of true branch behavior):
  1) No-format string: Fail("Hello world") should copy exactly "Hello world" into ReasonToFailBuffer.
  2) Simple formatting with one integer: Fail("Code %d", 42) should yield "Code 42".
  3) Mixed string and integer: Fail("Error %s code %d", "404", 404) should yield "Error 404 code 404".
- Coverage goals:
  - True branch: correct formatting path for simple and multi-argument cases.
  - False/edge behavior: not strictly applicable since Fail uses vsprintf; we ensure proper handling of typical inputs and that buffer content is exactly as expected for tested inputs.
- Static scope and non-private access:
  - We do not access any private data; we only rely on the public extern buffer and the public function Fail.

Step 3 - Test Case Refinement
- Use a compact C++11 test harness (no GTest). Implement three test functions with explicit comments describing intent.
- Each test resets ReasonToFailBuffer before invocation to ensure independence.
- Assertions are non-terminating: tests log failures but do not exit immediately, so all tests run.
- Provide a simple main that runs all tests and prints a summary.
- Use only standard C++11 facilities plus the provided C interface (testcms2.h). Do not mock any C++-only private/static internals.

Now the test code:

*/

// test_fail.cpp
#include <cstring>
#include <iostream>
#include <string>
#include <testcms2.h>


extern "C" {
    // Include the C header that declares Fail and ReasonToFailBuffer
    #include "testcms2.h"
}

// Simple, non-terminating test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const char* test_name, const char* message) {
    std::cerr << "TEST FAILED: " << test_name << " - " << message << "\n";
    ++g_failed_tests;
}
static void log_success(const char* test_name) {
    std::cout << "TEST PASSED: " << test_name << "\n";
}
static void record_result(bool passed, const char* test_name, const char* detail = nullptr) {
    ++g_total_tests;
    if (passed) {
        log_success(test_name);
    } else {
        if (detail && std::strlen(detail) > 0)
            log_failure(test_name, detail);
        else
            log_failure(test_name, "unexpected result");
    }
}

// Helper utilities
// Access to ReasonToFailBuffer is provided by the C interface (extern declaration in header)
static const char* read_buffer() {
    return ReasonToFailBuffer;
}

// Test 1: Basic formatting with no additional arguments
static void test_Fail_no_args() {
    const char* test_name = "Fail_no_args";

    // Reset buffer to a known state
    ReasonToFailBuffer[0] = '\0';

    // Call the focal method
    Fail("Hello world");

    // Expect exact content
    const char* actual = read_buffer();
    const char* expected = "Hello world";

    if (actual != nullptr && std::strcmp(actual, expected) == 0) {
        record_result(true, test_name);
    } else {
        std::string detail = "expected: 'Hello world', got: '";
        detail += (actual ? actual : "<null>");
        detail += "'";
        record_result(false, test_name, detail.c_str());
    }
}

// Test 2: Simple formatting with one integer argument
static void test_Fail_format_int() {
    const char* test_name = "Fail_format_int";

    ReasonToFailBuffer[0] = '\0';
    Fail("Code %d", 42);

    const char* actual = read_buffer();
    const char* expected = "Code 42";

    if (actual != nullptr && std::strcmp(actual, expected) == 0) {
        record_result(true, test_name);
    } else {
        std::string detail = "expected: 'Code 42', got: '";
        detail += (actual ? actual : "<null>");
        detail += "'";
        record_result(false, test_name, detail.c_str());
    }
}

// Test 3: Mixed string and integer formatting
static void test_Fail_format_string_and_int() {
    const char* test_name = "Fail_format_string_and_int";

    ReasonToFailBuffer[0] = '\0';
    Fail("Error %s code %d", "404", 404);

    const char* actual = read_buffer();
    const char* expected = "Error 404 code 404";

    if (actual != nullptr && std::strcmp(actual, expected) == 0) {
        record_result(true, test_name);
    } else {
        std::string detail = "expected: 'Error 404 code 404', got: '";
        detail += (actual ? actual : "<null>");
        detail += "'";
        record_result(false, test_name, detail.c_str());
    }
}

// Step 3: Run all tests and print summary
int main() {
    // Run tests
    test_Fail_no_args();
    test_Fail_format_int();
    test_Fail_format_string_and_int();

    // Summary
    std::cout << "Total tests: " << g_total_tests << "\n";
    std::cout << "Failed tests: " << g_failed_tests << "\n";

    return (g_failed_tests == 0) ? 0 : 1;
}