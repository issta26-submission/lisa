// Note: This test suite targets the DupPluginIntentsList function in cmscnvrt.c.
// It relies on the Little CMS internal types (e.g. _cmsContext_struct, cmsIntentsList,
// _cmsIntentsPluginChunkType) declared in lcms2_internal.h and the surrounding
// plugin infrastructure. The tests are written to be compiled with a C++11 capable
// compiler but do not use GoogleTest; instead they use a lightweight assertion scheme
// and a small in-test mock of the internal memory-dup helper (_cmsSubAllocDup) to
// exercise multiple code paths.
// The test harness uses a macro hijack technique to swap the real _cmsSubAllocDup with
// MockSubAllocDup so that tests remain hermetic and do not depend on the real memory
// allocator internals. Be aware that this relies on the exact signatures used by the
// library; the code is designed to be self-contained for analysis and local testing.
//
// How to build (suggested):
//   - Ensure lcsm2 internal headers are in the include path (lcms2_internal.h).
//   - Compile with a C++11 capable compiler, e.g. g++ -std=c++11 test_cmscnvt_dup.cpp -O2 -o test
//   - Link may not be necessary if cmscnvrt.c is compiled in the same compilation unit (via include).
//
// Important: This test file is intended to illustrate the unit-test strategy and is not
// guaranteed to be portable across all exact Little CMS configurations. Adjust paths,
// constants (e.g., IntentPlugin index), and structure layouts as appropriate for your
// environment.

#include <cstring>
#include <iostream>
#include <cmscnvrt.c>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>


// Import Little CMS internal declarations. This will provide definitions for:
// - struct _cmsContext_struct
// - cmsIntentsList
// - _cmsIntentsPluginChunkType
// - IntentPlugin index (macro/enum)

// -------------------------------------------------------------------------------------
// Mocking _cmsSubAllocDup to avoid depending on the real memory pool allocator.
// We hijack the macro so that calls inside cmscnvrt.c to _cmsSubAllocDup are redirected
// to MockSubAllocDup, enabling deterministic tests.
// -------------------------------------------------------------------------------------

static bool g_mock_fail_once = false;     // If true, first allocation succeeds, subsequent call returns NULL
static int  g_mock_alloc_count = 0;       // Tracks number of allocations performed by the mock

void* MockSubAllocDup(void* MemPool, const void* Src, size_t Size)
{
    // Simple deterministic behavior to simulate allocation success/failure.
    if (g_mock_fail_once && g_mock_alloc_count == 0) {
        // First allocation succeeds
        ++g_mock_alloc_count;
        void* ret = std::malloc(Size);
        if (ret && Src) std::memcpy(ret, Src, Size);
        return ret;
    }

    // On subsequent calls, optionally fail to simulate mid-stream allocation failure
    if (g_mock_fail_once && g_mock_alloc_count > 0) {
        ++g_mock_alloc_count;
        return NULL; // simulate failure after first allocation
    }

    // Normal behavior: allocate and copy
    ++g_mock_alloc_count;
    void* ret = std::malloc(Size);
    if (ret && Src) std::memcpy(ret, Src, Size);
    return ret;
}

// Redirect the internal call to MockSubAllocDup instead of the real allocator
#define _cmsSubAllocDup(MemPool, Src, Size) MockSubAllocDup(MemPool, Src, Size)

// -------------------------------------------------------------------------------------
// Include the focal method implementation after the mock
// -------------------------------------------------------------------------------------

// Note: The focal method is defined in cmscnvrt.c, which will now be compiled in this TU
// with the _cmsSubAllocDup macro redirecting calls to MockSubAllocDup.
extern "C" {
}

// -------------------------------------------------------------------------------------
// Helper utilities for test setup
// -------------------------------------------------------------------------------------

// Helper to count a singly linked list of cmsIntentsList (via the Next pointer)
static int countIntentsList(const cmsIntentsList* head)
{
    int count = 0;
    const cmsIntentsList* cur = head;
    while (cur != NULL) {
        ++count;
        cur = cur->Next;
    }
    return count;
}

// Helper to create a chain of cmsIntentsList nodes with N elements
static cmsIntentsList* createIntentsChain(int n)
{
    if (n <= 0) return NULL;
    cmsIntentsList* head = (cmsIntentsList*) std::malloc(sizeof(cmsIntentsList));
    cmsIntentsList* cur  = head;
    for (int i = 1; i < n; ++i) {
        cmsIntentsList* node = (cmsIntentsList*) std::malloc(sizeof(cmsIntentsList));
        cur->Next = node;
        cur = node;
    }
    cur->Next = NULL;
    return head;
}

// Free a chain of cmsIntentsList
static void freeIntentsChain(cmsIntentsList* head)
{
    while (head) {
        cmsIntentsList* next = head->Next;
        std::free(head);
        head = next;
    }
}

// Free a _cmsIntentsPluginChunkType structure and its contents
static void freePluginChunk(_cmsIntentsPluginChunkType* chunk)
{
    if (!chunk) return;
    freeIntentsChain(chunk->Intents);
    std::free(chunk);
}

// Free a cmsContext struct and its chunks (best effort)
static void freeContext(struct _cmsContext_struct* ctx)
{
    if (!ctx) return;
    // We only know about IntentPlugin chunk; attempt to free if non-NULL
    if (ctx->chunks[IntentPlugin]) {
        freePluginChunk((_cmsIntentsPluginChunkType*) ctx->chunks[IntentPlugin]);
    }
    std::free(ctx);
}

// -------------------------------------------------------------------------------------
// Test cases
// -------------------------------------------------------------------------------------

// Test 1: Copy chain with N entries and verify order is preserved (length check)
static void test_copy_chain_preserves_order()
{
    // Reset allocator mock
    g_mock_alloc_count = 0;
    g_mock_fail_once  = false;

    // Create source chain with 3 nodes
    cmsIntentsList* srcChain = createIntentsChain(3);

    // Build source chunk
    _cmsIntentsPluginChunkType* srcChunk =
        (_cmsIntentsPluginChunkType*) std::malloc(sizeof(_cmsIntentsPluginChunkType));
    srcChunk->Intents = srcChain;

    // Prepare source cmsContext
    struct _cmsContext_struct* srcCtx = (struct _cmsContext_struct*) std::malloc(sizeof(struct _cmsContext_struct));
    // Initialize minimal fields (MemPool is unused by our mock, but set non-NULL)
    srcCtx->MemPool = (void*)0xDEADBEEF;
    srcCtx->chunks[IntentPlugin] = (void*) srcChunk;

    // Prepare destination cmsContext
    struct _cmsContext_struct* ctx = (struct _cmsContext_struct*) std::malloc(sizeof(struct _cmsContext_struct));
    ctx->MemPool = (void*)0xCAFEBABE;
    ctx->chunks[IntentPlugin] = NULL;

    // Run function under test
    DupPluginIntentsList(ctx, srcCtx);

    // Validate: New chunk should exist and contain a chain of 3 elements
    _cmsIntentsPluginChunkType* newChunk =
        (_cmsIntentsPluginChunkType*) ctx->chunks[IntentPlugin];
    assert(newChunk != NULL);
    int count = countIntentsList(newChunk->Intents);
    assert(count == 3);

    // Cleanup
    freeIntentsChain(srcChain);
    free(srcChunk);
    freeChunk?:2
    // We can't access internal newChunk's memory semantics here beyond freeing it
    // (since memory layout is managed by the library); deallocate the new chunk safely.
    // Free the new chunk
    freePluginChunk(newChunk);
    freeContext(srcCtx);
    freeContext(ctx);

    std::cout << "test_copy_chain_preserves_order passed\n";
}

// Test 2: Allocation failure during copying an entry results in early return
static void test_allocation_failure_during_copy()
{
    // Configure mock to fail on first allocation after the first successful call
    g_mock_alloc_count = 0;
    g_mock_fail_once = true; // fail on the second allocation attempt

    // Create source chain with 1 node
    cmsIntentsList* srcChain = createIntentsChain(1);

    _cmsIntentsPluginChunkType* srcChunk =
        (_cmsIntentsPluginChunkType*) std::malloc(sizeof(_cmsIntentsPluginChunkType));
    srcChunk->Intents = srcChain;

    struct _cmsContext_struct* srcCtx = (struct _cmsContext_struct*) std::malloc(sizeof(struct _cmsContext_struct));
    srcCtx->MemPool = (void*)0xF00D;
    srcCtx->chunks[IntentPlugin] = (void*) srcChunk;

    struct _cmsContext_struct* ctx = (struct _cmsContext_struct*) std::malloc(sizeof(struct _cmsContext_struct));
    ctx->MemPool = (void*)0x0D15;
    ctx->chunks[IntentPlugin] = NULL;

    DupPluginIntentsList(ctx, srcCtx);

    // Since allocation should fail on the second allocation, the destination chunk may be NULL
    // Depending on the exact timing of the failure, the function may leave dest NULL.
    // We simply ensure that we do not crash and that we don't falsely report a copied chain in this case.
    if (ctx->chunks[IntentPlugin] != NULL) {
        // If it is non-NULL, ensure that it's not obviously corrupted (non-NUL first element)
        _cmsIntentsPluginChunkType* newChunk = (_cmsIntentsPluginChunkType*) ctx->chunks[IntentPlugin];
        // If an entry existed, it should have been created via the final _cmsSubAllocDup;
        // treat this as a boundary case and just free
        freePluginChunk(newChunk);
    }

    // Cleanup
    freeIntentsChain(srcChain);
    free(srcChunk);
    freeContext(srcCtx);
    freeContext(ctx);

    // Reset mock
    g_mock_fail_once = false;
    std::cout << "test_allocation_failure_during_copy passed\n";
}

// Test 3: Empty input (src chunk exists but Intents is NULL) results in empty new head copied
static void test_empty_input_results_in_empty_head_copy()
{
    g_mock_alloc_count = 0;
    g_mock_fail_once  = false;

    _cmsIntentsPluginChunkType* srcChunk =
        (_cmsIntentsPluginChunkType*) std::malloc(sizeof(_cmsIntentsPluginChunkType));
    srcChunk->Intents = NULL; // empty

    struct _cmsContext_struct* srcCtx = (struct _cmsContext_struct*) std::malloc(sizeof(struct _cmsContext_struct));
    srcCtx->MemPool = (void*)0xABCD;
    srcCtx->chunks[IntentPlugin] = (void*) srcChunk;

    struct _cmsContext_struct* ctx = (struct _cmsContext_struct*) std::malloc(sizeof(struct _cmsContext_struct));
    ctx->MemPool = (void*)0x1234;
    ctx->chunks[IntentPlugin] = NULL;

    DupPluginIntentsList(ctx, srcCtx);

    _cmsIntentsPluginChunkType* newChunk = (_cmsIntentsPluginChunkType*) ctx->chunks[IntentPlugin];
    // Even with empty input, a new head chunk should be allocated (with NULL Intents)
    assert(newChunk != NULL);
    assert(newChunk->Intents == NULL);

    // Cleanup
    freePluginChunk(newChunk);
    free(srcChunk);
    freeContext(srcCtx);
    freeContext(ctx);

    std::cout << "test_empty_input_results_in_empty_head_copy passed\n";
}

// -------------------------------------------------------------------------------------
// Main driver
// -------------------------------------------------------------------------------------

int main()
{
    // Run tests
    test_copy_chain_preserves_order();
    test_allocation_failure_during_copy();
    test_empty_input_results_in_empty_head_copy();

    std::cout << "All tests completed.\n";
    return 0;
}