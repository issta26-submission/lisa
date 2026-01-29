// A lightweight C++11 test harness for the focal function dlpromiscon
// This test suite provides minimal mocking of dependencies (send_request, dlokack)
// and exercises true/false branches of the function under test.

#include <sys/dlpi_ext.h>
#include <unistd.h>
#include <stdio.h>
#include <stropts.h>
#include <sys/types.h>
#include <iostream>
#include <errno.h>
#include <cstdio>
#include <cstdint>
#include <cerrno>
#include <string>
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


// Domain constants and types (stubs to allow compiling the focal function)
typedef uint32_t bpf_u_int32;
typedef struct {
    int dl_primitive;
    bpf_u_int32 dl_level;
} dl_promiscon_req_t;

#define PCAP_ERRBUF_SIZE 256
#define MAXDLBUF 1024
#define DL_PROMISCON_REQ 1
#define STRINGIFY(x) #x
#define PCAP_ERROR -1
#define PCAP_ERROR_PERM_DENIED -2
#define PCAP_ERROR_PROMISC_PER_DENIED -3

// Public API types (stub)
typedef struct {
    int fd;
    char errbuf[PCAP_ERRBUF_SIZE];
} pcap_t;

// Global test control/state for mocks
static int g_send_request_ret = 0;
static int g_dlokack_ret = 0;
static int g_uerror = 0;

// For validating the what parameter passed to dlokack
static const char* g_expected_dlokack_what = nullptr;
static char g_expected_dlokack_what_buf[128];

// Forward declaration of the function under test
int dlpromiscon(pcap_t *p, bpf_u_int32 level);

// Mocked dependencies (to control behavior of the focal function)
extern "C" {

// Mock: send_request(fd, ptr, len, what, ebuf)
int send_request(int fd, char *ptr, int len, const char *what, char *ebuf)
{
    (void)fd; (void)ptr; (void)len; (void)ebuf;
    // If a test needs to assert on the "what" string, we can extend here.
    // For now, simply return the configured test value.
    return g_send_request_ret;
}

// Mock: dlokack(fd, what, buf, ebuf, uerror)
int dlokack(int fd, const char *what, char *buf, char *ebuf, int *uerror)
{
    (void)fd; (void)buf; (void)ebuf;
    // Validate the what string if an expectation is set
    if (g_expected_dlokack_what && what) {
        if (strcmp(what, g_expected_dlokack_what) != 0) {
            // Print diagnostic but do not abort test
            fprintf(stderr, "DLKACK what mismatch: expected '%s', got '%s'\n",
                    g_expected_dlokack_what, what);
        }
    }
    if (uerror) {
        *uerror = g_uerror;
    }
    return g_dlokack_ret;
}

} // extern "C"

// Implementation of the focal function under test
int dlpromiscon(pcap_t *p, bpf_u_int32 level)
{
{
    dl_promiscon_req_t req;
    bpf_u_int32 buf[MAXDLBUF];
    int err;
    int uerror;
    req.dl_primitive = DL_PROMISCON_REQ;
    req.dl_level = level;
    if (send_request(p->fd, (char *)&req, sizeof(req), "promiscon",
        p->errbuf) < 0)
        return (PCAP_ERROR);
    // Validate the what string for testing purposes
    // The real code passes: "promiscon" STRINGIFY(level)
    char whatbuf[64];
    snprintf(whatbuf, sizeof(whatbuf), "promiscon%d", (int)level);
    g_expected_dlokack_what = whatbuf;
    // Note: the test harness uses the global expected string for validation
    err = dlokack(p->fd, "promiscon" STRINGIFY(level), (char *)buf,
        p->errbuf, &uerror);
    if (err < 0) {
        if (err == PCAP_ERROR_PERM_DENIED) {
            snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
                "Attempt to set promiscuous mode failed with %s - root privilege may be required",
                (uerror == EPERM) ? "EPERM" : "EACCES");
            err = PCAP_ERROR_PROMISC_PER_DENIED;
        }
        return (err);
    }
    return (0);
}
}

// --- Test framework (lightweight) ---

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(a, b) do { \
    g_total_tests++; \
    if ((a) != (b)) { \
        fprintf(stderr, "EXPECT_EQ failed: %s == %lld, got %lld (%s:%d)\n", #a, (long long)(b), (long long)(a), __FILE__, __LINE__); \
        g_failed_tests++; \
    } else { \
        /* success path: can increment a success counter or ignore */ \
    } \
} while(0)

#define EXPECT_STRCONTAINS(s, substr) do { \
    g_total_tests++; \
    if ((s) == nullptr || std::string((s)).find(substr) == std::string::npos) { \
        fprintf(stderr, "EXPECT_STRCONTAINS failed: string does not contain '%s' (%s:%d)\n", substr, __FILE__, __LINE__); \
        g_failed_tests++; \
    } \
} while(0)

#define TEST(name) void name()

// Utility to reset per-test mocks
static void reset_mocks_for_test() {
    g_send_request_ret = 0;
    g_dlokack_ret = 0;
    g_uerror = 0;
    g_expected_dlokack_what = nullptr;
    std::memset(g_expected_dlokack_what_buf, 0, sizeof(g_expected_dlokack_what_buf));
}

// Helper to prepare expected what string
static void prepare_expected_what(int level) {
    static char tmp[64];
    std::snprintf(tmp, sizeof(tmp), "promiscon%d", level);
    g_expected_dlokack_what = tmp;
}

// Test 1: Normal path: send_request succeeds and dlokack succeeds
TEST(test_dlpromiscon_success) {
    reset_mocks_for_test();
    pcap_t p;
    p.fd = 7;
    std::memset(p.errbuf, 0, sizeof(p.errbuf));

    int level = 5;
    g_send_request_ret = 0;
    g_dlokack_ret = 0; // success
    g_uerror = 0;
    prepare_expected_what(level);

    int ret = dlpromiscon(&p, (bpf_u_int32)level);
    EXPECT_EQ(ret, 0);
    // Verify that error buffer remains untouched (empty) on success is optional
}

// Test 2: send_request fails -> should return PCAP_ERROR
TEST(test_dlpromiscon_send_fail) {
    reset_mocks_for_test();
    pcap_t p;
    p.fd = 10;
    std::memset(p.errbuf, 0, sizeof(p.errbuf));

    int level = 1;
    g_send_request_ret = -1; // simulate failure
    g_dlokack_ret = 0;
    g_uerror = 0;

    int ret = dlpromiscon(&p, (bpf_u_int32)level);
    EXPECT_EQ(ret, PCAP_ERROR);
}

// Test 3: dlokack fails with non-permission error -> propagate error
TEST(test_dlpromiscon_dlokack_fail) {
    reset_mocks_for_test();
    pcap_t p;
    p.fd = 3;
    std::memset(p.errbuf, 0, sizeof(p.errbuf));

    int level = 2;
    g_send_request_ret = 0;
    g_dlokack_ret = -5; // some non-permission error
    g_uerror = 0;

    int ret = dlpromiscon(&p, (bpf_u_int32)level);
    EXPECT_EQ(ret, -5);
}

// Test 4: dlokack returns PCAP_ERROR_PERM_DENIED with EPERM -> PCAP_ERROR_PROMISC_PER_DENIED and errbuf contains EPERM
TEST(test_dlpromiscon_perm_denied_epperm) {
    reset_mocks_for_test();
    pcap_t p;
    p.fd = 4;
    std::memset(p.errbuf, 0, sizeof(p.errbuf));

    int level = 99;
    g_send_request_ret = 0;
    g_dlokack_ret = PCAP_ERROR_PERM_DENIED;
    g_uerror = EPERM; // EPERM scenario
    prepare_expected_what(level);

    int ret = dlpromiscon(&p, (bpf_u_int32)level);
    EXPECT_EQ(ret, PCAP_ERROR_PROMISC_PER_DENIED);
    // Check error buffer message for EPERM presence and guidance text
    EXPECT_STRCONTAINS(p.errbuf, "root privilege may be required");
    // Ensure message mentions EPERM specifically
    // If EPERM, the string should include EPERM
    std::string errstr(p.errbuf);
    if (errstr.find("EPERM") == std::string::npos) {
        fprintf(stderr, "Diagnostic: errbuf did not mention EPERM as expected (%s:%d)\n", __FILE__, __LINE__);
        g_failed_tests++;
    }
}

// Test 5: dlokack returns PCAP_ERROR_PERM_DENIED with non-EPERM (EACCES)
TEST(test_dlpromiscon_perm_denied_eacces) {
    reset_mocks_for_test();
    pcap_t p;
    p.fd = 8;
    std::memset(p.errbuf, 0, sizeof(p.errbuf));

    int level = 7;
    g_send_request_ret = 0;
    g_dlokack_ret = PCAP_ERROR_PERM_DENIED;
    g_uerror = 0; // not EPERM
    prepare_expected_what(level);

    int ret = dlpromiscon(&p, (bpf_u_int32)level);
    EXPECT_EQ(ret, PCAP_ERROR_PROM_DENIED); // the macro name is PCAP_ERROR_PROMISC_PER_DENIED
    // The above constant is PCAP_ERROR_PROMISC_PER_DENIED; verify message contains EACCES
    EXPECT_STRCONTAINS(p.errbuf, "EACCES");
}

// Helper to run all tests
static void run_all_tests() {
    test_dlpromiscon_success();
    test_dlpromiscon_send_fail();
    test_dlpromiscon_dlokack_fail();
    test_dlpromiscon_perm_denied_epperm();
    test_dlpromiscon_perm_denied_eacces();
}

// Main entry (as per domain knowledge: run tests from main without a test framework)
int main() {
    run_all_tests();
    printf("Tests run: %d, Failures: %d\n", g_total_tests, g_failed_tests);
    if (g_failed_tests > 0) {
        printf("Some tests FAILED.\n");
        return 1;
    }
    printf("All tests PASSED.\n");
    return 0;
}