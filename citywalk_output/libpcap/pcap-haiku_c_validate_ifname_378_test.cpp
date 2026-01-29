// Test suite for the focal method: validate_ifname(const char *device, char *errbuf)
// This test suite is designed for C++11 without Google Test.
// It uses a lightweight, non-terminating assertion framework to maximize code coverage.

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <errno.h>
#include <stdint.h>
#include <OS.h>
#include <net/if_dl.h>
#include <string>
#include <cstring>
#include <net/if_media.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_types.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <string.h>
#include <config.h>
#include <sys/utsname.h>


// If the test is compiled in an environment where PCAP_ERRBUF_SIZE is not defined
// by the included pcap-haiku dependencies, provide a sensible fallback.
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// Extern C declaration of the function under test.
// The actual implementation resides in pcap-haiku.c (part of the project under test).
extern "C" int validate_ifname(const char *device, char *errbuf);

// Lightweight global counters for test results
static int g_total = 0;
static int g_failed = 0;

// Simple non-terminating assertion macros

#define EXPECT_TRUE(cond, desc) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "[EXPECT_TRUE] " << (desc) << " -- FAILED\n"; \
    } \
} while(0)

#define EXPECT_EQ_INT(actual, expected, desc) do { \
    ++g_total; \
    if((actual) != (expected)) { \
        ++g_failed; \
        std::cerr << "[EXPECT_EQ_INT] " << (desc) \
                  << " -- expected: " << (expected) \
                  << ", actual: " << (actual) << "\n"; \
    } \
} while(0)

#define EXPECT_STR_CONTAINS(haystack, needle, desc) do { \
    ++g_total; \
    if(std::string((haystack)).find((needle)) == std::string::npos) { \
        ++g_failed; \
        std::cerr << "[EXPECT_STR_CONTAINS] " << (desc) \
                  << " -- did not find substring: \"" << (needle) \
                  << "\" in: \"" << (haystack) << "\"\n"; \
    } \
} while(0)

#define EXPECT_STR_EQ(actual, expected, desc) do { \
    ++g_total; \
    std::string a = (actual) ? std::string((actual)) : std::string(); \
    std::string e = (expected) ? std::string((expected)) : std::string(); \
    if(a != e) { \
        ++g_failed; \
        std::cerr << "[EXPECT_STR_EQ] " << (desc) \
                  << " -- expected: \"" << e \
                  << "\", actual: \"" << a << "\"\n"; \
    } \
} while(0)

// Helper to run all tests and print a final summary
static void print_summary() {
    std::cout << "\nTest Summary: " << g_total << " ran, "
              << g_failed << " failed.\n";
    if (g_failed > 0) {
        std::cout << "Some tests failed. See details above.\n";
    } else {
        std::cout << "All tests passed.\n";
    }
}

// Test 1: Short/valid interface name should pass (return 0) and not modify errbuf
// Rationale: validate_ifname should return 0 for device names shorter than IF_NAMESIZE.
// The exact IF_NAMESIZE boundary is internal; we verify by ensuring no error is returned for a small string.
static void test_validate_ifname_short_name_passes() {
    // Prepare a buffer for error messages with a safe size
    char errbuf[PCAP_ERRBUF_SIZE];
    // Initialize errbuf to a recognizable value to detect unintended modifications
   (std::memset(errbuf, 'Z', sizeof(errbuf)));

    // Use a minimal valid device name (empty string) to ensure normal path
    const char* device = "";

    int ret = validate_ifname(device, errbuf);

    // Expect success (0) and no modification to errbuf
    EXPECT_EQ_INT(ret, 0, "validate_ifname should return 0 for short/empty device name");

    // errbuf should remain unchanged (still all 'Z')
    EXPECT_TRUE(errbuf[0] == 'Z', "errbuf should be unchanged on success");
}

// Test 2: Detect boundary where too-long detection triggers (dynamic boundary discovery)
// Rationale: The boundary is IF_NAMESIZE. We don't rely on its concrete value; instead we discover it at runtime
// by finding the smallest length that yields an error.
static void test_validate_ifname_boundary_detection_and_message() {
    char errbuf[PCAP_ERRBUF_SIZE];
    // Boundary discovery: find the smallest length where validate_ifname returns a non-zero value
    int boundary = -1;
    for (int len = 0; len <= 512; ++len) {
        std::string s(len, 'A');
        int r = validate_ifname(s.c_str(), errbuf);
        if (r != 0) {
            boundary = len;
            break;
        }
    }

    EXPECT_TRUE(boundary > 0, "Could not detect a boundary where IFNAME becomes too long within tested range");

    if (boundary <= 0) {
        // If boundary not found, skip the rest of this test gracefully
        return;
    }

    // Sub-case 1: One character shorter than boundary should pass (return 0)
    if (boundary > 0) {
        std::string s(boundary - 1, 'A');
        int r = validate_ifname(s.c_str(), errbuf);
        EXPECT_EQ_INT(r, 0, "Length just below boundary should succeed");
    }

    // Sub-case 2: Exactly at boundary should fail (return non-zero)
    {
        std::string s(boundary, 'A');
        int r = validate_ifname(s.c_str(), errbuf);
        EXPECT_TRUE(r != 0, "Length at boundary should fail with PCAP_ERROR");
    }

    // Sub-case 3: Verify error message includes the device string and the phrase "too long"
    {
        std::string longdev(boundary, 'B'); // device string used to trigger error
        // Clear errbuf to inspect message content
        std::memset(errbuf, 0, sizeof(errbuf));
        int r = validate_ifname(longdev.c_str(), errbuf);
        EXPECT_TRUE(r != 0, "Error path should set an error message");
        // Build a std::string from errbuf for flexible checks
        std::string errstr(errbuf);
        EXPECT_STR_CONTAINS(errstr, longdev, "Error message should contain the device name");
        EXPECT_STR_CONTAINS(errstr, "too long", "Error message should mention 'too long'");
    }
}

// Test 3: Empty device name handling combined with non-modification of errbuf
static void test_validate_ifname_empty_name_behavior() {
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf)); // zeroed to detect any writes

    int ret = validate_ifname("", errbuf);
    // Expect success (0) and errbuf should remain at least as initialized
    EXPECT_EQ_INT(ret, 0, "Empty device name should be considered valid (return 0)");
    // If errbuf was not modified, it should remain exactly as initialized (all zeros)
    // Since errbuf is a character array, ensure at least the first byte remains zero
    EXPECT_TRUE(errbuf[0] == 0, "errbuf should remain unchanged (first byte == 0) for success case");
}

int main() {
    // Step-by-step test execution with explanatory comments inline
    // Step 1: Short name path
    test_validate_ifname_short_name_passes();

    // Step 2: Boundary detection path and error message content
    test_validate_ifname_boundary_detection_and_message();

    // Step 3: Empty name behavior
    test_validate_ifname_empty_name_behavior();

    // Print summary
    print_summary();

    // Return non-zero if any test failed, to aid batch scripting
    return (g_failed > 0) ? 1 : 0;
}