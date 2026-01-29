/*
Test Suite for the focal method: initchunks(compiler_state_t *cstate)
Context:
- Focal function initializes the per-chunk state for NCHUNKS chunks:
  for i in 0..NCHUNKS-1:
      cstate->chunks[i].n_left = 0;
      cstate->chunks[i].m = NULL;
  cstate->cur_chunk = 0;

- Core dependencies observed from the class/header block:
  Candidate Keywords: initchunks, compiler_state_t, NCHUNKS, chunks, n_left, m, cur_chunk

This test suite is designed to be C++11 compatible, without Google Test (GTest),
and uses a lightweight, self-contained test harness with non-terminating expectations
(EXPECT_*) to exercise the focal method and validate its post-conditions.

Note: We rely on the project-provided header gencode.h to declare compiler_state_t,
the chunks structure, and the initchunks function. The tests are written to work
with the existing project build (i.e., they should compile in the same environment
as the original codebase).
*/

// Expose C interface for the focal function and related types
extern "C" {
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
#include <setjmp.h>
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

}

// Lightweight test harness (non-terminating assertions)

// Expected "true" test pass flag for each test
#define EXPECT_TRUE(cond, msg)                                  \
    do { if (!(cond)) {                                        \
            std::cerr << "EXPECT_TRUE FAILED: " << (msg)      \
                      << " [at " << __FILE__ << ":" << __LINE__ << "]\n"; \
            ++g_total_errors;                                 \
        } } while (0)

#define EXPECT_EQ(a, b, msg)                                      \
    do { if (!((a) == (b))) {                                      \
            std::cerr << "EXPECT_EQ FAILED: " << (msg)             \
                      << " | " << #a << "=" << (a)                \
                      << " vs " << #b << "=" << (b)               \
                      << " [at " << __FILE__ << ":" << __LINE__ << "]\n"; \
            ++g_total_errors;                                     \
        } } while (0)

// Global error counter for the test suite
static int g_total_errors = 0;

/*
Test 1: Basic initialization correctness
- Precondition: Fill all chunks with non-zero n_left and non-NULL m to ensure
  the initchunks function overwrites them.
- Postcondition: Every chunk's n_left should be 0; every chunk's m should be NULL;
  cur_chunk should be reset to 0.
*/
bool test_initchunks_basic() {
    // Compile-time check: NCHUNKS must be > 0 for meaningful testing
    static_assert(NCHUNKS > 0, "NCHUNKS must be > 0 for initchunks test");

    compiler_state_t cstate;

    // Initialize with non-default (to ensure the function writes over existing state)
    for (size_t i = 0; i < NCHUNKS; ++i) {
        cstate.chunks[i].n_left = 12345;
        cstate.chunks[i].m = (void*)0xDEADBEEF;
    }
    cstate.cur_chunk = 999; // Arbitrary non-zero value to verify reset

    // Call the focal function
    initchunks(&cstate);

    // Verify all chunks are reset
    for (size_t i = 0; i < NCHUNKS; ++i) {
        EXPECT_EQ(cstate.chunks[i].n_left, 0, "n_left should be reset to 0 for each chunk");
        EXPECT_TRUE(cstate.chunks[i].m == NULL, "m should be reset to NULL for each chunk");
    }

    // Verify cur_chunk reset
    EXPECT_EQ(cstate.cur_chunk, 0, "cur_chunk should be reset to 0");

    return g_total_errors == 0;
}

/*
Test 2: cur_chunk reset is independent of initial value
- Precondition: Set cur_chunk to a non-zero value (simulate a mid-processing state).
- Postcondition: After initchunks, cur_chunk must be exactly 0.
*/
bool test_initchunks_cur_chunk_reset() {
    // Compile-time assertion
    static_assert(NCHUNKS > 0, "NCHUNKS must be > 0 for initchunks test");

    compiler_state_t cstate;

    // Prepare a non-zero cur_chunk to ensure the reset behavior is exercised
    cstate.cur_chunk = 7;

    // Initialize some chunk state to non-default values
    for (size_t i = 0; i < NCHUNKS; ++i) {
        cstate.chunks[i].n_left = -1;
        cstate.chunks[i].m = (void*)0x1;
    }

    initchunks(&cstate);

    // Only check cur_chunk in this test
    EXPECT_EQ(cstate.cur_chunk, 0, "cur_chunk must be reset to 0 by initchunks");

    // Also ensure that the per-chunk initialization occurs as a side-effect
    for (size_t i = 0; i < NCHUNKS; ++i) {
        EXPECT_EQ(cstate.chunks[i].n_left, 0, "n_left should be reset to 0 for each chunk (side-effect check)");
        EXPECT_TRUE(cstate.chunks[i].m == NULL, "m should be reset to NULL for each chunk (side-effect check)");
    }

    return g_total_errors == 0;
}

// Entry point for test execution
int main() {
    std::cout << std::boolalpha;
    std::cout << "Running tests for initchunks(compiler_state_t*)." << std::endl;

    bool ok1 = test_initchunks_basic();
    std::cout << "Test 1 (basic initialization) - " << (ok1 ? "PASSED" : "FAILED") << std::endl;

    // Reset error counter for isolated test result readability
    int saved_errors = g_total_errors;
    g_total_errors = 0;

    bool ok2 = test_initchunks_cur_chunk_reset();
    std::cout << "Test 2 (cur_chunk reset) - " << (ok2 ? "PASSED" : "FAILED") << std::endl;

    // Aggregate results
    int all_passed = (ok1 ? 0 : 1) + (ok2 ? 0 : 1) + (g_total_errors > 0 ? 1 : 0);
    if (all_passed == 0) {
        std::cout << "ALL TESTS PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed. See messages above for details." << std::endl;
        return 1;
    }
}