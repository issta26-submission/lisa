// Note: This test suite is designed to exercise the focal function:
//       pcapint_findalldevs_interfaces located in fad-glifc.c
//       using a lightweight, self-contained C++11 harness without GTest.
// The tests rely on a minimal lightweight mocking strategy implemented
// via function interposition compatible with a typical POSIX environment.
// The mocks override certain OS interaction points (socket/ioctl/close/calloc/free)
// and specific helper functions used by the focal function. The goal is to
// achieve high coverage by driving true/false branches and edge cases
// (e.g., socket failures, ioctl errors, and logic around logical interfaces).
// This test file uses a single main() to run all tests and prints results.

#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fad-glifc.c>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <sys/param.h>
#include <sys/socket.h>
#include <net/if.h>
#include <cstdarg>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pcap-int.h>
#include <sys/sockio.h>
#include <string.h>
#include <sys/file.h>
#include <memory.h>
#include <os-proto.h>
#include <config.h>
#include <cstring>


// Ensure the C code is compiled with C linkage for the focal function.
// We include fad-glifc.c inside extern "C" to avoid name mangling.
extern "C" {
}

// Domain knowledge helpers and test harness constants
#define PCAP_ERRBUF_SIZE 256       // Mirror typical PCAP error buffer size
#define TEST_NAME_MAX 128

// PCAP_ISDIGIT macro used by the focal code (mirror its usage)
#ifndef PCAP_ISDIGIT
#define PCAP_ISDIGIT(x) ((x) >= '0' && (x) <= '9')
#endif

// Typedefs matching the focal code's expectations (from pcap-int.h typically)
typedef int (*get_if_flags_func)(const char *);

// Forward declaration for the pcap_if_list_t type used by the focal code.
// In the real project, this type is defined in pcap-int.h.
// We declare an opaque placeholder here to keep tests compilable in isolation.
typedef struct pcap_if_list pcap_if_list_t;

// Global test knobs to drive behavior in the mocked OS layer
static bool g_mock_socket_fail = false;
static int g_mock_socket_call = 0;
static bool g_mock_second_socket_fail = false;
static bool g_mock_ioctl_fail_non_enxio = false;
static bool g_mock_netmask_not_available = false;
static bool g_mock_logical_interface = false;
static char g_last_added_name[TEST_NAME_MAX] = {0};

// A minimal mock of pcap_int helper functions expected by the focal code.
// These mocks are kept intentionally simple to enable focused coverage of
// the focal method's branching logic.
extern "C" {

// Mocked socket(): simulate AF_INET and AF_INET6 sockets in a deterministic way.
int socket(int domain, int type, int protocol) {
    if (g_mock_socket_fail) {
        // Simulate a failure at the first socket() call
        return -1;
    }
    ++g_mock_socket_call;
    // Return small, fake file descriptors
    if (domain == AF_INET) {
        return 3;
    } else if (domain == AF_INET6) {
        if (g_mock_second_socket_fail) {
            return -1;
        }
        return 4;
    }
    return -1;
}

// Mocked ioctl(): drive specific branches in the focal method.
// We populate necessary structures on demand to simulate interface data.
int ioctl(int fd, unsigned long request, void *arg) {
    // If test wants to force non-enxio ioctl failure, emulate that for a fall-through path
    if (g_mock_ioctl_fail_non_enxio && request != SIOCGLIFNUM) {
        errno = EPERM;
        return -1;
    }

    // SIOCGLIFNUM: pretend we have exactly 1 interface
    if (request == SIOCGLIFNUM) {
        struct lifnum *ln = (struct lifnum *)arg;
        if (ln) {
            ln->lifn_count = 1;
            ln->lifn_family = AF_UNSPEC;
            ln->lifn_flags = 0;
        }
        return 0;
    }

    // SIOCGLIFCONF: fill a single lifreq entry with a name
    if (request == SIOCGLIFCONF) {
        struct lifconf *lc = (struct lifconf *)arg;
        if (lc && lc->lifc_buf && lc->lifc_len >= sizeof(struct lifreq)) {
            struct lifreq *lr = (struct lifreq *)lc->lifc_buf;
            // Simulate a single interface "eth0" or "eth0:1" if logical
            if (g_mock_logical_interface) {
                strncpy(lr->lifr_name, "eth0:1", sizeof(lr->lifr_name) - 1);
            } else {
                strncpy(lr->lifr_name, "eth0", sizeof(lr->lifr_name) - 1);
            }
            lr->lifr_name[sizeof(lr->lifr_name) - 1] = '\0';
            // lifr_addr left zeroed (not used by tests that validate name)
        }
        return 0;
    }

    // SIOCGLIFFLAGS: set minimal flags
    if (request == SIOCGLIFFLAGS) {
        struct lifreq *ifr = (struct lifreq *)arg;
        if (ifr) {
            ifr->lifr_flags = 0; // no IFF_BROADCAST, no IFF_POINTOPOINT
        }
        return 0;
    }

    // SIOCGLIFNETMASK: either success with a zeroed netmask or ENOTAVAIL depending on test flag
    if (request == SIOCGLIFNETMASK) {
        if (g_mock_netmask_not_available) {
            errno = EADDRNOTAVAIL;
            return -1;
        }
        // On success, pretend we set a netmask in the provided buffer
        struct lifreq *ifr = (struct lifreq *)arg;
        if (ifr) {
            // leave as zeroed sockaddr, which is fine for test purposes
        }
        return 0;
    }

    // SIOCGLIFBRDADDR / SIOCGLIFDSTADDR: simulate not available in tests
    if (request == SIOCGLIFBRDADDR || request == SIOCGLIFDSTADDR) {
        if (g_mock_netmask_not_available) {
            errno = EADDRNOTAVAIL;
            return -1;
        }
        return -1; // default to not available for test simplicity
    }

    return 0;
}

// Mock close(): no-op
int close(int fd) { (void)fd; return 0; }

// Mock calloc(): simple wrapper using malloc
void *calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    return malloc(total);
}

// Mock free(): pass-through
void free(void *ptr) { ::free(ptr); }

// Mock pcapint_strlcpy(): safe string copy into fixed dst buffer
size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize) {
    if (!dst || dstsize == 0) return 0;
    std::strncpy(dst, src, dstsize - 1);
    dst[dstsize - 1] = '\0';
    return std::strlen(dst);
}

// Mock error message formatter: keep simple, write into errbuf if provided
void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbuf_size,
    int errnum, const char *fmt, ...) {
    if (!errbuf || errbuf_size == 0) return;
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(errbuf, errbuf_size, fmt, ap);
    va_end(ap);
}

// Forward declaration for the mocked pcapint_add_addr_to_if used by the focal code.
typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_storage sockaddr_storage_t;
typedef int (*get_if_flags_func)(const char *);
typedef struct pcap_if_list pcap_if_list_t;

// We implement a lightweight mock of pcapint_add_addr_to_if to capture the
// lifr_name being added for validation in tests.
int pcapint_add_addr_to_if(pcap_if_list_t *devlistp,
                           const char *lifr_name,
                           unsigned int lifr_flags,
                           get_if_flags_func get_flags_func,
                           struct sockaddr *addr,
                           size_t addrlen,
                           struct sockaddr *netmask,
                           size_t netmasklen,
                           struct sockaddr *broadaddr,
                           size_t broadaddrlen,
                           struct sockaddr *dstaddr,
                           size_t dstaddrlen,
                           char *errbuf)
{
    // Record the additive interface name for test verification.
    if (lifr_name && strlen(lifr_name) <  TEST_NAME_MAX) {
        strncpy(g_last_added_name, lifr_name, TEST_NAME_MAX - 1);
        g_last_added_name[TEST_NAME_MAX - 1] = '\0';
    }
    (void)devlistp; (void)lifr_flags; (void)get_flags_func;
    (void)addr; (void)addrlen; (void)netmask; (void)netmasklen;
    (void)broadaddr; (void)broadaddrlen; (void)dstaddr; (void)dstaddrlen;
    (void)errbuf;
    return 0;
}

// Simple test helper: reset global test state
void reset_test_state() {
    g_mock_socket_fail = false;
    g_mock_second_socket_fail = false;
    g_mock_ioctl_fail_non_enxio = false;
    g_mock_netmask_not_available = false;
    g_mock_logical_interface = false;
    g_last_added_name[0] = '\0';
    g_mock_socket_call = 0;
}

} // extern "C"

// The actual tests begin here
// We implement a lightweight assertion macro to avoid bringing in a heavy test framework.
#define TEST_ASSERT(cond, msg) \
    do { if (!(cond)) { std::cerr << "TEST FAILED: " << (msg) << " (in " << __FUNCTION__ << ")\n"; return 1; } } while (0)

// Test 1: Basic success path with a single interface "eth0"
int test_basic_success_path() {
    reset_test_state();
    // Drive single interface in SIOCGLIFCONF
    g_mock_logical_interface = false;
    // No failures in socket/ioctl
    // Call the focal function
    pcap_if_list_t *devlist = nullptr; // The real code may allocate; we pass NULL to keep it simple
    char errbuf[PCAP_ERRBUF_SIZE];
    int (*cb)(const char *) = [](const char *) -> int { return 1; };
    get_if_flags_func get_flags = cb;

    int ret = pcapint_findalldevs_interfaces(devlist, errbuf, cb, get_flags);
    // If we reach here, the function attempted to enumerate and invoked the mock addr addition
    TEST_ASSERT(ret == 0, "Expected non-negative return on success path");
    // The mock should have recorded the interface name (eth0)
    TEST_ASSERT(std::strcmp(g_last_added_name, "eth0") == 0,
                "Expected 'eth0' to be added to the interface list");
    return 0;
}

// Test 2: socket(AF_INET) failure -> function should return -1
int test_socket_failure() {
    reset_test_state();
    g_mock_socket_fail = true; // Force first socket() to fail
    pcap_if_list_t *devlist = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    int (*cb)(const char *) = [](const char *) -> int { return 1; };
    get_if_flags_func get_flags = cb;

    int ret = pcapint_findalldevs_interfaces(devlist, errbuf, cb, get_flags);
    TEST_ASSERT(ret == -1, "Expected -1 when socket(AF_INET) fails");
    return 0;
}

// Test 3: socket(AF_INET6) failure after AF_INET succeeds -> should return -1
int test_second_socket_failure() {
    reset_test_state();
    g_mock_second_socket_fail = true;
    pcap_if_list_t *devlist = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    int (*cb)(const char *) = [](const char *) -> int { return 1; };
    get_if_flags_func get_flags = cb;

    int ret = pcapint_findalldevs_interfaces(devlist, errbuf, cb, get_flags);
    TEST_ASSERT(ret == -1, "Expected -1 when second socket(AF_INET6) fails");
    return 0;
}

// Test 4: ioctl error on a non-ENXIO condition should cause termination (ret = -1)
int test_ioctl_error_non_enxio() {
    reset_test_state();
    g_mock_ioctl_fail_non_enxio = true;
    pcap_if_list_t *devlist = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    int (*cb)(const char *) = [](const char *) -> int { return 1; };
    get_if_flags_func get_flags = cb;

    int ret = pcapint_findalldevs_interfaces(devlist, errbuf, cb, get_flags);
    TEST_ASSERT(ret == -1, "Expected -1 when ioctl fails with non-ENXIO");
    return 0;
}

// Test 5: Logical interface stripping: eth0:1 should be stripped to eth0 when passed to pcapint_add_addr_to_if
int test_logical_interface_stripping() {
    reset_test_state();
    g_mock_logical_interface = true; // Simulate a logical interface "eth0:1"
    // We want to verify that the name passed to pcapint_add_addr_to_if is stripped to "eth0"
    pcap_if_list_t *devlist = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    int (*cb)(const char *) = [](const char *) -> int { return 1; };
    get_if_flags_func get_flags = cb;

    int ret = pcapint_findalldevs_interfaces(devlist, errbuf, cb, get_flags);
    // The internal stripping should have happened; the mock should see lifr_name trimmed to "eth0"
    TEST_ASSERT(ret == 0, "Expected success path for logical interface stripping");
    TEST_ASSERT(std::strcmp(g_last_added_name, "eth0") == 0,
                "Expected trimmed name 'eth0' to be added for a logical interface eth0:1");
    return 0;
}

// Main: Run all tests and report summary
int main() {
    int failures = 0;

    std::cout << "Running test: basic_success_path" << std::endl;
    failures += test_basic_success_path();

    std::cout << "Running test: socket_failure" << std::endl;
    failures += test_socket_failure();

    std::cout << "Running test: second_socket_failure" << std::endl;
    failures += test_second_socket_failure();

    std::cout << "Running test: ioctl_error_non_enxio" << std::endl;
    failures += test_ioctl_error_non_enxio();

    std::cout << "Running test: logical_interface_stripping" << std::endl;
    failures += test_logical_interface_stripping();

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << failures << " tests failed." << std::endl;
        return 1;
    }
}

// End of test suite

/***************************************************************
Notes for maintainers:
- This test suite uses a lightweight, in-source mock strategy to
  drive the focal function without requiring a full OS-level environment.
- The mocks are designed to be self-contained and to exercise
  specific branches within pcapint_findalldevs_interfaces:
  - Test 1 exercises a normal path with a single interface named "eth0".
  - Test 2 simulates a failure in the first socket() call (AF_INET).
  - Test 3 simulates a failure in the second socket() call (AF_INET6).
  - Test 4 triggers an ioctl error that is not ENXIO to ensure early termination.
  - Test 5 exercises the logical interface stripping logic (e.g., "eth0:1" -> "eth0").
- The test harness uses a single main() function, as GTest is not allowed per
  the instructions. Assertions are implemented via a simple TEST_ASSERT macro.
- Keep in mind: platform-specific header contents (lifnum, lifconf, lifreq, etc.)
  and behavior may differ across UNIX variants. This test harness assumes the
  environment provides the Solaris-like lif* APIs or their equivalents.
- If you integrate into a real CI, you may need to adjust the mock behavior to align
  with the exact lif* struct definitions on your target platform.
***************************************************************/