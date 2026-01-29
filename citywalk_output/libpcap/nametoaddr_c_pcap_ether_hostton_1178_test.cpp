// Test harness for pcap_ether_hostton (focal method) in nametoaddr.c
// This test uses a light-weight, self-contained approach with a mock
// for the dependency: pcap_next_etherent.
// It is written in C++11 but exercises C code via a single translation unit
// by including the focal C file. It uses a minimal test framework with
// non-terminating expectations as requested.

#define PCAP_ETHERS_FILE "pcap_ethers_mock.txt"  // macro used by focal code

#include <gencode.h>
#include <stdio.h>
#include <ethertype.h>
#include <iostream>
#include <errno.h>
#include <nametoaddr.h>
#include <config.h>
#include <thread-local.h>
#include <cstdio>
#include <nametoaddr.c>
#include <llc.h>
#include <pcap/namedb.h>
#include <diag-control.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <os-proto.h>
#include <cstdlib>
#include <cstring>


// Provide a minimal C-style struct matching the real pcap_etherent layout
// used by nametoaddr.c: the code accesses ep->name and ep->addr
struct pcap_etherent {
    const char *name;
    unsigned char addr[6];
};

// Forward declarations of the focal function and its dependency to be mocked
extern "C" unsigned char *pcap_ether_hostton(const char *name);

// Mock of pcap_next_etherent to drive deterministic test data
extern "C" struct pcap_etherent *pcap_next_etherent(FILE *fp);

// Internal static data for the mock: two entries plus a NULL sentinel
static struct pcap_etherent mock_entries[] = {
    { "host1", { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 } },
    { "host2", { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF } },
    { nullptr, { 0 } } // sentinel
};
static int mock_idx = 0;

// Implementation of the mock: returns entries sequentially on each call
extern "C" struct pcap_etherent *pcap_next_etherent(FILE *fp) {
    (void)fp; // unused in mock; focus is on deterministic output
    if (mock_entries[mock_idx].name == nullptr)
        return nullptr;
    return &mock_entries[mock_idx++];
}

// Simple non-terminating test framework (EXPECT_* style)
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++tests_run; \
    if (!(cond)) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
        ++tests_failed; \
    } \
} while (0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    ++tests_run; \
    if ((ptr) == nullptr) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
        ++tests_failed; \
    } \
} while (0)

#define EXPECT_NULL(ptr, msg) do { \
    ++tests_run; \
    if ((ptr) != nullptr) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
        ++tests_failed; \
    } \
} while (0)

#define EXPECT_MEM_EQ(expected, actual, n, msg) do { \
    ++tests_run; \
    if (std::memcmp((expected), (actual), (n)) != 0) { \
        std::cerr << "[TEST FAILED] " << __FILE__ << ":" << __LINE__ << " - " << msg << "\n"; \
        ++tests_failed; \
    } \
} while (0)

// Test 1: Verify that a known host ("host2") yields the expected 6-byte MAC.
void test_pcap_ether_hostton_found() {
    // Prepare a dummy ethers file path (not used by mock, but required by focal code)
    // Create a trivial file so fopen does not fail on platforms requiring it.
    FILE *f = std::fopen(PCAP_ETHERS_FILE, "w");
    if (f) {
        std::fputs("# mock ethers file (not consumed by mock)\n", f);
        std::fclose(f);
    }

    // Call the focal function with a host that exists in the mock
    unsigned char *res = pcap_ether_hostton("host2");

    // Validate result: non-NULL and matches mock addr
    EXPECT_NOT_NULL(res, "pcap_ether_hostton should return non-NULL for existing host 'host2'");
    if (res) {
        const unsigned char expected[6] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
        EXPECT_MEM_EQ(expected, res, 6, "Returned MAC for 'host2' does not match expected value");
        std::free(res);
    }

    // Cleanup the dummy file
    std::remove(PCAP_ETHERS_FILE);
}

// Test 2: Verify that a non-existent host returns NULL.
void test_pcap_ether_hostton_not_found() {
    // Ensure the dummy file exists (to exercise fopen path)
    FILE *f = std::fopen(PCAP_ETHERS_FILE, "w");
    if (f) {
        std::fputs("# mock ethers file (not consumed by mock)\n", f);
        std::fclose(f);
    }

    // Call the focal function with a host that does not exist in the mock
    unsigned char *res = pcap_ether_hostton("nonexistent-host");

    // Validate result: NULL
    EXPECT_NULL(res, "pcap_ether_hostton should return NULL for non-existing host");
    // No need to free as res must be NULL

    // Cleanup the dummy file
    std::remove(PCAP_ETHERS_FILE);
}

// Simple utility to run all tests
int main() {
    // Reset mock state before tests
    mock_idx = 0;

    // Execute tests
    test_pcap_ether_hostton_found();
    test_pcap_ether_hostton_not_found();

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";
    return (tests_failed == 0) ? 0 : 1;
}

// Include the focal C source at the end to ensure that the prototype
// declarations above are visible to the compiler in this single translation unit.
// This approach keeps everything in one compilation unit and allows the C code
// to be compiled together with the C++ test harness without needing separate
// build steps for nametoaddr.c.
// Note: In other environments, linking nametoaddr.c separately is typical.
// Here, we inline for a self-contained test harness as requested.