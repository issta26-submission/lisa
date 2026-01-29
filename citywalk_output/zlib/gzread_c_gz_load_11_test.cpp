// Unit test suite for the focal method gz_load (replicated for testability in C++11).
// This test harness is designed to be compiled with a C++11 compiler without GoogleTest.
// It creates a minimal environment that mimics the expected structures and dependencies
// (gz_statep, gz_error, zstrerror) so that gz_load can be exercised.
// The tests are written to maximize coverage by exercising success, EOF, and error paths
// using POSIX pipes as the underlying "read" source.

#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <vector>
#include <gzguts.h>
#include <unistd.h>
#include <sys/types.h>



// Domain knowledge aligned helpers and minimal dependencies to enable testing
// of the focal method logic in a standalone environment.

// Simulated error code (as in zlib/gzguts usage)
static const int Z_ERRNO_CONST = 1; // small stand-in for Z_ERRNO

// Forward-declare state structure and pointer type used by gz_load.
struct gz_state {
    int fd;
    int eof;
};
typedef struct gz_state* gz_statep;

// Tiny, test-oriented stubs for dependencies used inside gz_load.
static int g_error_called = 0;
static const char* g_last_error_msg = nullptr;
static int g_last_errno = 0;

// Mocked error handling used by gz_load when an error occurs.
void gz_error(gz_statep state, int err, const char* msg) {
    (void)state; // not used in test stub, but present to resemble original signature
    (void)err;
    g_error_called = 1;
    g_last_error_msg = msg;
    g_last_errno = err;
}

// Mocked function returning a pseudo-error string (as in zstrerror()).
const char* zstrerror() {
    return "mock_error";
}

// Replicated focal method (made non-static for testability in C++).
// This mirrors the exact logic shown in the provided snippet, adapted to the test harness.
int gz_load(gz_statep state, unsigned char *buf, unsigned len, unsigned *have) {
    int ret;
    unsigned get, max = ((unsigned)-1 >> 2) + 1;
    *have = 0;
    do {
        get = len - *have;
        if (get > max)
            get = max;
        ret = read(state->fd, buf + *have, get);
        if (ret <= 0)
            break;
        *have += (unsigned)ret;
    } while (*have < len);

    if (ret < 0) {
        gz_error(state, Z_ERRNO_CONST, zstrerror());
        return -1;
    }
    if (ret == 0)
        state->eof = 1;
    return 0;
}

// Simple test assertion macro: non-terminating (logs failure but continues).
static int g_total_tests = 0;
static int g_passed = 0;
static int g_failed = 0;

#define EXPECT(cond, desc) do { \
    ++g_total_tests; \
    if (cond) { \
        ++g_passed; \
        std::cout << "[PASS] " << desc << std::endl; \
    } else { \
        ++g_failed; \
        std::cerr << "[FAIL] " << desc << std::endl; \
    } \
} while(0)


// Test 1: gz_load should read exactly len bytes when data is available in the source.
// Verifies correct buffer fill, return code 0, and eof remains unset.
void test_gz_load_full_read() {
    const unsigned len = 16;
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test_gz_load_full_read" << std::endl;
        return;
    }

    // Prepare deterministic data to be read.
    unsigned char expected[len];
    for (unsigned i = 0; i < len; ++i) expected[i] = static_cast<unsigned char>(i);

    // Write len bytes into the pipe.
    ssize_t w = write(pipefd[1], expected, len);
    (void)w;

    gz_state state;
    state.fd = pipefd[0];
    state.eof = 0;

    unsigned have = 0;
    unsigned char buf[len];
    int ret = gz_load(&state, buf, len, &have);

    bool ok = (ret == 0) && (have == len) && (state.eof == 0) && (std::memcmp(buf, expected, len) == 0);
    close(pipefd[0]);
    close(pipefd[1]);

    EXPECT(ok, "test_gz_load_full_read: read full len bytes, correct data, ret==0, eof==0");
    // If needed, print additional diagnostic on failure (handled by EXPECT macro).
}

// Test 2: gz_load should set eof when the source returns 0 bytes (EOF) on the first read.
// This is achieved by closing the write end of a pipe before invoking gz_load.
void test_gz_load_eof_on_first_read() {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test_gz_load_eof_on_first_read" << std::endl;
        return;
    }

    // Close the write end to trigger EOF on first read.
    close(pipefd[1]);

    gz_state state;
    state.fd = pipefd[0];
    state.eof = 0;

    const unsigned len = 8;
    unsigned have = 0;
    unsigned char buf[len];
    int ret = gz_load(&state, buf, len, &have);

    bool ok = (ret == 0) && (have == 0) && (state.eof == 1);
    close(pipefd[0]);

    EXPECT(ok, "test_gz_load_eof_on_first_read: returns 0, have==0, eof==1 when source is at EOF immediately");
}

// Test 3: gz_load should return -1 and invoke gz_error on read failure (bad fd).
void test_gz_load_error_path() {
    gz_state state;
    state.fd = -1; // invalid fd to force error
    state.eof = 0;

    const unsigned len = 8;
    unsigned have = 0;
    unsigned char buf[len];
    int ret = gz_load(&state, buf, len, &have);

    bool ok = (ret == -1) && (g_error_called != 0);
    // Reset error state for potential further tests
    g_error_called = 0;
    g_last_error_msg = nullptr;
    g_last_errno = 0;

    EXPECT(ok, "test_gz_load_error_path: returns -1 and gz_error invoked on invalid fd");
}

// Test 4: gz_load should be able to handle multiple reads to accumulate len bytes.
// This test uses a pipe populated with len bytes; the OS may deliver data in multiple reads.
// We verify that the function completes with have == len and a non-error return.
void test_gz_load_multiple_reads() {
    const unsigned len = 24;
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test_gz_load_multiple_reads" << std::endl;
        return;
    }

    // Prepare deterministic data
    unsigned char data[len];
    for (unsigned i = 0; i < len; ++i) data[i] = static_cast<unsigned char>((i * 7) & 0xFF);

    // Write all data into the pipe
    ssize_t w = write(pipefd[1], data, len);
    (void)w;

    gz_state state;
    state.fd = pipefd[0];
    state.eof = 0;

    unsigned have = 0;
    unsigned char buf[len];
    int ret = gz_load(&state, buf, len, &have);

    bool ok = (have == len) && (ret >= 0) && (state.eof == 0);
    close(pipefd[0]);
    close(pipefd[1]);

    EXPECT(ok, "test_gz_load_multiple_reads: accumulates len bytes across potential multiple reads, ret>=0");
}

// Entry point: run all tests and print a summary.
// The harness uses non-terminating EXPECT checks to maximize code execution coverage.
int main() {
    // Reset global test counters
    g_total_tests = 0;
    g_passed = 0;
    g_failed = 0;
    g_error_called = 0;
    g_last_error_msg = nullptr;
    g_last_errno = 0;

    std::cout << "Starting gz_load unit tests (C++11, no GTest)..." << std::endl;

    // Run all tests
    test_gz_load_full_read();
    test_gz_load_eof_on_first_read();
    test_gz_load_error_path();
    test_gz_load_multiple_reads();

    // Summary
    std::cout << "Test summary: Total=" << g_total_tests
              << " Passed=" << g_passed
              << " Failed=" << g_failed << std::endl;

    // Exit code indicates failures (0 for success, non-zero for some failures)
    return (g_failed == 0) ? 0 : 1;
}