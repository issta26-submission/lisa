/*
  Lightweight C++11 test suite for the focal function:
  pcapint_vfmt_errmsg_for_errno

  Notes:
  - This test suite is designed to be compiled in a C++11 environment without GTest.
  - It uses a small in-file test harness with non-terminating assertions (tests return pass/fail).
  - It relies on the C library function pcapint_vfmt_errmsg_for_errno (extern "C").
  - It uses pcap_strerror(int) to obtain the system-specific error string for ENOENT.
  - A small wrapper test_call_vfmt_errmsg_for_errno is provided to call the variadic function
    through a non-variadic interface for ease of testing via a variadic wrapper.
  - Test cases cover:
    1) Insufficient buffer to append the error string (boundary/early return path).
    2) Normal path where the error string is appended after the provided message.
    3) Boundary case where only "fmt: " (no error text) can be appended due to space constraints.
    4) Formatting with additional format specifiers to ensure vsnprintf is used and the result is correct.
*/

#include <string.h>
#include <portability.h>
#include <errno.h>
#include <fmtutils.h>
#include <stdarg.h>
#include <cstdarg>
#include <stddef.h>
#include <stdio.h>
#include <cstdio>
#include <config.h>
#include <charconv.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>
#include <ftmacros.h>


// Declarations for the focal C function and related helpers.
// These signatures mirror the ones used in the original C implementation.
extern "C" {
    // Core focal function under test (non-variadic wrapper is provided below).
    size_t pcapint_vfmt_errmsg_for_errno(char *errbuf, size_t errbuflen, int errnum,
                                         const char *fmt, va_list ap);

    // Helper to obtain a readable OS error string for a given errno.
    const char* pcap_strerror(int errnum);

    // Wrapper to call the variadic interface for tests (implemented in C++)
    size_t test_call_vfmt_errmsg_for_errno(char* buf, size_t buflen, int errnum,
                                           const char* fmt, ...);
}

// Implementation of the variadic wrapper to enable testing of the variadic entry point.
extern "C" size_t test_call_vfmt_errmsg_for_errno(char* buf, size_t buflen, int errnum,
                                                  const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    // Call the non-variadic variant which expects a va_list.
    size_t ret = pcapint_vfmt_errmsg_for_errno(buf, buflen, errnum, fmt, ap);
    va_end(ap);
    return ret;
}

// Simple test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void report_test(const std::string& name, bool passed, const std::string& details = "")
{
    ++g_total_tests;
    if (passed) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        ++g_failed_tests;
        std::cout << "[FAIL] " << name;
        if (!details.empty()) {
            std::cout << " - " << details;
        }
        std::cout << std::endl;
    }
}

// Helper to fetch a stable expected error string for ENOENT using pcap_strerror
static std::string errno_string(int errnum)
{
    const char* s = pcap_strerror(errnum);
    if (s == nullptr) return "";
    return std::string(s);
}

// Test 1: Insufficient buffer to append the error string.
// When errbuflen is too small to accommodate the appended ": <error>", the function should
// return early, leaving the buffer as produced by vsnprintf (i.e., only the formatted message).
static void test_insufficient_buffer_no_append()
{
    char buf[4]; // 3 bytes for len, 1 byte for NUL
    // Use a small buffer so that vsnprintf writes at most 2 characters ("ok" fits in 2).
    // msglen = 2, errbuflen = 3, 2 + 3 > 3 => early return; buffer should be "ok"
    size_t ret = test_call_vfmt_errmsg_for_errno(buf, 3, ENOENT, "ok");
    std::string actual = std::string(buf);
    std::string expected = "ok";
    report_test("test_insufficient_buffer_no_append", actual == expected,
                "actual=\"" + actual + "\" expected=\"" + expected + "\"");
}

// Test 2: Normal path where the message is appended with the system error string.
static void test_append_error_string()
{
    char buf[256];
    // Use a simple message, large enough space to append the error string.
    size_t ret = test_call_vfmt_errmsg_for_errno(buf, sizeof(buf), ENOENT, "test");
    std::string actual = std::string(buf);
    std::string expected = "test: " + errno_string(ENOENT);
    report_test("test_append_error_string", actual == expected,
                "actual=\"" + actual + "\" expected=\"" + expected + "\"");
}

// Test 3: Boundary case where there is space for the "fmt: " prefix but not the error text.
// This simulates msglen + 3 == errbuflen, which should allow adding ':' and ' ' but leave
// no room for the actual error string.
static void test_boundary_only_prefix_no_error()
{
    // Choose a message length of 5 ("ABCDE"), and buffer length = 8.
    // msglen = 5, errbuflen = 8 -> remaining = 3; after writing ':' and ' ', remaining becomes 1.
    // snprintf(..., 1, "%s", ...) writes nothing; result should be "ABCDE: "
    char buf[16];
    size_t ret = test_call_vfmt_errmsg_for_errno(buf, 8, ENOENT, "ABCDE");
    std::string actual = std::string(buf);
    std::string expected = "ABCDE: ";
    report_test("test_boundary_only_prefix_no_error", actual == expected,
                "actual=\"" + actual + "\" expected=\"" + expected + "\"");
}

// Test 4: Formatting with additional format specifiers to ensure vsnprintf is used.
// The result should be "errno=<d> range=<d>: <error_string>"
static void test_format_with_specifiers()
{
    char buf[256];
    size_t ret = test_call_vfmt_errmsg_for_errno(buf, sizeof(buf), ENOENT, "errno=%d range=%d", 7, 3);
    std::string actual = std::string(buf);
    std::string expected = "errno=7 range=3: " + errno_string(ENOENT);
    report_test("test_format_with_specifiers", actual == expected,
                "actual=\"" + actual + "\" expected=\"" + expected + "\"");
}

// Main entry: run all tests
int main()
{
    std::cout << "pcapint_vfmt_errmsg_for_errno test suite (C++11, no GTest)" << std::endl;

    test_insufficient_buffer_no_append();
    test_append_error_string();
    test_boundary_only_prefix_no_error();
    test_format_with_specifiers();

    int passed = g_total_tests - g_failed_tests;
    std::cout << "Tests run: " << g_total_tests << ", Passed: " << passed
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests == 0 ? 0 : 1;
}