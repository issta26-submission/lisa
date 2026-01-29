// Test suite for UnrollLabDoubleToFloat in cmspack.c
// Notes:
// - This test relies on Little CMS internal types and macros (e.g., _cmsTRANSFORM, cmsFloat32Number, cmsFloat64Number, TYPE_RGB_8, PLANAR flags, PixelSize, T_PLANAR, T_EXTRA).
// - GTest is not used. A lightweight, non-terminating assertion mechanism is implemented to maximize test execution.
// - The tests cover both branches of the focal function: planar and non-planar inputs.
// - The tests are written to be compiled with a C++11 compiler in the same project that provides cms types and the UnrollLabDoubleToFloat function.
#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Include CMS internal definitions to access types/macros used by the focal method.
// Depending on your build setup, ensure this header resolves correctly in test env.

// Declare the focal function (C linkage) to be able to call it from C++ test code.
extern "C" cmsUInt8Number* UnrollLabDoubleToFloat(_cmsTRANSFORM* info,
                                                cmsFloat32Number wIn[],
                                                cmsUInt8Number* accum,
                                                cmsUInt32Number Stride);

// Lightweight non-terminating assertion helpers
static int g_testFailures = 0;

#define TEST_FAIL(msg) \
    do { \
        std::cerr << "[TEST FAILURE] " << msg << std::endl; \
        ++g_testFailures; \
    } while (0)

#define ASSERT_NEAR(actual, expected, tol, msg) \
    do { \
        double a = static_cast<double>(actual); \
        double e = static_cast<double>(expected); \
        if (std::fabs(a - e) > (tol)) { \
            std::cerr << "[ASSERT] " << msg << " | Expected: " << expected << " Actual: " << actual \
                      << " (diff=" << (std::fabs(a - e)) << ", tol=" << tol << ")" << std::endl; \
            ++g_testFailures; \
        } \
    } while (0)

#define ASSERT_PTR_EQ(p, q, tol, msg) \
    do { \
        if (std::abs(reinterpret_cast<std::uintptr_t>(p) - reinterpret_cast<std::uintptr_t>(q)) > (tol)) { \
            std::cerr << "[ASSERT] " << msg << " | Ptr1: " << static_cast<void*>(p) \
                      << " | Ptr2: " << static_cast<void*>(q) << std::endl; \
            ++g_testFailures; \
        } \
    } while (0)

// Helper to print a summary at the end
static void printSummary(const char* title) {
    std::cout << title << " - Failures: " << g_testFailures << std::endl;
}

// Test 1: Planar branch of UnrollLabDoubleToFloat
// Setup:
// - info.InputFormat is a planar format (TYPE_RGB_8 with PLANAR flag assumed).
// - Stride is chosen to yield Stride_after = 1 after division by PixelSize to access Pt[0], Pt[1], Pt[2].
// - Pt[0], Pt[1], Pt[2] are used according to the planar branch.
// Expected:
// - wIn[0] = Pt[0] / 100.0
// - wIn[1] = (Pt[Stride] + 128) / 255.0  where Stride was adjusted to 1
// - wIn[2] = (Pt[Stride*2] + 128) / 255.0
static void test_planar_branch()
{
    // Build a _cmsTRANSFORM with planar input format.
    _cmsTRANSFORM info;
    std::memset(&info, 0, sizeof(info));

    // Use TYPE_RGB_8 with PLANAR flag if available in this CMS version.
    // If PLANAR is not defined in the test environment, this line may need adjustment
    // to use a planar input format supported by the installed CMS headers.
#if defined(TYPE_RGB_8) && defined(PLANAR)
    info.InputFormat = TYPE_RGB_8 | PLANAR;
#else
    // Fallback: if the planar flag is not named PLANAR in this environment,
    // try an alternative planar-like macro (some versions expose TYPE_RGB_8_PLANAR).
    #if defined(TYPE_RGB_8_PLANAR)
    info.InputFormat = TYPE_RGB_8_PLANAR;
    #else
    // If neither planar flag is available, skip this test gracefully.
    std::cout << "[TEST] Planar branch skipped: planar format macro not defined in this CMS environment." << std::endl;
    return;
    #endif
#endif

    // Stride must be a multiple of PixelSize(info.InputFormat) to yield Stride_after = 1.
    cmsUInt32Number pixelSize = PixelSize(info.InputFormat);
    cmsUInt32Number Stride_in = pixelSize * 1; // choose 1 unit of pixelSize
    cmsFloat64Number PtBuffer[3]; // Pt[0], Pt[1], Pt[2]
    // Initialize Pt values to deterministic numbers
    PtBuffer[0] = 50.0;   // for wIn[0]
    PtBuffer[1] = -100.0; // for wIn[1] (Pt[Stride])
    PtBuffer[2] = 40.0;   // for wIn[2] (Pt[Stride*2])

    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(PtBuffer); // start memory with doubles that Pt points to
    cmsFloat32Number wIn[3];
    cmsUInt8Number* ret = UnrollLabDoubleToFloat(&info, wIn, accum, Stride_in);

    // Expected accumulator pointer: accum + sizeof(cmsFloat64Number)
    cmsUInt8Number* expected = accum + sizeof(cmsFloat64Number);

    // Validate pointer advancement
    ASSERT_PTR_EQ(ret, expected, 0, "Planar: return pointer should advance by sizeof(double)");

    // Validate outputs
    ASSERT_NEAR(static_cast<double>(wIn[0]), PtBuffer[0] / 100.0, 1e-6, "Planar: wIn[0] mismatch");
    double expected_wIn1 = (PtBuffer[1] + 128.0) / 255.0;
    ASSERT_NEAR(static_cast<double>(wIn[1]), expected_wIn1, 1e-6, "Planar: wIn[1] mismatch");
    double expected_wIn2 = (PtBuffer[2] + 128.0) / 255.0;
    ASSERT_NEAR(static_cast<double>(wIn[2]), expected_wIn2, 1e-6, "Planar: wIn[2] mismatch");

    // If execution reaches here, planar test passed for the checked values.
}

// Test 2: Non-planar branch of UnrollLabDoubleToFloat
// Setup:
// - info.InputFormat is a non-planar (chunky) format (TYPE_RGB_8 without PLANAR flag).
// - Pt[0], Pt[1], Pt[2] provide the data for wIn[0], wIn[1], wIn[2] in non-planar path.
// - Stride is not used in the non-planar path for computation, but passed to function.
// Expected:
// - wIn[0] = Pt[0] / 100.0
// - wIn[1] = (Pt[1] + 128) / 255.0
// - wIn[2] = (Pt[2] + 128) / 255.0
// - accum advanced by sizeof(double) * (3 + T_EXTRA(info->InputFormat))
static void test_non_planar_branch()
{
    _cmsTRANSFORM info;
    std::memset(&info, 0, sizeof(info));

#if defined(TYPE_RGB_8)
    // Ensure we avoid planar flag
    info.InputFormat = TYPE_RGB_8;
#else
    // If TYPE_RGB_8 is not defined, skip gracefully.
    std::cout << "[TEST] Non-planar branch skipped: TYPE_RGB_8 not defined in this CMS environment." << std::endl;
    return;
#endif

    // Pt[0], Pt[1], Pt[2] for wIn[0], wIn[1], wIn[2]
    double PtBuffer[3];
    PtBuffer[0] = 80.0;   // for wIn[0]
    PtBuffer[1] = -64.0;  // for wIn[1]
    PtBuffer[2] = 0.0;    // for wIn[2]

    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(PtBuffer);
    cmsFloat32Number wIn[3];
    cmsUInt32Number Stride_in = 1; // arbitrary; not used in non-planar path except for initial Stride pre-division

    cmsUInt8Number* ret = UnrollLabDoubleToFloat(&info, wIn, accum, Stride_in);

    // Expected accumulator pointer
    cmsUInt8Number* expected = accum + sizeof(cmsFloat64Number) * (3 + T_EXTRA(info.InputFormat));

    // Validate pointer advancement
    ASSERT_PTR_EQ(ret, expected, 0, "Non-planar: return pointer should advance by sizeof(double) * (3 + T_EXTRA(...))");

    // Validate outputs
    ASSERT_NEAR(static_cast<double>(wIn[0]), PtBuffer[0] / 100.0, 1e-6, "Non-planar: wIn[0] mismatch");
    double expected_wIn1 = (PtBuffer[1] + 128.0) / 255.0;
    ASSERT_NEAR(static_cast<double>(wIn[1]), expected_wIn1, 1e-6, "Non-planar: wIn[1] mismatch");
    double expected_wIn2 = (PtBuffer[2] + 128.0) / 255.0;
    ASSERT_NEAR(static_cast<double>(wIn[2]), expected_wIn2, 1e-6, "Non-planar: wIn[2] mismatch");

    // If execution reaches here, non-planar test passed for the checked values.
}

int main()
{
    std::cout << "Running UnrollLabDoubleToFloat unit tests (CMS cmspack.c) ...\n";

    // Run tests
    test_planar_branch();
    test_non_planar_branch();

    printSummary("UnrollLabDoubleToFloat test summary");

    if (g_testFailures == 0) {
        std::cout << "All tests passed (non-terminating assertions executed).\n";
        return 0;
    } else {
        std::cout << "Some tests failed. Failures: " << g_testFailures << "\n";
        return 1;
    }
}