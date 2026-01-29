// C++11 test suite for the focal method: is_dlpi_interface in pcap-libdlpi.c
// This test suite is designed to be run without Google Test (GTest).
// It uses a lightweight in-house test harness that continues execution even after a failed assertion.

#include <string.h>
#include <errno.h>
#include <dlpisubs.h>
#include <libdlpi.h>
#include <memory.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/bufmod.h>
#include <stropts.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <config.h>
#include <sys/types.h>
#include <cstring>
#include <sys/stream.h>
#include <iostream>


// Declare the focal C function. It is defined in pcap-libdlpi.c (linked during build).
extern "C" int is_dlpi_interface(const char *name);

// Simple global counter for failed assertions to allow non-terminating tests.
static int g_failures = 0;

// Lightweight assertion macro that does not terminate the test suite on failure.
// It reports the failure and increments the global failure counter.
#define CHECK_EQ(expected, actual) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "Failure at " << __FILE__ << ":" << __LINE__ \
                      << " - (" #actual ") != (" #expected ") " \
                      << " => actual: " << (actual) << ", expected: " << (expected) \
                      << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Test 1: Verify that the focal function returns 1 for a variety of common interface names.
// Rationale: This checks the positive (true-like) behavior implied by the function's contract.
// Note: The real function in the provided code always returns 1, so this test validates
// that the API remains stable across typical inputs.
void test_is_dlpi_interface_various_names_returns_one() {
    // Candidate Names representing typical DLPI interface identifiers
    const char *names[] = {
        "eth0",
        "lo0",
        "wlan0",
        "en0",
        "dummy0",
        nullptr
    };

    for (const char **p = names; *p != nullptr; ++p) {
        int ret = is_dlpi_interface(*p);
        CHECK_EQ(1, ret);
    }
    // Explanation: This test ensures that for a set of representative input strings,
    // the function consistently returns 1 as per the focal implementation.
}

// Test 2: Verify that the focal function returns 1 for an empty string.
// Rationale: Edge-case test to ensure no unexpected behavior occurs with empty input.
void test_is_dlpi_interface_empty_string_returns_one() {
    int ret = is_dlpi_interface("");
    CHECK_EQ(1, ret);
}

// Test 3: Verify that the focal function returns 1 for a very long interface name.
// Rationale: Stress the input size to ensure there is no buffer-related issue in the call.
// Note: The function is expected to return 1 regardless of input size in the provided focal code.
void test_is_dlpi_interface_long_string_returns_one() {
    char longname[512];
    // Fill with non-zero characters to simulate a long identifier; ensure null-termination.
    std::memset(longname, 'a', sizeof(longname) - 1);
    longname[sizeof(longname) - 1] = '\0';

    int ret = is_dlpi_interface(longname);
    CHECK_EQ(1, ret);
}

// Test 4: Verify that the function does not modify the input string (input const-correctness).
// Rationale: Confirm that the caller's input buffer remains unchanged after the call.
void test_is_dlpi_interface_input_not_modified() {
    char name[8] = "eth0";
    char before[8];
    std::strcpy(before, name);

    int ret = is_dlpi_interface(name);
    CHECK_EQ(1, ret);
    // Compare the input buffer to ensure no modification occurred.
    CHECK_EQ(0, std::strcmp(name, before));
}

// Run all tests. This function ensures tests are executed from main as required
// when not using a dedicated test framework like GTest.
void run_all_tests() {
    test_is_dlpi_interface_various_names_returns_one();
    test_is_dlpi_interface_empty_string_returns_one();
    test_is_dlpi_interface_long_string_returns_one();
    test_is_dlpi_interface_input_not_modified();
}

// Entry point for the test executable.
int main() {
    run_all_tests();

    if (g_failures) {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1; // Non-zero return indicates failure to the test harness.
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}