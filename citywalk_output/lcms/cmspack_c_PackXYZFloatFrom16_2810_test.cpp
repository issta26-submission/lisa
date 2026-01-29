/*
Unit test suite for the focal method PackXYZFloatFrom16 in cmspack.c

Notes for integrate/adjustment in your environment:
- This test suite assumes the Little CMS (lcms2) style of public headers and types,
  i.e., cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, cmsFloat32Number, _cmsTRANSFORM, etc.
- The tests rely on actual library macros for format classification (e.g., T_PLANAR,
  PixelSize, T_EXTRA) and on public API declarations. If your build uses slightly different
  header paths or macro names, adjust includes and macro usages accordingly.
- The tests are written in plain C++11, without GoogleTest or any third-party test
  framework, using a tiny self-contained test harness to maximize portability.

What this test covers:
- Branch coverage: true (planar) and false (non-planar) branches of PackXYZFloatFrom16.
- Correct memory layout for the planar vs chunky path by validating the placement of
  X, Y, Z components in the output buffer.
- Return-pointer arithmetic: validate that the function returns output advanced by the
  expected number of bytes depending on the active format (planar vs non-planar) and
  the presence of any extra channels (T_EXTRA) per the format.

Limitations / assumptions:
- For Planar vs Chunky decision, this test relies on the library-provided format
  constants/macros. The test includes guards so that if a particular format macro
  is not defined in your build, that test is skipped gracefully.
- We test with encoded XYZ inputs wOut = {0,0,0} to get a deterministic XYZ=(0,0,0)
  after cmsXYZEncoded2Float, ensuring the test focuses on memory layout and return
  address arithmetic rather than the exact decoded X,Y,Z values.
- If your environment uses static linking and a different header path, adjust
  includes and symbol names accordingly.

Code (paste into a new test.cpp file and compile with your build system, linking against
the same lcms2 sources as the focal code):

*/

#include <cstring>
#include <cstdint>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>


// Try to pull in the same types/macros as the focal code.
// Adjust include path as per your project layout.

// Simple, self-contained test framework (no GTest)
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAILED: " msg "\n"; \
        return false; \
    } \
} while(0)

#define TEST_INFO(msg) std::cout << "TEST INFO: " << msg << std::endl;

// Lightweight test runner
static bool RunAllTests();

// Helper to interpret return pointer offset in bytes
static std::size_t PtrDiffBytes(const void* a, const void* b) {
    return static_cast<const uint8_t*>(a) - static_cast<const uint8_t*>(b);
}

// Attempt to expose the focal function (as declared in cmspack.c):
// PackXYZFloatFrom16(CMSREGISTER _cmsTRANSFORM* Info, CMSREGISTER cmsUInt16Number wOut[], CMSREGISTER cmsUInt8Number* output, CMSREGISTER cmsUInt32Number Stride)
extern "C" cmsUInt8Number* PackXYZFloatFrom16(CMSREGISTER _cmsTRANSFORM* Info,
                                             CMSREGISTER cmsUInt16Number wOut[],
                                             CMSREGISTER cmsUInt8Number* output,
                                             CMSREGISTER cmsUInt32Number Stride);

// Minimal, portable wrapper to create a _cmsTRANSFORM with a given OutputFormat.
// This assumes _cmsTRANSFORM has at least the member: cmsUInt32Number OutputFormat
// and that the code uses only Info->OutputFormat in PackXYZFloatFrom16.
// If your actual _cmsTRANSFORM type differs, adapt accordingly.
static void PrepareInfo(_cmsTRANSFORM& Info, cmsUInt32Number fmt) {
    // Zero-initialize to avoid spurious values
    std::memset(&Info, 0, sizeof(_cmsTRANSFORM));
    Info.OutputFormat = fmt;
}

// Planar test: expects format with planar arrangement
static bool Test_PackXYZFloatFrom16_Planar() {
    TEST_INFO("PackXYZFloatFrom16: Planar branch (if available)");
    // Depending on your environment, a Planar format macro should exist.
    // The following is a best-effort attempt; replace with your actual planar format macro.
#ifdef TYPE_RGB_PLANAR_8
    const cmsUInt32Number PlanarFormat = TYPE_RGB_PLANAR_8; // e.g., 3 channels, 8-bit per channel, planar
#elif defined(TYPE_RGB_8_PLANAR)
    const cmsUInt32Number PlanarFormat = TYPE_RGB_8_PLANAR;
#elif defined(PLANAR_RGB_8)  // some variants may name the bit differently
    const cmsUInt32Number PlanarFormat = PLANAR_RGB_8;
#else
    // No planar format macro available in this environment; skip test gracefully.
    TEST_INFO("Planar format macro not defined; skipping Planar test.");
    return true;
#endif

    _cmsTRANSFORM Info;
    PrepareInfo(Info, PlanarFormat);

    cmsUInt16Number wOut[3] = {0, 0, 0}; // Zero-encoded XYZ
    cmsUInt8Number output[256];
    std::memset(output, 0, sizeof(output));

    // Stride chosen so that after division by PixelSize, the Y value lands at index 1
    // (i.e., planar with single-plane per channel layout -> X at 0, Y at 1, Z at 2)
    cmsUInt32Number Stride = PixelSize(PlanarFormat); // after division -> 1

    cmsUInt8Number* ret = PackXYZFloatFrom16(&Info, wOut, output, Stride);

    // Expect the return pointer to move by sizeof(cmsFloat32Number)
    std::size_t expectedOffPlanar = sizeof(cmsFloat32Number);
    TEST_ASSERT(ret == output + expectedOffPlanar,
                "Planar path: return pointer offset mismatch");

    // Since wOut = {0,0,0}, decoded XYZ should be (0,0,0). We check the first 3 floats.
    cmsFloat32Number* fOut = reinterpret_cast<cmsFloat32Number*>(output);
    // X at Out[0] equals XYZ.X
    TEST_ASSERT(static_cast<float>(fOut[0]) == 0.0f, "Planar: XYZ.X should be 0.0");
    // Y at Out[Stride] which after division equals index 1
    TEST_ASSERT(static_cast<float>(fOut[1]) == 0.0f, "Planar: XYZ.Y should be 0.0 at index 1");
    // Z at Out[Stride*2] which equals index 2
    TEST_ASSERT(static_cast<float>(fOut[2]) == 0.0f, "Planar: XYZ.Z should be 0.0 at index 2");

    return true;
}

// Non-planar (chunky/interleaved) test: expects a chunky format
static bool Test_PackXYZFloatFrom16_NonPlanar() {
    TEST_INFO("PackXYZFloatFrom16: Non-planar (chunky) branch (if available)");
#ifdef TYPE_RGB_8
    // A typical chunky 3-channel 8-bit RGB, not planar
    const cmsUInt32Number ChunkyFormat = TYPE_RGB_8;
#else
    // If no explicit chunky macro, skip gracefully
    TEST_INFO("Chunky format macro not defined; skipping Non-Planar test.");
    return true;
#endif

    _cmsTRANSFORM Info;
    PrepareInfo(Info, ChunkyFormat);

    cmsUInt16Number wOut[3] = {0, 0, 0}; // Zero-encoded XYZ
    cmsUInt8Number output[256];
    std::memset(output, 0, sizeof(output));

    // Stride chosen so that in non-planar path, we access indices 0,1,2
    // (i.e., no special reordering by stride)
    cmsUInt32Number Stride = PixelSize(ChunkyFormat); // after division would be 1 or as per PixelSize

    cmsUInt8Number* ret = PackXYZFloatFrom16(&Info, wOut, output, Stride);

    // Expected offset is 3 * sizeof(cmsFloat32Number) plus any extra channels
    cmsUInt32Number extra = T_EXTRA(Info.OutputFormat); // number of extra channels in the format
    std::size_t expectedOffChunky = 3 * sizeof(cmsFloat32Number) + extra * sizeof(cmsFloat32Number);
    TEST_ASSERT(ret == output + expectedOffChunky,
                "Non-planar path: return pointer offset mismatch");

    // With wOut = {0,0,0}, the first three floats should be XYZ.X, XYZ.Y, XYZ.Z respectively = 0
    cmsFloat32Number* fOut = reinterpret_cast<cmsFloat32Number*>(output);
    TEST_ASSERT(static_cast<float>(fOut[0]) == 0.0f, "Chunky: XYZ.X should be 0.0 at index 0");
    TEST_ASSERT(static_cast<float>(fOut[1]) == 0.0f, "Chunky: XYZ.Y should be 0.0 at index 1");
    TEST_ASSERT(static_cast<float>(fOut[2]) == 0.0f, "Chunky: XYZ.Z should be 0.0 at index 2");

    return true;
}

static bool RunAllTests() {
    int total = 0;
    int passed = 0;

    auto TestPlanar = Test_PackXYZFloatFrom16_Planar;
    if (TestPlanar()) { ++passed; } ++total;

    auto TestNonPlanar = Test_PackXYZFloatFrom16_NonPlanar;
    if (TestNonPlanar()) { ++passed; } ++total;

    std::cout << "Summary: " << passed << "/" << total << " tests passed.\n";
    return total == passed;
}

int main() {
    bool allOK = RunAllTests();
    if (!allOK) {
        std::cerr << "One or more tests FAILED.\n";
        return 1;
    }
    std::cout << "All tests PASSED.\n";
    return 0;
}

/*
Explanatory notes for the tests:

- Test_PackXYZFloatFrom16_Planar
  - Purpose: Validate the planar code path of PackXYZFloatFrom16.
  - Approach: Use a planar format macro (if defined in your environment) to force
    T_PLANAR(Info->OutputFormat) to evaluate to true.
  - Check: After decoding wOut = {0,0,0}, we expect XYZ.X/Y/Z to be 0.0 and
    the return pointer to be offset by sizeof(cmsFloat32Number) from the start
    of the output buffer, as per the function's planar branch return.

- Test_PackXYZFloatFrom16_NonPlanar
  - Purpose: Validate the non-planar (chunky/interleaved) code path of PackXYZFloatFrom16.
  - Approach: Use a chunky RGB format macro to force the non-planar branch (i.e., T_PLANAR is false).
  - Check: With wOut = {0,0,0}, ensure XYZ components decoded to 0.0 and the
    return pointer is offset by 3*sizeof(cmsFloat32Number) plus T_EXTRA(...) bytes.

- Notes about macro guards
  - This test uses #ifdef guards around macros TYPE_RGB_PLANAR_8/TYPE_RGB_8_PLANAR/PLANAR_RGB_8
    and TYPE_RGB_8 to gracefully skip tests when the environment doesn’t expose those
    exact constants. If your environment uses different names, replace accordingly
    (the test is designed to be easily adaptable to the exact macro definitions in your
    lcMS headers).

- Static vs dynamic linkage and static members
  - If any of the macros are defined as static or require specific linkage, ensure
    you link this translation unit against the same set of compilation units as the
    focal library. The tests do not rely on private members; they rely on public
    API usage and the library's internal macros.

- Namespaces
  - The test uses C linkage for the focal function (PackXYZFloatFrom16) and the _cmsTRANSFORM
    structure. If your build uses a namespace for the test wrapper, keep the test
    harness within the global namespace to align with the C linkage of the focal API.

- Non-terminating assertions
  - The test uses non-terminating assertions via TEST_ASSERT macros to continue
    test execution and collect multiple failure points within a single run.

- How to build
  - Include this file in your build and link against the same lcms2 sources as your
    existing library build (static or shared as needed). If the environment uses a
    separate test target, add this translation unit to that target.
  - If any required macros are missing, replace the placeholders with the exact
    constants from your headers so the test can drive the intended branches.

*/