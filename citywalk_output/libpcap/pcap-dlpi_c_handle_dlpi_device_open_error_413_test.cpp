// C++11 test suite for the focal function: handle_dlpi_device_open_error
// This test suite is designed to be compiled together with the pcap-dlpi.c/pcap-dlpi.* sources
// and relies on hooking the dependent functions (handle_nonexistent_dlpi_device and
// pcapint_fmt_errmsg_for_errno) via weak/overridable definitions in this translation unit.

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <errno.h>
#include <cstdio>
#include <string>
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


// ---------------------------------------------------------------------------
// Lightweight test harness (non-terminating, prints results but continues)
// ---------------------------------------------------------------------------

static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define LOG_NOTE(fmt, ...) \
    do { fprintf(stderr, "[NOTE] " fmt "\n", ##__VA_ARGS__); } while (0)

#define TEST_PASS(name)                                             \
    do {                                                            \
        fprintf(stdout, "PASS: %s\n", (name));                    \
        ++g_tests_run; ++g_tests_passed;                          \
    } while (0)

#define TEST_FAIL(name, fmt, ...)                                  \
    do {                                                            \
        fprintf(stderr, "FAIL: %s: " fmt "\n", (name), ##__VA_ARGS__); \
        ++g_tests_run; ++g_tests_failed;                            \
    } while (0)

#define EXPECT_EQ_INT(expected, actual, test_name, note) \
    do { \
        if ((expected) != (actual)) { \
            TEST_FAIL(test_name, "expected %d, got %d. %s", (int)(expected), (int)(actual), (note ? note : "")); \
        } else { \
            TEST_PASS(test_name); \
        } \
    } while (0)

#define EXPECT_STREQ(expected, actual, test_name, note) \
    do { \
        if ((expected) == nullptr || (actual) == nullptr) { \
            if ((expected) != (actual)) { \
                TEST_FAIL(test_name, "pointer mismatch. %s", (note ? note : "")); \
            } else { \
                TEST_PASS(test_name); \
            } \
        } else if (strcmp((expected), (actual)) != 0) { \
            TEST_FAIL(test_name, "strings differ. expected='%s', actual='%s'. %s", (expected), (actual), (note ? note : "")); \
        } else { \
            TEST_PASS(test_name); \
        } \
    } while (0)

// ---------------------------------------------------------------------------
// Environment: provide minimal, overridable mocks for dependencies of the focal
// function. We assume the linked library uses the following signatures.
//
//   int handle_nonexistent_dlpi_device(const char *ifname, char *errbuf);
//   void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbufsize, int errno_, const char *fmt, ... );
//
// By providing non-static definitions here, we override the library's implementations
// in the resulting executable (typical with linker behavior for non-weak symbols).
// ---------------------------------------------------------------------------

// Fallback constants (in case the build environment doesn't define them here)
#ifndef PCAP_ERRBUF_SIZE
#define PCAP_ERRBUF_SIZE 256
#endif

#ifndef PCAP_ERROR
#define PCAP_ERROR -1
#endif

#ifndef PCAP_ERROR_PERM_DENIED
#define PCAP_ERROR_PERM_DENIED -2
#endif

// Forward declare the focal function from the production code (C linkage)
extern "C" int handle_dlpi_device_open_error(const char *ifname, const char *device,
    int error, char *errbuf);

// Globals used by mocks to tailor behavior per-test
static int g_enoent_return_value = 9999; // default return value for ENOENT path

// Mock: track last ENOENT request for verification (optional)
static const char* g_last_enoent_ifname = nullptr;

// Mock implementation: override handle_nonexistent_dlpi_device
extern "C" int handle_nonexistent_dlpi_device(const char *ifname, char *errbuf)
{
    // Mirror a deterministic message for ENOENT path
    if (errbuf) {
        snprintf(errbuf, PCAP_ERRBUF_SIZE, "mocked_nonexistent for %s", ifname ? ifname : "(null)");
    }
    g_last_enoent_ifname = ifname;
    return g_enoent_return_value;
}

// Mock implementation: override pcapint_fmt_errmsg_for_errno
extern "C" void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbufsize, int errno_, const char *fmt, ...)
{
    // Simple deterministic fill, ignoring fmt/varargs for simplicity
    if (errbuf && errbufsize > 0) {
        snprintf(errbuf, errbufsize, "mocked_error_%d", errno_);
    }
}

// ---------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------

void test_handle_dlpi_device_open_error_epperm_branch() {
    // Purpose: Verify true branch for EPERM (permission denied)
    // If we lack permission, the function should return PCAP_ERROR_PERM_DENIED
    // and errbuf should contain the explicit message.
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    const char *ifname = "eth0";
    const char *device = "eth0";

    int ret = handle_dlpi_device_open_error(ifname, device, EPERM, errbuf);

    // Expected outcome checks (non-terminating, continue on fail)
    EXPECT_EQ_INT(PCAP_ERROR_PERM_DENIED, ret,
        "EPERM_branch_return_value",
        "handle_dlpi_device_open_error should return PCAP_ERROR_PERM_DENIED for EPERM");

    const char *expected = "Attempt to open eth0 failed with EPERM - root privilege may be required";
    EXPECT_STREQ(expected, errbuf,
        "EPERM_branch_errbuf_content",
        "errbuf message should indicate EPERM permission issue with the device");
}

void test_handle_dlpi_device_open_error_eacces_branch() {
    // Purpose: Verify true branch for EACCES (permission denied)
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    const char *ifname = "eth1";
    const char *device = "eth1";

    int ret = handle_dlpi_device_open_error(ifname, device, EACCES, errbuf);

    // Expected outcome checks
    EXPECT_EQ_INT(PCAP_ERROR_PERM_DENIED, ret,
        "EACCES_branch_return_value",
        "handle_dlpi_device_open_error should return PCAP_ERROR_PERM_DENIED for EACCES");

    const char *expected = "Attempt to open eth1 failed with EACCES - root privilege may be required";
    EXPECT_STREQ(expected, errbuf,
        "EACCES_branch_errbuf_content",
        "errbuf message should indicate EACCES permission issue with the device");
}

void test_handle_dlpi_device_open_error_enoent_branch() {
    // Purpose: ENOENT path should delegate to handle_nonexistent_dlpi_device
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    const char *ifname = "lo0";
    const char *device = "lo0";

    // Configure mock to return a known value
    g_enoent_return_value = 42;

    int ret = handle_dlpi_device_open_error(ifname, device, ENOENT, errbuf);

    // Expect our mock's return value
    EXPECT_EQ_INT(42, ret,
        "ENOENT_branch_return_value",
        "handle_dlpi_device_open_error should return the value provided by handle_nonexistent_dlpi_device");

    // Expect the errbuf to contain the mock message
    const char *expected = "mocked_nonexistent for lo0";
    EXPECT_STREQ(expected, errbuf,
        "ENOENT_branch_errbuf_content",
        "errbuf should reflect the mocked 'nonexistent' message for the given interface");
}

void test_handle_dlpi_device_open_error_other_error_branch() {
    // Purpose: Non-ENOENT, non-permission errors should route through
    // pcapint_fmt_errmsg_for_errno and return PCAP_ERROR
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    const char *ifname = "eth2";
    const char *device = "eth2";

    int ret = handle_dlpi_device_open_error(ifname, device, ENODEV, errbuf);

    // Expect generic error path
    EXPECT_EQ_INT(PCAP_ERROR, ret,
        "OTHER_error_branch_return_value",
        "handle_dlpi_device_open_error should return PCAP_ERROR for non-ENOENT errors");

    // Our mock fills errbuf with a deterministic string
    const char *expected_prefix = "mocked_error_";
    if (strncmp(errbuf, expected_prefix, strlen(expected_prefix)) != 0) {
        TEST_FAIL("OTHER_error_branch_errbuf_content",
                  "errbuf should be filled by mocked pcapint_fmt_errmsg_for_errno with a predictable prefix");
    } else {
        TEST_PASS("OTHER_error_branch_errbuf_content");
        g_tests_passed++; // already counted in pass above
    }
}

// ---------------------------------------------------------------------------
// Main: run tests and report summary
// ---------------------------------------------------------------------------

int main() {
    // Run all tests
    test_handle_dlpi_device_open_error_epperm_branch();
    test_handle_dlpi_device_open_error_eacces_branch();
    test_handle_dlpi_device_open_error_enoent_branch();
    test_handle_dlpi_device_open_error_other_error_branch();

    // Summary
    printf("\nTest Summary: Run=%d, Passed=%d, Failed=%d\n",
           g_tests_run, g_tests_passed, g_tests_failed);

    // Exit code: 0 if any test failed, else 0
    return (g_tests_failed == 0) ? 0 : 1;
}