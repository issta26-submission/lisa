// Test suite for the focal method: dlpromiscon in pcap-libdlpi.c
// Approach: Compile this test as a single translation unit by including the C source
// and mock the external dependency dlpi_promiscon via a test double.
// The test harness uses a small, non-terminating assertion framework to maximize coverage.

#include <libdlpi.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <errno.h>
#include <config.h>
#include <cstdio>
#include <cerrno>
#include <stdlib.h>
#include <pcap-int.h>
#include <dlpisubs.h>
#include <pcap-libdlpi.c>
#include <string.h>
#include <memory.h>
#include <sys/time.h>
#include <sys/bufmod.h>
#include <cstdlib>
#include <cstring>
#include <sys/stream.h>


// The test framework will include pcap-libdlpi.c in this translation unit.
// To intercept calls to the real dlpi_promiscon, we replace its name with a mock
// function (mock_dlpi_promiscon) via a preprocessor macro before including the C file.
#define dlpi_promiscon mock_dlpi_promiscon
extern "C" int mock_dlpi_promiscon(void *hd, unsigned int level);

// The mock state used by tests. Tests can set these globals to control behavior.
static int mock_retval;
static int mock_errno;

// Lightweight, non-terminating assertion helpers.
// They print diagnostic information when failures occur but do not abort tests.
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        printf("EXPECT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        tests_failed = 1; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if((a) != (b)) { \
        printf("EXPECT_EQ failed at %s:%d: %s != %s (actual=%ld, expected=%ld)\n", __FILE__, __LINE__, #a, #b, (long)(a), (long)(b)); \
        tests_failed = 1; \
    } \
} while(0)

#define EXPECT_STRSTR(haystack, needle) do { \
    if(!(haystack) || !(needle) || strstr((haystack), (needle)) == NULL) { \
        printf("EXPECT_STRSTR failed at %s:%d: couldn't find substring \"%s\" in \"%s\"\n", __FILE__, __LINE__, (needle), (haystack)); \
        tests_failed = 1; \
    } \
} while(0)

// The actual implementation under test is inside pcap-libdlpi.c.
// Include it here so that test code runs in the same translation unit and can access internal types.

// After including the C file, the following tests exercise the various code paths
// within dlpromiscon by controlling the mock, and by constructing a pcap_t instance
// with the fields accessed by the focal method.

static int tests_failed = 0;

// Helper: create a minimal pcap_t instance compatible with the focal code path.
// The internal layout of pcap_t and its priv/opt members are defined inside the
// included pcap library headers, which are part of the translation unit after inclusion.
// We rely on those definitions for field access (priv, errbuf, opt, etc.).
static void prepare_pcap_with_dlpi_priv(pcap_t **out_p, struct pcap_dlpi **out_pd) {
    // Allocate a token pcap_t instance and its private dlpi descriptor.
    pcap_t *p = (pcap_t *)calloc(1, sizeof(pcap_t));
    if (!p) {
        printf("Allocation failure in test setup\n");
        exit(1);
    }

    // Allocate the private dlpi structure used by dlpromiscon.
    struct pcap_dlpi *pd = (struct pcap_dlpi *)calloc(1, sizeof(struct pcap_dlpi));
    if (!pd) {
        printf("Allocation failure for pcap_dlpi in test setup\n");
        free(p);
        exit(1);
    }

    // Attach dlpi handle placeholder; the mock will ignore this in tests.
    pd->dlpi_hd = NULL;

    p->priv = pd;

    // Ensure errbuf is zeroed to detect error messages.
    if (p->errbuf) {
        p->errbuf[0] = '\0';
        p->errbuf[PCAP_ERRBUF_SIZE - 1] = '\0'; // safety
    }

    // Some tests need a device string; set a reasonable default.
    // If the real structure has a nested opt with device, this will be used by the error path.
    // We attempt to set it robustly by locating a member named 'opt' and its 'device'.
    // If the exact layout differs, the test path that uses this field is not taken in those cases.
    // The included code provides the actual layout; this line is conservative.
    if (p->opt.device) {
        strncpy(p->opt.device, "test-device", PCAP_ERRBUF_SIZE - 1);
    }

    *out_p = p;
    *out_pd = pd;
}

// Mock implementation of the external dependency, used by the focal function.
// The mock consults the shared state (mock_retval and mock_errno) to decide what to return.
extern "C" int mock_dlpi_promiscon(void *hd, unsigned int level) {
    (void)hd; // unused in tests; the focus is on the return value and errno propagation
    errno = mock_errno;
    return mock_retval;
}

// Test 1: DLPI_SUCCESS should result in a 0 return and no error buffer usage.
static void test_dlpromiscon_success() {
    printf("Running test_dlpromiscon_success...\n");
    pcap_t *p = NULL;
    struct pcap_dlpi *pd = NULL;
    prepare_pcap_with_dlpi_priv(&p, &pd);

    // Simulate success from the underlying DLPI call
    mock_retval = DLPI_SUCCESS;
    mock_errno = 0;

    int ret = dlpromiscon(p, DL_PROMISC_PHYS);

    EXPECT_EQ(ret, 0);
    if (p) {
        // The error buffer should remain untouched on success
        EXPECT_TRUE(p->errbuf[0] == '\0');
    }

    // Cleanup
    if (pd) free(pd);
    if (p) free(p);
}

// Test 2: DL_SYSERR with EPERM and level == DL_PROMISC_PHYS should yield PCAP_ERROR_PROMISC_PERM_DENIED
static void test_dlpromiscon_syserr_perm_phys() {
    printf("Running test_dlpromiscon_syserr_perm_phys...\n");
    pcap_t *p = NULL;
    struct pcap_dlpi *pd = NULL;
    prepare_pcap_with_dlpi_priv(&p, &pd);

    mock_retval = DL_SYSERR;
    mock_errno = EPERM; // EPERM should trigger root-privilege message
    int level = DL_PROMISC_PHYS;

    int ret = dlpromiscon(p, level);

    // Expect permission-denied error
    EXPECT_EQ(ret, PCAP_ERROR_PROMISC_PERM_DENIED);
    // Check that the error message mentions EPERM and the root privilege note
    EXPECT_STRSTR(p->errbuf, "root privilege may be required");
    EXPECT_STRSTR(p->errbuf, "EPERM");

    if (pd) free(pd);
    if (p) free(p);
}

// Test 3: DL_SYSERR with EACCES and level == DL_PROMISC_MULTI should yield a permission-denied
// and the error message should reference "multicast" and "EACCES".
static void test_dlpromiscon_syserr_eacces_multi() {
    printf("Running test_dlpromiscon_syserr_eacces_multi...\n");
    pcap_t *p = NULL;
    struct pcap_dlpi *pd = NULL;
    prepare_pcap_with_dlpi_priv(&p, &pd);

    mock_retval = DL_SYSERR;
    mock_errno = EACCES; // EACCES should trigger root-privilege message
    int level = DL_PROMISC_MULTI;

    int ret = dlpromiscon(p, level);

    // Expect permission-denied error
    EXPECT_EQ(ret, PCAP_ERROR_PERM_DENIED);
    // Check for "multicast" wording and EACCES
    EXPECT_STRSTR(p->errbuf, "multicast");
    EXPECT_STRSTR(p->errbuf, "EACCES");

    if (pd) free(pd);
    if (p) free(p);
}

// Test 4: Non-DLPI-success and not DL_SYSERR with EPERM/EACCES should go through the
// PCAP_ERROR path and populate errbuf via pcap_libdlpi_err.
static void test_dlpromiscon_error_path() {
    printf("Running test_dlpromiscon_error_path...\n");
    pcap_t *p = NULL;
    struct pcap_dlpi *pd = NULL;
    prepare_pcap_with_dlpi_priv(&p, &pd);

    mock_retval = -1; // Not DLPI_SUCCESS and not equal to DL_SYSERR in typical enum
    mock_errno = 0;
    int level = DL_PROMISC_PHYS;

    int ret = dlpromiscon(p, level);

    // Expect a generic PCAP_ERROR
    EXPECT_EQ(ret, PCAP_ERROR);
    // errbuf should be populated by pcap_libdlpi_err (non-empty)
    EXPECT_TRUE(p->errbuf[0] != '\0');

    if (pd) free(pd);
    if (p) free(p);
}

// Entry point for test execution
int main(void) {
    printf("Starting unit test suite for dlpromiscon...\n");
    test_dlpromiscon_success();
    test_dlpromiscon_syserr_perm_phys();
    test_dlpromiscon_syserr_eacces_multi();
    test_dlpromiscon_error_path();

    if (tests_failed) {
        printf("Unit tests completed with failures.\n");
        return 1;
    } else {
        printf("All unit tests completed successfully.\n");
        return 0;
    }
}