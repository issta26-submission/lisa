// This is a self-contained C++11 unit test suite for the focal method
// _cmsAllocIntentsPluginChunk extracted from cmscnvrt.c.
// The test suite provides minimal stubs for dependencies and exercises
// both branches of the method: when src != NULL and when src == NULL.
// No external testing framework (GTest) is used; a lightweight test harness
// accumulates failures and reports them at the end.

// The tests are designed to be non-terminating: failures are collected and the
// test suite continues to run subsequent tests.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstddef>


// Domain knowledge notes (informational for this test):
// - We simulate internal types and memory allocation behavior needed by
//   _cmsAllocIntentsPluginChunk.
// - We provide stubs for DupPluginIntentsList and _cmsSubAllocDup to observe
//   interactions without requiring the full upstream implementation.

namespace cms_test {

// Forward-declare the internal type used by the focal function
struct _cmsIntentsPluginChunkType;

// Minimal representation of the context structure used by the focal function
struct _cmsContext_struct {
    void* MemPool;
    void* chunks[8];
};

// The static type used inside the focal function for the NULL initialization
// of the plugin chunk type. We declare the struct here to match the signature
// used in the focal method.
struct _cmsIntentsPluginChunkType {
    void* dummy;
};

// A constant index used by the focal function to access the IntentPlugin chunk
enum { IntentPlugin = 0 };

// Globals to observe interactions in tests
static int DupPluginIntentsListCalled = 0;
static const _cmsContext_struct* DupSrcArg = nullptr;
static _cmsContext_struct* DupCtxArg = nullptr;

// Stub for DupPluginIntentsList(ctx, src) to verify it is invoked when src != NULL
void DupPluginIntentsList(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src) {
    ++DupPluginIntentsListCalled;
    DupCtxArg = ctx;
    DupSrcArg = src;
}

// Globals to observe allocation behavior
static int SubAllocDupCalls = 0;
static void* SubAllocDupReturnPointer = (void*)0x12345678;
static void* LastSubAllocDupTypeArg = nullptr;

// Stub for _cmsSubAllocDup(MemPool, Type, size)
void* _cmsSubAllocDup(void* MemPool, const void* Type, size_t size) {
    (void)MemPool; // MemPool is not used in the stub
    (void)size;    // size is not used in the stub
    LastSubAllocDupTypeArg = (void*)Type;
    ++SubAllocDupCalls;
    return SubAllocDupReturnPointer;
}

// The focal method under test (reimplemented here for isolated unit testing)
void _cmsAllocIntentsPluginChunk(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src) {
{
    if (src != NULL) {
        // Copy all linked list
        DupPluginIntentsList(ctx, src);
    }
    else {
        static _cmsIntentsPluginChunkType IntentsPluginChunkType = { NULL };
        ctx ->chunks[IntentPlugin] = _cmsSubAllocDup(ctx ->MemPool, &IntentsPluginChunkType, sizeof(_cmsIntentsPluginChunkType));
    }
}
}

// Simple assertion helpers that do not abort on failure
int g_failures = 0;

template <typename T, typename U>
void assert_ne(const T& a, const U& b, const char* msg) {
    if (!(a != b)) {
        std::cerr << "FAIL: " << msg << " (expected !=, got equal)\n";
        ++g_failures;
    }
}

template <typename T, typename U>
void assert_eq(const T& a, const U& b, const char* msg) {
    if (!(a == b)) {
        std::cerr << "FAIL: " << msg << " (expected " << (void*)static_cast<const void*>(&b)
                  << ", got " << (void*)static_cast<const void*>(&a) << ")\n";
        ++g_failures;
    }
}

// Helper to print pointer values in a readable form
void printPtr(const void* p) {
    std::cout << p;
}

// Test 1: src != NULL should call DupPluginIntentsList and not allocate new chunk
void test_SrcNotNull() {
    using namespace cms_test;

    // Reset state
    DupPluginIntentsListCalled = 0;
    DupCtxArg = nullptr;
    DupSrcArg = nullptr;
    SubAllocDupCalls = 0;
    LastSubAllocDupTypeArg = nullptr;

    // Prepare context and non-null source
    _cmsContext_struct ctx;
    ctx.MemPool = (void*)0xDEADBEEF;
    for (int i = 0; i < 8; ++i) ctx.chunks[i] = nullptr;
    void* initialChunk = (void*)0xCAFEBABE;
    ctx.chunks[IntentPlugin] = initialChunk;

    _cmsContext_struct src;
    src.MemPool = (void*)0xFEEDFACE;
    for (int i = 0; i < 8; ++i) src.chunks[i] = nullptr;

    // Execute
    _cmsAllocIntentsPluginChunk(&ctx, &src);

    // Verify: DupPluginIntentsList should have been called exactly once
    assert_eq(DupPluginIntentsListCalled, 1, "DupPluginIntentsList should be called once when src != NULL");

    // Verify: correct parameters passed
    assert_eq(DupCtxArg, &ctx, "DupCtxArg should be the same as ctx");
    assert_eq(DupSrcArg, &src, "DupSrcArg should be the same as src");

    // Verify: no allocation for the NULL-intents path should occur
    assert_eq(SubAllocDupCalls, 0, "SubAllocDup should not be called when src != NULL");

    // Verify: chunk pointer should remain unchanged
    if (ctx.chunks[IntentPlugin] != initialChunk) {
        std::cerr << "FAIL: ctx->chunks[IntentPlugin] should remain unchanged when src != NULL\n";
        ++g_failures;
    }
}

// Test 2: src == NULL should allocate a new plugin chunk via _cmsSubAllocDup
void test_SrcNull() {
    using namespace cms_test;

    // Reset state
    DupPluginIntentsListCalled = 0;
    DupCtxArg = nullptr;
    DupSrcArg = nullptr;
    SubAllocDupCalls = 0;
    LastSubAllocDupTypeArg = nullptr;

    // Prepare context
    _cmsContext_struct ctx;
    ctx.MemPool = (void*)0xABCDEF01;
    for (int i = 0; i < 8; ++i) ctx.chunks[i] = nullptr;

    // Source is NULL
    const _cmsContext_struct* src = nullptr;

    // Execute
    _cmsAllocIntentsPluginChunk(&ctx, src);

    // Verify: DupPluginIntentsList should not be called
    assert_eq(DupPluginIntentsListCalled, 0, "DupPluginIntentsList should not be called when src == NULL");

    // Verify: SubAllocDup should be called exactly once
    assert_eq(SubAllocDupCalls, 1, "_cmsSubAllocDup should be called once when src == NULL");

    // Verify: the chunk pointer should be set to the value returned by _cmsSubAllocDup
    if (ctx.chunks[IntentPlugin] != SubAllocDupReturnPointer) {
        std::cerr << "FAIL: ctx->chunks[IntentPlugin] should be set to SubAllocDupReturnPointer\n";
        ++g_failures;
    }

    // Verify: the Type argument passed to _cmsSubAllocDup should be non-null
    if (LastSubAllocDupTypeArg == nullptr) {
        std::cerr << "FAIL: _cmsSubAllocDup should receive a non-null Type argument\n";
        ++g_failures;
    }
}

// Test 3: src == NULL invoked twice should call _cmsSubAllocDup twice and update chunk each time
void test_SrcNullTwice() {
    using namespace cms_test;

    // Reset state
    DupPluginIntentsListCalled = 0;
    DupCtxArg = nullptr;
    DupSrcArg = nullptr;
    SubAllocDupCalls = 0;
    LastSubAllocDupTypeArg = nullptr;

    _cmsContext_struct ctx;
    ctx.MemPool = (void*)0x11111111;
    for (int i = 0; i < 8; ++i) ctx.chunks[i] = nullptr;

    // First call
    _cmsAllocIntentsPluginChunk(&ctx, nullptr);
    void* firstChunk = ctx.chunks[IntentPlugin];

    // Second call
    _cmsAllocIntentsPluginChunk(&ctx, nullptr);
    void* secondChunk = ctx.chunks[IntentPlugin];

    // Verify: two allocations should have occurred
    if (SubAllocDupCalls != 2) {
        std::cerr << "FAIL: Expected SubAllocDupCalls == 2 after two NULL calls, got " << SubAllocDupCalls << "\n";
        ++g_failures;
    }

    // Verify: both chunk pointers are set to the sentinel return value
    if (firstChunk != SubAllocDupReturnPointer || secondChunk != SubAllocDupReturnPointer) {
        std::cerr << "FAIL: Chunks should be set to SubAllocDupReturnPointer after each NULL call\n";
        ++g_failures;
    }
}

// Entry point for running all tests
void run_all_tests() {
    test_SrcNotNull();
    test_SrcNull();
    test_SrcNullTwice();
}

// Exposed C-style main entry to run the tests when compiled as a standalone program
} // namespace cms_test

int main() {
    cms_test::run_all_tests();

    if (cms_test::g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << cms_test::g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}