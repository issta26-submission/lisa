// Test suite for cmsRegisterTagPlugin (mocked minimal environment)
// This test suite uses a self-contained environment to validate the behavior
// of the focal function _cmsRegisterTagPlugin without relying on the full
// cmsLCMS2 project. It covers the true/false branches and insertion into the
// linked list as described in the focal method.
//
// Notes:
// - No GTest is used. A small inline test harness is implemented.
// - Static/global helpers are provided to emulate the environment (_cmsContextGetClientChunk,
//   _cmsPluginMalloc, and the TagPlugin chunk storage).
// - Tests are written to be executable with C++11 compiler.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain knowledge: minimal reimplementation of the necessary types and functions
// to exercise cmsRegisterTagPlugin in a controlled manner.

typedef int cmsBool;
#define TRUE 1
#define FALSE 0

typedef void* cmsContext;

// Forward declarations of structures used by the focal function
struct cmsPluginBase { int dummy; };
struct cmsPluginTag { unsigned Signature; void* Descriptor; };

struct _cmsTagLinkedList {
    unsigned Signature;
    void* Descriptor;
    _cmsTagLinkedList* Next;
};

struct _cmsTagPluginChunkType {
    _cmsTagLinkedList* Tag;
};

// Simulated TagPlugin chunk storage (per the test harness)
static _cmsTagPluginChunkType g_TagPluginChunk;

// Chunk identifier enum (mirroring the real code's TagPlugin enum)
enum _cmsChunkType { TagPlugin = 0 };

// Control flag to simulate allocation failure
static bool g_mallocFail = false;

// Mocked dependency: _cmsContextGetClientChunk
// Returns a pointer to the appropriate chunk structure based on the id and chunk type.
// For tests we only need TagPlugin, so we return pointer to g_TagPluginChunk.
void* _cmsContextGetClientChunk(cmsContext id, int chunk)
{
    (void)id; // unused in tests
    if (chunk == TagPlugin) {
        return &g_TagPluginChunk;
    }
    return NULL;
}

// Mocked dependency: _cmsPluginMalloc
// Simulates memory allocation, with the ability to force failure.
void* _cmsPluginMalloc(cmsContext id, size_t size)
{
    (void)id; // unused in tests
    if (g_mallocFail) return NULL;
    return std::malloc(size);
}

// Re-implemented minimal version of the focal function under test
cmsBool _cmsRegisterTagPlugin(cmsContext id, cmsPluginBase* Data)
{
{
    cmsPluginTag* Plugin = (cmsPluginTag*)Data;
    _cmsTagLinkedList *pt;
    _cmsTagPluginChunkType* TagPluginChunk = ( _cmsTagPluginChunkType*) _cmsContextGetClientChunk(id, TagPlugin);
    if (Data == NULL) {
        TagPluginChunk->Tag = NULL;
        return TRUE;
    }
    pt = (_cmsTagLinkedList*) _cmsPluginMalloc(id, sizeof(_cmsTagLinkedList));
    if (pt == NULL) return FALSE;
    pt ->Signature  = Plugin ->Signature;
    pt ->Descriptor = Plugin ->Descriptor;
    pt ->Next       = TagPluginChunk ->Tag;
    TagPluginChunk ->Tag = pt;
    
    return TRUE;
}
}

// Helpers for test hygiene
void freeTagList(_cmsTagLinkedList* head)
{
    while (head) {
        _cmsTagLinkedList* next = head->Next;
        std::free(head);
        head = next;
    }
}

// Test 1: Data == NULL should return TRUE and reset Tag to NULL
// This tests the NULL-branch in the focal method.
bool test_DataNull_ReturnsTrue_TagResetToNull()
{
    // Arrange
    g_TagPluginChunk.Tag = NULL;
    g_mallocFail = false;
    cmsPluginBase* Data = NULL;

    // Act
    cmsBool res = _cmsRegisterTagPlugin(NULL, Data);

    // Assert
    bool ok = (res == TRUE) && (g_TagPluginChunk.Tag == NULL);
    if (!ok) {
        std::cerr << "Test 1 FAILED: Data == NULL branch did not set Tag to NULL or did not return TRUE.\n";
    }
    return ok;
}

// Test 2: Non-NULL Data with successful allocation inserts a new node with correct fields
// This tests the normal path where a node is added to the head of the Tag list.
bool test_DataNonNull_AllocSucceeds_InsertsNodeAtHead()
{
    // Arrange
    g_TagPluginChunk.Tag = NULL;
    g_mallocFail = false;

    cmsPluginTag plugin;
    plugin.Signature = 0xABCDEF01;
    plugin.Descriptor = (void*)0x12345678;

    cmsPluginBase* Data = (cmsPluginBase*) &plugin;

    // Act
    cmsBool res = _cmsRegisterTagPlugin(NULL, Data);

    // Assert
    bool ok = (res == TRUE) && (g_TagPluginChunk.Tag != NULL);
    if (ok) {
        _cmsTagLinkedList* head = g_TagPluginChunk.Tag;
        ok = (head->Signature == plugin.Signature) &&
             (head->Descriptor == plugin.Descriptor) &&
             (head->Next == NULL);
    }
    if (!ok) {
        std::cerr << "Test 2 FAILED: Data != NULL with successful alloc did not insert node with correct fields.\n";
    }

    // Cleanup
    freeTagList(g_TagPluginChunk.Tag);
    g_TagPluginChunk.Tag = NULL;

    return ok;
}

// Test 3: Non-NULL Data with allocation failure returns FALSE and leaves Tag unchanged
bool test_DataNonNull_AllocFails_ReturnsFalse_NoTagChange()
{
    // Arrange
    g_TagPluginChunk.Tag = NULL;
    g_mallocFail = true;

    cmsPluginTag plugin;
    plugin.Signature = 0x11223344;
    plugin.Descriptor = (void*)0xAABBCCDD;

    cmsPluginBase* Data = (cmsPluginBase*) &plugin;

    // Act
    cmsBool res = _cmsRegisterTagPlugin(NULL, Data);

    // Assert
    bool ok = (res == FALSE) && (g_TagPluginChunk.Tag == NULL);
    if (!ok) {
        std::cerr << "Test 3 FAILED: Allocation failure did not return FALSE or Tag changed unexpectedly.\n";
    }

    // Cleanup
    g_mallocFail = false;
    return ok;
}

// Test 4: Insertion when existing list is present inserts at head (Next points to old head)
bool test_InsertAtHead_WhenExistingList_PointedToOldHead()
{
    // Arrange
    // Create existing head
    _cmsTagLinkedList* existing = (_cmsTagLinkedList*)std::malloc(sizeof(_cmsTagLinkedList));
    existing->Signature = 0xDEADBEAF;
    existing->Descriptor = (void*)0xCAFEBABE;
    existing->Next = NULL;

    g_TagPluginChunk.Tag = existing;
    g_mallocFail = false;

    cmsPluginTag plugin;
    plugin.Signature = 0xFEEDFACE;
    plugin.Descriptor = (void*)0x11223344;

    cmsPluginBase* Data = (cmsPluginBase*) &plugin;

    // Act
    cmsBool res = _cmsRegisterTagPlugin(NULL, Data);

    // Assert
    bool ok = (res == TRUE) && (g_TagPluginChunk.Tag != NULL) && (g_TagPluginChunk.Tag->Next == existing)
              && (g_TagPluginChunk.Tag->Signature == plugin.Signature)
              && (g_TagPluginChunk.Tag->Descriptor == plugin.Descriptor);

    if (!ok) {
        std::cerr << "Test 4 FAILED: Insertion at head with existing list did not update head correctly.\n";
    }

    // Cleanup
    freeTagList(g_TagPluginChunk.Tag); // frees new head and existing via Next
    g_TagPluginChunk.Tag = NULL;

    return ok;
}

int main()
{
    // Run all tests and report summary
    int total = 0;
    int passed = 0;

    auto run = [&](bool (*fn)(), const char* name) -> void {
        ++total;
        bool ok = fn();
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << "\n";
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    run(test_DataNull_ReturnsTrue_TagResetToNull, "DataNull_ReturnsTrue_TagResetToNull");
    run(test_DataNonNull_AllocSucceeds_InsertsNodeAtHead, "DataNonNull_AllocSucceeds_InsertsNodeAtHead");
    run(test_DataNonNull_AllocFails_ReturnsFalse_NoTagChange, "DataNonNull_AllocFails_ReturnsFalse_NoTagChange");
    run(test_InsertAtHead_WhenExistingList_PointedToOldHead, "InsertAtHead_WhenExistingList_PointedToOldHead");

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}