// This test suite targets the focal function:
// int pcap_nametoport(const char *name, int *port, int *proto)
// The function is defined in nametoaddr.c and is part of a larger PCAP-related project.
// The tests are written in C++11 without GoogleTest, using simple non-terminating assertions
// so that all tests execute even if some fail (as requested by the domain guidance).

// Step 1: Program Understanding (captured in comments within the code below)
// - The function resolves a service name (or numeric port) into a TCP/UDP port.
// - It first queries TCP (getaddrinfo with SOCK_STREAM, IPPROTO_TCP) and extracts a port.
// - Then it queries UDP (SOCK_DGRAM, IPPROTO_UDP) for a UDP port.
// - If a TCP port is found, it returns 1 and sets port/proto to TCP, unless UDP port exists with the same
//   value, in which case proto is set to PROTO_UNDEF to indicate ambiguity.
// - If no TCP port is found but a UDP port is found, it returns 1 with UDP fields.
// - If neither is found, it returns 0.
// - There are edge cases handling EAI_NONAME and EAI_SERVICE; real errors return 0 early.
// - The code relies on standard system name/service databases (e.g., /etc/services on UNIX-like systems).

// Step 2: Unit Test Generation (testable scenarios chosen below)
 // Candidate Keywords derived from the focal method logic:
 // - getaddrinfo, TCP (IPPROTO_TCP), UDP (IPPROTO_UDP), AF_INET/AF_INET6
 // - Service names: http (80, TCP), domain (53, TCP/UDP ambiguity), numeric ports like "8080"
 // - Ambiguity: when both TCP and UDP share same port (PROTO_UNDEF)
 // - Error paths: non-existent service name (returns 0)
 // - Port extraction: from IPv4/IPv6 sockaddr_in/sin6_port
 // - Return semantics: 1 on success, 0 on failure

// Step 3: Test Case Refinement (domain knowledge and robustness)
// - We'll test true branches: known service "http" (port 80, TCP)
// - Test ambiguity case: "domain" (port 53 on TCP and UDP exist on many systems) -> proto may be PROTO_UNDEF
// - Test false branch: non-existent service returns 0
// - Test numeric port path: "8080" should yield port 8080 and TCP (typical behavior)

#include <netinet/in.h>
#include <gencode.h>
#include <winsock2.h>
#include <stdio.h>
#include <ethertype.h>
#include <iostream>
#include <errno.h>
#include <nametoaddr.h>
#include <config.h>
#include <thread-local.h>
#include <cstdio>
#include <llc.h>
#include <pcap/namedb.h>
#include <diag-control.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <ws2tcpip.h>
#include <string.h>
#include <os-proto.h>
#include <cstdlib>
#include <cstring>


#if defined(_WIN32) || defined(_WIN64)
#else
#endif

extern "C" int pcap_nametoport(const char *name, int *port, int *proto);

// Lightweight non-terminating assertion helpers
static int g_failures = 0;

// Simple non-terminating assertion for integers
static void assert_int_equal(const char* label, int expected, int actual) {
    if (actual != expected) {
        std::cerr << "ASSERT_FAIL: " << label
                  << " expected=" << expected
                  << " actual=" << actual << std::endl;
        ++g_failures;
    }
}

// Simple non-terminating assertion for strings
static void assert_str_equal(const char* label, const char* expected, const char* actual) {
    if ((expected == nullptr && actual != nullptr) ||
        (expected != nullptr && actual == nullptr)) {
        std::cerr << "ASSERT_FAIL: " << label
                  << " expected=" << (expected ? expected : "nullptr")
                  << " actual=" << (actual ? actual : "nullptr") << std::endl;
        ++g_failures;
        return;
    }
    if (expected && actual && strcmp(expected, actual) != 0) {
        std::cerr << "ASSERT_FAIL: " << label
                  << " expected=" << expected
                  << " actual=" << actual << std::endl;
        ++g_failures;
    }
}

// Test 1: Known service "http" should resolve to TCP port 80
static void test_http_service_tcp() {
    int port = -1;
    int proto = -1;
    int ret = pcap_nametoport("http", &port, &proto);

    // Debug logging for traceability
    std::cout << "[TEST] test_http_service_tcp: ret=" << ret
              << " port=" << port << " proto=" << proto << std::endl;

    // Expect success
    assert_int_equal("http_serv_tcp:return", 1, ret);
    // Port should be 80 for HTTP over TCP in typical systems
    assert_int_equal("http_serv_tcp:port", 80, port);
    // Protocol should indicate TCP
    // We compare to IPPROTO_TCP if available; otherwise allow common fallback of -1.
#if defined(IPPROTO_TCP)
    assert_int_equal("http_serv_tcp:proto", IPPROTO_TCP, proto);
#else
    // If IPPROTO_TCP macro isn't available in the test environment, skip strict check
    (void)proto;
#endif
}

// Test 2: Ambiguity case using "domain" (DNS uses port 53 for UDP and TCP on many systems)
static void test_domain_ambiguous_ports() {
    int port = -1;
    int proto = -1;
    int ret = pcap_nametoport("domain", &port, &proto);

    std::cout << "[TEST] test_domain_ambiguous_ports: ret=" << ret
              << " port=" << port << " proto=" << proto << std::endl;

    // Expect success on systems where domain maps to port 53 for TCP/UDP
    assert_int_equal("domain_serv:port", 53, port);
    // Proto may be TCP, UDP, or PROTO_UNDEF depending on system and whether
    // both TCP/UDP entries exist and share the same port.
    if (proto != 0) { // proto should be a non-zero value if defined; allow -1 or 6/17 as valid
        // Acceptable possibilities: IPPROTO_TCP (6), IPPROTO_UDP (17), or PROTO_UNDEF (often -1)
#if defined(IPPROTO_TCP) && defined(IPPROTO_UDP)
        bool ok = (proto == IPPROTO_TCP) || (proto == IPPROTO_UDP) || (proto == -1);
        if (!ok) {
            std::cerr << "ASSERT_FAIL: domain_serv:proto unexpected value: " << proto << std::endl;
            ++g_failures;
        }
#else
        // If protocol constants are not available, just ensure proto is non-negative
        if (proto < 0) {
            std::cerr << "ASSERT_FAIL: domain_serv:proto negative value without PTOS\n";
            ++g_failures;
        }
#endif
    }
}

// Test 3: Non-existent service should fail (return 0)
static void test_nonexistent_service() {
    int port = -1;
    int proto = -1;
    int ret = pcap_nametoport("this_service_should_not_exist_abcdef_xyz", &port, &proto);

    std::cout << "[TEST] test_nonexistent_service: ret=" << ret
              << " port=" << port << " proto=" << proto << std::endl;

    // Expect failure
    assert_int_equal("nonexistent:return", 0, ret);
    // port/proto should remain unchanged (not strictly guaranteed by implementation; we just ensure no false positives)
}

// Test 4: Numeric port string like "8080" should yield that port via TCP resolution
static void test_numeric_port_string() {
    int port = -1;
    int proto = -1;
    int ret = pcap_nametoport("8080", &port, &proto);

    std::cout << "[TEST] test_numeric_port_string: ret=" << ret
              << " port=" << port << " proto=" << proto << std::endl;

    // Expect success; port should be 8080, typically using TCP
    assert_int_equal("numeric:port", 8080, port);
#if defined(IPPROTO_TCP)
    assert_int_equal("numeric:proto", IPPROTO_TCP, proto);
#else
    // If not available, ensure proto is non-negative
    if (proto < 0) {
        std::cerr << "ASSERT_FAIL: numeric:proto negative value without PTOS\n";
        ++g_failures;
    }
#endif
}

// Main test runner
int main() {
    // Initialize Windows sockets if needed (no-op on POSIX)
#if defined(_WIN32) || defined(_WIN64)
    WSADATA wsa;
    int wsa_init = WSAStartup(MAKEWORD(2,2), &wsa);
    if (wsa_init != 0) {
        std::cerr << "WSAStartup failed: " << wsa_init << std::endl;
        return 1;
    }
#endif

    std::cout << "Starting pcap_nametoport test suite (C++11, non-GTest)..." << std::endl;

    test_http_service_tcp();
    test_domain_ambiguous_ports();
    test_nonexistent_service();
    test_numeric_port_string();

    std::cout << "Test suite finished. Failures: " << g_failures << std::endl;

#if defined(_WIN32) || defined(_WIN64)
    WSACleanup();
#endif

    // Return non-zero if any test failed
    return (g_failures == 0) ? 0 : 1;
}