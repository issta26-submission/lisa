/*
Unit test suite for the focal method: free_reg(compiler_state_t *cstate, int n)
This test is written in C++11 (no Google Test) and links against the C code
in gencode.c via the corresponding header gencode.h.

Tests cover:
- The primary side-effect: cstate->regused[n] is cleared (set to 0).
- Non-destructive behavior for other registers (unaffected).
- Idempotence when the targeted register is already free.
- Basic behavior when the targeted register currently holds a non-zero value.

Notes:
- We assume the compiler_state_t struct contains an array regused that is indexable
  as regused[0], regused[1], ..., regused[N-1]. The tests use indices 0 and 15
  (typical values for a small register pool in BPF/JIT contexts). If your
  environment uses a different sized register pool, adjust the indices accordingly.
- The tests initialize the C state to zeros to keep gen_len/side-effects harmless
  and then verify the effect of free_reg on regused[n] only.
- No terminating tests: we continue running all tests even if one fails, to maximize coverage.

Compile-time prerequisites:
- Ensure the build links in gencode.c (or its object) and uses gencode.h for type
  declarations. The test file includes the header inside extern "C" to maintain
  C linkage for the focal function.

Code starts here:
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
#include <cstring>


// Include the C headers with C linkage for the focal function and types.
// We wrap the header in extern "C" to avoid C++ name mangling for C symbols.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_pass = 0;

#define TEST_ASSERT(cond, msg) do { \
    g_tests_run++; \
    if (cond) { \
        g_tests_pass++; \
    } else { \
        std::cerr << "Test failed: " << msg << std::endl; \
    } \
} while (0)

// Test 1: free_reg should clear regused[n] from non-zero to zero
static void test_free_reg_clears_target_register() {
    compiler_state_t cstate;
    // Zero initialize to avoid unintended side-effects from uninitialized data
    std::memset(&cstate, 0, sizeof(cstate));

    // Prepare: mark register 0 as used (non-zero)
    cstate.regused[0] = 1;

    // Act
    free_reg(&cstate, 0);

    // Assert
    TEST_ASSERT(cstate.regused[0] == 0, "regused[0] should be cleared to 0 by free_reg");
}

// Test 2: free_reg should clear regused[15] when targeted (typical max reg index)
static void test_free_reg_clears_high_index() {
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // Prepare: mark register 15 as used
    cstate.regused[15] = 2;

    // Act
    free_reg(&cstate, 15);

    // Assert
    TEST_ASSERT(cstate.regused[15] == 0, "regused[15] should be cleared to 0 by free_reg");
}

// Test 3: Other registers remain unaffected after freeing a specific register
static void test_free_reg_untouched_other_regs() {
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // Prepare: set several registers to distinct non-zero values
    cstate.regused[0] = 7;
    cstate.regused[1] = 11;
    cstate.regused[2] = 13;

    // Act: free only reg 1
    free_reg(&cstate, 1);

    // Assert: reg 1 cleared, others unchanged
    bool ok = (cstate.regused[1] == 0) && (cstate.regused[0] == 7) && (cstate.regused[2] == 13);
    TEST_ASSERT(ok, "free_reg should only clear the targeted register and leave others intact");
}

// Test 4: Idempotence: freeing an already-free register should leave it free
static void test_free_reg_idempotence() {
    compiler_state_t cstate;
    std::memset(&cstate, 0, sizeof(cstate));

    // reg 0 is already 0
    cstate.regused[0] = 0;

    // Act
    free_reg(&cstate, 0);

    // Assert
    TEST_ASSERT(cstate.regused[0] == 0, "free_reg should be idempotent when reg is already free");
}

// A simple test runner
static void run_all_tests() {
    std::cout << "Starting tests for free_reg...\n";

    test_free_reg_clears_target_register();
    test_free_reg_clears_high_index();
    test_free_reg_untouched_other_regs();
    test_free_reg_idempotence();

    std::cout << "Tests completed. Passed " << g_tests_pass << " of " << g_tests_run << " tests.\n";

    // Expose results via exit code for automation (0 = all passed)
    // If any test failed, return non-zero.
    if (g_tests_pass != g_tests_run) {
        std::exit(1);
    }
}

int main() {
    run_all_tests();
    return 0;
}

/*
Explanatory notes:
- The tests rely on the assumption that free_reg primarily modifies cstate->regused[n].
- We initialize the compiler_state_t to zeros to avoid uninitialized reads in the
  downstream gen_len call that occurs after the reg is freed.
- Indices used (0 and 15) are representative of common small register pools in
  BPF/packet-capture codegen contexts. If your environment uses a different max
  index, adjust the tests accordingly (e.g., test reg 7, reg 12, etc.).
- The test suite uses a minimal, non-terminating assertion style to maximize
  code coverage while preserving a simple, std::cout/std::cerr driven report.
- This test suite avoids private API exposure and relies only on public declarations
  provided by gencode.h (as included via extern "C" linkage).
*/