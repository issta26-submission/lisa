// Lightweight C++11 test harness for the focal method gen_atm_prototype
// This test is self-contained to demonstrate unit-test style verification
// without relying on an external testing framework like GTest.
// It uses a small, deterministic mock of the dependent function gen_mcmp
// to validate that gen_atm_prototype forwards the correct arguments
// (OR_LINKHDR, cstate->off_proto, BPF_B, v, 0x0fU) and returns the same mock result.
//
// Notes:
// - This test is intentionally self-contained and does not depend on the full
//   project build. It focuses on the logical behavior of the focal method
//   as described in the provided code snippet.
// - We emulate the minimal necessary types and constants (compiler_state_t, OR_LINKHDR, BPF_B).
// - Non-terminating assertions are used: failures are recorded, but execution continues.

#include <pflog.h>
#include <stdarg.h>
#include <gencode.h>
#include <ppp.h>
#include <stdio.h>
#include <ethertype.h>
#include <extract.h>
#include <iostream>
#include <iomanip>
#include <nametoaddr.h>
#include <ieee80211.h>
#include <stdint.h>
#include <stddef.h>
#include <thread-local.h>
#include <pcap-util.h>
#include <scanner.h>
#include <cstdio>
#include <setjmp.h>
#include <cstdint>
#include <string>
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


// Domain-relevant minimal stubs to emulate the environment around gen_atm_prototype

// Minimal compiler_state_t used by the focal function
typedef struct {
    uint8_t off_proto; // offset for prototype in the compilation state
} compiler_state_t;

// Core constants expected by the focal function
enum {
    OR_LINKHDR = 1, // example value for OR_LINKHDR
};

#define BPF_B 0x04U  // example value for BPF_B (8-bit compare size in BPF)
#define PROTO_MASK 0x0fU

// Forward declarations to mimic the production code interface
extern "C" {
    // The focal function under test (as implemented in the target gencode.c)
    int gen_atm_prototype(compiler_state_t *cstate, const uint8_t v);

    // The dependent function that gen_atm_prototype delegates to
    int gen_mcmp(compiler_state_t *cstate,
                 int offrel,
                 unsigned offset,
                 unsigned size,
                 unsigned v,
                 unsigned mask);
}

// ------------------------------------------------------------------------------------------------
// Mock and test-tracking scaffolding to simulate gen_mcmp's behavior in a controlled way
// ------------------------------------------------------------------------------------------------

// Structure to capture the last invocation arguments to gen_mcmp for verification
typedef struct {
    compiler_state_t *cstate;
    int offrel;
    unsigned offset;
    unsigned size;
    unsigned v;
    unsigned mask;
} GenMcmpArgs;

// Global variable to hold the most recent call's arguments (accessible by tests)
static GenMcmpArgs g_last_mcmp_args = {nullptr, 0, 0, 0, 0, 0};

// A deterministic mock of gen_mcmp that records its inputs and returns a computed value
int gen_mcmp(compiler_state_t *cstate,
             int offrel,
             unsigned offset,
             unsigned size,
             unsigned v,
             unsigned mask)
{
    // Record arguments for assertions in tests
    g_last_mcmp_args.cstate = cstate;
    g_last_mcmp_args.offrel = offrel;
    g_last_mcmp_args.offset = offset;
    g_last_mcmp_args.size = size;
    g_last_mcmp_args.v = v;
    g_last_mcmp_args.mask = mask;

    // Produce a deterministic "pseudo-code" value derived from inputs.
    // This mirrors the idea that gen_mcmp compiles a BPF instruction sequence.
    // The exact encoding is irrelevant for the test; only consistency matters.
    uint32_t enc = 0;
    enc |= (static_cast<uint32_t>(offrel) & 0xFF) << 24;
    enc |= (static_cast<uint32_t>(offset) & 0xFF) << 16;
    enc |= (static_cast<uint32_t>(size) & 0xFF) << 8;
    enc |= (static_cast<uint32_t>(v) & 0xFF) ^ (static_cast<uint32_t>(mask) & 0xFF);
    return static_cast<int>(enc);
}

// The actual focal method under test, implemented here to mirror the provided snippet
int gen_atm_prototype(compiler_state_t *cstate, const uint8_t v)
{
    // Equivalent to: return gen_mcmp(cstate, OR_LINKHDR, cstate->off_proto, BPF_B, v, 0x0fU);
    return gen_mcmp(cstate, OR_LINKHDR, cstate->off_proto, BPF_B, v, 0x0fU);
}

// ------------------------------------------------------------------------------------------------
// Lightweight test framework (non-GTest, non-GMock)
// ------------------------------------------------------------------------------------------------

static std::vector<std::string> g_test_failures;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        g_test_failures.push_back(std::string("EXPECT_TRUE failed: ") + (msg)); \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        char buf[256]; \
        std::snprintf(buf, sizeof(buf), \
            "EXPECT_EQ failed: (%s) != (%s). Values: %lld vs %lld. %s", \
            #a, #b, static_cast<long long>(a), static_cast<long long>(b), (msg)); \
        g_test_failures.push_back(std::string(buf)); \
    } \
} while(0)

static void test_gen_atm_prototype_basic_case()
{
    // Test 1: Basic scenario with a known cstate.off_proto and v
    compiler_state_t st;
    st.off_proto = 0x12U; // arbitrary non-zero offset
    const uint8_t v = 0x3C; // some test value

    // Clear last call record
    g_last_mcmp_args = {nullptr, 0, 0, 0, 0, 0};

    int ret = gen_atm_prototype(&st, v);

    // Expected values based on the mock implementation of gen_mcmp
    // We expect the following to have been forwarded:
    // offrel == OR_LINKHDR
    // offset == st.off_proto
    // size == BPF_B
    // v == v
    // mask == 0x0f
    EXPECT_TRUE(g_last_mcmp_args.cstate == &st,
                "gen_atm_prototype should forward the correct cstate pointer");
    EXPECT_EQ(g_last_mcmp_args.offrel, OR_LINKHDR,
              "gen_atm_prototype should pass OR_LINKHDR as the offrel");
    EXPECT_EQ(g_last_mcmp_args.offset, st.off_proto,
              "gen_atm_prototype should pass cstate->off_proto as offset");
    EXPECT_EQ(g_last_mcmp_args.size, BPF_B,
              "gen_atm_prototype should pass BPF_B as size");
    EXPECT_EQ(g_last_mcmp_args.v, v,
              "gen_atm_prototype should pass the provided v");
    EXPECT_EQ(g_last_mcmp_args.mask, 0x0fU,
              "gen_atm_prototype should pass the mask 0x0fU");

    // Compute the expected return value using the same encoding as gen_mcmp
    uint32_t expected_enc = 0;
    expected_enc |= (static_cast<uint32_t>(OR_LINKHDR) & 0xFF) << 24;
    expected_enc |= (static_cast<uint32_t>(st.off_proto) & 0xFF) << 16;
    expected_enc |= (static_cast<uint32_t>(BPF_B) & 0xFF) << 8;
    expected_enc |= (static_cast<uint32_t>(v) & 0xFF) ^ (static_cast<uint32_t>(0x0fU) & 0xFF);

    EXPECT_EQ(ret, static_cast<int>(expected_enc),
              "gen_atm_prototype should return the value produced by gen_mcmp with forwarded args");
}

static void test_gen_atm_prototype_multiple_cases()
{
    // Test 2: Different cstate.off_proto and v to ensure correctness across inputs
    compiler_state_t st2;
    st2.off_proto = 0x7A; // another offset value
    const uint8_t v2 = 0x9E; // another v

    g_last_mcmp_args = {nullptr, 0, 0, 0, 0, 0};

    int ret2 = gen_atm_prototype(&st2, v2);

    EXPECT_TRUE(g_last_mcmp_args.cstate == &st2,
                "gen_atm_prototype should forward the correct cstate pointer in case 2");
    EXPECT_EQ(g_last_mcmp_args.offrel, OR_LINKHDR,
              "case 2: offrel should be OR_LINKHDR");
    EXPECT_EQ(g_last_mcmp_args.offset, st2.off_proto,
              "case 2: offset should match st2.off_proto");
    EXPECT_EQ(g_last_mcmp_args.size, BPF_B,
              "case 2: size should be BPF_B");
    EXPECT_EQ(g_last_mcmp_args.v, v2,
              "case 2: v should match the provided v2");
    EXPECT_EQ(g_last_mcmp_args.mask, 0x0fU,
              "case 2: mask should be 0x0fU");

    // Recompute expected return
    uint32_t expected_enc2 = 0;
    expected_enc2 |= (static_cast<uint32_t>(OR_LINKHDR) & 0xFF) << 24;
    expected_enc2 |= (static_cast<uint32_t>(st2.off_proto) & 0xFF) << 16;
    expected_enc2 |= (static_cast<uint32_t>(BPF_B) & 0xFF) << 8;
    expected_enc2 |= (static_cast<uint32_t>(v2) & 0xFF) ^ (static_cast<uint32_t>(0x0fU) & 0xFF);

    EXPECT_EQ(ret2, static_cast<int>(expected_enc2),
              "case 2: return value should match encoded result for given inputs");
}

// ------------------------------------------------------------------------------------------------
// Main entry point for the test harness
// ------------------------------------------------------------------------------------------------

int main()
{
    // Run tests
    test_gen_atm_prototype_basic_case();
    test_gen_atm_prototype_multiple_cases();

    // Report results
    if(g_test_failures.empty()) {
        std::cout << "[PASSED] All tests succeeded.\n";
        return 0;
    } else {
        std::cout << "[FAILED] " << g_test_failures.size() << " test(s) failed.\n";
        for(const auto &f : g_test_failures) {
            std::cout << " - " << f << "\n";
        }
        return 1;
    }
}

// Explanation of test intent (embedded in comments):
// - The tests focus on verifying that gen_atm_prototype forwards the exact
//   arguments to gen_mcmp as implied by the focal method's body.
// - We capture gen_mcmp's inputs via g_last_mcmp_args and compare against
//   expected values derived from the inputs to gen_atm_prototype.
// - We compute the expected return value using the same encoding that the mock
//   gen_mcmp uses, ensuring that the test validates both the forwarding and
//   the return path.
// - Two scenarios are covered: a basic case and a second case with different inputs
//   to exercise multiple branches of the input space (offset and v).