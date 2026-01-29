// Unit tests for the focal function: get_if_flags from pcap-libdlpi.c
// This test suite is designed for C++11 and does not rely on Google Test.
// It uses a small homegrown test harness with non-terminating style checks.
// The tests exercise the behavior when the PCAP_IF_LOOPBACK flag is set and when it is not.

#include <string.h>
#include <errno.h>
#include <dlpisubs.h>
#include <libdlpi.h>
#include <memory.h>
#include <sys/time.h>
#include <cstdint>
#include <pcap/pcap.h>
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


// Try to use libpcap's flag definitions if available. These constants are used
// by the production code to identify loopback interfaces and to mark the
// "not applicable" status for connection state.
// If the environment has libpcap installed, these will be defined by including
// the proper headers. We include the header here to get the actual values.

// The production function uses bpf_u_int32 as its flags type. Define a safe uint32_t
// based alias to avoid depending on the exact typedefs in the headers.
typedef uint32_t bpf_u_int32;

// Forward declaration of the focal function with C linkage to match the production code.
// The real implementation is in pcap-libdlpi.c.
extern "C" int get_if_flags(const char *name, bpf_u_int32 *flags, char *errbuf);

// Simple test harness helpers
static void log_pass(const char* test_name) {
    std::cout << "[PASS] " << test_name << std::endl;
}
static void log_fail(const char* test_name, const char* detail) {
    std::cerr << "[FAIL] " << test_name << " - " << detail << std::endl;
}

// Test 1: When PCAP_IF_LOOPBACK is set in flags, the function should mark the
// interface as NOT_APPLICABLE for the connection status and return 0.
bool test_get_if_flags_loopback_sets_not_applicable() {
    const char* test_name = "test_get_if_flags_loopback_sets_not_applicable";
    bpf_u_int32 flags = PCAP_IF_LOOPBACK; // Loopback interface flag set
    char errbuf[256] = {0};

    int ret = get_if_flags("lo0", &flags, errbuf);

    // Expect success return value
    if (ret != 0) {
        log_fail(test_name, "return value is not 0");
        return false;
    }

    // Expect NOT_APPLICABLE flag to be set
    if ((flags & PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE) == 0) {
        log_fail(test_name, "PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE not set in flags");
        return false;
    }

    log_pass(test_name);
    return true;
}

// Test 2: When PCAP_IF_LOOPBACK is not set, the function should not modify flags
// (i.e., there should be no change to the flags in this code path) and should return 0.
bool test_get_if_flags_no_loopback_no_change() {
    const char* test_name = "test_get_if_flags_no_loopback_no_change";
    bpf_u_int32 flags = 0; // No loopback flag
    char errbuf[256] = {0};

    int ret = get_if_flags("eth0", &flags, errbuf);

    // Expect success return value
    if (ret != 0) {
        log_fail(test_name, "return value is not 0");
        return false;
    }

    // Expect no modifications to flags
    if (flags != 0) {
        log_fail(test_name, "flags were unexpectedly modified");
        return false;
    }

    log_pass(test_name);
    return true;
}

// Test 3: When multiple bits are set including PCAP_IF_LOOPBACK, the function should
// still mark NOT_APPLICABLE while preserving other bits (where applicable).
bool test_get_if_flags_loopback_with_other_bits() {
    const char* test_name = "test_get_if_flags_loopback_with_other_bits";
    // Include an extra arbitrary bit (0x1000) in addition to LOOPBACK
    bpf_u_int32 flags = PCAP_IF_LOOPBACK | 0x1000;
    char errbuf[256] = {0};

    int ret = get_if_flags("lo1", &flags, errbuf);

    if (ret != 0) {
        log_fail(test_name, "return value is not 0");
        return false;
    }

    if ((flags & PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE) == 0) {
        log_fail(test_name, "PCAP_IF_CONNECTION_STATUS_NOT_APPLICABLE not set in flags with extra bits");
        return false;
    }

    // Ensure the extra bit is still present (we expect only that the loopback case
    // marks the NOT_APPLICABLE; other bits, if preserved by the API, should remain).
    if ((flags & 0x1000) == 0) {
        log_fail(test_name, "extra bit (0x1000) was cleared unexpectedly");
        return false;
    }

    log_pass(test_name);
    return true;
}

// Main driver: run all tests and report summary
int main() {
    int total = 0;
    int passed = 0;

    // Run tests
    if (test_get_if_flags_loopback_sets_not_applicable()) ++passed;
    ++total;

    if (test_get_if_flags_no_loopback_no_change()) ++passed;
    ++total;

    if (test_get_if_flags_loopback_with_other_bits()) ++passed;
    ++total;

    std::cout << "Test suite: " << passed << " / " << total << " tests passed." << std::endl;

    return (passed == total) ? 0 : 1;
}