// Unit test suite for the focal method init_val in optimize.c
// This test suite is written in C++11, compiled as a separate translation unit.
// It relies on the project's C headers (optimize.h) and links against the C implementation.
// No GTest is used; a lightweight, non-terminating assertion framework is implemented here.

#include <string.h>
#include <errno.h>
#include <diag-control.h>
#include <memory.h>
#include <optimize.h>
#include <cstdlib>
#include <cstdint>
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


// Bring in the C declarations. Use extern "C" to avoid name mangling when linking from C++.
extern "C" {
}

// Simple non-terminating test assertion helpers
static int g_test_failures = 0;

// Report a failed expectation but continue executing tests
#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_test_failures; \
        } \
    } while (0)

#define EXPECT_EQ(a, b) \
    do { \
        if (!((a) == (b))) { \
            fprintf(stderr, "EXPECT_EQ failed: %s:%d: %s == %s -> %lld != %lld\n", __FILE__, __LINE__, #a, #b, (long long)(a), (long long)(b)); \
            ++g_test_failures; \
        } \
    } while (0)

// Helper to access the underlying type names safely across different compilers
// We rely on the public layout of opt_state_t as defined in optimize.h for testing init_val.

//
// Test Case 1: Normal scenario with a positive maxval
// - Initialize an opt_state_t object with a non-trivial vmap and hashtbl.
// - Call init_val and verify:
//    * curval becomes 0
//    * next_vnode is equal to vnode_base (as set prior to call)
//    * all elements of vmap[0 .. maxval-1] are zeroed
//    * all elements of hashtbl are zeroed
//
static void test_init_val_normal() {
    // Allocate a state object and initialize fields to known values
    opt_state_t *state = new opt_state_t();
    // Pre-fill with non-zero values to ensure init_val overwrites them
    state->curval = 123;
    // Set vnode_base and next_vnode to a non-null sentinel so we can assert equality after init_val
    state->vnode_base = reinterpret_cast<decltype(state->vnode_base)>(0xDEADBEEF);
    state->next_vnode = reinterpret_cast<decltype(state->next_vnode)>(0xFEEDFACE);
    // Positive maxval to exercise vmap clearing
    state->maxval = 5;

    // Allocate vmap with the expected length
    // We assume vmap is of type comparable to int*. If actual type differs but is compatible
    // with pointer semantics, this should still compile for typical upstream code.
    state->vmap = new int[state->maxval];
    for (int i = 0; i < state->maxval; ++i) {
        state->vmap[i] = 7; // non-zero initial values
    }

    // Initialize hashtbl with non-zero values to verify clearing behavior
    // If hashtbl is a fixed-size array, sizeof(state->hashtbl)/sizeof(state->hashtbl[0]) yields its length.
    // If it's a pointer, this may not reflect the true length, but we still exercise the memset path
    // over the actual array length as defined by the struct in optimize.h.
    size_t hashtbl_len = 0;
    // Compute length defensively if possible
    hashtbl_len = sizeof(state->hashtbl) / sizeof(state->hashtbl[0]);
    if (hashtbl_len == 0) {
        // Fallback: do nothing if length can't be determined reliably
        hashtbl_len = 0;
    }
    for (size_t i = 0; i < hashtbl_len; ++i) {
        state->hashtbl[i] = 1234;
    }

    // Call the focal function
    init_val(state);

    // Assertions to verify the post-conditions
    EXPECT_EQ(state->curval, 0);
    EXPECT_EQ(state->next_vnode, state->vnode_base);

    for (int i = 0; i < state->maxval; ++i) {
        EXPECT_EQ(state->vmap[i], 0);
    }

    for (size_t i = 0; i < hashtbl_len; ++i) {
        EXPECT_EQ(state->hashtbl[i], 0);
    }

    // Cleanup
    delete[] state->vmap;
    delete state;
}

//
// Test Case 2: Edge scenario with maxval == 0
// - Ensure init_val handles zero-length vmap gracefully (no writes beyond range).
// - Validate that curval resets to 0 and next_vnode equals vnode_base as in normal case.
// - hashtbl should still be zeroed across its fixed length.
//
static void test_init_val_zero_maxval() {
    opt_state_t *state = new opt_state_t();
    state->curval = 999;
    state->vnode_base = reinterpret_cast<decltype(state->vnode_base)>(0x1234);
    state->next_vnode = reinterpret_cast<decltype(state->next_vnode)>(0x5678);
    state->maxval = 0; // edge case

    // Allocate vmap with zero length (C++ allows new int[0])
    state->vmap = new int[state->maxval];
    // No elements to initialize since maxval == 0

    // Initialize hashtbl values to non-zero to confirm they get cleared
    size_t hashtbl_len = sizeof(state->hashtbl) / sizeof(state->hashtbl[0]);
    for (size_t i = 0; i < hashtbl_len; ++i) {
        state->hashtbl[i] = 777;
    }

    init_val(state);

    // Assertions for edge case
    EXPECT_EQ(state->curval, 0);
    EXPECT_EQ(state->next_vnode, state->vnode_base);

    // Since maxval == 0, there are no vmap elements to check
    // Still verify hashtbl is cleared
    for (size_t i = 0; i < hashtbl_len; ++i) {
        EXPECT_EQ(state->hashtbl[i], 0);
    }

    // Cleanup
    delete[] state->vmap;
    delete state;
}

// Entry point for the test suite
int main() {
    printf("Starting unit tests for init_val in optimize.c\n");

    test_init_val_normal();
    test_init_val_zero_maxval();

    if (g_test_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("%d TEST(S) FAILED\n", g_test_failures);
        return 1;
    }
}