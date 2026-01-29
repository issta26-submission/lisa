// cmsintrp_float_unit_tests.cpp
// Unit tests for LinLerp1Dfloat in cmsintrp.c using a custom test harness (no GTest).
// The tests target the 1D linear interpolation logic implemented in LinLerp1Dfloat.
// Notes:
// - We rely on cmsFloat32Number, cmsUInt32Number, cmsInterpParams types from the CMS headers.
// - The tests call the function via extern "C" to avoid C++ name mangling.
// - We intentionally avoid terminating assertions; failures are reported and execution continues.
// - The tests cover true/false branches and an interpolation case.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Include CMS internal headers to obtain required types (adjust path if necessary)

// Ensure the function under test is visible with C linkage
extern "C" void LinLerp1Dfloat(const cmsFloat32Number Value[],
                                cmsFloat32Number Output[],
                                const cmsInterpParams* p);

// Simple non-terminating test reporting
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void ReportTest(const char* testName, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "TEST PASSED: " << testName << "\n";
    } else {
        ++g_failed_tests;
        std::cerr << "TEST FAILED: " << testName << "\n";
    }
}

// Floating point comparison with tolerance for interpolation results
static bool AlmostEqual(float a, float b, float tol = 1e-6f) {
    return std::fabs(a - b) <= tol;
}

// Test 1: Domain[0] == 0 should always pick LutTable[0]
static void Test_DomainZero_ReturnsFirstLUTEntry() {
    const char* testName = "DomainZero_ReturnsFirstLUTEntry";

    // Prepare a small LUT
    cmsFloat32Number lut[] = { 10.0f, 20.0f, 30.0f, 40.0f };
    // Prepare an interp params structure as expected by the focal function
    cmsInterpParams p;
    // Initialize required fields
    // Table is a void pointer in the real struct
    p.Table = (void*)lut;
    // Domain[0] controls whether we interpolate or pick a direct LUT entry
    p.Domain[0] = 0; // triggers the special case

    cmsFloat32Number Value[1] = { 0.5f }; // value is irrelevant in this branch
    cmsFloat32Number Output[1];
    LinLerp1Dfloat(Value, Output, &p);

    bool ok = AlmostEqual(Output[0], lut[0]);
    ReportTest(testName, ok);
}

// Test 2: val2 == 1.0 (Value[0] == 1.0) should pick LutTable[Domain[0]]
static void Test_LastValue_UsesLastLUTEntry() {
    const char* testName = "LastValue_UsesDomainEntry";

    cmsFloat32Number lut[] = { 5.0f, 15.0f, 25.0f, 35.0f, 45.0f };
    cmsInterpParams p;
    p.Table = (void*)lut;
    p.Domain[0] = 3; // pick lut[3] when val2 == 1.0

    cmsFloat32Number Value[1] = { 1.0f }; // triggers the "val2 == 1.0" branch
    cmsFloat32Number Output[1];
    LinLerp1Dfloat(Value, Output, &p);

    bool ok = AlmostEqual(Output[0], lut[3]);
    ReportTest(testName, ok);
}

// Test 3: Normal interpolation between LUT entries
// Domain[0] > 0 and Value[0] not 0 or 1 to exercise interpolation
static void Test_InterpolationBetweenLUTEntries() {
    const char* testName = "Interpolation_Between_LUT_Entries";

    cmsFloat32Number lut[] = { 0.0f, 10.0f, 20.0f, 30.0f, 40.0f };
    cmsInterpParams p;
    p.Table = (void*)lut;
    p.Domain[0] = 3; // 3 segments -> indices 0..3

    cmsFloat32Number Value[1] = { 0.5f }; // val2 = 0.5 * 3 = 1.5 => between lut[1] and lut[2]
    cmsFloat32Number Output[1];
    LinLerp1Dfloat(Value, Output, &p);

    // Expected: y0 = lut[1] = 10, y1 = lut[2] = 20, rest = 0.5
    // Output = 10 + (20 - 10) * 0.5 = 15
    float expected = 15.0f;
    bool ok = AlmostEqual(Output[0], (cmsFloat32Number)expected);
    ReportTest(testName, ok);
}

// Test 4: Another interpolation point with fractional rest to verify arithmetic
static void Test_Interpolation_NonIntegerRest() {
    const char* testName = "Interpolation_NonIntegerRest";

    cmsFloat32Number lut[] = { 2.0f, 4.0f, 6.0f, 8.0f };
    cmsInterpParams p;
    p.Table = (void*)lut;
    p.Domain[0] = 3;

    // Choose Value to yield val2 = 0.25 * 3 = 0.75; rest = 0.75, between lut[0] and lut[1]
    // Actually val2 = Value[0] clamped to [0,1], then multiplied by Domain
    // To get rest=0.75 and cell0=0, cell1=1, set Value[0] = 0.25
    cmsFloat32Number Value[1] = { 0.25f };
    cmsFloat32Number Output[1];
    LinLerp1Dfloat(Value, Output, &p);

    // y0 = lut[0] = 2, y1 = lut[1] = 4, rest = val2 - cell0 = (0.25*3) - 0 = 0.75
    // Output = 2 + (4-2)*0.75 = 2 + 2*0.75 = 3.5
    float expected = 3.5f;
    bool ok = AlmostEqual(Output[0], (cmsFloat32Number)expected);
    ReportTest(testName, ok);
}

int main() {
    // Run all tests
    Test_DomainZero_ReturnsFirstLUTEntry();
    Test_LastValue_UsesDomainEntry();
    Test_InterpolationBetweenLUT_Entries();
    Test_Interpolation_NonIntegerRest();

    // Summary
    std::cout << "Total tests: " << g_total_tests << "\n";
    std::cout << "Failed tests: " << g_failed_tests << "\n";

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}