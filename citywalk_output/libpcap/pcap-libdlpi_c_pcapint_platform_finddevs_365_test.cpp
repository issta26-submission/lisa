// test_pcapint_platform_finddevs.cpp
// A C++11-based unit test suite for the focal C function:
//     pcapint_platform_finddevs(pcap_if_list_t *devlistp, char *errbuf)
//
// The tests use a small, non-terminating assertion framework (EXPECT_* macros)
// and provide mock implementations of dependent C functions to drive
// deterministic behavior of the focal function without requiring a real
// libdlpi/OS environment.
//
// Notes for maintainers:
// - The test suite relies on the project's public headers to declare the
//   involved types (pcap_if_list_t, linknamelist_t, linkwalk_t, etc.).
// - Mocks are declared with C linkage to override the expected runtime symbols
//   when linking the test binary with the production C file containing
//   pcapint_platform_finddevs.
//
// The concept here is to cover:
// - True/False branches of the major condition predicates.
// - The -1 error return paths (findalldevs_interfaces failure, dlpi_walk error,
//   and add_dev failure).
// - The errno preservation at function exit.
// - Multiple interface entries being processed.
// - Static-like behavior via "static" state inside the test module through
//   global mock flags (as used by the mocks).

#include <libdlpi.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <errno.h>
#include <config.h>
#include <cstdio>
#include <cstdint>
#include <cerrno>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/bufmod.h>
#include <cstdlib>
#include <cstring>
#include <sys/stream.h>


// Include the project's common headers to obtain type declarations.
// This assumes the build environment provides these headers.
// If their paths differ in your environment, adjust accordingly.
extern "C" {
}

// -----------------------------------------------------------------------------------------
// Lightweight non-terminating assertion framework
// -----------------------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    ++g_total_tests; \
    if (cond) { \
        fprintf(stderr, "EXPECT_FALSE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_NOT_NULL(p) do { \
    ++g_total_tests; \
    if ((p) == NULL) { \
        fprintf(stderr, "EXPECT_NOT_NULL failed: %s is NULL at %s:%d\n", #p, __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_STREQ(a,b) do { \
    ++g_total_tests; \
    if ((a) == NULL || (b) == NULL || strcmp((a),(b)) != 0) { \
        fprintf(stderr, "EXPECT_STREQ failed: \"%s\" vs \"%s\" at %s:%d\n", (a), (b), __FILE__, __LINE__); \
        ++g_failed_tests; \
    } \
} while(0)

static void test_report_summary() {
    fprintf(stdout, "Test summary: total=%d, failed=%d\n", g_total_tests, g_failed_tests);
    // Do not abort the program; non-terminating assertion strategy
}

// -----------------------------------------------------------------------------------------
// Mocks to drive pcapint_platform_finddevs behavior
// -----------------------------------------------------------------------------------------

// Forward declare the required types to ensure matching signatures.
// These are provided by the project's headers; we assume their presence.
// If your environment slightly differs, adapt the signatures accordingly.

typedef int (*IsDlpiInterfaceFn)(const char *name);
typedef int (*GetIfFlagsFn)(const char *name, uint32_t *flags, char *errbuf);

// Mocks for external C functions used by pcapint_platform_finddevs

extern "C" {

// Control knobs for test scenarios
static int g_mock_findalldevs_ret = 0;      // 0 = success, -1 = failure
static int g_mock_dlpi_walk_err = 0;        // 0 = success, non-zero = error
static int g_mock_fail_on_dev = 0;          // when non-zero, fail on a specific devname
static const char *g_mock_fail_devname = "dlpi_dev2";

// The mock for pcapint_findalldevs_interfaces
// Signature chosen to mirror the actual usage: (pcap_if_list_t*, char*, IsDlpiInterfaceFn, GetIfFlagsFn)
int pcapint_findalldevs_interfaces(pcap_if_list_t *devlistp, char *errbuf,
                                   IsDlpiInterfaceFn is_dlpi_interface,
                                   GetIfFlagsFn get_if_flags)
{
    // Return -1 to simulate immediate failure of interface discovery
    if (g_mock_findalldevs_ret == -1) {
        return -1;
    }
    // Otherwise behave as the real function would: do nothing here;
    // the subsequent dlpi_walk mock will populate lw_list.
    (void)devlistp;
    (void)errbuf;
    (void)is_dlpi_interface;
    (void)get_if_flags;
    return 0;
}

// The mock for dlpi_walk
// It is invoked as: dlpi_walk(list_interfaces, &lw, 0);
typedef struct linknamelist_tag {
    struct linknamelist_tag *lnl_next;
    const char *linkname;
} linknamelist_t;

typedef struct linkwalk_tag {
    linknamelist_t *lw_list;
    int lw_err;
} linkwalk_t;

int dlpi_walk(void (*walker)(const char *name, void *arg), void *arg, int flags)
{
    (void)walker; // unused in mock
    linkwalk_t *lw = (linkwalk_t *)arg;

    if (g_mock_dlpi_walk_err != 0) {
        // Simulate error in the dlpi_walk step
        lw->lw_err = g_mock_dlpi_walk_err;
        lw->lw_list = NULL;
        return 0;
    }

    // Populate a small list of two devices
    lw->lw_err = 0;
    lw->lw_list = (linknamelist_t *)malloc(sizeof(linknamelist_t));
    lw->lw_list->lnl_next = (linknamelist_t *)malloc(sizeof(linknamelist_t));
    lw->lw_list->linkname = strdup("dlpi_dev1");
    lw->lw_list->lnl_next->linkname = strdup("dlpi_dev2");
    lw->lw_list->lnl_next->lnl_next = NULL;
    return 0;
}

// The mock for pcapint_fmt_errmsg_for_errno
void pcapint_fmt_errmsg_for_errno(char *errbuf, size_t errbuf_size, int err, const char *what)
{
    if (errbuf && errbuf_size > 0) {
        snprintf(errbuf, errbuf_size, "mock errno %d (%s)", err, what ? what : "dlpi_walk");
    }
}

// The mock for pcapint_find_or_add_dev
pcap_if_list_t *pcapint_find_or_add_dev(pcap_if_list_t *devlistp, const char *linkname, int flags,
                                       GetIfFlagsFn get_if_flags, void *arg, char *errbuf)
{
    (void)flags;
    (void)arg;
    (void)get_if_flags;
    (void)errbuf;
    // Simulate a failure on a specific device when enabled
    if (g_mock_fail_on_dev && linkname && strcmp(linkname, g_mock_fail_devname) == 0) {
        return NULL;
    }
    // Return a non-NULL pointer to indicate success
    return (pcap_if_list_t *)((uintptr_t)0x1); // dummy non-NULL
}

} // extern "C"

// -----------------------------------------------------------------------------------------
// Test scaffolding
// -----------------------------------------------------------------------------------------

// Helper to reset mocks to default "success" state
static void reset_mocks() {
    g_mock_findalldevs_ret = 0;
    g_mock_dlpi_walk_err = 0;
    g_mock_fail_on_dev = 0;
    g_mock_fail_devname = "dlpi_dev2";
}

// Test case 1: Normal path - pcapint_platform_finddevs should return 0 and process devices
static void test_case_successful_discovery() {
    reset_mocks();
    g_mock_findalldevs_ret = 0;   // success for interface discovery
    g_mock_dlpi_walk_err = 0;      // dlpi_walk succeeds
    g_mock_fail_on_dev = 0;        // both devices should be added

    // errbuf size as in real usage
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // Call the focal function
    int ret = pcapint_platform_finddevs(NULL, errbuf);
    EXPECT_TRUE(ret == 0);
    // Ensure no error message was emitted
    EXPECT_TRUE(strnlen(errbuf, sizeof(errbuf)) == 0);

    // errno should be preserved by the function (save and restore)
    errno = 7;
    ret = pcapint_platform_finddevs(NULL, errbuf);
    EXPECT_TRUE(errno == 7);
}

// Test case 2: dlpi_walk reports an error, function should return -1
static void test_case_dlpi_walk_error_triggers_negative() {
    reset_mocks();
    g_mock_findalldevs_ret = 0;
    g_mock_dlpi_walk_err = -2; // non-zero triggers error handling
    g_mock_fail_on_dev = 0;

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    int ret = pcapint_platform_finddevs(NULL, errbuf);
    EXPECT_TRUE(ret == -1);
    // The error buffer should have been populated by the mock
    EXPECT_TRUE(strnlen(errbuf, sizeof(errbuf)) > 0);
}

// Test case 3: A device addition fails (simulate pcapint_find_or_add_dev returning NULL)
static void test_case_find_or_add_dev_failure() {
    reset_mocks();
    g_mock_findalldevs_ret = 0;
    g_mock_dlpi_walk_err = 0;
    g_mock_fail_on_dev = 1; // cause a failure for the default fail_devname
    // The code checks the first or second device; with our mock, "dlpi_dev2" will fail

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    int ret = pcapint_platform_finddevs(NULL, errbuf);
    EXPECT_TRUE(ret == -1);
}

// Test case 4: findalldevs_interfaces fails immediately
static void test_case_findalldevs_interfaces_failure() {
    reset_mocks();
    g_mock_findalldevs_ret = -1; // force early failure

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    int ret = pcapint_platform_finddevs(NULL, errbuf);
    EXPECT_TRUE(ret == -1);
}

// Test case 5: Ensure proper cleanup/looping behavior with multiple entries
static void test_case_multiple_entries_cleanup() {
    reset_mocks();
    g_mock_findalldevs_ret = 0;
    g_mock_dlpi_walk_err = 0;
    g_mock_fail_on_dev = 0;

    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    int ret = pcapint_platform_finddevs(NULL, errbuf);
    EXPECT_TRUE(ret == 0);
    // Sanity check: error buffer empty in success path
    EXPECT_TRUE(strnlen(errbuf, sizeof(errbuf)) == 0);
}

// -----------------------------------------------------------------------------------------
// Main driver to execute tests
// -----------------------------------------------------------------------------------------

int main() {
    fprintf(stdout, "Running test_pcapint_platform_finddevs suite (C++11, no GTest)...\n");

    test_case_successful_discovery();
    test_case_dlpi_walk_error_triggers_negative();
    test_case_find_or_add_dev_failure();
    test_case_findalldevs_interfaces_failure();
    test_case_multiple_entries_cleanup();

    test_report_summary();

    // Return non-zero if tests failed to aid automation
    return (g_failed_tests == 0) ? 0 : 1;
}