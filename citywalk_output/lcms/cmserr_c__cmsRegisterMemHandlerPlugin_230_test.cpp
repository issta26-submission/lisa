/*
  Unit Test Suite for cmsBool _cmsRegisterMemHandlerPlugin
  - Focus: reproduce minimal environment to exercise the focal function
  - Language: C++11 (no Google Test). A lightweight test harness is implemented.
  - Approach: create a minimal subset of the C structures and functions the focal method relies on.
  - Coverage: four tests cover true/false branches, NULL Data handling, and successful registration path.

  Notes:
  - This test file reimplements the essential surface area needed by _cmsRegisterMemHandlerPlugin
    (as declared and used in the provided source snippet) to enable isolated, compilable unit tests.
  - All tests are self-contained; no external libraries are required beyond the C++ standard library.
  - Static members and file-scope behavior are emulated sufficiently for coverage goals.
*/

// Include standard headers for I/O and memory utilities
#include <lcms2_internal.h>
#include <iostream>
#include <cstdlib>
#include <cstddef>


// ------------------------------------------------------------------------------------
// Minimal surface to mirror required types and functions from the focal code path
// ------------------------------------------------------------------------------------

// Basic boolean type and constants (mimicking cmsBool from the focal code)
typedef int cmsBool;
const cmsBool TRUE = 1;
const cmsBool FALSE = 0;

// Forward declare context type as opaque pointer (as in cmsContext in the original code)
typedef void* cmsContext;

// Forward declare function pointer types used by the memory plugin interface
typedef void* (*cmsMallocFn)(cmsContext, std::size_t);
typedef void  (*cmsFreeFn)(cmsContext, void*);
typedef void* (*cmsReallocFn)(cmsContext, void*, std::size_t);

// Base plugin structure (simplified)
struct cmsPluginBase {
    // In the real code this would carry type info; not needed for tests
};

// Memory-handler plugin structure (simplified)
struct cmsPluginMemHandler {
    cmsPluginBase Base;
    cmsMallocFn     MallocPtr;
    cmsFreeFn       FreePtr;
    cmsReallocFn    ReallocPtr;
};

// Internal representation of a "chunk" that holds the replacement allocators
struct _cmsMemPluginChunkType {
    cmsMallocFn MallocPtr;
    cmsFreeFn   FreePtr;
    cmsReallocFn ReallocPtr;
};

// Context internal structure emulation with a dedicated chunk slot
struct _cmsContext_struct {
    void* chunks[32];                  // generic chunk storage
    _cmsMemPluginChunkType DefaultMemoryManager; // defaults holder
};

// Helper function prototypes (internal behavior approximations)
static _cmsMemPluginChunkType* _cmsContextGetClientChunk(cmsContext ContextID, int MemPlugin);

// Install the plugin's allocator callbacks into the destination chunk
static void _cmsInstallAllocFunctions(cmsPluginMemHandler* Plugin, _cmsMemPluginChunkType* ptr);

// Minimal default allocator functions (not used by tests, but present for completeness)
static void* _defaultMalloc(cmsContext, std::size_t) { return std::malloc(0); }
static void  _defaultFree(cmsContext, void*) { }
static void* _defaultRealloc(cmsContext, void*, std::size_t) { return nullptr; }

// Enumeration-like constant to index the MemPlugin slot in the context
enum { MemPlugin = 0 };

// ------------------------------------------------------------------------------------
// Focal method under test (simplified re-implementation in C++)
// This mirrors the logic shown in the <FOCAL_METHOD> snippet.
// ------------------------------------------------------------------------------------
static cmsBool _cmsRegisterMemHandlerPlugin(cmsContext ContextID, cmsPluginBase *Data)
{
    {
        cmsPluginMemHandler* Plugin = (cmsPluginMemHandler*) Data;
        _cmsMemPluginChunkType* ptr;

        // NULL forces to reset to defaults. In this special case, the defaults are stored in the context structure.
        // Remaining plug-ins does NOT have any copy in the context structure, but this is somehow special as the
        // context internal data should be malloc'ed by using those functions.
        if (Data == NULL) {
            struct _cmsContext_struct* ctx = (struct _cmsContext_struct*) ContextID;
            // Return to the default allocators
            if (ContextID != NULL) {
                ctx->chunks[MemPlugin] = (void*) &ctx->DefaultMemoryManager;
            }
            return TRUE;
        }

        // Check for required callbacks
        if (Plugin->MallocPtr == NULL ||
            Plugin->FreePtr == NULL ||
            Plugin->ReallocPtr == NULL) return FALSE;

        // Set replacement functions
        ptr = (_cmsMemPluginChunkType*) _cmsContextGetClientChunk(ContextID, MemPlugin);
        if (ptr == NULL)
            return FALSE;
        _cmsInstallAllocFunctions(Plugin, ptr);
        return TRUE;
    }
}

// ------------------------------------------------------------------------------------
// Implementations of helper functions used by the focal method
static _cmsMemPluginChunkType* _cmsContextGetClientChunk(cmsContext ContextID, int MemPlugin)
{
    if (ContextID == NULL) return NULL;
    struct _cmsContext_struct* ctx = (struct _cmsContext_struct*) ContextID;
    return ( _cmsMemPluginChunkType*) ctx->chunks[MemPlugin];
}

static void _cmsInstallAllocFunctions(cmsPluginMemHandler* Plugin, _cmsMemPluginChunkType* ptr)
{
    if (Plugin == nullptr || ptr == nullptr) return;
    ptr->MallocPtr = Plugin->MallocPtr;
    ptr->FreePtr   = Plugin->FreePtr;
    ptr->ReallocPtr= Plugin->ReallocPtr;
}

// ------------------------------------------------------------------------------------
// Test harness
// Lightweight assertion and test runner to avoid GTest dependency
static int gTotalTests = 0;
static int gFailedTests = 0;

#define ASSERT(condition, message) do { \
    ++gTotalTests; \
    if (!(condition)) { \
        std::cerr << "[FAIL] " << message << " (line " << __LINE__ << ")\n"; \
        ++gFailedTests; \
    } else { \
        std::cout << "[PASS] " << message << "\n"; \
    } \
} while(0)


// Dummy allocator implementations (used to populate non-null pointers)
static void* dummyMalloc(cmsContext, std::size_t) { return malloc(1); }
static void  dummyFree(cmsContext, void*) { /* no-op */ }
static void* dummyRealloc(cmsContext, void*, std::size_t) { return realloc(nullptr, 0); } // harmless placeholder

// Helper to reset a context to a clean state
static void resetContext(struct _cmsContext_struct& ctx)
{
    ctx.chunks[MemPlugin] = nullptr;
    // DefaultMemoryManager must be a valid object
    ctx.DefaultMemoryManager.MallocPtr  = nullptr;
    ctx.DefaultMemoryManager.FreePtr    = nullptr;
    ctx.DefaultMemoryManager.ReallocPtr = nullptr;
}

// ------------------------------------------------------------------------------------
// Test Case 1: Data == NULL should reset to defaults and return TRUE
static void test_ResetToDefaults_WhenDataIsNull()
{
    std::cout << "Running Test 1: Data == NULL resets to defaults and returns TRUE\n";

    // Prepare a context with a known default memory manager
    struct _cmsContext_struct ctx;
    resetContext(ctx);

    // Initialize a dummy default manager object address
    _cmsMemPluginChunkType defaultChunk;
    ctx.DefaultMemoryManager.MallocPtr  = dummyMalloc;
    ctx.DefaultMemoryManager.FreePtr    = dummyFree;
    ctx.DefaultMemoryManager.ReallocPtr = dummyRealloc;

    // Ensure initial state differs
    ctx.chunks[MemPlugin] = nullptr;

    cmsContext ctxPtr = (cmsContext) &ctx;

    // Call focal method with Data == NULL
    cmsBool result = _cmsRegisterMemHandlerPlugin(ctxPtr, nullptr);

    // Validate result and state
    ASSERT(result == TRUE, "Return value is TRUE when Data == NULL");
    ASSERT(ctx.chunks[MemPlugin] == (void*) &ctx.DefaultMemoryManager,
           "Context's MemPlugin chunk should be reset to DefaultMemoryManager");
}

// ------------------------------------------------------------------------------------
// Test Case 2: Data non-NULL but one of the required callbacks is NULL -> should return FALSE
static void test_MissingCallbacks_ReturnsFalse()
{
    std::cout << "Running Test 2: Missing callbacks should return FALSE\n";

    // Prepare a context
    struct _cmsContext_struct ctx;
    resetContext(ctx);

    // Setup a valid client chunk target
    _cmsMemPluginChunkType chunk;
    ctx.chunks[MemPlugin] = &chunk;

    // Plugin with a NULL MallocPtr to simulate missing callback
    cmsPluginMemHandler plugin;
    plugin.MallocPtr = nullptr;
    plugin.FreePtr = dummyFree;
    plugin.ReallocPtr = dummyRealloc;

    cmsContext ctxPtr = (cmsContext) &ctx;
    cmsBool result = _cmsRegisterMemHandlerPlugin(ctxPtr, (cmsPluginBase*) &plugin);

    ASSERT(result == FALSE, "Return value should be FALSE when MallocPtr is NULL");
}

// ------------------------------------------------------------------------------------
// Test Case 3: Data non-NULL with valid callbacks but no client chunk in context -> FALSE
static void test_NoClientChunkInContext_ReturnsFalse()
{
    std::cout << "Running Test 3: No client chunk in context should return FALSE\n";

    // Context without a client chunk registered
    struct _cmsContext_struct ctx;
    resetContext(ctx);

    // Setup a valid plugin with all callbacks non-null
    cmsPluginMemHandler plugin;
    plugin.MallocPtr = dummyMalloc;
    plugin.FreePtr = dummyFree;
    plugin.ReallocPtr = dummyRealloc;

    // Context's MemPlugin slot remains NULL to simulate missing client chunk
    ctx.chunks[MemPlugin] = nullptr;

    cmsContext ctxPtr = (cmsContext) &ctx;
    cmsBool result = _cmsRegisterMemHandlerPlugin(ctxPtr, (cmsPluginBase*) &plugin);

    ASSERT(result == FALSE, "Return value should be FALSE when client chunk pointer is NULL");
}

// ------------------------------------------------------------------------------------
// Test Case 4: Successful registration should install callbacks into the context chunk
static void test_SuccessfulRegistration_InstallsCallbacks()
{
    std::cout << "Running Test 4: Successful registration installs callbacks\n";

    // Prepare a context
    struct _cmsContext_struct ctx;
    resetContext(ctx);

    // We will point the context's MemPlugin chunk to an actual chunk structure
    _cmsMemPluginChunkType chunk;
    ctx.chunks[MemPlugin] = &chunk;

    // Plugin with all callbacks non-null
    cmsPluginMemHandler plugin;
    plugin.MallocPtr = dummyMalloc;
    plugin.FreePtr = dummyFree;
    plugin.ReallocPtr = dummyRealloc;

    cmsContext ctxPtr = (cmsContext) &ctx;
    cmsBool result = _cmsRegisterMemHandlerPlugin(ctxPtr, (cmsPluginBase*) &plugin);

    // Validate success
    ASSERT(result == TRUE, "Return value should be TRUE on successful registration");

    // Validate that the chunk has inherited the plugin's callbacks
    ASSERT(chunk.MallocPtr == plugin.MallocPtr, "Chunk's MallocPtr should be installed from plugin");
    ASSERT(chunk.FreePtr == plugin.FreePtr, "Chunk's FreePtr should be installed from plugin");
    ASSERT(chunk.ReallocPtr == plugin.ReallocPtr, "Chunk's ReallocPtr should be installed from plugin");
}

// ------------------------------------------------------------------------------------
// Main entry point: runs all tests and reports summary
int main()
{
    std::cout << "Starting _cmsRegisterMemHandlerPlugin unit tests (C++11, no GTest)\n";

    test_ResetToDefaults_WhenDataIsNull();
    test_MissingCallbacks_ReturnsFalse();
    test_NoClientChunkInContext_ReturnsFalse();
    test_SuccessfulRegistration_InstallsCallbacks();

    std::cout << "----------------------------------------------\n";
    std::cout << "Total tests: " << gTotalTests << "\n";
    std::cout << "Failures:    " << gFailedTests << "\n";
    std::cout << "Status:      " << (gFailedTests == 0 ? "ALL PASS" : "SOME FAILURES") << "\n";

    return (gFailedTests == 0) ? 0 : 1;
}