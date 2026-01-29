// C++11 unit tests for the focal method gen_uncond in gencode.c
// Test harness is written without Google Test, using lightweight
// non-terminating assertions to maximize code execution and coverage.
// The tests assume the project under test provides the C headers and
// symbols (e.g., compiler_state_t, block, new_compiler_state, gen_uncond,
// gen_jmp_k, new_stmt, IS_TRUE, IS_FALSE, BPF constants) as in the
// provided source. The test links against the existing C codebase.

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


// Bring in the C declarations with C linkage
extern "C" {
}

/*
Candidate Keywords (derived from Step 1 analysis):
- compiler_state_t
- new_compiler_state (state creation)
- gen_uncond (target function)
- BPF_LD, BPF_IMM (instruction flags used by gen_uncond)
- new_stmt (to create s with BPF_LD|BPF_IMM)
- s->s.k (the immediate operand of the statement)
- gen_jmp_k (to create the jump block)
- block (return type with meaning field)
- IS_TRUE, IS_FALSE (meaning values assigned in gen_uncond)
- rsense (input predicate)
These keywords reflect the core components involved in gen_uncond's logic.
*/

// Lightweight, non-terminating test assertions
static int g_failure_count = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE FAILED: %s (at %s:%d)\n", (msg), __FILE__, __LINE__); \
        ++g_failure_count; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        fprintf(stderr, "EXPECT_EQ FAILED: %s (expected %ld, got %ld) (at %s:%d)\n", \
                (msg), static_cast<long>(b), static_cast<long>(a), __FILE__, __LINE__); \
        ++g_failure_count; \
    } \
} while (0)

// Test 1: rsense = 0 should yield IS_FALSE in the returned block's meaning
// Also ensures that the function returns a non-null block pointer.
static void test_gen_uncond_false_branch() {
    // Create a compiler state using the project's API
    compiler_state_t *cstate = new_compiler_state();
    EXPECT_TRUE(cstate != NULL, "new_compiler_state() should not return NULL");

    // Call gen_uncond with rsense = 0 (false branch)
    struct block *ret = gen_uncond(cstate, 0);
    EXPECT_TRUE(ret != NULL, "gen_uncond(cstate, 0) should return non-NULL block");

    // The implementation assigns: ret->meaning = IS_TRUE if rsense != 0, else IS_FALSE
    // Therefore for rsense == 0, we expect IS_FALSE
    EXPECT_EQ((int)ret->meaning, (int)IS_FALSE, "ret->meaning should be IS_FALSE when rsense == 0");

    // Note: We intentionally do not free memory to keep test simple and focused
    // on functional correctness of the focal path.
}

// Test 2: rsense = 1 should yield IS_TRUE in the returned block's meaning
// Verifies the true branch behavior and that the function is consistent.
static void test_gen_uncond_true_branch() {
    compiler_state_t *cstate = new_compiler_state();
    EXPECT_TRUE(cstate != NULL, "new_compiler_state() should not return NULL");

    // Call gen_uncond with rsense = 1 (true branch)
    struct block *ret = gen_uncond(cstate, 1);
    EXPECT_TRUE(ret != NULL, "gen_uncond(cstate, 1) should return non-NULL block");

    // For rsense != 0, ret->meaning should be IS_TRUE
    EXPECT_EQ((int)ret->meaning, (int)IS_TRUE, "ret->meaning should be IS_TRUE when rsense != 0");
}

// Domain knowledge notes for maintainers (comments):
// - The tests rely on the fact that gen_uncond constructs a statement (s) with
//   BPF_LD|BPF_IMM, assigns s->s.k to !rsense, and returns a jump block with
//   its meaning field set to IS_TRUE if rsense is non-zero, otherwise IS_FALSE.
// - We test both branches of the rsense predicate to ensure full predicate coverage.
// - We are using only the public API exposed by the C headers; static/internal
//   helpers in gencode.c are not accessed directly, matching the guideline
//   to avoid private members in tests.

// Simple test harness runner
int main() {
    // Run tests that cover both branches of the focal method
    test_gen_uncond_false_branch();
    test_gen_uncond_true_branch();

    if (g_failure_count == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Total failures: %d\n", g_failure_count);
        return 1;
    }
}