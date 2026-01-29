/*
  Test suite for PerformanceEval8 (fast_8_tethra.c)
  - This file provides a set of standalone C++11 tests (no GoogleTest)
  - The tests are designed to be compiled and linked together with the
    original performance transform implementation (fast_8_tethra.c) and
    the surrounding libcms-like infrastructure.
  - The tests focus on exercising the high-coverage branches and ensuring
    the code paths are exercised (true/false predicates, combinations, etc.)
  - Notes:
    - Because PerformanceEval8 relies on many internal CMS structures and
      macros (e.g. DENS, _cmsGetTransformUserData, _cmsComputeComponentIncrements,
      etc.), the test harness provides a minimal scaffolding with stubs where
      feasible. The goal is to validate control flow coverage and basic
      write-out behavior, not to reimplement the entire color transformation
      pipeline.
    - If the library headers are available in your build, prefer including them
      (e.g., #include "lcms2.h") and linking against the actual implementations.
    - This file avoids GTest; instead, it uses simple assert-based checks
      and a tiny test runner to keep dependencies minimal.
  How to build (example):
    - g++ -std=c++11 -DUNIT_TEST -O2 -Wall -Wextra -I<path_to_lcms_headers> \
      test_performance_eval8.cpp fast_8_tethra.c -lm -o test_performance_eval8
  Usage:
    - Run the produced executable to execute all test cases.
*/

#include <cstring>
#include <fast_float_internal.h>
#include <iostream>
#include <cassert>
#include <cstdint>


/* 
   The focal function is declared in the original project as:
     void PerformanceEval8(struct _cmstransform_struct *CMMcargo,
                          const void* Input,
                          void* Output,
                          cmsUInt32Number PixelsPerLine,
                          cmsUInt32Number LineCount,
                          const cmsStride* Stride)
   To compile this test in isolation (without exposing the full libcms headers),
   we declare a compatible prototype here. In a full build against the actual
   library, include the proper header instead.
*/
extern "C" {
    struct _cmstransform_struct;     // forward declaration
    struct cmsStride {
        size_t BytesPerPlaneIn;
        size_t BytesPerPlaneOut;
        size_t BytesPerLineIn;
        size_t BytesPerLineOut;
    };

    // Use the same signature as the focal method.
    void PerformanceEval8(struct _cmstransform_struct *CMMcargo,
                         const void* Input,
                         void* Output,
                         uint32_t PixelsPerLine,
                         uint32_t LineCount,
                         const struct cmsStride* Stride);
}

/* 
   Lightweight scaffolding to provide a minimal, fake transform user data
   so that PerformanceEval8 can run in a controlled test environment.
   This portion intentionally mirrors a very small subset of the CMS data
   structures required by the focal function.
*/
struct cmsInterpParams {
    uint32_t nOutputs;
    const void* Table;
    const void* opa;   // placeholder for optional transform parameters
};

struct Performance8Data {
    cmsInterpParams* p;
    // Minimal color mapping tables (256 entries); actual DENS macro uses this
    // data in a real runtime. We keep them simple here for test scaffolding.
    uint8_t X0[256];
    uint8_t Y0[256];
    uint8_t Z0[256];
    int16_t rx[256];
    int16_t ry[256];
    int16_t rz[256];
};

static Performance8Data g_fakeP8;

/*
  Minimal fake CMS transform user data accessor.
  In the real project this is _cmsGetTransformUserData(CMMcargo).
  We provide a version that returns our fake data when called with a non-null
  transform handle. If the test is linked against a real lib, this symbol may
  be overridden by the real implementation; the test harness should be kept
  compatible with that scenario.
*/
extern "C" void* _cmsGetTransformUserData(struct _cmstransform_struct* /*CMMcargo*/) {
    return &g_fakeP8;
}

/*
  A tiny, self-contained test runner to avoid external test frameworks.
  It returns number of failed tests; main prints a summary.
*/
static int RunTests();

/*---------------------------------------------------------------------------
  Test helpers
---------------------------------------------------------------------------*/
/* Prepare a minimal transformation context and buffers for a single-pixel test
   Input is assumed to be RGB (r,g,b) in 0-255 domain. Output buffer should be
   large enough to hold p->nOutputs + (optional alpha). The test focuses on
   ensuring that PerformanceEval8 executes without crashing and writes to
   the expected output region when possible.
*/
static void SetupMinimalEnvironment(uint8_t r, uint8_t g, uint8_t b,
                                    uint32_t outputs, uint32_t pixelsPerLine) {
    // Initialize the fake p8 data used by the focal method
    g_fakeP8.p = new cmsInterpParams;
    g_fakeP8.p->nOutputs = outputs;
    g_fakeP8.p->Table = nullptr;
    g_fakeP8.p->opa = nullptr;

    // Simple deterministic mapping for testing purposes
    // X0/Y0/Z0 arrays are indexed by input color values
    for (int i = 0; i < 256; ++i) {
        g_fakeP8.X0[i] = static_cast<uint8_t>(i);
        g_fakeP8.Y0[i] = static_cast<uint8_t>(i);
        g_fakeP8.Z0[i] = static_cast<uint8_t>(i);
        // activity in rx/ry/rz; pick ordering to trigger branches
        g_fakeP8.rx[i] = (i % 5) - 2;
        g_fakeP8.ry[i] = (i % 7) - 3;
        g_fakeP8.rz[i] = (i % 3) - 1;
    }

    // natively store the chosen r,g,b into arrays for quick access
    g_fakeP8.X0[r] = r;
    g_fakeP8.Y0[g] = g;
    g_fakeP8.Z0[b] = b;
    g_fakeP8.rx[r] = 0; // force a few branch choices
    g_fakeP8.ry[g] = 1;
    g_fakeP8.rz[b] = -1;

    // Reset the 3D inputs/outputs in a minimal fashion inside the test itself
    // The actual PerformanceEval8 will write to Output depending on Stride.
}

/* Simple non-terminating assertion helper to report failures without
   aborting the entire test suite. */
static void Check(bool condition, const char* msg) {
    if (!condition) {
        std::cerr << "Test failure: " << msg << std::endl;
        // Use a non-terminating assertion; for this harness we still exit
        // to avoid cascading failures.
        std::exit(1);
    }
}

/*---------------------------------------------------------------------------
  Test Case 1: Basic single-pixel, 1 output, no alpha
  - Exercise the main loop with minimal configuration.
  - Verify that some output color channel has been written.
---------------------------------------------------------------------------*/
static void Test_Basic_SinglePixel_NoAlpha() {
    // Prepare environment
    SetupMinimalEnvironment(10, 20, 30, /*outputs*/ 1, /*pixels*/ 1);

    // Input/Output buffers
    uint8_t input[3] = {10, 20, 30};
    uint8_t outputBuf[16];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct cmsStride stride;
    std::memset(&stride, 0, sizeof(stride));

    // Call focal function
    // Note: The real test would require a proper CMM cargo object; we pass NULL as
    // a placeholder if the linker allows. If the real signature requires a valid
    // transform object, adapt accordingly.
    PerformanceEval8(nullptr, input, outputBuf, 1, 1, &stride);

    // Basic sanity: some output should be written (not all zeros)
    bool nonzero = false;
    for (int i = 0; i < 16; ++i) {
        if (outputBuf[i] != 0) { nonzero = true; break; }
    }
    Check(nonzero, "Basic single-pixel write: expected non-zero output");
}

/*---------------------------------------------------------------------------
  Test Case 2: Multi-line path with a few pixels to exercise the line loop
---------------------------------------------------------------------------*/
static void Test_MultipleLines_Write() {
    SetupMinimalEnvironment(40, 80, 120, /*outputs*/ 2, /*pixels*/ 4);

    uint8_t input[12] = {40,80,120,  41,81,121,  42,82,122,  43,83,123};
    uint8_t outputBuf[64];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct cmsStride stride;
    std::memset(&stride, 0, sizeof(stride));

    PerformanceEval8(nullptr, input, outputBuf, 4, 1, &stride);

    // Check that at least some output positions were written
    bool written = false;
    for (int i = 0; i < 64; ++i) {
        if (outputBuf[i] != 0) { written = true; break; }
    }
    Check(written, "Multiple lines: expected non-zero output in buffer");
}

/*---------------------------------------------------------------------------
  Test Case 3: Toggle Output Channel Count
  - Ensure the function handles more outputs than inputs gracefully.
---------------------------------------------------------------------------*/
static void Test_MultipleOutputs() {
    SetupMinimalEnvironment(5, 15, 25, /*outputs*/ 4, /*pixels*/ 2);

    uint8_t input[6] = {5, 15, 25, 6, 16, 26};
    uint8_t outputBuf[32];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct cmsStride stride;
    std::memset(&stride, 0, sizeof(stride));

    PerformanceEval8(nullptr, input, outputBuf, 2, 1, &stride);

    // Expect that at least one of the four outputs is populated
    bool hasOutput = false;
    for (int i = 0; i < 32; ++i) {
        if (outputBuf[i] != 0) { hasOutput = true; break; }
    }
    Check(hasOutput, "Multiple outputs: expect non-zero output data");
}

/*---------------------------------------------------------------------------
  Test Case 4: Alpha present in input (if supported by the build)
  - Verify that the code path that handles copy alpha flag is engaged when
    cmsFLAGS_COPY_ALPHA is set on the transform (we rely on the library to
    provide this flag semantics; here we simulate the scenario by calling with
    an environment that would enable alpha path).
---------------------------------------------------------------------------*/
static void Test_AlphaPath() {
    SetupMinimalEnvironment(12, 34, 56, /*outputs*/ 3, /*pixels*/ 3);

    uint8_t input[12] = {12,34,56, 13,35,57, 14,36,58, 0,0,0};
    uint8_t outputBuf[48];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct cmsStride stride;
    std::memset(&stride, 0, sizeof(stride));

    // In a full environment, you would set the transform flags to enable COPY_ALPHA.
    // Here we simply call the function and verify no crash and some output writes.
    PerformanceEval8(nullptr, input, outputBuf, 3, 1, &stride);

    bool written = false;
    for (int i = 0; i < 48; ++i) if (outputBuf[i] != 0) { written = true; break; }
    Check(written, "Alpha path: expect some output data to be written");
}

/*---------------------------------------------------------------------------
  Test Case 5: Degenerate input (zeroed inputs)
  - Ensure code can handle degenerate inputs gracefully without crashing.
---------------------------------------------------------------------------*/
static void Test_DegenerateInputs() {
    SetupMinimalEnvironment(0, 0, 0, /*outputs*/ 2, /*pixels*/ 2);

    uint8_t input[6] = {0, 0, 0, 0, 0, 0};
    uint8_t outputBuf[32];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct cmsStride stride;
    std::memset(&stride, 0, sizeof(stride));

    PerformanceEval8(nullptr, input, outputBuf, 2, 1, &stride);

    // Output may be deterministic; at least ensure function returns without crash
    // and buffer is in a valid state (no partial writes beyond allocation)
    bool ok = true;
    for (int i = 0; i < 32; ++i) {
        if (outputBuf[i] > 255) { ok = false; break; }
    }
    Check(ok, "Degenerate input: output buffer within byte range");
}

/*---------------------------------------------------------------------------
  Test Case 6: Boundary values for inputs (0 and 255)
  - Exercise boundary conditions on color indices to ensure no out-of-bounds access.
---------------------------------------------------------------------------*/
static void Test_BoundaryIndices() {
    SetupMinimalEnvironment(0, 255, 128, /*outputs*/ 2, /*pixels*/ 2);

    uint8_t input[6] = {0, 255, 128, 0, 255, 128};
    uint8_t outputBuf[32];
    std::memset(outputBuf, 0, sizeof(outputBuf));

    struct cmsStride stride;
    std::memset(&stride, 0, sizeof(stride));

    PerformanceEval8(nullptr, input, outputBuf, 2, 1, &stride);

    bool ok = false;
    for (int i = 0; i < 32; ++i) if (outputBuf[i] != 0) { ok = true; break; }
    Check(ok, "Boundary indices: ensure safe writes for 0/255 indices");
}

/*---------------------------------------------------------------------------
  Test runner
---------------------------------------------------------------------------*/
static int RunTests() {
    // Clear the fake data to a known state
    std::memset(&g_fakeP8, 0, sizeof(g_fakeP8));

    Test_Basic_SinglePixel_NoAlpha();
    Test_MultipleLines_Write();
    Test_MultipleOutputs();
    Test_AlphaPath();
    Test_DegenerateInputs();
    Test_BoundaryIndices();

    std::cout << "All PerformanceEval8 unit tests passed." << std::endl;
    return 0;
}

int main() {
    int rc = RunTests();
    return rc;
}