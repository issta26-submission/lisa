// C++11 unit tests for the focal method opt_peep (located in optimize.c)
// Notes:
// - This test suite is designed to be compiled with the project's existing C sources.
// - It relies on the project's optimize.h to provide the correct type definitions
//   (opt_state_t, struct block, struct slist, struct stmt, etc.) and the
//   opt_peep function signature.
// - The tests are implemented without Google Test; a simple main-based test harness is used.
// - Tests focus on exercising a couple of key branches inside opt_peep by constructing
//   in-memory block and statement structures consistent with the project's API.
// - The tests assume the project defines BPF_* opcode constants as used in the focal method.
// - Each test includes explanatory comments describing intended branch coverage and expectations.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <cassert>
#include <memory.h>
#include <optimize.h>
#include <cstdlib>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <cstdio>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <cstring>
#include <pcap-types.h>


extern "C" {
}

// Helper to create a new slist node with a single statement
static struct slist* make_stmt_node(int code, unsigned int k, struct slist* next) {
    struct slist* node = (struct slist*)malloc(sizeof(struct slist));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for slist node\n");
        exit(EXIT_FAILURE);
    }
    // The project defines slist as { struct stmt s; struct slist *next; }
    node->next = next;
    node->s.code = code;
    node->s.k = k;
    // Initialize any other fields if present by zeroing the memory around s
    // (Assuming the struct layout starts with s then next; the above sets fields used by opt_peep)
    return node;
}

// Utility to safely free a linked list of slist
static void free_slist(struct slist* head) {
    while (head) {
        struct slist* tmp = head;
        head = head->next;
        free(tmp);
    }
}

// Test 1: opt_peep should handle a block with no statements (s == 0) gracefully.
// This exercises the early return path.
static bool test_no_stmts()
{
    printf("Test 1: No statements in block (early return path)\n");

    // Prepare a block with stmts == NULL
    struct block b;
    memset(&b, 0, sizeof(b));
    b.stmts = NULL;
    // Ensure other fields are initialized to a safe default
    b.out_use = 0;

    // Prepare a minimal opt_state_t (zero-initialized)
    opt_state_t opt;
    memset(&opt, 0, sizeof(opt));

    // Call the focal method
    opt_peep(&opt, &b);

    // If we reach here, there was no crash. No state change is expected.
    printf("  Passed: opt_peep handled empty block without crashing.\n");
    return true;
}

// Test 2: ST followed by LDX MEM with same k should transform into TAX.
// This exercises the first transformation inside the loop.
static bool test_st_then_ldx_mem_transform()
{
    printf("Test 2: Transform ST -> LDX[mem] with matching k into TAX\n");

    // Create two slist nodes: s (ST) and next (LDX|MEM)
    struct slist* s = make_stmt_node(BPF_ST, 123, NULL);
    struct slist* next = make_stmt_node((BPF_LDX | BPF_MEM), 123, NULL);
    s->next = next;

    // Prepare a block with these two statements
    struct block b;
    memset(&b, 0, sizeof(b));
    b.stmts = s;
    b.out_use = 0; // so ATOMELEM evaluates false and branch is considered
    // The rest of block fields are not directly used for this path in the test

    // Initialize opt_state_t
    opt_state_t opt;
    memset(&opt, 0, sizeof(opt));
    opt.done = 1;
    opt.non_branch_movement_performed = 0;

    // Call the focal method
    opt_peep(&opt, &b);

    // Verify the transformation occurred:
    // next (LDX|MEM) should become (BPF_MISC|BPF_TAX)
    // and s (ST) remains ST
    if (next->s.code != (BPF_MISC | BPF_TAX)) {
        printf("  Failed: expected next->s.code to be (BPF_MISC|BPF_TAX), got %d\n", next->s.code);
        free_slist(s);
        return false;
    }
    if (s->s.code != BPF_ST) {
        printf("  Failed: expected s->s.code to remain BPF_ST, got %d\n", s->s.code);
        free_slist(s);
        return false;
    }

    // Optionally, ensure optimizer state indicates a change occurred
    if (opt.done == 1) {
        printf("  Failed: opt_state 'done' should be cleared (0) after transformation\n");
        free_slist(s);
        return false;
    }

    // Cleanup
    free_slist(s); // which will free s and next
    printf("  Passed: ST -> LDX MEM transformed as expected.\n");
    return true;
}

// Test 3: LDI IMM followed by TAX should transform into LDX IMM followed by TXA.
// This exercises the second transformation inside the loop.
static bool test_ldi_imm_then_tax_transform()
{
    printf("Test 3: Transform LD IMM -> TAX into LDX IMM and TXA\n");

    // Create two slist nodes: s (LD IMM) and next (MISC|TAX)
    struct slist* s = make_stmt_node((BPF_LD | BPF_IMM), 0x42, NULL);
    struct slist* next = make_stmt_node((BPF_MISC | BPF_TAX), 0, NULL);
    s->next = next;

    // Prepare block
    struct block b;
    memset(&b, 0, sizeof(b));
    b.stmts = s;
    b.out_use = 0;

    // Init opt_state
    opt_state_t opt;
    memset(&opt, 0, sizeof(opt));
    opt.done = 1;
    opt.non_branch_movement_performed = 0;

    // Call
    opt_peep(&opt, &b);

    // Assertions:
    // s should now be (BPF_LDX|BPF_IMM)
    // next should now be (BPF_MISC|BPF_TXA)
    if (s->s.code != (BPF_LDX | BPF_IMM)) {
        printf("  Failed: expected s->s.code to be (BPF_LDX|BPF_IMM), got %d\n", s->s.code);
        free_slist(s);
        return false;
    }
    if (next->s.code != (BPF_MISC | BPF_TXA)) {
        printf("  Failed: expected next->s.code to be (BPF_MISC|BPF_TXA), got %d\n", next->s.code);
        free_slist(s);
        return false;
    }

    if (opt.done == 1) {
        printf("  Failed: opt_state 'done' should be cleared (0) after transformation\n");
        free_slist(s);
        return false;
    }

    // Cleanup
    free_slist(s);
    printf("  Passed: LD IMM -> TAX transformed into LDX IMM and TXA as expected.\n");
    return true;
}

int main()
{
    // Basic environment checks
    printf("Starting opt_peep unit tests (C++11 harness, no GTest)\n");

    int total = 0;
    int passed = 0;

    // Test 1
    total++;
    if (test_no_stmts()) ++passed;
    printf("Test 1 %s\n", (test_no_stmts() ? "PASSED" : "FAILED")); // redundant call but ensures isolated check

    // We should not double-run tests to count; run onceEach and maintain boolean results
    // Re-run properly to avoid double counting
    // Re-structure to ensure stable counts:
    // We'll recompute results in a cleaner fashion.

    // Clean slate re-run for clean counting:
    // (In case earlier prints executed; we re-run with separate booleans to maintain clarity.)
    bool t1 = test_no_stmts();
    bool t2 = test_st_then_ldx_mem_transform();
    bool t3 = test_ldi_imm_then_tax_transform();

    total = 3;
    passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);

    printf("\nSummary: %d/%d tests passed.\n", passed, total);
    if (passed == total) {
        printf("All tests PASSED.\n");
        return EXIT_SUCCESS;
    } else {
        printf("Some tests FAILED. See details above.\n");
        return EXIT_FAILURE;
    }
}