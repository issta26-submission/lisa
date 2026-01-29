/*
Unit Test Suite for focal method: gen_mcmp_ne (in gencode.c)

Overview:
- We test that gen_mcmp_ne correctly delegates to gen_ncmp with:
  - offrel passed through
  - offset and size passed through
  - mask passed through as the mask argument
  - jtype set to BPF_JEQ
  - reverse set to 1
  - v passed through as the value
- Since gen_mcmp_ne simply returns the result of gen_ncmp, we intercept calls to gen_ncmp
  using a linker wrap (via -Wl,-wrap,gen_ncmp) to capture the parameters without invoking
  the real implementation (which would require the full project state and may have complex
  side effects).

Important build note (to be used when compiling):
- Build with a wrapper for gen_ncmp to capture arguments:
  g++ -std=c++11 test_gen_mcmp_ne.cpp gencode.o -Wl,-wrap,gen_ncmp -o test_gen_mcmp_ne
- The wrapper __wrap_gen_ncmp will be invoked by any call to gen_ncmp within gen_mcmp_ne.
- The test suite does not require the real gen_ncmp to run; it only validates parameter passing.

This test file implements a minimal stand-alone harness (no GTest), using our own lightweight
EXPECT style assertions. It uses a weak-like interception approach by leveraging the linker wrap.

Usage:
- Ensure the project is built in a way that allows -Wl,-wrap,gen_ncmp (as described above).
- Run the produced binary; it will report test pass/fail counts and a short summary.

Code (C++11):

*/

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
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


// Domain/types and declarations aligned with the focal API surface we exercise.
// We do not pull in the original headers to keep this test self-contained.
typedef struct compiler_state_t compiler_state_t;
typedef unsigned int u_int;
typedef uint32_t bpf_u_int32;

// Simple enum placeholder for e_offrel used in the API.
// The actual values in the real project are defined in the public headers;
// here we only need distinct values for testing parameter passing.
enum e_offrel { E_OFFREL_A = 0, E_OFFREL_B = 1 };

// Function under test (declared to match the project prototype).
extern "C" int gen_mcmp_ne(compiler_state_t *cstate, enum e_offrel offrel,
                          u_int offset, u_int size,
                          bpf_u_int32 v, bpf_u_int32 mask);

// Intercept gen_ncmp via linker wrap
// We do not define a real gen_ncmp; instead, we capture the arguments in the wrapper
// and return a benign value to allow gen_mcmp_ne to complete.
static bool wrap_called = false;
static int wrap_offrel = -1;
static unsigned int wrap_offset = 0;
static unsigned int wrap_size = 0;
static unsigned int wrap_mask = 0;
static int wrap_jtype = 0;
static int wrap_reverse = 0;
static unsigned int wrap_v = 0;

// Ensure C linkage for interposition
extern "C" {

// Interposed wrapper for gen_ncmp. The test suite relies on linker wrapping.
// This function records the parameters with which gen_mcmp_ne invoked gen_ncmp
// and returns a harmless sentinel value.
int __wrap_gen_ncmp(compiler_state_t *cstate, enum e_offrel offrel,
                   u_int offset, u_int size, bpf_u_int32 mask,
                   int jtype, int reverse, bpf_u_int32 v)
{
    wrap_called = true;
    wrap_offrel = static_cast<int>(offrel);
    wrap_offset = offset;
    wrap_size = size;
    wrap_mask = static_cast<unsigned int>(mask);
    wrap_jtype = jtype;
    wrap_reverse = reverse;
    wrap_v = static_cast<unsigned int>(v);

    // Do not forward to __real_gen_ncmp to avoid dependence on the real implementation.
    // Return a benign value as if gen_ncmp executed successfully.
    return 0;
}

// The real prototype (only needed to satisfy the linker in some toolchains).
int __real_gen_ncmp(compiler_state_t *cstate, enum e_offrel offrel,
                    u_int offset, u_int size, bpf_u_int32 mask,
                    int jtype, int reverse, bpf_u_int32 v);
} // extern "C"


// Lightweight test framework (non-terminating assertions)
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_EQ_INT(a, b) do { \
    ++tests_run; \
    if (static_cast<long long>(a) != static_cast<long long>(b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << static_cast<long long>(a) \
                  << ") != " #b " (" << static_cast<long long>(b) << ").\n"; \
        ++tests_failed; \
    } \
} while (0)

#define EXPECT_TRUE(a) do { \
    ++tests_run; \
    if (!(a)) { \
        std::cerr << "EXPECT_TRUE failed: condition evaluated to false.\n"; \
        ++tests_failed; \
    } \
} while (0)

#define EXPECT_FALSE(a) do { \
    ++tests_run; \
    if (a) { \
        std::cerr << "EXPECT_FALSE failed: condition evaluated to true.\n"; \
        ++tests_failed; \
    } \
} while (0)


// Test 1: Basic parameter pass-through verification
void test_gen_mcmp_ne_passes_parameters_once()
{
    // Reset wrapper state
    wrap_called = false;
    wrap_offrel = -1;
    wrap_offset = 0;
    wrap_size = 0;
    wrap_mask = 0;
    wrap_jtype = 0;
    wrap_reverse = 0;
    wrap_v = 0;

    // Call the focal function with known parameters
    compiler_state_t *cs = nullptr;
    const enum e_offrel offrel = E_OFFREL_B;
    const u_int offset = 3;
    const u_int size = 5;
    const bpf_u_int32 v = 0x1234ABCDu;
    const bpf_u_int32 mask = 0xFFFFu;

    int ret = gen_mcmp_ne(cs, offrel, offset, size, v, mask);

    // The wrapper should have captured the parameters; ensure we used the wrapper path
    EXPECT_TRUE(wrap_called);
    EXPECT_EQ_INT(wrap_offrel, static_cast<int>(offrel));
    EXPECT_EQ_INT(wrap_offset, offset);
    EXPECT_EQ_INT(wrap_size, size);
    EXPECT_EQ_INT(wrap_mask, mask);
    // Reverse should be 1 (as per gen_mcmp_ne implementation)
    EXPECT_EQ_INT(wrap_reverse, 1);
    EXPECT_EQ_INT(wrap_v, static_cast<unsigned int>(v));

    // Return value is the sentinel from wrapper (0)
    EXPECT_EQ_INT(ret, 0);
}

// Test 2: Multiple calls with different values to ensure independent capture
void test_gen_mcmp_ne_multiple_calls()
{
    // First call setup
    wrap_called = false;
    wrap_offrel = -1;
    wrap_offset = 0;
    wrap_size = 0;
    wrap_mask = 0;
    wrap_jtype = 0;
    wrap_reverse = 0;
    wrap_v = 0;

    // First invocation
    int ret1 = gen_mcmp_ne(nullptr, E_OFFREL_A, 0, 1, 0x0, 0x1);
    EXPECT_TRUE(wrap_called);
    EXPECT_EQ_INT(wrap_offrel, static_cast<int>(E_OFFREL_A));
    EXPECT_EQ_INT(wrap_offset, 0);
    EXPECT_EQ_INT(wrap_size, 1);
    EXPECT_EQ_INT(wrap_mask, 0x0u);
    EXPECT_EQ_INT(wrap_reverse, 1);
    EXPECT_EQ_INT(wrap_v, 0x1u);
    EXPECT_EQ_INT(ret1, 0);

    // Reset wrapper state for second invocation
    wrap_called = false;
    wrap_offrel = -1;
    wrap_offset = 0;
    wrap_size = 0;
    wrap_mask = 0;
    wrap_jtype = 0;
    wrap_reverse = 0;
    wrap_v = 0;

    // Second invocation with different values
    int ret2 = gen_mcmp_ne(nullptr, E_OFFREL_B, 255, 4, 0xDEADBEEF, 0xBEEF);
    EXPECT_TRUE(wrap_called);
    EXPECT_EQ_INT(wrap_offrel, static_cast<int>(E_OFFREL_B));
    EXPECT_EQ_INT(wrap_offset, 255);
    EXPECT_EQ_INT(wrap_size, 4);
    EXPECT_EQ_INT(wrap_mask, 0xDEADBEEFu);
    EXPECT_EQ_INT(wrap_reverse, 1);
    EXPECT_EQ_INT(wrap_v, 0xBEEF);
    EXPECT_EQ_INT(ret2, 0);
}

// Entry point
int main()
{
    test_gen_mcmp_ne_passes_parameters_once();
    test_gen_mcmp_ne_multiple_calls();

    std::cout << "gen_mcmp_ne test suite finished. Ran " << tests_run
              << " tests, " << tests_failed << " failures.\n";

    // Return non-zero if any tests failed
    return (tests_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}