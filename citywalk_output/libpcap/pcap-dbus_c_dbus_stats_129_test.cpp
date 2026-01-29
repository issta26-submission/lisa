/*
Unit Test Suite for dbus_stats (pcap-dbus.c)

Notes:
- This test suite targets the focal function:
  int dbus_stats(pcap_t *handle, struct pcap_stat *stats)

- Since the project uses C code, we provide a minimal C-compatible type layout
  in C++ to enable testing without requiring the full external headers.
- The tests do not terminate on failure; they log failures and continue to
  exercise multiple scenarios (non-terminating assertions).
- The test suite is self-contained and relies on linking against the object file
  that implements dbus_stats (e.g., pcap-dbus.o or the corresponding C file).

Usage:
- Ensure the library/object containing dbus_stats is linked when building this test.
- Compile with a C++11 compiler, e.g.:
  g++ -std=c++11 -o test_dbus_stats test_dbus_stats.cpp -lpthread
  (adjust linking as needed for your environment)

*/

#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <dbus/dbus.h>
#include <config.h>
#include <pcap-dbus.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>


// Forward declaration of the focal C function.
// We provide minimal C-compatible type definitions here to compile in C++11.
extern "C" int dbus_stats(pcap_t *handle, struct pcap_stat *stats);

// Minimal C-compatible type definitions expected by dbus_stats implementation.
// These definitions mirror the fields accessed by the focal function.
struct pcap_dbus {
    unsigned int packets_read; // corresponding to handlep->packets_read in dbus_stats
};

struct pcap_t {
    pcap_dbus *priv; // must exist for dbus_stats to access handle->priv
};

// Structure used to return statistics from dbus_stats.
struct pcap_stat {
    unsigned int ps_recv;
    unsigned int ps_drop;
    unsigned int ps_ifdrop;
};

// Simple test harness state
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Logging helpers (non-terminating assertions)
static void log_pass(const char* test_name) {
    std::cout << "[PASS] " << test_name << "\n";
}

static void log_fail(const char* test_name, const char* detail) {
    std::cerr << "[FAIL] " << test_name << ": " << detail << "\n";
    ++g_tests_failed;
}

// High-level assertion helper for unsigned int comparisons
static void expect_uint_eq(unsigned int actual, unsigned int expected, const char* test_name, const char* detail) {
    if (actual != expected) {
        // Detail includes the actual/expected values to aid debugging
        std::cerr << "[ASSERT] " << test_name << ": " << detail
                  << " (actual=" << actual << ", expected=" << expected << ")\n";
        ++g_tests_failed;
    } else {
        // Only log a pass if the assertion is part of a larger test that doesn't
        // redundantly log each assertion. We choose to log per assertion for clarity.
        log_pass(test_name);
    }
}

/*
Test 1: Basic correctness
- Setup: prepare a pcap_t handle with priv->packets_read = a positive value.
- Action: call dbus_stats(handle, &stats).
- Expectations:
  - return value == 0
  - stats.ps_recv equals handle->priv->packets_read
  - stats.ps_drop == 0
  - stats.ps_ifdrop == 0
*/
static void test_case_basic_correctness() {
    const char* test_name = "dbus_stats_basic_correctness";
    ++g_tests_run;

    // Arrange
    pcap_t* handle = new pcap_t;
    handle->priv = new pcap_dbus;
    handle->priv->packets_read = 12345;

    struct pcap_stat stats;
    // Pre-fill to ensure the function overwrites all fields
    stats.ps_recv = 0;
    stats.ps_drop = 1;
    stats.ps_ifdrop = 2;

    // Act
    int ret = dbus_stats(handle, &stats);

    // Assert
    bool ok = true;
    if (ret != 0) {
        log_fail(test_name, "dbus_stats did not return 0");
        ok = false;
    }
    if (stats.ps_recv != 12345) {
        log_fail(test_name, "ps_recv mismatch with packets_read");
        ok = false;
        // do not early return; continue to report all failures
    }
    if (stats.ps_drop != 0) {
        log_fail(test_name, "ps_drop was not reset to 0");
        ok = false;
    }
    if (stats.ps_ifdrop != 0) {
        log_fail(test_name, "ps_ifdrop was not reset to 0");
        ok = false;
    }

    if (ok) {
        log_pass(test_name);
    }

    // Cleanup
    delete handle->priv;
    delete handle;
}

/*
Test 2: Zero packets_read edge case
- Setup: packets_read = 0
- Action: call dbus_stats
- Expectation: ps_recv == 0, other fields still 0
*/
static void test_case_zero_packets() {
    const char* test_name = "dbus_stats_zero_packets";
    ++g_tests_run;

    // Arrange
    pcap_t* handle = new pcap_t;
    handle->priv = new pcap_dbus;
    handle->priv->packets_read = 0;

    struct pcap_stat stats;
    std::memset(&stats, 0, sizeof(stats)); // ensure clean slate

    // Act
    int ret = dbus_stats(handle, &stats);

    // Assert
    bool ok = true;
    if (ret != 0) {
        log_fail(test_name, "dbus_stats did not return 0");
        ok = false;
    }
    if (stats.ps_recv != 0) {
        log_fail(test_name, "ps_recv should be 0 when packets_read is 0");
        ok = false;
    }
    if (stats.ps_drop != 0) {
        log_fail(test_name, "ps_drop should be 0");
        ok = false;
    }
    if (stats.ps_ifdrop != 0) {
        log_fail(test_name, "ps_ifdrop should be 0");
        ok = false;
    }

    if (ok) {
        log_pass(test_name);
    }

    // Cleanup
    delete handle->priv;
    delete handle;
}

/*
Test 3: Large value handling
- Setup: packets_read = 4294967295 (maximum for 32-bit unsigned)
- Action: call dbus_stats
- Expectation: ps_recv equals the large value, other fields 0
*/
static void test_case_large_value() {
    const char* test_name = "dbus_stats_large_value";
    ++g_tests_run;

    // Arrange
    pcap_t* handle = new pcap_t;
    handle->priv = new pcap_dbus;
    handle->priv->packets_read = 0xFFFFFFFF; // 4294967295

    struct pcap_stat stats;
    std::memset(&stats, 0, sizeof(stats));

    // Act
    int ret = dbus_stats(handle, &stats);

    // Assert
    bool ok = true;
    if (ret != 0) {
        log_fail(test_name, "dbus_stats did not return 0");
        ok = false;
    }
    if (stats.ps_recv != 0xFFFFFFFFU) {
        log_fail(test_name, "ps_recv did not reflect maximum value of packets_read");
        ok = false;
    }
    if (stats.ps_drop != 0) {
        log_fail(test_name, "ps_drop should be 0");
        ok = false;
    }
    if (stats.ps_ifdrop != 0) {
        log_fail(test_name, "ps_ifdrop should be 0");
        ok = false;
    }

    if (ok) {
        log_pass(test_name);
    }

    // Cleanup
    delete handle->priv;
    delete handle;
}

int main() {
    std::cout << "Starting dbus_stats test suite (C++11 harness)\n";

    // Run tests
    test_case_basic_correctness();
    test_case_zero_packets();
    test_case_large_value();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";

    // Exit code: non-zero if any test failed
    return g_tests_failed;
}