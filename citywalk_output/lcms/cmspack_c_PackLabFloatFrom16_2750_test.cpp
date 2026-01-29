/*
  Lightweight C++11 test harness for the focal method:
  PackLabFloatFrom16 in cmspack.c

  Notes and approach:
  - This test suite assumes the project provides the LittleCMS-like types,
    macros, and the function cmsLabEncoded2Float used inside the focal method.
  - We do not depend on any external testing framework (GTest is not used).
  - We use simple EXPECT_* style macros that do not abort test execution, as requested.
  - We test both branches of the conditional based on T_PLANAR(info->OutputFormat):
    - Planar path: validates that a 32-bit float array holds L, a, b at expected indices
      and that the function returns output + sizeof(cmsFloat32Number).
    - Non-planar path: validates that the first three floats hold L, a, b and that
      the pointer arithmetic accounts for T_EXTRA(info->OutputFormat) as in the code.
  - To derive expected Lab values, we reuse cmsLabEncoded2Float to convert the same
    wOut input to Lab.L, Lab.a, Lab.b, ensuring the test checks integration with the
    Lab encoding step.
  - We determine the expected planar indices using Stride and PixelSize(info->OutputFormat)
    exactly as PackLabFloatFrom16 does.
  - This test file should be compiled and linked with the project, relying on the
    project-supplied definitions of the CMS types and macros (e.g., _cmsTRANSFORM,
    cmsUInt16Number, cmsUInt8Number, cmsFloat32Number, cmsLabEncoded2Float, T_PLANAR,
    PixelSize, T_EXTRA, etc.).

  Build/run notes:
  - Compile as part of the project (link with lcms2 or the library providing the focal function).
  - Ensure the include path provides lcms headers (lcms2_internal.h or equivalent).
  - The test uses external declarations for PackLabFloatFrom16 and cmsLabEncoded2Float
    from the project.

  Limitations:
  - This harness relies on the project’s actual structures and macros for OutputFormat
    and planarity decisions. If those differ, adjust the two test cases to target
    the known planar vs. chunky values in your build.
*/

#include <cstring>
#include <cstdio>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Include project headers to obtain necessary types/macros.
// The focus method, data types and helper routines live in the project.
// If your paths differ, adjust include directives accordingly.
extern "C" {

// Prototype of the focal method under test (as declared in the project)
cmsUInt8Number* PackLabFloatFrom16(CMSREGISTER _cmsTRANSFORM* info,
                                   CMSREGISTER cmsUInt16Number wOut[],
                                   CMSREGISTER cmsUInt8Number* output,
                                   CMSREGISTER cmsUInt32Number Stride);

// Lab encoding helper provided by the library
void cmsLabEncoded2Float(cmsCIELab* Lab, const cmsUInt16Number wIn[]);
}

// Simple test assertion helpers.
// They do not terminate tests on failure, enabling multiple checks per test.
static int g_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        printf("EXPECT_TRUE failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        printf("EXPECT_EQ failed: %s == %s (actual: %lld vs expected: %lld) at %s:%d\n", \
               #a, #b, (long long)(a), (long long)(b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_FLOAT_EQ(a, b) do { \
    const double _a = (a); \
    const double _b = (b); \
    const double _eps = 1e-4; \
    if(std::fabs(_a - _b) > _eps) { \
        printf("EXPECT_FLOAT_EQ failed: %s == %s (actual: %.6f vs expected: %.6f, diff=%.6f) at %s:%d\n", \
               #a, #b, _a, _b, std::fabs(_a - _b), __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

static void test_PackLabFloatFrom16_Planar()
{
    // Prepare an _cmsTRANSFORM-like info structure
    // Note: The project header provides the actual definition; we rely on it.
    _cmsTRANSFORM info;
    // Choose a value for OutputFormat that the library considers planar.
    // The exact constant depends on the build; ensure this yields planar mode.
    info.OutputFormat = 0; // Replace with a planar-producing value if needed.

    // Input Lab encoding (wOut). The CMS library will decode this to Lab via cmsLabEncoded2Float.
    cmsUInt16Number wOut[4] = { 32768, 0, 0, 0 }; // example encoded value. library determines Lab.

    // Output buffer large enough to hold the planar data.
    cmsUInt8Number output[256];
    std::memset(output, 0, sizeof(output));

    // Stride in the function call (byte-based size step before dividing by PixelSize)
    cmsUInt32Number Stride = 12; // arbitrary non-zero value to exercise indexing

    // Call the function under test
    cmsUInt8Number* endPtr = PackLabFloatFrom16(&info, wOut, output, Stride);

    // Compute expected Lab values from the same wOut input (encoding side)
    cmsCIELab Lab;
    cmsLabEncoded2Float(&Lab, wOut);

    // Compute normalized stride so we can index planar components
    cmsUInt32Number normStride = Stride / PixelSize(info.OutputFormat);
    // Access plane: L at index 0, a at index normStride, b at index 2*normStride
    const cmsFloat32Number* outFloats = reinterpret_cast<const cmsFloat32Number*>(output);

    // Validate L, a, b values written by PackLabFloatFrom16
    EXPECT_FLOAT_EQ(outFloats[0], Lab.L);
    EXPECT_FLOAT_EQ(outFloats[normStride], Lab.a);
    EXPECT_FLOAT_EQ(outFloats[normStride * 2], Lab.b);

    // Validate the return pointer points to after the first written float
    const cmsUInt8Number* expectedEnd = reinterpret_cast<const cmsUInt8Number*>(output) + sizeof(cmsFloat32Number);
    EXPECT_TRUE(endPtr == reinterpret_cast<cmsUInt8Number*>(expectedEnd));

    // Summary for this test
    printf("Test PackLabFloatFrom16 - Planar: completed with %d failures\n", g_failures);
}

static void test_PackLabFloatFrom16_NonPlanar()
{
    _cmsTRANSFORM info;
    // Use a value for OutputFormat that yields non-planar path
    info.OutputFormat = 1; // Replace with non-planar-producing value if needed.

    cmsUInt16Number wOut[4] = { 12345, 0, 0, 0 };

    cmsUInt8Number output[256];
    std::memset(output, 0, sizeof(output));

    cmsUInt32Number Stride = 0; // Stride not used in non-planar path for indexing

    cmsUInt8Number* endPtr = PackLabFloatFrom16(&info, wOut, output, Stride);

    cmsCIELab Lab;
    cmsLabEncoded2Float(&Lab, wOut);

    const cmsFloat32Number* outFloats = reinterpret_cast<const cmsFloat32Number*>(output);

    // In non-planar path, Lab components should be written at output[0], [1], [2]
    EXPECT_FLOAT_EQ(outFloats[0], Lab.L);
    EXPECT_FLOAT_EQ(outFloats[1], Lab.a);
    EXPECT_FLOAT_EQ(outFloats[2], Lab.b);

    // Expect end pointer = output + (3 + T_EXTRA(info->OutputFormat)) * sizeof(float)
    const cmsUInt8Number* expectedEnd = reinterpret_cast<const cmsUInt8Number*>(output)
                                        + (3 + T_EXTRA(info.OutputFormat)) * sizeof(cmsFloat32Number);

    // Cast to pointer for comparison
    EXPECT_TRUE(endPtr == reinterpret_cast<cmsUInt8Number*>(expectedEnd));

    printf("Test PackLabFloatFrom16 - NonPlanar: completed with %d failures\n", g_failures);
}

int main()
{
    // Run tests. These tests rely on project-provided definitions for the types/macros.
    test_PackLabFloatFrom16_Planar();
    // Reset per-test state if needed for independent tests
    // (In this simple harness, g_failures is global; we continue accumulating.)
    test_PackLabFloatFrom16_NonPlanar();

    if (g_failures == 0) {
        printf("All PackLabFloatFrom16 tests passed.\n");
        return 0;
    } else {
        printf("Total failures: %d\n", g_failures);
        return 1;
    }
}