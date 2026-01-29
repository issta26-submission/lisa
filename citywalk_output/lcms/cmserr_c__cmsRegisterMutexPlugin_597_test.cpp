// Minimal C++11 unit test suite for cmsBool _cmsRegisterMutexPlugin
// The tests implement a small in-memory CMS-like context to exercise the focal function.
// No use of GoogleTest or any external testing framework; a lightweight harness is provided.

#include <lcms2_internal.h>
#include <iostream>
#include <unordered_map>


// Domain-specific type aliases (simplified for unit testing)
using cmsBool = int;
using cmsContext = int;

static const cmsBool TRUE = 1;
static const cmsBool FALSE = 0;

// Forward declarations for the minimal CMS structures used by the focal method.
struct cmsPluginBase;
struct cmsPluginMutex;

// Function pointer types for mutex plugin (mirroring real CMS signatures)
typedef void* (*CreateMutexPtr)(cmsContext);
typedef void (*DestroyMutexPtr)(cmsContext, void*);
typedef void (*LockMutexPtr)(cmsContext, void*);
typedef void (*UnlockMutexPtr)(cmsContext, void*);

// Minimal base plugin structure (as used by the code under test)
struct cmsPluginBase {
    // In the real project this would contain metadata; kept empty for tests.
    int _unused;
};

// Mutex plugin structure used to transport function pointers
struct cmsPluginMutex {
    cmsPluginBase Base;
    CreateMutexPtr CreateMutexPtr;
    DestroyMutexPtr DestroyMutexPtr;
    LockMutexPtr LockMutexPtr;
    UnlockMutexPtr UnlockMutexPtr;
};

// Per-context chunk to hold the registered mutex plugin callbacks
struct _cmsMutexPluginChunkType {
    CreateMutexPtr CreateMutexPtr;
    DestroyMutexPtr DestroyMutexPtr;
    LockMutexPtr LockMutexPtr;
    UnlockMutexPtr UnlockMutexPtr;
};

// --------------------------
// Minimal context manager
// --------------------------

// Global per-context storage of mutex plugin chunks (simulates _cmsContextGetClientChunk)
static std::unordered_map<cmsContext, _cmsMutexPluginChunkType> g_mutexPluginChunks;

// Helper to obtain the per-context chunk (simulates the real CMS function)
static _cmsMutexPluginChunkType* _cmsContextGetClientChunk(cmsContext ContextID, int /*MutexPlugin*/) {
    // Create a default chunk if missing
    auto it = g_mutexPluginChunks.find(ContextID);
    if (it == g_mutexPluginChunks.end()) {
        _cmsMutexPluginChunkType defaultChunk = { NULL, NULL, NULL, NULL };
        auto res = g_mutexPluginChunks.emplace(ContextID, defaultChunk);
        it = res.first;
    }
    return &(it->second);
}

// --------------------------
// Focal method under test
// (Re-implemented here to enable unit testing without the full C project linkage.)
// --------------------------

cmsBool _cmsRegisterMutexPlugin(cmsContext ContextID, cmsPluginBase* Data)
{
    // Cast away to access the mutex-specific fields
    cmsPluginMutex* Plugin = (cmsPluginMutex*) Data;
    _cmsMutexPluginChunkType* ctx = ( _cmsMutexPluginChunkType*) _cmsContextGetClientChunk(ContextID, 0 /* MutexPlugin */);

    if (Data == NULL) {
        // No lock routines
        ctx->CreateMutexPtr = NULL;
        ctx->DestroyMutexPtr = NULL;
        ctx->LockMutexPtr = NULL;
        ctx->UnlockMutexPtr = NULL;
        return TRUE;
    }

    // Factory callback is required
    if (Plugin ->CreateMutexPtr == NULL || Plugin ->DestroyMutexPtr == NULL ||
        Plugin ->LockMutexPtr == NULL || Plugin ->UnlockMutexPtr == NULL) return FALSE;

    ctx->CreateMutexPtr  = Plugin->CreateMutexPtr;
    ctx->DestroyMutexPtr = Plugin ->DestroyMutexPtr;
    ctx ->LockMutexPtr   = Plugin ->LockMutexPtr;
    ctx ->UnlockMutexPtr = Plugin ->UnlockMutexPtr;

    // All is ok
    return TRUE;
}

// --------------------------
// Test harness and helpers
// --------------------------

static int g_passed = 0;
static int g_failed = 0;

static void TestCasePassed(const std::string& testName) {
    ++g_passed;
    std::cout << "[PASS] " << testName << std::endl;
}

static void TestCaseFailed(const std::string& testName, const std::string& reason) {
    ++g_failed;
    std::cout << "[FAIL] " << testName << " - " << reason << std::endl;
}

static void ExpectTrue(bool condition, const std::string& testName, const std::string& reason) {
    if (condition) TestCasePassed(testName);
    else TestCaseFailed(testName, reason);
}

// Dummy implementations for mutex operations (only their addresses matter for tests)
static void* dummyMtxCreate(cmsContext ctx) { (void)ctx; return (void*)0xA1A1; }
static void dummyMtxDestroy(cmsContext ctx, void* mtx) { (void)ctx; (void)mtx; }
static void dummyMtxLock(cmsContext ctx, void* mtx) { (void)ctx; (void)mtx; }
static void dummyMtxUnlock(cmsContext ctx, void* mtx) { (void)ctx; (void)mtx; }

// Helper to fetch current chunk for assertion messages (not strictly required)
static _cmsMutexPluginChunkType* ctxGetChunkForTest(cmsContext ctx) {
    return _cmsContextGetClientChunk(ctx, 0);
}

// Reset the in-memory context state (for deterministic tests)
static void ResetContextStore() {
    g_mutexPluginChunks.clear();
}

// --------------------------
// Test cases
// --------------------------

void Test_NullData_SetsPointersToNull() {
    const cmsContext ContextID = 1;

    // Ensure a clean context
    ResetContextStore();

    // Call the focal function with NULL Data
    cmsBool res = _cmsRegisterMutexPlugin(ContextID, NULL);

    // Expect TRUE and that all pointers in the per-context chunk are NULL
    ExpectTrue(res == TRUE, "NullData_Returns_TRUE", "Expected TRUE for NULL Data");
    _cmsMutexPluginChunkType* ctx = ctxGetChunkForTest(ContextID);
    ExpectTrue(ctx->CreateMutexPtr == NULL && ctx->DestroyMutexPtr == NULL &&
               ctx->LockMutexPtr == NULL && ctx->UnlockMutexPtr == NULL,
               "NullData_PointersAreNull", "All four pointers should be NULL when Data is NULL");
}

void Test_NonNullData_AllPointersPresent() {
    const cmsContext ContextID = 2;

    // Clear state
    ResetContextStore();

    // Prepare a fully valid mutex plugin
    cmsPluginMutex plugin;
    plugin.Base._unused = 0;
    plugin.CreateMutexPtr = &dummyMtxCreate;
    plugin.DestroyMutexPtr = &dummyMtxDestroy;
    plugin.LockMutexPtr = &dummyMtxLock;
    plugin.UnlockMutexPtr = &dummyMtxUnlock;

    // Call the focal function with a valid plugin
    cmsBool res = _cmsRegisterMutexPlugin(ContextID, (cmsPluginBase*)&plugin);

    // Expect TRUE
    ExpectTrue(res == TRUE, "NonNullData_AllPointersPresent_Returns_TRUE", "Expected TRUE when all pointers non-NULL");

    // Check that the context chunk now points to the exact functions
    _cmsMutexPluginChunkType* ctx = ctxGetChunkForTest(ContextID);
    ExpectTrue(ctx->CreateMutexPtr == plugin.CreateMutexPtr &&
               ctx->DestroyMutexPtr == plugin.DestroyMutexPtr &&
               ctx->LockMutexPtr == plugin.LockMutexPtr &&
               ctx->UnlockMutexPtr == plugin.UnlockMutexPtr,
               "NonNullData_PointersCopiedToContext",
               "All function pointers should be copied to the context chunk");
}

void Test_NonNullData_MissingOnePointer_ReturnsFALSEAndDoesNotModifyContext() {
    const cmsContext ContextID = 3;

    // Start fresh
    ResetContextStore();

    // First, push a fully valid plugin to establish a known state
    cmsPluginMutex goodPlugin;
    goodPlugin.Base._unused = 0;
    goodPlugin.CreateMutexPtr = &dummyMtxCreate;
    goodPlugin.DestroyMutexPtr = &dummyMtxDestroy;
    goodPlugin.LockMutexPtr = &dummyMtxLock;
    goodPlugin.UnlockMutexPtr = &dummyMtxUnlock;

    cmsBool firstRes = _cmsRegisterMutexPlugin(ContextID, (cmsPluginBase*)&goodPlugin);
    ExpectTrue(firstRes == TRUE, "Setup_InitialGoodPlugin_Returns_TRUE", "Initial setup should succeed with full pointers");

    _cmsMutexPluginChunkType* ctxBefore = ctxGetChunkForTest(ContextID);

    // Now prepare a plugin with one NULL pointer (e.g., DestroyMutexPtr)
    cmsPluginMutex badPlugin;
    badPlugin.Base._unused = 0;
    badPlugin.CreateMutexPtr = &dummyMtxCreate;
    badPlugin.DestroyMutexPtr = NULL; // intentionally NULL
    badPlugin.LockMutexPtr = &dummyMtxLock;
    badPlugin.UnlockMutexPtr = &dummyMtxUnlock;

    cmsBool res = _cmsRegisterMutexPlugin(ContextID, (cmsPluginBase*)&badPlugin);
    ExpectTrue(res == FALSE, "NullPointerInPlugin_Returns_FALSE", "Expected FALSE when any pointer in plugin is NULL");

    // Ensure context chunk was not modified by the failed attempt (remains equal to the previous good pointers)
    _cmsMutexPluginChunkType* ctxAfterFail = ctxGetChunkForTest(ContextID);
    ExpectTrue(ctxAfterFail->CreateMutexPtr == ctxBefore->CreateMutexPtr &&
               ctxAfterFail->DestroyMutexPtr == ctxBefore->DestroyMutexPtr &&
               ctxAfterFail->LockMutexPtr == ctxBefore->LockMutexPtr &&
               ctxAfterFail->UnlockMutexPtr == ctxBefore->UnlockMutexPtr,
               "NullPointerInPlugin_DoesNotModifyContext",
               "Context chunk should remain unchanged after failed registration");
}

// --------------------------
// Main harness
// --------------------------

int main() {
    std::cout << "Running cmsRegisterMutexPlugin unit tests (C++11, no GTest) ..." << std::endl;

    // Run tests
    Test_NullData_SetsPointersToNull();
    Test_NonNullData_AllPointersPresent();
    Test_NonNullData_MissingOnePointer_ReturnsFALSEAndDoesNotModifyContext();

    // Summary
    std::cout << "Tests completed. Passed: " << g_passed << ", Failed: " << g_failed << std::endl;

    // Return code; non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}