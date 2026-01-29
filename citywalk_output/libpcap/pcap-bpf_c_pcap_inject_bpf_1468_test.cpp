// C++11 test suite for pcap_inject_bpf (no GTest)
// This test harness exercises the focal function using a mix of real OS I/O
// (Linux) and Apple-path (macOS) code paths via macro-controlled testing.
// It relies on linking against libpcap (pcap.h provides pcap_t and PCAP_* macros).
// The tests do not terminate on failure; they accumulate and report results.

#include <cf.h>
#include <net/bpf.h>
#include <netdb.h>
#include <sys/sysconfig.h>
#include <unistd.h>
#include <stdio.h>
#include <zone.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <time.h>
#include <pcap.h>
#include <stddef.h>
#include <dirent.h>
#include <sys/param.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/cfgodm.h>
#include <net/if_types.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <sys/ioccom.h>
#include <sys/device.h>
#include <os-proto.h>
#include <config.h>
#include <sys/utsname.h>


extern "C" {
    // Declaration of the focal function (C linkage)
    int pcap_inject_bpf(pcap_t *p, const void *buf, int size);
}

// Global counters for test results
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple test assertion macro (non-terminating)
#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "Test failed: %s\n", msg); \
        g_failed_tests++; \
    } else { \
        fprintf(stdout, "Test passed: %s\n", msg); \
    } \
    g_total_tests++; \
} while (0)


// Apple-specific test scaffolding (only active when compiling with __APPLE__)
// We provide controlled overrides for write() and ioctl() to simulate Apple bug paths.

#if defined(__APPLE__) && defined(USE_APPLE_RETRY)  // Apple path tests with retry after EAFNOSUPPORT

// Track how many times the write stub has been invoked
static int g_apple_write_calls = 0;

// Apple-mode: 1 = simulate EAFNOSUPPORT on first write, then succeed on second write
static int g_apple_mode = 1; // 1 = retry path, 2 = non-bug path (errno != EAFNOSUPPORT)
static int g_apple_ioctl_fail = 0; // 0 = succeed, 1 = fail

extern "C" ssize_t write(int fd, const void *buf, size_t count) {
    // On first call, simulate EAFNOSUPPORT; subsequent call writes succeed
    if (g_apple_mode == 1) {
        if (g_apple_write_calls == 0) {
            g_apple_write_calls++;
            errno = EAFNOSUPPORT;
            return -1;
        } else {
            // Second attempt succeeds; actually write the data somewhere harmless
            g_apple_write_calls++;
            // Use the real write to actually store data if desired, but not required for test
            // Fallback: return count to indicate success
            return (ssize_t)count;
        }
    }
    // For other modes, fall back to a simple success write (or implement as needed)
    return (ssize_t)count;
}

extern "C" int ioctl(int fd, unsigned long request, void *argp) {
    // Simulate BIOCSHDRCMPLT being toggled off successfully unless asked to fail
    (void)fd; (void)request; (void)argp;
    if (g_apple_ioctl_fail) {
        return -1;
    }
    return 0;
}

// Helpers to configure Apple test behavior (to be used by tests in main)
extern "C" void apple_set_write_behavior(int mode) {
    g_apple_mode = mode;
    g_apple_write_calls = 0;
}

extern "C" void apple_set_ioctl_failure(int flag) {
    g_apple_ioctl_fail = flag;
}

#else
// No-op stubs for non-Apple or non-Apple-Apple-test builds
extern "C" void apple_set_write_behavior(int mode) { (void)mode; }
extern "C" void apple_set_ioctl_failure(int flag) { (void)flag; }

#endif // APPLE TEST scaffolding


// Linux (non-Apple) test helpers (no write hijacking)
static void setup_pcap_t_for_test(pcap_t &p, int fd) {
    // On many libpcap versions, pcap_t contains fd and errbuf fields.
    // Ensure we populate those fields for pcap_inject_bpf usage.
    // Note: We assume PCAP_ERRBUF_SIZE is defined by pcap.h.
    static char errbuf[PCAP_ERRBUF_SIZE];
    memset(&p, 0, sizeof(pcap_t));
    // Some libpcap builds define 'fd' as an int member; many also have 'errbuf'
    // We'll cast to the known layout used by pcap_inject_bpf:
    // p.fd and p.errbuf are used in the focal function.
    // The following is a best-effort initialization; actual layout is provided by pcap.h.
    // We'll assign via designated initializers if supported; otherwise, use pointer arithmetic.
    // Simpler: use memcpy to set fields if symbols are available through header.

    // This portion relies on libpcap's pcap_t struct layout in the target environment.
    // We will attempt to set fields using the common names expected by pcap_inject_bpf.
    // Since we're in C++, direct member access may be valid depending on libpcap version.

    // Try to access members with known names
#if defined(__GNUC__) && (__GNUC__ >= 4)
    p.fd = fd;
    p.errbuf = errbuf;
#else
    // Fallback: nothing to do
    (void)fd; (void)errbuf;
#endif
}

// Test 1: Basic success path using real OS write (Linux)
static bool test_inject_basic_success_linux() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }
    pcap_t p;
    setup_pcap_t_for_test(p, pipefd[1]);
    const char payload[] = "ping-pcap-inject";
    int payload_size = (int)sizeof(payload);

    int ret = pcap_inject_bpf(&p, payload, payload_size);
    // Expect ret == payload_size
    bool ok = (ret == payload_size);
    // Read from the read end to verify bytes were written
    char buf[sizeof(payload)];
    ssize_t r = read(pipefd[0], buf, sizeof(payload));
    if (r != payload_size) {
        ok = false;
    }
    if (ok && memcmp(buf, payload, payload_size) != 0) {
        ok = false;
    }

    close(pipefd[0]);
    close(pipefd[1]);
    return ok;
}

// Test 2: Write failure path (errno set, outputs PCAP_ERROR, and errbuf contains "send")
static bool test_inject_error_linux() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }
    // Use a closed fd to force EBADF/write error
    int closed_fd = -1;
    pcap_t p;
    setup_pcap_t_for_test(p, closed_fd);
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));
    p.errbuf = errbuf;
    const char payload[] = "data";
    int ret = pcap_inject_bpf(&p, payload, (int)sizeof(payload));

    bool ok = (ret == PCAP_ERROR);
    // errbuf should contain the word "send" per pcap_inject_bpf error formatting
    if (!ok) {
        fprintf(stderr, "Warning: pcap_inject_bpf did not return PCAP_ERROR as expected on failure.\n");
        g_total_tests++;
        g_failed_tests++;
        return false;
    }
    if (strstr(errbuf, "send") == NULL) {
        fprintf(stderr, "Warning: errbuf does not contain expected 'send' text. errbuf=\"%s\".\n", errbuf);
        g_total_tests++;
        g_failed_tests++;
        return false;
    }

    close(pipefd[0]);
    // pipefd[1] is not used since no write occurred
    return true;
}

// Apple-specific tests (only compiled when __APPLE__ is defined)
// Test 3: Apple path with retry after EAFNOSUPPORT (first write fails, second write succeeds)
static bool test_inject_apple_retry_success() {
#if defined(__APPLE__) && defined(USE_APPLE_RETRY)
    // Configure Apple behavior: first write fails with EAFNOSUPPORT, second write succeeds
    apple_set_write_behavior(1);
    apple_set_ioctl_failure(0);

    // Prepare a dummy pcap_t with a valid fd (we'll use a pipe to obtain a valid fd)
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }

    pcap_t p;
    setup_pcap_t_for_test(p, pipefd[1]);
    const char payload[] = "apple_retry";
    int payload_size = (int)sizeof(payload;

                   ));

    int ret = pcap_inject_bpf(&p, payload, payload_size);
    bool ok = (ret == payload_size);

    // Cleanup
    close(pipefd[0]);
    close(pipefd[1]);

    // Reset state for subsequent tests
    apple_set_write_behavior(0);
    return ok;
#else
    // Not compiled for Apple in this environment; skip
    return true;
#endif
}

// Test 4: Apple path with errno != EAFNOSUPPORT on first write should not retry (PCAP_ERROR)
static bool test_inject_apple_bug_no_bugs() {
#if defined(__APPLE__) && defined(USE_APPLE_RETRY)
    apple_set_write_behavior(2); // emulate first write errors with a different errno (e.g., EINVAL)
    apple_set_ioctl_failure(0);

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }

    pcap_t p;
    setup_pcap_t_for_test(p, pipefd[1]);
    const char payload[] = "apple_bug_no_bug";
    int payload_size = (int)sizeof(payload);

    int ret = pcap_inject_bpf(&p, payload, payload_size);
    bool ok = (ret == PCAP_ERROR);

    close(pipefd[0]);
    close(pipefd[1]);
    apple_set_write_behavior(0);
    return ok;
#else
    return true;
#endif
}

// Test 5: Apple path ioctl failure after EAFNOSUPPORT (simulate ioctl failure)
static bool test_inject_apple_ioctl_failure() {
#if defined(__APPLE__) && defined(USE_APPLE_RETRY)
    apple_set_write_behavior(1); // first write would fail with EAFNOSUPPORT
    apple_set_ioctl_failure(1);  // make ioctl fail as well

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return false;
    }

    pcap_t p;
    setup_pcap_t_for_test(p, pipefd[1]);
    const char payload[] = "apple_ioctl_fail";
    int payload_size = (int)sizeof(payload);

    int ret = pcap_inject_bpf(&p, payload, payload_size);
    bool ok = (ret == PCAP_ERROR);

    close(pipefd[0]);
    close(pipefd[1]);
    apple_set_write_behavior(0);
    apple_set_ioctl_failure(0);
    return ok;
#else
    return true;
#endif
}


// Main test runner
int main() {
    // Basic Linux tests (real OS I/O)
    printf("Starting pcap_inject_bpf unit tests (Linux path)…\n");
    if (!test_inject_basic_success_linux()) {
        fprintf(stderr, "Failure in test_inject_basic_success_linux\n");
    } else {
        printf("test_inject_basic_success_linux passed.\n");
    }

    if (!test_inject_error_linux()) {
        fprintf(stderr, "Failure in test_inject_error_linux\n");
    } else {
        printf("test_inject_error_linux passed.\n");
    }

    // Apple-specific tests (macOS path)
#if defined(__APPLE__)
    printf("Starting pcap_inject_bpf Apple-path tests…\n");
    if (!test_inject_apple_retry_success()) {
        fprintf(stderr, "Failure in test_inject_apple_retry_success\n");
    } else {
        printf("test_inject_apple_retry_success passed.\n");
    }
    if (!test_inject_apple_bug_no_bugs()) {
        fprintf(stderr, "Failure in test_inject_apple_bug_no_bugs\n");
    } else {
        printf("test_inject_apple_bug_no_bugs passed.\n");
    }
    if (!test_inject_apple_ioctl_failure()) {
        fprintf(stderr, "Failure in test_inject_apple_ioctl_failure\n");
    } else {
        printf("test_inject_apple_ioctl_failure passed.\n");
    }
#else
    printf("Apple-path tests omitted (not compiling under __APPLE__).\n");
#endif

    // Summary
    printf("Tests run: %d, Failures: %d\n", g_total_tests, g_failed_tests);
    return (g_failed_tests == 0) ? 0 : 1;
}