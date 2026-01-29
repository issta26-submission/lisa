// Unit test suite for the focal method: gen_ether_linktype in gencode.c
// This test suite is written in C++11 (no Google Test) and exercises the
// gen_ether_linktype function by invoking it with representative ll_proto values.
// It relies on the provided C codebase (gencode.c and associated headers) and
// should be compiled together with the production sources.
// Notes:
// - This test uses a minimal, non-intrusive test harness with non-terminating
//   assertions (log failures but continue to run all tests).
// - We allocate a temporary compiler_state_t to serve as the cstate input. The
//   implementation assumes the production code will tolerate a zero-initialized
//   state for the purpose of code-generation graph construction.
// - Each test case focuses on a distinct code-path within the switch in
//   gen_ether_linktype to improve coverage across true/false branches.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <vector>
#include <linux/filter.h>
#include <stdlib.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <cstring>


// Import production headers to obtain required types, macros, and function
// declarations. These headers define things like compiler_state_t, bpf_u_int32,
// and the constants LLCSAP_ISONS, LLCSAP_IP, LLCSAP_NETBEUI, LLCSAP_IPX,
// ETHERTYPE_ATALK, ETHERTYPE_AARP, ETHERMTU, etc.
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_test_passed = 0;
static int g_test_failed = 0;

static void log_failure(const char* test_name, const char* detail, const char* file, int line) {
    std::fprintf(stderr, "TEST FAILED: %s | %s (at %s:%d)\n", test_name, detail, file, line);
}

#define EXPECT_NOT_NULL(ptr, test_name)                                          \
    do {                                                                           \
        if ((ptr) == nullptr) {                                                  \
            log_failure(test_name, "Pointer is NULL", __FILE__, __LINE__);        \
            g_test_failed++;                                                     \
        } else {                                                                 \
            g_test_passed++;                                                     \
        }                                                                        \
    } while (0)

#define EXPECT_TRUE(cond, test_name)                                               \
    do {                                                                             \
        if (!(cond)) {                                                               \
            log_failure(test_name, "Expression evaluated to false", __FILE__, __LINE__); \
            g_test_failed++;                                                       \
        } else {                                                                     \
            g_test_passed++;                                                         \
        }                                                                            \
    } while (0)

// Helper to safely allocate and initialize a compiler_state_t
static compiler_state_t* create_dummy_cstate() {
    // Allocate a zero-initialized state. The production code typically fills in
    // the necessary details during codegen; for unit testing the shape is enough
    // to exercise pathway selection in gen_ether_linktype.
    return (compiler_state_t*)calloc(1, sizeof(compiler_state_t));
}

// Test 1: LLCSAP_ISONS path - "OSI protocols and NetBEUI always use 802.2 encapsulation"
// We expect this path to return a non-null block pointer when called with a legal LLCSAP value.
static bool test_gen_ether_linktype_llcsap_isons_path() {
    compiler_state_t* cstate = create_dummy_cstate();
    if (!cstate) {
        log_failure("test_gen_ether_linktype_llcsap_isons_path", "Failed to allocate cstate", __FILE__, __LINE__);
        g_test_failed++;
        return false;
    }

    // Use a known LLCSAP constant for the test.
    bpf_u_int32 proto = LLCSAP_ISONS;
    struct block *res = gen_ether_linktype(cstate, proto);
    EXPECT_NOT_NULL(res, "gen_ether_linktype(LLCSAP_ISONS) should return non-null");

    free(cstate);
    return true;
}

// Test 2: LLCSAP_IP path
// Ensure non-null code path is produced for IP over LLC (802.2 encapsulation).
static bool test_gen_ether_linktype_llcsap_ip_path() {
    compiler_state_t* cstate = create_dummy_cstate();
    if (!cstate) {
        log_failure("test_gen_ether_linktype_llcsap_ip_path", "Failed to allocate cstate", __FILE__, __LINE__);
        g_test_failed++;
        return false;
    }

    bpf_u_int32 proto = LLCSAP_IP;
    struct block *res = gen_ether_linktype(cstate, proto);
    EXPECT_NOT_NULL(res, "gen_ether_linktype(LLCSAP_IP) should return non-null");

    free(cstate);
    return true;
}

// Test 3: LLCSAP_NETBEUI path
// Validate that a NetBEUI case yields a non-null result.
static bool test_gen_ether_linktype_llcsap_netbeui_path() {
    compiler_state_t* cstate = create_dummy_cstate();
    if (!cstate) {
        log_failure("test_gen_ether_linktype_llcsap_netbeui_path", "Failed to allocate cstate", __FILE__, __LINE__);
        g_test_failed++;
        return false;
    }

    bpf_u_int32 proto = LLCSAP_NETBEUI;
    struct block *res = gen_ether_linktype(cstate, proto);
    EXPECT_NOT_NULL(res, "gen_ether_linktype(LLCSAP_NETBEUI) should return non-null");

    free(cstate);
    return true;
}

// Test 4: LLCSAP_IPX path
// IPX has a more complex path (802.2 LLC and 802.3/SNAP variants).
static bool test_gen_ether_linktype_llcsap_ipx_path() {
    compiler_state_t* cstate = create_dummy_cstate();
    if (!cstate) {
        log_failure("test_gen_ether_linktype_llcsap_ipx_path", "Failed to allocate cstate", __FILE__, __LINE__);
        g_test_failed++;
        return false;
    }

    bpf_u_int32 proto = LLCSAP_IPX;
    struct block *res = gen_ether_linktype(cstate, proto);
    EXPECT_NOT_NULL(res, "gen_ether_linktype(LLCSAP_IPX) should return non-null");

    free(cstate);
    return true;
}

// Test 5: ETHERTYPE_ATALK / ETHERTYPE_AARP path
// Ensure both ATALK and AARP variants produce non-null results.
static bool test_gen_ether_linktype_ethertype_alternate_paths() {
    compiler_state_t* cstate1 = create_dummy_cstate();
    compiler_state_t* cstate2 = create_dummy_cstate();
    if (!cstate1 || !cstate2) {
        log_failure("test_gen_ether_linktype_ethertype_alternate_paths", "Failed to allocate cstate(s)", __FILE__, __LINE__);
        if (cstate1) free(cstate1);
        if (cstate2) free(cstate2);
        g_test_failed += (cstate1==nullptr) + (cstate2==nullptr);
        return false;
    }

    bpf_u_int32 proto_atalk = ETHERTYPE_ATALK;
    bpf_u_int32 proto_aarp  = ETHERTYPE_AARP;

    struct block *res1 = gen_ether_linktype(cstate1, proto_atalk);
    struct block *res2 = gen_ether_linktype(cstate2, proto_aarp);

    EXPECT_NOT_NULL(res1, "gen_ether_linktype(ETHERTYPE_ATALK) should return non-null");
    EXPECT_NOT_NULL(res2, "gen_ether_linktype(ETHERTYPE_AARP) should return non-null");

    free(cstate1);
    free(cstate2);
    return true;
}

// Test 6: Default path with LLC DSAP (ll_proto <= ETHERMTU)
// Exercise branch for LLC DSAP values that fit 802.2 framing.
static bool test_gen_ether_linktype_default_llc_dsap_path() {
    compiler_state_t* cstate = create_dummy_cstate();
    if (!cstate) {
        log_failure("test_gen_ether_linktype_default_llc_dsap_path", "Failed to allocate cstate", __FILE__, __LINE__);
        g_test_failed++;
        return false;
    }

    // Use a small LLC DSAP value (within UINT8_MAX)
    bpf_u_int32 proto = 0x01;
    struct block *res = gen_ether_linktype(cstate, proto);
    EXPECT_NOT_NULL(res, "gen_ether_linktype(default LLC DSAP) should return non-null");

    free(cstate);
    return true;
}

// Test 7: Default path with EtherType (ll_proto > ETHERMTU)
// Exercise branch for Ethernet type handling when the frame is not LLC 802.2.
static bool test_gen_ether_linktype_default_ethertype_path() {
    compiler_state_t* cstate = create_dummy_cstate();
    if (!cstate) {
        log_failure("test_gen_ether_linktype_default_ethertype_path", "Failed to allocate cstate", __FILE__, __LINE__);
        g_test_failed++;
        return false;
    }

    // Choose a value greater than ETHERMTU to hit EtherType branch
    bpf_u_int32 proto = ETHERMTU + 2; // e.g., 1502
    struct block *res = gen_ether_linktype(cstate, proto);
    EXPECT_NOT_NULL(res, "gen_ether_linktype(default EtherType) should return non-null");

    free(cstate);
    return true;
}

// Entry point: run all tests and summarize results
int main() {
    // Run tests and report
    printf("Starting unit tests for gen_ether_linktype...\n");

    bool ok;
    ok = test_gen_ether_linktype_llcsap_isons_path();
    if (!ok) printf("Test 1 reported failure via log.\n");

    ok = test_gen_ether_linktype_llcsap_ip_path();
    if (!ok) printf("Test 2 reported failure via log.\n");

    ok = test_gen_ether_linktype_llcsap_netbeui_path();
    if (!ok) printf("Test 3 reported failure via log.\n");

    ok = test_gen_ether_linktype_llcsap_ipx_path();
    if (!ok) printf("Test 4 reported failure via log.\n");

    ok = test_gen_ether_linktype_ethertype_alternate_paths();
    if (!ok) printf("Test 5 reported failure via log.\n");

    ok = test_gen_ether_linktype_default_llc_dsap_path();
    if (!ok) printf("Test 6 reported failure via log.\n");

    ok = test_gen_ether_linktype_default_ethertype_path();
    if (!ok) printf("Test 7 reported failure via log.\n");

    printf("Unit test summary: passed=%d, failed=%d\n", g_test_passed, g_test_failed);

    // Note: do not rely on tests to abort the process; return non-zero if failures occurred.
    return g_test_failed ? 1 : 0;
}