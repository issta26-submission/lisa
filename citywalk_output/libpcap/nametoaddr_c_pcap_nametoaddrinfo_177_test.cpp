// Test suite for pcap_nametoaddrinfo
// Purpose: Validate behavior of the focal method using a lightweight,
// non-terminating assertion framework without GTest.
// The tests are designed for a POSIX-like environment (Linux/macOS).
// The focal function bridges to getaddrinfo using specific hints and returns
// a linked list of struct addrinfo on success or NULL on error.
//
// Note: The test suite assumes that nametoaddr.c (containing pcap_nametoaddrinfo)
// is compiled and linked with this test binary.

#include <netinet/in.h>
#include <netdb.h>
#include <gencode.h>
#include <stdio.h>
#include <ethertype.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <nametoaddr.h>
#include <config.h>
#include <thread-local.h>
#include <llc.h>
#include <pcap/namedb.h>
#include <diag-control.h>
#include <vector>
#include <sys/socket.h>
#include <functional>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pcap-int.h>
#include <string.h>
#include <os-proto.h>
#include <cstdlib>
#include <cstring>


// Link against the C implementation that provides pcap_nametoaddrinfo
#ifdef __cplusplus
extern "C" {
#endif
struct addrinfo;
struct addrinfo* pcap_nametoaddrinfo(const char *name);
#ifdef __cplusplus
}
#endif

// Lightweight non-terminating test assertion helpers.
// They increment total/failed counters but never abort execution.
static int g_tests_total = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, desc) do { \
    ++g_tests_total; \
    if (!(cond)) { \
        std::cerr << "  [FAIL] " << (desc) << "\n"; \
        ++g_tests_failed; \
    } else { \
        std::cout << "  [OK]   " << (desc) << "\n"; \
    } \
} while (0)

#define EXPECT_NOT_NULL(ptr, desc) do { \
    ++g_tests_total; \
    if ((ptr) == nullptr) { \
        std::cerr << "  [FAIL] " << (desc) << " (null pointer)\n"; \
        ++g_tests_failed; \
    } else { \
        std::cout << "  [OK]   " << (desc) << "\n"; \
    } \
} while (0)

#define EXPECT_EQ_INT(a, b, desc) do { \
    ++g_tests_total; \
    if ((a) != (b)) { \
        std::cerr << "  [FAIL] " << (desc) << " (expected " << (b) \
                  << ", got " << (a) << ")\n"; \
        ++g_tests_failed; \
    } else { \
        std::cout << "  [OK]   " << (desc) << "\n"; \
    } \
} while (0)

#define SAFE_FREEADDRINFO(res) do { if ((res) != nullptr) { freeaddrinfo((res)); (res) = nullptr; } } while (0)

// Helper to print a single address in numeric form (IPv4/IPv6) if desired.
// Not strictly required for correctness, but useful for debugging.
static void print_num_addr(struct addrinfo *ai) {
    if (ai == nullptr || ai->ai_addr == nullptr) return;
    char host[NI_MAXHOST];
    host[0] = '\0';
    if (getnameinfo(ai->ai_addr, ai->ai_addrlen, host, sizeof(host),
                    NULL, 0, NI_NUMERICHOST) == 0) {
        std::cout << "    address: " << host << "\n";
    } else {
        std::cout << "    address: <unresolved>\n";
    }
}

// Test 1: Valid host should return a non-NULL addrinfo list.
// We expect the function to succeed for a well-known hostname such as "localhost".
static void test_pcap_nametoaddrinfo_valid_hostname() {
    const char *name = "localhost";
    std::cout << "Test 1: pcap_nametoaddrinfo with valid hostname \"" << name << "\"\n";

    struct addrinfo *res = pcap_nametoaddrinfo(name);
    EXPECT_NOT_NULL(res, "pcap_nametoaddrinfo should not return NULL for a valid hostname");

    if (res != nullptr) {
        // Validate that we received a plausible ai_family (IPv4 or IPv6)
        int fam = res->ai_family;
        bool ok_family = (fam == AF_INET) || (fam == AF_INET6);
        EXPECT_TRUE(ok_family, "addrinfo ai_family should be AF_INET or AF_INET6");
        // Optional: print the first resolved address for debugging
        print_num_addr(res);
        SAFE_FREEADDRINFO(res);
    }
}

// Test 2: Invalid hostname should return NULL.
// Ensure the function gracefully indicates failure without crashing.
static void test_pcap_nametoaddrinfo_invalid_hostname() {
    const char *name = "thishostname_should_not_exist_xyz123";
    std::cout << "Test 2: pcap_nametoaddrinfo with invalid hostname \"" << name << "\"\n";

    struct addrinfo *res = pcap_nametoaddrinfo(name);
    EXPECT_TRUE(res == NULL, "pcap_nametoaddrinfo should return NULL for an invalid hostname");
    // No need to free as per contract; res is NULL.
}

// Test 3: Empty string should be treated as invalid input and return NULL.
static void test_pcap_nametoaddrinfo_empty_string() {
    const char *name = "";
    std::cout << "Test 3: pcap_nametoaddrinfo with empty string\n";

    struct addrinfo *res = pcap_nametoaddrinfo(name);
    EXPECT_TRUE(res == NULL, "pcap_nametoaddrinfo should return NULL for empty input");
}

// Test 4: Repeated valid lookups should consistently return a non-NULL list and be safe to free.
// This helps assert stability across multiple calls in a single process.
static void test_pcap_nametoaddrinfo_repeated_valid_lookups() {
    const char *name = "localhost";
    std::cout << "Test 4: pcap_nametoaddrinfo repeated valid lookups for \"" << name << "\"\n";

    for (int i = 0; i < 3; ++i) {
        struct addrinfo *res = pcap_nametoaddrinfo(name);
        EXPECT_NOT_NULL(res, "pcap_nametoaddrinfo should not return NULL on repeated valid lookup");
        if (res != nullptr) {
            print_num_addr(res);
            SAFE_FREEADDRINFO(res);
        }
    }
}

// Test 5: Ensure that freeaddrinfo can be called on the result without crashing.
// This indirectly validates memory management expectations of the focal function.
static void test_pcap_nametoaddrinfo_memory_management() {
    const char *name = "localhost";
    std::cout << "Test 5: memory management test for pcap_nametoaddrinfo with \"" << name << "\"\n";

    struct addrinfo *res = pcap_nametoaddrinfo(name);
    EXPECT_NOT_NULL(res, "Expected non-null result to test freeaddrinfo");
    if (res != nullptr) {
        // Just exercise freeaddrinfo; no assertions about content
        freeaddrinfo(res);
    }
}

// Main driver: executes all tests and reports summary.
// This keeps the test suite self-contained and does not require an external test runner.
int main() {
    std::cout << "Starting test suite for pcap_nametoaddrinfo\n";

    // Execute tests
    test_pcap_nametoaddrinfo_valid_hostname();
    test_pcap_nametoaddrinfo_invalid_hostname();
    test_pcap_nametoaddrinfo_empty_string();
    test_pcap_nametoaddrinfo_repeated_valid_lookups();
    test_pcap_nametoaddrinfo_memory_management();

    // Summary
    std::cout << "Test suite completed. Total: " << g_tests_total
              << ", Passed: " << (g_tests_total - g_tests_failed)
              << ", Failed: " << g_tests_failed << "\n";

    // Return nonzero if any test failed to aid integration with build systems.
    return g_tests_failed == 0 ? 0 : 1;
}