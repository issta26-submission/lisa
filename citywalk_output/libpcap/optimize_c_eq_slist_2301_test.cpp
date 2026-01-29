// Lightweight C++11 test suite for eq_slist (from optimize.c)
// This test suite does not rely on Google Test. It provides small,
// non-terminating assertions and prints results to stdout.
// The tests exercise various paths of the eq_slist function, including
// handling of NOP nodes, end-of-list conditions, and element-wise equality.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdlib>
#include <gencode.h>
#include <os-proto.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <setjmp.h>
#include <config.h>
#include <pcap-int.h>
#include <iostream>
#include <pcap-types.h>


// Bring in the C declarations for eq_slist and related types.
extern "C" {
}

// Simple, non-terminating test assertion helpers
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << msg \
                  << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_FALSE(cond, msg) do { \
    if (cond) { \
        std::cerr << "EXPECT_FALSE FAILED: " << msg \
                  << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ_INT(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ_INT FAILED: " << msg \
                  << " | expected " << (b) << ", got " << (a) \
                  << " (line " << __LINE__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while (0)

// Helper: create a new slist node with given code, k, and next pointer
static struct slist* mk_node(int code, unsigned int k, struct slist* next) {
    // Allocate with malloc to stay compatible with C-style structs
    struct slist* n = (struct slist*)malloc(sizeof(struct slist));
    if (!n) {
        std::cerr << "malloc failed in mk_node" << std::endl;
        std::exit(1);
    }
    n->s.code = code;
    n->s.k = k;
    n->next = next;
    return n;
}

// Helper: free a linked list of slist nodes
static void free_list(struct slist* s) {
    while (s) {
        struct slist* t = s->next;
        free(s);
        s = t;
    }
}

// Entry point for tests
int main() {
    // Test 1: Both lists are NULL -> should return true
    {
        struct slist* x = NULL;
        struct slist* y = NULL;
        int res = eq_slist(x, y);
        EXPECT_TRUE(res != 0, "Test1: both NULL should yield true");
        // No allocations to free
    }

    // Test 2: x contains a single NOP node, y is NULL -> after skipping NOPs both become NULL -> true
    {
        struct slist* x = mk_node(NOP, 0, NULL);
        struct slist* y = NULL;
        int res = eq_slist(x, y);
        EXPECT_TRUE(res != 0, "Test2: single NOP in x should be skipped, result should be true");
        free_list(x);
        // y is NULL, nothing to free
    }

    // Test 3: Equal non-NOP sequences with exact same (code, k)
    {
        struct slist* x = mk_node(7, 3, NULL);
        struct slist* y = mk_node(7, 3, NULL);
        int res = eq_slist(x, y);
        EXPECT_TRUE(res != 0, "Test3: identical single-element lists should be true");
        free_list(x);
        free_list(y);
    }

    // Test 4: Interleaved NOPs between lists; after skipping NOPs, sequences match
    {
        struct slist* x = mk_node(NOP, 0, mk_node(9, 11, NULL));
        struct slist* y = mk_node(9, 11, NULL);
        int res = eq_slist(x, y);
        EXPECT_TRUE(res != 0, "Test4: NOPs should be ignored; sequences match -> true");
        free_list(x);
        free_list(y);
    }

    // Test 5: Mismatch in k after skipping NOPs -> false
    {
        struct slist* x = mk_node(NOP, 0, mk_node(9, 11, NULL));
        struct slist* y = mk_node(9, 12, NULL); // different k
        int res = eq_slist(x, y);
        EXPECT_FALSE(res != 0, "Test5: mismatch in k after skipping NOPs should be false");
        free_list(x);
        free_list(y);
    }

    // Test 6: Extra node in x beyond y -> false
    {
        struct slist* x = mk_node(5, 1, mk_node(6, 2, NULL));
        struct slist* y = mk_node(5, 1, NULL);
        int res = eq_slist(x, y);
        EXPECT_FALSE(res != 0, "Test6: x longer than y with matching prefix -> false");
        free_list(x);
        free_list(y);
    }

    // Test 7: Leading NOPs in both lists; after skipping, equality holds
    {
        struct slist* x = mk_node(NOP, 0, mk_node(NOP, 0, mk_node(4, 7, NULL)));
        struct slist* y = mk_node(NOP, 0, mk_node(4, 7, NULL)); // extra NOP in front, but still matches
        int res = eq_slist(x, y);
        EXPECT_TRUE(res != 0, "Test7: leading NOPs on both sides and a shared element -> true");
        free_list(x);
        free_list(y);
    }

    // Test 8: Multiple-element identical lists with no NOPs
    {
        struct slist* x = mk_node(1, 100, mk_node(2, 200, NULL));
        struct slist* y = mk_node(1, 100, mk_node(2, 200, NULL));
        int res = eq_slist(x, y);
        EXPECT_TRUE(res != 0, "Test8: multi-element identical lists -> true");
        free_list(x);
        free_list(y);
    }

    // Test 9: Leading NOPs and trailing NOPs around a common core -> true
    {
        struct slist* x = mk_node(NOP, 0, mk_node(NOP, 0, mk_node(8, 99, NULL)));
        struct slist* y = mk_node(NOP, 0, mk_node(8, 99, NULL));
        int res = eq_slist(x, y);
        EXPECT_TRUE(res != 0, "Test9: NOPs at both ends should be ignored -> true");
        free_list(x);
        free_list(y);
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with " << g_failures << " failure(s)." << std::endl;
        return 1;
    }
}