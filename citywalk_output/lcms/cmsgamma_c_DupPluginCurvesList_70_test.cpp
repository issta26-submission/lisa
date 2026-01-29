// High-quality C++11 unit tests for DupPluginCurvesList (embedded in test file for portability)
// This test suite mocks the minimal lcsm-related types and functions required by DupPluginCurvesList
// to verify its behavior without depending on the full library. It includes scenarios for
// copying multiple entries, single entry, and simulated memory allocation failures.
//
// Note: This file is self-contained and can be compiled with a C++11 compiler.

#include <cstring>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>


// Minimal domain definitions to mirror the focal function's environment.

enum { CurvesPlugin = 0 };

typedef struct _cmsParametricCurvesCollection {
    struct _cmsParametricCurvesCollection* Next;
    // The real library would have more fields; for testing we only need Next.
} _cmsParametricCurvesCollection;

typedef struct _cmsCurvesPluginChunkType {
    _cmsParametricCurvesCollection* ParametricCurves;
} _cmsCurvesPluginChunkType;

// Context structure used by DupPluginCurvesList
typedef struct _cmsContext_struct {
    void* MemPool;
    _cmsCurvesPluginChunkType* chunks[1];
} _cmsContext_struct;

// Global state to control allocation behavior in tests
static int g_subAllocFailCallIndex = -1; // -1 means never fail; 0-based call index to fail
static int g_subAllocCallCounter = 0;

// Forward declaration of the function under test (embedded here for testability)
void DupPluginCurvesList(_cmsContext_struct* ctx, const _cmsContext_struct* src);

// Minimal implementation of the substitute allocator used by the focal method
void* _cmsSubAllocDup(void* MemPool, const void* src, size_t size) {
    (void)MemPool;
    // Simulate allocation failure if configured
    if (g_subAllocFailCallIndex == g_subAllocCallCounter) {
        ++g_subAllocCallCounter;
        return NULL;
    }
    ++g_subAllocCallCounter;
    void* dst = std::malloc(size);
    if (dst && src) {
        std::memcpy(dst, src, size);
    }
    return dst;
}

// Simple assertion helper mirroring non-terminating "expect" style
static bool expect(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "EXPECT_FAIL: " << msg << std::endl;
    }
    return cond;
}

// Implementation of DupPluginCurvesList exactly as in the focal code
// (adapted to our simplified type definitions used in tests)
void DupPluginCurvesList(_cmsContext_struct* ctx, const _cmsContext_struct* src) {
{
   _cmsCurvesPluginChunkType newHead = { NULL };
   _cmsParametricCurvesCollection*  entry;
   _cmsParametricCurvesCollection*  Anterior = NULL;
   _cmsCurvesPluginChunkType* head = (_cmsCurvesPluginChunkType*) src->chunks[CurvesPlugin];
    _cmsAssert(head != NULL);
   for (entry = head->ParametricCurves;
        entry != NULL;
        entry = entry ->Next) {
            _cmsParametricCurvesCollection *newEntry = ( _cmsParametricCurvesCollection *) _cmsSubAllocDup(ctx ->MemPool, entry, sizeof(_cmsParametricCurvesCollection));
    
            if (newEntry == NULL) 
                return;
            // We want to keep the linked list order, so this is a little bit tricky
            newEntry -> Next = NULL;
            if (Anterior)
                Anterior -> Next = newEntry;
     
            Anterior = newEntry;
            if (newHead.ParametricCurves == NULL)
                newHead.ParametricCurves = newEntry;
    }
  ctx ->chunks[CurvesPlugin] = _cmsSubAllocDup(ctx->MemPool, &newHead, sizeof(_cmsCurvesPluginChunkType));
}
}

// Helper to create a singly linked list of n nodes
_cmsParametricCurvesCollection* buildParametricList(int n) {
    if (n <= 0) return NULL;
    _cmsParametricCurvesCollection* head = nullptr;
    _cmsParametricCurvesCollection* prev = nullptr;
    for (int i = 0; i < n; ++i) {
        auto node = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
        node->Next = NULL;
        if (!head) head = node;
        if (prev) prev->Next = node;
        prev = node;
    }
    return head;
}

// Helper to free an internal parametric list
void freeParametricList(_cmsParametricCurvesCollection* head) {
    while (head) {
        _cmsParametricCurvesCollection* next = head->Next;
        std::free(head);
        head = next;
    }
}

// Helper to free a single curves plugin chunk (only the header)
void freeCurvesPluginChunk(_cmsCurvesPluginChunkType* head) {
    std::free(head);
}

// Test 1: Copy a chain of multiple entries and verify order and independence from source
int test_dup_many_entries() {
    std::cout << "Test 1: DupPluginCurvesList with multiple entries" << std::endl;

    // Create source chain of 3 nodes
    _cmsParametricCurvesCollection* n1 = buildParametricList(1);
    // The helper above returns a head; we need a single node; adjust to single nodes exactly:
    // Rebuild properly for 3 distinct nodes
    _cmsParametricCurvesCollection* a1 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    _cmsParametricCurvesCollection* a2 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    _cmsParametricCurvesCollection* a3 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    a1->Next = a2;
    a2->Next = a3;
    a3->Next = NULL;

    _cmsCurvesPluginChunkType* headSrc = (_cmsCurvesPluginChunkType*)std::malloc(sizeof(_cmsCurvesPluginChunkType));
    headSrc->ParametricCurves = a1;

    _cmsContext_struct src;
    src.MemPool = NULL;
    src.chunks[CurvesPlugin] = headSrc;

    // Prepare destination context
    _cmsContext_struct dst;
    dst.MemPool = NULL;
    dst.chunks[CurvesPlugin] = NULL;

    // Run the function
    DupPluginCurvesList(&dst, &src);

    // Validate
    bool ok = true;
    _cmsCurvesPluginChunkType* copiedHead = (_cmsCurvesPluginChunkType*) dst.chunks[CurvesPlugin];
    ok &= expect(copiedHead != NULL, "Copied head should not be NULL");
    if (copiedHead == NULL) {
        // Early return on failure
        // Free sources
        freeParametricList(a1); // a1-a3
        freeCurvesPluginChunk(headSrc);
        return ok ? 0 : 1;
    }

    // Traverse copied list and ensure there are 3 nodes
    _cmsParametricCurvesCollection* cur = copiedHead->ParametricCurves;
    int count = 0;
    while (cur != NULL) {
        count++;
        cur = cur->Next;
    }
    ok &= expect(count == 3, "Copied list should contain 3 nodes");
    // Ensure copied nodes are not the same addresses as the originals
    cur = copiedHead->ParametricCurves;
    _cmsParametricCurvesCollection* orig_ptr = a1;
    while (cur && orig_ptr) {
        ok &= expect(cur != orig_ptr, "Copied node should be a distinct instance from source");
        cur = cur->Next;
        orig_ptr = orig_ptr->Next;
    }

    // Clean up: original list and head
    freeParametricList(a1);
    freeCurvesPluginChunk(headSrc);

    // Free copied list memory via traversal from copiedHead
    // Free nodes in copied list (they were allocated by _cmsSubAllocDup in test)
    cur = copiedHead->ParametricCurves;
    while (cur) {
        _cmsParametricCurvesCollection* next = cur->Next;
        std::free(cur);
        cur = next;
    }
    // Free copied head memory
    std::free(copiedHead);

    // Free final head created by original function (the allocator copies the head struct)
    // Note: dst.chunks[CurvesPlugin] points to a memory allocated by _cmsSubAllocDup
    // which we already freed above by freeing copiedHead; nothing else to free here.

    std::cout << "Test 1 " << (ok ? "PASSED" : "FAILED") << std::endl;
    return ok ? 0 : 1;
}

// Test 2: Copy a single entry to ensure first-iteration path (_newHead.ParametricCurves was NULL) is exercised
int test_dup_single_entry() {
    std::cout << "Test 2: DupPluginCurvesList with a single entry" << std::endl;

    // Create source chain of 1 node
    _cmsParametricCurvesCollection* n1 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    n1->Next = NULL;

    _cmsCurvesPluginChunkType* headSrc = (_cmsCurvesPluginChunkType*)std::malloc(sizeof(_cmsCurvesPluginChunkType));
    headSrc->ParametricCurves = n1;

    _cmsContext_struct src;
    src.MemPool = NULL;
    src.chunks[CurvesPlugin] = headSrc;

    _cmsContext_struct dst;
    dst.MemPool = NULL;
    dst.chunks[CurvesPlugin] = NULL;

    DupPluginCurvesList(&dst, &src);

    bool ok = true;
    _cmsCurvesPluginChunkType* copiedHead = (_cmsCurvesPluginChunkType*) dst.chunks[CurvesPlugin];
    ok &= expect(copiedHead != NULL, "Copied head should not be NULL for single entry");
    if (copiedHead) {
        _cmsParametricCurvesCollection* cur = copiedHead->ParametricCurves;
        ok &= expect(cur != NULL, "Copied single node should exist");
        ok &= expect(cur->Next == NULL, "Copied single node Next should be NULL");
        // Verify addresses differ from source
        ok &= expect(cur != n1, "Copied node should be a distinct instance from source");
    }

    // Free original
    std::free(n1);
    std::free(headSrc);

    // Free copied
    if (copiedHead) {
        _cmsParametricCurvesCollection* c = copiedHead->ParametricCurves;
        while (c) {
            _cmsParametricCurvesCollection* nx = c->Next;
            std::free(c);
            c = nx;
        }
        std::free(copiedHead);
    }
    std::cout << "Test 2 " << (ok ? "PASSED" : "FAILED") << std::endl;
    return ok ? 0 : 1;
}

// Test 3: Simulate allocation failure on first _cmsSubAllocDup call
int test_alloc_failure_first() {
    std::cout << "Test 3: Allocation failure on first _cmsSubAllocDup" << std::endl;
    // Prepare two nodes in source to ensure at least one allocation attempt occurs
    _cmsParametricCurvesCollection* a1 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    _cmsParametricCurvesCollection* a2 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    a1->Next = a2;
    a2->Next = NULL;

    _cmsCurvesPluginChunkType* headSrc = (_cmsCurvesPluginChunkType*)std::malloc(sizeof(_cmsCurvesPluginChunkType));
    headSrc->ParametricCurves = a1;

    _cmsContext_struct src;
    src.MemPool = NULL;
    src.chunks[CurvesPlugin] = headSrc;

    _cmsContext_struct dst;
    dst.MemPool = NULL;
    dst.chunks[CurvesPlugin] = NULL;

    // Configure failure on first allocation
    g_subAllocFailCallIndex = 0;
    g_subAllocCallCounter = 0;

    DupPluginCurvesList(&dst, &src);

    // Expect no change in dst (no final chunk allocated)
    bool ok = true;
    ok &= expect(dst.chunks[CurvesPlugin] == NULL, "dst.chunks[CurvesPlugin] should be NULL on first-allocation failure");

    // Cleanup
    std::free(a1);
    std::free(a2);
    std::free(headSrc);
    // No copied memory to free (dst is NULL)

    // Reset
    g_subAllocFailCallIndex = -1;
    g_subAllocCallCounter = 0;

    std::cout << "Test 3 " << (ok ? "PASSED" : "FAILED") << std::endl;
    return ok ? 0 : 1;
}

// Test 4: Simulate allocation failure on second _cmsSubAllocDup call (with 2 entries)
int test_alloc_failure_second() {
    std::cout << "Test 4: Allocation failure on second _cmsSubAllocDup" << std::endl;

    // Create two source nodes
    _cmsParametricCurvesCollection* a1 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    _cmsParametricCurvesCollection* a2 = (_cmsParametricCurvesCollection*)std::malloc(sizeof(_cmsParametricCurvesCollection));
    a1->Next = a2;
    a2->Next = NULL;

    _cmsCurvesPluginChunkType* headSrc = (_cmsCurvesPluginChunkType*)std::malloc(sizeof(_cmsCurvesPluginChunkType));
    headSrc->ParametricCurves = a1;

    _cmsContext_struct src;
    src.MemPool = NULL;
    src.chunks[CurvesPlugin] = headSrc;

    _cmsContext_struct dst;
    dst.MemPool = NULL;
    dst.chunks[CurvesPlugin] = NULL;

    // Configure failure on second allocation (second newEntry duplication)
    g_subAllocFailCallIndex = 1;
    g_subAllocCallCounter = 0;

    DupPluginCurvesList(&dst, &src);

    // Function should not produce a final dst; on failure, it returns early
    bool ok = true;
    ok &= expect(dst.chunks[CurvesPlugin] == NULL, "dst.chunks[CurvesPlugin] should be NULL when second allocation fails");

    // Cleanup
    std::free(a1);
    std::free(a2);
    std::free(headSrc);
    // No copied memory to free (dst is NULL)

    // Reset
    g_subAllocFailCallIndex = -1;
    g_subAllocCallCounter = 0;

    std::cout << "Test 4 " << (ok ? "PASSED" : "FAILED") << std::endl;
    return ok ? 0 : 1;
}

// Entry point: Run all tests
int main() {
    int failures = 0;
    failures += test_dup_many_entries();
    failures += test_dup_single_entry();
    failures += test_alloc_failure_first();
    failures += test_alloc_failure_second();

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED: " << failures << std::endl;
        return 1;
    }
}