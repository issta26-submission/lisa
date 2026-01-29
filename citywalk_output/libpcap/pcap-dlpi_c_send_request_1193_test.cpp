// Unit test suite for the focal method: send_request in pcap-dlpi.c
// Environment: C++11, no GoogleTest. We provide simple test harness and lightweight mocks.
// The goal is to exercise both true/false branches of putmsg and the error formatting path.

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/systeminfo.h>
#include <cstdarg>
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


// Forward declaration of the function under test.
// We assume the project compiles send_request in a C translation unit.
extern "C" int send_request(int fd, char *ptr, int len, char *what, char *ebuf);

// Minimal C-like struct mirroring the kernel/user-space strbuf used by putmsg.
// The actual layout is not important for the tests since we do not access fields here.
struct strbuf {
    unsigned int maxlen;
    unsigned int len;
    char *buf;
};

// Mocks and lightweight harness to exercise the focal function in a controlled way.

// Global control for the mock putmsg behavior.
// 0 -> success (return 0), 1 -> failure (return -1 and set errno)
static int g_putmsg_mode = 0;

// Simple, architecture-agnostic mock of putmsg used by the test.
// We avoid dereferencing the ctl pointer to keep ABI compatibility concerns minimal.
// The real putmsg signature: int putmsg(int fd, struct strbuf *ctl, struct strbuf *data, int flags)
extern "C" int putmsg(int fd, struct strbuf *ctl, struct strbuf *data, int flags)
{
    (void)fd;
    (void)ctl;
    (void)data;
    (void)flags;
    if (g_putmsg_mode == 0) {
        return 0;    // Simulate success
    } else {
        errno = EIO; // Simulate a I/O error
        return -1;
    }
}

// Mock for pcapint_fmt_errmsg_for_errno used by the focal function on error.
// Signature: int pcapint_fmt_errmsg_for_errno(char *buf, size_t buflen, int errno_val, const char *fmt, ...)
extern "C" int pcapint_fmt_errmsg_for_errno(char *buf, size_t buflen, int errno_val, const char *fmt, ...)
{
    // Build the message from the variadic fmt with the provided arguments, then
    // prefix with "errno=<errno_val>; " to make the result deterministic.
    char composed[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(composed, sizeof(composed), fmt, ap);
    va_end(ap);
    snprintf(buf, buflen, "errno=%d; %s", errno_val, composed);
    return 0;
}

// Helper to set the mock mode
static inline void set_putmsg_mode(int mode)
{
    g_putmsg_mode = mode;
}

// Utility: secure memory set for ebuf initialization
static void init_ebuf(char *ebuf, size_t size)
{
    if (ebuf && size > 0) {
        ebuf[0] = '\0';
    }
}

// Test 1: Success path when putmsg succeeds
// - Ensures send_request returns 0
// - Ensures ebuf is left untouched (empty)
bool test_send_request_success()
{
    // Arrange
    set_putmsg_mode(0); // Configure mock to succeed
    const int fd = 42;
    const char payload[] = "payload-data";
    const int len = (int)strlen(payload);
    const char *what = "test-success";
    char ebuf[256];
    init_ebuf(ebuf, sizeof(ebuf));

    // Act
    int ret = send_request(fd, (char *)payload, len, (char *)what, ebuf);

    // Assert (non-terminating, continue-on-failure)
    bool ok = true;
    if (ret != 0) {
        fprintf(stderr, "test_send_request_success: Expected ret=0, got %d\n", ret);
        ok = false;
    }
    if (ebuf[0] != '\0') {
        fprintf(stderr, "test_send_request_success: Expected empty ebuf, got '%s'\n", ebuf);
        ok = false;
    }
    return ok;
}

// Test 2: Failure path when putmsg fails
// - Ensures send_request returns -1
// - Ensures ebuf contains expected formatted error message including the what string and errno
bool test_send_request_failure()
{
    // Arrange
    set_putmsg_mode(1); // Configure mock to fail
    const int fd = 7;
    const char payload[] = "bad-payload";
    const int len = (int)strlen(payload);
    const char *what = "failure-case";
    char ebuf[256];
    init_ebuf(ebuf, sizeof(ebuf));

    // Act
    int ret = send_request(fd, (char *)payload, len, (char *)what, ebuf);

    // Assert
    bool ok = true;
    if (ret != -1) {
        fprintf(stderr, "test_send_request_failure: Expected ret=-1, got %d\n", ret);
        ok = false;
    }
    // The error formatter prefixes with errno and composes the message.
    // We expect something like: "errno=..." and "send_request: putmsg "failure-case""
    if (strstr(ebuf, "send_request: putmsg") == NULL) {
        fprintf(stderr, "test_send_request_failure: ebuf missing expected phrase: %s\n", ebuf);
        ok = false;
    }
    if (strstr(ebuf, what) == NULL) {
        fprintf(stderr, "test_send_request_failure: ebuf missing what string '%s': %s\n", what, ebuf);
        ok = false;
    }
    if (strstr(ebuf, "errno=") == NULL) {
        fprintf(stderr, "test_send_request_failure: ebuf missing errno information: %s\n", ebuf);
        ok = false;
    }
    return ok;
}

// Test 3: Edge case - zero-length payload and non-null what
// - Ensures function behaves correctly if len == 0
bool test_send_request_zero_len()
{
    // Arrange
    set_putmsg_mode(0); // succeed
    const int fd = 99;
    const char payload[] = "";
    const int len = 0;
    const char *what = "zero-len";
    char ebuf[256];
    init_ebuf(ebuf, sizeof(ebuf));

    // Act
    int ret = send_request(fd, (char *)payload, len, (char *)what, ebuf);

    // Assert
    bool ok = true;
    if (ret != 0) {
        fprintf(stderr, "test_send_request_zero_len: Expected ret=0, got %d\n", ret);
        ok = false;
    }
    if (ebuf[0] != '\0') {
        fprintf(stderr, "test_send_request_zero_len: Expected empty ebuf, got '%s'\n", ebuf);
        ok = false;
    }
    return ok;
}

// Main test runner
int main(void)
{
    int total = 0;
    int passed = 0;

    // Run test 1
    bool t1 = test_send_request_success();
    total++;
    if (t1) {
        passed++;
        printf("PASS: test_send_request_success\n");
    } else {
        printf("FAIL: test_send_request_success\n");
    }

    // Run test 2
    bool t2 = test_send_request_failure();
    total++;
    if (t2) {
        passed++;
        printf("PASS: test_send_request_failure\n");
    } else {
        printf("FAIL: test_send_request_failure\n");
    }

    // Run test 3
    bool t3 = test_send_request_zero_len();
    total++;
    if (t3) {
        passed++;
        printf("PASS: test_send_request_zero_len\n");
    } else {
        printf("FAIL: test_send_request_zero_len\n");
    }

    printf("Tests run: %d, Passed: %d, Failed: %d\n", total, passed, total - passed);
    return (passed == total) ? 0 : 1;
}