// Step 0 (overview comments for maintainers):
// This file provides a small, lightweight unit-test harness (no GoogleTest) for the focal method:
//   cmsUInt8Number* PackXYZDoubleFrom16(CMSREGISTER _cmsTRANSFORM* Info,
//                                        CMSREGISTER cmsUInt16Number wOut[],
//
// It aims to cover both branches of the internal if (T_PLANAR(Info->OutputFormat)) and its
// else counterpart by using two tests that exercise the function with zeroed input to validate
// pointers and basic memory side-effects. We rely on the actual LittleCMS types/macros (lcms2.h)
// to drive the behavior (e.g., T_PLANAR, PixelSize, T_EXTRA, cmsCIEXYZ, cmsXYZEncoded2Float).
// The test uses non-terminating expectations (EXPECT_*) to maximize coverage without aborting.
// If a particular macro (e.g., a specific planar/non-planar OutputFormat) is not defined in the
// test environment, the corresponding test is skipped gracefully with a message.

#include <cstring>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Import necessary dependencies with correct paths (assumes the library headers are in include path)
// The focal function is implemented in cmspack.c; we declare it here for linkage.
extern "C" cmsUInt8Number* PackXYZDoubleFrom16(CMSREGISTER _cmsTRANSFORM* Info,
                                              CMSREGISTER cmsUInt16Number wOut[],
                                              CMSREGISTER cmsUInt8Number* output,
                                              CMSREGISTER cmsUInt32Number Stride);

// Simple non-terminating test framework (no GTest). Keeps track of pass/fail counts.

static int gTestsRun = 0;
static int gTestsFailed = 0;

#define EXPECT_TRUE(cond) do {                                \
    ++gTestsRun;                                               \
    if(!(cond)) {                                              \
        ++gTestsFailed;                                        \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << "  EXPECT_TRUE(" << #cond << ") is false." << std::endl; \
    }                                                          \
} while(false)

#define EXPECT_EQ(a,b) do {                                     \
    ++gTestsRun;                                                  \
    if(!((a) == (b))) {                                           \
        ++gTestsFailed;                                           \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__  \
                  << "  EXPECT_EQ(" << (a) << ", " << (b) << ")" << std::endl; \
    }                                                             \
} while(false)

#define EXPECT_DOUBLE_EQ(a,b) do {                                \
    ++gTestsRun;                                                  \
    if(!((a) == (b))) {                                           \
        ++gTestsFailed;                                           \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__  \
                  << "  EXPECT_DOUBLE_EQ(" << (a) << ", " << (b) << ")" << std::endl; \
    }                                                             \
} while(false)


// Helper: reset buffer to a known state (all bytes to 0)
static void clearBuffer(uint8_t* buf, size_t size) {
    std::memset(buf, 0, size);
}


// Step 1: Candidate Keywords (represented in tests as comments and used by test harness)
// - Info -> OutputFormat (controls Planar vs Chunky path via T_PLANAR and PixelSize/T_EXTRA usage)
// - wOut[]: input 16-bit word triple that encodes XYZ
// - output: destination buffer for 16-bit->XYZ conversion
// - Stride: bytes-per-pixel related stride; divided by PixelSize(Info->OutputFormat) when planar
// - cmsCIEXYZ: XYZ storage structure used in non-planar branch
// - cmsXYZEncoded2Float: decoding of packed 16-bit XYZ -> double XYZ (affects outputs)
// - cmsFloat64Number: double precision type for XYZ in planar output
// - T_PLANAR(Format), PixelSize(Format), T_EXTRA(Format): macros used to select path and offsets
// - sizeof(cmsCIEXYZ), sizeof(cmsFloat64Number): used to compute expected return pointer offset


// Step 2: Test Suite for PackXYZDoubleFrom16
// The tests below aim to cover both branches of the focal function. Since precise OutputFormat
// values depend on the library's constants, tests are guarded with #ifdef checks. If a test
// macro is not defined in the environment, the test is skipped gracefully with a message.

void test_PackXYZDoubleFrom16_PlanarBranch_PlanarFormat() {
    // This test exercises the Planar branch (T_PLANAR is true).
    // It uses zeroed XYZ to verify that memory gets written as zeros and the
    // return pointer is positioned after the first cmsFloat64Number.

#ifdef TYPE_PLANAR_RGB_16
    _cmsTRANSFORM Info;
    std::memset(&Info, 0, sizeof(_cmsTRANSFORM));
    Info.OutputFormat = TYPE_PLANAR_RGB_16; // Mark this as planar for the test
#else
    // If the environment doesn't define a planar format, skip gracefully.
    std::cout << "[SKIP] TYPE_PLANAR_RGB_16 not defined; skipping Planar test." << std::endl;
    return;
#endif

    cmsUInt16Number wOut[3] = {0, 0, 0}; // encoded XYZ (zeros)
    cmsUInt8Number outputBuffer[256];
    clearBuffer(outputBuffer, sizeof(outputBuffer));

    // Stride is initially PixelSize(...) so that after division by PixelSize, Stride becomes 1
    cmsUInt32Number StrideInput = PixelSize(Info.OutputFormat);

    cmsUInt8Number* retPtr = PackXYZDoubleFrom16(&Info, wOut, outputBuffer, StrideInput);

    // Expected: planar path returns after the first cmsFloat64Number (i.e., after 8 bytes)
    cmsUInt8Number* expectedPtr = outputBuffer + sizeof(cmsFloat64Number);
    EXPECT_TRUE(retPtr == expectedPtr);

    // The first 3 doubles should represent X,Y,Z; with zero input, they should be 0.0
    cmsFloat64Number* OutDoubles = reinterpret_cast<cmsFloat64Number*>(outputBuffer);
    EXPECT_DOUBLE_EQ(static_cast<double>(OutDoubles[0]), 0.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(OutDoubles[1]), 0.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(OutDoubles[2]), 0.0);
}


// Step 2 (cont.): Non-Planar branch test
void test_PackXYZDoubleFrom16_NonPlanarBranch_NonPlanarFormat() {
    // This test exercises the non-planar branch (T_PLANAR is false).
    // It uses zeroed XYZ to verify the memory layout and return pointer offset.

#ifdef TYPE_RGB_16_NONPLANAR
    _cmsTRANSFORM Info;
    std::memset(&Info, 0, sizeof(_cmsTRANSFORM));
    Info.OutputFormat = TYPE_RGB_16_NONPLANAR; // Mark this as non-planar for the test
#else
    // If the environment doesn't define a non-planar format, skip gracefully.
    std::cout << "[SKIP] TYPE_RGB_16_NONPLANAR not defined; skipping Non-Planar test." << std::endl;
    return;
#endif

    cmsUInt16Number wOut[3] = {0, 0, 0}; // encoded XYZ (zeros)
    cmsUInt8Number outputBuffer[256];
    clearBuffer(outputBuffer, sizeof(outputBuffer));

    cmsUInt32Number StrideInput = PixelSize(Info.OutputFormat);

    cmsUInt8Number* retPtr = PackXYZDoubleFrom16(&Info, wOut, outputBuffer, StrideInput);

    // Expected: non-planar path returns after cmsCIEXYZ plus potential extra data
    size_t expectedDelta = sizeof(cmsCIEXYZ) + T_EXTRA(Info.OutputFormat) * sizeof(cmsFloat64Number);
    cmsUInt8Number* expectedPtr = outputBuffer + expectedDelta;
    EXPECT_TRUE(retPtr == expectedPtr);

    // The memory at the start of outputBuffer should hold XYZ (X,Y,Z) as doubles if it was written
    cmsFloat64Number* XYZAsDbl = reinterpret_cast<cmsFloat64Number*>(outputBuffer);
    EXPECT_DOUBLE_EQ(static_cast<double>(XYZAsDbl[0]), 0.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(XYZAsDbl[1]), 0.0);
    EXPECT_DOUBLE_EQ(static_cast<double>(XYZAsDbl[2]), 0.0);
}


// Step 3: Test Runner
// We expose a simple main that runs the tests above. The tests are designed to be non-terminating; failures
// only increment the failure counter and print descriptive messages.

int main() {
    std::cout << "Running PackXYZDoubleFrom16 unit tests (non-terminating assertions)." << std::endl;

    test_PackXYZDoubleFrom16_PlanarBranch_PlanarFormat();
    test_PackXYZDoubleFrom16_NonPlanarBranch_NonPlanarFormat();

    std::cout << "Tests completed. Ran: " << gTestsRun << ", Failed: " << gTestsFailed << std::endl;

    // Return non-zero if any test failed to aid integration with build systems.
    if (gTestsFailed > 0) {
        return 1;
    }
    return 0;
}