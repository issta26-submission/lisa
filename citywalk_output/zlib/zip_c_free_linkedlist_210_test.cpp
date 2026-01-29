// Unit tests for the focal method free_linkedlist located in zip.c
// This test suite is written in C++11 without any external testing framework.
// It re-implements only the minimal dependencies required by free_linkedlist
// to verify the core behavior: freeing the datablock(s) starting from
// ll->first_block and then setting ll->first_block and ll->last_block to NULL.
//
// Note: The real project may have additional complexities (static functions,
// exact memory layout, etc.). For the purpose of unit testing the core logic
// of free_linkedlist, we create a lightweight, self-contained reproduction
// of the necessary types and functions.

#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>
#include <cstdlib>


// --- Candidate Keywords (Step 1) ---
// - linkedlist_data
// - linkedlist_datablock_internal
// - first_block
// - last_block
// - free_datablock
// - free_linkedlist

// Minimal reproduction of the necessary structures (Step 2)
typedef struct linkedlist_datablock_internal_s
{
  struct linkedlist_datablock_internal_s* next_datablock;
  unsigned long avail_in_this_block;
  unsigned long filled_in_this_block;
  unsigned long unused;
  unsigned char data[4096]; // size chosen to resemble SIZEDATA_INDATABLOCK
} linkedlist_datablock_internal;

typedef struct linkedlist_data_s
{
    linkedlist_datablock_internal* first_block;
    linkedlist_datablock_internal* last_block;
} linkedlist_data;

// Global counter to verify how many datablock(s) were freed
static int g_free_calls = 0;

// Free datablock helper (simulates the real free_datablock behavior sufficiently for tests)
static void free_datablock(linkedlist_datablock_internal* ldi)
{
    // In the real code, this would traverse and free a chain of datablocks.
    // We implement a safe, iterative traversal to emulate this behavior
    // and increment a counter for verification.
    while (ldi != NULL)
    {
        linkedlist_datablock_internal* next = ldi->next_datablock;
        // Free the current block
        free(ldi);
        g_free_calls++;
        ldi = next;
    }
}

// The focal method under test (recreated here for the test TU)
// local void free_linkedlist(linkedlist_data* ll) { ... }
// In this test, we mirror the exact logic: free the first block(s) and
// then set both pointers to NULL.
static void free_linkedlist(linkedlist_data* ll)
{
    free_datablock(ll->first_block);
    ll->first_block = ll->last_block = NULL;
}

// Simple non-terminating assertion helper
static void expect_true(bool condition, const char* message)
{
    if (!condition)
    {
        std::cerr << "TEST FAILED: " << message << std::endl;
    }
}

// Test 1: Single datablock in the list
// - Allocate one datablock
// - Ensure free_datablock is called exactly once
// - Verify ll->first_block and ll->last_block are set to NULL afterwards
static bool test_free_linkedlist_single_block()
{
    // Setup
    linkedlist_data ll;
    ll.first_block = NULL;
    ll.last_block = NULL;

    linkedlist_datablock_internal* block =
        (linkedlist_datablock_internal*)malloc(sizeof(linkedlist_datablock_internal));
    block->next_datablock = NULL;
    block->avail_in_this_block = 0;
    block->filled_in_this_block = 0;
    block->unused = 0;

    ll.first_block = block;
    ll.last_block = block;

    g_free_calls = 0;

    // Action
    free_linkedlist(&ll);

    // Verification
    bool pointers_cleared = (ll.first_block == NULL) && (ll.last_block == NULL);
    bool freed_once = (g_free_calls == 1);

    expect_true(pointers_cleared, "After freeing single block, both pointers should be NULL.");
    expect_true(freed_once, "free_datablock should be called exactly once for a single block.");

    // Cleanup (if not already freed by free_linkedlist; in this test it should be freed)
    // No additional cleanup needed since ll.first_block is NULL now.

    return pointers_cleared && freed_once;
}

// Test 2: Two datablocks linked in a chain
// - Create a chain: block1 -> block2 -> NULL
// - Ensure free_datablock frees both blocks (counter == 2)
// - Verify ll->first_block and ll->last_block are NULL after call
static bool test_free_linkedlist_two_block_chain()
{
    // Setup
    linkedlist_data ll;
    ll.first_block = NULL;
    ll.last_block = NULL;

    linkedlist_datablock_internal* block1 =
        (linkedlist_datablock_internal*)malloc(sizeof(linkedlist_datablock_internal));
    linkedlist_datablock_internal* block2 =
        (linkedlist_datablock_internal*)malloc(sizeof(linkedlist_datablock_internal));

    block1->next_datablock = block2;
    block2->next_datablock = NULL;
    block1->avail_in_this_block = 0;
    block1->filled_in_this_block = 0;
    block1->unused = 0;
    block2->avail_in_this_block = 0;
    block2->filled_in_this_block = 0;
    block2->unused = 0;

    ll.first_block = block1;
    ll.last_block = block2;

    g_free_calls = 0;

    // Action
    free_linkedlist(&ll);

    // Verification
    bool pointers_cleared = (ll.first_block == NULL) && (ll.last_block == NULL);
    bool freed_twice = (g_free_calls == 2);

    expect_true(pointers_cleared, "After freeing two-block chain, both pointers should be NULL.");
    expect_true(freed_twice, "free_datablock should be called twice for two-block chain.");

    return pointers_cleared && freed_twice;
}

// Test 3: NULL first_block (no actual datablock to free)
// - Ensure function handles NULL gracefully and still NULLs both pointers
static bool test_free_linkedlist_null_first_block()
{
    linkedlist_data ll;
    ll.first_block = NULL;
    ll.last_block = NULL;

    g_free_calls = 0;

    // Action
    free_linkedlist(&ll);

    // Verification
    bool pointers_cleared = (ll.first_block == NULL) && (ll.last_block == NULL);
    bool no_free_calls = (g_free_calls == 0);

    expect_true(pointers_cleared, "When first_block is NULL, pointers should remain NULL.");
    expect_true(no_free_calls, "free_datablock should not be called when first_block is NULL.");

    return pointers_cleared && no_free_calls;
}

// Main test runner
int main()
{
    int total_tests = 3;
    int passed = 0;

    // Run tests
    if (test_free_linkedlist_single_block()) {
        std::cout << "Test 1 passed: single block freed and pointers cleared." << std::endl;
        ++passed;
    } else {
        std::cout << "Test 1 failed." << std::endl;
    }

    if (test_free_linkedlist_two_block_chain()) {
        std::cout << "Test 2 passed: two-block chain freed and pointers cleared." << std::endl;
        ++passed;
    } else {
        std::cout << "Test 2 failed." << std::endl;
    }

    if (test_free_linkedlist_null_first_block()) {
        std::cout << "Test 3 passed: NULL first_block handled gracefully." << std::endl;
        ++passed;
    } else {
        std::cout << "Test 3 failed." << std::endl;
    }

    // Summary
    std::cout << "Summary: " << passed << " / " << total_tests << " tests passed." << std::endl;

    return (passed == total_tests) ? 0 : 1;
}