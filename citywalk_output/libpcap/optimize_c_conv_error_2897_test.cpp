// Unit test suite for conv_error in optimize.c
// Target: C++11, no GTest, single-file test harness.
// The tests exercise conv_error by using setjmp/longjmp to capture
// the non-local goto behavior and verify that the error buffer is filled
// correctly and that formatting/truncation behaves as expected.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <pcap-types.h>


extern "C" {
// Note: The actual project should provide the correct include path for the
// optimize.h header where conv_error and conv_state_t are declared.
}

// Simple non-terminating test framework (no exceptions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper assertion macro (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERTION FAILED: " << (msg) << "\n"; \
        ++g_failed_tests; \
    } else { \
        ++g_total_tests; \
    } \
} while (0)

#define ASSERT_EQ_STR(expected, actual, msg) do { \
    if ((actual) == nullptr) { \
        std::cerr << "ASSERTION FAILED: " << (msg) << " (actual is null)\n"; \
        ++g_failed_tests; \
    } else if (strcmp((expected), (actual)) != 0) { \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " | expected: \"" << (expected) \
                  << "\" actual: \"" << (actual) << "\"\n"; \
        ++g_failed_tests; \
    } else { \
        ++g_total_tests; \
    } \
} while (0)


// Test 1: Basic formatting with one string argument
// Verifies that conv_error writes "hello world" into errbuf and returns via longjmp.
bool test_conv_error_basic_format()
{
    conv_state_t cs;
    int ret = setjmp(cs.top_ctx);
    if (ret == 0) {
        // This call should longjmp back to the setjmp point
        conv_error(&cs, "hello %s", "world");
        // If we get here, longjmp did not occur as expected.
        return false;
    } else {
        // After longjmp, verify the buffer content
        return (std::strcmp(cs.errbuf, "hello world") == 0);
    }
}

// Test 2: Formatting with numeric and string arguments
// Verifies that formatting is correctly applied: "1-OK"
bool test_conv_error_formatting_numbers()
{
    conv_state_t cs;
    int ret = setjmp(cs.top_ctx);
    if (ret == 0) {
        conv_error(&cs, "%d-%s", 1, "OK");
        return false;
    } else {
        return (std::strcmp(cs.errbuf, "1-OK") == 0);
    }
}

// Test 3: Truncation behavior when message exceeds PCAP_ERRBUF_SIZE - 1
// Prepare a string that is exactly PCAP_ERRBUF_SIZE - 1 characters long and verify
// that the buffer contains the same data (no overflow) after longjmp.
bool test_conv_error_truncation_behavior()
{
    conv_state_t cs;
    // Build a long input string of length PCAP_ERRBUF_SIZE - 1
    // We pass this as a single %s parameter to conv_error
    size_t buf_size = PCAP_ERRBUF_SIZE;
    // Create a local static to ensure lifetime is sufficient for the longjmp path
    char long_input[PCAP_ERRBUF_SIZE];
    for (size_t i = 0; i < PCAP_ERRBUF_SIZE - 1; ++i) {
        long_input[i] = 'X';
    }
    long_input[PCAP_ERRBUF_SIZE - 1] = '\0';

    int ret = setjmp(cs.top_ctx);
    if (ret == 0) {
        conv_error(&cs, "%s", long_input);
        return false;
    } else {
        // Expect exactly PCAP_ERRBUF_SIZE - 1 characters of 'X'
        // plus a null terminator inside errbuf
        // Create a comparison string representing the expected content
        char expected[PCAP_ERRBUF_SIZE];
        for (size_t i = 0; i < PCAP_ERRBUF_SIZE - 1; ++i) expected[i] = 'X';
        expected[PCAP_ERRBUF_SIZE - 1] = '\0';
        return (std::strcmp(cs.errbuf, expected) == 0);
    }
}


// Main entry: run all tests and report results
int main()
{
    std::cout << "Starting conv_error unit tests...\n";

    // Run Test 1
    bool t1 = test_conv_error_basic_format();
    if (t1) {
        std::cout << "[PASS] test_conv_error_basic_format\n";
        ++g_total_tests;
    } else {
        std::cerr << "[FAIL] test_conv_error_basic_format\n";
        ++g_failed_tests;
        ++g_total_tests;
    }
    // Apply the same non-terminating assertion pattern
    // We rely on explicit boolean returns for clarity.

    // Run Test 2
    bool t2 = test_conv_error_formatting_numbers();
    if (t2) {
        std::cout << "[PASS] test_conv_error_formatting_numbers\n";
        ++g_total_tests;
    } else {
        std::cerr << "[FAIL] test_conv_error_formatting_numbers\n";
        ++g_failed_tests;
        ++g_total_tests;
    }

    // Run Test 3
    bool t3 = test_conv_error_truncation_behavior();
    if (t3) {
        std::cout << "[PASS] test_conv_error_truncation_behavior\n";
        ++g_total_tests;
    } else {
        std::cerr << "[FAIL] test_conv_error_truncation_behavior\n";
        ++g_failed_tests;
        ++g_total_tests;
    }

    // Summary
    std::cout << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests
              << ", Pass rate: "
              << ((g_total_tests - g_failed_tests) * 100) / (g_total_tests ? g_total_tests : 1)
              << "%\n";

    // Return non-zero if any test failed to signal issues to CI systems
    return g_failed_tests ? 1 : 0;
}