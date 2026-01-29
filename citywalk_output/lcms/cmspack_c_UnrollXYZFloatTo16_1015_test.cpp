// Test suite for UnrollXYZFloatTo16 (from cmsPack.c) using a self-contained harness.
// This test is designed to be compiled together with the focal source (cmspack.c)
// by including cmspack.c in this translation unit, while patching basic dependencies
// via simple macros and lightweight stubs. The goal is to exercise the planar and
// non-planar branches of UnrollXYZFloatTo16 and verify correct encoding to wIn
// as well as correct pointer arithmetic on the accum buffer.

// Notes for the tester:
// - This harness uses a minimal, self-contained mock environment to enable compilation
//   without relying on the full original CMS LittleCMS project structure.
// - We override minimal pieces (types, macros, and a small cmsFloat2XYZEncoded) to
//   drive UnrollXYZFloatTo16 deterministically.
// - The approach assumes that you are able to compile this translation unit together
//   with cmspack.c (i.e., by including cmspack.c in this file). The code is designed
//   to be portable to C++11 compilers as a single translation unit.

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <lcms2_internal.h>
#include <cmath>


// User-provided overrides to mirror the FOCAL_METHOD's environment.
#define T_PLANAR(x) ((x) != 0)   // Treat nonzero InputFormat as Planar branch
#define T_EXTRA(x) 0             // Assume no extra bytes for simplicity in tests

// Basic type mappings (stand-ins for CMS types used in the focal function signature)
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef float    cmsFloat32Number;

// Minimal XYZ color container used by the focal code
typedef struct {
    cmsFloat32Number X;
    cmsFloat32Number Y;
    cmsFloat32Number Z;
} cmsCIEXYZ;

// Minimal _cmsTRANSFORM structure with the InputFormat field (as used by the focal method)
typedef struct _cmsTRANSFORM {
    cmsUInt32Number InputFormat;
} _cmsTRANSFORM;

// Forward declaration of the focal function (extern "C" to match the real linkage)
extern "C" cmsUInt8Number* UnrollXYZFloatTo16(_cmsTRANSFORM* info,
                                            cmsUInt16Number wIn[],
                                            cmsUInt8Number* accum,
                                            cmsUInt32Number Stride);

// Minimal encoding function to convert XYZ floating point values into 16-bit words.
// This mock mirrors the expected behavior for deterministic tests.
extern "C" void cmsFloat2XYZEncoded(cmsUInt16Number wIn[], cmsCIEXYZ* XYZ) {
    // Simple linear encoding: clamp to [0,1] then scale to [0,65535]
    auto clamp01 = [](float v) -> float {
        if (v < 0.0f) return 0.0f;
        if (v > 1.0f) return 1.0f;
        return v;
    };
    cmsFloat32Number x = clamp01(XYZ->X);
    cmsFloat32Number y = clamp01(XYZ->Y);
    cmsFloat32Number z = clamp01(XYZ->Z);

    wIn[0] = static_cast<cmsUInt16Number>(static_cast<float>(x) * 65535.0f);
    wIn[1] = static_cast<cmsUInt16Number>(static_cast<float>(y) * 65535.0f);
    wIn[2] = static_cast<cmsUInt16Number>(static_cast<float>(z) * 65535.0f);
}

// Small helper to compare two 16-bit values with a tolerance (exact match here since encoding is deterministic)
static bool eq16(cmsUInt16Number a, cmsUInt16Number b) {
    return a == b;
}

// Lightweight test harness
static int gFailures = 0;
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if (!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " (" << (a) << " != " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_PTR_EQ failed: " #a " (" << (void*)(a) << ") != " #b " (" << (void*)(b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gFailures; \
    } \
} while(0)


// The test cases

// Test 1: Planar path (T_PLANAR true). Verifies:
// - X,Y,Z are read from accum at offsets 0, Stride, 2*Stride
// - cmsFloat2XYZEncoded is invoked with those XYZ values
// - The function returns accum + sizeof(cmsFloat32Number) (i.e., 4 bytes ahead)
static void test_UnrollXYZFloatTo16_PlanarBranch() {
    std::cout << "Running test_UnrollXYZFloatTo16_PlanarBranch...\n";

    _cmsTRANSFORM info;
    // Planar path activated (nonzero)
    info.InputFormat = 1;

    cmsUInt16Number wIn[3] = {0,0,0};

    // accum must contain 3 consecutive floats (X,Y,Z) at offsets 0, Stride, 2*Stride
    // We'll set Stride = 4 (size of a float in bytes) so that Y is accum+4, Z accum+8.
    cmsUInt32Number Stride = 4;

    // Prepare the input XYZ: X at 0, Y at 4, Z at 8 bytes
    cmsFloat32Number accumBuf[3] = {0.25f, 0.50f, 0.75f}; // X,Y,Z values
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accumBuf);

    // Call
    cmsUInt8Number* ret = UnrollXYZFloatTo16(&info, wIn, accum, Stride);

    // Expected encoding from cmsFloat2XYZEncoded
    cmsUInt16Number expX = static_cast<cmsUInt16Number>(0.25f * 65535.0f);
    cmsUInt16Number expY = static_cast<cmsUInt16Number>(0.50f * 65535.0f);
    cmsUInt16Number expZ = static_cast<cmsUInt16Number>(0.75f * 65535.0f);

    EXPECT_TRUE(eq16(wIn[0], expX));
    EXPECT_TRUE(eq16(wIn[1], expY));
    EXPECT_TRUE(eq16(wIn[2], expZ));

    // Return value should be accum + sizeof(float) == 4 bytes
    cmsUInt8Number* expectedRet = accum + sizeof(cmsFloat32Number);
    EXPECT_PTR_EQ(ret, expectedRet);

    // And the input XYZ should remain in accum (no write-back to input buffer in this path)
    EXPECT_EQ(accumBuf[0], 0.25f);
    EXPECT_EQ(accumBuf[1], 0.50f);
    EXPECT_EQ(accumBuf[2], 0.75f);
}

// Test 2: Non-Planar path (T_PLANAR false). Verifies:
// - X,Y,Z are read from accum as Pt[0], Pt[1], Pt[2]
// - cmsFloat2XYZEncoded is invoked with those XYZ values
// - The function advances accum by 3*sizeof(float) + T_EXTRA(InputFormat)*sizeof(float)
//   In our test T_EXTRA is defined as 0, so we expect + 12 bytes
static void test_UnrollXYZFloatTo16_NonPlanarBranch() {
    std::cout << "Running test_UnrollXYZFloatTo16_NonPlanarBranch...\n";

    _cmsTRANSFORM info;
    // Non-planar path activated (zero InputFormat per our T_PLANAR macro)
    info.InputFormat = 0;

    cmsUInt16Number wIn[3] = {0,0,0};

    // accum must contain 3 floats representing X,Y,Z
    cmsFloat32Number accumBuf[3] = {0.10f, 0.20f, 0.30f};
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accumBuf);

    cmsUInt32Number Stride = 8; // value won't affect non-planar path, but keep plausible

    // Call
    cmsUInt8Number* ret = UnrollXYZFloatTo16(&info, wIn, accum, Stride);

    // Expected encoding from cmsFloat2XYZEncoded
    cmsUInt16Number expX = static_cast<cmsUInt16Number>(0.10f * 65535.0f);
    cmsUInt16Number expY = static_cast<cmsUInt16Number>(0.20f * 65535.0f);
    cmsUInt16Number expZ = static_cast<cmsUInt16Number>(0.30f * 65535.0f);

    EXPECT_TRUE(eq16(wIn[0], expX));
    EXPECT_TRUE(eq16(wIn[1], expY));
    EXPECT_TRUE(eq16(wIn[2], expZ));

    // Return value should be accum + 3*sizeof(float) + T_EXTRA(InputFormat)*sizeof(float)
    // With T_EXTRA(0) == 0, this should be accum + 12
    cmsUInt8Number* expectedRet = accum + 3 * sizeof(cmsFloat32Number);
    EXPECT_PTR_EQ(ret, expectedRet);

    // Ensure the input accum values remain unchanged
    EXPECT_EQ(accumBuf[0], 0.10f);
    EXPECT_EQ(accumBuf[1], 0.20f);
    EXPECT_EQ(accumBuf[2], 0.30f);
}

// Main function to run tests
int main() {
    std::cout << "Starting UnrollXYZFloatTo16 unit tests\n";

    test_UnrollXYZFloatTo16_PlanarBranch();
    test_UnrollXYZFloatTo16_NonPlanarBranch();

    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << gFailures << " test(s) FAILED\n";
        return 1;
    }
}

// Note: The above tests assume that the code under test (UnrollXYZFloatTo16) is compiled in
// the same translation unit (e.g., by including cmspack.c) and that the macro
// T_PLANAR and T_EXTRA control branch behavior as shown. If your build system uses separate
// compilation units, consider adapting this harness to link against the real implementation
// and to provide equivalent macro overrides in a header or build configuration.