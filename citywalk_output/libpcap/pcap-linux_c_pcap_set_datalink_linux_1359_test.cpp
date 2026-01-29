/*
Unit test suite for pcap_set_datalink_linux(pcap_t *handle, int dlt)

Step 1 - Candidate Keywords (represented in comments for traceability)
- pcap_t, handle, linktype
- set_vlan_offset(handle)
- pcap_open_dead, pcap_datalink
- DLT_EN10MB, DLT_IEEE802_11 (example DLT values)
- Linux datalink handling, VLAN offset update side-effect
- return value (0 on success)

Step 2 - Test approach
- Use libpcap public API to exercise pcap_set_datalink_linux via a real pcap_t handle obtained from pcap_open_dead.
- Validate observable state change by querying pcap_datalink(handle) after invocation.
- Verify that the function returns 0 (success) for the tested datalink values.
- Run multiple test scenarios to cover representative DLT values.

Step 3 - Test refinement
- Include a test that exercises multiple DLT values in a loop to ensure consistent behavior across values.
- Use a lightweight, non-terminating test harness (no GTest), printing per-test results to stdout/stderr.
- Ensure proper cleanup with pcap_close after each test.

Note: This test suite assumes libpcap development headers and library are available in the environment, and that pcap_open_dead/pcap_datalink/pcap_set_datalink_linux are accessible. It runs under C++11 as requested.

Code begins here
*/

// Minimal C++11 test harness that does not rely on GTest.
// It uses libpcap public API to exercise pcap_set_datalink_linux and verify observable state
// via pcap_datalink. Each test prints its pass/fail status and returns a non-zero value on failure.

#include <netinet/in.h>
#include <pcap/vlan.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <netlink/msg.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <pcap-snf.h>
#include <pcap/can_socketcan.h>
#include <pcap.h>
#include <pcap-util.h>
#include <dirent.h>
#include <netlink/genl/genl.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <string>
#include <diag-control.h>
#include <vector>
#include <sys/socket.h>
#include <sys/stat.h>
#include <linux/sockios.h>
#include <endian.h>
#include <linux/net_tstamp.h>
#include <linux/filter.h>
#include <poll.h>
#include <netlink/genl/family.h>
#include <limits.h>
#include <stdlib.h>
#include <linux/nl80211.h>
#include <pcap-int.h>
#include <netlink/attr.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/eventfd.h>
#include <linux/types.h>
#include <linux/if_arp.h>
#include <linux/ethtool.h>
#include <netlink/genl/ctrl.h>
#include <linux/if_bonding.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <sys/utsname.h>


// Simple non-terminating test reporter
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_test_pass(const std::string &name) {
    std::cout << "[PASS] " << name << std::endl;
}

static void log_test_fail(const std::string &name, const std::string &reason) {
    std::cerr << "[FAIL] " << name << " - " << reason << std::endl;
    ++g_failed_tests;
}

// Helper: run a test and report
#define RUN_TEST(test_func)                          \
    do {                                             \
        if ((test_func)()) { /* non-zero means failure */ \
            log_test_fail(#test_func, "Test reported failure"); \
        } else {                                      \
            log_test_pass(#test_func);               \
        }                                             \
    } while (0)

// Test 1: Verify that pcap_set_datalink_linux changes the datalink of a handle
// by comparing the observable datalink value via pcap_datalink after the call.
static int test_pcap_set_datalink_linux_changes_datalink_to_known_value() {
    const char *test_name = "pcap_set_datalink_linux_changes_datalink_to_known_value";

    // Create a dead capture handle with a known initial datalink type
    pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (handle == nullptr) {
        log_test_fail(test_name, "Failed to create dead pcap_t handle with pcap_open_dead");
        return 1;
    }

    // Choose a different datalink type to test the change
    int new_dlt = DLT_IEEE802_11;
    // Call the function under test
    int ret = pcap_set_datalink_linux(handle, new_dlt);
    if (ret != 0) {
        pcap_close(handle);
        log_test_fail(test_name, "pcap_set_datalink_linux returned non-zero");
        return 1;
    }

    // Observe the resulting datalink via the public accessor
    int current = pcap_datalink(handle);
    pcap_close(handle);

    if (current != new_dlt) {
        std::string reason = "Expected datalink " + std::to_string(new_dlt) +
                             " but got " + std::to_string(current);
        log_test_fail(test_name, reason);
        return 1;
    }

    // All checks passed
    log_test_pass(test_name);
    return 0;
}

// Test 2: Ensure the function returns 0 (success) for another datalink value
static int test_pcap_set_datalink_linux_returns_zero_for_second_dlt() {
    const char *test_name = "pcap_set_datalink_linux_returns_zero_for_second_dlt";

    pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
    if (handle == nullptr) {
        log_test_fail(test_name, "Failed to create dead pcap_t handle with pcap_open_dead");
        return 1;
    }

    int new_dlt = DLT_IEEE80211; // Some environments define DLT_IEEE80211 as alternative spelling
    // Some libpcap headers define DLT_IEEE802_11; to maintain broad compatibility,
    // try both spellings if available. We attempt a common one first.
    // Fallback to DLT_IEEE802_11 if DLT_IEEE80211 is not defined by the compiler will be handled by preprocessor.

#ifdef DLT_IEEE80211
    new_dlt = DLT_IEEE80211;
#else
    // If not defined, reuse the constant from previous test scenario
    new_dlt = DLT_IEEE802_11;
#endif

    int ret = pcap_set_datalink_linux(handle, new_dlt);
    if (ret != 0) {
        pcap_close(handle);
        log_test_fail(test_name, "pcap_set_datalink_linux returned non-zero");
        return 1;
    }

    int current = pcap_datalink(handle);
    pcap_close(handle);

    if (current != new_dlt) {
        std::string reason = "Expected datalink " + std::to_string(new_dlt) +
                             " but got " + std::to_string(current);
        log_test_fail(test_name, reason);
        return 1;
    }

    log_test_pass(test_name);
    return 0;
}

// Test 3: Regression-style test: exercise multiple DLT values to ensure consistent behavior
static int test_pcap_set_datalink_linux_multiple_values() {
    const char *test_name = "pcap_set_datalink_linux_multiple_values";

    // A small set of representative datalink types to validate behavior
    std::vector<int> dlt_values;

    dlt_values.push_back(DLT_EN10MB);

#ifdef DLT_IEEE802_11
    dlt_values.push_back(DLT_IEEE802_11);
#endif

    // Some environments may offer DLT_IEEE802_11_RADIO or similar; include if available
#ifdef DLT_IEEE802_11_RADIO
    dlt_values.push_back(DLT_IEEE802_11_RADIO);
#endif

    // Optionally ensure DLT_RAW if defined
#ifdef DLT_RAW
    dlt_values.push_back(DLT_RAW);
#endif

    bool all_ok = true;

    for (size_t i = 0; i < dlt_values.size(); ++i) {
        int new_dlt = dlt_values[i];
        pcap_t *handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (handle == nullptr) {
            log_test_fail(test_name, "Failed to create dead pcap_t handle in loop");
            all_ok = false;
            continue;
        }

        int ret = pcap_set_datalink_linux(handle, new_dlt);
        if (ret != 0) {
            pcap_close(handle);
            log_test_fail(test_name, "pcap_set_datalink_linux returned non-zero in loop");
            all_ok = false;
            continue;
        }

        int current = pcap_datalink(handle);
        pcap_close(handle);

        if (current != new_dlt) {
            std::string reason = "Loop index " + std::to_string(i) +
                                 ": Expected datalink " + std::to_string(new_dlt) +
                                 " but got " + std::to_string(current);
            log_test_fail(test_name, reason);
            all_ok = false;
            continue;
        }
    }

    if (all_ok) {
        log_test_pass(test_name);
        return 0;
    } else {
        return 1;
    }
}

// Main: run all tests
int main() {
    std::cout << "Starting unit tests for pcap_set_datalink_linux (C++11 harness, non-GTest)" << std::endl;

    RUN_TEST(test_pcap_set_datalink_linux_changes_datalink_to_known_value);
    RUN_TEST(test_pcap_set_datalink_linux_returns_zero_for_second_dlt);
    RUN_TEST(test_pcap_set_datalink_linux_multiple_values);

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}