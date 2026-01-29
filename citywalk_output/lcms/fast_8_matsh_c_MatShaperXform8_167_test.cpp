#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <cstdint>


// This test harness provides a minimal, self-contained environment to exercise
// the focal function MatShaperXform8 from fast_8_matsh.c without requiring the
// actual LittleCMS runtime. It stubs the necessary runtime hooks in a way that
// preserves the intended control flow and numeric behavior of the target function.

// ----------------------------------------------------------------------------
// Lightweight type and proto definitions to match the focal code's expectations
// ----------------------------------------------------------------------------

typedef uint32_t cmsUInt32Number;
typedef uint8_t  cmsUInt8Number;
typedef int32_t  cmsS1Fixed14Number;

#define cmsMAXCHANNELS 4
#define cmsFLAGS_COPY_ALPHA 0x04

// Forward declare the reshape/cargo types used by MatShaperXform8
struct _cmstransform_struct;

// Core data structure used by the transform (matching the class dependency in the source)
struct XMatShaper8Data {
    cmsUInt8Number Shaper1R[256];
    cmsUInt8Number Shaper1G[256];
    cmsUInt8Number Shaper1B[256];

    cmsUInt8Number Shaper2R[256];
    cmsUInt8Number Shaper2G[256];
    cmsUInt8Number Shaper2B[256];

    cmsS1Fixed14Number Mat[4][3]; // 4x3 matrix (as used in the code)
};

// The per-line stride descriptor (as used by MatShaperXform8)
struct cmsStride {
    cmsUInt32Number BytesPerPlaneIn;
    cmsUInt32Number BytesPerPlaneOut;
    cmsUInt32Number BytesPerLineIn;
    cmsUInt32Number BytesPerLineOut;
};

// Opaque transform object (only its pointer is passed around)
struct _cmstransform_struct {};

// Public function prototype of the focal method (to be linked from fast_8_matsh.c)
extern "C" void MatShaperXform8(struct _cmstransform_struct *CMMcargo,
                                const void* Input,
                                void* Output,
                                cmsUInt32Number PixelsPerLine,
                                cmsUInt32Number LineCount,
                                const cmsStride* Stride);

// -----------------------------------------------------------------------------
// Stubs to emulate the required LittleCMS runtime hooks for the focal code
// -----------------------------------------------------------------------------

// Global state to drive the tests
static XMatShaper8Data gP;                 // The user data for the transform (p)
static bool        gFourChannelsTest = false; // true => test RGBA path
static bool        gReturnCopyAlpha = false;  // true => _cmsGetTransformFlags returns COPY_ALPHA

// Small opaque _cmstransform_struct instance used as the cargo handle
static _cmstransform_struct gCMMcargo;

// Expose only C linkage for the runtime hooks so the C file can link cleanly
extern "C" {

// Return the test-provided user data (p)
void* _cmsGetTransformUserData(struct _cmstransform_struct* /*CMMcargo*/) {
    return static_cast<void*>(&gP);
}

// Populate component increment tables. This is a simplified stand-in for the real
// _cmsComputeComponentIncrements used by the focal code. It supports up to 4 channels.
void _cmsComputeComponentIncrements(cmsUInt32Number /*InputFormat*/,
                                    size_t /*BytesPerPlaneIn*/,
                                    void* /*p*/,
                                    cmsUInt32Number* nalpha,
                                    cmsUInt32Number SourceStartingOrder[/*4*/],
                                    cmsUInt32Number SourceIncrements[/*4*/]) {
    // Default to 3-channel (RGB) when not in four-channel test
    if (gFourChannelsTest) {
        // Interleaved RGBA
        SourceStartingOrder[0] = 0;
        SourceStartingOrder[1] = 1;
        SourceStartingOrder[2] = 2;
        SourceStartingOrder[3] = 3;

        // Each pixel has 4 bytes; advance by 4 per component
        SourceIncrements[0] = 4;
        SourceIncrements[1] = 4;
        SourceIncrements[2] = 4;
        SourceIncrements[3] = 4;

        // Indicate alpha channel exists
        if (nalpha) *nalpha = 1;
    } else {
        SourceStartingOrder[0] = 0;
        SourceStartingOrder[1] = 1;
        SourceStartingOrder[2] = 2;
        // Fourth channel placeholder unused in 3-channel tests

        SourceIncrements[0] = 3;
        SourceIncrements[1] = 3;
        SourceIncrements[2] = 3;

        if (nalpha) *nalpha = 0;
    }

    // Destination starting order mirrors input in this simplified stub
    // (the actual library may consider different permutations; for testing, map 0..n-1)
    // This function's original signature yields destination orders via DestStartingOrder
    // but since the destination arrays are indexed with DestStartingOrder in the caller,
    // we only need to ensure the increments are correct here. The test uses 0..N-1.
}

// Return the transform input format (stubbed)
cmsUInt32Number cmsGetTransformInputFormat(struct _cmstransform_struct* /*CMMcargo*/) {
    return 0;
}

// Return the transform output format (stubbed)
cmsUInt32Number cmsGetTransformOutputFormat(struct _cmstransform_struct* /*CMMcargo*/) {
    return 0;
}

// Return transform flags. We toggle the COPY_ALPHA bit via gReturnCopyAlpha
cmsUInt32Number _cmsGetTransformFlags(struct _cmstransform_struct* /*CMMcargo*/) {
    return gReturnCopyAlpha ? cmsFLAGS_COPY_ALPHA : 0;
}
} // extern "C"

// -----------------------------------------------------------------------------
// Test harness helpers
// -----------------------------------------------------------------------------

static void init_identity_p() {
    // Identity mapping for first stage (R->R, G->G, B->B)
    for (int i = 0; i < 256; ++i) {
        gP.Shaper1R[i] = (cmsUInt8Number)i;
        gP.Shaper1G[i] = (cmsUInt8Number)i;
        gP.Shaper1B[i] = (cmsUInt8Number)i;

        gP.Shaper2R[i] = (cmsUInt8Number)i;
        gP.Shaper2G[i] = (cmsUInt8Number)i;
        gP.Shaper2B[i] = (cmsUInt8Number)i;
    }

    // Identity 1.0 matrix in 1.14 fixed-point (0x4000 == 1.0)
    // Column 0 (R' = 1*R)
    gP.Mat[0][0] = 0x4000;
    gP.Mat[1][0] = 0;
    gP.Mat[2][0] = 0;
    gP.Mat[3][0] = 0;

    // Column 1 (G' = 1*G)
    gP.Mat[0][1] = 0;
    gP.Mat[1][1] = 0x4000;
    gP.Mat[2][1] = 0;
    gP.Mat[3][1] = 0;

    // Column 2 (B' = 1*B)
    gP.Mat[0][2] = 0;
    gP.Mat[1][2] = 0;
    gP.Mat[2][2] = 0x4000;
    gP.Mat[3][2] = 0;
}

// Simple byte-wise comparison utility
static bool bytes_equal(const void* a, const void* b, size_t n) {
    return std::memcmp(a, b, n) == 0;
}

// Assertion helper that logs but does not abort the test (non-terminating)
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { \
    std::cerr << "Test assertion failed: " << (msg) << " (line " << __LINE__ << ")\n"; \
    ++g_failures; \
} } while(0)

// Test 1: RGB input, 2 pixels, 3 channels, identity mapping (no alpha)
// Expect Output == Input
static void test_rgb_identity() {
    gFourChannelsTest = false;
    gReturnCopyAlpha = false;
    init_identity_p();

    // Prepare input: 2 pixels, RGB
    cmsUInt8Number Input[6]  = { 10, 20, 30, 40, 50, 60 };
    cmsUInt8Number Output[6] = { 0, 0, 0, 0, 0, 0 };

    cmsStride Stride;
    Stride.BytesPerPlaneIn  = 0;
    Stride.BytesPerPlaneOut = 0;
    Stride.BytesPerLineIn   = 0;
    Stride.BytesPerLineOut  = 0;

    struct _cmstransform_struct CMMtmp;
    // Use global CMM cargo (gCMMcargo) but pass a pointer to a local one as the interface expects
    // The stub _cmsGetTransformUserData will return gP regardless of the actual cargo pointer
    (void) &CMMtmp; // silence unused-warning in some compilers

    MatShaperXform8(&CMMtmp, Input, Output, 2, 1, &Stride);

    // Validate: Output should equal Input (2 RGB pixels)
    TEST_ASSERT(bytes_equal(Output, Input, sizeof(Input)),
                "RGB identity: Output should match Input for 2 RGB pixels");
}

// Test 2: RGBA input, 1 pixel, 4 channels, alpha should be copied (identity mapping for colors)
// Enable 4-channel path and COPY_ALPHA flag
static void test_rgba_alpha_copy() {
    gFourChannelsTest = true;
    gReturnCopyAlpha = true;
    init_identity_p();

    // Prepare input: 1 pixel, RGBA
    cmsUInt8Number Input[4]  = { 5, 6, 7, 128 };
    cmsUInt8Number Output[4] = { 0, 0, 0, 0 };

    cmsStride Stride;
    Stride.BytesPerPlaneIn  = 0;
    Stride.BytesPerPlaneOut = 0;
    Stride.BytesPerLineIn   = 0;
    Stride.BytesPerLineOut  = 0;

    struct _cmstransform_struct CMMtmp;
    (void) &CMMtmp; // silence unused-warning in some compilers

    MatShaperXform8(&CMMtmp, Input, Output, 1, 1, &Stride);

    // Validate: Color channels preserved (identity mapping) and alpha preserved in A channel
    bool colorsMatch = (Output[0] == Input[0]) && (Output[1] == Input[1]) && (Output[2] == Input[2]);
    bool alphaMatch  = (Output[3] == Input[3]);
    TEST_ASSERT(colorsMatch, "RGBA alpha copy: Red channel preserved");
    TEST_ASSERT(Output[1] == Input[1] && Output[2] == Input[2], "RGBA alpha copy: Green/Blue channels preserved");
    TEST_ASSERT(alphaMatch, "RGBA alpha copy: Alpha channel preserved");
}

// ----------------------------------------------------------------------------
// Main: run tests and report summary
// ----------------------------------------------------------------------------

int main() {
    // Initialize the transform data to a known identity state
    // Note: Each test reinitializes as needed

    test_rgb_identity();
    test_rgba_alpha_copy();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << g_failures << " test(s) failed.\n";
        return 1;
    }
}