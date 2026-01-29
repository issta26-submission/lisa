// Test suite for Unroll16ToFloat in cmspack.c (LittleCMS)
 // Note: This test suite is designed to be compiled as a C++11 translation unit.
 // It relies on the real lcms2 internal headers and the Unroll16ToFloat function
 // defined in cmspack.c. The exact InputFormat constants (e.g., TYPE_GRAY_16)
 // depend on the library version; adjust if your environment uses different names.
 //
 // Design goals:
 // - Cover true/false branches for key predicates: Planar, Extra, DoSwap,
 //   SwapFirst, and the rotation behavior that occurs when Extra == 0 && SwapFirst.
 // - Use non-terminating checks to maximize code execution (log failures but continue).
 // - Do not rely on GoogleTest; implement a lightweight inline test harness.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <cstring>


// Bring LCMS internal types/macros into scope
extern "C" {
}

// Prototype for the focal function (C linkage)
extern "C" cmsUInt8Number* Unroll16ToFloat(_cmsTRANSFORM* info,
                                         cmsFloat32Number wIn[],
                                         cmsUInt8Number* accum,
                                         cmsUInt32Number Stride);
}

// Simple non-terminating assertion helpers
static int gTestFailures = 0;
static int gTestsRun = 0;

static void logFail(const char* msg, const char* file, int line) {
    std::cerr << "Test failure: " << msg << " (at " << file << ":" << line << ")\n";
}

// Assertion macro: does not terminate execution
#define ASSERT_TRUE(cond, msg) do { \
    ++gTestsRun; \
    if(!(cond)) { \
        logFail(msg, __FILE__, __LINE__); \
        ++gTestFailures; \
    } \
} while(0)

#define ASSERT_FLOAT_EQ(a, b, eps, msg) do { \
    ++gTestsRun; \
    if(std::fabs((a) - (b)) > (eps)) { \
        char buf[256]; \
        std::snprintf(buf, sizeof(buf), "%s: Expected %+.6f, got %+.6f", (msg), (double)(b), (double)(a)); \
        logFail(buf, __FILE__, __LINE__); \
        ++gTestFailures; \
    } \
} while(0)

// Helper to print a summary at the end
static void printSummary() {
    std::cout << "\nTest Summary: " << gTestsRun << " tests run, "
              << gTestFailures << " failures.\n";
}

// Note on InputFormat constants
// The following tests assume typical 16-bit per-channel formats providing 1 or 2 channels.
// Depending on your LCMS version, the exact constants differ (e.g., TYPE_GRAY_16 for 1 channel,
// TYPE_RGB_16 or TYPE_GRAY_16 for 2 channels in some builds). If your environment uses
// different constants, replace TYPE_GRAY_16 and TYPE_RGB_16 with the correct ones
// available in lcms2_internal.h (or lcms2.h) for 16-bit inputs.

static bool test_Unroll16ToFloat_SingleChannel_NoSwap() {
    // Scenario: 1 channel, no swap, no extra, not planar. Ensure v = accum[0] / 65535.0
    _cmsTRANSFORM info;
    std::memset(&info, 0, sizeof(info));
    // Pick a 16-bit single-channel format. Adjust if your header defines a different name.
    info.InputFormat = TYPE_GRAY_16; // Expected: 1 channel, 16-bit per sample (adjust if needed)

    cmsFloat32Number wIn[1] = {0.0f};
    cmsUInt16Number accum16[2];
    accum16[0] = 0x8000; // 32768 -> 0.5
    accum16[1] = 0x0000; // unused here
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accum16);

    cmsUInt32Number Stride = 2; // PixelSize(InputFormat) assumed to be 2 for 16-bit formats

    cmsUInt8Number* ret = Unroll16ToFloat(&info, wIn, accum, Stride);
    cmsUInt8Number* expectedPtr = accum + sizeof(cmsUInt16Number); // 2 bytes advancement for nChan=1

    bool okVal = (wIn[0] > 0.0f) && (wIn[0] < 1.0f) && (std::fabs(wIn[0] - 0.5f) < 1e-4f);
    bool okPtr = (ret == expectedPtr);

    ASSERT_TRUE(okVal, "Unroll16ToFloat single-channel: value should be ~0.5 (0x8000) -> 0.5");
    ASSERT_TRUE(okPtr, "Unroll16ToFloat single-channel: return pointer should advance by 2 bytes for 1 channel");

    return (gTestFailures == 0);
}

// Scenario: 2 channels, SwapFirst = 1 and Extra = 0; DoSwap = 0 -> ExtraFirst = 1 (start = 0); 
// After processing, wIn should be rotated: [v1, v0] where v0 comes from accum[0], v1 from accum[1].
static bool test_Unroll16ToFloat_TwoChannel_SwapFirst_Rotation() {
    _cmsTRANSFORM info;
    std::memset(&info, 0, sizeof(info));
    info.InputFormat = TYPE_RGB_16; // Expect at least 2 channels; adjust as needed.

    cmsFloat32Number wIn[2] = {0.0f, 0.0f};
    cmsUInt16Number accum16[4];
    accum16[0] = 0x0000; // v0 = 0.0
    accum16[1] = 0xFFFF; // v1 = ~1.0
    // The second pair would be unused for 2-channel test
    accum16[2] = 0x0000;
    accum16[3] = 0x0000;
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accum16);

    // Stride = PixelSize (16-bit) = 2 -> Stride becomes 1 after division
    cmsUInt32Number Stride = 2;

    // Configure: 2 channels, DoSwap = 0, SwapFirst = 1, Extra = 0
    // With macros, DoSwap = T_DOSWAP(info->InputFormat);
    // SwapFirst = T_SWAPFIRST(info->InputFormat);
    // Extra = T_EXTRA(info->InputFormat);
    // We rely on the library to interpret these bits from InputFormat constant.
    // If your environment uses different bits, adjust INPUTFORMAT accordingly.
    info.InputFormat = TYPE_GRAY2_16_SWAPFIRST; // Placeholder concept: 2 channels, SwapFirst = true
    // If the specific constant TYPE_GRAY2_16_SWAPFIRST is not defined in your headers,
    // replace it with a matching 2-channel 16-bit format that yields SwapFirst = true.

    // Initialize wIn with expected 2-channel values before rotation
    wIn[0] = 0.0f;  // v0
    wIn[1] = 0.0f;  // v1

    cmsUInt8Number* ret = Unroll16ToFloat(&info, wIn, accum, Stride);
    cmsUInt8Number* expectedPtr = accum + 4; // nChan(2) * sizeof(uint16)

    // After processing with rotation: wIn should become [v1, v0] => [1.0, 0.0]
    ASSERT_FLOAT_EQ(wIn[0], 1.0f, 1e-4f, "Two-channel SwapFirst: wIn[0] should be ~1.0 after rotation");
    ASSERT_FLOAT_EQ(wIn[1], 0.0f, 1e-4f, "Two-channel SwapFirst: wIn[1] should be 0.0 after rotation");
    ASSERT_TRUE(ret == expectedPtr, "Two-channel SwapFirst: return pointer should advance by 4 bytes for 2 channels");

    return (gTestFailures == 0);
}

// Scenario: Planar = true with 2 channels; ensure Planar path uses accum[(i+start)*Stride]
// and returns accum + sizeof(cmsUInt16Number)
static bool test_Unroll16ToFloat_TwoChannel_Planar() {
    _cmsTRANSFORM info;
    std::memset(&info, 0, sizeof(info));
    // Pick a planar, 2-channel 16-bit format
    info.InputFormat = TYPE_PLANAR_16_RGB; // Placeholder: 2 channels, planar

    cmsFloat32Number wIn[2] = {0.0f, 0.0f};
    cmsUInt16Number accum16[2];
    accum16[0] = 0x4000; // 0.25
    accum16[1] = 0xC000; // 0.75
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accum16);

    cmsUInt32Number Stride = 2; // PixelSize 2 -> Stride becomes 1 after division

    cmsUInt8Number* ret = Unroll16ToFloat(&info, wIn, accum, Stride);

    // Expect wIn = [0.25, 0.75]
    ASSERT_FLOAT_EQ(wIn[0], 0.25f, 1e-4f, "Planar: wIn[0] should be 0.25");
    ASSERT_FLOAT_EQ(wIn[1], 0.75f, 1e-4f, "Planar: wIn[1] should be 0.75");

    cmsUInt8Number* expectedPtr = accum + sizeof(cmsUInt16Number);
    ASSERT_TRUE(ret == expectedPtr, "Planar: return pointer should advance by 2 bytes");
    return (gTestFailures == 0);
}

// Scenario: Extra > 0 (e.g., Extra = 2); ensure output pointer accounts for Extra
static bool test_Unroll16ToFloat_ExtraBytes() {
    _cmsTRANSFORM info;
    std::memset(&info, 0, sizeof(info));
    info.InputFormat = TYPE_GRAY_16_EXTRA; // Placeholder: 1 or more channels with Extra > 0

    cmsFloat32Number wIn[1] = {0.0f};
    cmsUInt16Number accum16[4];
    accum16[0] = 0x0000; // 0.0
    accum16[1] = 0xFFFF; // ~1.0 (not used for nChan=1, but part of accum)
    accum16[2] = 0x0001;
    accum16[3] = 0x0002;
    cmsUInt8Number* accum = reinterpret_cast<cmsUInt8Number*>(accum16);

    cmsUInt32Number Stride = 4; // 16-bit per sample
    // Assuming Extra = 2 for this InputFormat
    info.InputFormat = TYPE_GRAY_16_EXTRA_2; // Placeholder: Extra == 2

    cmsUInt8Number* ret = Unroll16ToFloat(&info, wIn, accum, Stride);

    // With nChan=1, v = 0.0, so wIn[0] should be 0.0
    ASSERT_FLOAT_EQ(wIn[0], 0.0f, 1e-6f, "ExtraBytes: wIn[0] should be 0.0 for v0==0");
    // Return pointer should be accum + (nChan + Extra)*sizeof(uint16) = 1 + 2 = 3? times 2 = 6
    // Since the exact calculation depends on the actual nChan, compute expected as we know: (nChan+Extra)*2
    cmsUInt32Number nChan = 1; // placeholder assumption; if your environment uses a different nChan for this format, adjust accordingly
    cmsUInt32Number Extra = 2;
    cmsUInt8Number* expectedPtr = accum + (nChan + Extra) * sizeof(cmsUInt16Number);
    ASSERT_TRUE(ret == expectedPtr, "ExtraBytes: return pointer should account for Extra bytes in planar/packed case");

    return (gTestFailures == 0);
}

// Main tester driver
int main() {
    // Run test cases
    bool t1 = test_Unroll16ToFloat_SingleChannel_NoSwap();
    bool t2 = test_Unroll16ToFloat_TwoChannel_SwapFirst_Rotation();
    bool t3 = test_Unroll16ToFloat_TwoChannel_Planar();
    bool t4 = test_Unroll16ToFloat_ExtraBytes();

    // Summary
    printSummary();

    // Return code: 0 if all tests pass, non-zero otherwise
    if (gTestFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << gTestFailures << " test(s) failed.\n";
        return 1;
    }
}