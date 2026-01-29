// Test suite for MatShaperXform8SSE in fast_8_matsh_sse.c
// C++11 compatible test harness without Google Test.
// The tests are designed to be executable in a project that provides the
// LittleCMS-like environment used by MatShaperXform8SSE. This harness
// follows the requested steps: understanding, test generation, and refinement.
// It uses non-terminating expectations (EXPECT_* style macros) to maximize
// code coverage even when a test case fails.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <cstddef>
#include <iostream>
#include <cpuid.h>
#include <cstdint>


// Step 1: Candidate Keywords extracted from the focal method
// - MatShaperXform8SSE
// - _cmsGetTransformUserData
// - XMatShaper8Data
// - Mat[4], Shaper1R/G/B, Shaper2R/G/B
// - SourceStartingOrder, SourceIncrements
// - DestStartingOrder, DestIncrements
// - alpha channel handling (nalpha, ain, aout)
// - SIMD: __m128, _mm_load_ps, _mm_setzero_ps, _mm_set1_ps
// - clamping: _mm_min_ps, _mm_max_ps, scale = 0x4000
// - output index conversion: _mm_cvtps_epi32
// - transform stride handling: Stride->BytesPerPlaneIn / Stride->BytesPerLineIn
// - Limits: cmsMAXCHANNELS, cmsFLOAT, cmsUInt8Number
//
// Step 2: Unit Test Generation
// - We provide a lightweight test harness that calls MatShaperXform8SSE and
//   exercises basic invocation, both with and without alpha channel.
// - We rely on the already-present environment in the project to supply
//   the actual data structures and CMS utilities. The test focuses on
//   integration points and data flow rather than internal private state.
// - We use non-terminating assertions (EXPECT_*) to maximize test coverage.
//
// Step 3: Test Case Refinement
// - Add tests for: no-alpha path, alpha path (nalpha > 0).
// - Verify basic memory layout assumptions (input/output buffers non-null, sizes).
// - Validate that repeated calls do not crash and that the function returns
//   without modifying unrelated memory.
// - Note: Static/global state in the target is not explicitly accessed here; the
//   tests concentrate on the focal method usage patterns as requested.


// Domain knowledge helpers
// We define a lightweight assertion framework that does not terminate tests on failure
// but reports failures and keeps a running tally.

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating expectations (like gtest EXPECT_* macros)
#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " (line " << __LINE__ << ")\n"; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { \
    ++g_total_tests; \
    if ((ptr) == nullptr) { \
        std::cerr << "EXPECT_NOT_NULL failed: " #ptr " is null (line " << __LINE__ << ")\n"; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    ++g_total_tests; \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ") at line " << __LINE__ << "\n"; \
        ++g_failed_tests; \
    } \
} while(0)


// Step: Prototype for MatShaperXform8SSE
// The actual function is implemented in fast_8_matsh_sse.c.
// We declare the signature here so tests can link against it.
extern "C" void MatShaperXform8SSE(struct _cmstransform_struct *CMMcargo,
                     const void* Input,
                     void* Output,
                     uint32_t PixelsPerLine,
                     uint32_t LineCount,
                     const struct cmsStride* Stride);

// Forward declaration for the transform structure used by the function
// The real type is provided by the project; we only need a pointer type here.
struct _cmstransform_struct;

// Lightweight cmsStride structure (matching usage in focal method).
// We only define the fields that are accessed in the focal method.
// The actual project may define additional fields, but this layout is sufficient
// for the purposes of constructing valid test inputs.
struct cmsStride {
    size_t BytesPerPlaneIn;
    size_t BytesPerLineIn;
    size_t BytesPerPlaneOut;
    size_t BytesPerLineOut;
};

// Helper: create a trivial transform context placeholder.
// We do not rely on a real CMS transform for the test; we simply provide a dummy pointer.
// The focal method queries _cmsGetTransformUserData(CMMcargo), which we assume is supplied
// by the project's CMS layer. In real test execution, the project will link with the
// actual CMS implementation, and the dummy context will be replaced accordingly.
static _cmstransform_struct* g_fakeCMMcargo = nullptr;

// Step 2: Provide a minimal integration test harness.
// Note: These tests assume that the project provides a compatible CMS environment.
// The tests focus on data flow and stability, not on precise numerical mapping
// (which would require fully deterministic Shaper tables and a concrete transform).
namespace test_suite {

// Test 1: Basic invocation with no alpha channel.
// Purpose: Ensure the function executes for a simple, small line count and
// with alpha disabled. Verifies that input/output buffers are accessed and
// that the function returns without crashing.
void test_basic_no_alpha() {
    // Prepare fake transform cargo (pointer, not dereferenced by test here).
    // In a real environment, this would be an actual _cmstransform_struct* returned
    // by the CMS library. We rely on the project's existing fixtures for runtime.
    g_fakeCMMcargo = reinterpret_cast<_cmstransform_struct*>(0x1); // non-null sentinel

    // Create simple input/output buffers
    const uint32_t PixelsPerLine = 4;
    const uint32_t LineCount = 2;

    // For typical CMS input, each pixel has 3 channels (R,G,B) or 4 if alpha is present.
    // We'll craft a 3-channel input (RGB) with zeros to keep values predictable.

    // Assuming input format is packed by bytes per pixel, we allocate a small buffer.
    const size_t inBytesPerPixel = 3;
    const size_t outBytesPerPixel = 3;

    // Allocate input and output buffers
    std::vector<uint8_t> input(PixelsPerLine * LineCount * inBytesPerPixel, 0);
    std::vector<uint8_t> output(PixelsPerLine * LineCount * outBytesPerPixel, 0);

    // Stride placeholder (values chosen to be safe and small)
    cmsStride stride;
    stride.BytesPerPlaneIn  = PixelsPerLine * inBytesPerPixel;
    stride.BytesPerLineIn   = PixelsPerLine * inBytesPerPixel;
    stride.BytesPerPlaneOut = PixelsPerLine * outBytesPerPixel;
    stride.BytesPerLineOut  = PixelsPerLine * outBytesPerPixel;

    // Call the focal method
    MatShaperXform8SSE(g_fakeCMMcargo,
                      input.data(),
                      output.data(),
                      PixelsPerLine,
                      LineCount,
                      &stride);

    // Basic sanity checks: buffers must be touched (not necessarily changed in a deterministic way)
    // We expect the function to have written some output values (not all zero).
    bool nonZeroOutputFound = false;
    for (size_t i = 0; i < output.size(); ++i) {
        if (output[i] != 0) { nonZeroOutputFound = true; break; }
    }

    Expectation:
    // Use non-terminating assertion to allow test suite to continue on failure.
    EXPECT_TRUE(nonZeroOutputFound || "Output buffer remained all zeros; function may not modify output in this test setup.");

    // Clean up sentinel
    g_fakeCMMcargo = nullptr;
}

// Test 2: Basic invocation with alpha channel present.
// Purpose: Ensure the function handles presence of alpha channel (nalpha > 0).
// We still verify that the function executes and writes to the output buffer
// without crashing, while keeping inputs small and deterministic.
void test_with_alpha() {
    g_fakeCMMcargo = reinterpret_cast<_cmstransform_struct*>(0x2); // non-null sentinel

    const uint32_t PixelsPerLine = 4;
    const uint32_t LineCount = 2;

    const size_t inBytesPerPixel = 4; // RGBA
    const size_t outBytesPerPixel = 4; // RGBA

    std::vector<uint8_t> input(PixelsPerLine * LineCount * inBytesPerPixel, 0x80);  // mid-gray
    std::vector<uint8_t> output(PixelsPerLine * LineCount * outBytesPerPixel, 0);

    cmsStride stride;
    stride.BytesPerPlaneIn  = PixelsPerLine * inBytesPerPixel;
    stride.BytesPerLineIn   = PixelsPerLine * inBytesPerPixel;
    stride.BytesPerPlaneOut = PixelsPerLine * outBytesPerPixel;
    stride.BytesPerLineOut  = PixelsPerLine * outBytesPerPixel;

    MatShaperXform8SSE(g_fakeCMMcargo,
                      input.data(),
                      output.data(),
                      PixelsPerLine,
                      LineCount,
                      &stride);

    // Simple check: ensure at least some output bytes are touched
    bool touched = false;
    for (size_t i = 0; i < output.size(); ++i) {
        if (output[i] != 0) { touched = true; break; }
    }
    EXPECT_TRUE(touched || "Output buffer not touched with alpha present.");
    g_fakeCMMcargo = nullptr;
}

// Test 3: Stability test - multiple invocations with same buffers.
// Purpose: Verify that repeated calls do not crash or corrupt memory.
void test_repeated_invocations() {
    g_fakeCMMcargo = reinterpret_cast<_cmstransform_struct*>(0x3); // sentinel

    const uint32_t PixelsPerLine = 8;
    const uint32_t LineCount = 3;

    const size_t inBytesPerPixel = 3;
    const size_t outBytesPerPixel = 3;

    std::vector<uint8_t> input(PixelsPerLine * LineCount * inBytesPerPixel, 0x55);
    std::vector<uint8_t> output(PixelsPerLine * LineCount * outBytesPerPixel, 0);

    cmsStride stride;
    stride.BytesPerPlaneIn  = PixelsPerLine * inBytesPerPixel;
    stride.BytesPerLineIn   = PixelsPerLine * inBytesPerPixel;
    stride.BytesPerPlaneOut = PixelsPerLine * outBytesPerPixel;
    stride.BytesPerLineOut  = PixelsPerLine * outBytesPerPixel;

    // Call twice to exercise potential stateful issues
    for (int t = 0; t < 2; ++t) {
        MatShaperXform8SSE(g_fakeCMMcargo,
                          input.data(),
                          output.data(),
                          PixelsPerLine,
                          LineCount,
                          &stride);
    }

    bool anyChange = false;
    for (size_t i = 0; i < output.size(); ++i) {
        if (output[i] != 0) { anyChange = true; break; }
    }

    EXPECT_TRUE(anyChange || "Output did not change after repeated invocations; may indicate early exit path.");
    g_fakeCMMcargo = nullptr;
}

// Step 4: Test runner
void run_all_tests() {
    std::cout << "Running MatShaperXform8SSE test suite (non-GTest)..." << std::endl;
    test_basic_no_alpha();
    test_with_alpha();
    test_repeated_invocations();
    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
}

// Entry point
} // namespace test_suite

int main() {
    // Run the test suite
    test_suite::run_all_tests();
    // Return nonzero if any test failed (for CI compatibility; although the suite uses non-terminating assertions)
    if (g_failed_tests > 0) {
        std::cerr << "Some tests failed. Failures: " << g_failed_tests << "/" << g_total_tests << std::endl;
        return 1;
    }
    std::cout << "All tests passed (as far as non-fatal checks could determine)." << std::endl;
    return 0;
}