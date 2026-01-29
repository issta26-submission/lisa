// High-quality C++11 unit tests for the focal function NullFloatXFORM
// This test suite is designed to be compiled with the project's C/C++ build
// environment (no GTest). It uses a lightweight in-file test harness with
// non-terminating expectations to maximize code coverage.
//
// Assumptions and approach:
// - The focal function NullFloatXFORM is defined in cmsxform.c and uses internal
//   types defined by the project (e.g., _cmsTRANSFORM, cmsStride, cmsUInt32Number, etc.).
// - The test code uses an incomplete forward declaration for _cmsTRANSFORM to
//   avoid needing the full private structure layout. We only pass a pointer to
//   the function and provide compatible FromInputFloat/ToOutputFloat function
//   pointers via a real _cmsTRANSFORM instance as defined by the project headers.
// - We include the project's internal headers to obtain the actual type definitions
//   for cmsStride and the function pointer types. If your environment uses different
//   headers, adjust the include accordingly (e.g., #include "lcms2_internal.h").
//
// The tests focus on:
 // 1) Basic execution path with a few lines and a few pixels per line.
// 2) Edge cases such as zero PixelsPerLine and zero LineCount.
// 3) Verification that the FromInputFloat and ToOutputFloat hooks are actually invoked
//    the expected number of times for the normal path.
//
// Note: We rely on the function being externally visible (non-static). If your build
// exposes it differently, remove extern "C" wrapping accordingly.

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific types and function prototypes.
// These definitions mirror the project-friendly types. If your environment
// provides a different header, include it here instead and remove local typedefs.

typedef unsigned int cmsUInt32Number;
typedef unsigned char cmsUInt8Number;
typedef float cmsFloat32Number;

// Forward declaration of the internal transform type to avoid needing the complete
// private layout in the test. We only need a pointer to this type to call the function.
struct _cmsTRANSFORM;

// Forward declare the focal function (exported from cmsxform.c)
extern "C" void NullFloatXFORM(_cmsTRANSFORM* p,
                             const void* in,
                             void* out,
                             cmsUInt32Number PixelsPerLine,
                             cmsUInt32Number LineCount,
                             const struct cmsStride* Stride);

// cmsStride type used by the focal method.
// If your project exposes cmsStride via a header, include it instead.
struct cmsStride {
    cmsUInt32Number BytesPerPlaneIn;
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerPlaneOut;
    cmsUInt32Number BytesPerLineOut;
};

// Lightweight test harness: non-terminating assertions
static int gFailures = 0;
static int gCallCount = 0;

// Simple expectation macros (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed at " << __LINE__ << std::endl; \
        ++gFailures; \
    } \
} while (0)

#define EXPECT_FALSE(cond) do { \
    if (cond) { \
        std::cerr << "EXPECT_FALSE failed at " << __LINE__ << std::endl; \
        ++gFailures; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed at " << __LINE__ << ": " \
                  << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")" << std::endl; \
        ++gFailures; \
    } \
} while (0)

#define EXPECT_NE(a, b) do { \
    if ((a) == (b)) { \
        std::cerr << "EXPECT_NE failed at " << __LINE__ << ": " \
                  << #a << " (" << (a) << ") == " << #b << " (" << (b) << ")" << std::endl; \
        ++gFailures; \
    } \
} while (0)

// Test helpers: fake transformer hooks
extern "C" {

// Incremental counter to verify number of pixel processing calls
static void reset_call_counter() { gCallCount = 0; }

// Fake FromInputFloat: pretend we read a pixel from input and advance the accumulator
// Signature assumed to match the project's internal function pointer type.
// We keep it simple: always advance by BytesPerPlaneIn.
static cmsUInt8Number* FakeFromInputFloat(_cmsTRANSFORM* /*p*/,
                                          cmsFloat32Number /*fIn*/[],
                                          cmsUInt8Number* accum,
                                          cmsUInt32Number BytesPerPlaneIn)
{
    // Count how many times a pixel was processed
    ++gCallCount;
    // Move the accum pointer forward by the size of one input plane
    return accum + static_cast<size_t>(BytesPerPlaneIn);
}

// Fake ToOutputFloat: write a recognizable signature to the output plane
static cmsUInt8Number* FakeToOutputFloat(_cmsTRANSFORM* /*p*/,
                                        cmsFloat32Number /*fIn*/[],
                                        cmsUInt8Number* output,
                                        cmsUInt32Number BytesPerPlaneOut)
{
    // Fill the output plane with a recognizable byte pattern
    for (cmsUInt32Number i = 0; i < BytesPerPlaneOut; ++i)
        output[i] = 0xAB; // arbitrary marker
    return output + static_cast<size_t>(BytesPerPlaneOut);
}
} // extern "C"
}

// Test 1: Basic execution path with small image (non-zero pixels per line and lines)
static void test_NullFloatXFORM_BasicPath() {
    // Reset counters
    reset_call_counter();

    // Prepare a Minimal Transform instance with our hooks.
    // We rely on the public/internal header to define _cmsTRANSFORM members.
    _cmsTRANSFORM xform = {};
    // The actual field names come from the project's internal header.
    // We assume the following common names exist (FromInputFloat / ToOutputFloat).
    // If your environment uses different names, adjust accordingly.
    xform.FromInputFloat = FakeFromInputFloat;
    xform.ToOutputFloat   = FakeToOutputFloat;

    // Prepare buffers
    const cmsUInt32Number PixelsPerLine = 4;
    const cmsUInt32Number LineCount = 3;

    // Bytes per plane in/out
    const cmsUInt32Number BytesPerPlaneIn  = 2; // 2 bytes per input pixel
    const cmsUInt32Number BytesPerPlaneOut = 3; // 3 bytes per output pixel

    // Stride configuration
    cmsStride Stride = {};
    Stride.BytesPerPlaneIn  = BytesPerPlaneIn;
    Stride.BytesPerLineIn   = PixelsPerLine * BytesPerPlaneIn;    // 4 * 2 = 8
    Stride.BytesPerPlaneOut = BytesPerPlaneOut;
    Stride.BytesPerLineOut  = PixelsPerLine * BytesPerPlaneOut;   // 4 * 3 = 12

    // Allocate input and output buffers
    std::vector<uint8_t> inBuf(LineCount * Stride.BytesPerLineIn, 0);      // initialized to 0
    std::vector<uint8_t> outBuf(LineCount * Stride.BytesPerLineOut, 0);    // initialized to 0

    // Call the focal function
    NullFloatXFORM(&xform,
                   inBuf.data(),
                   outBuf.data(),
                   PixelsPerLine,
                   LineCount,
                   &Stride);

    // Validate expectations:
    // - The FromInputFloat should be invoked exactly PixelsPerLine * LineCount times
    EXPECT_EQ(static_cast<cmsUInt32Number>(gCallCount), PixelsPerLine * LineCount);
    // - Each ToOutputFloat invocation should have filled an entire output plane with 0xAB
    //   Since each inner loop writes a plane of size BytesPerPlaneOut, the entire output buffer should be 0xAB
    bool allAB = true;
    for (size_t i = 0; i < outBuf.size(); ++i) {
        if (outBuf[i] != 0xAB) { allAB = false; break; }
    }
    EXPECT_TRUE(allAB);
}

// Test 2: Edge case with zero PixelsPerLine (no processing should occur, but function should not crash)
static void test_NullFloatXFORM_ZeroPixels() {
    // Reset counters
    reset_call_counter();

    _cmsTRANSFORM xform = {};
    xform.FromInputFloat = FakeFromInputFloat;
    xform.ToOutputFloat   = FakeToOutputFloat;

    // Zero pixels per line scenario
    const cmsUInt32Number PixelsPerLine = 0;
    const cmsUInt32Number LineCount = 5;

    const cmsUInt32Number BytesPerPlaneIn  = 2;
    const cmsUInt32Number BytesPerPlaneOut = 3;

    cmsStride Stride = {};
    Stride.BytesPerPlaneIn  = BytesPerPlaneIn;
    Stride.BytesPerLineIn   = PixelsPerLine * BytesPerPlaneIn;      // 0
    Stride.BytesPerPlaneOut = BytesPerPlaneOut;
    Stride.BytesPerLineOut  = PixelsPerLine * BytesPerPlaneOut;     // 0

    // Buffers exist, but with zero logical processing
    std::vector<uint8_t> inBuf(1, 0);  // at least 1 byte to be safe
    std::vector<uint8_t> outBuf(1, 0);

    NullFloatXFORM(&xform,
                   inBuf.data(),
                   outBuf.data(),
                   PixelsPerLine,
                   LineCount,
                   &Stride);

    // No pixel processed should mean gCallCount remains 0
    EXPECT_EQ(static_cast<cmsUInt32Number>(gCallCount), 0);
}

// Test 3: Edge case with LineCount = 0 (no processing; ensure no crash)
static void test_NullFloatXFORM_ZeroLines() {
    reset_call_counter();

    _cmsTRANSFORM xform = {};
    xform.FromInputFloat = FakeFromInputFloat;
    xform.ToOutputFloat   = FakeToOutputFloat;

    const cmsUInt32Number PixelsPerLine = 4;
    const cmsUInt32Number LineCount = 0;

    const cmsUInt32Number BytesPerPlaneIn  = 2;
    const cmsUInt32Number BytesPerPlaneOut = 3;

    cmsStride Stride = {};
    Stride.BytesPerPlaneIn  = BytesPerPlaneIn;
    Stride.BytesPerLineIn   = PixelsPerLine * BytesPerPlaneIn;
    Stride.BytesPerPlaneOut = BytesPerPlaneOut;
    Stride.BytesPerLineOut  = PixelsPerLine * BytesPerPlaneOut;

    std::vector<uint8_t> inBuf(Stride.BytesPerLineIn * LineCount, 0);
    std::vector<uint8_t> outBuf(Stride.BytesPerLineOut * LineCount, 0);

    NullFloatXFORM(&xform,
                   inBuf.data(),
                   outBuf.data(),
                   PixelsPerLine,
                   LineCount,
                   &Stride);

    EXPECT_EQ(static_cast<cmsUInt32Number>(gCallCount), 0);
}

// Driver: run all tests and report summary
int main() {
    std::cout << "Running NullFloatXFORM test suite (non-terminating assertions)..." << std::endl;

    test_NullFloatXFORM_BasicPath();
    test_NullFloatXFORM_ZeroPixels();
    test_NullFloatXFORM_ZeroLines();

    if (gFailures == 0) {
        std::cout << "All tests passed (non-terminating assertions)." << std::endl;
        return 0;
    } else {
        std::cout << gFailures << " test(s) failed." << std::endl;
        return 1;
    }
}