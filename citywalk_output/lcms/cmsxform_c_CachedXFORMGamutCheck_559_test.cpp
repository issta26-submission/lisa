// Automated unit tests for CachedXFORMGamutCheck (simplified in-test harness)
// This test suite is self-contained (no external test framework) and uses
// lightweight assertions that continue execution (non-terminating on failure).

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Domain-specific constants (simplified to allow in-test compilation)
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef uint8_t  cmsBool;

#define cmsMAXCHANNELS 8

// Forward declarations to mirror minimal dependencies (simplified)
struct _cmsTRANSFORM;

// Cache structure used inside the focal method
struct _cmsCACHE {
    cmsUInt16Number CacheIn[cmsMAXCHANNELS];
    cmsUInt16Number CacheOut[cmsMAXCHANNELS];
};

// Stride description used by the focal method
struct cmsStride {
    cmsUInt32Number BytesPerPlaneIn;
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerPlaneOut;
    cmsUInt32Number BytesPerLineOut;
};

// Transform structure (subset needed by focal method)
struct _cmsTRANSFORM {
    _cmsCACHE Cache;
    // Function pointers used by focal method
    cmsUInt8Number* (*FromInput)(struct _cmsTRANSFORM*, cmsUInt16Number wIn[],
                                 const cmsUInt8Number* accum, cmsUInt32Number BytesPerPlaneIn);
    cmsUInt8Number* (*ToOutput)(struct _cmsTRANSFORM*, cmsUInt16Number wOut[],
                                cmsUInt8Number* output, cmsUInt32Number BytesPerPlaneOut);
};

// Placeholder for required external function (no-op in tests)
void _cmsHandleExtraChannels(_cmsTRANSFORM* p, const void* in, void* out,
                             cmsUInt32Number PixelsPerLine,
                             cmsUInt32Number LineCount,
                             const cmsStride* Stride)
{
    // In the test harness we do not simulate extra channels; this is a no-op.
}

// Helpers to manage a small, deterministic FromInput sequence in tests
static int gFromInputCallCount = 0;
void ResetFromInputCounter() { gFromInputCallCount = 0; }

// Fake FromInput: produces a deterministic sequence of wIn values across calls.
// - Call 0: wIn[0] = 0
// - Call 1: wIn[0] = 1
// - Call 2+: wIn[0] = 0
// Other channels remain 0.
cmsUInt8Number* FakeFromInput(_cmsTRANSFORM* p, cmsUInt16Number wIn[],
                              const cmsUInt8Number* accum, cmsUInt32Number BytesPerPlaneIn)
{
    (void)p; (void)accum;
    if (gFromInputCallCount == 0) wIn[0] = 0;
    else if (gFromInputCallCount == 1) wIn[0] = 1;
    else wIn[0] = 0;
    for (int c = 1; c < cmsMAXCHANNELS; ++c) wIn[c] = 0;
    gFromInputCallCount++;
    return (cmsUInt8Number*)accum + BytesPerPlaneIn;
}

// Fake ToOutput: writes the first channel value to the output buffer
cmsUInt8Number* FakeToOutput(_cmsTRANSFORM* p, cmsUInt16Number wOut[],
                              cmsUInt8Number* output, cmsUInt32Number BytesPerPlaneOut)
{
    (void)p;
    output[0] = static_cast<cmsUInt8Number>(wOut[0]);
    for (int c = 1; c < cmsMAXCHANNELS; ++c) output[c] = 0;
    return output + BytesPerPlaneOut;
}

// Simple pixel transformation used in the non-cached path
void TransformOnePixelWithGamutCheck(_cmsTRANSFORM* p,
                                   const cmsUInt16Number wIn[],
                                   cmsUInt16Number wOut[])
{
    (void)p;
    // Simple deterministic transformation: add 5 to first channel
    wOut[0] = static_cast<cmsUInt16Number>(wIn[0] + 5);
    for (int c = 1; c < cmsMAXCHANNELS; ++c) wOut[c] = 0;
}

// The focal method under test (reproduced here for a self-contained test)
void CachedXFORMGamutCheck(_cmsTRANSFORM* p,
                           const void* in,
                           void* out, 
                           cmsUInt32Number PixelsPerLine,
                           cmsUInt32Number LineCount,
                           const cmsStride* Stride)
{
{
    cmsUInt8Number* accum;
    cmsUInt8Number* output;
    cmsUInt16Number wIn[cmsMAXCHANNELS], wOut[cmsMAXCHANNELS];
    _cmsCACHE Cache;
    size_t i, j, strideIn, strideOut;
    _cmsHandleExtraChannels(p, in, out, PixelsPerLine, LineCount, Stride);
    // Empty buffers for quick memcmp
    memset(wIn, 0, sizeof(wIn));
    memset(wOut, 0, sizeof(wOut));
    // Get copy of zero cache
    memcpy(&Cache, &p->Cache, sizeof(Cache));
    strideIn = 0;
    strideOut = 0;
    for (i = 0; i < LineCount; i++) {
        accum = (cmsUInt8Number*)in + strideIn;
        output = (cmsUInt8Number*)out + strideOut;
        for (j = 0; j < PixelsPerLine; j++) {
            accum = p->FromInput(p, wIn, accum, Stride->BytesPerPlaneIn);
            if (memcmp(wIn, Cache.CacheIn, sizeof(Cache.CacheIn)) == 0) {
                memcpy(wOut, Cache.CacheOut, sizeof(Cache.CacheOut));
            }
            else {
                TransformOnePixelWithGamutCheck(p, wIn, wOut);
                memcpy(Cache.CacheIn, wIn, sizeof(Cache.CacheIn));
                memcpy(Cache.CacheOut, wOut, sizeof(Cache.CacheOut));
            }
            output = p->ToOutput(p, wOut, output, Stride->BytesPerPlaneOut);
        }
        strideIn += Stride->BytesPerLineIn;
        strideOut += Stride->BytesPerLineOut;
    }
}
}

// Lightweight assertion helpers (non-terminating)
#define ASSERT_EQ(a, b) do { if ((a) != (b)) { \
    std::cerr << "Assertion failed: " << #a << " == " << #b \
              << " (" << (a) << " != " << (b) << ")\n"; \
    return false; } } while (0)
#define ASSERT_MEM_EQ(a, b, n) do { \
    if (std::memcmp((a), (b), (n)) != 0) { \
        std::cerr << "Assertion failed: memory not equal for " << n << " bytes\n"; \
        return false; } } while (0)

// Test 1: Basic caching path and gamut-check path within a single line
// Expectation:
// - Pixel 0: FromInput yields 0, matches initial zero CacheIn -> output 0
// - Pixel 1: FromInput yields 1, not matching cache -> gamut check applied -> output 6
// - Pixel 2: FromInput yields 0, not matching cache (CacheIn=1) -> gamut check -> output 5
// Resulting output: [0, 6, 5]
bool Test_CachedXFORMGamutCheck_BasicCaching_SingleLine()
{
    // Prepare transform with no extra channels
    _cmsTRANSFORM xform;
    std::memset(&xform, 0, sizeof(xform));
    xform.FromInput = FakeFromInput;
    xform.ToOutput   = FakeToOutput;

    // Initialize an all-zero cache
    std::memset(&xform.Cache, 0, sizeof(xform.Cache));

    // Input/Output buffers
    const cmsUInt32Number PixelsPerLine = 3;
    const cmsUInt32Number LineCount = 1; // single line for this test
    unsigned char in[3 * 1];  // 3 pixels * 1 byte per pixel
    unsigned char out[3 * 1]; // output buffer
    std::memset(in, 0, sizeof(in));
    std::memset(out, 0, sizeof(out));

    // Stride (bytes per plane = 1)
    cmsStride s;
    s.BytesPerPlaneIn  = 1;
    s.BytesPerLineIn   = PixelsPerLine * s.BytesPerPlaneIn;
    s.BytesPerPlaneOut = 1;
    s.BytesPerLineOut  = PixelsPerLine * s.BytesPerPlaneOut;

    // Reset deterministic FromInput sequence
    ResetFromInputCounter();
    gFromInputCallCount = 0;

    // Call the function under test
    CachedXFORMGamutCheck(&xform, in, out, PixelsPerLine, LineCount, &s);

    // Expected outputs: [0, 6, 5]
    unsigned char expected[3] = {0, 6, 5};
    for (int i = 0; i < 3; ++i) {
        if (out[i] != expected[i]) {
            std::cerr << "Test 1 failed at index " << i
                      << ": expected " << int(expected[i])
                      << ", got " << int(out[i]) << "\n";
            return false;
        }
    }
    return true;
}

// Test 2: Multi-line behavior to ensure cache persists across lines and more
// PixelsPerLine = 3, LineCount = 2
// FromInput sequence: 0,1,0,1,0,1
// Expected outputs:
// Line 1: 0, 6, 5
// Line 2: 6, 5, 6
bool Test_CachedXFORMGamutCheck_BasicCaching_TwoLines()
{
    // Prepare transform as in Test 1
    _cmsTRANSFORM xform;
    std::memset(&xform, 0, sizeof(xform));
    xform.FromInput = FakeFromInput;
    xform.ToOutput   = FakeToOutput;
    std::memset(&xform.Cache, 0, sizeof(xform.Cache));

    const cmsUInt32Number PixelsPerLine = 3;
    const cmsUInt32Number LineCount = 2; // two lines
    unsigned char in[6 * 1];
    unsigned char out[6 * 1];
    std::memset(in, 0, sizeof(in));
    std::memset(out, 0, sizeof(out));

    cmsStride s;
    s.BytesPerPlaneIn  = 1;
    s.BytesPerLineIn   = PixelsPerLine * s.BytesPerPlaneIn;
    s.BytesPerPlaneOut = 1;
    s.BytesPerLineOut  = PixelsPerLine * s.BytesPerPlaneOut;

    // Reset deterministic FromInput sequence
    ResetFromInputCounter();
    gFromInputCallCount = 0;

    // Call the function under test
    CachedXFORMGamutCheck(&xform, in, out, PixelsPerLine, LineCount, &s);

    // Expected outputs: [0,6,5,6,5,6]
    unsigned char expected[6] = {0, 6, 5, 6, 5, 6};
    for (int i = 0; i < 6; ++i) {
        if (out[i] != expected[i]) {
            std::cerr << "Test 2 failed at index " << i
                      << ": expected " << int(expected[i])
                      << ", got " << int(out[i]) << "\n";
            return false;
        }
    }
    return true;
}

// Minimal test runner
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running tests for CachedXFORMGamutCheck (in-test harness)...\n";

    total++;
    if (Test_CachedXFORMGamutCheck_BasicCaching_SingleLine()) {
        std::cout << "Test 1 (SingleLine basic caching) PASSED\n";
        passed++;
    } else {
        std::cout << "Test 1 (SingleLine basic caching) FAILED\n";
    }

    total++;
    if (Test_CachedXFORMGamutCheck_BasicCaching_TwoLines()) {
        std::cout << "Test 2 (TwoLines caching) PASSED\n";
        passed++;
    } else {
        std::cout << "Test 2 (TwoLines caching) FAILED\n";
    }

    std::cout << "Tests passed: " << passed << " / " << total << "\n";

    return (passed == total) ? 0 : 1;
}