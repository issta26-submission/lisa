/*
 * Unit tests for gen_vlan_patch_vid_test (gencode.c)
 * - No GoogleTest; a lightweight, non-terminating assertion framework is used.
 * - Tests are written in C++11 but exercise C code and data structures from the project.
 * - The tests rely on the project exposed API (gencode.h, compiler_state_t, struct block, struct slist, etc.).
 * - Test cases cover typical scenarios to improve coverage of gen_vlan_patch_vid_test behavior.
 * - Each test includes explanatory comments describing intent and expected outcomes.
 *
 * How to run (conceptual):
 * - Compile with the project sources and link this test file against the same build (same include paths and macros).
 * - Run the resulting executable; it will print test results and a summary count of failures.
 *
 * Notes:
 * - This harness uses a small, non-terminating assertion set (EXPECT_*) to maximize code paths execution.
 * - Static members and internal file-scope behavior are exercised by interacting with the public API the production code exposes.
 * - This test assumes Linux-like BPF related macros (e.g., BPF_LD, BPF_ABS, SKF_AD_OFF, etc.) are available via included headers in the project.
 * - If the project uses different include paths, adjust the include lines accordingly.
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


// Bring in the project's C API (gen_vlan_patch_vid_test and related types).
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_test_fail_count = 0;

#define LOG_TEST(msg) do { std::cout << "[TEST] " << msg << std::endl; } while (0)
#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " // " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")" \
                  << std::endl; \
        ++g_test_fail_count; \
    } \
} while (0)
#define EXPECT_NEQ(a, b) do { \
    if ((a) == (b)) { \
        std::cerr << "EXPECT_NEQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " // " << #a << " (" << (a) << ") == " << #b << " (" << (b) << ")" \
                  << std::endl; \
        ++g_test_fail_count; \
    } \
} while (0)
#define EXPECT_NOT_NULL(a) do { \
    if ((a) == NULL) { \
        std::cerr << "EXPECT_NOT_NULL failed at " << __FILE__ << ":" << __LINE__ \
                  << " // " << #a << " is NULL" << std::endl; \
        ++g_test_fail_count; \
    } \
} while (0)

//
// Helper: create an old head block (b_vid->stmts) with n statements.
// Each statement is a valid BPF instruction so that new_stmt() can be used.
// The exact content of these old statements is not important for the test logic;
// we just need a non-empty list to exercise the patch insertion logic.
//
static struct slist* make_old_head(compiler_state_t *cstate, int n) {
    if (n <= 0) return NULL;

    struct slist *head = NULL;
    struct slist *cur = NULL;

    for (int i = 0; i < n; ++i) {
        // Create a placeholder statement; content is not important for the test,
        // but we use a plausible BPF opcode combination to ensure validity.
        struct slist *node = new_stmt(cstate, BPF_LD|BPF_B|BPF_ABS);
        // Assign a non-zero k to avoid NULL or zero-magic values.
        node->s.k = (bpf_u_int32)(i + 1);
        node->next = NULL;
        if (!head) head = node;
        else cur->next = node;
        cur = node;
    }
    return head;
}

// Test harness: a small wrapper to create a fresh compiler state and a block
// with a given number of existing statements.
static void setup_block_with_old_head(compiler_state_t *cstate, int old_head_count,
                                      struct block **out_b, struct slist **out_old_head) {
    struct block *b = new_block(cstate, 0); // create empty block
    b->stmts = make_old_head(cstate, old_head_count);
    *out_b = b;
    *out_old_head = b->stmts;
}

// Test 1: Verify gen_vlan_patch_vid_test inserts the exact patch sequence
// when there are two existing statements in the b_vid block.
static void test_gen_vlan_patch_vid_two_old_stmts() {
    LOG_TEST("test_gen_vlan_patch_vid_two_old_stmts: patch path with 2 pre-existing stmts");

    compiler_state_t *cs = new compiler_state_t(); // create a fresh compiler state
    struct block *b_vid = NULL;
    struct slist *old_head = NULL;

    setup_block_with_old_head(cs, 2, &b_vid, &old_head);

    // Call the focal function
    gen_vlan_patch_vid_test(cs, b_vid);

    // Traverse the new chain and validate expected injection points.
    struct slist *head = b_vid->stmts;
    EXPECT_NOT_NULL(head);

    // 1) First injected statement: BPF_LD|BPF_B|BPF_ABS with k = SKF_AD_OFF + SKF_AD_VLAN_TAG_PRESENT
    EXPECT_EQ(head->s.k, (bpf_u_int32)(SKF_AD_OFF + SKF_AD_VLAN_TAG_PRESENT));

    // 2) Jump-if-equal to VLAN tag test
    struct slist *sjeq = head->next;
    EXPECT_NOT_NULL(sjeq);
    EXPECT_EQ(sjeq->s.k, (bpf_u_int32)1);

    // sjeq's false path (jf) should point to the original b_vid stmts head
    EXPECT_EQ(sjeq->s.jf, old_head);

    // 3) sjeq's true path should jump to VLAN tag load
    struct slist *s_vlan_load = sjeq->s.jt;
    EXPECT_NOT_NULL(s_vlan_load);
    EXPECT_EQ(s_vlan_load->s.k, (bpf_u_int32)(SKF_AD_OFF + SKF_AD_VLAN_TAG));

    // 4) End patch jump: k should be (cnt - 1) where cnt is number of original stmts (2)
    struct slist *s_end = s_vlan_load->next;
    EXPECT_NOT_NULL(s_end);
    EXPECT_EQ(s_end->s.k, (bpf_u_int32)1); // 2 - 1 = 1

    // The end patch should point to the original head
    EXPECT_EQ(s_end->next, old_head);

    // Clean up not strictly necessary for test exit, but good practice in real tests.
}

// Test 2: Verify behavior when there is exactly one existing statement.
// This confirms the other branch of the cnt calculation (cnt-1 == 0).
static void test_gen_vlan_patch_vid_one_old_stmt() {
    LOG_TEST("test_gen_vlan_patch_vid_one_old_stmt: patch path with 1 pre-existing stmt");

    compiler_state_t *cs = new compiler_state_t();
    struct block *b_vid = NULL;
    struct slist *old_head = NULL;

    setup_block_with_old_head(cs, 1, &b_vid, &old_head);

    gen_vlan_patch_vid_test(cs, b_vid);

    struct slist *head = b_vid->stmts;
    EXPECT_NOT_NULL(head);
    EXPECT_EQ(head->s.k, (bpf_u_int32)(SKF_AD_OFF + SKF_AD_VLAN_TAG_PRESENT));

    struct slist *sjeq = head->next;
    EXPECT_NOT_NULL(sjeq);
    EXPECT_EQ(sjeq->s.k, (bpf_u_int32)1);
    EXPECT_EQ(sjeq->s.jf, old_head);

    struct slist *s_vlan_load = sjeq->s.jt;
    EXPECT_NOT_NULL(s_vlan_load);
    EXPECT_EQ(s_vlan_load->s.k, (bpf_u_int32)(SKF_AD_OFF + SKF_AD_VLAN_TAG));

    struct slist *s_end = s_vlan_load->next;
    EXPECT_NOT_NULL(s_end);
    EXPECT_EQ(s_end->s.k, (bpf_u_int32)0); // 1 - 1 = 0

    EXPECT_EQ(s_end->next, old_head);
}

// Test 3: Optional edge-case: zero old statements (empty b_vid original block).
// This exercises the behavior cnt == 0 path (end patch k becomes -1 in unsigned form).
static void test_gen_vlan_patch_vid_zero_old_stmts() {
    LOG_TEST("test_gen_vlan_patch_vid_zero_old_stmts: patch path with 0 pre-existing stmts");

    compiler_state_t *cs = new compiler_state_t();
    struct block *b_vid = NULL;
    setup_block_with_old_head(cs, 0, &b_vid, NULL);

    gen_vlan_patch_vid_test(cs, b_vid);

    struct slist *head = b_vid->stmts;
    EXPECT_NOT_NULL(head);
    EXPECT_EQ(head->s.k, (bpf_u_int32)(SKF_AD_OFF + SKF_AD_VLAN_TAG_PRESENT));

    struct slist *sjeq = head->next;
    EXPECT_NOT_NULL(sjeq);
    EXPECT_EQ(sjeq->s.k, (bpf_u_int32)1);
    EXPECT_EQ(sjeq->s.jf, NULL); // no original head

    struct slist *s_vlan_load = sjeq->s.jt;
    EXPECT_NOT_NULL(s_vlan_load);
    EXPECT_EQ(s_vlan_load->s.k, (bpf_u_int32)(SKF_AD_OFF + SKF_AD_VLAN_TAG));

    struct slist *s_end = s_vlan_load->next;
    EXPECT_NOT_NULL(s_end);
    // cnt == 0 => end k == -1 (unsigned 0xFFFFFFFF)
    EXPECT_EQ(s_end->s.k, (bpf_u_int32)(-1));

    EXPECT_EQ(s_end->next, NULL);
}

// Utility: Run all tests and report results
static void run_all_tests() {
    test_gen_vlan_patch_vid_two_old_stmts();
    test_gen_vlan_patch_vid_one_old_stmt();
    test_gen_vlan_patch_vid_zero_old_stmts();
}

int main() {
    LOG_TEST("Starting gen_vlan_patch_vid_test suite");
    run_all_tests();
    if (g_test_fail_count == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_fail_count << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}