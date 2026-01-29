// Unit tests for the focal method: Type_UInt8_Read (in cmstypes.c)
// This test suite uses a minimal manual test harness (no GTest) suitable for C++11.
// It mocks only the necessary dependencies (_cmsCalloc, _cmsFree, _cmsReadUInt8Number)
// by providing C-compatible definitions in this test file. The goal is to exercise
// the true/false branches of allocation success/failure and read-success/read-failure.

#include <cstring>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain-adaptation: provide minimal type aliases compatible with cmstypes.c
typedef unsigned int cmsUInt32Number;
typedef unsigned char cmsUInt8Number;
typedef int cmsBool;
typedef void* cmsContext;
typedef void cmsIOHANDLER;

// Forward declaration of the focal type to match the signature of Type_UInt8_Read.
// We only rely on ContextID being accessible by the focal function; the exact
// layout is assumed to be compatible for the ContextID at the offset used by
// the actual library (typically the first member).
struct _cms_typehandler_struct {
    cmsContext ContextID;
};

// Declaration of the focal method (from cmstypes.c). We mirror the signature
// with the known parameter types to enable linking with the real implementation.
extern "C" void* Type_UInt8_Read(struct _cms_typehandler_struct* self,
                                cmsIOHANDLER* io,
                                cmsUInt32Number* nItems,
                                cmsUInt32Number SizeOfTag);

// Global test state to control mocks
static cmsUInt8Number testBuffer[512];
static int testBufferLen = 0;
static int readIndex = 0;

static int forceAllocFail = 0;      // when non-zero, _cmsCalloc will fail
static int readFailAtIndex = -1;      // if >=0, _cmsReadUInt8Number will fail on this read index
static int freeCallCount = 0;         // how many times _cmsFree was invoked

// Mocked external C-compatible hooks expected by Type_UInt8_Read
extern "C" {

// Simulated allocator that library code will call
void* _cmsCalloc(cmsContext ContextID, cmsUInt32Number n, cmsUInt32Number size)
{
    (void)ContextID; // ContextID is not relevant for test
    if (forceAllocFail) return NULL;
    if (n == 0 || size == 0) {
        // Match typical calloc behavior: may return NULL; keep simple
        return NULL;
    }
    return calloc(n, size);
}

// Simulated deallocator called by Type_UInt8_Read on failure
void _cmsFree(cmsContext ContextID, void* Ptr)
{
    (void)ContextID;
    freeCallCount++;
    free(Ptr);
}

// Simulated single-byte reader used by the focal method to read each element.
// We bypass any real io mechanism and drive reads from testBuffer.
 cmsBool _cmsReadUInt8Number(cmsIOHANDLER* io, cmsUInt8Number* Number)
{
    (void)io;
    if (readIndex >= testBufferLen) return 0;       // no more data
    if (readFailAtIndex >= 0 && readIndex == readFailAtIndex) return 0; // simulate failure at this index
    *Number = testBuffer[readIndex++];
    return 1;
}
} // extern "C"

// Lightweight assertion helper: non-terminating, prints result, and continues.
// This aligns with the instruction to use non-terminating assertions to maximize coverage.
static void expect(bool cond, const char* msg)
{
    if (cond) {
        printf("[OK] %s\n", msg);
    } else {
        printf("[ERR] %s\n", msg);
    }
}

// Test 1: Normal path - all reads succeed, ensure values are correctly returned and item count matches SizeOfTag
static void test_UInt8Read_NormalPath()
{
    printf("Running test_UInt8Read_NormalPath...\n");

    // Prepare test buffer and state
    testBufferLen = 5;
    testBuffer[0] = 10;
    testBuffer[1] = 20;
    testBuffer[2] = 30;
    testBuffer[3] = 40;
    testBuffer[4] = 50;
    readIndex = 0;
    forceAllocFail = 0;
    readFailAtIndex = -1;
    freeCallCount = 0;

    struct _cms_typehandler_struct self;
    self.ContextID = 0; // context id is ignored by mocks

    cmsUInt32Number nItems = 0;
    void* result = Type_UInt8_Read(&self, nullptr, &nItems, 5);

    expect(result != NULL, "NormalPath: returned non-NULL pointer");
    expect(nItems == 5, "NormalPath: nItems == 5");
    unsigned char* arr = static_cast<unsigned char*>(result);
    bool contents_ok = (arr[0] == 10 && arr[1] == 20 && arr[2] == 30 && arr[3] == 40 && arr[4] == 50);
    expect(contents_ok, "NormalPath: content matches input data");

    // Cleanup
    if (result) free(result);
    printf("Finished test_UInt8Read_NormalPath.\n\n");
}

// Test 2: Allocation failure path - simulate _cmsCalloc returning NULL
static void test_UInt8Read_AllocFailure()
{
    printf("Running test_UInt8Read_AllocFailure...\n");

    // Prepare test state to force allocation failure
    testBufferLen = 10;
    for (int i = 0; i < 10; ++i) testBuffer[i] = (cmsUInt8Number)(i + 1);
    readIndex = 0;
    forceAllocFail = 1;      // force _cmsCalloc to return NULL
    readFailAtIndex = -1;
    freeCallCount = 0;

    struct _cms_typehandler_struct self;
    self.ContextID = 9999;     // context id chosen to illustrate that it's ignored by mocks

    cmsUInt32Number nItems = 0;
    void* result = Type_UInt8_Read(&self, nullptr, &nItems, 5);

    expect(result == NULL, "AllocFailure: Type_UInt8_Read returns NULL on allocation failure");
    expect(nItems == 0, "AllocFailure: nItems remains 0 on allocation failure");

    printf("Finished test_UInt8Read_AllocFailure.\n\n");
}

// Test 3: Read failure path - simulate failure during one of the reads; ensure memory is freed and NULL is returned
static void test_UInt8Read_ReadFailure()
{
    printf("Running test_UInt8Read_ReadFailure...\n");

    // Prepare test state: ensure allocation would succeed
    testBufferLen = 5;
    testBuffer[0] = 7;
    testBuffer[1] = 8;
    testBuffer[2] = 9;
    testBuffer[3] = 10;
    testBuffer[4] = 11;
    readIndex = 0;
    forceAllocFail = 0;
    readFailAtIndex = 2; // fail on the 3rd read (index 2)
    freeCallCount = 0;

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    cmsUInt32Number nItems = 0;
    void* result = Type_UInt8_Read(&self, nullptr, &nItems, 5);

    expect(result == NULL, "ReadFailure: Type_UInt8_Read returns NULL when a read fails");
    // Ensure that Free was invoked to release allocated memory
    expect(freeCallCount > 0, "ReadFailure: memory was freed on read failure");

    printf("Finished test_UInt8Read_ReadFailure.\n\n");
}

int main()
{
    // Run the focused unit tests for Type_UInt8_Read
    test_UInt8Read_NormalPath();
    test_UInt8Read_AllocFailure();
    test_UInt8Read_ReadFailure();

    // Summary
    printf("Unit test suite for Type_UInt8_Read completed.\n");
    return 0;
}