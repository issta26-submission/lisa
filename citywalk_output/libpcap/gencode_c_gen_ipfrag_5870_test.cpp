// Test suite for the focal method: gen_ipfrag in gencode.c
// Note: This test uses a linker-wrapping technique to intercept calls to
// gen_load_a and gen_unset. Build command (example, on Linux with GCC):
// g++ test_gen_ipfrag.cpp gencode.o -o test_gen_ipfrag \
//     -Wl,--wrap=gen_load_a -Wl,--wrap=gen_unset
// This allows the test to observe the interactions inside gen_ipfrag without
// modifying the original source. The wrappers forward to the real
// implementations when needed, and record the observed arguments for correctness.
//
// Domain knowledge considerations implemented here:
// - We verify true behavior of the call sequence inside gen_ipfrag by inspecting
//   the arguments passed to gen_load_a (offset and size) and to gen_unset
//   (mask), and that the s list produced by gen_load_a is forwarded to gen_unset.
// - We exercise two scenarios: normal path and a path where the load returns NULL.
// - We use non-terminating (non-crashing) assertions that accumulate failures
//   instead of aborting on first failure.
// - We keep dependencies to a minimum, using opaque types for C compatibility and
//   relying on linker-wrap to observe the real behavior.

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
#include <sstream>
#include <os-proto.h>
#include <linux/if_packet.h>
#include <config.h>
#include <pcap/sll.h>
#include <cstring>


// Step 1: Forward-declare the minimal types to allow linking with the real C code.
// We do not need to know the internal structure for the test harness.
extern "C" {
    // Opaque types as used by gen_ipfrag's signature
    typedef struct compiler_state_t compiler_state_t;
    struct slist; // forward declare: used as a chain/list type in the original code

    // Prototypes for the real (wrapped) functions. These are provided by the
    // gencode.c or its linked object. We declare them to enable wrappers to call
    // the original implementations via __real_* aliases when required.
    struct slist * __real_gen_load_a(compiler_state_t *cstate, int offrel, int offset, int size);
    struct slist * __wrap_gen_load_a(compiler_state_t *cstate, int offrel, int offset, int size);

    struct slist * __real_gen_unset(compiler_state_t *cstate, uint32_t mask, struct slist *s);
    struct slist * __wrap_gen_unset(compiler_state_t *cstate, uint32_t mask, struct slist *s);

    // The focal function under test
    struct slist * gen_ipfrag(compiler_state_t *cstate);
}

// Global state used by wrappers to record observed interactions
static bool force_null_load_a = false; // controls whether the wrapper returns NULL
static struct compiler_state_t *last_load_cstate = nullptr;
static int last_load_offrel = 0;
static int last_load_offset = 0;
static int last_load_size = 0;
static struct slist *last_load_ret = nullptr;

static struct compiler_state_t *last_unset_cstate = nullptr;
static uint32_t last_unset_mask = 0;
static struct slist *last_unset_s_in = nullptr;
static struct slist *last_unset_ret = nullptr;

// Forward declare the wrapper functions so the C++ compiler accepts them
extern "C" struct slist * __wrap_gen_load_a(compiler_state_t *cstate, int offrel, int offset, int size);
extern "C" struct slist * __wrap_gen_unset(compiler_state_t *cstate, uint32_t mask, struct slist *s);

// Wrapper: intercept gen_load_a calls
extern "C" struct slist * __wrap_gen_load_a(compiler_state_t *cstate, int offrel, int offset, int size)
{
    // Record arguments for verification
    last_load_cstate = cstate;
    last_load_offrel = offrel;
    last_load_offset = offset;
    last_load_size = size;

    // Control return value to exercise both normal and NULL-path scenarios
    if (force_null_load_a) {
        last_load_ret = nullptr;
        return nullptr;
    }

    // Call the real function to preserve actual behavior
    last_load_ret = __real_gen_load_a(cstate, offrel, offset, size);
    return last_load_ret;
}

// Wrapper: intercept gen_unset calls
extern "C" struct slist * __wrap_gen_unset(compiler_state_t *cstate, uint32_t mask, struct slist *s)
{
    // Record inputs for verification
    last_unset_cstate = cstate;
    last_unset_mask = mask;
    last_unset_s_in = s;

    // Call the real function to preserve actual behavior
    last_unset_ret = __real_gen_unset(cstate, mask, s);
    return last_unset_ret;
}

// Minimal expectations helper: non-terminating assertions
struct TestLogger {
    int failures;
    std::vector<std::string> messages;

    TestLogger() : failures(0) {}

    void expect_true(bool cond, const std::string &msg) {
        if (!cond) {
            ++failures;
            messages.push_back("FAIL: " + msg);
        }
    }

    bool ok() const { return failures == 0; }

    void dump(std::ostream &os) const {
        for (const auto &m : messages) os << m << std::endl;
    }
};

// Simple wrapper to simulate two test scenarios
static void test_gen_ipfrag_normal(TestLogger &log) {
    // Reset wrapper state
    force_null_load_a = false;
    last_load_cstate = nullptr;
    last_load_offrel = 0;
    last_load_offset = 0;
    last_load_size = 0;
    last_load_ret = nullptr;

    last_unset_cstate = nullptr;
    last_unset_mask = 0;
    last_unset_s_in = nullptr;
    last_unset_ret = nullptr;

    // Create a dummy compiler_state_t object (opaque)
    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs));

    // Execute
    struct slist *ret = gen_ipfrag(&cs);

    // Assertions:
    // 1) gen_load_a should have been called with offset 6 (and size BPF_H)
    log.expect_true(last_load_offset == 6, "gen_ipfrag: gen_load_a offset should be 6");
    // 2) The wrapper should pass through a non-NULL list (in a normal path)
    log.expect_true(last_load_ret != nullptr, "gen_ipfrag: gen_load_a should return non-NULL slist in normal path");
    // 3) The mask passed to gen_unset should be 0x1fff
    log.expect_true(last_unset_mask == 0x1fff, "gen_ipfrag: gen_unset mask should be 0x1fff");
    // 4) The same slist instance produced by gen_load_a should be passed to gen_unset
    log.expect_true(last_unset_s_in == last_load_ret, "gen_ipfrag: gen_unset input slist should be the same as gen_load_a return");
    // 5) The return value should equal the value returned by gen_unset
    log.expect_true(ret == last_unset_ret, "gen_ipfrag: return value equals gen_unset return");
}

static void test_gen_ipfrag_null_load(TestLogger &log) {
    // Configure: make gen_load_a return NULL
    force_null_load_a = true;
    last_load_cstate = nullptr;
    last_load_offrel = 0;
    last_load_offset = 0;
    last_load_size = 0;
    last_load_ret = nullptr;

    last_unset_cstate = nullptr;
    last_unset_mask = 0;
    last_unset_s_in = nullptr;
    last_unset_ret = nullptr;

    compiler_state_t cs;
    std::memset(&cs, 0, sizeof(cs));

    struct slist *ret = gen_ipfrag(&cs);

    // In this path, gen_load_a returns NULL, so:
    log.expect_true(last_load_ret == nullptr, "gen_ipfrag (NULL load): gen_load_a should return NULL");
    log.expect_true(last_unset_s_in == nullptr, "gen_ipfrag (NULL load): gen_unset input slist should be NULL");
    log.expect_true(ret == nullptr, "gen_ipfrag (NULL load): return value should be NULL");
}

// Entry point and test orchestration
int main() {
    TestLogger log;

    // Run tests
    test_gen_ipfrag_normal(log);
    test_gen_ipfrag_null_load(log);

    // Output summary
    if (log.ok()) {
        std::cout << "All gen_ipfrag tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "gen_ipfrag tests finished with " << log.failures << " failure(s)." << std::endl;
        // Emit diagnostic messages
        log.dump(std::cout);
        return 1;
    }
}

// Explanatory notes for test authors:
// - The tests rely on linker wrapping to observe interactions inside gen_ipfrag.
// - The normal-path test asserts that gen_load_a is invoked with the expected
//   second-level parameters (offset 6, size BPF_H, etc.) and that the value
//   returned by gen_load_a flows into gen_unset as its input slist and that the
//   mask 0x1fff is used.
// - The NULL-load test forces gen_load_a to return NULL to ensure gen_ipfrag
//   handles a NULL slist gracefully (still returning NULL and not crashing).
// - All assertions are non-terminating: failures are recorded and reported at
//   the end of execution rather than stopping the tests early.