// Lightweight C++11 unit test suite for the Haiku pcap_stats_haiku function.
// This test harness is designed to be compiled alongside the pcap-haiku.c
// implementation (and its headers) in a typical project layout.
// It does not use GTest; it uses a minimal in-process test runner with
// non-terminating EXPECT_* style assertions to maximize code coverage.
//
// Important: This test file relies on the real pcap-haiku.c environment.
// It attempts to override ioctl_ifreq behavior via a test-scoped hook.
// Depending on the build configuration, the override mechanism may require
// adjustment (e.g., weak symbols). The code below provides a plausible
// approach and accompanying tests. If your build system hides or privatizes
// ioctl_ifreq in Haiku's C file, you may need to adapt the hook strategy.

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <OS.h>
#include <cstdint>
#include <net/if_dl.h>
#include <string>
#include <cstring>
#include <net/if_media.h>
#include <vector>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_types.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <string.h>
#include <config.h>
#include <sys/utsname.h>


// Minimal, test-oriented redefinitions. In a real environment, include the
// project headers (pcap.h, pcap-int.h, etc.) instead of redefining here.
// The goal is to mirror only what pcap_stats_haiku uses from the types.

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

#ifndef PCAP_ERROR
#define PCAP_ERROR (-1)
#endif

// Forward declarations to link with the production code.
// The actual production function is implemented in pcap-haiku.c.
extern "C" int pcap_stats_haiku(struct pcap_t* handle, struct pcap_stat* stats);

// Minimal mirrors of the production structs sufficient for test harness.
// These definitions are crafted to align with the expectations inside
// pcap_stats_haiku's implementation.
//
// Note: In a real test, include the project headers to ensure exact layout.
// We keep the layout intentionally minimal and focused on the fields used.

struct pcap_stat {
    unsigned int ps_recv;
    unsigned int ps_drop;
    unsigned int ps_ifdrop;
};

struct if_stats {
    unsigned int dropped;
};

struct ifr_stats {
    struct {
        struct {
            unsigned int dropped;
        } receive;
    } ifr_stats;
};

struct ifreq {
    ifr_stats ifr_stats;
};

// The Haiku-specific pcap_t wrapper used by pcap_stats_haiku.
struct pcap_haiku {
    struct pcap_stat stat;
    int aux_socket;
    struct ifreq ifreq;
    char errbuf[PCAP_ERRBUF_SIZE];
};

struct pcap_t {
    void* priv;
};

// ----------------------------------------------------------------------------
// Test hook for ioctl_ifreq.
// The production code calls ioctl_ifreq(..., &handlep->ifreq, ...).
// We provide a test-controlled override to simulate both success and failure
// paths without requiring actual OS/network activity.
// This uses a simple test-mode switch that the unit tests can control.
//
// Important: This hook is a best-effort for unit testing. Depending on your
// build system, you may need to link differently or use weak symbols.
// If the hook does not override the production symbol, you can still apply
// tests by exercising the failure path via an invalid aux_socket value.
// ----------------------------------------------------------------------------

// Test-mode globals
static int g_ioctl_test_mode = 0;      // 0 = normal (simulated by default), 1 = set drop value, 2 = fail
static unsigned int g_ioctrl_sim_dropped = 0;

// Prototype for the production ioctl_ifreq; we declare here to allow
// a compatible weak/strong symbol override in test builds.
extern "C" int ioctl_ifreq(int fd,
                          const unsigned long op,
                          const char* name,
                          struct ifreq* ifreq,
                          char* errbuf);

// Test override: provide a hook with the same signature.
// Depending on the linker, this may override the production symbol if the
// test object is linked with the production object that does not hide the symbol.
// If your build system enforces a strict symbol separation, switch to the
// invalid-aux-socket approach in Test 2 (see below).
extern "C" int ioctl_ifreq(int fd,
                          const unsigned long op,
                          const char* name,
                          struct ifreq* ifreq,
                          char* errbuf)
{
    // Test path: simulate a successful ioctl and fill received.dropped
    if (g_ioctl_test_mode == 2) {
        // Simulate ioctl failure
        return -1;
    }

    if (g_ioctl_test_mode == 1) {
        // Simulate wiring in a dropped counter value
        ifreq->ifr_stats.ifr_stats.receive.dropped = g_ioctrl_sim_dropped;
    } else {
        // Default: simulate some drop value if not explicitly set
        ifreq->ifr_stats.ifr_stats.receive.dropped = g_ioctrl_sim_dropped;
    }

    // In the real world, the function would also configure the error buffer.
    // We keep errbuf untouched when simulating success.
    (void)fd; (void)op; (void)name; (void)errbuf;
    return 0;
}

// ----------------------------------------------------------------------------
// Minimal non-terminating test framework
// ----------------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Simple EXPECT macros that do not abort on failure.
#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_tests_run; \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ << " - " << (msg) << "\n"; \
            ++g_tests_failed; \
        } else { \
            ++g_tests_passed; \
        } \
    } while (false)

#define EXPECT_EQ(a, b, msg) \
    do { \
        ++g_tests_run; \
        if (!((a) == (b))) { \
            std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                      << " - " << (msg) \
                      << " | " << (a) << " != " << (b) << "\n"; \
            ++g_tests_failed; \
        } else { \
            ++g_tests_passed; \
        } \
    } while (false)

#define EXPECT_NEQ(a, b, msg) \
    do { \
        ++g_tests_run; \
        if ((a) == (b)) { \
            std::cerr << "EXPECT_NEQ failed at " << __FILE__ << ":" << __LINE__ \
                      << " - " << (msg) \
                      << " | " << (a) << " == " << (b) << "\n"; \
            ++g_tests_failed; \
        } else { \
            ++g_tests_passed; \
        } \
    } while (false)

// Helpers to create a fake handle and initialize the Haiku-specific data
static pcap_t* create_fake_handle_for_test() {
    pcap_t* h = new pcap_t();
    pcap_haiku* hnd = new pcap_haiku();
    // Initialize a sane default state
    hnd->stat.ps_recv = 0;
    hnd->stat.ps_drop = 0;
    hnd->stat.ps_ifdrop = 0;
    hnd->aux_socket = 0; // will be overridden by tests if needed
    std::memset(&hnd->ifreq, 0, sizeof(hnd->ifreq));
    std::strncpy(hnd->errbuf, "", PCAP_ERRBUF_SIZE);
    h->priv = static_cast<void*>(hnd);
    return h;
}

static void destroy_fake_handle(pcap_t* h) {
    if (h) {
        delete static_cast<pcap_haiku*>(h->priv);
        delete h;
    }
}

// Test 1: Success path
// - Copy of handle->stat to *stats
// - ioctl_ifreq returns 0 and fills handlep->ifreq.receive.dropped
// - stats->ps_ifdrop becomes original ps_ifdrop subtracted from the new value
// - Returns 0
static void test_pcap_stats_haiku_success_path() {
    // Setup: prepare handle and stats
    pcap_t* handle = create_fake_handle_for_test();
    pcap_haiku* handlep = static_cast<pcap_haiku*>(handle->priv);

    // Initialize known state
    handlep->stat.ps_recv = 1234;
    handlep->stat.ps_drop = 42;
    handlep->stat.ps_ifdrop = 5;

    // Simulated current ifdrop value after activation
    g_ioctrl_sim_dropped = 7;

    // Configure test hook to return success and fill ifr_stats
    g_ioctl_test_mode = 1; // set to fill value
    // Ensure handle's aux_socket is a valid non-negative value
    handlep->aux_socket = 1;

    struct pcap_stat stats;
    // Zero out stats to confirm that pcap_stats_haiku writes into it
    std::memset(&stats, 0, sizeof(stats));

    int result = pcap_stats_haiku(handle, &stats);

    // Assertions
    EXPECT_EQ(result, 0, "pcap_stats_haiku should return 0 on success");
    EXPECT_EQ(stats.ps_recv, 1234, "ps_recv should be copied from handle->stat");
    EXPECT_EQ(stats.ps_drop, 42,  "ps_drop should be copied from handle->stat");
    // ps_ifdrop should be the difference: 7 - 5 = 2
    EXPECT_EQ(stats.ps_ifdrop, 2u, "ps_ifdrop should be updated to the difference");
    destroy_fake_handle(handle);
}

// Test 2: ioctl failure path
// - ioctl_ifreq returns -1; pcap_stats_haiku should return PCAP_ERROR
static void test_pcap_stats_haiku_ioctl_failure_path() {
    pcap_t* handle = create_fake_handle_for_test();
    pcap_haiku* handlep = static_cast<pcap_haiku*>(handle->priv);

    // Setup known state
    handlep->stat.ps_recv = 111;
    handlep->stat.ps_drop = 2;
    handlep->stat.ps_ifdrop = 1;

    // Force ioctl to fail
    g_ioctl_test_mode = 2; // simulate failure
    handlep->aux_socket = 1;

    struct pcap_stat stats;
    std::memset(&stats, 0, sizeof(stats));

    int result = pcap_stats_haiku(handle, &stats);

    // Assertions
    EXPECT_EQ(result, PCAP_ERROR, "pcap_stats_haiku should return PCAP_ERROR on ioctl failure");
    // In this failure path, *stats has been assigned before the ioctl attempt.
    // It should reflect the original handle->stat values.
    EXPECT_EQ(stats.ps_recv, 111u, "ps_recv should still reflect handle->stat on error");
    EXPECT_EQ(stats.ps_drop, 2u,   "ps_drop should still reflect handle->stat on error");
    // ps_ifdrop remains as in original handle->stat
    EXPECT_EQ(stats.ps_ifdrop, 1u, "ps_ifdrop should remain unchanged on error");

    destroy_fake_handle(handle);
}

// Test 3: ps_ifdrop non-negative difference edge case
// - Ensure that when dropped counter increases, the difference is computed correctly
static void test_pcap_stats_haiku_ps_ifdrop_difference() {
    pcap_t* handle = create_fake_handle_for_test();
    pcap_haiku* handlep = static_cast<pcap_haiku*>(handle->priv);

    // Setup initial state
    handlep->stat.ps_recv = 500;
    handlep->stat.ps_drop = 0;
    handlep->stat.ps_ifdrop = 3;

    // Simulate larger incoming dropped count
    g_ioctrl_sim_dropped = 10;

    // Simulate success
    g_ioctl_test_mode = 1;
    handlep->aux_socket = 1;

    struct pcap_stat stats;
    std::memset(&stats, 0, sizeof(stats));

    int result = pcap_stats_haiku(handle, &stats);

    // Assertions
    EXPECT_EQ(result, 0, "pcap_stats_haiku should succeed");
    EXPECT_EQ(stats.ps_recv, 500u, "ps_recv copied");
    EXPECT_EQ(stats.ps_drop, 0u,   "ps_drop copied");
    // 10 - 3 = 7
    EXPECT_EQ(stats.ps_ifdrop, 7u, "ps_ifdrop should reflect the difference 10 - 3");

    destroy_fake_handle(handle);
}

// Entry point for tests
int main() {
    std::cout << "pcap_stats_haiku unit tests (Haiku backend) - starting\n";

    // Run tests
    test_pcap_stats_haiku_success_path();
    test_pcap_stats_haiku_ioctl_failure_path();
    test_pcap_stats_haiku_ps_ifdrop_difference();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed << "\n";

    // Return non-zero if any test failed
    return (g_tests_failed != 0) ? 1 : 0;
}