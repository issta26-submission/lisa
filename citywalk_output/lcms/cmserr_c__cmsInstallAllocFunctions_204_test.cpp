// C++11 test suite for _cmsInstallAllocFunctions in cmserr.c
// - No GTest. A lightweight test harness is implemented.
// - Tests cover NULL Plugin path, full Plugin override, partial NULL overrides, and all-NULL Plugin case.
// - Uses extern "C" interfaces to interact with the C implementation.
// - Explanatory comments accompany each test.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>


// Import the relevant CMS/LCMS internal interfaces.
// Adjust include path as needed by your build environment.

extern "C" {
    // Prototypes for the function under test and necessary globals from the C source.
    void _cmsInstallAllocFunctions(cmsPluginMemHandler* Plugin, _cmsMemPluginChunkType* ptr);

    // The default memory plugin chunk available in the library
    extern _cmsMemPluginChunkType _cmsMemPluginChunk;

    // Function pointers used by the test to build a Plugin with various behaviors.
    void* _cmsMallocZeroDefaultFn(cmsContext ContextID, cmsUInt32Number size);
    void* _cmsDupDefaultFn(cmsContext ContextID, const void* Org, cmsUInt32Number size);
    void* _cmsCallocDefaultFn(cmsContext ContextID, cmsUInt32Number num, cmsUInt32Number size);
    void* _cmsMallocDefaultFn(cmsContext ContextID, cmsUInt32Number size);

    // Realloc/Free can be declared if needed for completeness (not strictly required by tests below)
    void* _cmsReallocDefaultFn(cmsContext ContextID, void* Ptr, cmsUInt32Number size);
    void _cmsFreeDefaultFn(cmsContext ContextID, void* Ptr);
}

// Helper: Lightweight test harness
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        printf("TEST FAIL: %s:%d: Expected true: %s\n", __FILE__, __LINE__, #cond); \
    } \
} while(0)

#define EXPECT_EQ_PTR(a, b) do { \
    ++g_totalTests; \
    if((void*)(a) != (void*)(b)) { \
        ++g_failedTests; \
        printf("TEST FAIL: %s:%d: Expected pointer equality: %p == %p\n", __FILE__, __LINE__, (void*)(a), (void*)(b)); \
    } \
} while(0)

#define EXPECT_EQ_BYTES(a, b, n) do { \
    ++g_totalTests; \
    if(std::memcmp((const void*)(a), (const void*)(b), (size_t)(n)) != 0) { \
        ++g_failedTests; \
        printf("TEST FAIL: %s:%d: Memory blocks differ (size=%zu)\n", __FILE__, __LINE__, (size_t)(n)); \
    } \
} while(0)


// Test 1: Plugin == NULL => ptr should be memcpy'ed from _cmsMemPluginChunk
void test_install_alloc_functions_with_null_plugin() {
    // Arrange
    _cmsMemPluginChunkType dest;
    // Ensure destination is not trivially equal before test
    std::memset(&dest, 0xAA, sizeof(dest));

    // Act
    _cmsInstallAllocFunctions(nullptr, &dest);

    // Assert: dest should now be a byte-for-byte copy of _cmsMemPluginChunk
    EXPECT_EQ_BYTES(&dest, &_cmsMemPluginChunk, sizeof(_cmsMemPluginChunkType));
}

// Helpers for function pointer semantics used in Plugin paths
// Distinct dummy implementations to verify pointer propagation.
static void* pluginMalloc(cmsContext, cmsUInt32Number) { return (void*)0x1001; }
static void  pluginFreeFunc(cmsContext, void*) { /* no-op */ }
static void* pluginReallocFunc(cmsContext, void*, cmsUInt32Number) { return (void*)0x2002; }
static void* pluginMallocZeroFunc(cmsContext, cmsUInt32Number) { return (void*)0x3003; }
static void* pluginCallocFunc(cmsContext, cmsUInt32Number num, cmsUInt32Number size) { return (void*)0x4004; }
static void* pluginDupFunc(cmsContext, const void*, cmsUInt32Number) { return (void*)0x5005; }

// Test 2: Plugin non-NULL with all pointers non-NULL => dest fields should reflect Plugin pointers
void test_install_alloc_functions_with_all_plugin_pointers() {
    cmsPluginMemHandler Plugin;
    // Populate Plugin with non-NULL function pointers
    Plugin.MallocPtr  = pluginMalloc;
    Plugin.FreePtr    = pluginFreeFunc;
    Plugin.ReallocPtr = pluginReallocFunc;
    Plugin.MallocZeroPtr = pluginMallocZeroFunc;
    Plugin.CallocPtr   = pluginCallocFunc;
    Plugin.DupPtr      = pluginDupFunc;

    _cmsMemPluginChunkType dest;

    _cmsInstallAllocFunctions(&Plugin, &dest);

    // Validate direct pointer propagation
    EXPECT_TRUE(dest.MallocPtr  == pluginMalloc);
    EXPECT_TRUE(dest.FreePtr    == pluginFreeFunc);
    EXPECT_TRUE(dest.ReallocPtr == pluginReallocFunc);
    // Since Plugin ptrs are non-NULL, defaults should be overridden
    EXPECT_TRUE(dest.MallocZeroPtr == pluginMallocZeroFunc);
    EXPECT_TRUE(dest.CallocPtr     == pluginCallocFunc);
    EXPECT_TRUE(dest.DupPtr        == pluginDupFunc);
}

// Test 3: Plugin non-NULL with some NULL pointers => defaults used for NULL fields
void test_install_alloc_functions_with_partial_null_pointers() {
    cmsPluginMemHandler Plugin;
    // Mix of NULL and non-NULLs
    Plugin.MallocPtr  = pluginMalloc;
    Plugin.FreePtr    = nullptr;
    Plugin.ReallocPtr = nullptr;
    Plugin.MallocZeroPtr = nullptr; // should not override default
    Plugin.CallocPtr   = pluginCallocFunc; // non-NULL, should override
    Plugin.DupPtr      = nullptr;

    _cmsMemPluginChunkType dest;
    _cmsInstallAllocFunctions(&Plugin, &dest);

    // Assertions
    EXPECT_EQ_PTR(dest.MallocPtr, pluginMalloc);
    EXPECT_EQ_PTR(dest.FreePtr, nullptr);
    EXPECT_EQ_PTR(dest.ReallocPtr, nullptr);

    // Defaults for NULL-specified fields
    EXPECT_TRUE(dest.MallocZeroPtr == _cmsMallocZeroDefaultFn);
    EXPECT_TRUE(dest.DupPtr == _cmsDupDefaultFn);
    EXPECT_TRUE(dest.CallocPtr == pluginCallocFunc);
}

// Test 4: Plugin entirely NULL fields again to ensure all defaults are used
void test_install_alloc_functions_with_all_null_plugin_fields() {
    cmsPluginMemHandler Plugin;
    // All NULL
    Plugin.MallocPtr  = nullptr;
    Plugin.FreePtr    = nullptr;
    Plugin.ReallocPtr = nullptr;
    Plugin.MallocZeroPtr = nullptr;
    Plugin.CallocPtr   = nullptr;
    Plugin.DupPtr      = nullptr;

    _cmsMemPluginChunkType dest;
    _cmsInstallAllocFunctions(&Plugin, &dest);

    // All should be defaults for the non-pointer fields as well
    EXPECT_TRUE(dest.MallocPtr  == nullptr);
    EXPECT_TRUE(dest.FreePtr    == nullptr);
    EXPECT_TRUE(dest.ReallocPtr == nullptr);
    EXPECT_TRUE(dest.MallocZeroPtr == _cmsMallocZeroDefaultFn);
    EXPECT_TRUE(dest.CallocPtr     == _cmsCallocDefaultFn);
    EXPECT_TRUE(dest.DupPtr        == _cmsDupDefaultFn);
}

// Entry point
int main() {
    printf("Starting _cmsInstallAllocFunctions test suite (no GTest).\n");

    test_install_alloc_functions_with_null_plugin();
    test_install_alloc_functions_with_all_plugin_pointers();
    test_install_alloc_functions_with_partial_null_pointers();
    test_install_alloc_functions_with_all_null_plugin_fields();

    printf("Test results: total=%d, failed=%d\n", g_totalTests, g_failedTests);
    if (g_failedTests) {
        printf("Some tests failed. Please review the logs above.\n");
    } else {
        printf("All tests passed.\n");
    }

    return (g_failedTests == 0) ? 0 : 1;
}