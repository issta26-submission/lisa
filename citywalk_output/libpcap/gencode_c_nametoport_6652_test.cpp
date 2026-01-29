/*
Unit test suite for nametoport (focal method) in gencode.c
- No Google Test usage; use a lightweight, self-contained test harness.
- Focus on positive path coverage for the typical use-case:
  - nametoport with service name "http" and TCP should return port 80
  - nametoport with service name "domain" and UDP should return port 53
- Tests are designed to be executable under a C++11 environment.
- We avoid depending on internal structures (e.g., compiler_state_t) by using a
  minimal external prototype and passing a nullptr for the state where safe
  (the success path does not use the cstate).
- The tests use only standard C/C++ facilities and the provided function signature.
*/

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>


// Declare the focal C function with C linkage to match the actual implementation.
// We use a minimal signature to avoid requiring full type definitions from the
// original C headers in the test environment.
extern "C" int nametoport(void *cstate, const char *name, int ipproto);

// Lightweight test harness helpers
static int tests_run = 0;
static int tests_failed = 0;

#define RUN_TEST(test_func, description) do { \
    ++tests_run; \
    try { \
        if (test_func()) { \
            std::cout << "[PASS] " description << std::endl; \
        } else { \
            std::cout << "[FAIL] " description << std::endl; \
            ++tests_failed; \
        } \
    } catch (...) { \
        std::cout << "[FAIL] " description << " (exception)" << std::endl; \
        ++tests_failed; \
    } \
} while (0)

/*
Test 1: Positive path for HTTP over TCP
- Purpose: Validate that nametoport resolves the service name "http" to the standard
  port 80 when ipproto corresponds to TCP.
- Rationale: This covers the success branch where getaddrinfo returns a list with
  AF_INET/AF_INET6 entries and a valid port number in sin_port.
- Assumptions: System provides the standard services entry "http" = 80.
*/
static bool test_nametoport_http_tcp() {
    void *cstate = nullptr;           // Safe to pass null for success path
    const char *name = "http";
    int ipproto = 6;                   // IPPROTO_TCP (numeric) to avoid OS headers
    int port = nametoport(cstate, name, ipproto);
    // Accept exact 80 as a robust expectation for the HTTP service in TCP
    if (port == 80) {
        return true;
    } else {
        std::cerr << "Expected port 80 for http/tcp, got " << port << std::endl;
        return false;
    }
}

/*
Test 2: Positive path for DOMAIN over UDP
- Purpose: Validate that nametoport resolves the service name "domain" to the standard
  port 53 when ipproto corresponds to UDP.
- Rationale: Covers success path for UDP service name resolution.
- Assumptions: System provides the standard services entry "domain" = 53.
*/
static bool test_nametoport_domain_udp() {
    void *cstate = nullptr;
    const char *name = "domain";
    int ipproto = 17;                   // IPPROTO_UDP (numeric)
    int port = nametoport(cstate, name, ipproto);
    if (port == 53) {
        return true;
    } else {
        std::cerr << "Expected port 53 for domain/udp, got " << port << std::endl;
        return false;
    }
}

/*
Test runner (main)
- Executes all tests and reports a final exit status.
- Follows the directive to call test methods from main when not using a test framework.
*/
int main() {
    RUN_TEST(test_nametoport_http_tcp, "nametoport should return 80 for 'http' over TCP");
    RUN_TEST(test_nametoport_domain_udp, "nametoport should return 53 for 'domain' over UDP");

    std::cout << "Tests run: " << tests_run
              << ", Passed: " << (tests_run - tests_failed)
              << ", Failed: " << tests_failed << std::endl;

    return (tests_failed == 0) ? 0 : 1;
}