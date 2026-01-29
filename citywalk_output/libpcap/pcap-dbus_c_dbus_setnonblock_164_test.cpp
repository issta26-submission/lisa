// Test suite for the focal method: dbus_setnonblock in pcap-dbus.c
// This test uses libpcap public API to obtain a valid pcap_t handle via pcap_open_dead
// and validates that dbus_setnonblock always returns -1 and reports the exact error
// message via pcap_geterr. The tests execute without GoogleTest (GTest) and use a
// lightweight custom assertion approach suitable for C++11.
// Note: Link against libpcap (e.g., -lpcap) when building this test.

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pcap.h>
#include <dbus/dbus.h>
#include <config.h>
#include <pcap-dbus.h>
#include <pcap-int.h>
#include <cstring>
#include <string>
#include <iostream>


extern "C" int dbus_setnonblock(pcap_t *p, int nonblock); // Prototype for the focal C function

// Global counter for failed tests (non-terminating assertions style)
static int g_test_failures = 0;

// Simple non-terminating assertion helper for string equality
static void assert_string_equal(const char* observed, const char* expected, const char* testName) {
    if (observed == nullptr) {
        std::cerr << "[FAIL] " << testName << " - observed NULL, expected: \"" << expected << "\"\n";
        ++g_test_failures;
        return;
    }
    if (std::string(observed) != std::string(expected)) {
        std::cerr << "[FAIL] " << testName << " - observed: \"" << observed
                  << "\", expected: \"" << expected << "\"\n";
        ++g_test_failures;
    } else {
        std::cout << "[PASS] " << testName << "\n";
    }
}

// Helper to create a valid pcap_t handle suitable for testing
static pcap_t* create_test_handle() {
    // Use pcap_open_dead to create a non-live, valid pcap_t object for testing.
    // The DLT_EN10MB (Ethernet) and a large snaplen are typical values.
    pcap_t* handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (handle == nullptr) {
        std::cerr << "[ERROR] Failed to create test pcap_t handle via pcap_open_dead\n";
    }
    return handle;
}

// Test 1: nonblock = 1 should return -1 and set the exact error message
static void test_dbus_setnonblock_nonblock_true() {
    const char* testName = "test_dbus_setnonblock_nonblock_true";
    pcap_t* handle = create_test_handle();
    if (handle == nullptr) {
        std::cerr << "[SKIP] " << testName << " - unable to create handle\n";
        ++g_test_failures;
        return;
    }

    int ret = dbus_setnonblock(handle, 1);

    const char* err = pcap_geterr(handle);
    const char* expected = "Non-blocking mode isn't supported for capturing on D-Bus";

    // Validate return value and error message
    if (ret != -1) {
        std::cerr << "[FAIL] " << testName << " - return value was " << ret
                  << " (expected -1)\n";
        ++g_test_failures;
    } else {
        // Verify exact error message content
        assert_string_equal(err, expected, testName);
    }

    pcap_close(handle);
}

// Test 2: nonblock = 0 should return -1 and set the exact error message
static void test_dbus_setnonblock_nonblock_false() {
    const char* testName = "test_dbus_setnonblock_nonblock_false";
    pcap_t* handle = create_test_handle();
    if (handle == nullptr) {
        std::cerr << "[SKIP] " << testName << " - unable to create handle\n";
        ++g_test_failures;
        return;
    }

    int ret = dbus_setnonblock(handle, 0);

    const char* err = pcap_geterr(handle);
    const char* expected = "Non-blocking mode isn't supported for capturing on D-Bus";

    // Validate return value and error message
    if (ret != -1) {
        std::cerr << "[FAIL] " << testName << " - return value was " << ret
                  << " (expected -1)\n";
        ++g_test_failures;
    } else {
        // Verify exact error message content
        assert_string_equal(err, expected, testName);
    }

    pcap_close(handle);
}

// Step 3: Test suite entry point (main)
int main() {
    // Run tests
    test_dbus_setnonblock_nonblock_true();
    test_dbus_setnonblock_nonblock_false();

    // Summary
    if (g_test_failures == 0) {
        std::cout << "[SUMMARY] All tests passed.\n";
        return 0;
    } else {
        std::cerr << "[SUMMARY] " << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}