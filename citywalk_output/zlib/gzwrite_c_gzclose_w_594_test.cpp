// Minimal C++11 test harness for gzclose_w (no GoogleTest, single translation unit)
// This test includes the focal C code gzwrite.c to provide the exact environment
// and relies on a small, controlled test environment to exercise specific branches.
//
// Instructions:
// - This test uses a lightweight, non-terminating assertion approach.
// - The environment must have the corresponding gzwrite.c and its dependencies.
// - The test demonstrates 3 scenarios: NULL input, wrong mode, and close() failure.
// - Explanatory comments are provided for each unit test.
//
// Note: This file is intended to be compiled as C++11 and linked with the gzwrite.c source.
// It also provides a minimal override for the POSIX close() to avoid terminating the test
// process on a controlled FD during tests.

#include <cstring>
#include <errno.h>
#include <stdarg.h>
#include <gzwrite.c>
#include <iostream>
#include <gzguts.h>
#include <cstdlib>


// Forward declarations and needed types/macros come from the focal code.
// Include the gzwrite.c directly to ensure the internal types like gzFile, gz_statep,
// and constants (GZ_WRITE, Z_STREAM_ERROR, Z_OK, Z_ERRNO, etc.) are available.
extern "C" {
}

// Lightweight override for close() to control behavior during tests without impacting
// the rest of the runtime. We provide C linkage to ensure it matches the call site.
extern "C" int close(int fd)
{
    // Test behavior:
    // - If a test passes a negative FD (e.g., -1), simulate EBADF and return -1.
    // - If a test uses a positive FD, simulate a successful close by returning 0.
    if (fd < 0) {
        errno = EBADF;
        return -1;
    }
    // Simulate successful close for any non-negative FD
    return 0;
}

// Utility: a simple test harness (non-terminating, prints per-test results)
static int tests_run = 0;
static int tests_passed = 0;

// Test 1: gzclose_w(nullptr) should return Z_STREAM_ERROR
static bool test_gzclose_w_null_file() {
    int ret = gzclose_w(NULL);
    bool ok = (ret == Z_STREAM_ERROR);
    std::cout << "test_gzclose_w_null_file: " << (ok ? "PASS" : "FAIL")
              << " (ret=" << ret << ")\n";
    return ok;
}

// Test 2: gzclose_w with an invalid mode (state->mode != GZ_WRITE) should return Z_STREAM_ERROR
static bool test_gzclose_w_wrong_mode() {
    // Allocate a fake gz_state object by leveraging the actual type from gzwrite.c
    gz_statep st = (gz_statep)malloc(sizeof(*st));
    if (st == NULL) {
        std::cerr << "test_gzclose_w_wrong_mode: allocation failed\n";
        return false;
    }
    // Ensure all fields start clean
    memset(st, 0, sizeof(*st));

    // Intentionally set an invalid mode (not GZ_WRITE)
    // The actual value of GZ_WRITE is defined in gzwrite.c; using 0 here guarantees mismatch.
    st->mode = 0;

    int ret = gzclose_w((gzFile)st);
    free(st);
    bool ok = (ret == Z_STREAM_ERROR);
    std::cout << "test_gzclose_w_wrong_mode: " << (ok ? "PASS" : "FAIL")
              << " (ret=" << ret << ")\n";
    return ok;
}

// Test 3: gzclose_w should handle close() failure (e.g., fd == -1) and return Z_ERRNO
static bool test_gzclose_w_close_failure() {
    // Prepare a minimal valid-looking state to exercise close() path
    gz_statep st = (gz_statep)malloc(sizeof(*st));
    if (st == NULL) {
        std::cerr << "test_gzclose_w_close_failure: allocation failed\n";
        return false;
    }
    memset(st, 0, sizeof(*st));

    // Configure as a writable state
    st->mode = GZ_WRITE;

    // Ensure we trigger the close() call path by giving an invalid descriptor
    st->fd = -1;          // Our close() override will return -1 for negative fd
    st->path = NULL;
    st->err = Z_OK;
    st->size = 1;         // Non-zero so that the end-path is considered; direct path will avoid deflateEnd
    st->direct = 1;       // Use "direct" mode to bypass the deflate path in gz_comp
    // Ensure no memory free side-effects (in/out NULL is safe for free(NULL))
    st->in = NULL;
    st->out = NULL;
    st->want = 1;

    // The strm field is a z_stream; zeroing is sufficient for the test path we take
    // so we avoid complex compression initialization.
    // We assume gzwrite.c initializes/uses only avail_in/next_in in this scenario.
    memset(&(st->strm), 0, sizeof(st->strm));

    int ret = gzclose_w((gzFile)st);
    free(st);

    bool ok = (ret == Z_ERRNO);
    std::cout << "test_gzclose_w_close_failure: " << (ok ? "PASS" : "FAIL")
              << " (ret=" << ret << ")\n";
    return ok;
}

// Test 4: Optional: gzclose_w should succeed when close() is mocked to succeed
static bool test_gzclose_w_close_success() {
    // Prepare a minimal, safe state where close() will succeed (our close() mock returns 0 for non-negative fds)
    gz_statep st = (gz_statep)malloc(sizeof(*st));
    if (st == NULL) {
        std::cerr << "test_gzclose_w_close_success: allocation failed\n";
        return false;
    }
    memset(st, 0, sizeof(*st));

    st->mode = GZ_WRITE;
    st->fd = 123;           // Valid FD (our close() mock will return 0)
    st->path = NULL;
    st->err = Z_OK;
    st->size = 1;
    st->direct = 1;           // Direct mode to avoid deflate path
    st->in = NULL;
    st->out = NULL;
    st->want = 1;
    memset(&(st->strm), 0, sizeof(st->strm));

    int ret = gzclose_w((gzFile)st);
    free(st);

    // Expect success (Z_OK). The close() mock returns 0 for non-negative fds.
    bool ok = (ret == Z_OK);
    std::cout << "test_gzclose_w_close_success: " << (ok ? "PASS" : "FAIL")
              << " (ret=" << ret << ")\n";
    return ok;
}

int main() {
    // Run tests and accumulate results
    std::cout << "Running gzclose_w unit tests (no GTest).\" << std::endl;

    tests_run = 0;
    tests_passed = 0;

    if (test_gzclose_w_null_file()) ++tests_passed;
    ++tests_run;

    if (test_gzclose_w_wrong_mode()) ++tests_passed;
    ++tests_run;

    if (test_gzclose_w_close_failure()) ++tests_passed;
    ++tests_run;

    if (test_gzclose_w_close_success()) ++tests_passed;
    ++tests_run;

    std::cout << "Summary: " << tests_passed << " / " << tests_run << " tests passed.\n";

    // Return non-zero if any test failed
    return (tests_passed == tests_run) ? 0 : 1;
}