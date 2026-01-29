// Minimal C++11 test suite for pcapint_platform_finddevs (from pcap-bpf.c)
// This test uses hand-written assertions (no GTest) and provides simple stubs for
// dependent functions to control the behavior and exercise branches.
// The test is designed to compile with the provided C source (pcap-bpf.c) and link
// against it. All tests run from main() as required when GTest is not allowed.

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
#include <cstdlib>
#include <stddef.h>
#include <cstdio>
#include <dirent.h>
#include <sys/param.h>
#include <fcntl.h>
#include <cstring>
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


// Step 1: Candidate Keywords derived from the focal method and dependencies
// - pcapint_platform_finddevs
// - pcapint_findalldevs_interfaces
// - pcapint_add_any_dev
// - __FreeBSD__ (branch for FreeBSD path)
// - HAVE_SOLARIS_ANY_DEVICE (branch for Solaris path)
// - finddevs_bpf / finddevs_usb (FreeBSD-specific)
// - devlistp, errbuf
// - check_bpf_bindable, get_if_flags (function pointers used by the dependency call)

// Forward declaration: Provide an opaque type compatible with the real code's usage.
typedef struct pcap_if_list pcap_if_list_t;

// The real function under test is declared in the C source; we declare it here
// with C linkage so the C++ test can call it directly.
extern "C" int pcapint_platform_finddevs(pcap_if_list_t *devlistp, char *errbuf);

// We need to provide exact signatures for the dependencies that the focal function
// calls. We declare them here so the linker can resolve them to our test stubs.

extern "C" {

// Dependency used by pcapint_platform_finddevs
int pcapint_findalldevs_interfaces(pcap_if_list_t *devlistp,
                                   char *errbuf,
                                   int (*check_bpf_bindable)(const char *name),
                                   unsigned int (*get_if_flags)(const char *name,
                                                               unsigned int *flags,
                                                               char *errbuf));

// Dependency that adds the "any" device (may be compiled conditionally)
void* pcapint_add_any_dev(pcap_if_list_t *devlistp, char *errbuf);

#if defined(__FreeBSD__)
// Optional FreeBSD-specific discovery helpers (not required for default Linux test builds)
int finddevs_bpf(pcap_if_list_t *devlistp, char *errbuf);
int finddevs_usb(pcap_if_list_t *devlistp, char *errbuf);
#endif

} // extern "C"

// ---- Test Stubs ----
// We provide lightweight, controllable stubs for the dependencies to drive
// the behaviour of pcapint_platform_finddevs during tests.

// Global state to control the return value of the interface discovery stub.
static int g_findall_interfaces_ret = 0;

// Stub for pcapint_findalldevs_interfaces. It ignores inputs and returns
// the value controlled by g_findall_interfaces_ret.
extern "C" int pcapint_findalldevs_interfaces(pcap_if_list_t *devlistp,
                                               char *errbuf,
                                               int (*check_bpf_bindable)(const char *name),
                                               unsigned int (*get_if_flags)(const char *name,
                                                                           unsigned int *flags,
                                                                           char *errbuf))
{
    (void)devlistp;
    (void)errbuf;
    (void)check_bpf_bindable;
    (void)get_if_flags;
    return g_findall_interfaces_ret;
}

// Stub for pcapint_add_any_dev. Returns non-NULL on success, NULL on failure.
extern "C" void* pcapint_add_any_dev(pcap_if_list_t *devlistp, char *errbuf)
{
    (void)devlistp;
    (void)errbuf;
    // For tests, we simulate success by returning a non-NULL pointer.
    return (void*)0x1;
}

// The following stubs are conditionally compiled if the real code uses them.
// They are provided to avoid linker errors in environments where __FreeBSD__ is
// defined. For standard test runs on non-FreeBSD platforms, these won't be invoked.

// We implement non-functional stubs to satisfy potential symbol resolution.
// They are not used in our test scenarios since we do not toggle FreeBSD branches.

#if defined(__FreeBSD__)
extern "C" int finddevs_bpf(pcap_if_list_t *devlistp, char *errbuf)
{
    (void)devlistp; (void)errbuf;
    return 0; // success by default
}
extern "C" int finddevs_usb(pcap_if_list_t *devlistp, char *errbuf)
{
    (void)devlistp; (void)errbuf;
    return 0; // success by default
}
#endif

// ---- Lightweight Test Harness ----
static int g_test_failures = 0;

// Simple assertion helpers (non-terminating) to maximize code coverage
#define TEST_ASSERT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        std::printf("Assertion failed in %s:%d: expected %ld, got %ld\n", __FILE__, __LINE__, static_cast<long>(expected), static_cast<long>(actual)); \
        g_test_failures++; \
    } \
} while(0)

#define TEST_ASSERT_TRUE(condition) do { \
    if (!(condition)) { \
        std::printf("Assertion failed in %s:%d: condition is false\n", __FILE__, __LINE__); \
        g_test_failures++; \
    } \
} while(0)

//
// Test Case 1: When the interface discovery function reports failure, pcapint_platform_finddevs should propagate the failure (-1).
// This exercises the true/false branch of the first conditional in the focal method.
void test_case_finddevs_failure()
{
    // Arrange
    g_findall_interfaces_ret = -1; // Simulate failure in interface discovery
    char errbuf[256];
    std::memset(errbuf, 0, sizeof(errbuf));

    // Act
    int ret = pcapint_platform_finddevs(nullptr, errbuf);

    // Assert
    // Expecting -1 to indicate failure
    TEST_ASSERT_EQ(-1, ret);
}

// Test Case 2: When interface discovery succeeds, the function should return 0
// This covers the success path after the initial check (no further failures in the
// Solaris/FreeBSD branches are compiled in this environment).
void test_case_finddevs_success()
{
    // Arrange
    g_findall_interfaces_ret = 0; // Simulate success in interface discovery
    char errbuf[256];
    std::memset(errbuf, 0, sizeof(errbuf));

    // Act
    int ret = pcapint_platform_finddevs(nullptr, errbuf);

    // Assert
    // Expecting 0 to indicate success
    TEST_ASSERT_EQ(0, ret);
}

// Entry-point for running all tests
int main()
{
    std::printf("Starting unit tests for pcapint_platform_finddevs...\n");

    test_case_finddevs_failure();
    test_case_finddevs_success();

    if (g_test_failures == 0) {
        std::printf("All tests PASSED.\n");
    } else {
        std::printf("Some tests FAILED: %d failure(s).\n", g_test_failures);
    }

    return g_test_failures;
}