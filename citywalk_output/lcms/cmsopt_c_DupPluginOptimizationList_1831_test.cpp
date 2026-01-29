// Test suite for DupPluginOptimizationList in cmsopt.c
// Notes:
// - This test suite is written for a C/C++11 environment.
// - It relies on the internal lcms2 structures/types being exposed via lcms2_internal.h.
// - We provide two unit tests: one for non-empty optimization chain duplication and one for empty chain duplication.
// - The tests use the actual DupPluginOptimizationList function (extern "C") and validate structural correctness
//   (i.e., the copied list has the same length, but nodes are newly allocated and not identical to the originals).

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Assume the project provides the internal header with necessary type definitions.
// The test uses the project's real types to minimize mismatch between test and production code.

// Ensure the DupPluginOptimizationList function is linked with C linkage
extern "C" void DupPluginOptimizationList(struct _cmsContext_struct* ctx,
                                        const struct _cmsContext_struct* src);

// Global test result counter
static int g_testFailures = 0;

// Simple non-terminating assertion helpers
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
        ++g_testFailures; \
    } \
} while (0)

#define ASSERT_NOT_NULL(ptr, msg) do { \
    if ((ptr) == nullptr) { \
        std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
        ++g_testFailures; \
    } \
} while (0)

#define ASSERT_EQ_INT(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " (" << (a) << " != " << (b) << ")" << std::endl; \
        ++g_testFailures; \
    } \
} while (0)

#define ASSERT_PTR_NE(a, b, msg) do { \
    if ((a) == (b)) { \
        std::cerr << "ASSERT FAILED: " << (msg) \
                  << " (pointers are equal: " << (void*)(a) << " == " << (void*)(b) << ")" << std::endl; \
        ++g_testFailures; \
    } \
} while (0)


// ---------------------------------------------------------------------------
// Test 1: Non-empty optimization chain should be duplicated with a new chain.
// Setup original chain with 3 nodes and verify:
// - The new head is non-NULL
// - The new chain length equals the original (3)
// - The new chain nodes are distinct from the original nodes (no pointer aliasing)
// ---------------------------------------------------------------------------

void testDupPluginOptimizationList_nonEmpty() {
    // Build original chain: a -> b -> c -> NULL
    _cmsOptimizationCollection a;
    _cmsOptimizationCollection b;
    _cmsOptimizationCollection c;

    a.Next = &b;
    b.Next = &c;
    c.Next = NULL;

    // Build head chunk pointing to the first element
    _cmsOptimizationPluginChunkType head;
    head.OptimizationCollection = &a;

    // Source context
    _cmsContext_struct src;
    src.MemPool = NULL; // Not used by our test allocator
    src.chunks[OptimizationPlugin] = &head;  // Access via the real enum/index

    // Destination context
    _cmsContext_struct ctx;
    ctx.MemPool = NULL;
    // The destination's chunk pointer will be filled by DupPluginOptimizationList
    // Initialize to NULL to easily detect successful write
    ctx.chunks[OptimizationPlugin] = NULL;

    // Execute the function under test
    DupPluginOptimizationList(&ctx, &src);

    // Validate results
    _cmsOptimizationPluginChunkType* newHead =
        ( _cmsOptimizationPluginChunkType*) ctx.chunks[OptimizationPlugin];
    ASSERT_NOT_NULL(newHead, "Copied head should not be NULL for non-empty input");

    _cmsOptimizationCollection* newChainHead = newHead ? newHead->OptimizationCollection : NULL;
    ASSERT_NOT_NULL(newChainHead, "Copied chain head should be non-NULL");

    // Count nodes in new chain and compare with original (3)
    int copiedCount = 0;
    _cmsOptimizationCollection* cur = newChainHead;
    _cmsOptimizationCollection* origNodes[3] = { &a, &b, &c };
    bool aliasFound = false;

    while (cur != NULL) {
        copiedCount++;

        // Verify that the new node is not the same memory as the corresponding original node
        if (copiedCount >= 1 && copiedCount <= 3) {
            if (cur == origNodes[copiedCount - 1]) {
                aliasFound = true;
            }
        }

        cur = cur->Next;
        // Only 3 nodes expected
        if (copiedCount > 3) break;
    }

    ASSERT_EQ_INT(copiedCount, 3, "Copied chain length should be 3");
    ASSERT_TRUE(!aliasFound, "Copied nodes should be newly allocated (not aliasing original nodes)");

    // Ensure the copied chain ends correctly (last node's Next should be NULL)
    // Re-traverse to last node
    cur = newChainHead;
    while (cur != NULL && cur->Next != NULL) {
        cur = cur->Next;
    }
    ASSERT_TRUE(cur != NULL && cur->Next == NULL, "Last node's Next should be NULL in copied chain");
}

// ---------------------------------------------------------------------------
// Test 2: Empty optimization chain should be preserved as NULL in the destination.
// Setup original head with NULL OptimizationCollection and verify resulting destination head has NULL OptimizationCollection.
// ---------------------------------------------------------------------------

void testDupPluginOptimizationList_empty() {
    // Build empty original chain
    _cmsOptimizationPluginChunkType head;
    head.OptimizationCollection = NULL;

    // Source context
    _cmsContext_struct src;
    src.MemPool = NULL;
    src.chunks[OptimizationPlugin] = &head;

    // Destination context
    _cmsContext_struct ctx;
    ctx.MemPool = NULL;
    ctx.chunks[OptimizationPlugin] = NULL;

    // Execute
    DupPluginOptimizationList(&ctx, &src);

    // Validate
    _cmsOptimizationPluginChunkType* newHead =
        ( _cmsOptimizationPluginChunkType*) ctx.chunks[OptimizationPlugin];
    ASSERT_NOT_NULL(newHead, "Copied head should be non-NULL even for empty input (header copied)");
    ASSERT_TRUE(newHead->OptimizationCollection == NULL, "Copied chain should be NULL when input chain is NULL");
}

// ---------------------------------------------------------------------------
// Main: run tests and report summary.
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Starting DupPluginOptimizationList unit tests..." << std::endl;

    testDupPluginOptimizationList_nonEmpty();
    testDupPluginOptimizationList_empty();

    if (g_testFailures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_testFailures << " test(s) FAILED." << std::endl;
        return 1;
    }
}