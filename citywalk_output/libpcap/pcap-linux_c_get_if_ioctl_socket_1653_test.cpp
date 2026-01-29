// Unit test suite for get_if_ioctl_socket() in pcap-linux.c
// This test suite is written in C++11 (no GoogleTest). It exercises
// the focal function by invoking it in a real environment and performing
// lightweight sanity checks on its return value.
// The tests are non-terminating: failures are reported, but do not abort
// the entire test suite, allowing multiple tests to run in a single run.
//
// Explanation notes (embedded as comments):
// - Candidate Keywords (core dependencies observed in the focal method):
//   socket, AF_NETLINK, SOCK_RAW, NETLINK_GENERIC, ioctl, SIOCGIFNAME,
//   ifreq, memset, close, errno, EOPNOTSUPP, AF_UNIX, AF_INET6, AF_INET,
//   SOCK_DGRAM, SOCK_RAW
// - Test approach:
//   1) Call get_if_ioctl_socket() and verify a valid file descriptor is returned
//      (non-negative). This indirectly exercises multiple branches in typical
//      Linux environments where at least one of the socket types is usable.
//   2) Verify the returned descriptor is usable (e.g., can be queried with fcntl).
//   3) Call the function multiple times to exercise repeated path exploration and
//      ensure there are no resource leaks (sockets are closed by the library when
//      needed and not leaked by tests).
// - Static/private symbols in the focal file are not accessed directly; only the
//   public function get_if_ioctl_socket() is used.

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


// Forward declaration to link to the C function defined in pcap-linux.c.
// The function has C linkage; we declare it as extern "C" in C++ to avoid
// C++ name mangling issues during linking.
extern "C" int get_if_ioctl_socket(void);

// Lightweight test harness (non-intrusive assertions)
static int g_test_failures = 0;

#define LOG_INFO(msg) do { std::cout << "[INFO] " << msg << std::endl; } while (0)
#define LOG_FAIL(msg) do { std::cerr << "[FAIL] " << msg << std::endl; } while (0)
#define EXPECT_TRUE(expr, msg) do { if(!(expr)) { LOG_FAIL(msg); ++g_test_failures; } } while(0)
#define EXPECT_GE(a, b, msg) do { if((a) < (b)) { LOG_FAIL(msg); ++g_test_failures; } } while(0)
#define EXPECT_NONNEG(val, msg) EXPECT_GE((val), 0, msg)

static void test_get_if_ioctl_socket_basic()
{
    LOG_INFO("Test: get_if_ioctl_socket returns a non-negative fd and it's usable (basic sanity).");

    int fd = get_if_ioctl_socket();
    EXPECT_NONNEG(fd, "get_if_ioctl_socket() should return a non-negative fd (>= 0).");
    if (fd >= 0) {
        // Quick sanity: the descriptor should be a valid file descriptor.
        int flags = fcntl(fd, F_GETFD);
        EXPECT_TRUE(flags != -1, "Returned fd should be a valid file descriptor (fcntl F_GETFD).");
        // Close the descriptor to avoid leakage.
        if (close(fd) != 0) {
            LOG_FAIL("Failed to close the test socket descriptor.");
        } else {
            LOG_INFO("Test socket descriptor closed successfully.");
        }
    }
}

static void test_get_if_ioctl_socket_multiple_calls()
{
    LOG_INFO("Test: calling get_if_ioctl_socket() repeatedly should be safe and non-leaking.");

    const int iterations = 10;
    for (int i = 0; i < iterations; ++i) {
        int fd = get_if_ioctl_socket();
        // We tolerate -1 in pathological environments, but record the outcome.
        if (fd < 0) {
            LOG_INFO("Iteration " << i << ": get_if_ioctl_socket() returned -1 (no usable socket in this environment).");
            continue;
        }
        EXPECT_GE(fd, 0, "Iteration " << i << ": fd should be non-negative when a socket is returned.");
        if (fd >= 0) {
            int flags = fcntl(fd, F_GETFD);
            EXPECT_TRUE(flags != -1, "Iteration " << i << ": fd should be valid (fcntl F_GETFD).");
            if (close(fd) != 0) {
                LOG_FAIL("Iteration " << i << ": failed to close the test socket descriptor.");
            }
        }
    }
}

static void run_all_tests()
{
    test_get_if_ioctl_socket_basic();
    test_get_if_ioctl_socket_multiple_calls();

    if (g_test_failures) {
        std::cout << "TOTAL FAILURES: " << g_test_failures << std::endl;
    } else {
        std::cout << "ALL TESTS PASSED" << std::endl;
    }
}

int main()
{
    // Run the test suite
    run_all_tests();
    // Return non-zero if any test failed
    return g_test_failures ? 1 : 0;
}