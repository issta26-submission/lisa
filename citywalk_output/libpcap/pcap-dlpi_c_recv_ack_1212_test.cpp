/*
  C++11 Test Suite for the focal method:
  recv_ack(int fd, int size, const char *what, char *bufp, char *ebuf, int *uerror)

  Approach:
  - We create a small test harness that links with the original pcap-dlpi.c source.
  - We override the system getmsg(...) via a test-scoped C linkage symbol to control
    the flow of recv_ack without needing real STREAMS/DLPI setup.
  - We implement two executable tests:
      1) getmsg failure path -> expect PCAP_ERROR and uerror remains 0
      2) OK-ACK path (DL_INFO_ACK) with ctl.len == size -> expect return equals size
  - The tests rely on the actual constants (DL_INFO_ACK, PCAP_ERROR, etc.) provided
    by the included system headers (e.g., <sys/dlpi.h>, <stropts.h>).

  Notes:
  - This test suite uses a lightweight, non-terminating assertion style (EXPECT_EQ)
    to maximize coverage of code paths without aborting on first failure.
  - It assumes the environment provides the typical PCAP_* and DLPI symbols used by
    the focal code (pcap-dlpi.c).

  Candidate Keywords (from Step 1)
  - getmsg, MAXDLBUF, DL_INFO_ACK, DL_BIND_ACK, DL_OK_ACK, DL_ERROR_ACK
  - DL_SYSERR, DL_BADPPA, DL_ACCESS, EPERM, EACCES
  - pcapint_fmt_errmsg_for_errno, dlstrerror, pcapprim/dlprim
  - PCAP_ERROR, PCAP_ERROR_PERM_DENIED, PCAP_ERROR_NO_SUCH_DEVICE
  - uerror, ctl.buf, ctl.len, ctl.maxlen, bufp, ebuf
  - What: what (description string), size, fd
  - DLPI primitives layout and unions (DL_primitives, error_ack)
  - PCAP_ERRBUF_SIZE

  Limitations:
  - The tests intentionally exercise only the safe, controlled path (DL_INFO_ACK) to
    avoid requiring exact nested DLPI error structures in memory. A fuller suite would
    need precise binary layouts of DLPI structures which vary by platform.
  - If your environment uses different symbol visibility rules, ensure the test executable
    allows interposition of getmsg (the overriding symbol within the test translation unit
    should be linked before the library).

  Build:
  - Compile this test together with pcap-dlpi.c using a C++11 capable compiler, e.g.:
      g++ -std=c++11 -O0 -g -I. -I/path/to/pcap/includes test_recv_ack.cpp pcap-dlpi.c -o test_recv_ack
  - Ensure system headers provide sys/dlpi.h and stropts.h.
*/

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/systeminfo.h>
#include <limits.h>
#include <sys/dlpi.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <os-proto.h>
#include <sys/bufmod.h>
#include <config.h>
#include <cstring>
#include <sys/stream.h>


// Platform headers providing DLPI constants and DLPI types.
// These headers exist on systems with DLPI support (e.g., Solaris, some Linux variants with DLPI dev headers).

extern "C" {
    // Focal method prototype (as in pcap-dlpi.c)
    int recv_ack(int fd, int size, const char *what, char *bufp, char *ebuf, int *uerror);

    // Externally declared to allow our test harness to override getmsg.
    // The real function signature used in pcap-dlpi.c:
    // int getmsg(int fd, struct strbuf *ctl, struct strbuf *ptr, int *flags);
    // We'll provide a test implementation with C linkage to interpose.
    int getmsg(int fd, struct strbuf *ctl, struct strbuf *mp, int *flags);
}

// Global test hooks to control fake getmsg behavior
static int g_fake_getmsg_return = 0;       // 0 = success, -1 = simulate error
static int g_fake_primitive = 0;           // Value to place in ctl.buf[0] as the primitive
static int g_fake_len = 0;                 // ctl.len value to simulate message length
static unsigned char test_buf[256];          // Buffer presented to recv_ack via ctl.buf

// A dedicated test buffer to pass as 'bufp' to recv_ack
static unsigned char g_bufp[256];

// Fake getmsg to control behavior of the focal method during tests
extern "C" int getmsg(int fd, struct strbuf *ctl, struct strbuf *mp, int *flags) {
    (void)fd; (void)mp; (void)flags;
    if (g_fake_getmsg_return < 0) {
        errno = EAGAIN; // Simulate a transient error
        return -1;
    }
    // Prepare ctl to look like a single DLPI primitive in memory.
    // The code does: dlp = MAKE_DL_PRIMITIVES(ctl.buf); then uses dlp->dl_primitive
    // We place the primitive at the start of ctl.buf so that ctl.buf[0..] can be interpreted as needed.
    std::memset(test_buf, 0, sizeof(test_buf));
    std::memcpy(test_buf, &g_fake_primitive, sizeof(g_fake_primitive));

    // Populate ctl for the caller
    ctl->buf = (char*)test_buf;
    ctl->maxlen = 4096;  // arbitrary, just needs to be large enough
    ctl->len = g_fake_len; // number of bytes actually received

    // Also copy to the user-provided output buffer if needed by caller
    // (recv_ack uses ctl.buf for the data; it doesn't copy into bufp in success path.)
    // We leave g_bufp unused for the test path we exercise.

    return 0; // success
}

// Simple non-terminating test assertion macro
#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        ++g_tests_failed; \
        std::cerr << "Test failed: " << (msg) \
                  << " (expected: " << (expected) \
                  << ", actual: " << (actual) << ")" << std::endl; \
    } \
} while (0)

// Global counters for test progress
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Each test is implemented as a function to allow clear isolation and comments
bool test_recv_ack_failure_getmsg() {
    // Arrange
    g_fake_getmsg_return = -1;              // simulate getmsg failure
    g_fake_primitive = 0;                   // value won't be used since getmsg fails
    g_fake_len = 0;
    memset(g_bufp, 0, sizeof(g_bufp));
    char ebuf[256];
    int uerror = 0;

    // Act
    int ret = recv_ack(0, 10, "test", (char*)g_bufp, ebuf, &uerror);

    // Assert
    // Expect PCAP_ERROR and uerror remains 0
    // We rely on PCAP_ERROR symbol from included pcap/dlpi headers
    EXPECT_EQ(ret, PCAP_ERROR, "recv_ack should return PCAP_ERROR on getmsg failure");
    EXPECT_EQ(uerror, 0, "uerror should remain 0 when getmsg fails");
    return true;
}

bool test_recv_ack_ok_path_info_ack() {
    // Arrange
    g_fake_getmsg_return = 0;                // simulate getmsg success
    g_fake_primitive = DL_INFO_ACK;          // primitive indicating OK ACK
    const int expected_len = 16;
    g_fake_len = expected_len;
    memset(g_bufp, 0, sizeof(g_bufp));
    char ebuf[256];
    int uerror = 0;

    // Act
    int ret = recv_ack(0, expected_len, "test", (char*)g_bufp, ebuf, &uerror);

    // Assert
    // The function should return the length of the ack (ctl.len)
    EXPECT_EQ(ret, expected_len, "recv_ack should return the ack length when primitive is DL_INFO_ACK and lengths match");
    // uerror is not set in this path
    EXPECT_EQ(uerror, 0, "uerror should remain 0 on success path");
    return true;
}

bool test_recv_ack_unknown_primitive_default_path() {
    // Arrange
    g_fake_getmsg_return = 0;
    g_fake_primitive = 9999; // Some unknown primitive
    g_fake_len = 8;
    memset(g_bufp, 0, sizeof(g_bufp));
    char ebuf[256];
    int uerror = 0;

    // Act
    int ret = recv_ack(0, 8, "test", (char*)g_bufp, ebuf, &uerror);

    // Assert
    // Expect PCAP_ERROR in default/unknown-primitive path
    EXPECT_EQ(ret, PCAP_ERROR, "recv_ack should return PCAP_ERROR on unknown primitive");
    return true;
}

int main() {
    std::cout << "Starting recv_ack test suite (non-GTest, self-contained)..." << std::endl;

    test_recv_ack_failure_getmsg();
    test_recv_ack_ok_path_info_ack();
    test_recv_ack_unknown_primitive_default_path();

    std::cout << "Ran " << g_tests_run << " tests, "
              << g_tests_failed << " failed." << std::endl;

    if (g_tests_failed > 0) {
        std::cerr << "Some tests FAILED." << std::endl;
        return 2;
    }
    std::cout << "All tests PASSED (best-effort in this constrained environment)." << std::endl;
    return 0;
}