// Unit test suite for pcap_libdlpi_err (no Google Test, pure C++11 test harness)
// This test suite is designed to be linked against the C implementation of
// pcap_libdlpi_err and the accompanying dlpi_strerror symbol.
// It uses a lightweight, non-terminating assertion mechanism to maximize
// test coverage without stopping execution on first failure.

#include <string.h>
#include <errno.h>
#include <dlpisubs.h>
#include <libdlpi.h>
#include <memory.h>
#include <sys/time.h>
#include <stdio.h>
#include <cstdio>
#include <sys/bufmod.h>
#include <stropts.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <config.h>
#include <string>
#include <sys/types.h>
#include <cstring>
#include <sys/stream.h>
#include <iostream>


// Ensure PCAP_ERRBUF_SIZE is defined for buffer sizing.
// If not defined by the build system, provide a reasonable default.
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// C linkage for the focal function and its dependency.
extern "C" {
    // Focal function under test
    void pcap_libdlpi_err(const char *linkname, const char *func, int err, char *errbuf);

    // Dependency used by the focal function to convert error codes to strings
    // This is provided by the underlying libdlpi in the actual project.
    const char* dlpi_strerror(int err);
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macro
#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "TEST FAILED: " << (msg) \
                      << "  [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
            ++g_failed_tests; \
        } \
        ++g_total_tests; \
    } while (0)

// Helper to compute the error string portion after the ": "
static std::string after_colon_space(const std::string& s) {
    size_t pos = s.find(": ");
    if (pos == std::string::npos) return "";
    return s.substr(pos + 2);
}

// Test 1: Basic usage with typical small strings
// Verifies the prefix, and that the trailing error string matches dlpi_strerror(err)
static void test_basic_usage() {
    const char* linkname = "eth0";
    const char* func = "pcap_open_live";
    int err = 2;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_libdlpi_err(linkname, func, err, errbuf);

    std::string s(errbuf);
    std::string expected_prefix = std::string(func) + " failed on " + std::string(linkname) + ": ";

    // Check that the formatted message starts with "<func> failed on <link>: "
    CHECK(s.find(expected_prefix) == 0,
          "Message should start with '<func> failed on <link>: ' prefix");

    // After the colon-space, there should be a non-empty error string
    std::string rest = after_colon_space(s);
    CHECK(!rest.empty(), "Trailing dlpi_strerror(err) must be non-empty");

    // The trailing part should match the string returned by dlpi_strerror(err)
    const char* expected = dlpi_strerror(err);
    CHECK(rest == std::string(expected),
          "Trailing error string should match dlpi_strerror(err)");
}

// Test 2: Long input strings to exercise buffer handling and formatting
// Ensures that long function/linkname inputs still produce a well-formed message
// and that the total length does not overflow the buffer (PCAP_ERRBUF_SIZE - 1 at most)
static void test_long_inputs() {
    // Build long strings (within reasonable bounds so snprintf does not truncate excessively)
    std::string long_link(120, 'L'); // 120 chars
    std::string long_func(100, 'F'); // 100 chars

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_libdlpi_err(long_link.c_str(), long_func.c_str(), 13, errbuf);

    std::string s(errbuf);

    // Prefix should reflect provided long inputs
    std::string expected_prefix = long_func + " failed on " + long_link + ": ";
    CHECK(s.find(expected_prefix) == 0,
          "Long input: message should start with long '<func> failed on <link>: ' prefix");

    // Ensure length is within buffer bounds (should be <= PCAP_ERRBUF_SIZE - 1)
    CHECK(static_cast<size_t>(s.size()) <= static_cast<size_t>(PCAP_ERRBUF_SIZE - 1),
          "Message length must not overflow PCAP_ERRBUF_SIZE minus terminator");

    // The trailing part should match dlpi_strerror(13)
    std::string rest = after_colon_space(s);
    const char* expected = dlpi_strerror(13);
    CHECK(rest == std::string(expected),
          "Trailing error string should match dlpi_strerror(13)");
}

// Test 3: Zero error code path
// Verifies that err == 0 is correctly translated to its string
static void test_zero_error_code() {
    const char* linkname = "lo";
    const char* func = "pcap_close";
    int err = 0;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_libdlpi_err(linkname, func, err, errbuf);

    std::string s(errbuf);
    std::string rest = after_colon_space(s);
    const char* expected = dlpi_strerror(err);
    CHECK(rest == std::string(expected),
          "Zero error code should map to dlpi_strerror(0) string");
}

// Test 4: Empty string inputs
// Verifies behavior when empty strings are passed for linkname and func
static void test_empty_strings() {
    const char* linkname = "";
    const char* func = "";
    int err = 1;
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_libdlpi_err(linkname, func, err, errbuf);

    std::string s(errbuf);
    // Expect the format to reflect empty func/linkname gracefully
    std::string expected_prefix = "" + std::string(" failed on ") + "" + std::string(": ");
    // Since func and linkname are empty, the constructed string starts with " failed on : "
    CHECK(s.find(" failed on : ") == 0,
          "Empty func/linkname should produce a message starting with ' failed on : '");

    // Trailing error string should match dlpi_strerror(1)
    std::string rest = after_colon_space(s);
    const char* expected = dlpi_strerror(err);
    CHECK(rest == std::string(expected),
          "Trailing error string should match dlpi_strerror(1) for empty inputs");
}

// Entry point
int main() {
    // Run tests
    test_basic_usage();
    test_long_inputs();
    test_zero_error_code();
    test_empty_strings();

    // Summary
    std::cout << "Total tests executed: " << g_total_tests << std::endl;
    std::cout << "Total tests failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}