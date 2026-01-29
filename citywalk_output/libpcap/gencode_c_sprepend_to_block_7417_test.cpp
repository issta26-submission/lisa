// Test suite for the focal function: sprepend_to_block in gencode.c
// This test suite is written in C++11 (no GoogleTest) and uses a lightweight
// custom test harness. It exercises the behavior of sprepend_to_block with
// different combinations of input pointers to ensure correct mutations and
// return values. The tests rely on the project headers to provide the exact
// struct definitions and constants (e.g., struct slist, struct block, IS_UNCERTAIN).

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


// Include the C headers defining the function under test.
// The extern "C" ensures correct linkage when compiled with a C++ compiler.
extern "C" {
}

// Simple test harness
static int total_tests = 0;
static int passed_tests = 0;

#define TEST_PASS() do { ++total_tests; ++passed_tests; printf("[PASS] %s:%d\n", __FILE__, __LINE__); } while (0)
#define TEST_FAIL(msg) do { ++total_tests; printf("[FAIL] %s:%d - %s\n", __FILE__, __LINE__, (msg)); } while (0)

// Helper to dump a quick summary at the end
static void report_summary() {
    printf("\nTest Summary: %d ran, %d passed\n", total_tests, passed_tests);
}

// Test 1: sprepend_to_block with s == NULL should leave b unchanged and return b.
// Observations:
// - The function should not mutate b->stmts or b->meaning.
// - The returned pointer should be the same as the input pointer.
static int test_spprepend_null_s_keeps_block_intact() {
    // Prepare a block with a non-null stmts to verify it remains unchanged when s is NULL
    struct block *b = (struct block *)malloc(sizeof(struct block));
    if (!b) return 0; // allocation failed, treat as test failure

    // Preserve an identifiable non-null stmts value
    struct slist *orig_stmts = (struct slist *)malloc(sizeof(struct slist));
    if (!orig_stmts) { free(b); return 0; }

    b->stmts = orig_stmts;
    b->meaning = 0; // some initial value

    // Call with s == NULL
    struct slist *s = NULL;
    struct block *ret = sprepend_to_block(s, b);

    // Check that return value is the same block pointer
    if (ret != b) {
        free(orig_stmts);
        free(b);
        TEST_FAIL("Return value should be the same input block when s is NULL");
        return 0;
    }

    // Verify no mutation occurred
    if (b->stmts != orig_stmts || b->meaning != 0) {
        free(orig_stmts);
        free(b);
        TEST_FAIL("Block should be unchanged when s is NULL");
        return 0;
    }

    // Cleanup
    free(orig_stmts);
    free(b);

    TEST_PASS();
    return 1;
}

// Test 2: sprepend_to_block with s != NULL and b->stmts == NULL should assign b->stmts = s
// and set meaning to IS_UNCERTAIN. The return value should be the same block pointer.
static int test_spprepend_s_nonnull_bstmts_null() {
    // Prepare input
    struct block *b = (struct block *)malloc(sizeof(struct block));
    if (!b) return 0;
    b->stmts = NULL;
    b->meaning = 0;

    struct slist *s = (struct slist *)malloc(sizeof(struct slist));
    if (!s) { free(b); return 0; }

    struct block *ret = sprepend_to_block(s, b);

    // Expectations
    int ok = 1;
    if (ret != b) {
        ok = 0;
        TEST_FAIL("Return value should be the same input block when s is non-null");
    }
    if (b->stmts != s) {
        ok = 0;
        TEST_FAIL("Block's stmts should be set to input s when b->stmts was NULL");
    }
    if (b->meaning != IS_UNCERTAIN) {
        ok = 0;
        TEST_FAIL("Block's meaning should be IS_UNCERTAIN after prepend_to_block");
    }

    // Cleanup
    // Note: We must avoid freeing 's' twice; 'b->stmts' points to 's'
    free(b);
    free(s);

    if (ok) TEST_PASS();
    return ok;
}

// Test 3: sprepend_to_block with s != NULL and b->stmts != NULL should assign b->stmts = s
// and set meaning to IS_UNCERTAIN. Verifies both branches execute when both are non-null.
static int test_spprepend_s_nonnull_bstmts_nonnull() {
    // Prepare input
    struct block *b = (struct block *)malloc(sizeof(struct block));
    if (!b) return 0;
    struct slist *initial = (struct slist *)malloc(sizeof(struct slist));
    if (!initial) { free(b); return 0; }
    b->stmts = initial;
    b->meaning = 0;

    struct slist *s = (struct slist *)malloc(sizeof(struct slist));
    if (!s) { free(initial); free(b); return 0; }

    struct block *ret = sprepend_to_block(s, b);

    // Expectations
    int ok = 1;
    if (ret != b) {
        ok = 0;
        TEST_FAIL("Return value should be the same input block when both s and b->stmts are non-null");
    }
    if (b->stmts != s) {
        ok = 0;
        TEST_FAIL("Block's stmts should be updated to input s when b->stmts is non-null");
    }
    if (b->meaning != IS_UNCERTAIN) {
        ok = 0;
        TEST_FAIL("Block's meaning should be IS_UNCERTAIN after prepend_to_block");
    }

    // Cleanup
    free(s);       // s is now the active stmts
    free(initial); // original stmts memory no longer referenced, safe to free
    free(b);

    if (ok) TEST_PASS();
    return ok;
}

int main() {
    // Run tests
    test_spprepend_null_s_keeps_block_intact();
    test_spprepend_s_nonnull_bstmts_null();
    test_spprepend_s_nonnull_bstmts_nonnull();

    // Optional: force all tests to print results even if earlier ones fail
    report_summary();
    return (passed_tests == total_tests) ? 0 : 1;
}