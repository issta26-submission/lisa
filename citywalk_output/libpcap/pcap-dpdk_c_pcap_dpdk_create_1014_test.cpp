// C++11 test suite for the focal C function: pcap_dpdk_create
// This test suite is designed to be compiled alongside the project that provides pcap-dpdk.c
// It uses minimal, non-terminating assertions to maximize coverage without invoking a full test framework.

#include <netdb.h>
#include <rte_bus.h>
#include <rte_atomic.h>
#include <unistd.h>
#include <stdio.h>
#include <rte_eal.h>
#include <iostream>
#include <rte_per_lcore.h>
#include <errno.h>
#include <time.h>
#include <rte_ethdev.h>
#include <rte_launch.h>
#include <rte_version.h>
#include <rte_random.h>
#include <rte_lcore.h>
#include <rte_ether.h>
#include <rte_memory.h>
#include <pcap-dpdk.h>
#include <rte_cycles.h>
#include <diag-control.h>
#include <rte_debug.h>
#include <rte_common.h>
#include <rte_malloc.h>
#include <rte_interrupts.h>
#include <limits.h>
#include <rte_mempool.h>
#include <rte_config.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <rte_errno.h>
#include <string.h>
#include <rte_mbuf.h>
#include <rte_branch_prediction.h>
#include <sys/time.h>
#include <config.h>
#include <rte_log.h>
#include <cstring>


// Forward declaration of the C function under test.
// We treat pcap_t as an opaque type here since its internal layout is defined in the C module.
extern "C" {
    struct pcap_t;
    typedef struct pcap_t pcap_t;
    pcap_t *pcap_dpdk_create(const char *device, char *ebuf, int *is_ours);
}

// Simple non-terminating assertion macro to collect failures without aborting tests.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT(cond, msg)                                                        \
    do {                                                                           \
        g_tests_run++;                                                             \
        if (!(cond)) {                                                             \
            std::cerr << "TEST FAILURE: " << (msg)                                \
                      << " [File: " << __FILE__ << ", Line: " << __LINE__ << "]\n";\
            ++g_tests_failed;                                                      \
        } else {                                                                   \
            /* Test passed; optionally print progress uncommented if needed. */  \
        }                                                                          \
    } while (0)


// Test 1: Non-DPDK device should yield NULL and is_ours = 0.
// Rationale: The function sets is_ours to 0 and returns NULL when device does not start with "dpdk:".
void test_pcap_dpdk_create_not_ours()
{
    // Arrange
    char ebuf[256];
    int is_ours = -1;

    // Act
    pcap_t *p = pcap_dpdk_create("eth0", ebuf, &is_ours);

    // Assert
    EXPECT(p == NULL, "pcap_dpdk_create should return NULL for non-DPDK devices");
    EXPECT(is_ours == 0, "is_ours should be 0 when device does not start with 'dpdk:'");
}

// Test 2: DPDK device should yield non-NULL and is_ours = 1.
// Rationale: The function should recognize the prefix and attempt to create a pcap_t via PCAP_CREATE_COMMON,
// setting the activate_op field internally. We verify basic existence and flag without dereferencing internal fields.
void test_pcap_dpdk_create_ours_prefix()
{
    // Arrange
    char ebuf[256];
    int is_ours = 0;

    // Act
    pcap_t *p1 = pcap_dpdk_create("dpdk:0", ebuf, &is_ours);
    pcap_t *p2 = pcap_dpdk_create("dpdk:abc", ebuf, &is_ours);

    // Assert
    EXPECT(p1 != NULL, "pcap_dpdk_create should return non-NULL for dpdk: prefix");
    EXPECT(is_ours == 1, "is_ours should be 1 after dpdk: prefix");

    // The internal structure is provided by the C module; we cannot inspect fields here without
    // including internal headers. We at least verify another dpdk: input behaves similarly.
    EXPECT(p2 != NULL, "pcap_dpdk_create should return non-NULL for dpdk:abc prefix");
    EXPECT(is_ours == 1, "is_ours should remain 1 after second dpdk:abc prefix");
}


int main()
{
    // Run tests
    test_pcap_dpdk_create_not_ours();
    test_pcap_dpdk_create_ours_prefix();

    // Summary
    if (g_tests_failed == 0) {
        std::cout << "ALL TESTS PASSED (" << g_tests_run << " tests executed)." << std::endl;
        return 0;
    } else {
        std::cerr << g_tests_failed << " TEST(S) FAILED out of " << g_tests_run << " ("
                  << g_tests_run - g_tests_failed << " passed)." << std::endl;
        return 1;
    }
}