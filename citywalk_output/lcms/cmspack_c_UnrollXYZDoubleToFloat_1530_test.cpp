// cms_unroll_xyz_double_to_float_test.cpp
// Unit tests for UnrollXYZDoubleToFloat in cmspack.c
// This test suite is designed to be compiled with C++11, without Google Test.
// It relies on internal CMS LittleCMS headers to provide required types/macros.

#include <cstring>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Include internal CMS headers to obtain necessary types, macros and function prototypes.
// The focal function UnrollXYZDoubleToFloat is implemented in cmspack.c and relies on
// internal definitions such as _cmsTRANSFORM, PixelSize, T_PLANAR, MAX_ENCODEABLE_XYZ, etc.

// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;
#define TEST_EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "TEST FAILURE: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++g_test_failures; \
        } \
    } while (0)

#define TEST_EXPECT_NEAR(a, b, eps, msg) \
    do { \
        if (std::fabs((a) - (b)) > (eps)) { \
            std::cerr << "TEST FAILURE: " << (msg) \
                      << " | Got: " << (a) << "  Expected: " << (b) \
                      << "  Diff: " << std::fabs((a)-(b)) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++g_test_failures; \
        } \
    } while (0)

#define TEST_PRINT_SUMMARY() \
    do { \
        if (g_test_failures == 0) { \
            std::cout << "ALL TESTS PASSED\n"; \
        } else { \
            std::cerr << g_test_failures << " TEST(s) FAILED\n"; \
        } \
    } while (0)


// Helper to create an _cmsTRANSFORM object with required InputFormat.
// We rely on cms internal definitions; the struct layout is defined in lcms2_internal.h
// Thus we can create/initialize the object directly.
static void init_transform_with_input_format(_cmsTRANSFORM* info, cmsUInt32Number inputFormat)
{
    // The actual _cmsTRANSFORM structure is complex; we only need to set the InputFormat
    // field for our tests. Zero the structure first to avoid uninitialized memory usage.
    std::memset(info, 0, sizeof(_cmsTRANSFORM));
    info->InputFormat = inputFormat;
}


// Test 1: UnrollXYZDoubleToFloat in planar mode (T_PLANAR == true)
static void test_UnrollXYZDoubleToFloat_PlannarMode()
{
    // Explanatory: verify that in planar mode, the function reads 3 double values
    // from Pt[0], Pt[Stride], Pt[Stride*2] where Stride is adjusted by PixelSize.
    // wIn[0], wIn[1], wIn[2] should be Pt[i] / MAX_ENCODEABLE_XYZ respectively.

    _cmsTRANSFORM info;
    // Set InputFormat such that T_PLANAR(info->InputFormat) evaluates to true.
    // We rely on typical CMS flags where a low nonzero value (e.g., 1) denotes planar.
    init_transform_with_input_format(&info, 1u); // planar

    // Prepare Pt values (Pt is Pt = (cmsFloat64Number*) accum)
    double Pt[3] = { 2.0, 4.0, 6.0 }; // chosen so Pt[i] / MAX_ENCODEABLE_XYZ yields known values
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(Pt);

    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};
    cmsUInt32Number Stride = 3; // initial Stride (will be divided by PixelSize)
    // We expect PixelSize(InputFormat) to yield 3 for 3 channels so Stride becomes 1.
    // The function will access Pt[0], Pt[Stride], Pt[Stride*2] -> Pt[0], Pt[1], Pt[2]

    cmsUInt8Number* ret = UnrollXYZDoubleToFloat(&info, wIn, accum, Stride);

    // Verify that wIn values equal Pt[i] / MAX_ENCODEABLE_XYZ
    const cmsFloat64Number expected0 = Pt[0] / MAX_ENCODEABLE_XYZ;
    const cmsFloat64Number expected1 = Pt[1] / MAX_ENCODEABLE_XYZ;
    const cmsFloat64Number expected2 = Pt[2] / MAX_ENCODEABLE_XYZ;

    TEST_EXPECT_NEAR(static_cast<cmsFloat32Number>(wIn[0]),
                     static_cast<cmsFloat32Number>(expected0),
                     1e-6,
                     "Planar: wIn[0] should equal Pt[0] / MAX_ENCODEABLE_XYZ");
    TEST_EXPECT_NEAR(static_cast<cmsFloat32Number>(wIn[1]),
                     static_cast<cmsFloat32Number>(expected1),
                     1e-6,
                     "Planar: wIn[1] should equal Pt[Stride] / MAX_ENCODEABLE_XYZ (Pt[1])");
    TEST_EXPECT_NEAR(static_cast<cmsFloat32Number>(wIn[2]),
                     static_cast<cmsFloat32Number>(expected2),
                     1e-6,
                     "Planar: wIn[2] should equal Pt[Stride*2] / MAX_ENCODEABLE_XYZ (Pt[2])");

    // Ensure the function returns a non-null pointer and advances the accum buffer
    TEST_EXPECT_TRUE(ret != nullptr, "Planar: return pointer should not be null");
    TEST_EXPECT_TRUE(ret >= accum, "Planar: return pointer should advance from accum");
}


// Test 2: UnrollXYZDoubleToFloat non-planar mode (T_PLANAR == false)
static void test_UnrollXYZDoubleToFloat_NonPlanarMode()
{
    // Explanatory: verify that in non-planar mode, the function reads Pt[0], Pt[1], Pt[2]
    // and advances accum by sizeof(cmsFloat64Number) * (3 + T_EXTRA(info->InputFormat)).
    _cmsTRANSFORM info;
    init_transform_with_input_format(&info, 0u); // non-planar (false)

    double Pt[3] = { 10.0, 20.0, 30.0 };
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(Pt);

    cmsFloat32Number wIn[3] = {0.0f, 0.0f, 0.0f};
    cmsUInt32Number Stride = 5; // arbitrary; non-planar path should not use Stride

    cmsUInt8Number* ret = UnrollXYZDoubleToFloat(&info, wIn, accum, Stride);

    const cmsFloat64Number expected0 = Pt[0] / MAX_ENCODEABLE_XYZ;
    const cmsFloat64Number expected1 = Pt[1] / MAX_ENCODEABLE_XYZ;
    const cmsFloat64Number expected2 = Pt[2] / MAX_ENCODEABLE_XYZ;

    TEST_EXPECT_NEAR(static_cast<cmsFloat32Number>(wIn[0]),
                     static_cast<cmsFloat32Number>(expected0),
                     1e-6,
                     "Non-planar: wIn[0] should equal Pt[0] / MAX_ENCODEABLE_XYZ");
    TEST_EXPECT_NEAR(static_cast<cmsFloat32Number>(wIn[1]),
                     static_cast<cmsFloat32Number>(expected1),
                     1e-6,
                     "Non-planar: wIn[1] should equal Pt[1] / MAX_ENCODEABLE_XYZ");
    TEST_EXPECT_NEAR(static_cast<cmsFloat32Number>(wIn[2]),
                     static_cast<cmsFloat32Number>(expected2),
                     1e-6,
                     "Non-planar: wIn[2] should equal Pt[2] / MAX_ENCODEABLE_XYZ");

    TEST_EXPECT_TRUE(ret != nullptr, "Non-planar: return pointer should not be null");
    TEST_EXPECT_TRUE(ret > accum, "Non-planar: return pointer should advance from accum");
}


int main()
{
    // Run tests
    test_UnrollXYZDoubleToFloat_PlannarMode();   // Planar mode true branch
    test_UnrollXYZDoubleToFloat_NonPlanarMode();  // Planar mode false branch

    // Print summary
    TEST_PRINT_SUMMARY();
    return g_test_failures ? 1 : 0;
}