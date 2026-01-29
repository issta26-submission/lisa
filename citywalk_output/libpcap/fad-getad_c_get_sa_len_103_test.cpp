// C++11 test suite for the C function get_sa_len from fad-getad.c
// This test harness uses non-terminating assertions (test functions return bool)
// and reports a final summary in main() instead of using a test framework like GTest.

#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <os-proto.h>
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <config.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <iostream>


// Declare the C function from fad-getad.c with C linkage
extern "C" int get_sa_len(struct sockaddr *addr);


// AF_PACKET specific header is Linux only (for sockaddr_ll)
#ifdef __linux__
#endif

// Lightweight non-terminating assertion helper
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "TEST FAIL: " << (msg) << "\n"; \
        return false; \
    } \
} while (0)

// Test 1: AF_INET branch should return sizeof(struct sockaddr_in)
bool test_sa_len_inet4() {
    struct sockaddr addr;
    addr.sa_family = AF_INET;

    int len = get_sa_len(&addr);
    size_t expected = sizeof(struct sockaddr_in);

    TEST_ASSERT(len == (int)expected, "AF_INET should return sizeof(struct sockaddr_in)");
    return true;
}

// Test 2: AF_INET6 branch should return sizeof(struct sockaddr_in6)
bool test_sa_len_inet6() {
    struct sockaddr addr;
    addr.sa_family = AF_INET6;

    int len = get_sa_len(&addr);
    size_t expected = sizeof(struct sockaddr_in6);

    TEST_ASSERT(len == (int)expected, "AF_INET6 should return sizeof(struct sockaddr_in6)");
    return true;
}

// Test 3: AF_PACKET branch (Linux) should return sizeof(struct sockaddr_ll)
// This branch is compiled only when AF_PACKET is defined in the environment.
bool test_sa_len_packet() {
#ifdef AF_PACKET
    struct sockaddr addr;
    addr.sa_family = AF_PACKET;

    int len = get_sa_len(&addr);
    size_t expected = sizeof(struct sockaddr_ll);

    TEST_ASSERT(len == (int)expected, "AF_PACKET should return sizeof(struct sockaddr_ll)");
    return true;
#else
    // If AF_PACKET is not available, skip this test gracefully.
    std::cout << "SKIP: AF_PACKET not defined in this environment.\n";
    return true;
#endif
}

// Test 4: Default/unknown family should fall back to sizeof(struct sockaddr)
bool test_sa_len_default() {
    struct sockaddr addr;
    // Use a value outside known families to exercise the default case
    addr.sa_family = 0xFFFF;

    int len = get_sa_len(&addr);
    size_t expected = sizeof(struct sockaddr);

    TEST_ASSERT(len == (int)expected, "Unknown sa_family should return sizeof(struct sockaddr)");
    return true;
}

// Test 5: AF_LINK branch (only if AF_LINK is defined in the environment)
// If AF_LINK is not defined (common on Linux), this test is skipped.
bool test_sa_len_af_link() {
#ifdef AF_LINK
    struct sockaddr addr;
    addr.sa_family = AF_LINK;

    int len = get_sa_len(&addr);
    // The size depends on the platform's sockaddr_dl struct
    // We check for non-zero and equal to the expected type size.
    #ifdef __APPLE__
    size_t expected = sizeof(struct sockaddr_dl);
    #else
    // On some systems AF_LINK may map differently; we still expect the size of the corresponding link-layer sockaddr
    size_t expected = sizeof(struct sockaddr_dl);
    #endif

    TEST_ASSERT(len == (int)expected, "AF_LINK should return sizeof(struct sockaddr_dl)");
    return true;
#else
    std::cout << "SKIP: AF_LINK not defined in this environment.\n";
    return true;
#endif
}

// Entry point: run all tests and print a final summary.
// Uses non-terminating assertions via bool returns to allow all tests to execute.
int main() {
    int total = 0;
    int failed = 0;

    auto run = [&](bool (*test_func)(), const char* name) -> void {
        ++total;
        bool ok = test_func();
        if (!ok) {
            ++failed;
        }
        // Non-terminating: continue running remaining tests
    };

    std::cout << "Starting get_sa_len test suite...\n";

    run(test_sa_len_inet4, "test_sa_len_inet4");
    run(test_sa_len_inet6, "test_sa_len_inet6");
    run(test_sa_len_packet, "test_sa_len_packet");
    run(test_sa_len_default, "test_sa_len_default");
    run(test_sa_len_af_link, "test_sa_len_af_link");

    std::cout << "Test results: " << (total - failed) << " passed, " << failed << " failed, out of " << total << " tests.\n";

    // Return non-zero if any test failed to aid integration with build scripts
    return failed ? 1 : 0;
}