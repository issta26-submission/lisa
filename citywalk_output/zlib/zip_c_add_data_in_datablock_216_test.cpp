// C++11 unit tests for add_data_in_datablock in zip.c
// - No GoogleTest/GTest usage
// - Simple non-terminating assertions implemented via macros
// - Tests cover NULL ll, zero-length input, cross-block copy, and initialization path
// - Explanatory comments accompany each test

#include <cstdio>
#include <cstring>
#include <string.h>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>
#include <cstdlib>


// Include the C header for the function under test and related types.
// The header is expected to provide the exact C types and ZIP_* constants.
extern "C" {
}

// Simple non-terminating assertion framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(a, b) \
    do { \
        ++g_total_tests; \
        if ((a) != (b)) { \
            std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ \
                      << " EXPECT_EQ: " #a " != " #b << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ \
                      << " EXPECT_TRUE: condition is false" << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)

#define EXPECT_FALSE(cond) \
    do { \
        ++g_total_tests; \
        if (cond) { \
            std::cerr << "[FAILED] " __FILE__ ":" << __LINE__ \
                      << " EXPECT_FALSE: condition is true" << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)


// Helper to free a linked list datablock chain
static void free_datablocks(linkedlist_datablock_internal* start) {
    linkedlist_datablock_internal* cur = start;
    while (cur != NULL) {
        linkedlist_datablock_internal* nxt = cur->next_datablock;
        // Free the block; assume malloc'ed per typical implementation
        free(cur);
        cur = nxt;
    }
}


// Test 1: When ll is NULL, function should return ZIP_INTERNALERROR
static void test_add_data_in_datablock_null_ll() {
    // Provide a small buffer; it should be ignored due to NULL ll
    const unsigned char input[] = {1, 2, 3, 4, 5};
    int ret = add_data_in_datablock(nullptr, input, sizeof(input));
    EXPECT_EQ(ret, ZIP_INTERNALERROR);
    // No resources to free
}

// Test 2: Zero-length input should return ZIP_OK and not fail catastrophically.
// Behavior: even if ll has no blocks yet, function should handle gracefully.
static void test_add_data_in_datablock_zero_length() {
    // Prepare an empty linkedlist_data (no last/first blocks yet)
    linkedlist_data* ll = (linkedlist_data*)malloc(sizeof(linkedlist_data));
    if (ll == NULL) {
        std::cerr << "Memory allocation for ll failed in test." << std::endl;
        ++g_total_tests;
        ++g_failed_tests;
        return;
    }
    ll->first_block = NULL;
    ll->last_block = NULL;

    const unsigned char input[] = {10, 20, 30}; // length 3, but len=0 to exercise code path
    int ret = add_data_in_datablock(ll, input, 0);

    EXPECT_EQ(ret, ZIP_OK);

    // After zero-length, the function should have allocated at least one block
    EXPECT_TRUE(ll->first_block != NULL);
    EXPECT_TRUE(ll->last_block != NULL);

    // Cleanup: free allocated datablocks and ll
    free_datablocks(ll->first_block);
    free(ll);

    // Note: We only verify that the function returns ZIP_OK and does not crash.
}

// Test 3: Cross-block write when input length exceeds a single datablock's capacity.
// We manually seed an initial datablock with limited available space to force allocation of a second block.
static void test_add_data_in_datablock_cross_block_copy() {
    // Prepare input of 4 bytes
    const unsigned char input[] = {1, 2, 3, 4};
    const size_t len = sizeof(input);

    // Create an initial datablock with only 2 bytes space available
    linkedlist_datablock_internal* init_block = (linkedlist_datablock_internal*)malloc(sizeof(linkedlist_datablock_internal));
    init_block->next_datablock = NULL;
    init_block->avail_in_this_block = 2;       // force cross-block copy
    init_block->filled_in_this_block = 0;
    init_block->unused = 0;
    // Note: Not initializing init_block->data[0..] since it will be overwritten for copied bytes

    // Create ll pointing to this initial block
    linkedlist_data* ll = (linkedlist_data*)malloc(sizeof(linkedlist_data));
    ll->first_block = init_block;
    ll->last_block = init_block;

    int ret = add_data_in_datablock(ll, input, len);

    EXPECT_EQ(ret, ZIP_OK);

    // Validate: there should be two blocks now: init_block and its next_datablock
    linkedlist_datablock_internal* first = ll->first_block;
    linkedlist_datablock_internal* second = first->next_datablock;

    // There should be a second block allocated
    EXPECT_TRUE(second != NULL);

    // The total bytes copied should equal 4
    unsigned long total_filled =
        first->filled_in_this_block +
        (second ? second->filled_in_this_block : 0);
    EXPECT_EQ(total_filled, (unsigned long)len);

    // Verify contents copied across blocks
    // First block should have input[0..1]
    if (first != NULL) {
        EXPECT_EQ(first->filled_in_this_block, 2U);
        if (first->filled_in_this_block >= 2) {
            // Copy check
            unsigned char v0 = first->data[0];
            unsigned char v1 = first->data[1];
            EXPECT_EQ(v0, input[0]);
            EXPECT_EQ(v1, input[1]);
        }
    }

    // Second block should have input[2..3]
    if (second != NULL) {
        EXPECT_EQ(second->filled_in_this_block, 2U);
        if (second->filled_in_this_block >= 2) {
            unsigned char v2 = second->data[0];
            unsigned char v3 = second->data[1];
            EXPECT_EQ(v2, input[2]);
            EXPECT_EQ(v3, input[3]);
        }
    }

    // ll->last_block should be the second block
    EXPECT_TRUE(ll->last_block == second);

    // Cleanup
    free_datablocks(ll->first_block);
    free(ll);
}

// Test 4: Initialization path when ll->last_block is NULL (allocate initial block) and small data is written.
// Verifies that the initial block is created and data is stored correctly.
static void test_add_data_in_datablock_initialization_path() {
    // ll with no blocks
    linkedlist_data* ll = (linkedlist_data*)malloc(sizeof(linkedlist_data));
    ll->first_block = NULL;
    ll->last_block = NULL;

    const unsigned char input[] = {7, 8, 9};
    int ret = add_data_in_datablock(ll, input, sizeof(input));

    EXPECT_EQ(ret, ZIP_OK);
    EXPECT_TRUE(ll->first_block != NULL);
    EXPECT_TRUE(ll->last_block != NULL);

    // Check that 3 bytes were stored across the blocks (most likely in the first block)
    unsigned long total_filled = 0;
    linkedlist_datablock_internal* cur = ll->first_block;
    while (cur != NULL) {
        total_filled += cur->filled_in_this_block;
        cur = cur->next_datablock;
    }
    EXPECT_EQ(total_filled, sizeof(input));

    // Check content in first block
    cur = ll->first_block;
    if (cur != NULL) {
        for (size_t i = 0; i < cur->filled_in_this_block && i < 256; ++i) {
            if (i < sizeof(input)) {
                EXPECT_EQ(cur->data[i], input[i]);
            }
        }
    }

    // Cleanup
    free_datablocks(ll->first_block);
    free(ll);
}


// Main runner
int main() {
    // Run unit tests
    test_add_data_in_datablock_null_ll();
    test_add_data_in_datablock_zero_length();
    test_add_data_in_datablock_cross_block_copy();
    test_add_data_in_datablock_initialization_path();

    // Summary
    std::cout << "Total tests run: " << g_total_tests << std::endl;
    std::cout << "Tests failed: " << g_failed_tests << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "All tests PASSED." << std::endl;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
    }

    // Return non-zero if any test failed
    return g_failed_tests;
}