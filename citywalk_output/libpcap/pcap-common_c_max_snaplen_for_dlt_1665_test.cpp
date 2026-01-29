/*
 * Lightweight C++11 test suite for max_snaplen_for_dlt from pcap-common.c
 * - No GoogleTest; uses a minimal, non-terminating assertion framework
 * - Tests:
 *     * Known DLT values return expected snaplen
 *     * Default/unknown DLT values return MAXIMUM_SNAPLEN
 * - Assumes the project headers define DLT_DBUS, DLT_EBHSCR, DLT_USBPCAP and MAXIMUM_SNAPLEN
 * - Linkage to C code is via extern "C"
 *
 * Explanatory comments are included for each unit test.
 */

#include <pcap-common.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// Include project headers to obtain DLT_* constants and MAXIMUM_SNAPLEN

extern "C" {
    // Explicitly declare the function under test for C linkage.
    int max_snaplen_for_dlt(int dlt);
}

// Minimal test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion for integers
static void expect_eq_int(int a, int b, const char* file, int line)
{
    ++g_tests_run;
    if (a != b) {
        std::cerr << "Test failed at " << file << ":" << line
                  << " - Expected: " << b << ", Actual: " << a << std::endl;
        ++g_tests_failed;
    }
}

// Convenience macro to capture file/line information
#define EXPECT_EQ(a, b) expect_eq_int((a), (b), __FILE__, __LINE__)

// Test 1: Validate that known DLT constants map to their expected snaplen values
// - DLT_DBUS -> 128 * 1024 * 1024
// - DLT_EBHSCR -> 8 * 1024 * 1024
// - DLT_USBPCAP -> 1024 * 1024
static void test_max_snaplen_for_known_dlts()
{
    // Expected values computed to avoid magic numbers in assertions
    const int expected_dbus   = 128 * 1024 * 1024; // 134217728
    const int expected_ebhscr = 8 * 1024 * 1024;    // 8388608
    const int expected_usbpcap = 1024 * 1024;       // 1048576

    // true branches: exact matches for each DLT case
    EXPECT_EQ(max_snaplen_for_dlt(DLT_DBUS), expected_dbus);
    EXPECT_EQ(max_snaplen_for_dlt(DLT_EBHSCR), expected_ebhscr);
    EXPECT_EQ(max_snaplen_for_dlt(DLT_USBPCAP), expected_usbpcap);
}

// Test 2: Validate that non-matching DLT values fall back to MAXIMUM_SNAPLEN
// - Ensures the default case is exercised and returns the static maximum
static void test_max_snaplen_for_unknown_dlts()
{
    // Values guaranteed not to match the three explicit cases
    const int unknown_value_1 = -1;
    const int unknown_value_2 = 0;
    const int unknown_value_3 = 999999;

    // Default branch should be taken for non-matching values
    EXPECT_EQ(max_snaplen_for_dlt(unknown_value_1), MAXIMUM_SNAPLEN);
    EXPECT_EQ(max_snaplen_for_dlt(unknown_value_2), MAXIMUM_SNAPLEN);
    EXPECT_EQ(max_snaplen_for_dlt(unknown_value_3), MAXIMUM_SNAPLEN);
}

// Entry point for tests
int main()
{
    // Run tests
    test_max_snaplen_for_known_dlts();
    test_max_snaplen_for_unknown_dlts();

    // Summary of test results
    int tests_passed = g_tests_run - g_tests_failed;
    std::cout << "Test Summary: "
              << "Run=" << g_tests_run
              << ", Passed=" << tests_passed
              << ", Failed=" << g_tests_failed
              << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}