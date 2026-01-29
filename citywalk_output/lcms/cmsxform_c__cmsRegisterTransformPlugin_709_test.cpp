// Minimal C++11 test harness for the focal method: _cmsRegisterTransformPlugin
// This test reproduces the very small portion of cmsxform.c necessary for unit testing,
// with lightweight stubs for dependencies.
// The tests cover true/false branches and edge conditions of the function.

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-like typedefs (simplified for unit testing)
typedef int cmsContext;
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
#define TRUE 1
#define FALSE 0

// Forward declarations for the data structures used by the focal method
struct cmsPluginBase {
    int ExpectedVersion;
};

struct cmsPluginTransform {
    cmsPluginBase base;
    struct { void* xform; } factories;
};

struct _cmsTransformCollection {
    int OldXform;
    void* Factory;
    _cmsTransformCollection* Next;
};

struct _cmsTransformPluginChunkType {
    _cmsTransformCollection* TransformCollection;
};

// Global per-context storage for the tests
static _cmsTransformPluginChunkType g_transformPluginChunks[64];

// Simple per-context malloc failure flag for testing malloc failure path
static int g_pluginMallocFail[64];

// Helper to get the client chunk (simulating _cmsContextGetClientChunk)
static _cmsTransformPluginChunkType* _cmsContextGetClientChunk(cmsContext ContextID, int /*TransformPlugin*/)
{
    // In tests, ContextID is used as an index into a small fixed array
    return &g_transformPluginChunks[ContextID];
}

// Helper to simulate the plugin malloc function with optional failure
static void* _cmsPluginMalloc(cmsContext ContextID, std::size_t size)
{
    if (ContextID < 0 || ContextID >= (int)(sizeof(g_pluginMallocFail)/sizeof(g_pluginMallocFail[0])))
        return malloc(size);
    if (g_pluginMallocFail[ContextID])
        return NULL;
    return malloc(size);
}

// Convenience to toggle malloc failure for a context
static void cmsSetPluginMallocFail(cmsContext ctx, bool fail)
{
    if (ctx >= 0 && ctx < (int)(sizeof(g_pluginMallocFail)/sizeof(g_pluginMallocFail[0]))) {
        g_pluginMallocFail[ctx] = fail ? 1 : 0;
    }
}

// The focal method under test (reproduced in test code for unit testing)
static cmsBool _cmsRegisterTransformPlugin(cmsContext ContextID, cmsPluginBase* Data)
{
{
    cmsPluginTransform* Plugin = (cmsPluginTransform*) Data;
    _cmsTransformCollection* fl;
    _cmsTransformPluginChunkType* ctx = ( _cmsTransformPluginChunkType*) _cmsContextGetClientChunk(ContextID, 0 /*TransformPlugin*/);
    if (Data == NULL) {
        // Free the chain. Memory is safely freed at exit
        ctx->TransformCollection = NULL;
        return TRUE;
    }
    // Factory callback is required
    if (Plugin->factories.xform == NULL) return FALSE;

    fl = (_cmsTransformCollection*) _cmsPluginMalloc(ContextID, sizeof(_cmsTransformCollection));
    if (fl == NULL) return FALSE;
    // Check for full xform plug-ins previous to 2.8, we would need an adapter in that case
    if (Plugin->base.ExpectedVersion < 2080) {
           fl->OldXform = TRUE;
    }
    else
           fl->OldXform = FALSE;
    // Copy the parameters
    fl->Factory = Plugin->factories.xform;
    // Keep linked list
    fl ->Next = ctx->TransformCollection;
    ctx->TransformCollection = fl;
    // All is ok
    return TRUE;
}
}

// Helper to reset context state between tests
static void resetContext(cmsContext ctx)
{
    g_transformPluginChunks[ctx].TransformCollection = NULL;
    g_pluginMallocFail[ctx] = 0;
}

// Helper to inspect and free a chain of _cmsTransformCollection
static void freeTransformChain(_cmsTransformCollection* head)
{
    while (head) {
        _cmsTransformCollection* next = head->Next;
        free(head);
        head = next;
    }
}

// Lightweight test assertion style: non-terminating but reports failures
static int g_testFailures = 0;
static void expect(bool condition, const char* msg)
{
    if (!condition) {
        std::cerr << "Test failure: " << msg << std::endl;
        ++g_testFailures;
    }
}

// Test 1: Data == NULL should clear the chain and return TRUE
static void test_DataNull_ClearsChain_ReturnsTrue()
{
    cmsContext ctx = 1;
    resetContext(ctx);

    // Preload a non-NULL chain to simulate "free the chain" scenario
    _cmsTransformCollection* dummy = (_cmsTransformCollection*)malloc(sizeof(_cmsTransformCollection));
    if (dummy != NULL) {
        dummy->OldXform = FALSE;
        dummy->Factory = (void*)0xDEADBEEF;
        dummy->Next = NULL;
        g_transformPluginChunks[ctx].TransformCollection = dummy;
    }

    cmsBool ok = _cmsRegisterTransformPlugin(ctx, NULL);

    expect(ok == TRUE, "Data==NULL should return TRUE");
    expect(g_transformPluginChunks[ctx].TransformCollection == NULL, "Data==NULL should set TransformCollection to NULL");

    // Cleanup any remaining memory
    if (g_transformPluginChunks[ctx].TransformCollection != NULL) {
        freeTransformChain(g_transformPluginChunks[ctx].TransformCollection);
        g_transformPluginChunks[ctx].TransformCollection = NULL;
    }
}

// Test 2: Data non-NULL but Plugin->factories.xform == NULL should return FALSE
static void test_DataNonNull_NoFactory_ReturnsFalse()
{
    cmsContext ctx = 2;
    resetContext(ctx);

    cmsPluginTransform plugin;
    plugin.base.ExpectedVersion = 2100;
    plugin.factories.xform = NULL; // Missing factory

    // Pre-set a non-NULL previous chain to ensure no modification happens (we'll ignore freeing here)
    g_transformPluginChunks[ctx].TransformCollection = NULL;

    cmsBool ok = _cmsRegisterTransformPlugin(ctx, (cmsPluginBase*) &plugin);

    expect(ok == FALSE, "Data non-NULL with NULL factory should return FALSE");
    expect(g_transformPluginChunks[ctx].TransformCollection == NULL, "TransformCollection should remain NULL when factory is NULL");
}

// Test 3: Data valid with OldXform path (ExpectedVersion < 2080)
static void test_DataValid_OldXformPath()
{
    cmsContext ctx = 3;
    resetContext(ctx);

    cmsPluginTransform plugin;
    plugin.base.ExpectedVersion = 1000; // < 2080
    plugin.factories.xform = (void*)0x1234;

    // Ensure malloc will succeed
    cmsSetPluginMallocFail(ctx, false);
    g_transformPluginChunks[ctx].TransformCollection = NULL; // start empty

    cmsBool ok = _cmsRegisterTransformPlugin(ctx, (cmsPluginBase*) &plugin);

    expect(ok == TRUE, "Data valid should return TRUE");
    // Verify chain has exactly one element with OldXform = TRUE and correct Factory
    _cmsTransformCollection* head = g_transformPluginChunks[ctx].TransformCollection;
    expect(head != NULL, "TransformCollection should not be NULL after insertion");
    if (head) {
        expect(head->OldXform == TRUE, "OldXform flag should be TRUE for versions < 2080");
        expect(head->Factory == plugin.factories.xform, "Factory pointer should be copied into the chain node");
        expect(head->Next == NULL, "New node should be the only element (Next == NULL)");
    }

    // Cleanup
    freeTransformChain(g_transformPluginChunks[ctx].TransformCollection);
    g_transformPluginChunks[ctx].TransformCollection = NULL;
}

// Test 4: Data valid with NewXform path (ExpectedVersion >= 2080)
static void test_DataValid_NewXformPath()
{
    cmsContext ctx = 4;
    resetContext(ctx);

    // Pre-populate an existing chain to ensure linking works (simulate prior node)
    _cmsTransformCollection* existing = (_cmsTransformCollection*)malloc(sizeof(_cmsTransformCollection));
    existing->OldXform = FALSE;
    existing->Factory = (void*)0xBEEF;
    existing->Next = NULL;
    g_transformPluginChunks[ctx].TransformCollection = existing;

    cmsPluginTransform plugin;
    plugin.base.ExpectedVersion = 2100; // >= 2080
    plugin.factories.xform = (void*)0xCAFE;

    cmsSetPluginMallocFail(ctx, false);
    // Call
    cmsBool ok = _cmsRegisterTransformPlugin(ctx, (cmsPluginBase*) &plugin);

    expect(ok == TRUE, "Data valid should return TRUE");
    _cmsTransformCollection* head = g_transformPluginChunks[ctx].TransformCollection;
    expect(head != NULL, "TransformCollection should not be NULL after insertion");
    if (head) {
        expect(head->OldXform == FALSE, "OldXform flag should be FALSE for versions >= 2080");
        expect(head->Factory == plugin.factories.xform, "Factory pointer should be copied into the new head");
        expect(head->Next == existing, "New head's Next should point to the previous head");
    }

    // Cleanup
    freeTransformChain(g_transformPluginChunks[ctx].TransformCollection);
    g_transformPluginChunks[ctx].TransformCollection = NULL;
}

// Test 5: Malloc failure path should return FALSE
static void test_DataValid_MallocFailure()
{
    cmsContext ctx = 5;
    resetContext(ctx);

    cmsPluginTransform plugin;
    plugin.base.ExpectedVersion = 1000;
    plugin.factories.xform = (void*)0xDEAD;

    // Force malloc to fail
    cmsSetPluginMallocFail(ctx, true);

    cmsBool ok = _cmsRegisterTransformPlugin(ctx, (cmsPluginBase*) &plugin);

    expect(ok == FALSE, "Malloc failure should return FALSE");
    expect(g_transformPluginChunks[ctx].TransformCollection == NULL, "TransformCollection should remain NULL on malloc failure");

    // Cleanup just in case
    freeTransformChain(g_transformPluginChunks[ctx].TransformCollection);
    g_transformPluginChunks[ctx].TransformCollection = NULL;
}

// Main entry point for tests
int main()
{
    std::cout << "Running focal method unit tests for _cmsRegisterTransformPlugin...\n";

    // Run tests
    test_DataNull_ClearsChain_ReturnsTrue();
    test_DataNonNull_NoFactory_ReturnsFalse();
    test_DataValid_OldXformPath();
    test_DataValid_NewXformPath();
    test_DataValid_MallocFailure();

    // Summary
    if (g_testFailures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_testFailures << " TEST(S) FAILED\n";
        return 1;
    }
}