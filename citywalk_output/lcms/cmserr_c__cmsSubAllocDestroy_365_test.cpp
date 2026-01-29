// Test suite for _cmsSubAllocDestroy in cmserr.c
// Notes:
// - This test harness is designed to compile under C++11 without GTest.
// - We provide a minimal mock for _cmsFree to intercept and record frees.
// - The test focuses on the behavior of _cmsSubAllocDestroy with various chunk scenarios.
// - We declare the necessary types locally to avoid depending on the full cms headers.
// - All tests are self-contained and call the production function via C linkage.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain knowledge: ensure correct namespace and C linkage for production function.
// We declare the minimal signatures/types required to compile and link with the real library.

extern "C" {

// Forward declaration of the production function under test (from cmserr.c)
void _cmsSubAllocDestroy(struct _cmsSubAllocator* sub);

// Mockable _cmsFree function (to intercept frees performed by _cmsSubAllocDestroy)
typedef void* cmsContext; // Simplified context type for testing

// We'll provide our own implementation of _cmsFree below (in this TU) to intercept calls.
void _cmsFree(cmsContext ContextID, void *Ptr);
}

// ---------------------------
// Minimal type definitions
// ---------------------------

// Simplified representations of the internal structures used by _cmsSubAllocDestroy.
// We only implement the fields that are accessed by the focal function.

typedef void* cmsContext;            // Context handle (opaque in real code)
typedef unsigned int cmsUInt32Number;  // Size type used by real code

typedef struct _cmsSubAllocator_chunk {
    void* Block;
    struct _cmsSubAllocator_chunk* next;
} _cmsSubAllocator_chunk;

typedef struct _cmsSubAllocator {
    cmsContext ContextID;
    _cmsSubAllocator_chunk* h;
} _cmsSubAllocator;

// ---------------------------
// Test harness state
// ---------------------------

// Track all pointers freed by the mocked _cmsFree
static std::vector<void*> freedPointers;

// A simple wrapper to reset state between tests
static void reset_freed_pointers() {
    freedPointers.clear();
}

// Mock implementation of _cmsFree to intercept deallocation calls.
// We record the pointer being freed and then perform an actual free to avoid leaks.
void _cmsFree(cmsContext ContextID, void *Ptr) {
    // Record only non-null pointers for assertion purposes
    if (Ptr != NULL) {
        freedPointers.push_back(Ptr);
        free(Ptr);
    }
}

// ---------------------------
// Helper: a lightweight assertion
// ---------------------------

#define ASSERT_TRUE(cond, message)                                           \
    do {                                                                       \
        if (!(cond)) {                                                         \
            std::cerr << "ASSERT FAILED: " << (message)                       \
                      << "  [Function: " << __FUNCTION__ << "]"               \
                      << std::endl;                                           \
            return false;                                                      \
        }                                                                      \
    } while (0)

// ---------------------------
// Test cases for _cmsSubAllocDestroy
// ---------------------------

/*
1) No chunks in the sub allocator (sub->h == NULL)
   Expectation: Only the sub header is freed. One pointer should be freed: sub.
*/
static bool test_suballoc_destroy_no_chunks() {
    reset_freed_pointers();

    // Allocate sub header
    _cmsSubAllocator* sub = (_cmsSubAllocator*)malloc(sizeof(_cmsSubAllocator));
    ASSERT_TRUE(sub != NULL, "Failed to allocate _cmsSubAllocator");

    sub->ContextID = (cmsContext)0xABCDEF01; // arbitrary non-null context
    sub->h = NULL;                          // no chunks

    // Call the function under test
    _cmsSubAllocDestroy(sub);

    // Assertions
    // Exactly one free call expected: the sub header itself
    ASSERT_TRUE(freedPointers.size() == 1, "Expected exactly 1 frees (sub header) when no chunks");
    ASSERT_TRUE(freedPointers[0] == sub, "The freed pointer should be the sub header");

    // Cleanup: sub has already been freed by production code
    // Nothing else to free here.

    return true;
}

/*
2) One chunk with Block == NULL
   Expectation: Free the chunk, then the sub header. Block is not freed.
*/
static bool test_suballoc_destroy_single_chunk_nullblock() {
    reset_freed_pointers();

    // Create a single chunk
    _cmsSubAllocator_chunk* chunk = (_cmsSubAllocator_chunk*)malloc(sizeof(_cmsSubAllocator_chunk));
    ASSERT_TRUE(chunk != NULL, "Failed to allocate _cmsSubAllocator_chunk");
    chunk->Block = NULL;
    chunk->next = NULL;

    // Create sub and link the chunk
    _cmsSubAllocator* sub = (_cmsSubAllocator*)malloc(sizeof(_cmsSubAllocator));
    ASSERT_TRUE(sub != NULL, "Failed to allocate _cmsSubAllocator");
    sub->ContextID = (cmsContext)0x12345678;
    sub->h = chunk;

    // Call the function under test
    _cmsSubAllocDestroy(sub);

    // Assertions
    // Expected frees: chunk, sub
    ASSERT_TRUE(freedPointers.size() == 2, "Expected 2 frees (chunk + sub) when one chunk with NULL Block");
    ASSERT_TRUE(freedPointers[0] == chunk, "First freed pointer should be the chunk");
    ASSERT_TRUE(freedPointers[1] == sub, "Second freed pointer should be the sub header");

    // Cleanup: both chunk and sub should have been freed
    return true;
}

/*
3) One chunk with non-NULL Block
   Expectation: Free Block, then chunk, then sub.
*/
static bool test_suballoc_destroy_single_chunk_with_block() {
    reset_freed_pointers();

    // Allocate block
    void* block = malloc(64);
    ASSERT_TRUE(block != NULL, "Failed to allocate Block memory");

    // Create chunk and link to sub
    _cmsSubAllocator_chunk* chunk = (_cmsSubAllocator_chunk*)malloc(sizeof(_cmsSubAllocator_chunk));
    ASSERT_TRUE(chunk != NULL, "Failed to allocate _cmsSubAllocator_chunk (chunk)");
    chunk->Block = block;
    chunk->next = NULL;

    _cmsSubAllocator* sub = (_cmsSubAllocator*)malloc(sizeof(_cmsSubAllocator));
    ASSERT_TRUE(sub != NULL, "Failed to allocate _cmsSubAllocator (sub)");
    sub->ContextID = (cmsContext)0x87654321;
    sub->h = chunk;

    // Call the function under test
    _cmsSubAllocDestroy(sub);

    // Assertions
    // Expected frees: block, chunk, sub
    ASSERT_TRUE(freedPointers.size() == 3, "Expected 3 frees (Block, chunk, sub)");
    ASSERT_TRUE(freedPointers[0] == block, "First freed pointer should be the Block");
    ASSERT_TRUE(freedPointers[1] == chunk, "Second freed pointer should be the chunk");
    ASSERT_TRUE(freedPointers[2] == sub, "Third freed pointer should be the sub header");

    return true;
}

/*
4) Multiple chunks with a mix of non-NULL and NULL Blocks
   Expectation: Free each non-NULL Block, then its chunk, in order, finally free sub header.
*/
static bool test_suballoc_destroy_multiple_chunks_mixed() {
    reset_freed_pointers();

    // Create first chunk with non-NULL Block
    void* block1 = malloc(32);
    ASSERT_TRUE(block1 != NULL, "Failed to allocate Block1");

    _cmsSubAllocator_chunk* chunk1 = (_cmsSubAllocator_chunk*)malloc(sizeof(_cmsSubAllocator_chunk));
    ASSERT_TRUE(chunk1 != NULL, "Failed to allocate chunk1");
    chunk1->Block = block1;

    // Create second chunk with NULL Block
    _cmsSubAllocator_chunk* chunk2 = (_cmsSubAllocator_chunk*)malloc(sizeof(_cmsSubAllocator_chunk));
    ASSERT_TRUE(chunk2 != NULL, "Failed to allocate chunk2");
    chunk2->Block = NULL;

    chunk1->next = chunk2;
    chunk2->next = NULL;

    // Sub header linking
    _cmsSubAllocator* sub = (_cmsSubAllocator*)malloc(sizeof(_cmsSubAllocator));
    ASSERT_TRUE(sub != NULL, "Failed to allocate sub");
    sub->ContextID = (cmsContext)0xCAFEBABE;
    sub->h = chunk1;

    // Call the function under test
    _cmsSubAllocDestroy(sub);

    // Assertions
    // Expected frees: block1, chunk1, chunk2, sub
    ASSERT_TRUE(freedPointers.size() == 4, "Expected 4 frees (Block1, chunk1, chunk2, sub)");
    ASSERT_TRUE(freedPointers[0] == block1, "First freed should be Block1");
    ASSERT_TRUE(freedPointers[1] == chunk1, "Second freed should be chunk1");
    ASSERT_TRUE(freedPointers[2] == chunk2, "Third freed should be chunk2");
    ASSERT_TRUE(freedPointers[3] == sub, "Fourth freed should be sub header");

    // Cleanup: all allocations have been freed by production code
    return true;
}

// ---------------------------
// Test runner
// ---------------------------

int main() {
    int total = 0;
    int failed = 0;

    std::cout << "Running tests for _cmsSubAllocDestroy..." << std::endl;

    // Run tests
    if (test_suballoc_destroy_no_chunks()) {
        std::cout << "[PASS] test_suballoc_destroy_no_chunks" << std::endl;
    } else {
        std::cout << "[FAIL] test_suballoc_destroy_no_chunks" << std::endl;
        failed++;
    }
    total++;

    if (test_suballoc_destroy_single_chunk_nullblock()) {
        std::cout << "[PASS] test_suballoc_destroy_single_chunk_nullblock" << std::endl;
    } else {
        std::cout << "[FAIL] test_suballoc_destroy_single_chunk_nullblock" << std::endl;
        failed++;
    }
    total++;

    if (test_suballoc_destroy_single_chunk_with_block()) {
        std::cout << "[PASS] test_suballoc_destroy_single_chunk_with_block" << std::endl;
    } else {
        std::cout << "[FAIL] test_suballoc_destroy_single_chunk_with_block" << std::endl;
        failed++;
    }
    total++;

    if (test_suballoc_destroy_multiple_chunks_mixed()) {
        std::cout << "[PASS] test_suballoc_destroy_multiple_chunks_mixed" << std::endl;
    } else {
        std::cout << "[FAIL] test_suballoc_destroy_multiple_chunks_mixed" << std::endl;
        failed++;
    }
    total++;

    // Summary
    std::cout << "Tests run: " << total << ", Passed: " << (total - failed) << ", Failed: " << failed << std::endl;

    // Return non-zero if any test failed
    return failed != 0 ? 1 : 0;
}