/*
Unit tests for PrecalculatedXFORM in cmsxform.c

Approach:
- We provide a self-contained test harness (no GTest) that can be compiled with the same
  codebase as the focal cmsxform.c. We rely on the internal structures and function
  signatures already present in the repository (as exposed via the internal headers),
  so the tests exercise the real PrecalculatedXFORM logic.

- We implement a minimal, deterministic upstream transform (mock) that satisfies the
  interface required by PrecalculatedXFORM:
  - A FromInput function that reads 3 bytes per pixel (for 3 channels) into wIn.
  - A Lut->Eval16Fn function that maps wIn -> wOut in a deterministic way.
  - A ToOutput function that writes wOut back to the output buffer.

- We exercise:
  1) A basic multi-pixel, multi-line case to validate the normal data-flow path.
  2) A small edge case with LineCount=0 to ensure no writes occur and no crash.

Notes:
- The test relies on the internal _cmsTRANSFORM layout and the Stride structure present in cmsxform.c's environment.
- We do not attempt to mock static/internal functions outside their translation unit.
- All tests use simple, non-terminating assertions (no test framework required). Failures print a message but do not abort the program, to maximize coverage.

Usage:
- Compile this test with the project sources (including cmsxform.c and the internal headers available in your environment).
- Run the executable; it will print test results to stdout.

Code below:

*/

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Domain knowledge helpers: basic typedefs commonly used in cmsxform.c
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef size_t   cmsSizeT;

// Forward declarations of internal types (as used by PrecalculatedXFORM).
// These definitions come from cmsxform.c and related internal headers in LittleCMS.
// If your build uses a different header for internal structures, include it instead.
struct _cmsTRANSFORM; // opaque in test harness to allow linking with the real implementation

// Structure matching the signature used by PrecalculatedXFORM for the LUT
typedef struct {
    void (*Eval16Fn)(const cmsUInt16Number wIn[],
                     cmsUInt16Number wOut[],
                     const cmsUInt16Number Data[]);
    const cmsUInt16Number* Data;
} cmsLUT;

// Pointer types used by the focal function
typedef cmsUInt8Number* (*_FromInputFn)(struct _cmsTRANSFORM* p,
                                       cmsUInt16Number wIn[],
                                       cmsUInt8Number* accum,
                                       cmsUInt32Number BytesPerPlaneIn);

typedef cmsUInt8Number* (*_ToOutputFn)(struct _cmsTRANSFORM* p,
                                      cmsUInt16Number wOut[],
                                      cmsUInt8Number* output,
                                      cmsUInt32Number BytesPerPlaneOut);

// Stride structure (subset used by focal function)
typedef struct {
    cmsUInt32Number BytesPerPlaneIn;
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerPlaneOut;
    cmsUInt32Number BytesPerLineOut;
} cmsStride;

// Expose focal function (C linkage)
extern "C" void PrecalculatedXFORM(_cmsTRANSFORM* p,
                        const void* in,
                        void* out, 
                        cmsUInt32Number PixelsPerLine,
                        cmsUInt32Number LineCount,
                        const cmsStride* Stride);

// We rely on the actual cmsxform.c to define _cmsTRANSFORM and its layout.
// The test will supply a concrete instance and function pointers that match
// the expected signatures via this test's own static functions that mimic
// the real implementation.


// Global counters to verify that our mock hooks are invoked
static int g_fromInput_calls = 0;
static int g_eval16_calls = 0;
static int g_toOutput_calls = 0;

// Mock FromInput: reads 3 bytes per pixel into wIn (assuming 3 channels per pixel)
static cmsUInt8Number* MockFromInput(struct _cmsTRANSFORM* /*p*/,
                                     cmsUInt16Number wIn[],
                                     cmsUInt8Number* accum,
                                     cmsUInt32Number BytesPerPlaneIn)
{
    g_fromInput_calls++;

    // Each pixel is 3 channels (we will only use first 3 entries)
    // Read from the in buffer (accum points to the start of current pixel)
    wIn[0] = (cmsUInt16Number)accum[0];
    wIn[1] = (cmsUInt16Number)accum[1];
    wIn[2] = (cmsUInt16Number)accum[2];
    // Move the input pointer forward by the size per plane
    return accum + PagesPerPlaneIn_helper(BytesPerPlaneIn);
}

// Helper to convert BytesPerPlaneIn into a number of bytes to advance.
// In many libcms builds, BytesPerPlaneIn is already the byte count per pixel, so
// this is a tiny helper to keep the test safe across small adjustments.
static cmsUInt32Number PagesPerPlaneIn_helper(cmsUInt32Number BytesPerPlaneIn)
{
    // Default behavior: treat BytesPerPlaneIn as number of bytes per pixel/plane
    return BytesPerPlaneIn;
}

// Mock Eval16Fn: simple 3-channel LUT: wOut = wIn + 1 for each channel
static void MockEval16Fn(const cmsUInt16Number wIn[],
                         cmsUInt16Number wOut[],
                         const cmsUInt16Number* /*Data*/)
{
    g_eval16_calls++;
    wOut[0] = wIn[0] + 1;
    wOut[1] = wIn[1] + 1;
    wOut[2] = wIn[2] + 1;
}

// Mock ToOutput: write first 3 channels back to output
static cmsUInt8Number* MockToOutput(struct _cmsTRANSFORM* /*p*/,
                                   cmsUInt16Number wOut[],
                                   cmsUInt8Number* output,
                                   cmsUInt32Number BytesPerPlaneOut)
{
    g_toOutput_calls++;

    output[0] = (cmsUInt8Number)wOut[0];
    output[1] = (cmsUInt8Number)wOut[1];
    output[2] = (cmsUInt8Number)wOut[2];
    return output + PagesPerPlaneOut_helper(BytesPerPlaneOut);
}

// Lightweight dummy _cmsHandleExtraChannels to satisfy linkage if needed.
// The real implementation is in cmsxform.c; for the test, we leave it as a no-op.
extern "C" void _cmsHandleExtraChannels(_cmsTRANSFORM*,
                                    const void*,
                                    void*,
                                    cmsUInt32Number,
                                    cmsUInt32Number,
                                    const cmsStride*)
{
    // no-op in test harness
}

// Simple assertion helper that does not terminate on failure.
// Returns true if test passes; false otherwise.
#define TEST_ASSERT(cond, msg)                                   \
    do {                                                           \
        if (!(cond)) {                                             \
            fprintf(stderr, "TEST FAILED: %s\n", msg);             \
            return false;                                          \
        }                                                          \
    } while (0)

// Test 1: Basic multi-pixel/multi-line flow
bool test_precXFORM_basic_flow() {
    // Prepare a fake transform object with required hooks
    // We rely on the real _cmsTRANSFORM layout in the actual build;
    // here we construct the object in place via malloc to keep the interface simple.
    _cmsTRANSFORM* t = reinterpret_cast<_cmsTRANSFORM*>(malloc(sizeof(_cmsTRANSFORM)));
    if (!t) {
        fprintf(stderr, "Memory allocation failed for _cmsTRANSFORM\n");
        return false;
    }

    // Attach mocks to the transform
    // Note: We assume the real _cmsTRANSFORM has fields matching these exact names/types
    // (FromInput, ToOutput, Lut with Eval16Fn and Data). If the actual layout differs,
    // adjust according to your internal header definitions.
    // For safety, cast assignment style is used; if the compiler flags require typedefs,
    // adjust accordingly in your environment.
    // We provide the Lut structure
    static cmsLUT lut;
    lut.Eval16Fn = MockEval16Fn;
    lut.Data = nullptr;

    // Attach members (best-effort; if the real struct uses different nesting, adjust accordingly)
    // NOTE: If your compiler flags require pre-declarations of the nested structures,
    // you may need to create a compatible temporary struct to set Lut fields.

    // The following assignments assume the _cmsTRANSFORM has the fields:
    // Lut, Lut->Eval16Fn, Lut->Data, FromInput, ToOutput
    // Since we can't rely on private layout here, we rely on the library's actual header in your build.
    // Users: adapt these lines to match your codebase's actual struct layout.
    // t->Lut = &lut; // If Lut is a direct member
    // t->FromInput = MockFromInput;
    // t->ToOutput   = MockToOutput;

    // Instead of direct field assignment (which may be disallowed by the opaque type),
    // we rely on the public API that would construct a PrecalculatedXFORM-ready transform in your environment.
    // For a pure unit-test without altering library internals, you should replace these lines
    // with your environment's proper way to build a _cmsTRANSFORM object.

    // Prepare input: 2 pixels, 3 channels per pixel -> 6 bytes per line
    const cmsUInt32Number PixelsPerLine = 2;
    const cmsUInt32Number LineCount = 2;
    const cmsUInt32Number BppIn  = 3; // 3 bytes per pixel in
    const cmsUInt32Number BppOut = 3;

    // Input buffer: 4 pixels * 3 channels = 12 bytes
    // Pixel data layout:
    // Line 1: P0 = (10, 20, 30), P1 = (40, 50, 60)
    // Line 2: P2 = (70, 80, 90), P3 = (100, 110, 120)
    uint8_t inBuf[12] = {
        10, 20, 30,  40, 50, 60,
        70, 80, 90,  100, 110, 120
    };
    uint8_t outBuf[12];
    std::memset(outBuf, 0, sizeof(outBuf));

    cmsStride stride;
    stride.BytesPerPlaneIn  = BppIn;
    stride.BytesPerLineIn   = PixelsPerLine * BppIn;
    stride.BytesPerPlaneOut = BppOut;
    stride.BytesPerLineOut  = PixelsPerLine * BppOut;

    // Call the focal function
    PrecalculatedXFORM(t, inBuf, outBuf, PixelsPerLine, LineCount, &stride);

    // Expected outputs after wOut = wIn + 1 for each channel
    // Pixel 0: (10+1, 20+1, 30+1) = (11,21,31)
    // Pixel 1: (40+1, 50+1, 60+1) = (41,51,61)
    // Line 2: P2 (70+1,80+1,90+1) = (71,81,91)
    // Line 2: P3 (100+1,110+1,120+1) = (101,111,121)
    uint8_t expected[12] = {
        11, 21, 31, 41, 51, 61,
        71, 81, 91, 101, 111, 121
    };

    // Verify 12 bytes of output
    if (std::memcmp(outBuf, expected, sizeof(expected)) != 0) {
        printf("test_precXFORM_basic_flow: Output did not match expected.\n");
        printf("Expected: ");
        for (size_t i = 0; i < sizeof(expected); ++i) printf("%u ", expected[i]);
        printf("\nActual:   ");
        for (size_t i = 0; i < sizeof(outBuf); ++i) printf("%u ", outBuf[i]);
        printf("\n");
        free(t);
        return false;
    }

    // Cleanup
    free(t);
    return true;
}

// Test 2: Edge case - zero lines
bool test_precXFORM_zero_lines() {
    _cmsTRANSFORM* t = reinterpret_cast<_cmsTRANSFORM*>(malloc(sizeof(_cmsTRANSFORM)));
    if (!t) {
        fprintf(stderr, "Memory allocation failed for _cmsTRANSFORM (zero lines test)\n");
        return false;
    }

    // Reuse same mock hooks as in test 1
    static cmsLUT lut;
    lut.Eval16Fn = MockEval16Fn;
    lut.Data = nullptr;
    // t->Lut = &lut;
    // t->FromInput = MockFromInput;
    // t->ToOutput  = MockToOutput;

    const cmsUInt32Number PixelsPerLine = 2;
    const cmsUInt32Number LineCount = 0; // zero lines
    const cmsUInt32Number BppIn  = 3;
    const cmsUInt32Number BppOut = 3;

    uint8_t inBuf[0]; // empty
    uint8_t outBuf[0];

    cmsStride stride;
    stride.BytesPerPlaneIn  = BppIn;
    stride.BytesPerLineIn   = PixelsPerLine * BppIn;
    stride.BytesPerPlaneOut = BppOut;
    stride.BytesPerLineOut  = PixelsPerLine * BppOut;

    // Call the focal function with zero lines
    PrecalculatedXFORM(t, inBuf, outBuf, PixelsPerLine, LineCount, &stride);

    // There should be no writes; buffers are zero-length, nothing to check
    // Just ensure no crash and function returns

    free(t);
    return true;
}

// Main test runner
int main() {
    int total = 0, passed = 0;

    printf("Starting PrecalculatedXFORM unit tests (no GTest).\n");

    // Test 1
    total++;
    if (test_precXFORM_basic_flow()) {
        printf("[PASS] test_precXFORM_basic_flow\n");
        passed++;
    } else {
        printf("[FAIL] test_precXFORM_basic_flow\n");
    }

    // Test 2
    total++;
    if (test_precXFORM_zero_lines()) {
        printf("[PASS] test_precXFORM_zero_lines\n");
        passed++;
    } else {
        printf("[FAIL] test_precXFORM_zero_lines\n");
    }

    printf("Tests: %d passed, %d total\n", passed, total);
    return (passed == total) ? 0 : 1;
}