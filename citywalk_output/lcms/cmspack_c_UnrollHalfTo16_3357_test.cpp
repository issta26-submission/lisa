// A self-contained C++11 test suite for the focal function UnrollHalfTo16
// This file provides a minimal mock environment to exercise UnrollHalfTo16
// without requiring the full cmsLCMS library. It focuses on the core logic
// and covers true/false branches of the predicate logic inside the function.
// The tests are non-terminating (they run all cases) and report a final summary.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Domain: Minimal re-implementation of the types and helpers used by UnrollHalfTo16

using cmsUInt8Number   = uint8_t;
using cmsUInt16Number  = uint16_t;
using cmsUInt32Number  = uint32_t;
using cmsFloat32Number = float;
using cmsFloat64Number = double;
using cmsBool          = int;

////////////////////////////////////////////////////////////////////////////////
// Faked transform structures and macros to drive UnrollHalfTo16 logic
////////////////////////////////////////////////////////////////////////////////

typedef struct _cmsTRANSFORM _cmsTRANSFORM;

typedef struct {
    uint32_t nChannels;
    uint32_t DoSwap;
    uint32_t Flavor;     // alias for "Reverse" in original code
    uint32_t SwapFirst;
    uint32_t Extra;
    uint32_t Planar;
    uint32_t InkSpace;   // affects maximum in computation
} _cmsInputFormat;

typedef struct {
    // Placeholder: only existence is required for compatibility
} _cmsOutputFormat;

struct _cmsTRANSFORM {
    _cmsInputFormat InputFormat;
    _cmsOutputFormat OutputFormat;
};

// Macros to mimic the usage inside UnrollHalfTo16
#define T_CHANNELS(fmt)      ((fmt).nChannels)
#define T_DOSWAP(fmt)         ((fmt).DoSwap)
#define T_FLAVOR(fmt)          ((fmt).Flavor)
#define T_SWAPFIRST(fmt)       ((fmt).SwapFirst)
#define T_EXTRA(fmt)           ((fmt).Extra)
#define T_PLANAR(fmt)          ((fmt).Planar)
#define IsInkSpace(fmt)         ((fmt).InkSpace)

static inline cmsUInt32Number PixelSize(const _cmsOutputFormat& /*fmt*/) {
    // In tests we use 16-bit per sample, so 2 bytes per sample
    return 2;
}

// Basic helpers to emulate the cms LCMS half-to-float conversion and saturate
static inline cmsFloat32Number _cmsHalf2Float(cmsUInt16Number h) {
    // Simple linear mapping: 0..65535 -> 0.0f .. 1.0f
    return (cmsFloat32Number)h / 65535.0f;
}
static inline cmsUInt16Number _cmsQuickSaturateWord(cmsFloat64Number x) {
    if (x <= 0.0) return 0;
    if (x >= 65535.0) return 65535;
    return (cmsUInt16Number)(x);
}

// The focal function under test (a near-direct translation of the snippet)
cmsUInt8Number* UnrollHalfTo16(_cmsTRANSFORM* info,
                               cmsUInt16Number wIn[],
                               cmsUInt8Number* accum,
                               cmsUInt32Number Stride)
{
    {
        cmsUInt32Number nChan      = T_CHANNELS(info -> InputFormat);
        cmsUInt32Number DoSwap     = T_DOSWAP(info ->InputFormat);
        cmsUInt32Number Reverse    = T_FLAVOR(info ->InputFormat);
        cmsUInt32Number SwapFirst  = T_SWAPFIRST(info -> InputFormat);
        cmsUInt32Number Extra      = T_EXTRA(info ->InputFormat);
        cmsUInt32Number ExtraFirst = DoSwap ^ SwapFirst;
        cmsUInt32Number Planar     = T_PLANAR(info -> InputFormat);
        cmsFloat32Number v;
        cmsUInt32Number i, start = 0;
        cmsFloat32Number maximum = IsInkSpace(info ->InputFormat) ? 655.35F : 65535.0F;

        Stride /= PixelSize(info->OutputFormat);
        if (ExtraFirst)
                start = Extra;
        for (i=0; i < nChan; i++) {
            cmsUInt32Number index = DoSwap ? (nChan - i - 1) : i;
            if (Planar)
                v = _cmsHalf2Float ( ((cmsUInt16Number*) accum)[(i + start) * Stride] );
            else
                v = _cmsHalf2Float ( ((cmsUInt16Number*) accum)[i + start] ) ;
            if (Reverse) v = maximum - v;
            wIn[index] = _cmsQuickSaturateWord((cmsFloat64Number) v * maximum);
        }

        if (Extra == 0 && SwapFirst) {
            cmsUInt16Number tmp = wIn[0];
            memmove(&wIn[0], &wIn[1], (nChan-1) * sizeof(cmsUInt16Number));
            wIn[nChan-1] = tmp;
        }
        if (T_PLANAR(info -> InputFormat))
            return accum + sizeof(cmsUInt16Number);
        else
            return accum + (nChan + Extra) * sizeof(cmsUInt16Number);
    }
}

// ---------------------------------------------------------------------------
// Lightweight test harness (non-GTest, purely std::cout driven)
// ---------------------------------------------------------------------------

static int g_failures = 0;

// Helper macro to record test results without exiting on first failure
#define EXPECT_EQACT(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "Assertion failed: " << msg \
                  << " | actual=" << (actual) << " expected=" << (expected) << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Assertion failed: " << msg << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "PASS: " << msg << std::endl; \
    } \
} while(0)


// Step 1 (Test planning): The Candidate Keywords identified relate to
// - nChannels, DoSwap, Reverse (Flavor), SwapFirst, Extra, Planar, InkSpace
// - Stride handling, Start index logic, ExtraFirst, planar vs chunky handling
// - Maximum value handling (InkSpace path), rotation when Extra==0 && SwapFirst
// - Return pointer offset depending on Planar

// Step 2: Test case generation for UnrollHalfTo16

// Test 1: Basic non-Planar flow with rotation (Extra==0 and SwapFirst==true)
// Verifies rotation behavior and the simple non-Planar path.
void test_UnrollHalfTo16_BasicRotation_and_NonPlanar() {
    std::cout << "Running test_UnrollHalfTo16_BasicRotation_and_NonPlanar" << std::endl;

    _cmsTRANSFORM info;
    info.InputFormat.nChannels = 3;
    info.InputFormat.DoSwap     = 0;
    info.InputFormat.Flavor     = 0;
    info.InputFormat.SwapFirst  = 1;
    info.InputFormat.Extra      = 0;
    info.InputFormat.Planar     = 0;
    info.InputFormat.InkSpace     = 0;

    cmsUInt16Number accum16[3] = { 0xFFFF, 0xFFFF, 0x0000 }; // Half values -> 1.0, 1.0, 0.0
    cmsUInt8Number* accumBytes = (cmsUInt8Number*)accum16;
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt32Number Stride = 1;

    cmsUInt8Number* ret = UnrollHalfTo16(&info, wIn, accumBytes, Stride);

    // Expected after conversion:
    // pre-rotation: [65535, 65535, 0]
    // rotation due to Extra==0 && SwapFirst -> [65535, 0, 65535]
    EXPECT_EQACT(wIn[0], (cmsUInt16Number)65535, "Test1: wIn[0] after rotation");
    EXPECT_EQACT(wIn[1], (cmsUInt16Number)0,      "Test1: wIn[1] after rotation");
    EXPECT_EQACT(wIn[2], (cmsUInt16Number)65535,  "Test1: wIn[2] after rotation");

    // Ensure pointer arithmetic returns the expected offset
    cmsUInt8Number* expectedRet = accumBytes + 6; // nChan(3) + Extra(0) => 3*2 = 6
    EXPECT_EQACT(ret - accumBytes, (cmsUInt8Number*)(6), "Test1: return pointer offset");
}

// Test 2: Planar path reading from accum[0], accum[1], ... with Planar set
void test_UnrollHalfTo16_PlanarPath() {
    std::cout << "Running test_UnrollHalfTo16_PlanarPath" << std::endl;

    _cmsTRANSFORM info;
    info.InputFormat.nChannels = 2;
    info.InputFormat.DoSwap     = 0;
    info.InputFormat.Flavor     = 0;
    info.InputFormat.SwapFirst  = 0;
    info.InputFormat.Extra      = 0;
    info.InputFormat.Planar     = 1;
    info.InputFormat.InkSpace     = 0;

    cmsUInt16Number accum16[2] = { 0x0000, 0xFFFF }; // 0.0 and ~1.0
    cmsUInt8Number* accumBytes = (cmsUInt8Number*)accum16;
    cmsUInt16Number wIn[2] = {0, 0};
    cmsUInt32Number Stride = 1;

    cmsUInt8Number* ret = UnrollHalfTo16(&info, wIn, accumBytes, Stride);

    // Planar: v from accum[(i+start)*Stride], start=0
    // wIn should be [0, 65535]
    EXPECT_EQACT(wIn[0], (cmsUInt16Number)0,      "Test2: wIn[0] planar");
    EXPECT_EQACT(wIn[1], (cmsUInt16Number)65535,  "Test2: wIn[1] planar");

    // Return should advance by (nChan + Extra)*sizeof(uint16) = (2+0)*2 = 4
    EXPECT_EQACT(ret - accumBytes, (cmsUInt8Number)(4), "Test2: return pointer offset planar");
}

// Test 3: Reverse branch active: Maximization path
void test_UnrollHalfTo16_ReverseBranch() {
    std::cout << "Running test_UnrollHalfTo16_ReverseBranch" << std::endl;

    _cmsTRANSFORM info;
    info.InputFormat.nChannels = 2;
    info.InputFormat.DoSwap     = 0;
    info.InputFormat.Flavor     = 1; // Reverse = 1
    info.InputFormat.SwapFirst  = 0;
    info.InputFormat.Extra      = 0;
    info.InputFormat.Planar     = 0;
    info.InputFormat.InkSpace     = 0;

    cmsUInt16Number accum16[2] = { 0x0000, 0xFFFF }; // 0.0 and 1.0
    cmsUInt8Number* accumBytes = (cmsUInt8Number*)accum16;
    cmsUInt16Number wIn[2] = {0, 0};
    cmsUInt32Number Stride = 1;

    cmsUInt8Number* ret = UnrollHalfTo16(&info, wIn, accumBytes, Stride);

    // With Reverse: v becomes maximum - v
    // v0 = 0 -> 65535; v1 = 1 -> 0
    EXPECT_EQACT(wIn[0], (cmsUInt16Number)65535, "Test3: wIn[0] reverse");
    EXPECT_EQACT(wIn[1], (cmsUInt16Number)0,       "Test3: wIn[1] reverse");

    // Return offset for non-planar: 3 channels
    EXPECT_EQACT(ret - accumBytes, (cmsUInt8Number)(4), "Test3: return pointer offset reverse?"); // Wait: nChan(2) + Extra(0) = 2 -> 4 bytes
}

// Test 4: ExtraFirst path with DoSwap leading to start != 0
void test_UnrollHalfTo16_ExtraFirst_SwapPath() {
    std::cout << "Running test_UnrollHalfTo16_ExtraFirst_SwapPath" << std::endl;

    _cmsTRANSFORM info;
    info.InputFormat.nChannels = 3;
    info.InputFormat.DoSwap     = 1;
    info.InputFormat.Flavor     = 0;
    info.InputFormat.SwapFirst  = 0;
    info.InputFormat.Extra      = 1;
    info.InputFormat.Planar     = 0;
    info.InputFormat.InkSpace     = 0;

    cmsUInt16Number accum16[5] = { 0xFFFF, 0xFFFF, 0x0000, 0x0000, 0xFFFF }; // ensure indices exist
    cmsUInt8Number* accumBytes = (cmsUInt8Number*)accum16;
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt32Number Stride = 1;

    cmsUInt8Number* ret = UnrollHalfTo16(&info, wIn, accumBytes, Stride);

    // With DoSwap=1, SwapFirst=0 => index mapping: wIn[2], wIn[1], wIn[0]
    // start = Extra (1) due to ExtraFirst = DoSwap ^ SwapFirst = 1 ^ 0 = 1
    // Values used from accum starting at index 1: accum[1], accum[2], accum[3] => {0xFFFF, 0x0000, 0x0000}
    // v: 1.0, 0.0, 0.0 -> wIn[2] = 65535, wIn[1] = 0, wIn[0] = 0
    EXPECT_EQACT(wIn[0], (cmsUInt16Number)0,      "Test4: wIn[0] extra-first");
    EXPECT_EQACT(wIn[1], (cmsUInt16Number)0,      "Test4: wIn[1] extra-first");
    EXPECT_EQACT(wIn[2], (cmsUInt16Number)65535,  "Test4: wIn[2] extra-first");

    // Return pointer should advance by (nChan + Extra) * sizeof(uint16) = (3+1)*2 = 8
    EXPECT_EQACT(ret - accumBytes, (cmsUInt8Number)(8), "Test4: return offset extra-first");
}

// Test 5: InkSpace path (maximum slightly different). Basic coverage to touch the maximum branch.
void test_UnrollHalfTo16_InkSpace_Maximum() {
    std::cout << "Running test_UnrollHalfTo16_InkSpace_Maximum" << std::endl;

    _cmsTRANSFORM info;
    info.InputFormat.nChannels = 3;
    info.InputFormat.DoSwap     = 0;
    info.InputFormat.Flavor     = 0;
    info.InputFormat.SwapFirst  = 0;
    info.InputFormat.Extra      = 0;
    info.InputFormat.Planar     = 0;
    info.InputFormat.InkSpace     = 1; // maximum becomes 655.35

    cmsUInt16Number accum16[3] = { 0xFFFF, 0x0000, 0x0000 }; // 1.0, 0, 0
    cmsUInt8Number* accumBytes = (cmsUInt8Number*)accum16;
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt32Number Stride = 1;

    cmsUInt8Number* ret = UnrollHalfTo16(&info, wIn, accumBytes, Stride);

    // wIn should map 1.0 -> maximum ~ 65535 (saturates to 65535)
    EXPECT_EQACT(wIn[0], (cmsUInt16Number)65535, "Test5: wIn[0] when InkSpace is set");
    EXPECT_EQACT(wIn[1], (cmsUInt16Number)0,       "Test5: wIn[1] when InkSpace is set");
    EXPECT_EQACT(wIn[2], (cmsUInt16Number)0,       "Test5: wIn[2] when InkSpace is set");

    // Return offset: (nChan + Extra) * sizeof(uint16) = 6
    EXPECT_EQACT(ret - accumBytes, (cmsUInt8Number)(6), "Test5: return offset inkspace");
}

// Step 3: Test runner main
int main() {
    std::cout << "Starting UnrollHalfTo16 unit tests (CM Shims, C++11)" << std::endl;

    test_UnrollHalfTo16_BasicRotation_and_NonPlanar();
    test_UnrollHalfTo16_PlanarPath();
    test_UnrollHalfTo16_ReverseBranch();
    test_UnrollHalfTo16_ExtraFirst_SwapPath();
    test_UnrollHalfTo16_InkSpace_Maximum();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}