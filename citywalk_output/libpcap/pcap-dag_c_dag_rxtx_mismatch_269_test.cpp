/*
Unit test suite for dag_rxtx_mismatch in pcap-dag.c

Overview:
- This test suite targets the focal function:
  dag_rxtx_mismatch(const char *func, pcap_t *p)
- The function formats an error message into p->errbuf and returns PCAP_ERROR.
- We exercise both RX and TX paths by manipulating the internal fields via the
  internal libpcap structures exposed by the project (pcap-int.h, etc.).

Notes:
- The test relies on the internal libpcap structures (pcap_t, pcap_dag, etc.).
  This is typical for unit tests that live inside the project tree with access
  to internal headers.
- This harness uses only C++11 standard library facilities and simple
  non-terminating assertions (i.e., it logs failures but continues).
- We instantiate and tear down the pcap_t structure consistent with the real
  environment so that dag_rxtx_mismatch writes into a real errbuf.
- We test two canonical branches: RX (dag_stream maps to "rx") and TX (maps to "tx").
  The RXTX_STR macro is evaluated inside the compiled C translation unit;
  we rely on the standard DAG enumeration typically used by libpcap/DAG to map
  0 -> "rx" and 1 -> "tx".

Build and run notes:
- Compile this file together with the project sources (including pcap-dag.c and
  its internal headers such as pcap-int.h) in a C++11 capable toolchain.
- Ensure the environment has PCAP_ERRBUF_SIZE defined via libpcap internal headers.
- The test uses the internal pcap structures defined in pcap-int.h.

Code begins here.
*/

#include <string.h>
#include <errno.h>
#include <cstdlib>
#include <endian.h>
#include <dagpci.h>
#include <pcap-util.h>
#include <unistd.h>
#include <pcap-dag.h>
#include <limits.h>
#include <dag_config_api.h>
#include <cstdio>
#include <stdlib.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <iostream>
#include <dagapi.h>


extern "C" {
    // Internal libpcap headers (provided by the project). 
    // These define the exact layout of pcap_t and related structures
    // used by dag_rxtx_mismatch.
    #include "pcap-int.h"

    // Function under test
    int dag_rxtx_mismatch(const char *func, pcap_t *p);
}

// Candidate Keywords for test scaffolding (extracted from the focal method)
//
// - dag_rxtx_mismatch: focal function under test
// - pcap_t: primary object containing device options and private DAG state
// - pcap_dag: private per-device state containing dag_stream
// - dag_stream: enumeration/flag indicating RX/TX stream
// - RXTX_STR: macro converting dag_stream into a human-readable "rx"/"tx" string
// - errbuf: destination buffer in pcap_t used to report errors
// - device: the selected network device name used in the error message
// - PCAP_ERROR: return code of the function
// - PCAP_ERRBUF_SIZE: size of the error buffer

// Small non-terminating assertion framework
static int g_failures = 0;

static void test_log_failure(const char* test_name, const char* condition_desc, const char* details) {
    std::cerr << "[FAIL] " << test_name << ": " << condition_desc;
    if (details && details[0] != '\0') {
        std::cerr << " - " << details;
    }
    std::cerr << std::endl;
    ++g_failures;
}

#define TEST_ASSERT_TRUE(cond, test_name, desc) \
    do { if (!(cond)) test_log_failure(test_name, (desc), nullptr); } while (0)

#define TEST_ASSERT_STR_EQ(expected, actual, test_name) \
    do { if (strcmp((expected), (actual)) != 0) { \
            char buf[512]; std::snprintf(buf, sizeof(buf), "expected '%s' but got '%s'", (expected), (actual)); \
            test_log_failure(test_name, "string mismatch", buf); \
        } } while (0)


// Utility to prepare a pcap_t instance with required fields for dag_rxtx_mismatch
static pcap_t* make_test_pcap(const char* device_name, int dag_stream_value) {
    // Allocate the public pcap_t structure (as defined by internal headers)
    // Note: We rely on the internal layout defined by pcap-int.h
    pcap_t *p = (pcap_t*)calloc(1, sizeof(pcap_t));
    if (!p) return nullptr;

    // Allocate and assign the private DAG-specific state
    p->priv = (struct pcap_dag*)calloc(1, sizeof(struct pcap_dag));
    if (!p->priv) {
        free(p);
        return nullptr;
    }

    // Initialize device name
    // pcap_t.opt is typically a struct with a char* device field.
    // We allocate and copy the device name to keep test independent.
    if (device_name) {
        // Some internal layouts store device as a pointer; handle accordingly
        // Use strdup if available; otherwise allocate compatible memory.
        // We try strdup first; if not available, fall back to malloc+strcpy.
        char* dev = std::strdup(device_name);
        if (dev) {
            p->opt.device = dev;
        } else {
            // Fallback (shouldn't usually happen in normal environments)
            p->opt.device = (char*)std::malloc(std::strlen(device_name) + 1);
            if (p->opt.device) std::strcpy(p->opt.device, device_name);
        }
    }

    // Initialize dag_stream
    // pcap_dag is the type containing dag_stream
    p->priv->dag_stream = dag_stream_value; // 0 for RX, 1 for TX (typical mapping)

    // Ensure errbuf is zeroed out before function writes to it
    if (p->errbuf) {
        std::memset(p->errbuf, 0, PCAP_ERRBUF_SIZE);
    }

    return p;
}

// Cleanup utility
static void destroy_test_pcap(pcap_t* p) {
    if (!p) return;
    if (p->priv) {
        // Free member allocations if present
        if (p->priv->dag_stream) { /* no-op: just to indicate ownership */ }
        free(p->priv);
        p->priv = nullptr;
    }
    // Free device string if allocated
    if (p->opt.device) {
        free(p->opt.device);
        p->opt.device = nullptr;
    }
    free(p);
}

// Test 1: RX path produces "rx" in the error message
static void test_dag_rxtx_mismatch_rx() {
    const char* funcname = "test_func_rx";

    // Create a pcap_t with RX mode (dag_stream = 0)
    pcap_t* p = make_test_pcap("eth0", 0);
    if (!p) {
        test_log_failure("test_dag_rxtx_mismatch_rx", "setup failed", "unable to allocate pcap_t");
        return;
    }

    // Call function under test
    int ret = dag_rxtx_mismatch(funcname, p);

    // Expected: PCAP_ERROR and errbuf formatted accordingly
    const int expected_ret = PCAP_ERROR;
    TEST_ASSERT_TRUE(ret == expected_ret, "test_dag_rxtx_mismatch_rx", "return value is PCAP_ERROR");

    // Build expected string: "<func>: device <device> is <rx|tx>-only"
    char expected[512];
    // RX path should map to "rx"
    std::snprintf(expected, sizeof(expected), "%s: device %s is %s-only",
                  funcname, p->opt.device ? p->opt.device : "<unknown>", "rx");

    TEST_ASSERT_STR_EQ(expected, p->errbuf, "test_dag_rxtx_mismatch_rx: errbuf content");

    destroy_test_pcap(p);
}

// Test 2: TX path produces "tx" in the error message
static void test_dag_rxtx_mismatch_tx() {
    const char* funcname = "test_func_tx";

    // Create a pcap_t with TX mode (dag_stream = 1)
    pcap_t* p = make_test_pcap("eth1", 1);
    if (!p) {
        test_log_failure("test_dag_rxtx_mismatch_tx", "setup failed", "unable to allocate pcap_t");
        return;
    }

    // Call function under test
    int ret = dag_rxtx_mismatch(funcname, p);

    // Expected: PCAP_ERROR and errbuf formatted accordingly
    const int expected_ret = PCAP_ERROR;
    TEST_ASSERT_TRUE(ret == expected_ret, "test_dag_rxtx_mismatch_tx", "return value is PCAP_ERROR");

    // Build expected string: "<func>: device <device> is <rx|tx>-only"
    char expected[512];
    // TX path should map to "tx"
    std::snprintf(expected, sizeof(expected), "%s: device %s is %s-only",
                  funcname, p->opt.device ? p->opt.device : "<unknown>", "tx");

    TEST_ASSERT_STR_EQ(expected, p->errbuf, "test_dag_rxtx_mismatch_tx: errbuf content");

    destroy_test_pcap(p);
}

// Optional: Test 3 - multiple devices to ensure formatting robustness
static void test_dag_rxtx_mismatch_multiple_devices() {
    const char* funcname = "test_func_multi";

    // Device with unusual name
    pcap_t* p = make_test_pcap("lo0", 0); // RX path
    if (!p) {
        test_log_failure("test_dag_rxtx_mismatch_multiple_devices", "setup failed", "unable to allocate pcap_t");
        return;
    }

    int ret = dag_rxtx_mismatch(funcname, p);

    const int expected_ret = PCAP_ERROR;
    TEST_ASSERT_TRUE(ret == expected_ret, "test_dag_rxtx_mismatch_multiple_devices", "return value is PCAP_ERROR");

    char expected[512];
    std::snprintf(expected, sizeof(expected), "%s: device %s is %s-only",
                  funcname, p->opt.device ? p->opt.device : "<unknown>", "rx");

    TEST_ASSERT_STR_EQ(expected, p->errbuf, "test_dag_rxtx_mismatch_multiple_devices: errbuf content");

    destroy_test_pcap(p);
}

int main() {
    // Run test cases
    test_dag_rxtx_mismatch_rx();
    test_dag_rxtx_mismatch_tx();
    test_dag_rxtx_mismatch_multiple_devices();

    // Summary
    if (g_failures == 0) {
        std::cout << "[OK] All tests passed (" << 3 << " tests)." << std::endl;
        return 0;
    } else {
        std::cout << "[SUMMARY] Failures: " << g_failures << " out of 3 tests." << std::endl;
        return 1;
    }
}