// Lightweight unit tests for _cmsDeleteTagByPos in cmsio0.c
// This test suite does not rely on GTest; it uses a small in-file harness.
// It targets true/false branches of the critical predicate and validates
// that the function interacts with TagPtrs, TagSaveAsRaw and TagTypeHandlers
// as described in the focal method.
//
// Assumptions:
// - The project exposes the internal types _cmsICCPROFILE, cmsTagTypeHandler, cmsBool, etc.
// - The function _cmsDeleteTagByPos is visible with C linkage.
// - We link against the library that provides the actual implementations of the types and functions.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Bring in the internal structures/types used by cmsio0.c
extern "C" {
}

// Declare the function under test (from cmsio0.c). Use C linkage.
extern "C" void _cmsDeleteTagByPos(_cmsICCPROFILE* Icc, int i);

// Simple test harness
static int g_TestFailures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test Failure: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        ++g_TestFailures; \
    } \
} while(0)

static int g_FreeCalls = 0;

// Test hook: a fake FreePtr implementation that just records it's been called.
static void TestFreePtr(cmsTagTypeHandler* self, void* data)
{
    (void)self; (void)data;
    ++g_FreeCalls;
}

// Global fake tag type handler instance used for tests
static cmsTagTypeHandler FakeTypeHandler;

// Helper to initialize Icc structure for tests
static void init_test_Icc(_cmsICCPROFILE& Icc, int tagCount)
{
    // Zero the core structure to avoid uninitialized members
    std::memset(&Icc, 0, sizeof(Icc));

    // Allocate minimal arrays that _cmsDeleteTagByPos will touch
    // TagPtrs: array of pointers to tag data
    // TagSaveAsRaw: whether a tag was saved as RAW
    // TagTypeHandlers: per-tag type handler (may be NULL)
    // Note: We only manage memory for the arrays here; individual elements are set per-test.
    // The actual _cmsICCPROFILE struct layout is assumed to expose these fields.
    Icc.TagPtrs = new void*[tagCount];
    Icc.TagSaveAsRaw = new cmsBool[tagCount];
    Icc.TagTypeHandlers = new cmsTagTypeHandler*[tagCount];
    for (int i = 0; i < tagCount; ++i) {
        Icc.TagPtrs[i] = NULL;
        Icc.TagSaveAsRaw[i] = FALSE;
        Icc.TagTypeHandlers[i] = NULL;
    }

    Icc.ContextID = 0;
    Icc.Version = 0;
}

// Helper to cleanup the test Icc structure
static void cleanup_test_Icc(_cmsICCPROFILE& Icc)
{
    // Free per-tag data pointers if still allocated
    // Note: We cannot rely on _cmsDeleteTagByPos to free raw data here.
    // We'll free any non-null TagPtrs by the test after each scenario using the saved original pointer.
    // Free the arrays themselves
    if (Icc.TagPtrs) {
        // Any non-NULL pointers inside were deliberately freed by tests if needed.
        delete[] Icc.TagPtrs;
        Icc.TagPtrs = NULL;
    }
    if (Icc.TagSaveAsRaw) {
        delete[] Icc.TagSaveAsRaw;
        Icc.TagSaveAsRaw = NULL;
    }
    if (Icc.TagTypeHandlers) {
        delete[] Icc.TagTypeHandlers;
        Icc.TagTypeHandlers = NULL;
    }
}

// TEST 1: Ensure that when TagSaveAsRaw[i] is FALSE and a non-NULL TypeHandler exists,
// the FreePtr is invoked and TagPtrs[i] is set to NULL after deletion.
static void test_DeleteTagByPos_FreesViaTypeHandlerAndNullsPtr()
{
    _cmsICCPROFILE Icc;
    init_test_Icc(Icc, 4);

    // Prepare a non-NULL tag payload
    void* originalPtr0 = std::malloc(32);
    Icc.TagPtrs[0] = originalPtr0;
    Icc.TagSaveAsRaw[0] = FALSE;

    // Attach a fake type handler with a known FreePtr function
    FakeTypeHandler.FreePtr = TestFreePtr;
    Icc.TagTypeHandlers[0] = &FakeTypeHandler;

    // Call the function under test
    g_FreeCalls = 0;
    _cmsDeleteTagByPos(&Icc, 0);

    // Validate: FreePtr should have been invoked exactly once
    ASSERT_TRUE(g_FreeCalls == 1, "FreePtr should be called once when valid TypeHandler is present");

    // Validate that TagPtrs[0] has been set to NULL
    ASSERT_TRUE(Icc.TagPtrs[0] == NULL, "TagPtrs[0] should be NULL after deletion with valid TypeHandler");

    // Cleanup
    // originalPtr0 was not freed by the API (the test-side FreePtr doesn't free memory), so we free it now.
    // If the code path freed it, the pointer could be NULL here; but since FreePtr only increments a counter, we free the original.
    if (originalPtr0 != NULL) {
        std::free(originalPtr0);
    }
    cleanup_test_Icc(Icc);
}

// TEST 2: When TagSaveAsRaw[i] is TRUE, the function frees the tag memory via _cmsFree
// and clears TagSaveAsRaw, but does not set TagPtrs[i] to NULL (as per the original code path).
static void test_DeleteTagByPos_FreeRawPath_TogglesFlagOnly()
{
    _cmsICCPROFILE Icc;
    init_test_Icc(Icc, 4);

    void* originalPtr = std::malloc(24);
    Icc.TagPtrs[1] = originalPtr;
    Icc.TagSaveAsRaw[1] = TRUE;
    Icc.TagTypeHandlers[1] = NULL; // Ensure we take the RAW path

    // Call the function under test
    _cmsDeleteTagByPos(&Icc, 1);

    // Validate: TagSaveAsRaw should be FALSE after deletion
    ASSERT_TRUE(Icc.TagSaveAsRaw[1] == FALSE, "TagSaveAsRaw[1] should be FALSE after RAW deletion");

    // Validate: TagPtrs[1] should remain non-NULL (code path does not NULL-ify in this branch)
    ASSERT_TRUE(Icc.TagPtrs[1] == originalPtr, "TagPtrs[1] should remain unchanged in RAW deletion path");

    // Cleanup
    // Memory originally allocated should still be valid; clean up now if not freed by the API
    if (Icc.TagPtrs[1] != NULL) {
        std::free(Icc.TagPtrs[1]);
        Icc.TagPtrs[1] = NULL;
    }
    cleanup_test_Icc(Icc);
}

// TEST 3: When TagPtrs[i] is NULL, the function should be a no-op for that index.
static void test_DeleteTagByPos_NoOpWhenPtrNull()
{
    _cmsICCPROFILE Icc;
    init_test_Icc(Icc, 4);

    // Ensure pointer is NULL and some flag is set to a known value
    Icc.TagPtrs[2] = NULL;
    Icc.TagSaveAsRaw[2] = TRUE;
    // TagTypeHandlers[2] can be NULL

    _cmsDeleteTagByPos(&Icc, 2);

    // Validate: TagPtrs[2] remains NULL and TagSaveAsRaw untouched
    ASSERT_TRUE(Icc.TagPtrs[2] == NULL, "TagPtrs[2] should remain NULL after no-op");
    ASSERT_TRUE(Icc.TagSaveAsRaw[2] == TRUE, "TagSaveAsRaw[2] should remain unchanged after no-op");

    cleanup_test_Icc(Icc);
}

// TEST 4: When TagPtrs[i] is non-NULL but TypeHandler is NULL, the function should take no action on that tag.
static void test_DeleteTagByPos_NoActionIfTypeHandlerNull()
{
    _cmsICCPROFILE Icc;
    init_test_Icc(Icc, 4);

    void* ptr = std::malloc(40);
    Icc.TagPtrs[3] = ptr;
    Icc.TagSaveAsRaw[3] = FALSE;
    Icc.TagTypeHandlers[3] = NULL; // No type handler

    _cmsDeleteTagByPos(&Icc, 3);

    // Validate that memory pointer is unchanged and no crash occurred
    ASSERT_TRUE(Icc.TagPtrs[3] == ptr, "TagPtrs[3] should remain non-NULL when TypeHandler is NULL");
    cleanup_test_Icc(Icc);
    std::free(ptr);
}

int main()
{
    std::cout << "Running cmsDeleteTagByPos unit tests (non-GTest)..." << std::endl;

    test_DeleteTagByPos_FreesViaTypeHandlerAndNullsPtr();
    test_DeleteTagByPos_FreeRawPath_TogglesFlagOnly();
    test_DeleteTagByPos_NoOpWhenPtrNull();
    test_DeleteTagByPos_NoActionIfTypeHandlerNull();

    if (g_TestFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << g_TestFailures << " test(s) failed." << std::endl;
        return 1;
    }
}