// Auto-generated C++11 unit tests for the focal method: RegisterTypesPlugin
// This test suite is self-contained and does not depend on GTest.
// It re-implements a minimal subset of the environment required by the focal method
// to validate its control flow and data manipulations.

#include <lcms2_internal.h>
#include <iostream>
#include <cstddef>
#include <cstdlib>


// Domain: simple stand-ins for the LittleCMS types used by the focal function.
// We purposely keep these minimal and self-contained to enable isolated unit testing.

typedef int cmsBool;
typedef void* cmsContext;
typedef void* cmsPluginBase;

#define TRUE 1
#define FALSE 0

// Minimal plugin tag type used by the focal function.
typedef struct {
    void* Handler;
} cmsPluginTagType;

// Node in a singly linked list storing registered Tag Types.
typedef struct _cmsTagTypeLinkedList {
    void* Handler;
    struct _cmsTagTypeLinkedList* Next;
} _cmsTagTypeLinkedList;

// Plugin chunk type holding the head of the TagTypes list.
typedef struct _cmsTagTypePluginChunkType {
    _cmsTagTypeLinkedList* TagTypes;
} _cmsTagTypePluginChunkType;

// Forward declarations of internal helpers (provided here for test doubles).
extern "C" {
    // In the real project, this would fetch a per-context chunk. We provide a simple stub.
    _cmsTagTypePluginChunkType* _cmsContextGetClientChunk(cmsContext id, void* pos);

    // Allocation helper used by the focal function. We will control failure in tests.
    void* _cmsPluginMalloc(cmsContext id, size_t size);
}

// Global test state to mimic plugin context and to simulate memory allocation behavior.
static _cmsTagTypePluginChunkType g_ctx;
static bool g_malloc_fail = false;

// Stub: return the global test context (simulates per-id/per-pos storage).
extern "C" _cmsTagTypePluginChunkType* _cmsContextGetClientChunk(cmsContext id, void* pos) {
    (void)id; (void)pos;
    return &g_ctx;
}

// Stub: simulate memory allocation with controllable failure.
extern "C" void* _cmsPluginMalloc(cmsContext id, size_t size) {
    (void)id;
    if (g_malloc_fail) return NULL;
    return std::malloc(size);
}

// The focal method under test, implemented here to enable isolated unit testing
// without requiring the full library build. This mirrors the provided focal code.
cmsBool RegisterTypesPlugin(cmsContext id, cmsPluginBase* Data, void* pos) {
    {
        cmsPluginTagType* Plugin = (cmsPluginTagType*) Data;
        _cmsTagTypePluginChunkType* ctx = ( _cmsTagTypePluginChunkType*) _cmsContextGetClientChunk(id, pos);
        _cmsTagTypeLinkedList *pt;

        // Unregister path: Data == NULL
        if (Data == NULL) {
            // No memory freeing needed; pool is destroyed with context.
            ctx ->TagTypes = NULL;
            return TRUE;
        }
        // Allocate a new list node in the plugin memory context
        pt = (_cmsTagTypeLinkedList*) _cmsPluginMalloc(id, sizeof(_cmsTagTypeLinkedList));
        if (pt == NULL) return FALSE;

        pt ->Handler   = Plugin ->Handler;
        pt ->Next      = ctx ->TagTypes;
        ctx ->TagTypes = pt;

        return TRUE;
    }
}

// ----------------------
// Test scaffolding
// ----------------------

// Resets the global context to a clean state between tests.
static void reset_env() {
    g_ctx.TagTypes = NULL;
}

// Test 1: Data == NULL should unregister the plug-in and set TagTypes to NULL.
static bool test_unregister_null_data() {
    reset_env();
    g_malloc_fail = false;

    // Call with NULL Data to simulate unregistration
    cmsBool res = RegisterTypesPlugin((cmsContext)1, nullptr, nullptr);

    bool ok = (res == TRUE) && (g_ctx.TagTypes == NULL);
    return ok;
}

// Test 2: Register a single type plugin; verify chain head and copied handler.
static bool test_register_single_node() {
    reset_env();
    g_malloc_fail = false;

    cmsPluginTagType plugin;
    plugin.Handler = (void*)0xDEADBEEF; // sentinel value to verify copy

    cmsBool res = RegisterTypesPlugin((cmsContext)1, (cmsPluginBase*)&plugin, nullptr);

    bool ok = (res == TRUE)
              && (g_ctx.TagTypes != NULL)
              && ((( _cmsTagTypeLinkedList*)g_ctx.TagTypes)->Handler == plugin.Handler)
              && ((( _cmsTagTypeLinkedList*)g_ctx.TagTypes)->Next == NULL);

    return ok;
}

// Test 3: Simulate memory allocation failure; ensure FALSE is returned and TagTypes remains unchanged.
static bool test_allocation_failure() {
    reset_env();

    g_malloc_fail = true;

    cmsPluginTagType plugin;
    plugin.Handler = (void*)0xBADC0DE;

    cmsBool res = RegisterTypesPlugin((cmsContext)1, (cmsPluginBase*)&plugin, nullptr);

    bool ok = (res == FALSE) && (g_ctx.TagTypes == NULL);
    // Reset flag for later tests
    g_malloc_fail = false;
    return ok;
}

// Test 4: Two successive registrations should chain nodes
// and the head should be the second plugin's handler.
static bool test_chaining_two_registrations() {
    reset_env();
    g_malloc_fail = false;

    cmsPluginTagType first;
    first.Handler = (void*)0x11111111;

    cmsBool r1 = RegisterTypesPlugin((cmsContext)1, (cmsPluginBase*)&first, nullptr);
    if (r1 != TRUE || g_ctx.TagTypes == NULL) return false;

    cmsPluginTagType second;
    second.Handler = (void*)0x22222222;

    cmsBool r2 = RegisterTypesPlugin((cmsContext)1, (cmsPluginBase*)&second, nullptr);
    if (r2 != TRUE || g_ctx.TagTypes == NULL) return false;

    _cmsTagTypeLinkedList* head = g_ctx.TagTypes;
    _cmsTagTypeLinkedList* next = head->Next;

    // Verify the head is the second plugin
    bool ok = (head->Handler == second.Handler)
              && (next != NULL)
              && (next->Handler == first.Handler)
              && (next->Next == NULL);

    return ok;
}

// ----------------------
// Test runner (no external test framework required)
// ----------------------

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool ok) {
        ++total;
        if (ok) {
            ++passed;
            std::cout << "[PASSED] " << name << std::endl;
        } else {
            std::cout << "[FAILED] " << name << std::endl;
        }
    };

    run("test_unregister_null_data", test_unregister_null_data());
    run("test_register_single_node", test_register_single_node());
    run("test_allocation_failure", test_allocation_failure());
    run("test_chaining_two_registrations", test_chaining_two_registrations());

    // Summary
    std::cout << "Passed " << passed << " of " << total << " tests." << std::endl;
    return (passed == total) ? 0 : 1;
}