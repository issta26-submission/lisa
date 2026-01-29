// This test suite targets the focal method:
// cmsUInt8Number* Pack4BytesSwapSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
//                                         CMSREGISTER cmsUInt16Number wOut[],
 //                                        CMSREGISTER cmsUInt8Number* output,
//                                         CMSREGISTER cmsUInt32Number Stride)
 // The function packs 4 bytes by taking the low byte of wOut[2], wOut[1], wOut[0], wOut[3]
// and writes them sequentially to output, returning the pointer after the four writes.
//
// Notes:
// - We provide a minimal C++11 test harness without GoogleTest, focusing on
//   executable unit tests that can be run from main().
// - We assume the common CMS typedef mappings (cmsUInt8Number -> unsigned char,
//   cmsUInt16Number -> unsigned short, cmsUInt32Number -> unsigned int).
// - The tests verify both content and pointer arithmetic (end pointer).
// - The test framework uses non-terminating assertions (print-and-continue) to maximize coverage.
// - This file is designed to be linked with the project's cmspack.c (and related headers).

#include <lcms2_internal.h>
#include <cstring>
#include <cstdio>
#include <cstdint>


// Candidate Keywords (Step 1) for test target understanding:
// - Pack4BytesSwapSwapFirst, wOut, output, Stride
// - _cmsTRANSFORM, CMSREGISTER, cmsUInt16Number, cmsUInt8Number
// - FROM_16_TO_8 macro (used inside the focal function)
// - Unused parameters handling (cmsUNUSED_PARAMETER)
// - Call conventions and C linkage between test and target C code

// Re-create minimal test-facing type aliases to maximize compatibility with the
// project's cmspack.c expectations without pulling in the entire CMS header suite.
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Opaque transform type (as declared in the real project)
typedef struct _cmsTRANSFORM cmsTRANSFORM;

// The function under test is defined in cmspack.c with C linkage.
// We declare a compatible prototype for linking from C++ test code.
// We intentionally keep parameter types broad (matching equivalents) to avoid
// compile-time mismatches across translation units.
extern "C" cmsUInt8Number* Pack4BytesSwapSwapFirst(void* info,
                                                cmsUInt16Number wOut[],
                                                cmsUInt8Number* output,
                                                cmsUInt32Number Stride);

// Simple test harness (non-terminating assertions)
static int g_failures = 0;

static void testAssert(const char* label, cmsUInt8Number expected, cmsUInt8Number actual)
{
    if (expected != actual) {
        std::printf("FAIL  %s: expected 0x%02X, got 0x%02X\n", label, expected, actual);
        ++g_failures;
    } else {
        std::printf("PASS  %s\n", label);
    }
}

// A helper to run a single test scenario for Pack4BytesSwapSwapFirst
static void runScenario(unsigned short wOut[4], unsigned char* outputBuf, unsigned int Stride)
{
    // Prepare a fresh output buffer with recognizable pattern
    std::memset(outputBuf, 0xAA, 4);

    // Use a non-null info to exercise unused-parameter path
    struct dummy { int dummy; } dummyInfo = { 0 };
    void* info = &dummyInfo;

    // Call the function under test
    cmsUInt8Number* endPtr = Pack4BytesSwapSwapFirst(info, wOut, outputBuf, Stride);

    // Expected bytes based on FROM_16_TO_8(wOut[i]) semantics.
    // We assume FROM_16_TO_8 returns the low byte: (wOut[i] & 0xFF)
    cmsUInt8Number expected0 = static_cast<cmsUInt8Number>(wOut[2] & 0xFF);
    cmsUInt8Number expected1 = static_cast<cmsUInt8Number>(wOut[1] & 0xFF);
    cmsUInt8Number expected2 = static_cast<cmsUInt8Number>(wOut[0] & 0xFF);
    cmsUInt8Number expected3 = static_cast<cmsUInt8Number>(wOut[3] & 0xFF);

    // 1) The function should advance the output pointer by 4 bytes
    cmsUInt8Number* expectedEnd = outputBuf + 4;
    if (endPtr != expectedEnd) {
        std::printf("FAIL  end-pointer: expected %p, got %p\n",
                    (void*)expectedEnd, (void*)endPtr);
        ++g_failures;
    } else {
        std::printf("PASS  end-pointer\n");
    }

    // 2) Validate the four bytes written
    testAssert("byte0 (wOut[2] low)", expected0, outputBuf[0]);
    testAssert("byte1 (wOut[1] low)", expected1, outputBuf[1]);
    testAssert("byte2 (wOut[0] low)", expected2, outputBuf[2]);
    testAssert("byte3 (wOut[3] low)", expected3, outputBuf[3]);
}

// Test 1: Normal non-zero input values
static void test_Pack4BytesSwapSwapFirst_Normal()
{
    // Candidate Values: choose a pattern with distinct low bytes
    unsigned short wOut[4] = { 0x0102, 0x0304, 0x0506, 0x0708 }; // low bytes: 02,04,06,08
    unsigned char outputBuf[4];
    runScenario(wOut, outputBuf, 0);
}

// Test 2: All maximum/minimum boundaries to ensure macro correctness
static void test_Pack4BytesSwapSwapFirst_Bounds()
{
    unsigned short wOut[4] = { 0xFFFF, 0x0000, 0x1234, 0xABCD }; // low bytes: FF, 00, 0x34, 0xCD
    unsigned char outputBuf[4];
    runScenario(wOut, outputBuf, 0);
}

// Test 3: Mixed values including zeros to ensure handling of edge cases
static void test_Pack4BytesSwapSwapFirst_EdgeCases()
{
    unsigned short wOut[4] = { 0x0000, 0x00FF, 0x0F0F, 0xF00F }; // low bytes: 0x00, 0xFF, 0x0F, 0x0F
    unsigned char outputBuf[4];
    runScenario(wOut, outputBuf, 123); // non-zero stride to exercise parameter usage
}

// Entry point to run all tests
int main(void)
{
    std::printf("Starting Pack4BytesSwapSwapFirst test suite (C++11).\n");

    test_Pack4BytesSwapSwapFirst_Normal();
    test_Pack4BytesSwapSwapFirst_Bounds();
    test_Pack4BytesSwapSwapFirst_EdgeCases();

    if (g_failures == 0) {
        std::printf("ALL TESTS PASSED\n");
    } else {
        std::printf("TOTAL FAILURES: %d\n", g_failures);
    }

    return g_failures;
}