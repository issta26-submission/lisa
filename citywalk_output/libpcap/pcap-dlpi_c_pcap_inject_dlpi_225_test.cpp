// Test suite for pcap_inject_dlpi (C function) using a custom lightweight harness.
// This test is designed to exercise the DLIOCRAW and DL_HP_RAWDLS code paths as well as
// the "not supported" path. It does not rely on Google Test; it uses a simple, non-terminating
// assertion scheme and prints a summary at the end.
//
// Build notes:
// - To test the DLIOCRAW path (the write() based path), compile with -DDLIOCRAW
// - To test the DL_HP_RAWDLS path (the raw DLPI path), compile with -DDL_HP_RAWDLS
// - To test the "not supported" path, compile with neither DLIOCRAW nor DL_HP_RAWDLS
// - The test provides minimal stubs for dependencies (pcapint_fmt_errmsg_for_errno and
//   pcapint_strlcpy) to allow linking without the full pcap runtime present in the
//   environment. The stubs intentionally keep error messages deterministic for tests.
// - The real project may have a fuller environment; this harness focuses on exercising
//   the control-flow branches of the focal method.

// Include standard headers
#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <fcntl.h>
#include <vector>
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


// Lightweight redefinitions to mirror the minimal subset used by pcap_inject_dlpi.
// We assume the project uses PCAP_ERRBUF_SIZE in its public API; the test uses a fixed
// buffer to avoid depending on the exact value in the external project.
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

// Forward declare the focal function the test will invoke.
// The real project provides pcap_t; for the purpose of this harness, we declare a minimal
// compatible structure locally so we can construct instances used by the test code.
extern "C" {
    // The focal function under test
    int pcap_inject_dlpi(struct pcap_t *p, const void *buf, int size);

    // Stubbed external dependencies expected by the focal function's implementation
    // (the real project has these in the runtime; we provide simple deterministic stubs here).
    void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbufsize, int errno_val, const char *what);
    size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize);
}

// Minimal local definitions to simulate the project's pcap_t and related structs.
// We purposefully keep the layout minimal and predictable to allow tests to manipulate the
// structure in a straightforward way. If the real project uses a different layout, you may
// need to adapt accordingly when integrating into your environment.

struct pcap_dlpi {
    int send_fd; // used by DL_HP_RAWDLS path
};

// The test-visible pcap_t layout. This mirrors the fields used by pcap_inject_dlpi in tests.
struct pcap_t {
    int fd;                          // file descriptor used by DLIOCRAW path
    char errbuf[PCAP_ERRBUF_SIZE];   // error buffer (we keep size ample for tests)
    void *priv;                      // opaque pointer used by DL_HP_RAWDLS path
};

// Global flag to control simulated behavior of the dlrawdatareq helper.
static int simulate_dlraw_success = 0;

// Forward declare a very small wrapper for pcap_inject_dlpi that uses our test layout.
// We assume the real function accepts a pointer to a compatible pcap_t type.
// The object memory layout is crafted to be compatible for testing purposes in this harness.
extern "C" int pcap_inject_dlpi(struct pcap_t *p, const void *buf, int size);

// Stub implementations to satisfy linker for dependencies used by the focal function.
// These are intentionally simple and deterministic to support testing.

extern "C" void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbufsize, int errno_val, const char *what) {
    // Example deterministic error message to allow string assertions in tests.
    // We mimic the expected pattern: "<what>: errno <errno_val>"
    if (errbuf == NULL || errbufsize == 0) return;
    const char *fmt = "%s: errno %d";
    int written = snprintf(errbuf, errbufsize, fmt, what, errno_val);
    (void)written;
}

// Safe string copy helper mirroring the real project's behavior in a simplified form.
extern "C" size_t pcapint_strlcpy(char *dst, const char *src, size_t dstsize) {
    if (dstsize == 0) return strlen(src);
    size_t len = strlen(src);
    if (len >= dstsize) len = dstsize - 1;
    memcpy(dst, src, len);
    dst[len] = '\0';
    return len;
}

// Stub for the DLPI raw data request call; this is where we can simulate success/failure.
extern "C" int dlrawdatareq(int fd, const void *buf, int size) {
    (void)fd; (void)buf;
    if (simulate_dlraw_success) {
        return size; // pretend all bytes were written
    }
    errno = EIO;
    return -1;
}

// Helper: reset the test environment before each test
static void reset_test_env(struct pcap_t &p) {
    p.fd = -1;
    p.priv = nullptr;
    std::memset(p.errbuf, 0, PCAP_ERRBUF_SIZE);
}

// Helper: check condition and accumulate errors without terminating the test program.
struct TestResult {
    bool ok;
    std::string message;
    TestResult(bool o, const std::string &m) : ok(o), message(m) {}
};

static void log_result(const TestResult &r, int &fail_count) {
    if (r.ok) {
        std::cout << "[OK] " << r.message << "\n";
    } else {
        ++fail_count;
        std::cout << "[FAIL] " << r.message << "\n";
    }
}

// Test: DLIOCRAW success path
static TestResult test_inject_dlpi_dlraw_success() {
#ifdef DLIOCRAW
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return TestResult(false, "pipe() failed to create test pipe");
    }

    struct pcap_t p;
    reset_test_env(p);
    p.fd = pipefd[1]; // write-end of the pipe
    // errbuf is large enough (PCAP_ERRBUF_SIZE); we won't touch it in this path if successful.

    const char *payload = "HELLO";
    int size = (int)std::strlen(payload);

    int ret = pcap_inject_dlpi(&p, payload, size);

    // Read exactly 'size' bytes from the read end; ensure they match
    char readbuf[256];
    std::memset(readbuf, 0, sizeof(readbuf));
    ssize_t r = read(pipefd[0], readbuf, size);
    // Close descriptors
    close(pipefd[0]);
    close(pipefd[1]);

    if (ret != size) {
        return TestResult(false, "pcap_inject_dlpi did not return expected size on DLIOCRAW success path");
    }
    if (r != size || std::strncmp(payload, readbuf, size) != 0) {
        return TestResult(false, "Written bytes do not match payload on DLIOCRAW success path");
    }

    return TestResult(true, "DLIOCRAW success path exercised correctly");
#else
    return TestResult(true, "DLIOCRAW not enabled in this build; skipping test");
#endif
}

// Test: DLIOCRAW failure path (write returns -1)
static TestResult test_inject_dlpi_dlraw_failure() {
#ifdef DLIOCRAW
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        return TestResult(false, "pipe() failed to create test pipe");
    }

    struct pcap_t p;
    reset_test_env(p);
    p.fd = -1; // force write() to fail (EBADF)
    // place a dummy error
    std::strcpy(p.errbuf, "");

    const char *payload = "DATA";
    int size = 4;

    int ret = pcap_inject_dlpi(&p, payload, size);

    close(pipefd[0]);
    close(pipefd[1]);

    if (ret != -1) {
        return TestResult(false, "pcap_inject_dlpi should fail when write() is not possible");
    }
    // The error message should contain "send" per original code's semantics
    if (std::strstr(p.errbuf, "send") == nullptr) {
        return TestResult(false, "pcap_inject_dlpi did not populate error with expected 'send' text");
    }
    return TestResult(true, "DLIOCRAW failure path behaves as expected");
#else
    return TestResult(true, "DLIOCRAW not enabled in this build; skipping test");
#endif
}

// Test: DL_HP_RAWDLS success path (mocked dlrawdatareq to return size)
static TestResult test_inject_dlpi_dlraw_raw_success() {
#ifdef DL_HP_RAWDLS
    // Prepare pcap_t with a valid 'priv' structure
    struct pcap_t p;
    reset_test_env(p);
    struct pcap_dlpi pd;
    pd.send_fd = 0; // non-negative to satisfy "send_fd < 0" check
    p.priv = &pd;

    // Ensure our simulate flag yields a successful dlrawdatareq
    simulate_dlraw_success = 1;

    const char payload[] = "RAWOK";
    int size = (int)sizeof(payload) - 1;

    int ret = pcap_inject_dlpi(&p, payload, size);

    if (ret != size) {
        return TestResult(false, "pcap_inject_dlpi did not return size on DL_HP_RAWDLS success path");
    }
    // If we had access to the DLPI path internals, we could verify output; here we
    // only verify return value correctness as a proxy for path success.
    return TestResult(true, "DL_HP_RAWDLS success path exercised (simulated)");
#else
    return TestResult(true, "DL_HP_RAWDLS not enabled in this build; skipping test");
#endif
}

// Test: DL_HP_RAWDLS failure path (send_fd < 0)
static TestResult test_inject_dlpi_dlraw_raw_bad_fd() {
#ifdef DL_HP_RAWDLS
    struct pcap_t p;
    reset_test_env(p);
    struct pcap_dlpi pd;
    pd.send_fd = -1; // triggers open error path
    p.priv = &pd;

    const char payload[] = "X";
    int size = 1;

    int ret = pcap_inject_dlpi(&p, payload, size);

    if (ret != -1) {
        return TestResult(false, "pcap_inject_dlpi should fail when send_fd is negative in DL_HP_RAWDLS");
    }
    if (std::strstr(p.errbuf, "Output FD couldn't be opened") == nullptr) {
        return TestResult(false, "DL_HP_RAWDLS error path did not populate expected message");
    }
    return TestResult(true, "DL_HP_RAWDLS failure path (negative send_fd) behaved correctly");
#else
    return TestResult(true, "DL_HP_RAWDLS not enabled in this build; skipping test");
#endif
}

// Test: Not supported path (neither DLIOCRAW nor DL_HP_RAWDLS defined)
static TestResult test_inject_dlpi_not_supported() {
#if !defined(DLIOCRAW) && !defined(DL_HP_RAWDLS)
    struct pcap_t p;
    reset_test_env(p);

    const char payload[] = "X";
    int size = 1;

    int ret = pcap_inject_dlpi(&p, payload, size);

    if (ret != -1) {
        return TestResult(false, "pcap_inject_dlpi should return -1 for not-supported path");
    }
    if (std::strstr(p.errbuf, "send: Not supported") == nullptr) {
        // The exact string may vary; check that a "Not supported" phrase exists.
        return TestResult(false, "Not-supported path did not populate expected error text");
    }
    return TestResult(true, "Not-supported path handled correctly");
#else
    return TestResult(true, "Not-supported path not applicable in this build; skipping test");
#endif
}

// Simple test runner
int main() {
    std::vector<TestResult> results;

    // Execute tests; each test reports pass/fail with a descriptive message.
    results.push_back(test_inject_dlpi_dlraw_success());
    results.push_back(test_inject_dlpi_dlraw_failure());
    results.push_back(test_inject_dlpi_dlraw_raw_success());
    results.push_back(test_inject_dlpi_dlraw_raw_bad_fd());
    results.push_back(test_inject_dlpi_not_supported());

    // Summary
    int passed = 0;
    int failed = 0;
    for (const auto &r : results) {
        if (r.ok) ++passed;
        else ++failed;
    }

    std::cout << "\nTest Summary: " << passed << " passed, " << failed << " failed, "
              << (passed + failed) << " total tests." << std::endl;

    // Return code indicates if any test failed
    return failed ? 1 : 0;
}