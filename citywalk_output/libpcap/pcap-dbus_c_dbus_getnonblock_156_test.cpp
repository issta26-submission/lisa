// Test suite for the focal method: dbus_getnonblock(pcap_t *p) in pcap-dbus.c
// This file provides a small C++11-based test harness (no GTest) to verify the
// behavior of the focal function. The tests are designed to be non-terminating
// and continue execution after failures to maximize code coverage.

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dbus/dbus.h>
#include <config.h>
#include <pcap-dbus.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>


// Step 1 (Conceptual): Identify core dependent components.
// - dbus_getnonblock(pcap_t *p): sets p->errbuf with a fixed message and returns -1.
// - pcap_t: a struct containing errbuf (size PCAP_ERRBUF_SIZE).
// - PCAP_ERRBUF_SIZE: macro defining the size of the error buffer.
// - The test relies on the actual definitions from pcap-int.h and pcap-dbus.h.

extern "C" {
    // Include C headers defining the pcap_t structure and the focal function.
    #include "pcap-int.h"  // defines pcap_t and PCAP_ERRBUF_SIZE
    #include "pcap-dbus.h" // declares int dbus_getnonblock(pcap_t *p)
}

// Ensure the focal function is linked with C linkage in C++ tests
extern "C" int dbus_getnonblock(pcap_t *p);

static const char* EXPECTED_ERR =
    "Non-blocking mode isn't supported for capturing on D-Bus";

// Simple non-terminating test harness
static int total_tests = 0;
static int passed_tests = 0;

// Report helper that prints per-test results but does not exit the program
void report_result(const char* test_name, bool ok) {
    ++total_tests;
    if (ok) {
        ++passed_tests;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name << "\n";
    }
}

// Test 1: Verify that dbus_getnonblock returns -1 and writes the exact error message
bool test_dbus_getnonblock_sets_error_message() {
    // Prepare a valid pcap_t instance. The internal layout is provided by pcap-int.h.
    pcap_t p;
    // Zero-initialize to a clean state
    std::memset(&p, 0, sizeof(pcap_t));

    int ret = dbus_getnonblock(&p);

    // Check return value and the content of errbuf
    bool ok = (ret == -1) && (std::strcmp(p.errbuf, EXPECTED_ERR) == 0);
    return ok;
}

// Test 2: Verify that the function overwrites a pre-existing errbuf value
// This ensures the function does not leave stale content in errbuf.
bool test_dbus_getnonblock_overwrites_existing_errbuf() {
    pcap_t p;
    std::memset(&p, 0, sizeof(pcap_t));

    // Pre-fill errbuf with a distinct value to emulate a prior error
    // Use the maximum typical length to avoid overflow
    const char* initial = "PREVIOUS_ERROR_STATE";
    std::strncpy(p.errbuf, initial, PCAP_ERRBUF_SIZE);
    // Ensure null-termination in case strncpy truncated
    p.errbuf[PCAP_ERRBUF_SIZE - 1] = '\0';

    int ret = dbus_getnonblock(&p);

    bool ok = (ret == -1) && (std::strcmp(p.errbuf, EXPECTED_ERR) == 0);
    return ok;
}

// Main function: run all tests and print a short summary.
// This avoids terminating early on first failure to maximize coverage.
int main() {
    std::cout << "Starting test suite for dbus_getnonblock(pcap_t *p)\n";

    report_result("test_dbus_getnonblock_sets_error_message",
                  test_dbus_getnonblock_sets_error_message());

    report_result("test_dbus_getnonblock_overwrites_existing_errbuf",
                  test_dbus_getnonblock_overwrites_existing_errbuf());

    // Summary
    std::cout << "\nTest summary: " << passed_tests << " / " << total_tests
              << " tests passed.\n";

    // Return non-zero if any test failed to indicate overall failure
    return (passed_tests == total_tests) ? 0 : 1;
}