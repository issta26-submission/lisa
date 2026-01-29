// C++11 test suite for the focal method pcap_inject_libdlpi (C file: pcap-libdlpi.c)
// This test suite uses a lightweight, self-contained harness (no GTest) and provides
// mock implementations for the DLPI-related functions used by the focal method.
// The tests focus on verifying success and failure branches of the pcap_inject_libdlpi path.

#include <string.h>
#include <errno.h>
#include <dlpisubs.h>
#include <libdlpi.h>
#include <memory.h>
#include <sys/time.h>
#include <cstdlib>
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


// Include the relevant pcap internal definitions to obtain pcap_t and pcap_dlpi types.
// These headers are part of the project under test and are required to match the
// production code layout.

extern "C" {
    // Prototype of the focal function under test.
    int pcap_inject_libdlpi(pcap_t *p, const void *buf, int size);

    // The following DLPI-related symbols are provided by the test harness as mocks.
    // dlpi_send is a system call in the actual project; here we mock its behavior.
    int dlpi_send(void *dlpi_hd, void *ndata, int nbytes, const void *buf, int size, void *info);

    // dlpi_linkname is used by the error path; provide a mock that returns a fixed string.
    const char* dlpi_linkname(void *hd);
}

// --- Test-specific mocks and helpers ---

// Define mock DLPI HD handles to drive success/failure branches in tests.
static void *TEST_DLPI_HDL_GOOD = (void*)0x1234;
static void *TEST_DLPI_HDL_BAD  = (void*)0x5678;

// The mock for dlpi_send: returns DLPI_SUCCESS (assumed 0) for the GOOD handle,
// and a non-success value for other handles to trigger the error path.
int dlpi_send(void *dlpi_hd, void *ndata, int nbytes, const void *buf, int size, void *info) {
    (void)ndata; (void)nbytes; (void)buf; (void)size; (void)info;
    // DLPI_SUCCESS is defined in libdlpi.h; we rely on that value (typically 0).
    extern int DLPI_SUCCESS;
    // If the header "libdlpi.h" is available, DLPI_SUCCESS will be defined.
    // For safety, fall back to 0 if not linked with the header in the test environment.
#ifdef DLPI_SUCCESS
    if (dlpi_hd == TEST_DLPI_HDL_GOOD) {
        return DLPI_SUCCESS;
    } else {
        return -1; // Non-success to exercise the error path
    }
#else
    if (dlpi_hd == TEST_DLPI_HDL_GOOD) {
        return 0;
    } else {
        return -1;
    }
#endif
}

// Mock for dlpi_linkname; returns a fixed synthetic link name.
const char* dlpi_linkname(void *hd) {
    (void)hd;
    return "mock_link";
}

// --- Minimal test harness (no external framework) ---

// Test 1: Successful inject should return the requested size
bool test_inject_libdlpi_success() {
    // Prepare a pcap_t instance with a proper priv pointer to a pcap_dlpi struct.
    pcap_t *p = (pcap_t*)std::malloc(sizeof(pcap_t));
    if (!p) return false;

    pcap_dlpi *pd = (pcap_dlpi*)std::malloc(sizeof(pcap_dlpi));
    if (!pd) {
        std::free(p);
        return false;
    }
    // Attach the mock DLPI handle that signals success
    pd->dlpi_hd = TEST_DLPI_HDL_GOOD;
    p->priv = pd;

    // Prepare a small payload
    const char payload[] = "hello";
    int size = (int)sizeof(payload) - 1;

    int ret = pcap_inject_libdlpi(p, payload, size);

    // Cleanup
    std::free(pd);
    std::free(p);

    // Expect the function to report success by returning the original size
    bool ok = (ret == size);
    return ok;
}

// Test 2: Failure inject should return -1 (simulate dlpi_send error)
bool test_inject_libdlpi_failure() {
    pcap_t *p = (pcap_t*)std::malloc(sizeof(pcap_t));
    if (!p) return false;

    pcap_dlpi *pd = (pcap_dlpi*)std::malloc(sizeof(pcap_dlpi));
    if (!pd) {
        std::free(p);
        return false;
    }
    // Use a DLPI handle that our mock considers as "bad" to trigger error path
    pd->dlpi_hd = TEST_DLPI_HDL_BAD;
    p->priv = pd;

    const char payload[] = "world";
    int size = (int)sizeof(payload) - 1;

    int ret = pcap_inject_libdlpi(p, payload, size);

    // Cleanup
    std::free(pd);
    std::free(p);

    // Expect the function to report failure
    bool ok = (ret == -1);
    return ok;
}

// Test 3: Zero-size inject should return 0 when the DLPI layer reports success
bool test_inject_libdlpi_zero_size() {
    pcap_t *p = (pcap_t*)std::malloc(sizeof(pcap_t));
    if (!p) return false;

    pcap_dlpi *pd = (pcap_dlpi*)std::malloc(sizeof(pcap_dlpi));
    if (!pd) {
        std::free(p);
        return false;
    }
    pd->dlpi_hd = TEST_DLPI_HDL_GOOD;
    p->priv = pd;

    const void *buf = nullptr;
    int size = 0;

    int ret = pcap_inject_libdlpi(p, buf, size);

    // Cleanup
    std::free(pd);
    std::free(p);

    bool ok = (ret == 0);
    return ok;
}

// Helper to run all tests and report results
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool result){
        total++;
        if (result) {
            passed++;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    // Run tests (execute in a single process; static/global state is avoided)
    run("test_inject_libdlpi_success", test_inject_libdlpi_success());
    run("test_inject_libdlpi_failure", test_inject_libdlpi_failure());
    run("test_inject_libdlpi_zero_size", test_inject_libdlpi_zero_size());

    std::cout << "Tests: " << passed << "/" << total << " passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}