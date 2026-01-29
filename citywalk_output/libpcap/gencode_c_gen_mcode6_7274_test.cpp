// Generated C++11 test suite for gen_mcode6 in gencode.c
// This test suite does not rely on GTest; it uses a simple main() driven harness.
// It provides minimal C stubs to exercise the C code path in gen_mcode6, including
// error paths via bpf_error and a stub for gen_host6 to observe calls.

#include <netinet/in.h>
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
#include <cstdint>
#include <llc.h>
#include <pcap/namedb.h>
#include <pcap/ipnet.h>
#include <diag-control.h>
#include <cstdarg>
#include <linux/filter.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pcap-int.h>
#include <string.h>
#include <memory.h>
#include <linux/types.h>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <cstring>


// Bring in C declarations from the focal module.
// We assume the project provides gencode.h (or an equivalent header) that exposes:
// - struct compiler_state_t
// - struct qual and its fields (addr, proto, dir, etc.)
// - enum qualifiers Q_DEFAULT, Q_HOST, Q_NET, Q_GATEWAY
// - function: void *gen_mcode6(compiler_state_t *cstate, const char *s, bpf_u_int32 masklen, struct qual q)
extern "C" {
}

// Global buffers to capture error messages and host6 invocation details
static char g_last_error[1024] = {0};

// Captured details for gen_host6 stub
static struct in6_addr g_last_addr;
static struct in6_addr g_last_mask;
static unsigned int g_last_proto = 0;
static unsigned int g_last_dir = 0;
static int g_host6_called = 0;
static char g_last_context[256] = {0};

// Simple context for testing: reset global traces
static void reset_traces() {
    std::memset(&g_last_addr, 0, sizeof(g_last_addr));
    std::memset(&g_last_mask, 0, sizeof(g_last_mask));
    g_last_proto = 0;
    g_last_dir = 0;
    g_host6_called = 0;
    std::memset(g_last_context, 0, sizeof(g_last_context));
    g_last_error[0] = '\0';
}

// Stub for bpf_error to capture error messages and unwind control flow
extern "C" void bpf_error(compiler_state_t *cstate, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(g_last_error, sizeof(g_last_error), fmt, ap);
    va_end(ap);
    // Propagate error out of gen_mcode6 by longjmp to the stored top_CTX
    // The real project uses a proper longjmp path; this mirrors that behavior.
    if (cstate && &cstate->top_ctx) {
        longjmp(cstate->top_ctx, 1);
    } else {
        // If top_ctx isn't available, exit to avoid crashing the test
        fprintf(stderr, "Error: bpf_error invoked without valid jump context\n");
        std::exit(1);
    }
}

// Stubbed helper: intercept calls to gen_host6 and record inputs for verification
extern "C" void *gen_host6(compiler_state_t *cstate,
                         int a, const struct in6_addr *addr,
                         const struct in6_addr *mask,
                         const unsigned int proto,
                         const unsigned int dir,
                         const int not, const char *context) {
    (void)a; (void)not;
    if (addr) g_last_addr = *addr;
    if (mask) g_last_mask = *mask;
    g_last_proto = proto;
    g_last_dir = dir;
    g_host6_called = 1;
    if (context) {
        std::strncpy(g_last_context, context, sizeof(g_last_context) - 1);
        g_last_context[sizeof(g_last_context) - 1] = '\0';
    } else {
        g_last_context[0] = '\0';
    }
    // Return a non-NULL dummy pointer to indicate success
    return (void *)0xDEADBEEF;
}

// Utility to compare two in6_addr values
static bool in6_addr_equal(const struct in6_addr *a, const struct in6_addr *b) {
    return std::memcmp(a, b, sizeof(struct in6_addr)) == 0;
}

// Test 1: Successful path - Q_NET with 128-bit IPv6 net, expect non-NULL and host6 invoked
static bool test_gen_mcode6_success_net() {
    reset_traces();

    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs)); // minimal init for testing

    const char *ipv6 = "2001:db8::1";
    bpf_u_int32 masklen = 128;

    struct qual q;
    // Place a typical NET qualifier
    q.addr = Q_NET;
    q.proto = 0; // not used by the test, but should be propagated
    q.dir = 0;

    void *ret = gen_mcode6(&cs, ipv6, masklen, q);
    if (ret == NULL) {
        printf("TEST 1 FAILED: gen_mcode6 returned NULL on valid input. Error: %s\n", g_last_error);
        return false;
    }

    // Validate that gen_host6 was invoked with the expected address and mask
    struct in6_addr expected_addr;
    inet_pton(AF_INET6, ipv6, &expected_addr);

    if (!in6_addr_equal(&g_last_addr, &expected_addr)) {
        printf("TEST 1 FAILED: gen_host6 received unexpected address.\n");
        return false;
    }

    // Expect all 0xff mask for 128-bit prefix
    bool all_ff = true;
    for (int i = 0; i < 16; ++i) {
        if (g_last_mask.s6_addr[i] != 0xff) { all_ff = false; break; }
    }
    if (!all_ff) {
        printf("TEST 1 FAILED: gen_host6 received non-128-bit mask as all ones.\n");
        return false;
    }

    if (g_last_proto != q.proto || g_last_dir != q.dir) {
        printf("TEST 1 FAILED: gen_host6 received incorrect proto/dir values.\n");
        return false;
    }

    if (!g_host6_called) {
        printf("TEST 1 FAILED: gen_host6 stub was not invoked.\n");
        return false;
    }

    printf("TEST 1 PASSED: Successful path for Q_NET with 128-bit IPv6 mask.\n");
    return true;
}

// Test 2: Failure path - invalid IPv6 address string
static bool test_gen_mcode6_invalid_ipv6() {
    reset_traces();

    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs));

    const char *ipv6 = "not-an-ipv6";
    bpf_u_int32 masklen = 128;

    struct qual q;
    q.addr = Q_NET;
    q.proto = 0;
    q.dir = 0;

    void *ret = gen_mcode6(&cs, ipv6, masklen, q);
    if (ret != NULL) {
        printf("TEST 2 FAILED: Expected NULL return for invalid IPv6 address.\n");
        return false;
    }
    // Expect an error message about invalid IPv6 address
    if (g_last_error[0] == '\0' || std::strstr(g_last_error, "valid IPv6 address") == nullptr) {
        printf("TEST 2 FAILED: Error message did not indicate invalid IPv6 address. Msg: %s\n", g_last_error);
        return false;
    }

    printf("TEST 2 PASSED: Proper error path for invalid IPv6 address.\n");
    return true;
}

// Test 3: Failure path - mask length too large
static bool test_gen_mcode6_masklen_too_big() {
    reset_traces();

    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs));

    const char *ipv6 = "2001:db8::1";
    bpf_u_int32 masklen = 9999; // clearly larger than 128-bit space

    struct qual q;
    q.addr = Q_NET;
    q.proto = 0;
    q.dir = 0;

    void *ret = gen_mcode6(&cs, ipv6, masklen, q);
    if (ret != NULL) {
        printf("TEST 3 FAILED: Expected NULL return for masklen too large.\n");
        return false;
    }
    if (g_last_error[0] == '\0' || std::strstr(g_last_error, "mask length") == nullptr) {
        printf("TEST 3 FAILED: Error message did not mention mask length. Msg: %s\n", g_last_error);
        return false;
    }

    printf("TEST 3 PASSED: Proper error path for oversized mask length.\n");
    return true;
}

// Test 4: Failure path - non-network bits set in address given mask
static bool test_gen_mcode6_non_network_bits() {
    reset_traces();

    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs));

    // This IPv6 should be valid, but masklen 0 yields non-network bits set
    const char *ipv6 = "2001:db8::1";
    bpf_u_int32 masklen = 0;

    struct qual q;
    q.addr = Q_NET;
    q.proto = 0;
    q.dir = 0;

    void *ret = gen_mcode6(&cs, ipv6, masklen, q);
    if (ret != NULL) {
        printf("TEST 4 FAILED: Expected NULL return for non-network bits set scenario.\n");
        return false;
    }
    if (g_last_error[0] == '\0' || std::strstr(g_last_error, "non-network bits set") == nullptr) {
        printf("TEST 4 FAILED: Error did not indicate non-network bits. Msg: %s\n", g_last_error);
        return false;
    }

    printf("TEST 4 PASSED: Proper error when non-network bits are set in address/mask.\n");
    return true;
}

// Test 5: Failure path - Q_HOST with invalid host qualifier mask
static bool test_gen_mcode6_host_qual_invalid_mask() {
    reset_traces();

    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs));

    const char *ipv6 = "2001:db8::1";
    bpf_u_int32 masklen = 64; // invalid for host qualifier

    struct qual q;
    q.addr = Q_HOST;
    q.proto = 0;
    q.dir = 0;

    void *ret = gen_mcode6(&cs, ipv6, masklen, q);
    if (ret != NULL) {
        printf("TEST 5 FAILED: Expected NULL return for invalid host qualifier with non-128 mask.\n");
        return false;
    }
    if (g_last_error[0] == '\0') {
        printf("TEST 5 FAILED: Expected an error message for invalid host qualifier.\n");
        return false;
    }

    printf("TEST 5 PASSED: Proper error for Q_HOST with non-128 mask.\n");
    return true;
}

// Test 6: Failure path - Q_GATEWAY with non-128 mask
static bool test_gen_mcode6_gateway_invalid_mask() {
    reset_traces();

    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs));

    const char *ipv6 = "2001:db8::1";
    bpf_u_int32 masklen = 64;

    struct qual q;
    q.addr = Q_GATEWAY;
    q.proto = 0;
    q.dir = 0;

    void *ret = gen_mcode6(&cs, ipv6, masklen, q);
    if (ret != NULL) {
        printf("TEST 6 FAILED: Expected NULL return for invalid gateway qualifier with non-128 mask.\n");
        return false;
    }
    if (g_last_error[0] == '\0') {
        printf("TEST 6 FAILED: Expected an error message for invalid gateway qualifier.\n");
        return false;
    }

    printf("TEST 6 PASSED: Proper error for Q_GATEWAY with non-128 mask.\n");
    return true;
}

// Entry point: run all tests
int main() {
    // Run tests and report summary
    int total = 0;
    int passed = 0;

    printf("Starting test suite for gen_mcode6...\n");

    if (test_gen_mcode6_success_net()) ++passed; ++total;
    reset_traces();

    if (test_gen_mcode6_invalid_ipv6()) ++passed; ++total;
    reset_traces();

    if (test_gen_mcode6_masklen_too_big()) ++passed; ++total;
    reset_traces();

    if (test_gen_mcode6_non_network_bits()) ++passed; ++total;
    reset_traces();

    if (test_gen_mcode6_host_qual_invalid_mask()) ++passed; ++total;
    reset_traces();

    if (test_gen_mcode6_gateway_invalid_mask()) ++passed; ++total;
    reset_traces();

    printf("Test suite completed: %d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}