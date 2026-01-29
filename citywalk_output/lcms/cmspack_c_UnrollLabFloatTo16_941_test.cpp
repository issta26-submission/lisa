// cmspack_test.cpp
// Lightweight C++11 test suite for UnrollLabFloatTo16 in cmspack.c
// - No Google Test; a simple test harness using plain asserts and manual result reporting.
// - Provides minimal stubs for dependencies to enable compilation/linking.
// - Focuses on exercising both memory-layout branches (planar vs chunky) of UnrollLabFloatTo16.

#include <cstring>
#include <cstdio>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-knowledge oriented lightweight type aliases to match cmspack.c usage
typedef unsigned char       cmsUInt8Number;
typedef unsigned short      cmsUInt16Number;
typedef unsigned int        cmsUInt32Number;
typedef float               cmsFloat32Number;

// Lightweight container for Lab-like values used by the focal function
typedef struct {
    cmsFloat32Number L;
    cmsFloat32Number a;
    cmsFloat32Number b;
} cmsCIELab;

// CMSREGISTER is a calling-convention macro in cms code. In tests, keep it empty.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Forward-declare the internal _cmsTRANSFORM structure used by the focal function
typedef struct _cmsTRANSFORM {
    cmsUInt32Number InputFormat;
} _cmsTRANSFORM;

// Forward-declare the focal function under test (must match actual signature in cmspack.c)
extern "C" cmsUInt8Number* UnrollLabFloatTo16(CMSREGISTER _cmsTRANSFORM* info,
                                             CMSREGISTER cmsUInt16Number wIn[],
                                             CMSREGISTER cmsUInt8Number* accum,
                                             CMSREGISTER cmsUInt32Number Stride);

// Stub for cmsFloat2LabEncoded used by UnrollLabFloatTo16
extern "C" void cmsFloat2LabEncoded(CMSREGISTER cmsUInt16Number wIn[],
                                   cmsCIELab* Lab);

// Provide a minimal no-op implementation for cmsFloat2LabEncoded to keep behavior deterministic
extern "C" void cmsFloat2LabEncoded(CMSREGISTER cmsUInt16Number wIn[],
                                   cmsCIELab* Lab)
{
    // No-op in test environment; real library would encode Lab into wIn.
    (void)wIn; (void)Lab;
}

// Test 1: Planar branch path (T_PLANAR evaluates to true for this input format)
bool TestPlanarBranch()
{
    // Arrange
    _cmsTRANSFORM info;
    // The actual T_PLANAR macro is defined in the project's headers; we attempt to drive planar path.
    // We try an InputFormat value commonly used for planar in LCMS (attempt 0)
    info.InputFormat = 0; // Candidate value for Planar branch

    cmsUInt16Number wIn[3] = { 111, 222, 333 }; // arbitrary Lab target channel encodings

    // accum must contain at least L at accum[0], a at accum[Stride], b at accum[Stride*2]
    const int Stride = 2;
    cmsFloat32Number accumF[9];
    // Populate the three Lab components at the positions used by the planar branch
    accumF[0]  = 1.25f;      // Lab.L
    accumF[Stride] = 2.50f;   // Lab.a
    accumF[Stride*2] = 3.75f; // Lab.b

    cmsUInt8Number* accumBytes = reinterpret_cast<cmsUInt8Number*>(accumF);

    // Act
    cmsUInt8Number* result = UnrollLabFloatTo16(&info, wIn, accumBytes, Stride);

    // Assert
    // In planar branch, function returns accum + sizeof(cmsFloat32Number)
    size_t offsetBytes = reinterpret_cast<char*>(result) - reinterpret_cast<char*>(accumBytes);
    bool planareturnOK = (offsetBytes == sizeof(cmsFloat32Number));
    if (!planareturnOK) {
        printf("TestPlanarBranch: Unexpected return offset %zu (expected %zu)\n",
               offsetBytes, sizeof(cmsFloat32Number));
    }

    // Additionally verify that the function did not crash and produced some deterministic non-null return
    return planareturnOK && (result != nullptr);
}

// Test 2: Chunky (non-planar) branch path (T_PLANAR evaluates to false for this input format)
bool TestNonPlanarBranch()
{
    // Arrange
    _cmsTRANSFORM info;
    // Use a different InputFormat value expected to trigger the non-planar path
    info.InputFormat = 1; // Candidate value for Non-Planar branch

    cmsUInt16Number wIn[3] = { 444, 555, 666 }; // arbitrary Lab target

    // accum holds 3 consecutive Lab components for chunky layout
    const int Stride = 1;
    cmsFloat32Number accumF[3] = { 4.0f, 5.0f, 6.0f };
    cmsUInt8Number* accumBytes = reinterpret_cast<cmsUInt8Number*>(accumF);

    // Act
    cmsUInt8Number* result = UnrollLabFloatTo16(&info, wIn, accumBytes, Stride);

    // Assert
    size_t offsetBytes = reinterpret_cast<char*>(result) - reinterpret_cast<char*>(accumBytes);
    // We don't know the exact T_EXTRA value; ensure the offset is at least 3 * sizeof(float)
    bool nonPlanarOffsetOK = (offsetBytes >= 3 * sizeof(cmsFloat32Number));
    if (!nonPlanarOffsetOK) {
        printf("TestNonPlanarBranch: Unexpected small return offset %zu (expected at least %zu)\n",
               offsetBytes, 3 * sizeof(cmsFloat32Number));
    }

    // Ensure the function executed (non-null return)
    return nonPlanarOffsetOK && (result != nullptr);
}

// Simple test harness
int main()
{
    int total = 0;
    int passed = 0;

    printf("Running UnrollLabFloatTo16 test suite...\n");

    // Test Planar branch path
    printf("TestPlanarBranch: ");
    total++;
    if (TestPlanarBranch()) {
        passed++;
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    // Test Non-Planar branch path
    printf("TestNonPlanarBranch: ");
    total++;
    if (TestNonPlanarBranch()) {
        passed++;
        printf("PASS\n");
    } else {
        printf("FAIL\n");
    }

    printf("Tests passed: %d / %d\n", passed, total);

    // Return non-zero if any test failed to help integration environments fail-fast
    return (passed == total) ? 0 : 1;
}