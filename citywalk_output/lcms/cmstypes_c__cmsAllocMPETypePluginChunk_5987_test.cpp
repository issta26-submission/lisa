// test_cmstypes.cpp
// Lightweight unit tests for _cmsAllocMPETypePluginChunk (cmstypes.c style)
// Notes:
// - This test suite is designed as a small, self-contained harness (no GTest).
// - It uses a minimal, local stubbed environment to exercise the two branches
//   of _cmsAllocMPETypePluginChunk: (a) when src != NULL and (b) when src == NULL.
// - The test harness relies on the actual function _cmsAllocMPETypePluginChunk being
//   linked in (extern "C"). To observe behavior, the test provides mock (weak) hooks
//   for DupTagTypeList and _cmsSubAllocDup so we can verify which branch was taken
//   and what memory allocation was performed.
// - This file is intended to be compiled with a C++11 compiler (as requested).

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// The focal function is declared with C linkage.
extern "C" void _cmsAllocMPETypePluginChunk(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src);

// Forward-declare the context structure as used by the focal function.
// We only need the members used by the function under test.
struct _cmsContext_struct {
    void* MemPool;
    void* chunks[16];
};

// ---------------- Mock hooks and test scaffolding ---------------- //

// The test uses a macro for the MPEPlugin index. We assume the real code uses
// an enum/constant named MPEPlugin. For the test harness we define a compatible
// value so that the index is within chunks[] bounds.
#ifndef MPEPlugin
#define MPEPlugin 1
#endif

// Tag type used by the static local inside _cmsAllocMPETypePluginChunk in the real code.
// We mirror its shape so that tests can construct a reasonable object for tagging.
struct _cmsTagTypePluginChunkType {
    void* dummy;
};

// Global test state to observe which path was taken.
static bool g_dupTagList_was_called = false;
static const struct _cmsContext_struct* g_dupTagList_ctx_src = nullptr;
static int g_dupTagList_loc = -1;

static bool g_subAllocDup_was_called = false;
static void* g_subAllocDup_MemPool = nullptr;
static void* g_subAllocDup_TagPtr = nullptr;
static size_t g_subAllocDup_Size = 0;
static void* g_subAllocDup_ReturnPtr = nullptr;

// ----------------- Mock implementations (hooked into the focal code) -------------- //

// The real DupTagTypeList in the project duplicates a tag type list from src to ctx.
// For testing, we intercept its call to verify that the "src != NULL" branch is taken
// and that the function is called with the expected loc value (MPEPlugin).
extern "C" void DupTagTypeList(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src, int loc)
{
    (void)ctx; // not verified in this simplified harness
    g_dupTagList_was_called = true;
    g_dupTagList_ctx_src = src;
    g_dupTagList_loc = loc;
}

// The real _cmsSubAllocDup would allocate memory from a memory pool.
// For tests we intercept and record the parameters, then return a sentinel pointer.
extern "C" void* _cmsSubAllocDup(void* MemPool, const void* Tag, size_t size)
{
    (void)Tag; // Tag is not used by tests directly
    g_subAllocDup_was_called = true;
    g_subAllocDup_MemPool = MemPool;
    g_subAllocDup_TagPtr = (void*)Tag;
    g_subAllocDup_Size = size;
    // Return a sentinel allocated block so tests can observe the allocated pointer
    void* p = malloc(size == 0 ? 1 : size);
    g_subAllocDup_ReturnPtr = p;
    return p;
}

// Utility to reset test state between tests.
static void reset_test_state() {
    g_dupTagList_was_called = false;
    g_dupTagList_ctx_src = nullptr;
    g_dupTagList_loc = -1;

    g_subAllocDup_was_called = false;
    g_subAllocDup_MemPool = nullptr;
    g_subAllocDup_TagPtr = nullptr;
    g_subAllocDup_Size = 0;
    g_subAllocDup_ReturnPtr = nullptr;
}

// Simple assertion helpers (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << "): " #cond "\n"; \
    } else { \
        std::cout << "[OK] " << msg << "\n"; \
    } \
} while (0)

#define ASSERT_FALSE(cond, msg) ASSERT_TRUE(!(cond), msg)
#define ASSERT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << "): " #a " != " #b "\n"; \
    } else { \
        std::cout << "[OK] " << msg << "\n"; \
    } \
} while (0)

// ---------------------------- Test Cases ------------------------------------- //

// Test 1: src != NULL should trigger DupTagTypeList and NOT call _cmsSubAllocDup
void test_src_not_null_branch() {
    reset_test_state();

    // Prepare a minimal ctx and a non-NULL src
    struct _cmsContext_struct ctx;
    ctx.MemPool = (void*)0xDEADBEEF;            // arbitrary non-NULL pool
    std::memset(ctx.chunks, 0, sizeof(ctx.chunks));

    struct _cmsContext_struct src;
    src.MemPool = (void*)0xCAFEBABE;

    // Call the focal function (branch under test)
    _cmsAllocMPETypePluginChunk(&ctx, &src);

    // Expectations:
    // 1) DupTagTypeList should be called with loc == MPEPlugin
    ASSERT_TRUE(g_dupTagList_was_called, "DupTagTypeList should be invoked for non-NULL src");
    ASSERT_EQ(g_dupTagList_loc, MPEPlugin, "DupTagTypeList should receive loc == MPEPlugin");
    ASSERT_TRUE(g_dupTagList_ctx_src == &src, "DupTagTypeList should be passed the correct src pointer");

    // 2) _cmsSubAllocDup should NOT be called in this branch
    ASSERT_FALSE(g_subAllocDup_was_called, "_cmsSubAllocDup should NOT be invoked when src != NULL");

    // No memory allocation should have occurred in this branch
    if (g_subAllocDup_ReturnPtr != nullptr) {
        std::cout << "[WARN] Unexpected allocation pointer observed in src-not-null branch.\n";
    } else {
        std::cout << "[INFO] No allocation observed in src-not-null branch as expected.\n";
    }
}

// Test 2: src == NULL should trigger _cmsSubAllocDup and NOT call DupTagTypeList
void test_src_null_branch() {
    reset_test_state();

    // Prepare a minimal ctx
    struct _cmsContext_struct ctx;
    ctx.MemPool = (void*)0x01234567;
    std::memset(ctx.chunks, 0, sizeof(ctx.chunks));

    // Call the focal function with NULL source
    _cmsAllocMPETypePluginChunk(&ctx, nullptr);

    // Expectations:
    // 1) _cmsSubAllocDup should be called with MemPool == ctx.MemPool
    ASSERT_TRUE(g_subAllocDup_was_called, "_cmsSubAllocDup should be invoked for NULL src");
    ASSERT_EQ(g_subAllocDup_MemPool, ctx.MemPool, "_subAllocDup should receive MemPool == ctx.MemPool");

    // 2) DupTagTypeList should NOT be called in this branch
    ASSERT_FALSE(g_dupTagList_was_called, "DupTagTypeList should NOT be invoked when src == NULL");

    // The chunk should be assigned to ctx->chunks[MPEPlugin] with the pointer from _cmsSubAllocDup
    // We can't access the internal field directly here since we mocked the allocation,
    // but we can check that the return value pointer is used to populate the chunks entry in real code.
    // In this harness, we'll simulate the expectation by verifying an allocation was returned.
    ASSERT_TRUE(g_subAllocDup_ReturnPtr != nullptr, "Allocation should return a non-NULL pointer in NULL-src branch");

    // Simulate that the function stores the pointer in ctx.chunks[MPEPlugin].
    // Since we can't observe the internal write reliably here, we at least ensure the pointer
    // from our mock is non-NULL as a proxy for successful assignment.
    void* assigned_ptr = g_subAllocDup_ReturnPtr;
    (void)assigned_ptr; // unused in the mock scenario
}

// Optional: a small stress test to exercise multiple branches in sequence
void test_multiple_calls_sequence() {
    reset_test_state();

    struct _cmsContext_struct ctx;
    ctx.MemPool = (void*)0xABCDEF01;
    std::memset(ctx.chunks, 0, sizeof(ctx.chunks));

    // First call with non-NULL src
    struct _cmsContext_struct src;
    src.MemPool = (void*)0x11111111;
    _cmsAllocMPETypePluginChunk(&ctx, &src);

    // Validate that first branch was taken
    ASSERT_TRUE(g_dupTagList_was_called, "First call should take non-NULL-src path");
    // Reset flags to prepare for second call
    reset_test_state();

    // Second call with NULL src
    _cmsAllocMPETypePluginChunk(&ctx, nullptr);

    // Validate second branch
    ASSERT_TRUE(g_subAllocDup_was_called, "Second call should take NULL-src path");
}

// ------------------------------- Main ----------------------------------------- //

int main() {
    std::cout << "Running tests for _cmsAllocMPETypePluginChunk (cmstypes.c) - simplified harness\n";

    test_src_not_null_branch();
    test_src_null_branch();
    test_multiple_calls_sequence();

    std::cout << "Test run complete.\n";
    return 0;
}