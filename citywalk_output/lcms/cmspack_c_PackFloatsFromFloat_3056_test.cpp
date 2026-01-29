// Unit test suite for the focal method: PackFloatsFromFloat
// This test suite has been crafted to exercise PackFloatsFromFloat
// in a minimal, deterministic manner without relying on GTest.
// It uses a compact in-file test harness with non-terminating expectations.
// Note: The tests rely on the project's internal cms types/macros
// (e.g., _cmsTRANSFORM, T_CHANNELS, PixelSize, etc.) being available
// via the same headers as the library (lcms2_internal.h). Ensure the
// test is compiled in an environment where the LittleCMS internals are
// accessible and consistent with the focal implementation.

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cmath>


// Forward declaration of the focal function from the library.
// We declare the function with C linkage to avoid name mangling issues
// and to match the library's C-API.
extern "C" {
    // Forward-declare the internal transform structure to be compatible
    // with the library's function. We use a complete type declaration
    // here because the actual layout is defined in the library headers.
    // If the project provides a different internal type, this forward
    // declaration should still be compatible due to how the function is used.
    struct _cmsTRANSFORM;
    typedef struct _cmsTRANSFORM _cmsTRANSFORM;

    // Declaration of the focal method under test.
    // Signature observed in the provided focal method.
    // wOut is an array of input floats, output is a byte buffer to be filled with converted data.
    cmsUInt8Number* PackFloatsFromFloat(_cmsTRANSFORM* info,
                                        cmsFloat32Number wOut[],
                                        cmsUInt8Number* output,
                                        cmsUInt32Number Stride);
}

// Typedefs matching the library's style (as seen in the focal method snippet)
using cmsUInt32Number = std::uint32_t;
using cmsUInt8Number  = std::uint8_t;
using cmsFloat32Number = float;
using cmsFloat64Number = double;

// To ensure portability across different compilers
#define EPSILON 1e-5f

// Simple non-throwing test harness.
// We implement lightweight EXPECT_* macros that accumulate failures
// but do not abort the test run, enabling "maximal coverage".
// These mirror a subset of typical gtest-like assertions but are self-contained.

static int g_test_failures = 0;

#define EXPECT_TRUE(cond, msg)                                   \
    do {                                                          \
        if (!(cond)) {                                           \
            std::cerr << "[TEST FAIL] " << (msg) << "\n";       \
            ++g_test_failures;                                   \
        }                                                        \
    } while(0)

#define EXPECT_EQ(val, expected, msg)                             \
    do {                                                            \
        if (!((val) == (expected))) {                             \
            std::cerr << "[TEST FAIL] " << (msg)                 \
                      << " | Expected: " << (expected)            \
                      << ", Got: " << (val) << "\n";            \
            ++g_test_failures;                                     \
        }                                                         \
    } while(0)

#define EXPECT_FLOAT_NEAR(val, expected, eps, msg)                 \
    do {                                                             \
        if (!((std::fabs((val) - (expected)) <= (eps)))) {          \
            std::cerr << "[TEST FAIL] " << (msg) << " | "            \
                      << "Expected ~" << (expected)                \
                      << ", Got ~" << (val) << "\n";               \
            ++g_test_failures;                                      \
        }                                                          \
    } while(0)

// Lightweight helper to cast raw output bytes to a float array for inspection
static inline float to_float(const cmsUInt8Number* ptr) {
    float v;
    std::memcpy(&v, ptr, sizeof(float));
    return v;
}

// Candidate keywords extracted from the focal method (Step 1):
// - nChan: number of output channels derived from info->OutputFormat
// - DoSwap: channel order reversal flag
// - Reverse: color/float flavor flip (0 <= v <= maximum)
// - Extra: extra samples prefix (padding, etc.)
// - SwapFirst: special handling when Extra == 0 (swap first element with tail)
// - Planar: planar vs chunky memory layout
// - maximum: scaling factor, 100.0 if InkSpace, else 1.0
// These keywords guide test case design to cover multiple branches.

// Test 1: Basic 1-channel, non-swapped, non-reversing path, non-planar.
//         Verifies that a single input float is scaled (by maximum=1.0 for standard space)
//         and written contiguously to the output buffer. Verifies the return pointer as well.
static void test_PackFloatsFromFloat_BasicSingleChannel() {
    // Arrange
    _cmsTRANSFORM info;
    // We rely on the library header to define the correct bit-encoded OutputFormat
    // value that yields 1 output channel with all predicates false.
    // Setting to 0 typically corresponds to a "default" 1-channel format in many configurations.
    info.OutputFormat = 0;

    cmsFloat32Number wOut[1] = { 0.5f };          // input sample
    cmsUInt8Number outBuf[16] = { 0 };             // output buffer large enough for several floats

    // Stride represents the number of bytes between successive samples in memory.
    // PackFloatsFromFloat divides Stride by PixelSize(OutputFormat) internally.
    // We use a known 4-byte per-float alignment for 32-bit floats.
    cmsUInt32Number Stride = 4 * 1; // 4 bytes per pixel, 1 channel

    // Act
    cmsUInt8Number* retPtr = PackFloatsFromFloat(&info, wOut, outBuf, Stride);

    // The function, for non-planar single-channel data with Extra=0, should
    // write exactly one float and return output + sizeof(float).
    cmsUInt8Number* expectedPtr = outBuf + sizeof(float);

    // Read back the first written float from the buffer for verification
    float writtenVal = to_float(outBuf);

    // Assert
    EXPECT_TRUE(retPtr == expectedPtr, "Return pointer should be after the single written float.");
    EXPECT_FLOAT_NEAR(writtenVal, 0.5f, EPSILON, "Written value should equal input scaled by maximum (1.0).");
}

// Test 2: Boundary condition: ensure that a 1-channel input with a different value
//         is correctly scaled and placed in memory. This test also ensures
//         that the function does not overwrite adjacent memory when Stride is larger.
//         Note: This test uses the same 1-channel virtual format; to explore more
//         branches (e.g., SwapFirst, ExtraFirst, Planar), extend info.OutputFormat
//         with additional platform-specific constants in your environment.
static void test_PackFloatsFromFloat_SingleChannelValueVariability() {
    // Arrange
    _cmsTRANSFORM info;
    info.OutputFormat = 0; // 1 channel, default predicates false

    cmsFloat32Number wOut[1] = { 0.25f };      // input sample
    cmsUInt8Number outBuf[16] = { 0 };         // output buffer
    cmsUInt32Number Stride = 8;                // larger stride (non-default)

    // Act
    cmsUInt8Number* retPtr = PackFloatsFromFloat(&info, wOut, outBuf, Stride);

    // Expected: first element equals 0.25 (scaled by maximum=1.0)
    cmsUInt8Number* expectedPtr = outBuf + sizeof(float);
    float writtenVal = to_float(outBuf);

    // Assert
    EXPECT_TRUE(retPtr == expectedPtr, "Return pointer should be after the single written float (Stride-influenced).");
    EXPECT_FLOAT_NEAR(writtenVal, 0.25f, EPSILON, "Written value should equal input scaled by maximum (1.0).");
}

// Note on extending tests (Step 3 guidance):
// - To cover other branches (e.g., DoSwap, Reverse, Extra handling, Planar layout,
//   InkSpace maximum scaling, SwapFirst memmove, etc.), extend the OutputFormat
//   flags in _cmsTRANSFORM to encode those predicates. Then craft wOut with
//   multiple channels (nChan > 1) and verify:
//     - Correct channel order when DoSwap is true
//     - Correct clamping/scaling when InkSpace is true (maximum=100.0)
//     - Correct memory layout for Planar vs Chunky (by analyzing the index mapping)
//     - Behavior of memmove when Extra == 0 and SwapFirst == true (the swapped first value)
// - For each added case, create a dedicated test similar to the two above,
//   adjusting info.OutputFormat, the wOut array size, and the expected output.
//
// Step 3 (practical): The tests in this file rely on the library's internal OutputFormat
// encoding provided by lcms2_internal.h. If your build uses a different layout for
// OutputFormat bitfields, update the test inputs accordingly to force the
// corresponding branches to execute.

int main() {
    // Run tests
    test_PackFloatsFromFloat_BasicSingleChannel();
    test_PackFloatsFromFloat_SingleChannelValueVariability();

    if (g_test_failures == 0) {
        std::cout << "[TEST SUITE] All tests passed.\n";
        return 0;
    } else {
        std::cerr << "[TEST SUITE] " << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}