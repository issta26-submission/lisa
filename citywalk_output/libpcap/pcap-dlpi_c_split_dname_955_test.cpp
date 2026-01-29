// This test suite targets the focal function split_dname from pcap-dlpi.c.
// It covers true/false branches of the critical predicates and validates
// both successful parsing and all defined error paths without terminating
// on first failure (non-terminating assertions).

// Domain knowledge notes applied:
// - Import dependencies and define necessary types (u_int).
// - Test both success and each error path.
// - Use a minimal, in-process test harness (no gtest/gmock).
// - Use correct C linkage for the focal C function.
// - Use only standard C++/C facilities available in the environment.

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/systeminfo.h>
#include <limits.h>
#include <sys/dlpi.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <os-proto.h>
#include <sys/bufmod.h>
#include <config.h>
#include <cstring>
#include <sys/stream.h>


#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// Provide C linkage for the C function under test.
extern "C" {
    typedef unsigned int u_int;
    // Declaration of the focal function under test.
    // The actual implementation should be linked with this test.
    char *split_dname(char *device, u_int *unitp, char *ebuf);
}

// Simple in-process test harness (non-terminating assertions)
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_CHECK(cond, msg) do { \
    tests_run++; \
    if (cond) { \
        tests_passed++; \
    } else { \
        tests_failed++; \
        std::cerr << "[FAIL] " << msg << "\n"; \
    } \
} while (0)

static void test_valid_eth0() {
    // Classic valid input: single trailing digit
    char dev[256];
    std::strcpy(dev, "eth0");
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    // Expect success, unit == 0, and return pointer to the first digit ('0')
    TEST_CHECK(res != nullptr, "split_dname should return non-NULL for 'eth0'");
    TEST_CHECK(unit == 0, "split_dname should parse unit 0 for 'eth0'");
    TEST_CHECK((res - dev) == 3, "split_dname should return pointer to the first digit ('0') in 'eth0'");
}

static void test_valid_eth012() {
    // Valid input with multiple digits
    char dev[256];
    std::strcpy(dev, "eth012");
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    // Expect success: digits "012" mean unit 12
    TEST_CHECK(res != nullptr, "split_dname should return non-NULL for 'eth012'");
    TEST_CHECK(unit == 12, "split_dname should parse unit 12 for 'eth012'");
    TEST_CHECK((res - dev) == 3, "split_dname should return pointer to the first digit ('0') in 'eth012'");
}

static void test_has_only_unit_number_no_slash() {
    // "12" should be rejected as having only a unit number
    char dev[256];
    std::strcpy(dev, "12");
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    TEST_CHECK(res == nullptr, "'12' should fail with NULL return");
    // Expect specific error
    const char* expected = "12 has only a unit number";
    TEST_CHECK(std::strcmp(errbuf, expected) == 0, "Error message should indicate device has only a unit number");
}

static void test_has_only_unit_number_with_slash() {
    // "abc/12" should be rejected due to '/' before digits
    char dev[256];
    std::strcpy(dev, "abc/12");
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    TEST_CHECK(res == nullptr, "'abc/12' should fail with NULL return");
    const char* expected = "abc/12 has only a unit number";
    TEST_CHECK(std::strcmp(errbuf, expected) == 0, "Error message should indicate device has only a unit number (with slash)");
}

static void test_missing_unit_number_last_char_non_digit() {
    // Last character is non-digit: "eth" -> missing unit number
    char dev[256];
    std::strcpy(dev, "eth");
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    TEST_CHECK(res == nullptr, "'eth' without trailing digits should fail");
    const char* expected = "eth missing unit number";
    TEST_CHECK(std::strcmp(errbuf, expected) == 0, "Error message should indicate missing unit number");
}

static void test_bad_unit_number_trailing_chars() {
    // Ends with digits but has trailing non-digit after digits: "eth1a" -> bad unit number
    char dev[256];
    std::strcpy(dev, "eth1a");
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    TEST_CHECK(res == nullptr, "'eth1a' should fail due to bad unit number");
    const char* expected_sub = "bad unit number";
    TEST_CHECK(std::strstr(errbuf, expected_sub) != nullptr, "Error message should indicate bad unit number");
}

static void test_unit_number_too_large() {
    // Extremely large unit number to trigger too-large error
    char dev[256];
    // Build a device like "eth" + a long sequence of digits
    std::strcpy(dev, "eth");
    for (int i = 0; i < 50; ++i) dev[std::strlen(dev) + i] = '9';
    dev[std::strlen(dev) + 50] = '\0';
    // Ensure string ends with digit
    // (Already ends with '9' due to loop)

    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    TEST_CHECK(res == nullptr, "Too large unit number should fail");
    // We can't rely on full exact message due to overflow behavior across platforms,
    // but we can ensure the message mentions "unit number too large".
    const char* expected_sub = "unit number too large";
    TEST_CHECK(std::strstr(errbuf, expected_sub) != nullptr, "Error should mention unit number too large");
}

static void test_bad_unit_number_due_to_trailing_non_digit_after_digits() {
    // Another form of bad number: "eth1a" already tested; additional variant
    test_bad_unit_number_trailing_chars();
}

static void test_missing_unit_number_last_char_with_extra() {
    // Non-digit at end after digits: "eth0x" -> missing unit number
    char dev[256];
    std::strcpy(dev, "eth0x");
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));
    u_int unit = 0;
    char *res = split_dname(dev, &unit, errbuf);

    TEST_CHECK(res == nullptr, "'eth0x' should fail due to missing unit number");
    const char* expected = "eth0x missing unit number";
    TEST_CHECK(std::strcmp(errbuf, expected) == 0, "Error message should indicate missing unit number for eth0x");
}

int main() {
    // Run tests
    test_valid_eth0();
    test_valid_eth012();
    test_has_only_unit_number_no_slash();
    test_has_only_unit_number_with_slash();
    test_missing_unit_number_last_char_non_digit();
    test_bad_unit_number_trailing_chars();
    test_unit_number_too_large();
    test_missing_unit_number_last_char_with_extra();

    // Summary
    std::cout << "\nTest results: " << tests_passed << "/" << tests_run << " passed, "
              << tests_failed << " failed.\n";
    return tests_failed ? 1 : 0;
}