// Unit tests for PackLabDoubleFrom16 in cmspack.c
// This test suite targets the focal method PackLabDoubleFrom16 and its behavior
// under both possible branches of the OutputFormat predicate (planar vs chunky).
// The tests rely on the actual LCMS headers and implementation available in the
// environment (e.g., lcms2_internal.h and associated cmsLabEncoded2Float, etc.).
// Note: This test uses a lightweight, self-contained test harness (no Google Test).

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Include the LCMS internal headers to access required types/macros
// (Assumes the project provides these headers with the usual LCMS definitions).

// Minimal test harness
static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg)                               \
    do {                                                     \
        if (!(cond)) {                                       \
            std::cerr << "EXPECT_TRUE failed: " << (msg)    \
                      << " at " << __FILE__ << ":" << __LINE__ \
                      << std::endl;                          \
            ++g_test_failures;                               \
        }                                                    \
    } while (0)

#define EXPECT_NEAR(a, b, tol, msg)                          \
    do {                                                       \
        if (std::fabs((a) - (b)) > (tol)) {                   \
            std::cerr << "EXPECT_NEAR failed: " << (msg)      \
                      << " |a-b|=" << std::fabs((a)-(b))    \
                      << " at " << __FILE__ << ":" << __LINE__  \
                      << std::endl;                           \
            ++g_test_failures;                                 \
        }                                                      \
    } while (0)

static bool nearly_equal_double(double a, double b, double tol = 1e-6)
{
    return std::fabs(a - b) <= tol;
}

// Test 1: Planar branch (T_PLANAR(info->OutputFormat) evaluates to true)
// We verify that:
// - The returned pointer equals output + sizeof(cmsFloat64Number).
// - The first three doubles in the output buffer correspond to Lab.L, Lab.a, Lab.b
//   decoded from the provided 16-bit Lab-encoded triplet wOut[0..2].
// - The stride handling works as expected with Stride = 1 (simplifies checks).
static void test_PackLabDoubleFrom16_Planar()
{
    // Prepare a small 16-bit Lab encoding to decode into L, a, b
    cmsUInt16Number wOut[3] = { 1000, 2000, 3000 };

    // Decode to know what to expect
    cmsCIELab LabDecoded;
    cmsLabEncoded2Float(&LabDecoded, wOut);
    // LabDecoded.L, LabDecoded.a, LabDecoded.b now hold expected values

    // Create a _cmsTRANSFORM-like info instance with planar OutputFormat
    _cmsTRANSFORM infoPlanar;
    // Initialize to zero then set OutputFormat to a value that (in this environment)
    // is known to trigger the planar branch. We rely on the environment constants.
    // If in your environment a different value is required, swap 0 -> PLANAR_FORMAT
    // where PLANAR_FORMAT equals the planar flag. Here we assume 0 is planar.
    std::memset(&infoPlanar, 0, sizeof(infoPlanar));
    infoPlanar.OutputFormat = 0; // Assumed planar in this environment

    // Output buffer: 3 doubles expected when Stride = 1
    const cmsUInt32Number Stride = 1;
    cmsUInt8Number* outputBuf = static_cast<cmsUInt8Number*>(std::malloc(3 * sizeof(cmsFloat64Number)));
    std::memset(outputBuf, 0, 3 * sizeof(cmsFloat64Number));

    // Call the function
    cmsUInt8Number* ret = PackLabDoubleFrom16(&infoPlanar, wOut, outputBuf, Stride);

    // Assertions
    // 1) Returned pointer should be input pointer plus sizeof(cmsFloat64Number)
    EXPECT_TRUE(ret == outputBuf + sizeof(cmsFloat64Number),
                "Planar: return pointer should be output + sizeof(cmsFloat64Number)");

    // 2) Output values should equal decoded Lab values in order: L, a, b
    cmsFloat64Number* Out = reinterpret_cast<cmsFloat64Number*>(outputBuf);
    EXPECT_NEAR((double)Out[0], (double)LabDecoded.L, 1e-6,
                "Planar: L channel mismatch after PackLabDoubleFrom16");
    EXPECT_NEAR((double)Out[Stride], (double)LabDecoded.a, 1e-6,
                "Planar: a channel mismatch after PackLabDoubleFrom16");
    EXPECT_NEAR((double)Out[Stride * 2], (double)LabDecoded.b, 1e-6,
                "Planar: b channel mismatch after PackLabDoubleFrom16");

    std::free(outputBuf);
}

// Test 2: Chunky/planar-agnostic branch (T_PLANAR(info->OutputFormat) evaluates to false)
// We verify that:
// - The returned pointer equals output + (sizeof(cmsCIELab) + T_EXTRA(OutputFormat) * sizeof(cmsFloat64Number)).
// - The initial cmsCIELab portion of the output buffer matches LabDecoded (L, a, b).
static void test_PackLabDoubleFrom16_Chunky()
{
    // Prepare the same 16-bit Lab encoding
    cmsUInt16Number wOut[3] = { 1500, 2500, 3500 };

    // Decode to know expected Lab values
    cmsCIELab LabDecoded;
    cmsLabEncoded2Float(&LabDecoded, wOut);

    // Create a _cmsTRANSFORM-like info instance with non-planar (chunky) OutputFormat
    _cmsTRANSFORM infoChunky;
    std::memset(&infoChunky, 0, sizeof(infoChunky));
    // Set to a value that triggers the chunky path; assume 1 is non-planar in this environment
    infoChunky.OutputFormat = 1; // Assumed chunky in this environment

    // Output buffer large enough for cmsCIELab plus any extra space
    size_t extra = static_cast<size_t>(T_EXTRA(infoChunky.OutputFormat)) * sizeof(cmsFloat64Number);
    cmsUInt8Number* outputBuf = static_cast<cmsUInt8Number*>(std::malloc(sizeof(cmsCIELab) + extra));
    std::memset(outputBuf, 0, sizeof(cmsCIELab) + extra);

    // Call the function
    cmsUInt8Number* ret = PackLabDoubleFrom16(&infoChunky, wOut, outputBuf, 1);

    // Assertions
    // 1) Returned pointer should be output + (sizeof(cmsCIELab) + T_EXTRA(OutputFormat) * sizeof(cmsFloat64Number))
    cmsUInt8Number* expectedRet = outputBuf + (sizeof(cmsCIELab) + extra);
    EXPECT_TRUE(ret == expectedRet,
                "Chunky: return pointer should be output + (sizeof(cmsCIELab) + T_EXTRA(OutputFormat) * sizeof(cmsFloat64Number))");

    // 2) The cmsCIELab at the beginning should reflect LabDecoded values
    cmsCIELab* labOut = reinterpret_cast<cmsCIELab*>(outputBuf);
    EXPECT_NEAR(labOut->L, LabDecoded.L, 1e-6,
                "Chunky: L value mismatch after PackLabDoubleFrom16");
    EXPECT_NEAR(labOut->a, LabDecoded.a, 1e-6,
                "Chunky: a value mismatch after PackLabDoubleFrom16");
    EXPECT_NEAR(labOut->b, LabDecoded.b, 1e-6,
                "Chunky: b value mismatch after PackLabDoubleFrom16");

    std::free(outputBuf);
}

int main()
{
    std::cout << "Running PackLabDoubleFrom16 unit tests..." << std::endl;

    test_PackLabDoubleFrom16_Planar();
    test_PackLabDoubleFrom16_Chunky();

    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
}