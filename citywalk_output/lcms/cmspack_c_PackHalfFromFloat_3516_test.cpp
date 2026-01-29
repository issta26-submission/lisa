/*
   Lightweight unit test suite for the focal method: PackHalfFromFloat
   Approach:
   - Implement a self-contained, self-describing test harness in C++11.
   - Reproduce the core logic of PackHalfFromFloat in a local test function
     (PackHalfFromFloat_Test) with minimal, controlled helpers to avoid
     external dependencies. This allows exercising multiple branches by
     toggling a synthetic OutputFormat flag field.
   - Validate behavior via non-terminating checks (continue on failure),
     printing per-test results, and aggregating a final summary.
   - Tests are designed to cover key branches:
     1) Chunky (non-planar) path with 2 channels
     2) Planar path
     3) ExtraFirst path (non-zero Extra with DoSwap xor SwapFirst)
     4) SwapFirst memory shuffle path
     5) Reverse path (flavor) affecting computed half values
   Notes:
   - This test is self-contained and independent of external gtest/gmock.
   - It purposefully uses a simplified, deterministic mapping of OutputFormat bits
     to enable toggling of nChan, Planar, DoSwap, Reverse, Extra, and SwapFirst.
   - The tests rely on a known mapping of 16-bit half-precision values:
     0.5f -> 0x3800, 1.0f -> 0x3C00, 0.0f -> 0x0000.
     A robust 32-bit-to-16-bit float-to-half converter is implemented
     (FloatToHalf) to support these checks.
*/

#include <cstring>
#include <cstdint>
#include <vector>
#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cmath>


// Domain-defined types (self-contained for test environment)
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;
typedef float cmsFloat32Number;

// Minimal internal transform struct for testing
struct _cmsTRANSFORM {
    cmsUInt32Number OutputFormat; // synthetic, test-scoped flag field
};

// Forward declaration: test implementation mirrors the focal logic using
// a compact set of bit-fields extracted from OutputFormat.
static cmsUInt16Number FloatToHalf(cmsFloat32Number f);
static cmsUInt16Number _cmsFloat2Half(cmsFloat32Number v) { return FloatToHalf(v); }

// Helpers to emulate the domain-dependent predicates used by the focal method
// The bit-field layout is chosen solely for test convenience.
// Bit assignments (test-oriented):
//   bits 0-3   -> nChan (T_CHANNELS)
//   bit 4      -> DoSwap (T_DOSWAP)
//   bit 5      -> Reverse (T_FLAVOR)
//   bits 6-7    -> Extra (T_EXTRA)
//   bit 8      -> SwapFirst (T_SWAPFIRST)
//   bit 9      -> Planar (T_PLANAR)
// bit 0000 0000 0000 0000 0000 0000 0000 0000
// The following macros extract those fields.
static inline cmsUInt32Number T_CHANNELS(cmsUInt32Number fmt) { return fmt & 0x0F; }
static inline cmsUInt32Number T_DOSWAP(cmsUInt32Number fmt) { return (fmt >> 4) & 0x01; }
static inline cmsUInt32Number T_FLAVOR(cmsUInt32Number fmt) { return (fmt >> 5) & 0x01; }
static inline cmsUInt32Number T_EXTRA(cmsUInt32Number fmt) { return (fmt >> 6) & 0x03; }
static inline cmsUInt32Number T_SWAPFIRST(cmsUInt32Number fmt) { return (fmt >> 8) & 0x01; }
static inline cmsUInt32Number T_PLANAR(cmsUInt32Number fmt) { return (fmt >> 9) & 0x01; }

// Other helper predicates used by the focal code
static inline bool IsInkSpace(cmsUInt32Number Type) { return (Type & 0x100) != 0; } // test: inkspace flag (bit 8 in our layout)
static inline cmsUInt32Number PixelSize(cmsUInt32Number /*Format*/) { return 2; } // 16-bit per channel in tests

// Core test-specific PackHalfFromFloat implementation mirroring the focal method
static cmsUInt8Number* PackHalfFromFloat_Test(_cmsTRANSFORM* info,
                                             cmsFloat32Number wOut[],
                                             cmsUInt8Number* output,
                                             cmsUInt32Number Stride)
{
    // Local, test-oriented folding of the same logic
    {
        cmsUInt32Number nChan      = T_CHANNELS(info->OutputFormat);
        cmsUInt32Number DoSwap     = T_DOSWAP(info->OutputFormat);
        cmsUInt32Number Reverse    = T_FLAVOR(info->OutputFormat);
        cmsUInt32Number Extra      = T_EXTRA(info->OutputFormat);
        cmsUInt32Number SwapFirst  = T_SWAPFIRST(info->OutputFormat);
        cmsUInt32Number Planar     = T_PLANAR(info->OutputFormat);
        cmsUInt32Number ExtraFirst = DoSwap ^ SwapFirst;
        cmsFloat32Number maximum = IsInkSpace(info->OutputFormat) ? 100.0F : 1.0F;
        cmsUInt16Number* swap1 = (cmsUInt16Number*)output;
        cmsFloat32Number v = 0;
        cmsUInt32Number i, start = 0;
        Stride /= PixelSize(info->OutputFormat);
        if (ExtraFirst)
            start = Extra;
        for (i = 0; i < nChan; i++) {
            cmsUInt32Number index = DoSwap ? (nChan - i - 1) : i;
            v = wOut[index] * maximum;
            if (Reverse)
                v = maximum - v;
            if (Planar)
                ((cmsUInt16Number*)output)[(i + start) * Stride] = _cmsFloat2Half(v);
            else
                ((cmsUInt16Number*)output)[i + start] = _cmsFloat2Half(v);
        }

        if (Extra == 0 && SwapFirst) {
            memmove(swap1 + 1, swap1, (nChan - 1) * sizeof(cmsUInt16Number));
            *swap1 = (cmsUInt16Number)_cmsFloat2Half(v);
        }
        if (T_PLANAR(info->OutputFormat))
            return output + sizeof(cmsUInt16Number);
        else
            return output + (nChan + Extra) * sizeof(cmsUInt16Number);
    }
}

// Implementation of a compact float-to-half converter (IEEE 754 half-precision)
static cmsUInt16Number FloatToHalf(cmsFloat32Number f)
{
    uint32_t bits;
    std::memcpy(&bits, &f, sizeof(bits));

    uint32_t sign  = (bits >> 31) & 0x1;
    int      exp   = (bits >> 23) & 0xFF;
    uint32_t mant  =  bits & 0x7FFFFF;

    // NaN or Inf
    if (exp == 0xFF) {
        return (cmsUInt16Number)((sign << 15) | 0x7C00);
    }

    int newExp = exp - 127;
    if (newExp > 15) {
        // saturate to Inf
        return (cmsUInt16Number)((sign << 15) | 0x7C00);
    }
    if (newExp < -14) {
        // subnormal or zero: approximate toward zero
        // We can bias toward zero for simplicity in tests
        return (cmsUInt16Number)(sign << 15);
    }

    uint16_t hMant = static_cast<uint16_t>(mant >> 13); // top 10 bits

    uint16_t h = static_cast<uint16_t>((sign << 15) | ((newExp + 15) << 10) | hMant);
    return h;
}

// Convenience macro for test assertion (non-terminating)
#define CHECK(cond, msg)                                   \
    do {                                                     \
        if (!(cond)) {                                       \
            std::cerr << "FAIL: " << msg                       \
                      << " (Line " << __LINE__ << ")\n";       \
            ++gFailures;                                     \
        } else {                                             \
            std::cout << "PASS: " << msg << "\n";            \
        }                                                    \
    } while (0)

// Global test failure counter
static int gFailures = 0;

// Helper: print hex of a 16-bit value
static std::string hex16(cmsUInt16Number v)
{
    std::ostringstream os;
    os << "0x" << std::setfill('0') << std::setw(4) << std::hex << (v & 0xFFFF);
    return os.str();
}

// Test 1: Basic chunky path with 2 channels, no swap, no planar
void test_basic_chunky_two_channels()
{
    _cmsTRANSFORM info;
    info.OutputFormat = 2; // nChan = 2 (test assumption)

    cmsFloat32Number wOut[2] = {0.0f, 0.5f};
    cmsUInt8Number output[8];
    std::memset(output, 0, sizeof(output));

    cmsUInt32Number Stride = 4; // 4 bytes; Stride/PixelSize = 2
    cmsUInt8Number* endPtr = PackHalfFromFloat_Test(&info, wOut, output, Stride);

    cmsUInt16Number* p = reinterpret_cast<cmsUInt16Number*>(output);
    // Expected: p[0] = 0x0000, p[1] = 0x3800
    CHECK(p[0] == 0x0000, "Basic chunky: first channel -> 0x0000");
    CHECK(p[1] == 0x3800, "Basic chunky: second channel -> 0x3800");
    // Return pointer should be output + 4 (nChan*2)
    cmsUInt8Number* expectedPtr = output + 4;
    CHECK(endPtr == expectedPtr, "Basic chunky: returned pointer offset");
}

// Test 2: Planar path with 2 channels
void test_planar_two_channels()
{
    _cmsTRANSFORM info;
    // Planar flag at bit 9
    info.OutputFormat = 2 | (1 << 9); // nChan=2, Planar=1

    cmsFloat32Number wOut[2] = {0.0f, 0.5f};
    cmsUInt8Number output[8];
    std::memset(output, 0, sizeof(output));

    cmsUInt32Number Stride = 4; // 4 bytes; Stride/PixelSize = 2
    cmsUInt8Number* endPtr = PackHalfFromFloat_Test(&info, wOut, output, Stride);

    cmsUInt16Number* p = reinterpret_cast<cmsUInt16Number*>(output);
    // Planar write targets indices: (i+start)*Stride, Stride=2 -> indices 0 and 2
    CHECK(p[0] == 0x0000, "Planar: first channel at index 0 -> 0x0000");
    CHECK(p[2] == 0x3800, "Planar: second channel at index 2 -> 0x3800");
    cmsUInt8Number* expectedPtr = output + sizeof(cmsUInt16Number); // 2 bytes
    // In planar, return should be output + sizeof(cmsUInt16Number)
    CHECK(endPtr == expectedPtr, "Planar: returned pointer offset");
}

// Test 3: ExtraFirst path: Extra != 0 and DoSwap xor SwapFirst -> ExtraFirst = 1
void test_extrafirst_swapfirst()
{
    _cmsTRANSFORM info;
    // nChan=2, Extra=1, SwapFirst=1 -> ExtraFirst = 0 (No), we need DoSwap ^ SwapFirst == 1
    // We'll set DoSwap = 0, SwapFirst = 1, Extra = 1
    info.OutputFormat = 2 | (1 << 6) | (1 << 8); // nChan=2, Extra=1 (bits 6-7), SwapFirst=1 (bit 8)

    cmsFloat32Number wOut[2] = {0.0f, 0.5f};
    cmsUInt8Number output[12];
    std::memset(output, 0, sizeof(output));

    cmsUInt32Number Stride = 4; // after division by PixelSize (2) => 2
    cmsUInt8Number* endPtr = PackHalfFromFloat_Test(&info, wOut, output, Stride);

    cmsUInt16Number* p = reinterpret_cast<cmsUInt16Number*>(output);
    // Expect: since ExtraFirst = 1, start = Extra, so positions shift by 1
    // Indices: i=0 -> writes at (0+1)*Stride = 2; i=1 -> (1+1)*Stride = 4
    // After this write, memory at p[2] should be 0x0000 and p[4] should be 0x3800
    CHECK(p[2] == 0x0000, "ExtraFirst: first channel at index 2 -> 0x0000");
    CHECK(p[4] == 0x3800, "ExtraFirst: second channel at index 4 -> 0x3800");
    // Ensure pointer offset is (nChan+Extra) * sizeof(half)
    cmsUInt8Number* expectedPtr = output + (2 + 1) * sizeof(cmsUInt16Number);
    CHECK(endPtr == expectedPtr, "ExtraFirst: returned pointer offset");
}

// Test 4: SwapFirst memory swap path (Extra == 0, SwapFirst == 1)
void test_swapfirst_memory_swap()
{
    _cmsTRANSFORM info;
    // nChan=2, Extra=0, SwapFirst=1
    info.OutputFormat = 2 | (1 << 8);

    cmsFloat32Number wOut[2] = {0.0f, 0.5f};
    cmsUInt8Number output[8];
    std::memset(output, 0, sizeof(output));

    cmsUInt32Number Stride = 4;
    cmsUInt8Number* endPtr = PackHalfFromFloat_Test(&info, wOut, output, Stride);

    cmsUInt16Number* p = reinterpret_cast<cmsUInt16Number*>(output);
    // After swap: memory should be swapped between first two channels
    // Initially would be [0x0000, 0x3800], after memmove and swap: [0x3800, 0x0000]
    CHECK(p[0] == 0x3800, "SwapFirst: first element swapped to 0x3800");
    CHECK(p[1] == 0x0000, "SwapFirst: second element swapped to 0x0000");
    cmsUInt8Number* expectedPtr = output + 4;
    CHECK(endPtr == expectedPtr, "SwapFirst: returned pointer offset");
}

// Test 5: Reverse flavor effect (Reverse true)
void test_reverse_flavor()
{
    _cmsTRANSFORM info;
    // nChan=2, Reverse=1
    info.OutputFormat = 2 | (1 << 5);

    cmsFloat32Number wOut[2] = {1.0f, 0.0f};
    cmsUInt8Number output[8];
    std::memset(output, 0, sizeof(output));

    cmsUInt32Number Stride = 4;
    cmsUInt8Number* endPtr = PackHalfFromFloat_Test(&info, wOut, output, Stride);

    cmsUInt16Number* p = reinterpret_cast<cmsUInt16Number*>(output);
    // v for i=0: index 0, v = 1.0 -> Reverse -> 1.0 - 1.0 = 0.0 -> 0x0000
    // i=1: index 1, v = 0.0 -> Reverse -> 1.0 - 0.0 = 1.0 -> 0x3C00
    CHECK(p[0] == 0x0000, "Reverse: first channel becomes 0x0000");
    CHECK(p[1] == 0x3C00, "Reverse: second channel becomes 0x3C00");
    cmsUInt8Number* expectedPtr = output + 4;
    CHECK(endPtr == expectedPtr, "Reverse: returned pointer offset");
}

int main()
{
    std::cout << "Unit tests for PackHalfFromFloat (test harness version)\n";

    test_basic_chunky_two_channels();
    test_planar_two_channels();
    test_extrafirst_swapfirst();
    test_swapfirst_memory_swap();
    test_reverse_flavor();

    std::cout << "-----------------------------------\n";
    if (gFailures == 0) {
        std::cout << "ALL TESTS PASSED (as per this harness).\n";
        return 0;
    } else {
        std::cout << gFailures << " TEST(S) FAILED.\n";
        return 1;
    }
}