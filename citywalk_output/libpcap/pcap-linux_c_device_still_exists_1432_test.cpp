// Unit test suite for the focal method: device_still_exists(pcap_t *handle)
// This test suite is written in C++11, uses a lightweight, non-terminating assertion approach,
// and mocks external OS interactions (getsockname) to cover all branches.
// The tests are designed to be portable with minimal dependencies and to compile
// alongside the target C sources (pcap-linux.c) by providing compatible type declarations
// and function stubs.
// Notes:
// - The tests do not rely on GTest/GMock. They implement simple test harness with main().
// - We mock getsockname() to control the behavior of the underlying system call.
// - We verify: (1) ifindex == -1 returns 1; (2) getsockname failure returns -1; (3) on success with sll_ifindex == -1 returns 0; (4) on success with sll_ifindex != -1 returns 1.

#include <netinet/in.h>
#include <pcap/vlan.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <netlink/msg.h>
#include <iostream>
#include <sys/ioctl.h>
#include <errno.h>
#include <pcap-snf.h>
#include <cstdlib>
#include <pcap/can_socketcan.h>
#include <pcap-util.h>
#include <cstdio>
#include <dirent.h>
#include <cerrno>
#include <netlink/genl/genl.h>
#include <linux/netlink.h>
#include <fcntl.h>
#include <cstring>
#include <diag-control.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <linux/sockios.h>
#include <endian.h>
#include <linux/net_tstamp.h>
#include <linux/filter.h>
#include <poll.h>
#include <netlink/genl/family.h>
#include <limits.h>
#include <stdlib.h>
#include <linux/nl80211.h>
#include <pcap-int.h>
#include <netlink/attr.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/eventfd.h>
#include <linux/types.h>
#include <linux/if_arp.h>
#include <linux/ethtool.h>
#include <netlink/genl/ctrl.h>
#include <linux/if_bonding.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <sys/utsname.h>


// Minimal re-declarations to be compatible with the focal function's expectations.
// These do not replace the real headers; they merely provide a compatible environment for testing.

#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// Public-facing pcap types used by the focal function.
// We provide a light-weight approximation؛ just enough for unit tests.
typedef struct pcap_linux {
    int ifindex;
} pcap_linux;

typedef struct pcap_t {
    pcap_linux *priv;
    int fd;
    char errbuf[PCAP_ERRBUF_SIZE];
} pcap_t;

// Forward declaration of the focal function (C linkage)
extern "C" int device_still_exists(pcap_t *handle);

// Global variables to drive mock behavior for getsockname
static int mock_getsockname_result = 0;      // 0 => success, -1 => failure
static int mock_sll_ifindex_value = 0;       // value to write into addr.sll_ifindex on success
static int mock_errno_for_getsockname = 0;   // errno to simulate on failure

// Mocked sockaddr_ll structure matching Linux layout (only fields used by test wrapper)
#ifndef AF_PACKET
#define AF_PACKET 17
#endif

struct sockaddr {
    unsigned short sa_family;
};

// Linux specific sockaddr_ll (as used by actual code)
struct sockaddr_ll {
    unsigned short sll_family;   // AF_PACKET
    unsigned short sll_protocol; // protocol
    int            sll_ifindex;  // interface index
    unsigned short sll_halen;    // length of addr
    unsigned char  sll_addr[8];  // ethernet address
};

// Mocked getsockname to intercept and control behavior for testing.
// This overrides the real libc getsockname when linking this test binary.
extern "C" int getsockname(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    (void)sockfd; // unused in tests

    if (mock_getsockname_result == -1) {
        // Simulate error path
        errno = mock_errno_for_getsockname ? mock_errno_for_getsockname : EINVAL;
        return -1;
    }

    // Simulate success: fill in a sockaddr_ll with a controllable sll_ifindex
    if (addr == nullptr || addrlen == nullptr) {
        // Invalid parameters; mimic the standard behavior
        errno = EINVAL;
        return -1;
    }

    struct sockaddr_ll *ll = reinterpret_cast<struct sockaddr_ll*>(addr);
    ll->sll_family    = AF_PACKET;
    ll->sll_protocol  = 0;
    ll->sll_ifindex   = mock_sll_ifindex_value;
    ll->sll_halen     = 0;
    // sll_addr not used by tests; zero it for determinism
    std::memset(ll->sll_addr, 0, sizeof(ll->sll_addr));

    *addrlen = sizeof(struct sockaddr_ll);

    return 0;
}

// Mocked pcapint_fmt_errmsg_for_errno used by device_still_exists on error.
// We capture the errno and a flag indicating the function was invoked.
static int last_errnum_seen = 0;
static int got_error_message = 0;

extern "C" void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbuf_size,
                                             int errnum, const char *fmt, ...)
{
    (void)errbuf; (void)errbuf_size; (void)fmt; // ignore formatting for test
    last_errnum_seen = errnum;
    got_error_message = 1;
}

// Helper to reset test state
static void reset_test_context()
{
    mock_getsockname_result = 0;
    mock_sll_ifindex_value = 0;
    mock_errno_for_getsockname = 0;
    last_errnum_seen = 0;
    got_error_message = 0;
}

// Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;

#define TEST_PASSED() do { ++tests_run; ++tests_passed; } while(0)
#define TEST_FAILED(msg) do { ++tests_run; std::cerr << "[FAIL] " << __FUNCTION__ << " - " << (msg) << std::endl; } while(0)
#define ASSERT_EQ(actual, expected, msg)                                           \
    do {                                                                             \
        if ((actual) != (expected)) {                                              \
            TEST_FAILED(msg);                                                      \
            std::cerr << "  Expected: " << (expected) << ", Got: " << (actual) << std::endl; \
        } else {                                                                   \
            TEST_PASSED();                                                       \
        }                                                                          \
    } while(0)

#define ASSERT_TRUE(cond, msg)                                                       \
    do {                                                                             \
        if (!(cond)) {                                                               \
            TEST_FAILED(msg);                                                      \
        } else {                                                                     \
            TEST_PASSED();                                                         \
        }                                                                            \
    } while(0)


// Test 1: If handlep->ifindex == -1, device_still_exists should return 1 without touching socket
void test_device_still_exists_ifindex_minus_one()
{
    reset_test_context();

    struct pcap_linux pl = { .ifindex = -1 };
    pcap_t handle;
    handle.priv = &pl;
    handle.fd = 123; // arbitrary fd
    std::memset(handle.errbuf, 0, sizeof(handle.errbuf));

    int res = device_still_exists(&handle);

    ASSERT_EQ(res, 1, "Expected return value 1 when ifindex is -1");
}

// Test 2: getsockname failure should return -1 and set errno via pcapint_fmt_errmsg_for_errno
void test_device_still_exists_getsockname_failure()
{
    reset_test_context();

    struct pcap_linux pl = { .ifindex = 2 };
    pcap_t handle;
    handle.priv = &pl;
    handle.fd = 42;
    std::memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // Configure mock to fail
    mock_getsockname_result = -1;
    mock_errno_for_getsockname = EACCES;

    int res = device_still_exists(&handle);

    ASSERT_EQ(res, -1, "Expected return value -1 on getsockname failure");
    ASSERT_TRUE(got_error_message, "Expected pcapint_fmt_errmsg_for_errno to be invoked on error");
    ASSERT_EQ(last_errnum_seen, EACCES, "Expected errno EACCES to be passed to pcapint_fmt_errmsg_for_errno");
}

// Test 3: getsockname success and sll_ifindex == -1 -> device considered disappeared, return 0
void test_device_still_exists_getsockname_success_minus_one()
{
    reset_test_context();

    struct pcap_linux pl = { .ifindex = 2 };
    pcap_t handle;
    handle.priv = &pl;
    handle.fd = 55;
    std::memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // Configure mock to succeed and return sll_ifindex = -1
    mock_getsockname_result = 0;
    mock_sll_ifindex_value = -1;

    int res = device_still_exists(&handle);

    ASSERT_EQ(res, 0, "Expected return value 0 when getsockname reports sll_ifindex == -1 (device went away)");
}

// Test 4: getsockname success and sll_ifindex != -1 -> device considered still present, return 1
void test_device_still_exists_getsockname_success_non_negative_index()
{
    reset_test_context();

    struct pcap_linux pl = { .ifindex = 3 };
    pcap_t handle;
    handle.priv = &pl;
    handle.fd = 77;
    std::memset(handle.errbuf, 0, sizeof(handle.errbuf));

    // Configure mock to succeed and return a valid interface index
    mock_getsockname_result = 0;
    mock_sll_ifindex_value = 5;

    int res = device_still_exists(&handle);

    ASSERT_EQ(res, 1, "Expected return value 1 when getsockname reports a valid sll_ifindex (device likely down elsewhere, but still exists here)");
}


// Runner
int main()
{
    std::cout << "Running tests for device_still_exists (pcap-linux.c) - C++11 test harness" << std::endl;

    test_device_still_exists_ifindex_minus_one();
    test_device_still_exists_getsockname_failure();
    test_device_still_exists_getsockname_success_minus_one();
    test_device_still_exists_getsockname_success_non_negative_index();

    std::cout << "Test results: " << tests_passed << " passed, " << (tests_run - tests_passed) << " failed, out of " << tests_run << " tests." << std::endl;

    return (tests_run == tests_passed) ? 0 : 1;
}