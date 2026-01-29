/************* Unit Test Suite for Pack3BytesSwapOptimized *************
   - Target: cmsUInt8Number* Pack3BytesSwapOptimized(CMSREGISTER _cmsTRANSFORM* info,
              CMSREGISTER cmsUInt16Number wOut[],
              CMSREGISTER cmsUInt8Number* output,
              CMSREGISTER cmsUInt32Number Stride)
   - Environment: C++11, no GoogleTest. Uses CMS/Little CMS internal types from lcms2_internal.h.
   - Rationale: Validate that the function correctly writes the three swapped-bytes
     from wOut[2], wOut[1], wOut[0] (upper values masked with 0xFF) into output,
     and returns output advanced by 3 bytes. Also confirm that Stride and info are unused
     and do not affect the result.
**********************************************************************/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Import CMS internal types used by the focal function.
// This header is part of the project and provides definitions like cmsUInt8Number, etc.

extern "C" {
// Forward declaration of the focal function to ensure correct linkage from C++ test code.
// The exact signature mirrors the implementation in cmspack.c.
cmsUInt8Number* Pack3BytesSwapOptimized(CMSREGISTER _cmsTRANSFORM* info,
                                       CMSREGISTER cmsUInt16Number wOut[],
                                       CMSREGISTER cmsUInt8Number* output,
                                       CMSREGISTER cmsUInt32Number Stride);
}

// Simple non-terminating test harness (EXPECT_*) for C++11 without GTest.
// This harness prints failures but continues executing other tests to maximize coverage.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ_UINT8(actual, expected, msg) do {                                \
    ++g_total_tests;                                                                 \
    if (static_cast<uint8_t>(actual) != static_cast<uint8_t>(expected)) {           \
        ++g_failed_tests;                                                            \
        std::cout << "FAIL: " << (msg)                                               \
                  << " | Expected: 0x" << std::hex << int(expected)                 \
                  << "  Got: 0x" << int(actual) << std::dec << std::endl;          \
    }                                                                                \
} while (0)

#define EXPECT_EQ_PTR(actual, expected, msg) do {                                   \
    ++g_total_tests;                                                                 \
    if ((actual) != (expected)) {                                                    \
        ++g_failed_tests;                                                            \
        std::cout << "FAIL: " << (msg)                                               \
                  << " | Expected pointer: " << static_cast<void*>(expected)        \
                  << "  Got: " << static_cast<void*>(actual) << std::endl;          \
    }                                                                                \
} while (0)

static void test_Pack3BytesSwapOptimized_BasicCase() {
    // Domain knowledge reference: Basic swap should output wOut[2], wOut[1], wOut[0]
    // with only the least significant byte (LSB) of each value (mask 0xFF).
    cmsUInt16Number wOut[3] = { 0x0102, 0x0304, 0x0506 }; // wOut[0] = 0x0102, wOut[1] = 0x0304, wOut[2] = 0x0506
    cmsUInt8Number outputBuf[3];
    cmsUInt8Number* end = Pack3BytesSwapOptimized(nullptr, wOut, outputBuf, 0);

    // Expected: [wOut[2] & 0xFF, wOut[1] & 0xFF, wOut[0] & 0xFF] => [0x02, 0x04, 0x06]
    EXPECT_EQ_UINT8(outputBuf[0], 0x02, "BasicCase: first swapped byte should be 0x02");
    EXPECT_EQ_UINT8(outputBuf[1], 0x04, "BasicCase: second swapped byte should be 0x04");
    EXPECT_EQ_UINT8(outputBuf[2], 0x06, "BasicCase: third swapped byte should be 0x06");

    // Return pointer should advance by 3 bytes from the original output buffer.
    EXPECT_EQ_PTR(end, outputBuf + 3, "BasicCase: return pointer should be outputBuf + 3");
}

static void test_Pack3BytesSwapOptimized_LowByteMasking() {
    // Domain knowledge: The function uses & 0xFFU on each wOut[] element.
    // Use values with high bytes set to verify masking behavior.
    cmsUInt16Number wOut[3] = { 0xFFFF, 0xABCD, 0x1234 }; // wOut[0]=0xFFFF, wOut[1]=0xABCD, wOut[2]=0x1234
    cmsUInt8Number outputBuf[3];
    cmsUInt8Number* end = Pack3BytesSwapOptimized(nullptr, wOut, outputBuf, 0);

    // Expected: [0x1234 & 0xFF, 0xABCD & 0xFF, 0xFFFF & 0xFF] => [0x34, 0xCD, 0xFF]
    EXPECT_EQ_UINT8(outputBuf[0], 0x34, "Masking: first swapped byte should be 0x34");
    EXPECT_EQ_UINT8(outputBuf[1], 0xCD, "Masking: second swapped byte should be 0xCD");
    EXPECT_EQ_UINT8(outputBuf[2], 0xFF, "Masking: third swapped byte should be 0xFF");

    // Pointer should again be outputBuf + 3
    EXPECT_EQ_PTR(end, outputBuf + 3, "Masking: return pointer should be outputBuf + 3");
}

static void test_Pack3BytesSwapOptimized_MultipleCalls() {
    // Call the function twice with different inputs to ensure no internal state leaks.
    cmsUInt16Number wOut1[3] = { 0x1122, 0x3344, 0x5566 };
    cmsUInt8Number out1[3];
    cmsUInt8Number* end1 = Pack3BytesSwapOptimized(nullptr, wOut1, out1, 8);

    // Expected: [0x66, 0x44, 0x22]
    EXPECT_EQ_UINT8(out1[0], 0x66, "MultiCall1: first byte should be 0x66");
    EXPECT_EQ_UINT8(out1[1], 0x44, "MultiCall1: second byte should be 0x44");
    EXPECT_EQ_UINT8(out1[2], 0x22, "MultiCall1: third byte should be 0x22");
    EXPECT_EQ_PTR(end1, out1 + 3, "MultiCall1: end pointer should be out1 + 3");

    cmsUInt16Number wOut2[3] = { 0xAABB, 0xCCDD, 0xEEFF };
    cmsUInt8Number out2[3];
    cmsUInt8Number* end2 = Pack3BytesSwapOptimized(nullptr, wOut2, out2, 0);

    // Expected: [0xFF, 0xDD, 0xBB]
    EXPECT_EQ_UINT8(out2[0], 0xFF, "MultiCall2: first byte should be 0xFF");
    EXPECT_EQ_UINT8(out2[1], 0xDD, "MultiCall2: second byte should be 0xDD");
    EXPECT_EQ_UINT8(out2[2], 0xBB, "MultiCall2: third byte should be 0xBB");
    EXPECT_EQ_PTR(end2, out2 + 3, "MultiCall2: end pointer should be out2 + 3");
}

int main() {
    // Run focused unit tests for the focal function.
    test_Pack3BytesSwapOptimized_BasicCase();
    test_Pack3BytesSwapOptimized_LowByteMasking();
    test_Pack3BytesSwapOptimized_MultipleCalls();

    // Summary
    std::cout << "Pack3BytesSwapOptimized Test Summary: "
              << g_total_tests << " tests executed, "
              << g_failed_tests << " failures." << std::endl;

    // Return 0 even if some tests fail to allow continuous integration to proceed.
    return (g_failed_tests == 0) ? 0 : 1;
}