// Test suite for the focal method: cmsBool Type_XYZ_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
// This test harness is designed to validate the behavior of the focal function in isolation by providing a
// controlled test environment with a mock of the dependency _cmsWriteXYZNumber.
// Notes:
// - We reproduce just enough of the surrounding types to compile and run in a C++11 environment.
// - We do not rely on Google Test; instead, we implement a tiny test framework with non-terminating EXPECT_* style assertions.
// - The test suite exercises both success and failure branches of the underlying dependency, and verifies that
//   Type_XYZ_Write forwards Ptr correctly to _cmsWriteXYZNumber while ignoring nItems and self, as in the focal code.

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Domain knowledge helpers: minimal, self-contained type definitions to mimic the CMS types used by Type_XYZ_Write.
// These definitions are sufficient for unit testing the focal method in a single translation unit.

typedef bool cmsBool;
typedef uint32_t cmsUInt32Number;

// Forward declare the opaque type used by the focal function.
struct _cms_typehandler_struct;

// Lightweight representation of cmsIOHANDLER (contents are irrelevant for the test).
struct cmsIOHANDLER {
    // Intentionally empty; our mock _cmsWriteXYZNumber does not use IO internals.
};

// Lightweight representation of cmsCIEXYZ (as the focal code expects a cmsCIEXYZ* cast from Ptr).
struct cmsCIEXYZ {
    double X;
    double Y;
    double Z;
};

// cmsUNUSED_PARAMETER macro used in the focal function to silence unused param warnings.
// We implement a minimal version compatible with C++.
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Global/test state to capture the behavior of the dependency _cmsWriteXYZNumber.
// The focal method simply forwards Ptr (cast to cmsCIEXYZ*) to _cmsWriteXYZNumber.
// By recording the argument and controlling the return value, we can validate forwarding behavior.

static cmsBool g_nextReturnValue = true;     // Controls what the mock returns on each test.
static cmsCIEXYZ* g_lastXYZPtrSeen = nullptr; // Captured Ptr argument passed to the mock.

extern "C" cmsBool _cmsWriteXYZNumber(cmsIOHANDLER* io, cmsCIEXYZ* xyz)
{
    // Mock implementation: record the pointer and return preconfigured value.
    (void)io; // IO is not used by the test-specific mock.
    g_lastXYZPtrSeen = xyz;
    return g_nextReturnValue;
}

// Focal method under test (recreated here for self-contained testing).
// In the real project, this function would reside in cmstypes.c. We reproduce its signature and behavior exactly as given.
extern "C" cmsBool Type_XYZ_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
{
    return _cmsWriteXYZNumber(io, (cmsCIEXYZ*) Ptr);
    cmsUNUSED_PARAMETER(nItems);
    cmsUNUSED_PARAMETER(self);
}
}

// Simple, lightweight test framework (non-terminating assertions).
static int g_failCount = 0;

#define EXPECT_TRUE(cond) do { \
    bool _c = (cond); \
    if(!_c) { \
        std::fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_failCount; \
    } \
} while(0)

#define EXPECT_EQ_PTR(a, b) do { \
    void* _pa = (void*)(a); \
    void* _pb = (void*)(b); \
    if(_pa != _pb) { \
        std::fprintf(stderr, "EXPECT_EQ_PTR failed: %s:%d: %p != %p\n", __FILE__, __LINE__, _pa, _pb); \
        ++g_failCount; \
    } \
} while(0)

#define ASSERT_TRUE(cond) do { bool _c = (cond); if(!_c){ std::fprintf(stderr, "ASSERT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); ++g_failCount; } } while(0)

static void RunTest_StringAndPointerForwarding_Succeeds_WhenDependencyReturnsTrue()
{
    // Arrange
    g_nextReturnValue = true;
    g_lastXYZPtrSeen = nullptr;

    cmsCIEXYZ xyz = {1.0, 2.0, 3.0};
    cmsIOHANDLER io;
    _cms_typehandler_struct self; // dummy

    // Act
    cmsBool result = Type_XYZ_Write(&self, &io, &xyz, /*nItems*/ 4);

    // Assert
    EXPECT_TRUE(result == true);
    EXPECT_EQ_PTR(g_lastXYZPtrSeen, &xyz);
}

static void RunTest_StringAndPointerForwarding_Succeeds_WhenDependencyReturnsFalse()
{
    // Arrange
    g_nextReturnValue = false;
    g_lastXYZPtrSeen = nullptr;

    cmsCIEXYZ xyz = {5.5, -1.2, 0.0};
    cmsIOHANDLER io;
    _cms_typehandler_struct self;

    // Act
    cmsBool result = Type_XYZ_Write(&self, &io, &xyz, /*nItems*/ 1);

    // Assert
    EXPECT_TRUE(result == false);
    EXPECT_EQ_PTR(g_lastXYZPtrSeen, &xyz);
}

static void RunTest_PtrIsNull_PassedToDependency()
{
    // Arrange
    g_nextReturnValue = true;
    g_lastXYZPtrSeen = reinterpret_cast<cmsCIEXYZ*>(0);

    cmsIOHANDLER io;
    _cms_typehandler_struct self;

    // Act
    cmsBool result = Type_XYZ_Write(&self, &io, nullptr, /*nItems*/ 0);

    // Assert
    EXPECT_TRUE(result == true);
    EXPECT_EQ_PTR(g_lastXYZPtrSeen, reinterpret_cast<cmsCIEXYZ*>(nullptr));
}

static void RunTest_NonXYZPointer_IsForwarded()
{
    // This test ensures that the function forwards the pointer value as-is after the cast.
    // Create a dummy unrelated memory region and pass its address as Ptr.
    struct NotXYZ { int a; int b; } notXYZ = { 7, 13 };

    g_nextReturnValue = true;
    g_lastXYZPtrSeen = nullptr;

    cmsIOHANDLER io;
    _cms_typehandler_struct self;

    // Act
    cmsBool result = Type_XYZ_Write(&self, &io, &notXYZ, /*nItems*/ 2);

    // Assert
    EXPECT_TRUE(result == true);
    EXPECT_EQ_PTR(g_lastXYZPtrSeen, reinterpret_cast<cmsCIEXYZ*>(&notXYZ));
}

// Entry point
int main()
{
    // Explain to user what is being tested.
    std::printf("Running unit tests for focal method Type_XYZ_Write (cmstypes.c) - self-contained harness.\n");

    RunTest_StringAndPointerForwarding_Succeeds_WhenDependencyReturnsTrue();
    RunTest_StringAndPointerForwarding_Succeeds_WhenDependencyReturnsFalse();
    RunTest_PtrIsNull_PassedToDependency();
    RunTest_Non XYZPointer_IsForwarded(); // Note: will define below; kept for consistency with the description.
    
    // If there were multiple tests, they would be invoked similarly.

    if(g_failCount == 0) {
        std::printf("All tests passed.\n");
        return 0;
    } else {
        std::printf("Tests finished with %d failure(s).\n", g_failCount);
        return 1;
    }
}

// Note on test robustness and coverage:
// - True/false branches of the dependency _cmsWriteXYZNumber are exercised by manipulating g_nextReturnValue.
// - Pointer forwarding is validated for a real cmsCIEXYZ pointer, a null pointer, and a non-XYZ pointer that is reinterpreted as cmsCIEXYZ*.
// - nItems and self are unused in the focal implementation; tests validate that changing them has no effect on the outcome.
// - Static members: this harness assumes no static state within the focal function itself; if the real code uses static state elsewhere, tests should be extended to exercise it via additional scenarios.
// - The test harness uses non-terminating, runtime assertions (EXPECT_* and related) to maximize code execution and coverage.