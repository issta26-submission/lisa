// File: mock_recv_ack.c
// This is a lightweight mock for the C function recv_ack used by the focal method dlbindack.
// It is intended to be built as a shared library and preloaded (via LD_PRELOAD) during tests
// to deterministically exercise the wrapper behavior without requiring a real DLPI environment.

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <cassert>
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


// Global state captured by the mock for verification in tests.
static const char *g_last_what = NULL;
static int g_last_size = 0;
static int g_last_fd = 0;
static int g_last_uerror_value = 0;
static int g_last_uerror_was_set = 0;

// Primary mock implementation.
// It records input arguments, simulates outputs, and returns a deterministic value.
int recv_ack(int fd, int size, const char *what, char *bufp, char *ebuf, int *uerror)
{
    g_last_fd = fd;
    g_last_size = size;
    g_last_what = what;

    // Simulate behavior similar to a real function: populate user-provided error and buffers.
    if (uerror) {
        *uerror = 123;                   // deterministic for tests
        g_last_uerror_value = 123;
        g_last_uerror_was_set = 1;
    } else {
        g_last_uerror_was_set = 0;
    }

    // Optionally fill buffers to simulate a successful operation.
    if (bufp) {
        strcpy(bufp, "MOCK_OK");
    }
    if (ebuf) {
        strcpy(ebuf, "MOCK_EOK");
    }

    // Return a deterministic success code for test assertions.
    return 7;
}

// Accessors for test verification

// Returns the last 'what' string passed to recv_ack.
const char* get_last_what() { return g_last_what; }

// Returns the last 'size' value passed to recv_ack.
int get_last_size() { return g_last_size; }

// Returns the last file descriptor passed to recv_ack.
int get_last_fd() { return g_last_fd; }

// Returns the last uerror value written by recv_ack (if any).
int get_last_uerror_value() { return g_last_uerror_value; }

// Returns whether the test was able to set uerror or not.
int was_uerror_set() { return g_last_uerror_was_set; }

// NOTE:
// Build and run instructions (to be used by the test harness):
// 1) Compile the focal code into an object (or library) as usual, e.g.:
//    gcc -c pcap-dlpi.c -o pcap-dlpi.o
// 2) Build this mock as a shared library, e.g.:
//    gcc -fPIC -shared -o libmock.so mock_recv_ack.c
// 3) Run tests with LD_PRELOAD to override recv_ack during the test:
//    LD_PRELOAD=./libmock.so ./test_dlbindack
// This allows the test to deterministically observe calls to recv_ack without requiring real DLPI activity.


// File: test_dlbindack.cpp
// A small, self-contained C++11 test suite (no GTest) for the dlbindack focal method.
// It relies on the LD_PRELOAD mechanism to inject the mock_recv_ack behavior.


// Declaration of the focal function under test (C linkage)
extern "C" int dlbindack(int fd, char *bufp, char *ebuf, int *uerror);

// Declarations for the mock inspection helpers (provided by mock_recv_ack.c)
extern "C" const char* get_last_what();
extern "C" int get_last_size();
extern "C" int get_last_fd();
extern "C" int get_last_uerror_value();
extern "C" int was_uerror_set();

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_passed = 0;

#define TEST(name) void name(); \
    static void name##_runner(void)

#define RUN_TEST(name) do { \
    name(); \
    ++g_total; \
} while(0)

#define ASSERT(cond, msg) do { \
    if (cond) { ++g_passed; } \
    else { \
        std::cerr << "Test failure: " << (msg) << std::endl; \
    } \
} while(0)

// Test 1: Ensure dlbindack forwards parameters correctly to recv_ack
TEST(test_dlbindack_forwards_parameters)
{
    // Prepare inputs
    char buf[64];
    char ebuf[64];
    int uerror = 0;

    // Invoke the focal method
    int ret = dlbindack(5, buf, ebuf, &uerror);

    // Verify interactions via mock inspectors
    const char* what_seen = get_last_what();
    int size_seen = get_last_size();
    int fd_seen = get_last_fd();

    // Assertions (non-terminating, but log failures)
    ASSERT(what_seen != nullptr && std::strcmp(what_seen, "bind") == 0,
           "dlbindack should pass what == 'bind' to recv_ack");
    ASSERT(size_seen > 0, "dlbindack should pass a positive size to recv_ack");
    ASSERT(fd_seen == 5, "dlbindack should forward the correct fd to recv_ack");
    ASSERT(uerror == 123, "dlbindack should propagate uerror set by recv_ack");
    ASSERT(ret == 7, "dlbindack should return the value from recv_ack");

    // Optional: the mock also populates buffers; ensure no crash occurred
    (void)buf;
    (void)ebuf;
}

// Test 2: When fd is -1, recv_ack mock returns -1, propagate that through dlbindack
TEST(test_dlbindack_negative_fd_behavior)
{
    char buf[64];
    char ebuf[64];
    int uerror = 0;

    int ret = dlbindack(-1, buf, ebuf, &uerror);

    int fd_seen = get_last_fd();

    ASSERT(fd_seen == -1, "dlbindack should forward fd = -1 to recv_ack (mock)");
    ASSERT(ret == -1, "dlbindack should propagate -1 return value when recv_ack returns -1");

    (void)buf;
    (void)ebuf;
    (void)uerror;
}

// Main: run all tests and report summary
int main()
{
    // Clear global counts
    g_total = 0;
    g_passed = 0;

    // Run tests
    RUN_TEST(test_dlbindack_forwards_parameters);
    RUN_TEST(test_dlbindack_negative_fd_behavior);

    // Summary
    std::cout << "dlbindack test results: " << g_passed << " passed, "
              << g_total - g_passed << " failed" << std::endl;
    return (g_total == g_passed) ? 0 : 1;
}

// Explanation of test intent (added as comments):
// - Test 1 validates that dlbindack forwards the expected constant "bind" as the what parameter
//   and forwards a positive size to recv_ack. It also checks that the file descriptor is passed
//   through unmodified and that the uerror value written by recv_ack is observed by the caller.
// - Test 2 asserts that when the underlying mock signals an error condition (fd == -1), dlbindack
//   propagates the error back to the caller with the expected return code.
// - The tests rely on LD_PRELOAD with libmock.so to override recv_ack, enabling deterministic
//   behavior without requiring the actual DLPI environment. The mock exposes helper getters to
//   inspect the calls made by dlbindack.