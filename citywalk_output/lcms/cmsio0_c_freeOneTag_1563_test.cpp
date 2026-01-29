// Minimal unit-test suite for the focal method freeOneTag (reproduced inline for testability)
// This test harness is self-contained and does not rely on GTest.
// It covers true/false branches of the two key predicates inside freeOneTag.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain: lightweight mock types to model _cmsICCPROFILE and cmsTagTypeHandler used by freeOneTag.

// Typedefs matching the original code semantics (simplified for test environment)
typedef unsigned int cmsUInt32Number;
typedef bool cmsBool;
#define TRUE  true
#define FALSE false

// Forward declaration of the test's FreePtr function type
typedef struct cmsTagTypeHandler cmsTagTypeHandler;

// Mock of the tag type handler structure used by freeOneTag
struct cmsTagTypeHandler {
    cmsUInt32Number ContextID;
    cmsUInt32Number ICCVersion;
    void (*FreePtr)(cmsTagTypeHandler*, void*);
};

// Minimal mock of the ICC profile structure used by freeOneTag
struct _cmsICCPROFILE {
    cmsUInt32Number ContextID;
    cmsUInt32Number Version;
    void** TagPtrs;                    // array of pointers to tag data
    cmsTagTypeHandler** TagTypeHandlers; // array of pointers to tag type handlers
};

// Forward declaration of the focal function (reproduced for testability)
void freeOneTag(_cmsICCPROFILE* Icc, cmsUInt32Number i);

// Reproduce necessary internal API points (simplified)
static void _cmsFree(cmsUInt32Number ContextID, void* Ptr) {
    // In test, free path is tracked by g_freed_ptrs
    // We keep a global record of freed pointers for assertions.
    extern std::vector<void*> g_freed_ptrs;
    g_freed_ptrs.push_back(Ptr);
}

// Global test state for FreePtr path verification
static cmsUInt32Number g_test_context_id;
static cmsUInt32Number g_test_version;
static int g_freeptr_path_ok = 0; // set to 1 when FreePtr path matches expectations
static std::vector<void*> g_freed_ptrs; // tracks pointers freed via _cmsFree

// FreePtr function used by the test to verify that LocalTypeHandler contents are updated
static void TestFreePtrFunc(cmsTagTypeHandler* self, void* data) {
    // Validate that FreePtr sees the correct ContextID and ICCVersion set by freeOneTag
    if (self->ContextID == g_test_context_id && self->ICCVersion == g_test_version) {
        g_freeptr_path_ok = 1;
    } else {
        // Incorrect state observed; count as a failure (non-terminating)
        std::cerr << "FreePtr observed incorrect ContextID/ICCVersion: "
                  << self->ContextID << "/" << self->ICCVersion << "\n";
    }
    // Do not free data here (we keep control of memory in test flow)
    (void)data; // suppress unused warning if compiled with -Wextra
}

// Implementation of the focal function (inline reproduction)
void freeOneTag(_cmsICCPROFILE* Icc, cmsUInt32Number i)
{
{
    if (Icc->TagPtrs[i]) {
        cmsTagTypeHandler* TypeHandler = Icc->TagTypeHandlers[i];
        if (TypeHandler != NULL) {
            cmsTagTypeHandler LocalTypeHandler = *TypeHandler;
            LocalTypeHandler.ContextID = Icc->ContextID;             
            LocalTypeHandler.ICCVersion = Icc->Version;
            LocalTypeHandler.FreePtr(&LocalTypeHandler, Icc->TagPtrs[i]);
        }
        else
            _cmsFree(Icc->ContextID, Icc->TagPtrs[i]);
    }
    Icc->TagPtrs[i] = NULL;
}
}

// Helper: create a small ICC profile with required arrays
static _cmsICCPROFILE* CreateProfile(size_t size) {
    _cmsICCPROFILE* p = new _cmsICCPROFILE;
    p->ContextID = 0;
    p->Version = 0;
    p->TagPtrs = new void*[size];
    p->TagTypeHandlers = new cmsTagTypeHandler*[size];
    std::memset(p->TagPtrs, 0, size * sizeof(void*));
    std::memset(p->TagTypeHandlers, 0, size * sizeof(cmsTagTypeHandler*));
    return p;
}

// Helper: cleanup profile
static void DestroyProfile(_cmsICCPROFILE* p) {
    if (!p) return;
    // Note: Tests manage memory of TagPtrs independently when needed
    delete[] p->TagPtrs;
    delete[] p->TagTypeHandlers;
    delete p;
}

// Simple test harness (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(x) do { if(!(x)) { ++g_failures; std::cerr << "EXPECT_TRUE failed: " #x " at " << __FILE__ << ":" << __LINE__ << "\n"; } } while(0)
#define EXPECT_FALSE(x) do { if((x)) { ++g_failures; std::cerr << "EXPECT_FALSE failed: " #x " at " << __FILE__ << ":" << __LINE__ << "\n"; } } while(0)
#define EXPECT_EQ(a,b) do { if(!((a)==(b))) { ++g_failures; std::cerr << "EXPECT_EQ failed: " #a " == " #b " (actual " << (a) << " vs " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; } } while(0)

// Test 1: TagPtrs[i] non-NULL and TypeHandler non-NULL -> FreePtr path invoked; TagPtrs[i] becomes NULL
static void test_freeOneTag_with_typehandler_nonnull_path() {
    // Domain setup
    g_test_context_id = 42;
    g_test_version = 7;
    g_freeptr_path_ok = 0;
    g_failures = 0;
    g_freed_ptrs.clear();
    // Profile with one tag
    _cmsICCPROFILE* Icc = CreateProfile(3);

    Icc->ContextID = g_test_context_id;
    Icc->Version = g_test_version;

    // Allocate tag data
    void* tag = malloc(16);
    Icc->TagPtrs[0] = tag;

    // Set non-NULL TypeHandler with FreePtr pointing to TestFreePtrFunc
    cmsTagTypeHandler handler;
    handler.ContextID = 0; // value overwritten in freeOneTag
    handler.ICCVersion = 0;
    handler.FreePtr = TestFreePtrFunc;
    Icc->TagTypeHandlers[0] = &handler;

    // Execute
    freeOneTag(Icc, 0);

    // Verify: TagPtrs[0] should be NULL
    EXPECT_TRUE(Icc->TagPtrs[0] == NULL);
    // Verify FreePtr path was taken and observed proper ContextID/ICCVersion
    EXPECT_TRUE(g_freeptr_path_ok == 1);

    // Cleanup: the test does not rely on FreePtr to free memory
    // Free the allocated tag to avoid leaks
    // (TagPtrs[0] is NULL now; we still hold 'tag' pointer)
    free(tag);

    DestroyProfile(Icc);
}

// Test 2: TagPtrs[i] non-NULL but TagTypeHandlers[i] NULL -> _cmsFree path invoked
static void test_freeOneTag_with_null_typehandler_free_path() {
    // Reset test state
    g_test_context_id = 55;
    g_test_version = 1;
    g_failures = 0;
    g_freed_ptrs.clear();

    _cmsICCPROFILE* Icc = CreateProfile(3);
    Icc->ContextID = g_test_context_id;
    Icc->Version = g_test_version;

    void* tag = malloc(24);
    Icc->TagPtrs[1] = tag;
    Icc->TagTypeHandlers[1] = NULL; // NULL path

    // Execute
    freeOneTag(Icc, 1);

    // Verify: TagPtrs[1] NULL
    EXPECT_TRUE(Icc->TagPtrs[1] == NULL);
    // Verify _cmsFree path invoked by ensuring the free pointer was recorded
    EXPECT_TRUE(g_freed_ptrs.size() == 1 && g_freed_ptrs[0] == tag);

    // Cleanup
    // The memory should have been recorded as freed; nothing else to free here
    DestroyProfile(Icc);
}

// Test 3: TagPtrs[i] NULL -> no action occurs
static void test_freeOneTag_with_null_ptr_no_action() {
    // Reset test state
    g_test_context_id = 99;
    g_test_version = 2;
    g_failures = 0;
    g_freeptr_path_ok = 0;
    g_freed_ptrs.clear();

    _cmsICCPROFILE* Icc = CreateProfile(3);
    Icc->ContextID = g_test_context_id;
    Icc->Version = g_test_version;

    // Ensure TagPtrs[2] is NULL
    Icc->TagPtrs[2] = NULL;
    Icc->TagTypeHandlers[2] = NULL; // doesn't matter

    freeOneTag(Icc, 2);

    // Verify: no changes to flags
    EXPECT_FALSE(g_freeptr_path_ok);
    EXPECT_TRUE(g_freed_ptrs.empty());

    DestroyProfile(Icc);
}

// Test runner
int main() {
    test_freeOneTag_with_typehandler_nonnull_path();
    test_freeOneTag_with_null_typehandler_free_path();
    test_freeOneTag_with_null_ptr_no_action();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED.\n";
        return 1;
    }
}