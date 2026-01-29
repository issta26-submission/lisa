/*
  Lightweight C++11 test-suite (no GoogleTest) for the focal method:
  pcap_inject_linux(pcap_t *handle, const void *buf, int size)

  Approach:
  - Provide minimal, local stand-ins for the external types used by the focal method
    (pcap_t, pcap_linux) that mirror the actual fields accessed by the function.
  - Provide tiny stubs for helper functions that pcap_inject_linux calls
    (pcapint_strlcpy, pcapint_fmt_errmsg_for_errno) so the test can execute.
  - Interpose a fake 'send' function to exercise the different code paths
    (success, errno-based failure).
  - Exercise true/false branches of the predicate checks and both success and error paths.

  Important notes:
  - This test is designed to be compiled and linked in a C++11 environment.
  - It does not rely on GTest; it uses simple assertions implemented as macros.
  - The real project may have tighter coupling with libpcap internals; this test
    focuses on the logical branches of pcap_inject_linux as exercised with a
    controlled fake environment.
  - The test code includes explanatory comments above each unit test case.
*/

#include <netinet/in.h>
#include <pcap/vlan.h>
#include <unistd.h>
#include <stdio.h>
#include <linux/if_ether.h>
#include <netlink/msg.h>
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
#include <cstddef>
#include <netlink/genl/ctrl.h>
#include <linux/if_bonding.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <sys/utsname.h>


// Provide a consistent error buffer size as used by libpcap
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// ---------------------------------------------------------------------------
// Minimal stand-ins for production types (as used by pcap_inject_linux)
//
// Note: We mirror only the fields accessed by the focal function to keep the
// test self-contained and avoid depending on libpcap internal headers.
// ---------------------------------------------------------------------------

struct pcap_linux {
    int ifindex;   // -1 means "any" device (as used in the focal method)
    int cooked;    // non-zero means cooked mode (as used in the focal method)
};

struct pcap_t {
    pcap_linux *priv;       // private per-platform state (mirrored)
    int fd;                   // socket/file descriptor used for send()
    char errbuf[PCAP_ERRBUF_SIZE]; // error string buffer
};

// Forward declaration of the focal function (as it would be provided by the
// production code in pcap-linux.c)
extern "C" int pcap_inject_linux(pcap_t *handle, const void *buf, int size);

// ---------------------------------------------------------------------------
// Lightweight, test-specific stubs for library helpers called by pcap_inject_linux
// These are simple, non-terminating routines suitable for unit tests.
// ---------------------------------------------------------------------------

extern "C" void pcapint_strlcpy(char *dst, const char *src, size_t siz)
{
    if (dst == nullptr || src == nullptr || siz == 0) return;
    std::strncpy(dst, src, siz - 1);
    dst[siz - 1] = '\0';
}

extern "C" void pcapint_fmt_errmsg_for_errno(char *buf, size_t buflen, int err, const char *funcname)
{
    // Minimal formatting that mirrors typical behavior described in tests
    std::snprintf(buf, buflen, "%s: errno %d", funcname, err);
}

// ---------------------------------------------------------------------------
// Interposed send() to control the outcome of the focal function's write path
// The tests manipulate the return value to exercise different branches.
// ---------------------------------------------------------------------------

static int g_send_ret = 0;     // controlled return value for send()
static int g_last_send_ret_errno = 0; // controlled errno that send() should set
extern "C" int send(int sockfd, const void *buf, size_t len, int flags)
{
    (void)sockfd; (void)buf; (void)len; (void)flags;
    // Simulate errno for the next call to pcap_inject_linux
    errno = g_last_send_ret_errno;
    return g_send_ret;
}

// ---------------------------------------------------------------------------
// Lightweight testing framework (non-terminating assertions)
// ---------------------------------------------------------------------------

#define STR_STRSTR(hay, needle) (strstr((hay), (needle)) != nullptr)
#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { printf("    [FAIL] %s\n", (msg)); return false; } } while(0)
#define EXPECT_EQ(a, b, msg) \
    do { if ((a) != (b)) { printf("    [FAIL] %s: expected %d, got %d\n", (msg), (int)(b), (int)(a)); return false; } } while(0)
#define EXPECT_STR_CONTAINS(buf, needle, msg) \
    do { if (!STR_STRSTR((buf), (needle))) { printf("    [FAIL] %s: buffer did not contain \"%s\". Buffer=\"%s\"\n", (msg), (needle), (buf)); return false; } } while(0)
#define TEST_PASSED(msg) printf("    [PASS] %s\n", (msg))

// ---------------------------------------------------------------------------
// Helper to reset the error buffer to a known state (clears previous content)
// ---------------------------------------------------------------------------

static void reset_errbuf(pcap_t &h)
{
    std::memset(h.errbuf, 0, PCAP_ERRBUF_SIZE);
}

// ---------------------------------------------------------------------------
// Test cases for pcap_inject_linux
// Each test captures a single logical scenario described in comments above.
// ---------------------------------------------------------------------------

static bool test_inject_any_device()
{
    // Test 1: If ifindex == -1, the function should return -1 and set the
    // appropriate error message indicating sending on "any" device isn't supported.
    // Setup
    pcap_linux priv;
    priv.ifindex = -1;    // "any" device
    priv.cooked = 0;      // not cooked mode

    pcap_t handle;
    handle.priv = &priv;
    handle.fd = 12345;      // dummy socket descriptor
    reset_errbuf(handle);
    const char payload[] = "HELLO";
    int size = (int)sizeof(payload) - 1;

    // Exercise
    int ret = pcap_inject_linux(&handle, payload, size);

    // Verify
    if (ret != -1) {
        printf("    [FAIL] test_inject_any_device: expected -1, got %d\n", ret);
        return false;
    }
    EXPECT_STR_CONTAINS(handle.errbuf, "Sending packets isn't supported on the \"any\" device",
                      "pcap_inject_linux should report any-device limitation");

    TEST_PASSED("test_inject_any_device");
    return true;
}

static bool test_inject_cooked_mode()
{
    // Test 2: cooked mode is not supported for sending; expect -1 and an
    // error message about cooked mode.
    pcap_linux priv;
    priv.ifindex = 5;   // valid index
    priv.cooked = 1;    // cooked mode active

    pcap_t handle;
    handle.priv = &priv;
    handle.fd = 1234;
    reset_errbuf(handle);
    const char payload[] = "DATA";
    int size = (int)sizeof(payload) - 1;

    int ret = pcap_inject_linux(&handle, payload, size);

    if (ret != -1) {
        printf("    [FAIL] test_inject_cooked_mode: expected -1, got %d\n", ret);
        return false;
    }
    EXPECT_STR_CONTAINS(handle.errbuf, "Sending packets isn't supported in cooked mode",
                      "pcap_inject_linux should report cooked-mode limitation");

    TEST_PASSED("test_inject_cooked_mode");
    return true;
}

static bool test_inject_send_success()
{
    // Test 3: Normal path where send() succeeds and returns the same size.
    pcap_linux priv;
    priv.ifindex = 2;
    priv.cooked = 0;

    pcap_t handle;
    handle.priv = &priv;
    handle.fd = 9999;
    reset_errbuf(handle);
    const char payload[] = "PAYLOAD";
    int size = (int)sizeof(payload) - 1;

    // Configure the interposed send() to succeed
    g_send_ret = size;        // emulate bytes sent
    g_last_send_ret_errno = 0;
    errno = 0;

    int ret = pcap_inject_linux(&handle, payload, size);

    if (ret != size) {
        printf("    [FAIL] test_inject_send_success: expected %d, got %d\n", size, ret);
        return false;
    }

    // No error buffer should be touched in success path (but we won't assert that strictly)
    TEST_PASSED("test_inject_send_success");
    return true;
}

static bool test_inject_send_errno()
{
    // Test 4: send() returns -1 with errno set; verify -1 and that
    // the error message includes "send" and the errno code.
    pcap_linux priv;
    priv.ifindex = 3;
    priv.cooked = 0;

    pcap_t handle;
    handle.priv = &priv;
    handle.fd = 8888;
    reset_errbuf(handle);
    const char payload[] = "PKT";
    int size = (int)sizeof(payload) - 1;

    // Configure the interposed send() to fail
    g_send_ret = -1;
    g_last_send_ret_errno = 13; // EACCES as a representative errno
    errno = 0;

    int ret = pcap_inject_linux(&handle, payload, size);

    if (ret != -1) {
        printf("    [FAIL] test_inject_send_errno: expected -1, got %d\n", ret);
        return false;
    }
    // Check that error message indicates "send" and includes the errno
    EXPECT_STR_CONTAINS(handle.errbuf, "send", "pcap_inject_linux should set errno-based message on failure");
    // Additional check: ensure errno value is reflected
    char expected_fragment[64];
    std::snprintf(expected_fragment, sizeof(expected_fragment), "errno %d", 13);
    EXPECT_STR_CONTAINS(handle.errbuf, expected_fragment,
                      "pcap_inject_linux error message should include errno value");

    TEST_PASSED("test_inject_send_errno");
    return true;
}

// ---------------------------------------------------------------------------
// Main entry: run all tests
// ---------------------------------------------------------------------------

int main()
{
    int passed = 0;
    int failed = 0;

    printf("pcap_inject_linux unit tests (lightweight, non-terminating assertions)\n");

    if (test_inject_any_device()) ++passed; else ++failed;
    if (test_inject_cooked_mode()) ++passed; else ++failed;
    if (test_inject_send_success()) ++passed; else ++failed;
    if (test_inject_send_errno()) ++passed; else ++failed;

    printf("Summary: %d passed, %d failed\n", passed, failed);
    return (failed == 0) ? 0 : 1;
}