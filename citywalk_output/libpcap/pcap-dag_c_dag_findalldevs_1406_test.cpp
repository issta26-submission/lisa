// dag_findalldevs_test.cpp
// A lightweight, self-contained C++11 unit-test harness for the focal function
// dag_findalldevs in pcap-dag.c. This test uses hand-written stubs to mock
// the DAG subsystem and related pcap helpers so that tests can run in a
// non-hardware environment without GMock/GTest. It exercises true/false
// branches of key predicates and ensures test coverage suitable for CI.
//
// Important: This harness is designed to be compiled and linked together with
// the actual pcap-dag.c implementation in the project. It provides minimal
// C-linkage stubs for the DAG API expected by dag_findalldevs and a tiny
// test framework in place of Google Test.
//
// Notes on test strategy:
// - We only need to exercise the high-level logic of dag_findalldevs. The
//   stubs simulate the hardware/driver behavior and allow us to verify
//   device- enumeration paths.
// - We trigger multiple scenarios by switching global knobs (scenarios) to
//   cover true/false branches of condition predicates.
// - We avoid private/private-like members; focus is on public API surface used by
//   dag_findalldevs.
// - We implement a tiny assertion framework and report a non-zero exit code on
//   failures, suitable for simple CI usage.
//
// How to run (conceptual):
// - Compile this test file together with pcap-dag.c in the project (the project
//   should have its own Makefile/CMake rules). No GTest required.
// - Ensure the DAG APIs are satisfied by the small test stubs provided here.
// - Run the test binary; it will print per-test results and a final summary.

// Compile-time/platform notes:
// - We avoid including any real DAG library headers. We provide minimal C linkage
//   stubs to satisfy dag_findalldevs while controlling behavior in tests.

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <config.h>
#include <pcap-util.h>
#include <pcap-dag.h>
#include <cstdio>
#include <dag_config_api.h>
#include <string>
#include <dagapi.h>
#include <vector>
#include <endian.h>
#include <dagpci.h>
#include <limits.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <cstdlib>
#include <cstring>


// Forward declare the type used by dag_findalldevs for the device list.
// We only pass NULL or opaque pointers from tests; no actual content is required.
struct pcap_if_list_t;

// Fwd declare the focal function to be tested (from pcap-dag.c)
extern "C" int dag_findalldevs(struct pcap_if_list_t *devlistp, char *errbuf);

// ---------------------------------------------------------------------------
// Minimal types and macros to model the types used by dag_findalldevs stubs
// ---------------------------------------------------------------------------

typedef unsigned int bpf_u_int32;
typedef long long dag_ssize_t;
typedef unsigned long long dag_size_t;

// Basic in-file DAG card info stub
typedef struct dag_card_inf_t {
    int device_code;
} dag_card_inf_t;

// VDAG device id const (value is not important for tests; we just compare)
#define PCI_DEVICE_ID_VDAG 0x1234

// Internal behavior control for stubs (will influence the test scenarios)
enum OpenScenario {
    OPEN_OK,           // dag_open succeeds for /dev/dag0
    OPEN_EACCES,        // dag_open fails with EACCES for /dev/dag0
    OPEN_FAIL           // dag_open fails with some error for /dev/dag0
};

// Global knobs controlled by tests to drive behavior
static OpenScenario g_open_scenario = OPEN_OK;
static int g_rx_streams = 2;                 // number of RX streams returned by dag_rx_get_stream_count
static int g_tx_streams = 0;                 // TX streams if ENABLE_DAG_TX is defined (not used in tests)
static int g_bufsize_stream0 = 1024;          // buffer size for stream 0
static int g_bufsize_stream1 = 1024;          // buffer size for stream 1
static int g_current_device_code = (int)PCI_DEVICE_ID_VDAG; // device_code from dag_pciinfo
static bool g_add_dev_fail = false;           // whether pcapint_add_dev should fail (to test failclose)

// A small container to verify devices added by dag_findalldevs
static std::vector<std::string> g_added_devices;

// ---------------------------------------------------------------------------
// Stubs for the DAG/pcap internal interfaces used by dag_findalldevs
// These mock implementations run in place of real hardware/library calls.
// All functions use C linkage to match the actual binary's symbol resolution.
// ---------------------------------------------------------------------------

extern "C" {

struct pcap_if_list_t {}; // opaque; tests only pass NULL or opaque pointers

// dag_open: simulate opening DAG device files.
// - OPEN_OK: succeed for /dev/dag0 (return a non-negative fd, here 0)
// - OPEN_EACCES: simulate permission denied
// - OPEN_FAIL: simulate general failure
int dag_open(const char *dagname) {
    if (dagname == nullptr) return -1;
    if (strcmp(dagname, "/dev/dag0") != 0) return -1;
    switch (g_open_scenario) {
        case OPEN_OK:
            return 0; // non-negative fd
        case OPEN_EACCES:
            errno = EACCES;
            return -1;
        case OPEN_FAIL:
        default:
            errno = ENOENT;
            return -1;
    }
}

// dag_pciinfo: return a pointer to a fake inf struct with a device_code
const dag_card_inf_t* dag_pciinfo(int dagfd) {
    static dag_card_inf_t fake_inf;
    fake_inf.device_code = g_current_device_code;
    return &fake_inf;
}

// dag_rx_get_stream_count: simulate number of RX streams for the opened dag
int dag_rx_get_stream_count(int dagfd) {
    (void)dagfd; // unused in test stub
    // If OPEN_OK, return configured RX streams; otherwise return error
    if (g_open_scenario == OPEN_OK) {
        return g_rx_streams;
    }
    // In error scenarios, simulate error via negative return
    return -1;
}

// Optional TX path (not used in this test harness by default)
// Provide a default implementation if ENABLE_DAG_TX is defined in the build.
// We keep a simple return so tests can still compile even if TX paths exist.
#ifdef ENABLE_DAG_TX
int dag_tx_get_stream_count(int dagfd) {
    (void)dagfd;
    return g_tx_streams;
}
#endif

// dag_get_stream_buffer_size64: return per-stream buffer sizes; -1 means non-existent
dag_ssize_t dag_get_stream_buffer_size64(int dagfd, int stream) {
    (void)dagfd;
    if (stream == 0) return g_bufsize_stream0;
    if (stream == 1) return g_bufsize_stream1;
    // beyond known streams
    return -1;
}

// dag_close: no-op in test
void dag_close(int dagfd) {
    (void)dagfd;
}

// Device description strings (simple placeholders for test validation)
const char* dag_device_description(const unsigned dagid) {
    (void)dagid;
    return "Test DAG device";
}
const char* dag_stream_long_description(const unsigned stream, const dag_size_t bufsize, const dag_card_inf_t *inf) {
    (void)stream; (void)bufsize; (void)inf;
    return "Test long stream description";
}
const char* dag_stream_short_description(const unsigned stream) {
    (void)stream;
    return "Test short stream";
}

// pcapint_add_dev: capture added devices in a global vector for assertions.
// Return non-NULL on success, NULL on failure to simulate errors.
void* pcapint_add_dev(struct pcap_if_list_t *devlistp,
                     const char *name,
                     const bpf_u_int32 flags,
                     const char *description,
                     char *errbuf)
{
    (void)devlistp;
    (void)flags;
    (void)description;
    (void)errbuf;
    if (g_add_dev_fail) {
        return NULL;
    }
    // Record device name for later assertions
    if (name != nullptr) {
        g_added_devices.push_back(std::string(name));
    } else {
        g_added_devices.push_back(std::string("<null>"));
    }
    return reinterpret_cast<void*>(0x1); // non-NULL
}

// Optional helper to emulate error message formatting when needed.
// We provide a simple colorless stub; dag_findalldevs uses only the side effect
// of writing into errbuf.
int pcapint_fmt_errmsg_for_errno(char *errbuf, int errbuf_size, int errno_value, const char *func_name) {
    (void)errbuf; (void)errbuf_size; (void)errno_value; (void)func_name;
    // Minimal stub: just return success, fill nothing meaningful
    if (errbuf && errbuf_size > 0) {
        snprintf(errbuf, errbuf_size, "errno=%d in %s", errno_value, func_name ? func_name : "unknown");
    }
    return 0;
}

} // extern "C"

// ---------------------------------------------------------------------------
// Tiny, framework-free unit-test harness
// ---------------------------------------------------------------------------

static int g_failures = 0;

// Simple assertion macro for clarity
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERTION FAILED: " << (msg) << " [Line " << __LINE__ << "]\n"; \
        ++g_failures; \
    } \
} while (0)

#define TEST_REPORT() do { \
    if (g_failures == 0) { \
        std::cout << "[TEST PASSED] All tests succeeded.\n"; \
    } else { \
        std::cout << "[TEST FAILED] " << g_failures << " assertion(s) failed.\n"; \
    } \
} while (0)

// Helper to reset test state between tests
static void reset_test_state() {
    g_added_devices.clear();
    g_open_scenario = OPEN_OK;
    g_rx_streams = 2;
    g_bufsize_stream0 = 1024;
    g_bufsize_stream1 = 1024;
    g_current_device_code = (int)PCI_DEVICE_ID_VDAG;
    g_add_dev_fail = false;
}

// ---------------------------------------------------------------------------
// Unit tests for dag_findalldevs
// Each test is documented with its intent and branches covered.
// ---------------------------------------------------------------------------

// Test 1: Primary path - open succeeds for /dev/dag0, multiple streams exist and
//          all are usable (bufsize > 0) and device code is not VDAG (forces
//          creation of shorthand and per-stream devices).
// Expected: dag_findalldevs returns 0 and adds at least 3 devices
//           ("dag0", "dag0:0", "dag0:1") to the devlist.
static void test_primary_path_enumeration() {
    reset_test_state();
    // Configure behavior to trigger the main branch
    g_open_scenario = OPEN_OK;
    g_rx_streams = 2;              // two streams available
    g_bufsize_stream0 = 1024;      // > 0 to allow shorthand and stream1
    g_bufsize_stream1 = 2048;      // > 0 for stream1
    g_current_device_code = 0;       // not VDAG, so condition is true for MD/streams
    g_add_dev_fail = false;

    char errbuf[256];
    int rc = dag_findalldevs(nullptr, errbuf);

    // Assertions
    ASSERT_TRUE(rc == 0, "dag_findalldevs should return 0 on success in primary path");
    // Expect at least three devices added: "dag0" (shorthand), "dag0:0", "dag0:1"
    bool found_all = (g_added_devices.end() - g_added_devices.begin()) >= 3;
    ASSERT_TRUE(found_all, "Expected at least 3 devices to be added in primary path");
    // Ensure names present in the set
    bool has_shorthand = false, has_stream0 = false, has_stream1 = false;
    for (const auto &d : g_added_devices) {
        if (d == "dag0") has_shorthand = true;
        if (d == "dag0:0") has_stream0 = true;
        if (d == "dag0:1") has_stream1 = true;
    }
    ASSERT_TRUE(has_shorthand, "Shorthand device 'dag0' should be added");
    ASSERT_TRUE(has_stream0, "Stream device 'dag0:0' should be added");
    ASSERT_TRUE(has_stream1, "Stream device 'dag0:1' should be added");
}

// Test 2: No usable streams scenario - when RX/ TX streams count declares zero,
//          dag_findalldevs should complete with success but no devices added.
// Expected: rc == 0 and no devices added.
static void test_no_usable_streams_results_in_no_devices() {
    reset_test_state();
    // Configure to yield zero streams
    g_open_scenario = OPEN_OK;
    g_rx_streams = 0;              // no RX streams
    g_bufsize_stream0 = -1;         // not used
    g_bufsize_stream1 = -1;
    g_current_device_code = 0;
    g_add_dev_fail = false;

    char errbuf[256];
    int rc = dag_findalldevs(nullptr, errbuf);

    // Assertions
    ASSERT_TRUE(rc == 0, "dag_findalldevs should return 0 even if no devices are found");
    ASSERT_TRUE(g_added_devices.empty(), "No devices should be added when there are no streams");
}

// Test 3: Error path when RX stream count retrieval fails (negative return).
//          This validates that a non-zero return (PCAP_ERROR) propagates.
// Expected: rc != 0 (error).
static void test_rx_count_failure_propagates_error() {
    reset_test_state();
    // Configure to simulate a failure in rx stream count
    g_open_scenario = OPEN_OK;
    g_rx_streams = -1;             // signals error in dag_rx_get_stream_count
    g_bufsize_stream0 = 1024;
    g_bufsize_stream1 = 1024;
    g_current_device_code = 0;
    g_add_dev_fail = false;

    char errbuf[256];
    int rc = dag_findalldevs(nullptr, errbuf);

    // Assertions
    ASSERT_TRUE(rc != 0, "dag_findalldevs should return a non-zero value on rx count failure");
    // We do not rely on device additions in this path; ensure no crash
    (void)rc;
}

// Test runner
int main() {
    std::cout << "Starting test suite for dag_findalldevs (pcap-dag.c) - lightweight harness\n";

    test_primary_path_enumeration();
    test_no_usable_streams_results_in_no_devices();
    test_rx_count_failure_propagates_error();

    TEST_REPORT();
    return (g_failures == 0) ? 0 : 1;
}