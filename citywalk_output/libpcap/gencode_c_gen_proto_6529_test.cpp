// Test suite for the focal method gen_proto in gencode.c
// This test suite targets the gen_proto function and its branching logic.
// It uses a lightweight, non-GTest test harness with non-terminating
// assertions (printed logs) to maximize code execution paths.
// The tests are designed to compile in a C++11 environment and link with
// the original C sources (gencode.c) to exercise real code paths.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <climits>
#include <ieee80211.h>
#include <cstdlib>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <cstdint>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <linux/filter.h>
#include <functional>
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


// Import the project headers to obtain type definitions and function declarations.
// We rely on the project's existing API (gencode.h) that defines
// compiler_state_t, block, and the enumerations/constants used by gen_proto.
extern "C" {
}

// Lightweight non-terminating assertion macro.
// Logs a failure message but continues execution to maximize coverage.
static int test_failures = 0;
#define TEST_EXPECT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[TEST FAIL] " << msg \
                  << " (in " << __func__ << ", line " << __LINE__ << ")" << std::endl; \
        ++test_failures; \
    } \
} while(0)

// Helper to create a minimal compiler_state_t with the least required initialization.
// We rely on the project's type layout for compiler_state_t; we initialize the
// memory to zero and set the linktype field, which is read by gen_proto (e.g., in Q_ISO paths).
static compiler_state_t* create_cstate(int linktype)
{
    compiler_state_t* cstate = (compiler_state_t*)calloc(1, sizeof(compiler_state_t));
    if (!cstate)
        return nullptr;
    // Initialize the linktype field; tests will vary this to exercise ISO/HDLC branches.
    cstate->linktype = linktype;
    return cstate;
}

// Test 1: Default path (proto = Q_DEFAULT)
static void test_proto_default_path()
{
    // Objective: Ensure gen_proto(Q_DEFAULT, v) can be invoked safely and returns a non-null block.
    // This exercises the top-level switch branch for Q_DEFAULT which internally expands
    // into Q_IP and Q_IPV6 paths.
    compiler_state_t* cstate = create_cstate(0);
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_DEFAULT test");

    bpf_u_int32 v = 5; // within UINT8_MAX
    int proto = Q_DEFAULT;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_DEFAULT, v) should return non-null block");
    free(cstate);
}

// Test 2: Link-based path (proto = Q_LINK)
static void test_proto_link_path()
{
    // Objective: Verify that Q_LINK path returns a valid block.
    compiler_state_t* cstate = create_cstate(0);
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_LINK test");

    bpf_u_int32 v = 0; // value is passed to gen_linktype
    int proto = Q_LINK;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_LINK, v) should return non-null block");
    free(cstate);
}

// Test 3: IPv4-like path (proto = Q_IP)
static void test_proto_ip_path()
{
    // Objective: Ensure Q_IP path returns a non-null block and exercises ETHERTYPE_IP + IP proto path.
    compiler_state_t* cstate = create_cstate(0);
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_IP test");

    bpf_u_int32 v = 42; // within UINT8_MAX
    int proto = Q_IP;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_IP, v) should return non-null block");
    free(cstate);
}

// Test 4: IPv6 path (proto = Q_IPV6)
static void test_proto_ipv6_path()
{
    // Objective: Exercise the IPv6 path, including IPPROTO_FRAGMENT handling.
    compiler_state_t* cstate = create_cstate(0);
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_IPV6 test");

    bpf_u_int32 v = 1;
    int proto = Q_IPV6;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_IPV6, v) should return non-null block");
    free(cstate);
}

// Test 5: ISO path with FRELAY linkage (proto = Q_ISO, linktype = DLT_FRELAY)
static void test_proto_iso_frelay_path()
{
    // Objective: Validate the Q_ISO path when the linktype is FRELAY.
    compiler_state_t* cstate = create_cstate(DLT_FRELAY);
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_ISO FRELAY test");

    bpf_u_int32 v = 0x20; // arbitrary value within UINT8_MAX
    int proto = Q_ISO;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_ISO, v) with FRELAY should return non-null block");
    free(cstate);
}

// Test 6: ISO path with HDLC variants (proto = Q_ISO, linktype = DLT_HDLC)
static void test_proto_iso_hdlc_path()
{
    // Objective: Validate the Q_ISO path when the linktype is HDLC (and C_HDLC).
    compiler_state_t* cstate = create_cstate(DLT_HDLC);
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_ISO HDLC test");

    bpf_u_int32 v = 0x55;
    int proto = Q_ISO;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_ISO, v) with HDLC should return non-null block");
    free(cstate);
}

// Test 7: ISO path with C_HDLC (different offset behavior)
static void test_proto_iso_cc_hdlc_path()
{
    // Objective: Validate the Q_ISO path when the linktype is C_HDLC.
    compiler_state_t* cstate = create_cstate(DLT_C_HDLC);
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_ISO C_HDLC test");

    bpf_u_int32 v = 0x11;
    int proto = Q_ISO;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_ISO, v) with C_HDLC should return non-null block");
    free(cstate);
}

// Test 8: ISIS path (proto = Q_ISIS)
static void test_proto_isis_path()
{
    // Objective: Exercise the Q_ISIS path, ensuring the IS-IS PDU type comparison path is exercised.
    compiler_state_t* cstate = create_cstate(0); // default/linktype
    TEST_EXPECT(cstate != nullptr, "Failed to allocate compiler_state_t for Q_ISIS test");

    bpf_u_int32 v = ISIS_PDU_TYPE_MAX < 2 ? 1 : 2; // ensure within ISIS_PDU_TYPE_MAX
    int proto = Q_ISIS;

    struct block* res = gen_proto(cstate, v, proto);

    TEST_EXPECT(res != nullptr, "gen_proto(Q_ISIS, v) should return non-null block");
    free(cstate);
}

// Entry point: Run all tests and report aggregate results.
int main(void)
{
    std::cout << "Starting gen_proto unit tests (C++11 harness, non-GTest)" << std::endl;

    test_proto_default_path();
    test_proto_link_path();
    test_proto_ip_path();
    test_proto_ipv6_path();
    test_proto_iso_frelay_path();
    test_proto_iso_hdlc_path();
    test_proto_iso_cc_hdlc_path();
    test_proto_isis_path();

    if (test_failures == 0) {
        std::cout << "[ALL TESTS PASSED] gen_proto coverage achieved (non-terminating assertions)." << std::endl;
        return 0;
    } else {
        std::cerr << "[SOME TESTS FAILED] Failures: " << test_failures << std::endl;
        return 1;
    }
}