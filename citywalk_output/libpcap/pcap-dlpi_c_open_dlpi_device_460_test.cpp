// OpenDLPI_Device_Test.cpp
// Lightweight unit tests for open_dlpi_device without a full GTest framework.
// - Uses custom non-terminating EXPECT-like helpers to exercise branches.
// - Provides stubs for dependent functions to isolate open_dlpi_device behavior.
// - Designed to be compiled in two configurations (with and without HAVE_DEV_DLPI).
//   Build steps (not shown here) would typically compile this translation unit twice:
//     1) with -DHAVE_DEV_DLPI to test the DLPI path
//     2) without -DHAVE_DEV_DLPI to test the non-DLPI path

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
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


// Declaration of the focal function (as it would be in the production code)
extern "C" int open_dlpi_device(const char *name, unsigned int *ppa, char *errbuf);

// Simple non-terminating assertion helpers
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) == (expected)) { \
        std::cout << "[PASS] " << msg << " | got: " << (actual) << "\n"; \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[FAIL] " << msg << " | expected: " << (expected) \
                  << ", got: " << (actual) << "\n"; \
    } \
} while (0)

#define EXPECT_NEQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        std::cout << "[PASS] " << msg << " | got: " << (actual) << "\n"; \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[FAIL] " << msg << " | value unexpectedly equal to: " \
                  << (expected) << "\n"; \
    } \
} while (0)

#define EXPECT_STR_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) && (expected) && std::strcmp((actual), (expected)) == 0) { \
        std::cout << "[PASS] " << msg << " | got: " << (actual) << "\n"; \
        ++g_tests_passed; \
    } else { \
        std::cerr << "[FAIL] " << msg << " | expected: " << (expected) \
                  << ", got: " << (actual) << "\n"; \
    } \
} while (0)


// Stubbed dependencies for the DLPI path (HAVE_DEV_DLPI defined)
#ifdef HAVE_DEV_DLPI

// Global knobs to drive behavior of stubs
static int g_fake_open_return = -1;
static int g_fake_get_dlpi_ppa_status = -1;
static unsigned int g_fake_ppa_value = 0;

// Provide a stubbed open() to control DLPI opening behavior
extern "C" int open(const char *pathname, int /*flags*/) {
    // First call simulates failure to open the DLPI device
    errno = ENOENT;
    return g_fake_open_return;
}

// Stub get_dlpi_ppa() to control PPA discovery results
extern "C" int get_dlpi_ppa(int /*fd*/, const char * /*dname*/, unsigned int /*unit*/,
                          unsigned int *ppa, char * /*errbuf*/) {
    if (ppa) *ppa = g_fake_ppa_value;
    return g_fake_get_dlpi_ppa_status;
}

// Stub split_dname() to avoid depending on actual parsing logic
extern "C" char *split_dname(char *device, unsigned int *unitp, char * /*ebuf*/) {
    // Pretend we parsed the unit as 0 and return the same device buffer so code can null-terminate it
    if (unitp) *unitp = 0;
    return device;
}

// Stub for error handler to provide a deterministic return value
extern "C" int handle_dev_dlpi_open_error(const char * /*ifname*/, int /*error*/, char * /*errbuf*/) {
    // Return a deterministic error code that tests can check
    return -42;
}

#endif // HAVE_DEV_DLPI



// Stubbed dependencies for the Non-DLPI path (no HAVE_DEV_DLPI)
// These provide minimal behavior to exercise the non-DLPI branch without touching real devices.
#ifndef HAVE_DEV_DLPI

static int g_open_call_count = 0;
static int g_open_ret0 = -1; // first open return
static int g_open_ret1 = -1; // second open return

// Helper to configure the two-open sequence
extern "C" void configure_non_dlpi_opens(int first, int second) {
    g_open_ret0 = first;
    g_open_ret1 = second;
}

// Stub open() to mimic path: first open attempts fail with ENOENT, second may succeed
extern "C" int open(const char *pathname, int /*flags*/) {
    ++g_open_call_count;
    if (g_open_call_count == 1) {
        errno = ENOENT;
        return g_open_ret0;
    } else {
        errno = 0;
        return g_open_ret1;
    }
}

// Stub split_dname() required by non-DLPI path
extern "C" char *split_dname(char *device, unsigned int *unitp, char * /*ebuf*/) {
    if (unitp) *unitp = 0;
    return device;
}

// Stub error handler for non-DLPI path
extern "C" int handle_dlpi_device_open_error(const char * /*ifname*/, const char * /*device*/,
                                           int /*error*/, char * /*errbuf*/) {
    return -99;
}

#endif // !HAVE_DEV_DLPI



// Test entry points for the two configurations

static void test_dlpi_failure_open_path() {
    // Purpose: verify that when opening /dev/dlpi fails (open returns -1),
    // the function returns the sentinel value from handle_dev_dlpi_open_error.
#ifdef HAVE_DEV_DLPI
    g_fake_open_return = -1;      // simulate open() failure
    g_fake_get_dlpi_ppa_status = 0;
    g_fake_ppa_value = 0;

    char errbuf[256] = {0};
    unsigned int ppa = 123;
    int ret = open_dlpi_device("eth0", &ppa, errbuf);

    // Expect the sentinel error code from our handle_dev_dlpi_open_error stub
    EXPECT_EQ(static_cast<int>(ret), -42, "DLPI path: open failure should return sentinel error code");
#else
    std::cout << "[SKIP] DLPI failure test requires HAVE_DEV_DLPI defined." << std::endl;
#endif
}

static void test_dlpi_success_path() {
#ifdef HAVE_DEV_DLPI
    g_fake_open_return = 3;       // simulate a valid fd being opened
    g_fake_get_dlpi_ppa_status = 0;
    g_fake_ppa_value = 7;

    char errbuf[256] = {0};
    unsigned int ppa = 0;
    int ret = open_dlpi_device("eth0", &ppa, errbuf);

    EXPECT_EQ(static_cast<unsigned int>(ret), 3u, "DLPI path: success should return fd from open()");
    EXPECT_EQ(ppa, 7u, "DLPI path: ppa should be set by get_dlpi_ppa()");
#else
    std::cout << "[SKIP] DLPI success test requires HAVE_DEV_DLPI defined." << std::endl;
#endif
}

#ifndef HAVE_DEV_DLPI
static void test_non_dlpi_path_complex_open_sequence() {
#ifndef HAVE_DEV_DLPI
    // Configure two opens: first fails with ENOENT, second succeeds
    configure_non_dlpi_opens(-1, 5);

    char errbuf[256] = {0};
    unsigned int ppa = 42;
    int ret = open_dlpi_device("eth0", &ppa, errbuf);

    // Expect the second open to succeed and the function to return its fd, with ppa forced to 0
    EXPECT_EQ(static_cast<unsigned int>(ret), 5u, "Non-DLPI path: second open should succeed and return fd");
    EXPECT_EQ(ppa, 0u, "Non-DLPI path: ppa should be set to 0 when second open succeeds");
#else
    std::cout << "[SKIP] Non-DLPI test compiled with HAVE_DEV_DLPI." << std::endl;
#endif
}
#endif // not HAVE_DEV_DLPI

static void run_all_tests() {
#ifdef HAVE_DEV_DLPI
    std::cout << "Running tests for DLPI path (HAVE_DEV_DLPI defined)." << std::endl;
    test_dlpi_failure_open_path();
    test_dlpi_success_path();
#else
    std::cout << "Running tests for non-DLPI path (HAVE_DEV_DLPI not defined)." << std::endl;
    test_non_dlpi_path_complex_open_sequence();
#endif
    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed << std::endl;
}

int main() {
    run_all_tests();
    // Return non-zero if any test failed (best-effort heuristic)
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}

// End of OpenDLPI_Device_Test.cpp