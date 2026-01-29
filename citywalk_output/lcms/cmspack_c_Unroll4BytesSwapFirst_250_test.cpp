// Lightweight C++11 unit test harness for Unroll4BytesSwapFirst
// This test does not rely on any external testing framework (GTest, etc.).
// It uses a minimalist EXPECT-style assertions that do not abort on failure
// to maximize code coverage across tests.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstddef>


// Domain knowledge driven assumptions:
// - The focal function maps 4 input bytes to 4 16-bit words using zero-extension.
//   Specifically:
//     wIn[3] = 8-bit at accum (B0) zero-extended to cmsUInt16Number
//     wIn[0] = 8-bit at accum (B1) zero-extended
//     wIn[1] = 8-bit at accum (B2) zero-extended
//     wIn[2] = 8-bit at accum (B3) zero-extended
//   Then accum is advanced by 4 bytes and the pointer is returned.
// - The function signature and types are defined in the cmsLCMS codebase.
//   We provide compatible forward declarations to allow linking with the real
//   implementation (cmspack.c) in the test environment.

extern "C" {
    // Minimal typedefs mirroring the real library's widths.
    typedef unsigned char cmsUInt8Number;
    typedef unsigned short cmsUInt16Number;
    typedef unsigned int cmsUInt32Number;

    // Opaque transform type used by the function (actual contents are not needed here)
    typedef struct _cmsTRANSFORM _cmsTRANSFORM;

    // Macro used by the real code (present in the library headers)
    #define CMSREGISTER

    // Function declaration (signature matches the library)
    cmsUInt8Number* Unroll4BytesSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                           CMSREGISTER cmsUInt16Number wIn[],
                                           CMSREGISTER cmsUInt8Number* accum,
                                           CMSREGISTER cmsUInt32Number Stride);
}

// Simple, non-terminating test harness
static int g_totalTests = 0;
static int g_failures = 0;

static void _log_failure(const char* testName, const char* message) {
    std::cerr << "[FAIL] " << testName << ": " << message << std::endl;
}

#define EXPECT_EQ_U(a, b, testName) do { \
    ++g_totalTests; \
    if (static_cast<unsigned int>(a) != static_cast<unsigned int>(b)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " << testName << " - expected " \
                  << static_cast<unsigned int>(b) \
                  << ", got " \
                  << static_cast<unsigned int>(a) << std::endl; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b, testName) do { \
    ++g_totalTests; \
    if ((a) != (b)) { \
        ++g_failures; \
        std::cerr << "[FAIL] " << testName \
                  << " - expected pointer " << static_cast<const void*>(static_cast<const void*>(b)) \
                  << ", got " << static_cast<const void*>(static_cast<const void*>(a)) << std::endl; \
    } \
} while(0)

static void test_Unroll4BytesSwapFirst_basic_mapping() {
    // Test description:
    // Given 4 input bytes, verify that wIn gets populated in the expected order
    // and that accum advances by 4 bytes.

    // Prepare input/output
    cmsUInt16Number wIn[4] = {0, 0, 0, 0};

    // Accumulator buffer with 8 bytes (first 4 used by the first call, next 4 for a second call)
    cmsUInt8Number buffer[8] = { 0x11, 0x22, 0x33, 0x44,
                                0x55, 0x66, 0x77, 0x88 };
    cmsUInt8Number* accum = buffer;
    const cmsUInt32Number Stride = 0; // unused by this function

    // Call the focal function
    extern "C" cmsUInt8Number* Unroll4BytesSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                                    CMSREGISTER cmsUInt16Number wIn[],
                                                    CMSREGISTER cmsUInt8Number* accum,
                                                    CMSREGISTER cmsUInt32Number Stride);

    // We pass a null info (unused) since the test cares only about mapping behavior
    Unroll4BytesSwapFirst(nullptr, wIn, accum, Stride);

    // Expected mapping:
    // wIn[3] = buffer[0] (0x11)
    // wIn[0] = buffer[1] (0x22)
    // wIn[1] = buffer[2] (0x33)
    // wIn[2] = buffer[3] (0x44)
    EXPECT_EQ_U(wIn[3], 0x11, "basic_mapping.wIn[3] should be 0x11");
    EXPECT_EQ_U(wIn[0], 0x22, "basic_mapping.wIn[0] should be 0x22");
    EXPECT_EQ_U(wIn[1], 0x33, "basic_mapping.wIn[1] should be 0x33");
    EXPECT_EQ_U(wIn[2], 0x44, "basic_mapping.wIn[2] should be 0x44");

    // accum advanced by 4 bytes
    EXPECT_PTR_EQ(accum, buffer + 4, "basic_mapping.accum should advance by 4 bytes");
}

static void test_Unroll4BytesSwapFirst_multiple_calls() {
    // Test description:
    // Call Unroll4BytesSwapFirst twice on the same buffer (8 bytes total)
    // to ensure that subsequent call uses the next 4 bytes correctly.

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number buffer[8] = { 0xAA, 0xBB, 0xCC, 0xDD,
                               0x11, 0x22, 0x33, 0x44 };
    cmsUInt8Number* accum = buffer;
    const cmsUInt32Number Stride = 0;

    extern "C" cmsUInt8Number* Unroll4BytesSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                                    CMSREGISTER cmsUInt16Number wIn[],
                                                    CMSREGISTER cmsUInt8Number* accum,
                                                    CMSREGISTER cmsUInt32Number Stride);

    // First call
    Unroll4BytesSwapFirst(nullptr, wIn, accum, Stride);
    // Expected after first call:
    // wIn[3] = 0xAA, wIn[0] = 0xBB, wIn[1] = 0xCC, wIn[2] = 0xDD
    EXPECT_EQ_U(wIn[3], 0xAA, "first_call.wIn[3] should be 0xAA");
    EXPECT_EQ_U(wIn[0], 0xBB, "first_call.wIn[0] should be 0xBB");
    EXPECT_EQ_U(wIn[1], 0xCC, "first_call.wIn[1] should be 0xCC");
    EXPECT_EQ_U(wIn[2], 0xDD, "first_call.wIn[2] should be 0xDD");
    EXPECT_PTR_EQ(accum, buffer + 4, "first_call.accum should advance by 4 bytes");

    // Prepare for second call
    cmsUInt16Number wIn2[4] = {0, 0, 0, 0};
    cmsUInt8Number* accum2 = accum; // already at buffer+4

    // Second call
    Unroll4BytesSwapFirst(nullptr, wIn2, accum2, Stride);

    // Expected after second call:
    // wIn2[3] = buffer[4] (0x11), wIn2[0] = buffer[5] (0x22),
    // wIn2[1] = buffer[6] (0x33), wIn2[2] = buffer[7] (0x44)
    EXPECT_EQ_U(wIn2[3], 0x11, "second_call.wIn[3] should be 0x11");
    EXPECT_EQ_U(wIn2[0], 0x22, "second_call.wIn[0] should be 0x22");
    EXPECT_EQ_U(wIn2[1], 0x33, "second_call.wIn[1] should be 0x33");
    EXPECT_EQ_U(wIn2[2], 0x44, "second_call.wIn[2] should be 0x44");
    EXPECT_PTR_EQ(accum2, buffer + 8, "second_call.accum should advance by 4 more bytes");
}

static void test_Unroll4BytesSwapFirst_zero_bytes() {
    // Test description:
    // When input bytes are all zero, 16-bit outputs should be zero and accum advances correctly.

    cmsUInt16Number wIn[4] = {0, 0, 0, 0};
    cmsUInt8Number buffer[4] = {0, 0, 0, 0};
    cmsUInt8Number* accum = buffer;
    const cmsUInt32Number Stride = 0;

    extern "C" cmsUInt8Number* Unroll4BytesSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                                    CMSREGISTER cmsUInt16Number wIn[],
                                                    CMSREGISTER cmsUInt8Number* accum,
                                                    CMSREGISTER cmsUInt32Number Stride);

    Unroll4BytesSwapFirst(nullptr, wIn, accum, Stride);

    EXPECT_EQ_U(wIn[3], 0x00, "zero_case.wIn[3] should be 0x00");
    EXPECT_EQ_U(wIn[0], 0x00, "zero_case.wIn[0] should be 0x00");
    EXPECT_EQ_U(wIn[1], 0x00, "zero_case.wIn[1] should be 0x00");
    EXPECT_EQ_U(wIn[2], 0x00, "zero_case.wIn[2] should be 0x00");
    EXPECT_PTR_EQ(accum, buffer + 4, "zero_case.accum should advance by 4 bytes");
}

int main() {
    // Run all test cases
    test_Unroll4BytesSwapFirst_basic_mapping();
    test_Unroll4BytesSwapFirst_multiple_calls();
    test_Unroll4BytesSwapFirst_zero_bytes();

    // Summary
    std::cout << "Total tests: " << g_totalTests << ", Failures: " << g_failures << std::endl;
    if (g_failures != 0) {
        std::cout << "Some tests failed. Review the details above." << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }
    return g_failures ? 1 : 0;
}