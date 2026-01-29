// Test suite for the focal method: gen_ip_proto
// Target: verify that gen_ip_proto correctly delegates to gen_cmp with expected parameters.
// Approach: Use a linker-wrapped gen_cmp to intercept calls from gen_ip_proto.
// Build note: To activate wrapper behavior, compile and link with the GNU linker flag:
//   -Wl,--wrap=gen_cmp
// Then provide an implementation of __wrap_gen_cmp in this test file to capture the arguments.
// This allows us to validate that gen_ip_proto passes the correct proto value and
// the expected call site (offset = 9) to gen_cmp, without needing to inspect internal
// state of the production code beyond the call interface.
//
// Important: This test relies on the project providing appropriate type definitions
// for compiler_state_t and related types via included headers (as in the real build).
// The test is designed to work on a Linux environment where BPF_B and related constants
// are defined (e.g., <linux/filter.h>).

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
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <setjmp.h>
#include <cstdint>
#include <string>
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
#include <cstddef>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>


// Forward declare the production types and functions to build against the real code.
// We only rely on the symbol signature; the actual definitions come from the production file
// (gencode.c) and its headers.
extern "C" {
    // Opaque type from the production code; we only use pointers to it in tests.
    typedef struct compiler_state_t compiler_state_t;

    // Focal function under test
    int gen_ip_proto(compiler_state_t *cstate, const uint8_t proto);

    // The real signature of gen_cmp used by gen_ip_proto
    // (We do not define a body here; the linker will bind to the production implementation
    // unless wrapped by __wrap_gen_cmp.)
    int gen_cmp(compiler_state_t *cstate, int offrel, unsigned int offset,
                unsigned int size, uint32_t v);
}

// Global state captured by the wrapper to verify call parameters
static bool g_gen_cmp_called = false;
static const compiler_state_t* g_last_cstate = nullptr;
static int g_last_offrel = 0;
static unsigned int g_last_offset = 0;
static unsigned int g_last_size = 0;
static uint32_t g_last_v = 0;

// Wrapper to intercept calls to gen_cmp when linking with -Wl,--wrap=gen_cmp
extern "C" int __wrap_gen_cmp(compiler_state_t *cstate, int offrel, unsigned int offset,
                             unsigned int size, uint32_t v)
{
    g_gen_cmp_called = true;
    g_last_cstate = cstate;
    g_last_offrel = offrel;
    g_last_offset = offset;
    g_last_size = size;
    g_last_v = v;
    // Return a sentinel value to help detect incorrect forwarding if needed
    return 0xDEADBEEF;
}

// Lightweight, non-terminating assertion helper
#define TEST_ASSERT(cond, msg)                                   \
    do {                                                           \
        if (!(cond)) {                                             \
            std::cerr << "ASSERTION FAILED: " msg << "\n";        \
            g_test_failures++;                                     \
        }                                                          \
    } while (0)

static int g_test_failures = 0;

// Test 1: Verify that gen_ip_proto forwards the provided proto value to gen_cmp
// and that the call site uses offset 9 with the BPF_B size parameter.
// This ensures the true behavior of the method under normal circumstances.
static void test_gen_ip_proto_forwards_proto_tcp()
{
    // Reset captured state
    g_gen_cmp_called = false;
    g_last_cstate = nullptr;
    g_last_offrel = 0;
    g_last_offset = 0;
    g_last_size = 0;
    g_last_v = 0;

    // Create a dummy cstate object (we only pass the pointer through)
    compiler_state_t dummy_state = {};
    const uint8_t test_proto = 0x06; // TCP

    // Call the focal method
    int ret = gen_ip_proto(&dummy_state, test_proto);

    // Assertions
    TEST_ASSERT(g_gen_cmp_called, "gen_cmp should be invoked by gen_ip_proto");
    TEST_ASSERT(g_last_offset == 9, "gen_ip_proto should pass offset 9 to gen_cmp");
    TEST_ASSERT(g_last_v == test_proto, "gen_ip_proto should pass the input proto through to gen_cmp");
    TEST_ASSERT(ret == 0xDEADBEEF, "gen_ip_proto return value should be the wrapper's sentinel if wrapper is used");
}

// Test 2: Verify with a different protocol value (UDP) to ensure parameter propagation remains consistent
static void test_gen_ip_proto_forwards_proto_udp()
{
    // Reset captured state
    g_gen_cmp_called = false;
    g_last_cstate = nullptr;
    g_last_offrel = 0;
    g_last_offset = 0;
    g_last_size = 0;
    g_last_v = 0;

    compiler_state_t dummy_state = {};
    const uint8_t test_proto = 0x11; // UDP

    int ret = gen_ip_proto(&dummy_state, test_proto);

    // Assertions
    TEST_ASSERT(g_gen_cmp_called, "gen_cmp should be invoked by gen_ip_proto (UDP case)");
    TEST_ASSERT(g_last_offset == 9, "gen_ip_proto should pass offset 9 to gen_cmp (UDP case)");
    TEST_ASSERT(g_last_v == test_proto, "gen_ip_proto should pass UDP proto through to gen_cmp");
    TEST_ASSERT(ret == 0xDEADBEEF, "Wrapper return value should propagate through gen_ip_proto");
}

// Array of tests for convenient iteration and reporting
typedef void (*TestFunc)();
static struct {
    const char* name;
    TestFunc func;
} g_tests[] = {
    {"test_gen_ip_proto_forwards_proto_tcp", test_gen_ip_proto_forwards_proto_tcp},
    {"test_gen_ip_proto_forwards_proto_udp", test_gen_ip_proto_forwards_proto_udp},
};

// Simple test runner
static void run_all_tests()
{
    std::cout << "Running gen_ip_proto tests (wrapper-based interception)..." << std::endl;
    const size_t total = sizeof(g_tests) / sizeof(g_tests[0]);
    for (size_t i = 0; i < total; ++i) {
        g_test_failures = 0;
        std::cout << "[ RUN  ] " << g_tests[i].name << std::endl;
        g_tests[i].func();
        if (g_test_failures == 0) {
            std::cout << "[  OK  ] " << g_tests[i].name << std::endl;
        } else {
            std::cout << "[ FAILED ] " << g_tests[i].name
                      << " (failures: " << g_test_failures << ")" << std::endl;
        }
        // Accumulate global failures into a final result
        if (g_test_failures > 0) {
            // Note: we could accumulate a global counter here if desired
        }
    }
}

// Entrypoint
int main()
{
    run_all_tests();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed. See details above." << std::endl;
        return 1;
    }
}

// Explanatory notes:
// - This test suite uses a linker-wrapped override of gen_cmp to observe how gen_ip_proto
//   calls gen_cmp. The wrapper __wrap_gen_cmp captures arguments without executing the
//   production logic of gen_cmp. This approach isolates the unit under test (gen_ip_proto)
//   and validates parameter passing behavior.
// - Two test cases exercise different proto values (TCP and UDP) to ensure the proto value
//   is correctly forwarded. The offset (9) is asserted as part of call-site correctness.
// - The tests are written in standard C++11, avoid private member access, and rely on a
//   lightweight custom test harness (no GTest). Use of EXPECT-like assertions is emulated
//   via TEST_ASSERT to keep tests non-terminating on failure and continue execution.
// - Build instructions (for your CI/build script):
//     g++ -std=c++11 -c test_gen_ip_proto.cpp
//     gcc -c gencode.c
//     g++ test_gen_ip_proto.o gencode.o -o test_gen_ip_proto -Wl,--wrap=gen_cmp
//   Adjust include paths as necessary to locate production headers (e.g., gencode.h, pcap headers).
// - This test relies on the production headers providing the correct type definitions such as
//   compiler_state_t. The wrapper only needs a compatible signature to compile; the actual
//   behavior is intercepted at link time.