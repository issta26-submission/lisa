// cms_unroll_planar_words_tests.cpp
// Lightweight unit tests for UnrollPlanarWords (recreated minimal environment)
//
// Notes:
// - This test reproduces a minimal subset of the cmsUnrollPlanarWords environment
//   to exercise the logic around channel swapping, endian handling, and flavor reversal.
// - No external testing framework is used (GTest is avoided as requested).
// - A tiny test harness is implemented to collect and report failures without terminating
//   on first failure (non-terminating assertions).
// - The test is designed to be self-contained and compile with C++11.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>
#include <sstream>


// Domain knowledge: Only the standard library is allowed.
// Provide a minimal reimplementation of the necessary cms types, structures, and helpers.

typedef unsigned char  cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int   cmsUInt32Number;
typedef int              cmsBool;

// Forward declaration to mimic the CMSREGISTER usage
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Minimal type definitions to mirror the focal function's expectations
typedef struct {
    cmsUInt32Number channels;
    cmsUInt32Number doswap;
    cmsUInt32Number flavor;
    cmsUInt32Number endian16;
    cmsUInt32Number extra;
} cmsInputFormat;

// Minimal _cmsTRANSFORM structure with InputFormat member
typedef struct _cmsTRANSFORM {
    cmsInputFormat InputFormat;
} _cmsTRANSFORM;

// Macro-like accessors (as used in the original code)
#define T_CHANNELS(x)     ((x).channels)
#define T_DOSWAP(x)        ((x).doswap)
#define T_FLAVOR(x)        ((x).flavor)
#define T_ENDIAN16(x)      ((x).endian16)
#define T_EXTRA(x)         ((x).extra)

// Endian and flavor helpers (mirroring the original intent)
static inline cmsUInt16Number CHANGE_ENDIAN(cmsUInt16Number v) {
    // Swap the two bytes of a 16-bit value
    return (cmsUInt16Number)((v << 8) | (v >> 8));
}
static inline cmsUInt16Number REVERSE_FLAVOR_16(cmsUInt16Number v) {
    // Reverse flavor (byte swap as a stand-in for "reverse" behavior)
    return (cmsUInt16Number)((v << 8) | (v >> 8));
}

// The focal method to test (reproduced in this test file)
cmsUInt8Number* UnrollPlanarWords(CMSREGISTER _cmsTRANSFORM* info,
                                  CMSREGISTER cmsUInt16Number wIn[],
                                  CMSREGISTER cmsUInt8Number* accum,
                                  CMSREGISTER cmsUInt32Number Stride)
{
{
    cmsUInt32Number nChan = T_CHANNELS(info -> InputFormat);
    cmsUInt32Number DoSwap= T_DOSWAP(info ->InputFormat);
    cmsUInt32Number Reverse= T_FLAVOR(info ->InputFormat);
    cmsUInt32Number SwapEndian = T_ENDIAN16(info -> InputFormat);
    cmsUInt32Number i;
    cmsUInt8Number* Init = accum;
    if (DoSwap) {
        accum += T_EXTRA(info -> InputFormat) * Stride;
    }
    for (i=0; i < nChan; i++) {
        cmsUInt32Number index = DoSwap ? (nChan - i - 1) : i;
        cmsUInt16Number v = *(cmsUInt16Number*) accum;
        if (SwapEndian)
            v = CHANGE_ENDIAN(v);
        wIn[index] = Reverse ? REVERSE_FLAVOR_16(v) : v;
        accum +=  Stride;
    }
    return (Init + sizeof(cmsUInt16Number));
}
}

// Note: The rest of the file (other Unroll* functions) is not required for these tests.


// Lightweight test harness
static std::vector<std::string> g_failure_messages;

static void recordFailure(const std::string& msg) {
    g_failure_messages.push_back(msg);
}

// Helper to format an array for debugging
static std::string formatArray(const cmsUInt16Number arr[], size_t n) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < n; ++i) {
        if (i) oss << ", ";
        oss << arr[i];
    }
    oss << "]";
    return oss.str();
}

// Test Case 1: No swap, no endian, no flavor; straightforward sequential read
bool test_case_1_NoSwap_NoEndian_NoFlavor() {
    // Prepare 3 input values: 100, 200, 300
    cmsUInt8Number raw[6] = {
        0x64, 0x00, // 100
        0xC8, 0x00, // 200
        0x2C, 0x01  // 300
    };
    cmsUInt16Number wIn[3] = {0,0,0};
    _cmsTRANSFORM info;
    info.InputFormat.channels = 3;
    info.InputFormat.doswap   = 0;
    info.InputFormat.flavor   = 0;
    info.InputFormat.endian16   = 0;
    info.InputFormat.extra      = 0;

    cmsUInt8Number* accum = raw;
    cmsUInt32Number Stride = 2;

    UnrollPlanarWords(&info, wIn, accum, Stride);

    cmsUInt16Number exp[3] = {100, 200, 300};
    bool ok = true;
    for (int i = 0; i < 3; ++i) {
        if (wIn[i] != exp[i]) { ok = false; break; }
    }
    if (!ok) {
        std::ostringstream oss;
        oss << "Test1 Failed: expected " << formatArray(exp,3)
            << " got " << formatArray(wIn,3);
        recordFailure(oss.str());
    }
    return ok;
}

// Test Case 2: DoSwap = true; extra shifts starting pointer; reversed index mapping
bool test_case_2_DoSwap_WithExtra() {
    // 4 channels, values at accum+2: 10,11,12,13
    cmsUInt8Number raw[8] = {
        0x0A, 0x00, // 10
        0x0B, 0x00, // 11
        0x0C, 0x00, // 12
        0x0D, 0x00  // 13
    };
    cmsUInt16Number wIn[4] = {0,0,0,0};
    _cmsTRANSFORM info;
    info.InputFormat.channels = 4;
    info.InputFormat.doswap   = 1;
    info.InputFormat.flavor   = 0;
    info.InputFormat.endian16   = 0;
    info.InputFormat.extra      = 1; // shift by Stride when DoSwap is true

    cmsUInt8Number* accum = raw;
    cmsUInt32Number Stride = 2;

    UnrollPlanarWords(&info, wIn, accum, Stride);

    // Mapping: i=0 -> index=3 gets 10; i=1 -> index=2 gets 11; i=2 -> idx=1 ->12; i=3 -> idx=0 ->13
    cmsUInt16Number exp[4] = {13, 12, 11, 10};
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        if (wIn[i] != exp[i]) { ok = false; break; }
    }
    if (!ok) {
        std::ostringstream oss;
        oss << "Test2 Failed: expected " << formatArray(exp,4)
            << " got " << formatArray(wIn,4);
        recordFailure(oss.str());
    }
    return ok;
}

// Test Case 3: Endian swap enabled; values read should be byte-swapped
bool test_case_3_EndianSwapOnly() {
    // 3 channels: values 1, 2, 3 with little-endian memory
    cmsUInt8Number raw[6] = {
        0x01, 0x00, // 1
        0x02, 0x00, // 2
        0x03, 0x00  // 3
    };
    cmsUInt16Number wIn[3] = {0,0,0};
    _cmsTRANSFORM info;
    info.InputFormat.channels = 3;
    info.InputFormat.doswap   = 0;
    info.InputFormat.flavor   = 0;
    info.InputFormat.endian16   = 1; // Swap endian
    info.InputFormat.extra      = 0;

    cmsUInt8Number* accum = raw;
    cmsUInt32Number Stride = 2;

    UnrollPlanarWords(&info, wIn, accum, Stride);

    // After endian swap, values become 0x0100, 0x0200, 0x0300 -> 256, 512, 768
    cmsUInt16Number exp[3] = {256, 512, 768};
    bool ok = true;
    for (int i = 0; i < 3; ++i) {
        if (wIn[i] != exp[i]) { ok = false; break; }
    }
    if (!ok) {
        std::ostringstream oss;
        oss << "Test3 Failed: expected " << formatArray(exp,3)
            << " got " << formatArray(wIn,3);
        recordFailure(oss.str());
    }
    return ok;
}

// Test Case 4: Flavor reversal (Reverse) enabled; values should be byte-swapped as flavor
bool test_case_4_FlavorReverse() {
    // 3 channels: memory encodes 0x0123, 0x0234, 0x0345
    // To get v = 0x0123, we store bytes 0x23, 0x01
    cmsUInt8Number raw[6] = {
        0x23, 0x01, // 0x0123
        0x34, 0x02, // 0x0234
        0x45, 0x03  // 0x0345
    };
    cmsUInt16Number wIn[3] = {0,0,0};
    _cmsTRANSFORM info;
    info.InputFormat.channels = 3;
    info.InputFormat.doswap   = 0;
    info.InputFormat.flavor   = 1; // Reverse
    info.InputFormat.endian16   = 0;
    info.InputFormat.extra      = 0;

    cmsUInt8Number* accum = raw;
    cmsUInt32Number Stride = 2;

    UnrollPlanarWords(&info, wIn, accum, Stride);

    // After reversal, wIn should be [REVERSE(0x0123)=0x2301, REVERSE(0x0234)=0x3402, REVERSE(0x0345)=0x4503]
    cmsUInt16Number exp[3] = {0x2301, 0x3402, 0x4503};
    bool ok = true;
    for (int i = 0; i < 3; ++i) {
        if (wIn[i] != exp[i]) { ok = false; break; }
    }
    if (!ok) {
        std::ostringstream oss;
        oss << "Test4 Failed: expected " << formatArray(exp,3)
            << " got " << formatArray(wIn,3);
        recordFailure(oss.str());
    }
    return ok;
}

// Test Case 5: DoSwap + Endian + Endian+Swap combo (extra coverage)
bool test_case_5_Combined_DoSwap_Endian_Flip() {
    // 4 channels: raw values 0x0001,0x0002,0x0003,0x0004
    cmsUInt8Number raw[8] = {
        0x01,0x00,
        0x02,0x00,
        0x03,0x00,
        0x04,0x00
    };
    cmsUInt16Number wIn[4] = {0,0,0,0};
    _cmsTRANSFORM info;
    info.InputFormat.channels = 4;
    info.InputFormat.doswap   = 1;
    info.InputFormat.flavor   = 0; // no reverse
    info.InputFormat.endian16   = 1; // swap endian
    info.InputFormat.extra      = 0;

    cmsUInt8Number* accum = raw;
    cmsUInt32Number Stride = 2;

    UnrollPlanarWords(&info, wIn, accum, Stride);

    // Expected: DoSwap true with extra 0 retains mapping: indices reversed, v after endian swap
    // Original v: 1,2,3,4
    // After endian swap: 0x0100,0x0200,0x0300,0x0400
    // index mapping: i=0 -> index=3 gets 0x0100 (256)
    // i=1 -> index=2 gets 0x0200 (512)
    // i=2 -> index=1 gets 0x0300 (768)
    // i=3 -> index=0 gets 0x0400 (1024)
    cmsUInt16Number exp[4] = {1024, 768, 512, 256};
    bool ok = true;
    for (int i = 0; i < 4; ++i) {
        if (wIn[i] != exp[i]) { ok = false; break; }
    }
    if (!ok) {
        std::ostringstream oss;
        oss << "Test5 Failed: expected " << formatArray(exp,4)
            << " got " << formatArray(wIn,4);
        recordFailure(oss.str());
    }
    return ok;
}


// Main test runner
int main() {
    // Run tests
    bool t1 = test_case_1_NoSwap_NoEndian_NoFlavor();
    bool t2 = test_case_2_DoSwap_WithExtra();
    bool t3 = test_case_3_EndianSwapOnly();
    bool t4 = test_case_4_FlavorReverse();
    bool t5 = test_case_5_Combined_DoSwap_Endian_Flip();

    // Report results
    int total = 5;
    int failures = (int)g_failure_messages.size();

    // Explain results
    if (failures == 0) {
        std::cout << "All tests passed: " << total << " cases." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with failures: " << failures << " / " << total << std::endl;
        for (const auto& msg : g_failure_messages) {
            std::cout << " - " << msg << std::endl;
        }
        return 1;
    }
}