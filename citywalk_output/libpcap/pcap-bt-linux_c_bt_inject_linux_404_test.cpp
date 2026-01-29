// Test suite for the focal method bt_inject_linux (pcap-bt-linux.c) using a
// lightweight C++11 test harness (no Google Test).
// The tests assume a libpcap-like environment where pcap_t has a member
// errbuf[PCAP_ERRBUF_SIZE], which is used by bt_inject_linux to report errors.

#include <pcap/bluetooth.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <pcap.h>
#include <cstdio>
#include <fcntl.h>
#include <diag-control.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <bluetooth/hci.h>
#include <bluetooth/bluetooth.h>
#include <pcap-int.h>
#include <string.h>
#include <pcap-bt-linux.h>
#include <config.h>
#include <cstring>

extern "C" int bt_inject_linux(pcap_t *handle, const void *buf, int size); // Focal method prototype (C)

static int g_failures = 0;

// Simple non-terminating logger for test results.
static void log_failure(const char* msg) {
    std::cerr << "FAIL: " << msg << std::endl;
    ++g_failures;
}
static void log_pass(const char* msg) {
    std::cout << "PASS: " << msg << std::endl;
}

// Domain-aligned utilities for clearer test messages.
static const char EXPECTED_ERR[] = "Packet injection is not supported on Bluetooth devices";

// Test 1: Verify that bt_inject_linux returns -1 and sets the exact error message
// in handle->errbuf when attempting to inject a packet on a Bluetooth device.
// This confirms the function's intended behavior and proper interaction with
// the error buffer used by the PCAP framework.
static void test_bt_inject_linux_sets_error_and_returns_minus1() {
    pcap_t handle;
    // Ensure a clean, zeroed handle so errbuf starts empty.
    std::memset(&handle, 0, sizeof(handle));

    unsigned char dummy[1] = {0};
    int ret = bt_inject_linux(&handle, dummy, sizeof(dummy));

    // Non-terminating assertions: report and continue.
    if (ret != -1) {
        log_failure("bt_inject_linux should return -1");
    } else {
        log_pass("bt_inject_linux returned -1 as expected");
    }

    // The function uses snprintf to write to handle->errbuf; verify exact message.
    if (std::strcmp(handle.errbuf, EXPECTED_ERR) != 0) {
        log_failure("bt_inject_linux did not set the exact expected error message in errbuf");
    } else {
        log_pass("bt_inject_linux set the correct error message in errbuf");
    }
}

// Test 2: Ensure that bt_inject_linux overwrites any existing content in errbuf.
// This checks that the error message is consistently updated regardless of prior state.
static void test_bt_inject_linux_overwrites_existing_errbuf() {
    pcap_t handle;
    std::memset(&handle, 0, sizeof(handle));

    // Pre-fill errbuf with a distinct value to simulate prior state.
    const char* prefill = "OLD ERROR MESSAGE";
    std::size_t len = std::min(std::strlen(prefill), (std::size_t)PCAP_ERRBUF_SIZE - 1);
    std::memcpy(handle.errbuf, prefill, len);
    handle.errbuf[len] = '\0';

    unsigned char dummy[1] = {0};
    int ret = bt_inject_linux(&handle, dummy, 0);
    (void)ret; // ret is expected to be -1; not strictly required for this test

    // The errbuf should now contain the new standard message.
    if (std::strcmp(handle.errbuf, EXPECTED_ERR) != 0) {
        log_failure("bt_inject_linux failed to overwrite existing errbuf with the expected message");
    } else {
        log_pass("bt_inject_linux overwrote existing errbuf correctly");
    }
}

// Minimal test harness driver.
int main() {
    std::cout << "Starting bt_inject_linux unit tests (C++11 test harness)" << std::endl;

    test_bt_inject_linux_sets_error_and_returns_minus1();
    test_bt_inject_linux_overwrites_existing_errbuf();

    if (g_failures) {
        std::cout << "Total failures: " << g_failures << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed" << std::endl;
        return 0;
    }
}