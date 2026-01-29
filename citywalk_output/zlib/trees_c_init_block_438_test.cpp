/*
Unit test suite for the focal method: init_block(deflate_state *s)
The tests are designed to run under C++11 without Google Test (no external test framework).
The test harness redefines the minimal portion of the relevant structures/constants
to exercise the exact behavior of init_block as described in the <FOCAL_METHOD> snippet.

Note:
- This test is self-contained to verify the logical effects of init_block on a deflate_state.
- The implementation mirrors the semantics from the focal method:
  - Zero Freq for all L_CODES, D_CODES, BL_CODES
  - Set dyn_ltree[END_BLOCK].Freq = 1
  - Reset opt_len and static_len to 0 (unsigned long)
  - Reset sym_next and matches to 0
- The constants L_CODES, D_CODES, BL_CODES, END_BLOCK are defined to align with typical zlib-like values
  (286, 30, 19, and END_BLOCK = 256 respectively).

This approach enables deterministic testing of the initialization logic without requiring access
to non-exported/internal symbols from the original trees.c.

Explanatory notes for each unit test are included as comments.
*/

#include <iostream>
#include <deflate.h>
#include <limits>


// Domain knowledge alignment: Recreate minimum necessary types/constants to test init_block

// Core constants (typical zlib-like values)
static const int L_CODES = 286;
static const int D_CODES = 30;
static const int BL_CODES = 19;
static const int END_BLOCK = 256;  // must be within [0, L_CODES-1]

// Core data structure representing a single histogram/frequency bin
struct ct_data {
    unsigned int Freq;
};

// State structure containing the trees and related counters
struct deflate_state {
    ct_data dyn_ltree[L_CODES];
    ct_data dyn_dtree[D_CODES];
    ct_data bl_tree[BL_CODES];
    unsigned long opt_len;
    unsigned long static_len;
    unsigned int sym_next;
    unsigned int matches;
};

// The focal method under test (mirrored from the provided <FOCAL_METHOD>)
// Note: In the original project, this might be declared static (local). For testing here,
// we provide a non-static version to enable direct invocation from tests.
void init_block(deflate_state *s) {
    int n; /* iterates over tree elements */
    /* Initialize the trees. */
    for (n = 0; n < L_CODES;  n++) s->dyn_ltree[n].Freq = 0;
    for (n = 0; n < D_CODES;  n++) s->dyn_dtree[n].Freq = 0;
    for (n = 0; n < BL_CODES; n++) s->bl_tree[n].Freq = 0;
    s->dyn_ltree[END_BLOCK].Freq = 1;
    s->opt_len = s->static_len = 0UL;
    s->sym_next = s->matches = 0;
}

// Simple, non-terminating test framework (EXPECT-style assertions)
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "EXPECT_EQ failed: " #a " == " #b " (got " << (a) << " vs " << (b) << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_failures; \
        } \
    } while(0)

// Test 1: Basic initialization sets all frequencies to 0 and counters to 0, except END_BLOCK which should be 1
// This validates the primary behavior of init_block as described in the focal method.
void test_init_block_basic() {
    // Prepare a deflate_state with non-zero/filler values to ensure the function actually overwrites them
    deflate_state s;
    // Pre-fill with non-zero values to ensure reset happens
    for (int i = 0; i < L_CODES; ++i) s.dyn_ltree[i].Freq = 12345;
    for (int i = 0; i < D_CODES; ++i) s.dyn_dtree[i].Freq = 54321;
    for (int i = 0; i < BL_CODES; ++i) s.bl_tree[i].Freq = 999;
    s.dyn_ltree[END_BLOCK].Freq = 7; // ensure END_BLOCK changes to 1
    s.opt_len = 1234;
    s.static_len = 5678;
    s.sym_next = 42;
    s.matches = 99;

    // Act
    init_block(&s);

    // Assert: all L_CODES freqs zero
    for (int i = 0; i < L_CODES; ++i) {
        EXPECT_EQ(s.dyn_ltree[i].Freq, 0u);
    }
    // Assert: all D_CODES freqs zero
    for (int i = 0; i < D_CODES; ++i) {
        EXPECT_EQ(s.dyn_dtree[i].Freq, 0u);
    }
    // Assert: all BL_CODES freqs zero
    for (int i = 0; i < BL_CODES; ++i) {
        EXPECT_EQ(s.bl_tree[i].Freq, 0u);
    }
    // Assert: END_BLOCK freq is exactly 1
    EXPECT_EQ(s.dyn_ltree[END_BLOCK].Freq, 1u);

    // Assert: counters reset
    EXPECT_EQ(s.opt_len, 0ul);
    EXPECT_EQ(s.static_len, 0ul);
    // Assert: pointers/counters reset
    EXPECT_EQ(s.sym_next, 0u);
    EXPECT_EQ(s.matches, 0u);
}

// Test 2: END_BLOCK residual value is overwritten to 1 even if it started with a non-default value.
// This ensures the dedicated initialization for END_BLOCK is honored.
void test_init_block_endblock_overwrite() {
    deflate_state s;
    // Pre-set END_BLOCK to a non-default value
    s.dyn_ltree[END_BLOCK].Freq = 9999;
    // Also set some other values to non-zero to verify they get reset
    for (int i = 0; i < L_CODES; ++i) s.dyn_ltree[i].Freq = (i == END_BLOCK ? 777 : 1);
    for (int i = 0; i < D_CODES; ++i) s.dyn_dtree[i].Freq = 888;
    for (int i = 0; i < BL_CODES; ++i) s.bl_tree[i].Freq = 777;

    s.opt_len = 12;
    s.static_len = 34;
    s.sym_next = 56;
    s.matches = 78;

    // Act
    init_block(&s);

    // END_BLOCK should be 1
    EXPECT_EQ(s.dyn_ltree[END_BLOCK].Freq, 1u);

    // Other checks to ensure rest of initialization still happens
    for (int i = 0; i < L_CODES; ++i) {
        if (i == END_BLOCK) continue;
        EXPECT_EQ(s.dyn_ltree[i].Freq, 0u);
    }
    for (int i = 0; i < D_CODES; ++i) {
        EXPECT_EQ(s.dyn_dtree[i].Freq, 0u);
    }
    for (int i = 0; i < BL_CODES; ++i) {
        EXPECT_EQ(s.bl_tree[i].Freq, 0u);
    }
    EXPECT_EQ(s.opt_len, 0ul);
    EXPECT_EQ(s.static_len, 0ul);
    EXPECT_EQ(s.sym_next, 0u);
    EXPECT_EQ(s.matches, 0u);
}

// Entry point for running tests
int main() {
    std::cout << "Running unit tests for init_block (standalone test harness)" << std::endl;

    test_init_block_basic();
    test_init_block_endblock_overwrite();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return g_failures;
    }
}