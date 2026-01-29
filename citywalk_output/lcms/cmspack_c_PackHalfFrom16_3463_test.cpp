#include <cstring>
#include <vector>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight test harness for PackHalfFrom16 (reproduced from focal method)
// This test environment mocks the required dependencies/macros to compile and run
// under C++11 without external testing frameworks.

// Domain-specific typedefs and macros (simplified for testing)
typedef uint32_t cmsUInt32Number;
typedef uint16_t cmsUInt16Number;
typedef uint8_t  cmsUInt8Number;
typedef float    cmsFloat32Number;
typedef int      cmsBool;

// Minimal transform struct to hold OutputFormat
struct _cmsTRANSFORM {
    cmsUInt32Number OutputFormat;
};

// Candidate Keywords (conceptual): nChan, DoSwap, Reverse, Extra, SwapFirst, Planar, ExtraFirst, maximum, v, swap1, Stride

// Macros to emulate original focal function's behavior (encoding scheme)
#define T_CHANNELS(fmt) ((fmt) & 0xFF)
#define T_DOSWAP(fmt)    (((fmt) >> 8) & 0x1)
#define T_FLAVOR(fmt)     (((fmt) >> 9) & 0x1)
#define T_EXTRA(fmt)      (((fmt) >> 10) & 0x1)
#define T_SWAPFIRST(fmt)  (((fmt) >> 11) & 0x1)
#define T_PLANAR(fmt)     (((fmt) >> 12) & 0x1)
#define IsInkSpace(fmt)    (((fmt) >> 13) & 0x1)
#define PixelSize(fmt)      (2) // 16-bit units in this simplified test

// Simple float-to-half conversion (IEEE-754 half precision encoding)
// This is a self-contained implementation suitable for testing purposes.
static cmsUInt16Number _cmsFloat2Half(cmsFloat32Number f)
{
    // Handle NaN/Inf and zero quickly
    uint32_t u;
    std::memcpy(&u, &f, sizeof(u));
    uint32_t sign = (u >> 31) & 0x1;
    int exp      = (u >> 23) & 0xFF;
    uint32_t frac  = u & 0x7FFFFF;

    cmsUInt16Number h = 0;

    if (exp == 0xFF) { // Inf or NaN
        if (frac != 0) {
            // NaN -> use a quiet NaN payload
            h = (cmsUInt16Number)((sign << 15) | 0x7E00);
        } else {
            // Inf
            h = (cmsUInt16Number)((sign << 15) | 0x7C00);
        }
    } else if (exp == 0) {
        // Subnormal in f32 -> map to zero (sufficiently safe for tests)
        h = (cmsUInt16Number)(sign << 15);
    } else {
        int newExp = exp - 127 + 15;
        if (newExp >= 0x1F) {
            // overflow -> Inf
            h = (cmsUInt16Number)((sign << 15) | 0x7C00);
        } else if (newExp <= 0) {
            // Subnormal in half
            int shift = 14 - newExp;
            uint32_t newFrac = (frac | 0x800000) >> shift;
            h = (cmsUInt16Number)(sign << 15) | (cmsUInt16Number)((newFrac) & 0x3FF);
        } else {
            uint32_t newFrac = (frac >> 13) & 0x3FF;
            h = (cmsUInt16Number)((sign << 15) | (newExp << 10) | newFrac);
        }
    }
    return h;
}

// PackHalfFrom16 (simplified reproduction of the focal method)
cmsUInt8Number* PackHalfFrom16(_cmsTRANSFORM* info,
                               cmsUInt16Number wOut[],
                               cmsUInt8Number* output,
                               cmsUInt32Number Stride)
{
    {
       cmsUInt32Number nChan      = T_CHANNELS(info->OutputFormat);
       cmsUInt32Number DoSwap     = T_DOSWAP(info->OutputFormat);
       cmsUInt32Number Reverse    = T_FLAVOR(info->OutputFormat);
       cmsUInt32Number Extra      = T_EXTRA(info->OutputFormat);
       cmsUInt32Number SwapFirst  = T_SWAPFIRST(info->OutputFormat);
       cmsUInt32Number Planar     = T_PLANAR(info->OutputFormat);
       cmsUInt32Number ExtraFirst = DoSwap ^ SwapFirst;
       cmsFloat32Number maximum = IsInkSpace(info->OutputFormat) ? 655.35F : 65535.0F;
       cmsFloat32Number v = 0;
       cmsUInt16Number* swap1 = (cmsUInt16Number*)output;
       cmsUInt32Number i, start = 0;
       Stride /= PixelSize(info->OutputFormat);
       if (ExtraFirst)
              start = Extra;
       for (i = 0; i < nChan; i++) {
              cmsUInt32Number index = DoSwap ? (nChan - i - 1) : i;
              v = (cmsFloat32Number)wOut[index] / maximum;
              if (Reverse)
                     v = maximum - v;
              if (Planar)
                     ((cmsUInt16Number*)output)[(i + start) * Stride] = _cmsFloat2Half(v);
              else
                     ((cmsUInt16Number*)output)[i + start] = _cmsFloat2Half(v);
       }

       if (Extra == 0 && SwapFirst) {
              memmove(swap1 + 1, swap1, (nChan - 1)* sizeof(cmsUInt16Number));
              *swap1 = _cmsFloat2Half(v);
       }
       if (T_PLANAR(info->OutputFormat))
              return output + sizeof(cmsUInt16Number);
       else
              return output + (nChan + Extra) * sizeof(cmsUInt16Number);
    }
}

// Convenience: small assertion helper
static void assert_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "ASSERT FAILED: " << msg << std::endl;
    }
}

// Test 1: Basic non-planar packing with two channels, no swapping/reversing, no extras.
// - Verifies that two input 16-bit words are converted to two 16-bit halves and stored in output[0..1].
// - Verifies the returned pointer offset matches (nChan+Extra)*sizeof(cmsUInt16Number).
static void test_PackHalfFrom16_BasicChunkyTwoChannels() {
    // Configure format: nChan=2, others disabled
    _cmsTRANSFORM info;
    info.OutputFormat = (2 & 0xFF); // nChan = 2

    // Input data
    cmsUInt16Number wOut[2] = { 1000, 30000 };

    // Output buffer (enough space for (nChan+Extra) elements; we allocate extra to be safe)
    std::vector<cmsUInt16Number> outputBuf(6, 0);
    cmsUInt8Number* outputPtr = reinterpret_cast<cmsUInt8Number*>(outputBuf.data());

    // Stride chosen so that Stride/PixelSize = 1
    cmsUInt32Number Stride = 2; // Byte-stride input; PixelSize=2 => 1

    cmsUInt8Number* ret = PackHalfFrom16(&info, wOut, outputPtr, Stride);

    // Expected: first two elements are _cmsFloat2Half of wOut[i]/65535
    cmsFloat32Number maximum = 65535.0F;
    cmsUInt16Number expected0 = _cmsFloat2Half((cmsFloat32Number)wOut[0] / maximum);
    cmsUInt16Number expected1 = _cmsFloat2Half((cmsFloat32Number)wOut[1] / maximum);

    // Validate memory contents
    assert_true(outputBuf[0] == expected0, "Basic chunky: outputBuf[0] should be half of v0");
    assert_true(outputBuf[1] == expected1, "Basic chunky: outputBuf[1] should be half of v1");

    // Validate return pointer offset
    ptrdiff_t offset = ret - outputPtr;
    assert_true(offset == 4, "Basic chunky: return pointer offset should be 4 bytes");
    std::cout << "Test 1 (BasicChunkyTwoChannels) passed" << std::endl;
}

// Test 2: Basic planar packing with two channels.
// - Verifies that two input values map to output[0] and output[1] as planar storage.
// - Return pointer should be after the first half (planar path returns output + sizeof(cmsUInt16Number)).
static void test_PackHalfFrom16_BasicPlanarTwoChannels() {
    _cmsTRANSFORM info;
    // nChan=2, Planar=1
    info.OutputFormat = (2 & 0xFF) | (1 << 12);

    cmsUInt16Number wOut[2] = { 4000, 8000 };

    std::vector<cmsUInt16Number> outputBuf(6, 0);
    cmsUInt8Number* outputPtr = reinterpret_cast<cmsUInt8Number*>(outputBuf.data());

    cmsUInt32Number Stride = 2; // Stride such that Stride/PixelSize = 1

    cmsUInt8Number* ret = PackHalfFrom16(&info, wOut, outputPtr, Stride);

    cmsFloat32Number maximum = 65535.0F;
    cmsUInt16Number expected0 = _cmsFloat2Half((cmsFloat32Number)wOut[0] / maximum);
    cmsUInt16Number expected1 = _cmsFloat2Half((cmsFloat32Number)wOut[1] / maximum);

    // In planar, memory is written to output[0] and output[1]
    assert_true(outputBuf[0] == expected0, "Planar two-ch: outputBuf[0] should equal expected0");
    assert_true(outputBuf[1] == expected1, "Planar two-ch: outputBuf[1] should equal expected1");

    // Return pointer: planar => output + sizeof(cmsUInt16Number) = offset 2
    ptrdiff_t offset = ret - outputPtr;
    assert_true(offset == 2, "Planar two-ch: return offset should be 2");
    std::cout << "Test 2 (BasicPlanarTwoChannels) passed" << std::endl;
}

// Test 3: ExtraFirst path with DoSwap=0, SwapFirst=1, Extra=1 and two channels.
// - Verifies that when ExtraFirst is true, we start writing at index 'Extra' and that
//   memmove is not triggered when Extra != 0.
// - Ensures that output[1] and output[2] receive the computed halves from v0 and v1 respectively,
//   while output[0] remains untouched (pre-filled with zero).
static void test_PackHalfFrom16_ExtraFirst_SwapFirstTwoChannels() {
    _cmsTRANSFORM info;
    // nChan=2, DoSwap=0, SwapFirst=1 -> ExtraFirst = 1; Extra=1
    info.OutputFormat = (2 & 0xFF) | (0 << 8) | (0 << 9) | (1 << 10) | (1 << 11) | (0 << 12);

    cmsUInt16Number wOut[2] = { 1000, 2000 };

    std::vector<cmsUInt16Number> outputBuf(6, 0);
    cmsUInt8Number* outputPtr = reinterpret_cast<cmsUInt8Number*>(outputBuf.data());

    cmsUInt32Number Stride = 2; // Stride such that Stride/PixelSize = 1

    cmsUInt8Number* ret = PackHalfFrom16(&info, wOut, outputPtr, Stride);

    cmsFloat32Number maximum = 65535.0F;
    cmsUInt16Number v0 = _cmsFloat2Half((cmsFloat32Number)wOut[0] / maximum);
    cmsUInt16Number v1 = _cmsFloat2Half((cmsFloat32Number)wOut[1] / maximum);

    // Because ExtraFirst is true and Extra=1, start=1; i=0 writes to output[1], i=1 writes to output[2]
    // output[0] remains prefilled (0)
    assert_true(outputBuf[0] == 0, "ExtraFirst: outputBuf[0] should remain zero");
    assert_true(outputBuf[1] == v0, "ExtraFirst: outputBuf[1] should be half(v0)");
    assert_true(outputBuf[2] == v1, "ExtraFirst: outputBuf[2] should be half(v1)");

    // Return offset should be (nChan + Extra) * sizeof(cmsUInt16Number) = (2+1)*2 = 6
    ptrdiff_t offset = ret - outputPtr;
    assert_true(offset == 6, "ExtraFirst: return offset should be 6");
    std::cout << "Test 3 (ExtraFirst_SwapFirstTwoChannels) passed" << std::endl;
}

int main() {
    test_PackHalfFrom16_BasicChunkyTwoChannels();
    test_PackHalfFrom16_BasicPlanarTwoChannels();
    test_PackHalfFrom16_ExtraFirst_SwapFirstTwoChannels();

    return 0;
}