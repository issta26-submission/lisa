/*
  Unit test suite for pcap_stats_hurd in pcap-hurd.c

  Notes:
  - This test targets the function:
      int pcap_stats_hurd(pcap_t *p, struct pcap_stat *ps)
    which copies the internal ph->stat into *ps and returns 0.
  - We rely on the internal types defined by the project (pcap_t, struct pcap_hurd, struct pcap_stat).
  - The tests are designed to be non-terminating: they report failures but continue executing
    to maximize coverage.
  - This file is intended to be compiled as C++11, without a testing framework (no GTest).
  - We use the actual library types via pcap-int.h (as included by the project) to ensure
    compatibility with the focal function's expectations.
*/

#include <string.h>
#include <errno.h>
#include <device/net_status.h>
#include <net/if_ether.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <hurd.h>
#include <device/device.h>
#include <stdlib.h>
#include <device/device_types.h>
#include <config.h>
#include <pcap-int.h>
#include <mach.h>
#include <fcntl.h>


// Include internal PCAP types so that pcap_stats_hurd can be called correctly.
// The production build provides these headers; we rely on them for type correctness.
extern "C" {
int pcap_stats_hurd(pcap_t *p, struct pcap_stat *ps);
}

// Global counters for the lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to compare two pcap_stat structures by content (field-wise)
static bool stat_equal(const struct pcap_stat& a, const struct pcap_stat& b)
{
    return (a.ps_recv == b.ps_recv) &&
           (a.ps_drop == b.ps_drop) &&
           (a.ps_ifdrops == b.ps_ifdrops);
}

// Utility for logging failures without terminating tests
static void log_failure(const char* test_name, const char* message)
{
    std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
    g_failed_tests++;
}

// Test 1: Basic copying of a non-zero stat set from ph.stat to ps
// Scenario: ph.stat contains typical non-zero counters; ensure pcap_stats_hurd copies exactly.
static void test_pcap_stats_hurd_basic_copy()
{
    const char* test_name = "test_pcap_stats_hurd_basic_copy";

    // Set up the underlying pcap_hurd state
    struct pcap_hurd ph;
    ph.stat.ps_recv    = 100;
    ph.stat.ps_drop    = 2;
    ph.stat.ps_ifdrops = 1;

    // Set up pcap_t to point to ph
    struct pcap_t p;
    p.priv = &ph;

    // Destination to be filled by the function
    struct pcap_stat ps;

    // Execute
    int rv = pcap_stats_hurd(&p, &ps);

    // Validate
    g_total_tests++;
    if (rv != 0) {
        log_failure(test_name, "pcap_stats_hurd returned non-zero status");
        return;
    }
    if (!stat_equal(ps, ph.stat)) {
        log_failure(test_name, "Copied statistics do not match expected ph.stat values");
        std::cerr << "  Expected: { ps_recv=" << ph.stat.ps_recv
                  << ", ps_drop=" << ph.stat.ps_drop
                  << ", ps_ifdrops=" << ph.stat.ps_ifdrops << " }\n";
        std::cerr << "  Actual:   { ps_recv=" << ps.ps_recv
                  << ", ps_drop=" << ps.ps_drop
                  << ", ps_ifdrops=" << ps.ps_ifdrops << " }\n";
    }
}

// Test 2: Copying zeroed statistics
// Scenario: ph.stat is all zeros; ensure the function copies zeros correctly.
static void test_pcap_stats_hurd_zero_values()
{
    const char* test_name = "test_pcap_stats_hurd_zero_values";

    struct pcap_hurd ph;
    ph.stat.ps_recv    = 0;
    ph.stat.ps_drop    = 0;
    ph.stat.ps_ifdrops = 0;

    struct pcap_t p;
    p.priv = &ph;

    struct pcap_stat ps;

    int rv = pcap_stats_hurd(&p, &ps);

    g_total_tests++;
    if (rv != 0) {
        log_failure(test_name, "pcap_stats_hurd returned non-zero status on zero values");
        return;
    }
    if (!stat_equal(ps, ph.stat)) {
        log_failure(test_name, "Copied statistics do not match zero values");
        std::cerr << "  Expected: { ps_recv=0, ps_drop=0, ps_ifdrops=0 }\n";
        std::cerr << "  Actual:   { ps_recv=" << ps.ps_recv
                  << ", ps_drop=" << ps.ps_drop
                  << ", ps_ifdrops=" << ps.ps_ifdrops << " }\n";
    }
}

// Test 3: Copying large values to exercise unsigned range handling
// Scenario: ph.stat fields are set to maximum unsigned int values.
static void test_pcap_stats_hurd_large_values()
{
    const char* test_name = "test_pcap_stats_hurd_large_values";

    struct pcap_hurd ph;
    ph.stat.ps_recv    = 0xFFFFFFFFu;
    ph.stat.ps_drop    = 0xFFFFFFFFu;
    ph.stat.ps_ifdrops = 0xFFFFFFFFu;

    struct pcap_t p;
    p.priv = &ph;

    struct pcap_stat ps;

    int rv = pcap_stats_hurd(&p, &ps);

    g_total_tests++;
    if (rv != 0) {
        log_failure(test_name, "pcap_stats_hurd returned non-zero status for large values");
        return;
    }
    if (!stat_equal(ps, ph.stat)) {
        log_failure(test_name, "Copied statistics do not match large value scenario");
        std::cerr << "  Expected: { ps_recv=" << ph.stat.ps_recv
                  << ", ps_drop=" << ph.stat.ps_drop
                  << ", ps_ifdrops=" << ph.stat.ps_ifdrops << " }\n";
        std::cerr << "  Actual:   { ps_recv=" << ps.ps_recv
                  << ", ps_drop=" << ps.ps_drop
                  << ", ps_ifdrops=" << ps.ps_ifdrops << " }\n";
    }
}

// Test 4: Mixed values to ensure field-wise copying integrity
static void test_pcap_stats_hurd_mixed_values()
{
    const char* test_name = "test_pcap_stats_hurd_mixed_values";

    struct pcap_hurd ph;
    ph.stat.ps_recv    = 7;
    ph.stat.ps_drop    = 1024;
    ph.stat.ps_ifdrops = 65535;

    struct pcap_t p;
    p.priv = &ph;

    struct pcap_stat ps;

    int rv = pcap_stats_hurd(&p, &ps);

    g_total_tests++;
    if (rv != 0) {
        log_failure(test_name, "pcap_stats_hurd returned non-zero status on mixed values");
        return;
    }
    if (!stat_equal(ps, ph.stat)) {
        log_failure(test_name, "Copied statistics do not match mixed values");
        std::cerr << "  Expected: { ps_recv=" << ph.stat.ps_recv
                  << ", ps_drop=" << ph.stat.ps_drop
                  << ", ps_ifdrops=" << ph.stat.ps_ifdrops << " }\n";
        std::cerr << "  Actual:   { ps_recv=" << ps.ps_recv
                  << ", ps_drop=" << ps.ps_drop
                  << ", ps_ifdrops=" << ps.ps_ifdrops << " }\n";
    }
}

// Entry point: run all tests and print a concise summary.
// Uses non-terminating assertions: test continues even if one fails.
int main()
{
    // Run all test cases
    test_pcap_stats_hurd_basic_copy();
    test_pcap_stats_hurd_zero_values();
    test_pcap_stats_hurd_large_values();
    test_pcap_stats_hurd_mixed_values();

    // Summary
    std::cout << std::dec;
    std::cout << "\nTest Summary: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures.\n";

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}