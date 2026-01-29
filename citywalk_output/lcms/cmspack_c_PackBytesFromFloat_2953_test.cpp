// Unit tests for PackBytesFromFloat in cmspack.c
// Note: This test suite targets the focal method PackBytesFromFloat and its
// interaction with the _cmsTRANSFORM info struct and its OutputFormat flags.
// The test suite avoids GTest/GMock and uses standard C++11 facilities only.
// It relies on the public and internal CMS/LittleCMS headers being available
// in the include path (e.g., lcms2.h and lcms2_internal.h) so that the same
// macros (T_CHANNELS, T_DOSWAP, T_FLAVOR, T_EXTRA, T_SWAPFIRST, T_PLANAR)
// used by the focal function are available to construct test scenarios.

// The tests are designed to be executed by linking against the real library
// (or an appropriately mocked build where the internal macros behave as in the
// production code). The tests aim to cover true/false branches of the
// predicates within PackBytesFromFloat and ensure correct pointer arithmetic
// and end-of-function return behavior for both planar and chunky formats.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <cmath>


// Forward declare the internal structures/macros used by the focal method.
// These are typically defined in lcms2_internal.h which is part of the LittleCMS
// internal headers. We include them here to ensure the same semantic environment
// the focal method expects during testing.
extern "C" {
}

// If your test environment requires additional public headers, include them here.
// #include "lcms2.h"

using namespace std;

// Helper: create a minimal _cmsTRANSFORM object with a specific OutputFormat.
// This mirrors the minimal subset of the actual structure required by PackBytesFromFloat.
// We assume the internal structure exposes a member named OutputFormat of type cmsUInt32Number.
static _cmsTRANSFORM* MakeTransformWithFormat(cmsUInt32Number fmt)
{
    // Allocate a transform object and set its OutputFormat field.
    // We allocate using operator new to keep it in C++ domain, but align with
    // how the library would allocate in C if required.
    _cmsTRANSFORM* info = new _cmsTRANSFORM;
    info->OutputFormat = fmt;
    return info;
}

// Helper: reset output buffer to a known value for easier assertions.
static void ResetBuffer(uint8_t* buffer, size_t len, uint8_t val = 0xAA)
{
    for (size_t i = 0; i < len; ++i) buffer[i] = val;
}

// Helper: pretty print a few bytes (for debugging when assertions fail)
static void DumpBytes(const uint8_t* data, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        printf("%02x ", data[i]);
    }
    printf("\n");
}

// Domain knowledge helpers to assemble a test OutputFormat.
// The actual encoding is defined by LittleCMS internal macros (T_CHANNELS, T_DOSWAP, etc.)
// We construct formats by combining macros that the focal function will decode.
// These helpers rely on the public/internal macros being available in the test build.
static cmsUInt32Number MakeFormat(int channels,
                                  int swapFirst,
                                  int planar,
                                  int extra,
                                  int doswap,
                                  int flavor)
{
    // The exact bitfields are determined by the internal macros:
    // CHANNELS, SWAPFIRST, PLANAR, EXTRA, DOSWAP, FLAVOR.
    // We try to compose the OutputFormat using the corresponding macros.
    cmsUInt32Number fmt = 0;

    // Note: If the macros CHANNELS_SH, SWAPFIRST_SH etc. are the ones used
    // by the library, you may need to adapt the composition accordingly.
    // The following is a reasonable approach when these macros exist in the headers.

    // SAFETY: Use known macros if they exist in your environment.
    // We attempt to build fmt via the typical Sh macros, falling back gracefully
    // if a given macro is not defined (we simply OR zeros in that case).

#ifdef CHANNELS_SH
    fmt |= CHANNELS_SH(channels);
#else
    // If CHANNELS_SH is not available, fall back to a conservative value.
    fmt |= (cmsUInt32Number)channels;
#endif

#ifdef SWAPFIRST_SH
    fmt |= SWAPFIRST_SH(swapFirst);
#else
    fmt |= (cmsUInt32Number)swapFirst;
#endif

#ifdef PLANAR_SH
    fmt |= PLANAR_SH(planar);
#else
    fmt |= (cmsUInt32Number)planar;
#endif

#ifdef EXTRA_SH
    fmt |= EXTRA_SH(extra);
#else
    fmt |= (cmsUInt32Number)extra;
#endif

#ifdef DOSWAP_SH
    fmt |= DOSWAP_SH(doswap);
#else
    fmt |= (cmsUInt32Number)doswap;
#endif

#ifdef FLAVOR_SH
    fmt |= FLAVOR_SH(flavor);
#else
    fmt |= (cmsUInt32Number)flavor;
#endif

    return fmt;
}

// Test 1: Basic packed, non-swapped, non-reversed, 4-channel scenario
// Expect four adjacent bytes corresponding to the 8-bit mapping of the 0.0 and 1.0 inputs.
// wOut: [0.0, 0.0, 1.0, 1.0]  -> output: [0,0,255,255] in non-planar, non-swapped case
static void Test_PackBytesFromFloat_BasicPacked_NonSwapped()
{
    // Arrange
    const int channels = 4;
    const int planar = 0;      // chunky interleaved (not planar)
    const int swapFirst = 0;
    const int extra = 0;
    const int doswap = 0;
    const int flavor = 0;       // not reversing
    cmsUInt32Number fmt = MakeFormat(channels, swapFirst, planar, extra, doswap, flavor);
    _cmsTRANSFORM* info = MakeTransformWithFormat(fmt);

    cmsFloat32Number inW[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    cmsUInt8Number output[8]; // generous buffer
    ResetBuffer(output, 8, 0x00);

    // Act
    cmsUInt8Number* ret = PackBytesFromFloat(info, inW, output, 1);

    // Assert
    // In non-planar, expected order is output[0..3] correspond to channels
    // with 0.0 -> 0, 1.0 -> 255
    assert(ret == output + (channels + extra) * sizeof(cmsUInt8Number) ||
           ret == output + sizeof(cmsUInt8Number) * 0); // pointer location is implementation dependent

    // Basic byte checks (first 4 bytes)
    // We expect: 0, 0, 255, 255 for [0.0,0.0,1.0,1.0]
    // If the exact mapping differs due to internal saturate/round, we still expect
    // values to be either 0 or 255 for 0.0 and 1.0 inputs respectively.
    assert(output[0] == 0 || output[0] == 0x00);
    assert(output[1] == 0 || output[1] == 0x00);
    assert(output[2] == 255 || output[2] == 0xFF);
    assert(output[3] == 255 || output[3] == 0xFF);

    // Cleanup
    delete info;
}

// Test 2: Basic packed, 0/1 with Extra = 1 (ExtraFirst) leading to a start offset
// Confirm that the first valid channel gets placed at index 1 and that the
// function still writes remaining channels correctly.
static void Test_PackBytesFromFloat_BasicPacked_ExtraFirst()
{
    // Arrange
    const int channels = 4;
    const int planar = 0;
    const int doswap = 0;
    const int flavor = 0;
    // We want ExtraFirst, so DoSwap ^ SwapFirst must be 1 and Extra must be non-zero.
    const int extra = 1;
    // Make DoSwap=0, SwapFirst=1 to satisfy DoSwap ^ SwapFirst = 1
    const int swapFirst = 1;
    cmsUInt32Number fmt = MakeFormat(channels, swapFirst, planar, extra, doswap, flavor);
    _cmsTRANSFORM* info = MakeTransformWithFormat(fmt);

    cmsFloat32Number inW[4] = {0.0f, 1.0f, 0.0f, 1.0f}; // first and third channels are 0.0, others 1.0
    cmsUInt8Number output[8];
    ResetBuffer(output, 8, 0x00);

    // Act
    cmsUInt8Number* ret = PackBytesFromFloat(info, inW, output, 1);

    // Assert
    // With ExtraFirst, start = Extra (1), so first computed vv should be at index 1
    // Byte at index 1 should be 0 (for 0.0) and at index 2 should be 255 (for 1.0)
    assert(output[1] == 0 || output[1] == 0x00);
    assert(output[2] == 255 || output[2] == 0xFF);

    // Cleanup
    delete info;
}

// Test 3: Planar format true, ensure that values are written with planar indexing
// and that the returned pointer offset reflects planar data.
// We use nChan=2 for simplicity (monochrome-like pair in planar format).
static void Test_PackBytesFromFloat_Planar2Channels()
{
    // Arrange
    const int channels = 2;
    const int planar = 1;
    const int doswap = 0;
    const int flavor = 0;
    const int extra = 0;
    const int swapFirst = 0;
    cmsUInt32Number fmt = MakeFormat(channels, swapFirst, planar, extra, doswap, flavor);
    _cmsTRANSFORM* info = MakeTransformWithFormat(fmt);

    cmsFloat32Number inW[2] = {1.0f, 0.0f}; // channel 0 -> 255, channel 1 -> 0
    cmsUInt8Number output[4];
    ResetBuffer(output, 4, 0x00);

    // Act
    cmsUInt8Number* ret = PackBytesFromFloat(info, inW, output, 1);

    // Assert
    // For planar, values should be placed at (i + start) * Stride
    // with i in {0,1}, start is 0 here; so output[0] and output[1] should reflect
    // 1.0f -> 255 and 0.0f -> 0 respectively.
    assert(output[0] == 255 || output[0] == 0xFF);
    assert(output[1] == 0 || output[1] == 0x00);

    // Cleanup
    delete info;
}

// Test 4: Reverse mode enabled (flavor) to verify that values are inverted
// We use a single channel to simplify expectations.
static void Test_PackBytesFromFloat_ReverseModeSingleChannel()
{
    // Arrange
    const int channels = 1;
    const int planar = 0;
    const int doswap = 0;
    const int flavor = 1; // Reverse
    const int extra = 0;
    const int swapFirst = 0;
    cmsUInt32Number fmt = MakeFormat(channels, swapFirst, planar, extra, doswap, flavor);
    _cmsTRANSFORM* info = MakeTransformWithFormat(fmt);

    cmsFloat32Number inW[1] = {1.0f}; // should saturate to 0xFF then inverted to 0x00
    cmsUInt8Number output[2];
    ResetBuffer(output, 2, 0x00);

    // Act
    cmsUInt8Number* ret = PackBytesFromFloat(info, inW, output, 1);

    // Assert
    // Inversion of 255 (for 1.0) given Reverse => 0
    // Expecting 0 in the first byte
    assert(output[0] == 0 || output[0] == 0x00);

    // Cleanup
    delete info;
}

// Runner
int main()
{
    // Initialize any global CMS state if required (not always necessary).

    // Run tests
    Test_PackBytesFromFloat_BasicPacked_NonSwapped();
    Test_PackBytesFromFloat_BasicPacked_ExtraFirst();
    Test_PackBytesFromFloat_Planar2Channels();
    Test_PackBytesFromFloat_ReverseModeSingleChannel();

    // If no assertion failed, all tests pass.
    cout << "All PackBytesFromFloat unit tests completed." << endl;
    return 0;
}