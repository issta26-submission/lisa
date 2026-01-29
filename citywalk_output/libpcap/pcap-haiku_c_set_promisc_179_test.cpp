// Lightweight unit test suite for the focal method set_promisc
// This test is designed to run in a C++11 environment without Google Test.
// It focuses on validating the bitwise manipulation of IFF_PROMISC within
// the pcap_haiku private structure, exercising true/false branches of enable.
// NOTE: This test uses a minimal surface area of the real structures and relies
// on the side-effect of set_promisc mutating handlep->ifreq.ifr_flags prior to
// the ioctl call. Return values of ioctl are not asserted to avoid OS-specific behavior.

#include <string.h>
#include <net/if_media.h>
#include <errno.h>
#include <config.h>
#include <sys/socket.h>
#include <net/if.h>
#include <stdint.h>
#include <net/if_types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <OS.h>
#include <net/if_dl.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <iostream>
#include <cstring>


// Forward declaration of the focal function (C linkage)
extern "C" // Ensure C linkage for the library function
int set_promisc(struct pcap_t* handle, const int enable);

// Minimal stub definitions to enable compilation of test code
// These definitions are intentionally lightweight and only provide the
// fields accessed by set_promisc through the priv pointer.
struct pcap_t {
    void* priv; // Expected to point to a pcap_haiku instance in the real code
};

// The test assumes the following layout for the Haiku private structure.
// If the real project uses a different layout, this test should be adapted
// to reflect the actual fields/order. The important pieces are:
// - an ifreq structure named 'ifreq' with field 'ifr_flags'
// - an integer 'aux_socket' used for ioctl
// - an error buffer 'errbuf' used by ioctl_ifreq
struct pcap_haiku {
    int aux_socket;
    struct ifreq ifreq;
    char errbuf[256];
};

// Lightweight test harness
static int g_total = 0;
static int g_failed = 0;

// Helper to report a test result
static void report_test(const std::string& name, bool passed, const std::string& detail = "") {
    ++g_total;
    if (passed) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        ++g_failed;
        std::cout << "[FAIL] " << name;
        if (!detail.empty()) {
            std::cout << " - " << detail;
        }
        std::cout << "\n";
    }
}

// Test 1: enable = true should set IFF_PROMISC bit and preserve other bits
static void test_set_promisc_enable_sets_flag() {
    // Arrange
    struct pcap_t testHandle;
    struct pcap_haiku haiku;

    haiku.aux_socket = -1; // Invalid fd to avoid real IO on wrap; we only care about flag mutation
    // Zero initialize to ensure a clean starting point
    std::memset(&haiku.ifreq, 0, sizeof(haiku.ifreq));
    // errbuf must be large enough; zero it
    std::memset(haiku.errbuf, 0, sizeof(haiku.errbuf));

    testHandle.priv = &haiku;

    // Seed some non-flag bits to verify preservation
    haiku.ifreq.ifr_flags = 0xA5; // Arbitrary bits other than IFF_PROMISC

    // Act
    int ret = set_promisc(&testHandle, 1); // enable PROMISC

    // Assert
    unsigned int expected = 0xA5 | IFF_PROMISC;
    bool okFlags = (haiku.ifreq.ifr_flags == (int)expected);

    if (!okFlags) {
        report_test("test_set_promisc_enable_sets_flag", false,
                    "Flags mismatch after enabling PROMISC. Expected " +
                    std::to_string(expected) + ", got " +
                    std::to_string(haiku.ifreq.ifr_flags) +
                    ". ioctl return: " + std::to_string(ret));
        return;
    }
    report_test("test_set_promisc_enable_sets_flag", true);
}

// Test 2: enable = false should clear IFF_PROMISC bit and preserve other bits
static void test_set_promisc_disable_clears_flag() {
    // Arrange
    struct pcap_t testHandle;
    struct pcap_haiku haiku;

    haiku.aux_socket = -1;
    std::memset(&haiku.ifreq, 0, sizeof(haiku.ifreq));
    std::memset(haiku.errbuf, 0, sizeof(haiku.errbuf));

    testHandle.priv = &haiku;

    // Start with PROMISC enabled to ensure the clear path has effect
    haiku.ifreq.ifr_flags = IFF_PROMISC | 0x22; // Some extra bits present

    // Act
    int ret = set_promisc(&testHandle, 0); // disable PROMISC

    // Assert
    unsigned int expected = (0x22); // IFF_PROMISC cleared, other bits preserved
    bool okFlags = ((haiku.ifreq.ifr_flags & IFF_PROMISC) == 0) && ((haiku.ifreq.ifr_flags & 0x22) != 0);

    if (!okFlags) {
        report_test("test_set_promisc_disable_clears_flag", false,
                    "PROMISC not cleared correctly or other bits lost. "
                    "Flags now: " + std::to_string(haiku.ifreq.ifr_flags) +
                    ", ioctl return: " + std::to_string(ret));
        return;
    }

    // A stricter check on exact expected value (given initial 0x22 present)
    if ((haiku.ifreq.ifr_flags & ~IFF_PROMISC) != 0x22) {
        report_test("test_set_promisc_disable_clears_flag", false,
                    "Unexpected mutation of non-PROMISC bits. Got: " +
                    std::to_string(haiku.ifreq.ifr_flags) +
                    ", expected: " + std::to_string(0x22));
        return;
    }

    report_test("test_set_promisc_disable_clears_flag", true);
}

// Test 3: Idempotence: enabling when PROMISC already set should keep the bit set
static void test_set_promisc_enable_idempotent() {
    // Arrange
    struct pcap_t testHandle;
    struct pcap_haiku haiku;

    haiku.aux_socket = -1;
    std::memset(&haiku.ifreq, 0, sizeof(haiku.ifreq));
    haiku.errbuf[0] = '\0';
    testHandle.priv = &haiku;

    haiku.ifreq.ifr_flags = IFF_PROMISC; // PROMISC already set

    // Act
    int ret = set_promisc(&testHandle, 1);

    // Assert
    if ((haiku.ifreq.ifr_flags & IFF_PROMISC) == 0) {
        report_test("test_set_promisc_enable_idempotent", false,
                    "PROMISC was not preserved on enabling again. "
                    "Flags: " + std::to_string(haiku.ifreq.ifr_flags) +
                    ", return: " + std::to_string(ret));
        return;
    }

    report_test("test_set_promisc_enable_idempotent", true);
}

// Entry point
int main() {
    std::cout << "Starting unit tests for set_promisc (Haiku PCM) - C++11 harness\n";

    test_set_promisc_enable_sets_flag();
    test_set_promisc_disable_clears_flag();
    test_set_promisc_enable_idempotent();

    std::cout << "Tests completed. Passed: "
              << (g_total - g_failed) << ", Failed: " << g_failed
              << ", Total: " << g_total << "\n";

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}