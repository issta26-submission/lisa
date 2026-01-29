// Unit test suite for the focal method: Type_ParametricCurve_Write
// Target: C++11 compatible test harness (no GTest). Tests are designed to
// run against the real LittleCMS structures exposed by the public headers.
// The tests cover true/false branches, output path behavior, and basic
// parameter encoding paths for single-segment parametric curves.

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstring>


// Include the public LittleCMS header to obtain the correct type definitions.
// If your environment uses a different include path, adjust accordingly.

extern "C" {

// Forward declaration of the focal function (C linkage to ensure correct binding)
cmsBool Type_ParametricCurve_Write(struct _cms_typehandler_struct* self,
                                  cmsIOHANDLER* io,
                                  void* Ptr,
                                  cmsUInt32Number nItems);

}

// Simple test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;
static int g_write_calls = 0;

// Lightweight assertion macro to accumulate failures without aborting tests
#define TEST_ASSERT(cond, msg)                                  \
    do {                                                         \
        ++g_total_tests;                                         \
        if (!(cond)) {                                           \
            ++g_failed_tests;                                    \
            std::cerr << "[FAIL] " << msg << "\n";             \
        }                                                        \
    } while(false)

// IO mock: Write function that always succeeds and counts how many times it is invoked.
// The actual signature of the Write callback may vary between lib versions.
// We provide a compatible function signature based on the typical cmsIOHANDLER layout.
// This function is intentionally permissive to focus on control-flow in Type_ParametricCurve_Write.
static cmsBool ioWrite_Succeed(struct _cms_iohandler_struct* io, void* Buffer, cmsUInt32Number Count)
{
    (void)io;
    (void)Buffer;
    (void)Count;
    ++g_write_calls;
    return 1;
}

// IO mock: Write function that fails (first write attempt causes the function to fail).
static cmsBool ioWrite_Fail(struct _cms_iohandler_struct* io, void* Buffer, cmsUInt32Number Count)
{
    (void)io;
    (void)Buffer;
    (void)Count;
    return 0;
}

// Helper to initialize a simple cmsIOHANDLER with a success-writing callback
static void InitIoHandlerSuccess(cmsIOHANDLER* io)
{
    // Zero initialize to avoid any incidental behavior
    std::memset(io, 0, sizeof(cmsIOHANDLER));
    // The actual field name for the Write callback depends on the version;
    // we attempt to assign the common signature used by LittleCMS public API.
    // If your environment uses a different member name, adapt accordingly.
    io->Write = reinterpret_cast<cmsBool (*)(struct _cms_iohandler_struct*, void*, cmsUInt32Number)>(ioWrite_Succeed);
    io->Read  = nullptr; // Not used in ParametricCurve_Write path
    // ContextID defaults to 0 (neutral context)
    io->ContextID = 0;
}

// Helper to initialize a cmsToneCurve as a single-segment parametric curve
static void InitSingleSegmentCurve(cmsToneCurve* Curve, int type, const cmsS15Fixed16Number* params, int nParams)
{
    std::memset(Curve, 0, sizeof(cmsToneCurve));
    Curve->nSegments = 1;
    Curve->Segments[0].Type = static_cast<cmsUInt16Number>(type);
    // Copy the provided parameters
    for (int i = 0; i < nParams && i < 8; ++i) {
        Curve->Segments[0].Params[i] = params ? params[i] : 0;
    }
    // Remaining Params[] are left as zero as initialized
}

// Test 1: Success path for a valid single-segment parametric curve (Type in [1..5], nSegments == 1)
// Expect: Type_ParametricCurve_Write returns TRUE and writes 2 + nParams bytes (writes counted)
static bool test_ParametricCurve_Write_Success_SingleSegment()
{
    cmsToneCurve Curve;
    cmsS15Fixed16Number dummyParams[8];
    // Use Type 1 which uses nParams = 1 from ParamsByType
    // Fill Params[0] with a valid fixed-point number (e.g., 1.0 in 15.16)
    dummyParams[0] = static_cast<cmsS15Fixed16Number>(0x00010000); // 1.0
    InitSingleSegmentCurve(&Curve, 1, dummyParams, 1);

    cmsIOHANDLER io;
    InitIoHandlerSuccess(&io);

    g_write_calls = 0;
    // The function under test expects a self pointer of type struct _cms_typehandler_struct*
    // Acquire a minimal, compatible handler from the public API header
    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    cmsBool result = Type_ParametricCurve_Write(&self, &io, &Curve, 1);

    TEST_ASSERT(result == 1, "ParametricCurve_Write should succeed for valid single-segment curve");
    TEST_ASSERT(g_write_calls == 3, "Expected 3 write calls: 2 header writes + 1 param for Type=1");
    return (g_failed_tests == 0);
}

// Test 2: Failure path when multiple segments are present (nSegments > 1)
// Expect: FALSE and appropriate error signaling (no crash)
static bool test_ParametricCurve_Write_MultiSegment_Fails()
{
    cmsToneCurve Curve;
    cmsS15Fixed16Number dummyParams[8];
    dummyParams[0] = static_cast<cmsS15Fixed16Number>(0x00010000);

    // Force multi-segment
    InitSingleSegmentCurve(&Curve, 1, dummyParams, 1);
    Curve.nSegments = 2; // override to simulate multi-segment

    cmsIOHANDLER io;
    InitIoHandlerSuccess(&io);

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    g_write_calls = 0;
    cmsBool result = Type_ParametricCurve_Write(&self, &io, &Curve, 1);

    TEST_ASSERT(result == 0, "ParametricCurve_Write should fail for multi-segment curves");
    // No need to inspect write count in this path (should fail before writes)
    return (g_failed_tests == 0);
}

// Test 3: Failure path for an unsupported parametric type (typen > 5)
// Expect: FALSE
static bool test_ParametricCurve_Write_UnsupportedType_Fails()
{
    cmsToneCurve Curve;
    cmsS15Fixed16Number dummyParams[8];
    dummyParams[0] = static_cast<cmsS15Fixed16Number>(0x00010000);
    // Use an invalid type value 6 (typen > 5 triggers FALSE)
    InitSingleSegmentCurve(&Curve, 6, dummyParams, 1);

    cmsIOHANDLER io;
    InitIoHandlerSuccess(&io);

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    g_write_calls = 0;
    cmsBool result = Type_ParametricCurve_Write(&self, &io, &Curve, 1);

    TEST_ASSERT(result == 0, "ParametricCurve_Write should fail for unsupported type (>5)");
    // Ensure no write calls occurred in the invalid path
    TEST_ASSERT(g_write_calls == 0, "No writes should occur when type is unsupported");
    return (g_failed_tests == 0);
}

// Test 4: Failure path for an inverted parametric curve (type < 1)
// Expect: FALSE
static bool test_ParametricCurve_Write_InvertedType_Fails()
{
    cmsToneCurve Curve;
    cmsS15Fixed16Number dummyParams[8];
    dummyParams[0] = static_cast<cmsS15Fixed16Number>(0x00010000);
    InitSingleSegmentCurve(&Curve, 0, dummyParams, 1); // Type = 0 => invalid

    cmsIOHANDLER io;
    InitIoHandlerSuccess(&io);

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    g_write_calls = 0;
    cmsBool result = Type_ParametricCurve_Write(&self, &io, &Curve, 1);

    TEST_ASSERT(result == 0, "ParametricCurve_Write should fail for inverted type (<1)");
    TEST_ASSERT(g_write_calls == 0, "No writes should occur when type is inverted");
    return (g_failed_tests == 0);
}

// Test 5: Failure path when the IO write operation itself fails (io callback returns FALSE)
// This simulates a write error during the essential parametric data encoding
static bool test_ParametricCurve_Write_IOFailure_RaisesFalse()
{
    cmsToneCurve Curve;
    cmsS15Fixed16Number dummyParams[8];
    dummyParams[0] = static_cast<cmsS15Fixed16Number>(0x00010000);

    InitSingleSegmentCurve(&Curve, 1, dummyParams, 1);

    // Use a failing IO handler: its Write callback returns FALSE
    cmsIOHANDLER io;
    // Initialize with zero and then set fail callback
    std::memset(&io, 0, sizeof(io));
    io.ContextID = 0;
    io.Write = reinterpret_cast<cmsBool (*)(struct _cms_iohandler_struct*, void*, cmsUInt32Number)>(ioWrite_Fail);
    io.Read  = nullptr;

    struct _cms_typehandler_struct self;
    self.ContextID = 0;

    g_write_calls = 0;
    cmsBool result = Type_ParametricCurve_Write(&self, &io, &Curve, 1);

    TEST_ASSERT(result == 0, "ParametricCurve_Write should fail if IO Write callback fails");
    // Even if the callback fails on the first call, ensure we didn't crash and the test captured the failure
    return (g_failed_tests == 0);
}

} // extern "C"

int main()
{
    std::cout << "Starting tests for Type_ParametricCurve_Write\n";

    // Run tests (each test prints its own internal diagnostics via TEST_ASSERT)
    bool ok1 = test_ParametricCurve_Write_Success_SingleSegment();
    bool ok2 = test_ParametricCurve_Write_MultiSegment_Fails();
    bool ok3 = test_ParametricCurve_Write_UnsupportedType_Fails();
    bool ok4 = test_ParametricCurve_Write_InvertedType_Fails();
    bool ok5 = test_ParametricCurve_Write_IOFailure_RaisesFalse();

    // Aggregate results
    int failures = g_failed_tests;
    int total = g_total_tests;

    std::cout << "Tests run: " << total << ", Failures: " << failures << "\n";

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed. See details above.\n";
        return 1;
    }
}