// Self-contained C++11 test suite for the focal method Type_Data_Dup
// This test mocks the minimal cms ICC data structures and memory duplication
// routine to validate the behavior of Type_Data_Dup as described in the focal method.

// Domain assumptions replicated for tests:
// - cmsICCData has a len member and a flexible data region starting at data[0].
// - Type_Data_Dup duplicates memory of size: sizeof(cmsICCData) + BinData->len - 1.
// - _cmsDupMem is called with the ContextID from the handler and the exact length above.
// - The test does not depend on private/internal static members of the real project.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Basic type definitions to mirror the focal environment
typedef unsigned int cmsUInt32Number;
typedef int cmsContext;

// Forward declaration of the type handler structure used by Type_Data_Dup
struct _cms_typehandler_struct;

// Minimal cmsICCData structure matching the expectations used by Type_Data_Dup
typedef struct cmsICCData {
    cmsUInt32Number len;   // length of the payload following the header
    unsigned char data[1];  // first byte of payload (flexible region in memory)
} cmsICCData;

// File-scope macro to silence unused parameter warnings, matching the test's needs
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Global (test-scoped) variable to capture the ContextID passed into _cmsDupMem
static int g_lastContextID = -1;

// Mocked memory duplication function (extern "C" to mimic the real project's linkage)
extern "C" void* _cmsDupMem(int ContextID, const void* Ptr, cmsUInt32Number Len)
{
    g_lastContextID = ContextID;
    if (Len == 0) return nullptr;
    void* dup = std::malloc((size_t)Len);
    if (!dup) return nullptr;
    if (Ptr) {
        std::memcpy(dup, Ptr, (size_t)Len);
    }
    return dup;
}

// The focal method under test (re-implemented here for a self-contained test)
extern "C" void* Type_Data_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n)
{
    // Cast the input to cmsICCData* and duplicate memory accordingly
    cmsICCData* BinData = (cmsICCData*) Ptr;
    return _cmsDupMem(self ->ContextID, Ptr, sizeof(cmsICCData) + BinData ->len - 1);
    cmsUNUSED_PARAMETER(n);
}

// Minimal definition of the type handler structure used by Type_Data_Dup
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Simple test framework (non-terminating assertions)
static int g_failures = 0;
#define EXPECT(cond, desc) \
    do { if(!(cond)) { std::cerr << "EXPECT FAILED: " << (desc) \
        << " in " << __func__ << "();\n"; ++g_failures; } } while(0)

// Helper to create a cmsICCData object with given payload length and pattern
static void* CreateCmsICCDataWithPayload(cmsUInt32Number payloadLen, unsigned char pattern)
{
    // Allocate memory for cmsICCData plus (payloadLen - 1) extra bytes in data[]
    size_t total_size = sizeof(cmsICCData) + (size_t)payloadLen - 1;
    void* mem = std::malloc(total_size);
    if (!mem) return nullptr;
    cmsICCData* obj = (cmsICCData*)mem;
    obj->len = payloadLen;
    // Fill payload bytes (starting at data[0])
    for (cmsUInt32Number i = 0; i < payloadLen; ++i) {
        obj->data[i] = (unsigned char)(pattern + i);
    }
    return mem;
}

// Test 1: Basic scenario with a moderate payload length
static bool test_DataDup_basic()
{
    std::cout << "Running test_DataDup_basic...\n";

    // Prepare handler with a specific context ID
    struct _cms_typehandler_struct handler;
    handler.ContextID = 12345;

    // Create original data: len = 16
    cmsUInt32Number payloadLen = 16;
    void* Ptr = CreateCmsICCDataWithPayload(payloadLen, 0xA0); // pattern: 0xA0, 0xA1, ...
    if (!Ptr) {
        std::cerr << "Failed to allocate test data in test_DataDup_basic.\n";
        ++g_failures;
        return false;
    }

    cmsUInt32Number expectedCopySize = sizeof(cmsICCData) + payloadLen - 1;

    // Reset last context for verification
    g_lastContextID = -1;

    void* Dup = Type_Data_Dup(&handler, Ptr, 0);
    EXPECT(Dup != nullptr, "Duplication should return a non-null pointer");

    // Verify that the ContextID was passed through to the memory copy routine
    EXPECT(g_lastContextID == handler.ContextID, "Type_Data_Dup should pass correct ContextID to _cmsDupMem");

    // Verify that the duplicated memory matches the original memory region
    EXPECT(std::memcmp(Dup, Ptr, (size_t)expectedCopySize) == 0,
           "Duplicated memory should be an exact copy of the source");

    // Verify that a new memory block was allocated (not the same as Ptr)
    EXPECT(Dup != Ptr, "Duplicated memory should be a different block from the source");

    // Cleanup
    std::free(Ptr);
    std::free(Dup);

    return g_failures == 0;
}

// Test 2: Edge case with the smallest payload (len = 1)
static bool test_DataDup_len1()
{
    std::cout << "Running test_DataDup_len1...\n";

    struct _cms_typehandler_struct handler;
    handler.ContextID = 42;

    cmsUInt32Number payloadLen = 1;
    void* Ptr = CreateCmsICCDataWithPayload(payloadLen, 0x11);
    if (!Ptr) {
        std::cerr << "Failed to allocate test data in test_DataDup_len1.\n";
        ++g_failures;
        return false;
    }

    cmsUInt32Number expectedCopySize = sizeof(cmsICCData) + payloadLen - 1;

    g_lastContextID = -1;
    void* Dup = Type_Data_Dup(&handler, Ptr, 0);
    EXPECT(Dup != nullptr, "Duplication should return a non-null pointer for len=1");
    EXPECT(g_lastContextID == handler.ContextID, "ContextID should be passed for len=1");
    EXPECT(std::memcmp(Dup, Ptr, (size_t)expectedCopySize) == 0,
           "Duplicated memory should match source for len=1");
    EXPECT(Dup != Ptr, "Duplicated memory should be a different block for len=1");

    std::free(Ptr);
    std::free(Dup);
    return g_failures == 0;
}

// Test 3: Larger payload with non-sequential pattern to ensure data integrity
static bool test_DataDup_largePayload()
{
    std::cout << "Running test_DataDup_largePayload...\n";

    struct _cms_typehandler_struct handler;
    handler.ContextID = 999;

    cmsUInt32Number payloadLen = 64;
    void* Ptr = CreateCmsICCDataWithPayload(payloadLen, 0x10);
    if (!Ptr) {
        std::cerr << "Failed to allocate test data in test_DataDup_largePayload.\n";
        ++g_failures;
        return false;
    }

    cmsUInt32Number expectedCopySize = sizeof(cmsICCData) + payloadLen - 1;

    g_lastContextID = -1;
    void* Dup = Type_Data_Dup(&handler, Ptr, 0);
    EXPECT(Dup != nullptr, "Duplication should succeed for large payload");
    EXPECT(g_lastContextID == handler.ContextID, "ContextID should be propagated for large payload");
    EXPECT(std::memcmp(Dup, Ptr, (size_t)expectedCopySize) == 0,
           "Duplicated data should match original for large payload");
    EXPECT(Dup != Ptr, "Duplicated memory should be a different block for large payload");

    std::free(Ptr);
    std::free(Dup);
    return g_failures == 0;
}

// Entry point: run all tests and report results
int main()
{
    // Run tests
    test_DataDup_basic();
    test_DataDup_len1();
    test_DataDup_largePayload();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}