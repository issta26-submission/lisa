// Test suite for pcapint_findalldevs_interfaces
// This test harness uses a lightweight, non-GTest approach suitable for
// C++11 environments. It provides two focused tests:
//
// 1) Socket failure path: ensures the function returns -1 and reports "socket"
// 2) No-interfaces path: forces SIOCGIFCONF to present zero interfaces (via ioctl)
//    and ensures the function returns 0 without adding any interfaces.
//
// Important: This test relies on symbol interposition for C system calls (socket/ioctl)
// and on linking against the production implementation of pcapint_findalldevs_interfaces.
// In environments where binary interposition is supported (LD_PRELOAD, or link-time overrides),
// these tests will exercise the known error paths without requiring actual OS-level interfaces.
//
// Notes:
// - The test uses forward-declared minimal types to avoid depending on the full pcap headers
//   in the test file, while still matching the function signature expected by pcapint_findalldevs_interfaces.
// - Real-world CI should link against the library/object that provides pcapint_findalldevs_interfaces.

#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <cstdio>
#include <cerrno>
#include <string>
#include <sys/param.h>
#include <sys/socket.h>
#include <net/if.h>
#include <limits.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <os-proto.h>
#include <config.h>
#include <cstring>


// Forward declarations to avoid pulling full pcap headers in test file.
// These mirror the types used by the focal function, but are kept lightweight here.
struct pcap_if_list;
typedef struct pcap_if_list pcap_if_list_t;
typedef int (*get_if_flags_func)(const char *);

// Prototype of the focal function under test (as it would appear in the real project).
extern "C" int pcapint_findalldevs_interfaces(pcap_if_list_t *devlistp, char *errbuf,
    int (*check_usable)(const char *), get_if_flags_func get_flags_func);

// Global test hooks to control behavior of system-call interposition
static bool g_socket_fail = false;      // When true, our socket() override will fail
static bool g_ioctl_sim_zero = true;    // When true, our ioctl(SIOCGIFCONF, ...) returns zero interfaces

// Simple forward declarations to provide minimal behavior for test helpers.
// These are not real mocks; they rely on the test harness providing interposed symbols
// for system calls (socket, ioctl) when linked with the production code.
extern "C" {

    // Intercept socket() to simulate failure or success without touching real OS state
    int socket(int domain, int type, int protocol) {
        if (g_socket_fail) {
            errno = ENOENT; // Certainly reflect a typical "no such file or directory" style error
            return -1;
        }
        // Simulate a valid descriptor; the exact value is not important for the logic under test.
        return 3;
    }

    // Intercept ioctl() to control behavior of SIOCGIFCONF and other IOCTLs used in the focal code
    int ioctl(int fd, unsigned long request, void *arg) {
        // We only need to model SIOCGIFCONF for the focused tests.
        // Other requests can be treated opaquely.
        if (request == SIOCGIFCONF) {
            // arg is a pointer to struct ifconf. We simulate the OS filling it.
            // For test 2 (no interfaces), we return zero interfaces by setting ifc_len to 0.
            struct ifconf *ifc = (struct ifconf *)arg;
            if (ifc) {
                if (g_ioctl_sim_zero) {
                    ifc->ifc_len = 0;
                    ifc->ifc_buf = NULL; // no buffer used in this scenario
                } else {
                    // If not simulating zero interfaces, provide a tiny single-entry footprint
                    // without filling a real interface list (keep behavior safe).
                    ifc->ifc_len = 0;
                    ifc->ifc_buf = NULL;
                }
                return 0;
            }
            return -1;
        }

        // For other IOCTLs (e.g., SIOCGIFFLAGS, SIOCGIFNETMASK, etc.), we don't need to exercise them in these tests.
        // Return an error by default to surface unexpected usage during these controlled tests.
        errno = ENOTTY;
        return -1;
    }

    // Optional: Provide stub for pcapint_add_addr_to_if and other helpers if the library needs them during extended tests.
    // In these two tests, the code path does not reach the call that requires this function when we simulate failures.
    int pcapint_add_addr_to_if(pcap_if_list_t *devlistp, const char *name,
                               unsigned flags, get_if_flags_func get_flags_func,
                               const struct sockaddr *addr, size_t addr_size,
                               const struct sockaddr *netmask, size_t netmask_size,
                               const struct sockaddr *broadaddr, size_t broadaddr_size,
                               const struct sockaddr *dstaddr, size_t dstaddr_size,
                               char *errbuf)
    {
        (void)devlistp; (void)name; (void)flags; (void)get_flags_func;
        (void)addr; (void)addr_size; (void)netmask; (void)netmask_size;
        (void)broadaddr; (void)broadaddr_size; (void)dstaddr; (void)dstaddr_size;
        (void)errbuf;
        // For the focused tests, we won't actually populate any data structures.
        // Return success to allow downstream code to continue in tests that don't rely on this function.
        return 0;
    }

    // Simple string copy helper used by the focal code (pcapint_strlcpy signature)
    size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize) {
        if (dst == nullptr || dstsize == 0) return 0;
        size_t srclen = strlen(src);
        size_t copylen = srclen < (dstsize - 1) ? srclen : (dstsize - 1);
        memcpy(dst, src, copylen);
        dst[copylen] = '\0';
        return copylen;
    }

} // extern "C"

// Lightweight true/false assertion helpers (non-terminating in case of failure)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST FAILED] " << (msg) << " (\"" #cond "\")\n"; \
        return false; \
    } else { \
        std::cout << "[TEST PASSED] " << (msg) << "\n"; \
    } \
} while (0)

// Dummy predicate used by the focal method to decide usability of an interface.
// Our tests rely on it being always true (i.e., "usable") so that we focus on error paths.
extern "C" int test_always_usable(const char *name) {
    (void)name;
    return 1;
}

// Dummy get_flags function passed to the focal method (just a stub for compilation)
extern "C" int test_get_flags(const char *name) {
    (void)name;
    return 0;
}

// Candidate tests

// Test 1: Socket failure path
// - Simulate socket() failing and verify pcapint_findalldevs_interfaces returns -1.
// - The error buffer should be populated with a message that mentions "socket".
bool test_socket_failure_path() {
    g_socket_fail = true;      // Force socket() to fail
    g_ioctl_sim_zero = true;   // IOCTL path won't be exercised due to early return

    char errbuf[256];
    // Use NULL for devlistp to isolate the error path (no real devlist needed)
    int ret = pcapint_findalldevs_interfaces(nullptr, errbuf, test_always_usable, test_get_flags);

    // Expect immediate failure (-1) due to socket() failure
    bool ok = (ret == -1);
    if (ok) {
        // Verify the error buffer contains the word "socket"
        if (strstr(errbuf, "socket") == nullptr) {
            std::cerr << "[TEST FAILED] Error message should mention 'socket'. errbuf='" << errbuf << "'\n";
            ok = false;
        } else {
            std::cout << "[TEST PASSED] Socket failure path reported correctly.\n";
        }
    } else {
        std::cerr << "[TEST FAILED] Expected -1 return on socket failure, got " << ret << "\n";
    }
    // Reset global flag for subsequent tests
    g_socket_fail = false;
    return ok;
}

// Test 2: No interfaces found path
// - Simulate SIOCGIFCONF returning zero interfaces, ensuring function returns 0
//   and does not attempt to add any interfaces.
bool test_no_interfaces_path() {
    g_socket_fail = false;       // Allow socket() to succeed
    g_ioctl_sim_zero = true;     // SIOCGIFCONF will claim zero interfaces

    char errbuf[256];
    int ret = pcapint_findalldevs_interfaces(nullptr, errbuf, test_always_usable, test_get_flags);

    bool ok = (ret == 0);
    if (ok) {
        std::cout << "[TEST PASSED] No-interfaces path returns 0 as expected.\n";
    } else {
        std::cerr << "[TEST FAILED] Expected 0 return for no interfaces, got " << ret
                  << ". errbuf='" << errbuf << "'\n";
    }
    return ok;
}

// Main test runner
int main() {
    std::cout << "Running unit tests for pcapint_findalldevs_interfaces...\n";

    bool all_ok = true;

    // Run Test 1
    all_ok &= test_socket_failure_path();

    // Run Test 2
    all_ok &= test_no_interfaces_path();

    if (all_ok) {
        std::cout << "ALL TESTS PASSED.\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED.\n";
        return 1;
    }
}