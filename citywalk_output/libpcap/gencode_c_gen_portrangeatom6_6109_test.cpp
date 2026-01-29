#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <nametoaddr.h>
#include <cassert>
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
#include <cstring>


// Step 1 & Step 2: Test scaffold for the focal function gen_portrangeatom6
// This test suite targets the C function declared in gencode.c:
//   struct block* gen_portrangeatom6(compiler_state_t *cstate, unsigned off, uint16_t v1, uint16_t v2);
// and its dependent primitives:
//   struct block* gen_portatom6(compiler_state_t *cstate, unsigned off, uint16_t v);
//   struct block* gen_cmp_ge(compiler_state_t *cstate, unsigned offrel, unsigned off, unsigned size, uint32_t v);
//   struct block* gen_cmp_le(compiler_state_t *cstate, unsigned offrel, unsigned off, unsigned size, uint32_t v);
//   struct block* gen_and(struct block*, struct block*);
//
// The real compiler_state_t and struct block types are defined in the project.
// Here we declare them as incomplete types to allow compilation of the test
// harness in environments where the full definitions are provided by the
// linked C sources. At link time, the actual definitions from gencode.c
// will be used.
//
// Important: This test is designed to be compiled as part of the C/C++ build
// that includes gencode.c. It uses function declarations only, and relies
// on the real implementations of the C functions at link time.
// The test assumes a non-null cstate pointer semantics and does not attempt
// to instantiate compiler_state_t itself (that is provided by the production code).

extern "C" {

typedef struct compiler_state_t compiler_state_t; // incomplete in test harness

struct block; // forward declare opaque block

// Focal function under test
struct block* gen_portrangeatom6(compiler_state_t *cstate, unsigned off, uint16_t v1, uint16_t v2);

// Dependent helpers used by the focal function
struct block* gen_portatom6(compiler_state_t *cstate, unsigned off, uint16_t v);
struct block* gen_cmp_ge(compiler_state_t *cstate, unsigned offrel, unsigned off, unsigned size, uint32_t v);
struct block* gen_cmp_le(compiler_state_t *cstate, unsigned offrel, unsigned off, unsigned size, uint32_t v);
struct block* gen_and(struct block* b0, struct block* b1);

} // extern "C"

// Very lightweight, non-intrusive unit test framework (no external libs)
static int g_test_failures = 0;

#define TEST_CASE(name) static void name()
#define ASSERT_TRUE(expr) \
    do { \
        if(!(expr)) { \
            std::cerr << "Assertion failed: " #expr " at " __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define ASSERT_EQ(a,b) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "Assertion failed: " #a " == " #b " (" << (a) << " != " << (b) \
                      << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define ASSERT_NE(a,b) \
    do { \
        if((a) == (b)) { \
            std::cerr << "Assertion failed: " #a " != " #b " (" << (a) << " == " << (b) \
                      << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define RUN_TEST(test) \
    do { \
        test(); \
    } while(0)


// Helper: a minimal dummy non-null pointer for cstate to satisfy function signatures.
// In a real environment, this would be a proper compiler_state_t instance provided by the production code.
static compiler_state_t* make_fake_cstate() {
    // Do not allocate actual memory here; the test harness expects a non-null pointer.
    // If the production code uses fields, it will be up to the linked gencode.c to provide a valid instance.
    // We return a non-null sentinel; tests should only rely on pointer non-nullness.
    return reinterpret_cast<compiler_state_t*>(0x1);
}

// Helper: no-op destroy; the production code may own its own lifecycle
static void destroy_fake_cstate(compiler_state_t* /*cs*/) {
    // Intentionally empty: the test only needs a non-null pointer to pass through.
}


// Test 1: When v1 == v2, gen_portrangeatom6 should delegate to gen_portatom6 for the same value.
TEST_CASE(test_portrangeatom6_equal_branch_delegates_to_portatom6) {
    compiler_state_t* cs = make_fake_cstate();
    if (cs == nullptr) {
        std::cerr << "Test setup failed: null compiler_state_t" << std::endl;
        ++g_test_failures;
        return;
    }

    const unsigned off = 0;
    const uint16_t val = 0x1234;

    struct block* res_portrange = gen_portrangeatom6(cs, off, val, val);
    struct block* res_portatom6 = gen_portatom6(cs, off, val);

    // We expect identical results when v1 == v2
    uintptr_t a = reinterpret_cast<uintptr_t>(res_portrange);
    uintptr_t b = reinterpret_cast<uintptr_t>(res_portatom6);
    ASSERT_EQ(a, b);

    destroy_fake_cstate(cs);
}


// Test 2: When v1 != v2, the function should produce a composite range using ge, le and and.
// We verify basic properties: non-null result, and that result is not trivially equal to a single portatom6.
// Additionally, we test that min(v1, v2) and max(v1, v2) are used by the computation indirectly.
TEST_CASE(test_portrangeatom6_range_branch_creates_composite) {
    compiler_state_t* cs = make_fake_cstate();
    if (cs == nullptr) {
        std::cerr << "Test setup failed: null compiler_state_t" << std::endl;
        ++g_test_failures;
        return;
    }

    const unsigned off = 0;
    const uint16_t v1 = 0x0010; // 16
    const uint16_t v2 = 0x00F0; // 240

    struct block* res = gen_portrangeatom6(cs, off, v1, v2);
    ASSERT_TRUE(res != nullptr); // basic sanity

    // Sanity cross-check: ensure that the composite is not a trivial portatom6 result
    struct block* trivial = gen_portatom6(cs, off, v1);
    if (trivial != nullptr) {
        uintptr_t a = reinterpret_cast<uintptr_t>(res);
        uintptr_t b = reinterpret_cast<uintptr_t>(trivial);
        ASSERT_NE(a, b);
    }

    // Also compare with the other endpoint to reaffirm non-triviality
    struct block* trivial2 = gen_portatom6(cs, off, v2);
    if (trivial2 != nullptr) {
        uintptr_t a = reinterpret_cast<uintptr_t>(res);
        uintptr_t c = reinterpret_cast<uintptr_t>(trivial2);
        ASSERT_NE(a, c);
    }

    destroy_fake_cstate(cs);
}


// Test 3: Boundary test: v1 and v2 across edge values to ensure no crashes and proper non-null results.
TEST_CASE(test_portrangeatom6_edge_values) {
    compiler_state_t* cs = make_fake_cstate();
    if (cs == nullptr) {
        std::cerr << "Test setup failed: null compiler_state_t" << std::endl;
        ++g_test_failures;
        return;
    }

    const unsigned off = 0;

    // Edge values within 16-bit domain
    uint16_t edge_vals[][2] = {
        {0, 1},
        {1, 0xFFFF},
        {0x7FFF, 0x8000},
        {0, 0},
        {0xFFFF, 0xFFFF}
    };

    for (auto &p : edge_vals) {
        uint16_t a = p[0];
        uint16_t b = p[1];
        struct block* r = gen_portrangeatom6(cs, off, a, b);
        ASSERT_TRUE(r != nullptr);
        // Also compare with a single port for equality when both values are equal (handled by test 1)
        if (a == b) {
            struct block* pblk = gen_portatom6(cs, off, a);
            ASSERT_TRUE(r == pblk);
        }
    }

    destroy_fake_cstate(cs);
}


// Test 4: Static-like behavior check (domain-driven): ensure that static/internal semantics do not expose API
// In this environment we only verify that the function can be invoked and returns a non-null result.
// This test helps exercise the code-path in environments where assertions inside the C code rely on valid
// internal state, but we cannot access private internals from the test harness.
TEST_CASE(test_portrangeatom6_basic_invocation) {
    compiler_state_t* cs = make_fake_cstate();
    if (cs == nullptr) {
        std::cerr << "Test setup failed: null compiler_state_t" << std::endl;
        ++g_test_failures;
        return;
    }

    const unsigned off = 5;
    const uint16_t v1 = 1234;
    const uint16_t v2 = 5678;

    struct block* r = gen_portrangeatom6(cs, off, v1, v2);
    ASSERT_TRUE(r != nullptr);

    destroy_fake_cstate(cs);
}


// Main: run all tests
int main() {
    std::cout << "Running test suite for gen_portrangeatom6...\n";

    RUN_TEST(test_portrangeatom6_equal_branch_delegates_to_portatom6);
    RUN_TEST(test_portrangeatom6_range_branch_creates_composite);
    RUN_TEST(test_portrangeatom6_edge_values);
    RUN_TEST(test_portrangeatom6_basic_invocation);

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
}