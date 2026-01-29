/*
  Lightweight C++11 unit test suite for the focal method:
  Unroll4BytesSwap in cms2/cmspack.c

  Approach:
  - Provide minimal, non-terminating test assertions (EXPECT_*) to maximize code execution.
  - Create two test cases that validate the behavior of Unroll4BytesSwap:
      - It reads 4 bytes from the input accumulator and writes them into wIn in reverse order (indices 3..0).
      - It advances the accum pointer by 4 bytes (returns accum + 4).
  - Use extern "C" prototype to link against the C function provided in cms spacings.
  - Do not rely on GTest; implement a tiny harness with simple assertions and print results.
  - Access to types/macros is via lcms2_internal.h as per the repository structure.
  - Keep tests self-contained and provide comments explaining the intent of each test.

  Important notes:
  - This test assumes the macro FROM_8_TO_16(x) yields a 16-bit value equivalent to the 8-bit input (i.e., no endian-flipping beyond casting to 16 bits). The tests verify the numeric contents as cast to 16-bit.
  - We call Unroll4BytesSwap with a NULL info pointer and Stride = 0 since these parameters are unused in the focal implementation.
  - The file relies on the project’s existing types (cmsUInt8Number, cmsUInt16Number, cmsUInt32Number, _cmsTRANSFORM, etc.) being defined in lcms2_internal.h.

  Candidate Keywords (Step 1):
  - Unroll4BytesSwap, cmsUInt16Number, cmsUInt8Number, cmsUInt32Number, cmsUInt8Number*, _cmsTRANSFORM, FROM_8_TO_16, accum, Stride, info
*/

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>


// Include core internal headers to obtain the necessary CMS types/macros.
// The repository structure indicates dependencies are declared in lcms2_internal.h.

// Forward declaration to ensure correct linkage in C++ (extern "C" to match C linkage)
extern "C" cmsUInt8Number* Unroll4BytesSwap(CMSREGISTER _cmsTRANSFORM* info,
                                           CMSREGISTER cmsUInt16Number wIn[],
                                           CMSREGISTER cmsUInt8Number* accum,
                                           CMSREGISTER cmsUInt32Number Stride);

// Minimal test harness (non-terminating assertions)
static int g_current_failures = 0;
static void TEST_BEGIN() { g_current_failures = 0; }
static bool TEST_END() { return g_current_failures == 0; }
#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            printf("    [FAILED] Expectation failed: %s:%d: %s is false\n", __FILE__, __LINE__, #cond); \
            ++g_current_failures; \
        } \
    } while (0)
#define EXPECT_EQ(a, b) \
    do { \
        if( (static_cast<decltype(a)>(a)) != (static_cast<decltype(b)>(b)) ) { \
            printf("    [FAILED] Expectation failed: %s:%d: %s == %s, actual %lld vs %lld\n", __FILE__, __LINE__, #a, #b, (long long)(a), (long long)(b)); \
            ++g_current_failures; \
        } \
    } while (0)


// Test 1: Basic behavior - ensure 4 input bytes are read and written in reverse order.
// Verifies: accum advances by 4, and wIn contents correspond to the reversed input bytes.
// Candidate Keywords: Unroll4BytesSwap, FROM_8_TO_16, wIn[], accum, Stride, info
static bool Test_Unroll4BytesSwap_Basic() {
    TEST_BEGIN();

    // Prepare 8 bytes of input to observe the first 4 consumed bytes
    cmsUInt8Number src[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 };

    cmsUInt8Number* accum = src;            // points to first input byte
    _cmsTRANSFORM* info = nullptr;           // unused in this focal function
    cmsUInt32Number Stride = 0;              // unused in this focal function

    cmsUInt8Number* end = Unroll4BytesSwap(info, wIn, accum, Stride);

    // Expectation 1: accum advanced by 4 bytes
    EXPECT_TRUE(end == src + 4);

    // Expectation 2: wIn contents are reversed input bytes into 16-bit slots
    // wIn[0] should contain the 4th input byte (value 4)
    // wIn[1] should contain the 3rd input byte (value 3)
    // wIn[2] should contain the 2nd input byte (value 2)
    // wIn[3] should contain the 1st input byte (value 1)
    EXPECT_EQ(wIn[0], (cmsUInt16Number)src[3]);
    EXPECT_EQ(wIn[1], (cmsUInt16Number)src[2]);
    EXPECT_EQ(wIn[2], (cmsUInt16Number)src[1]);
    EXPECT_EQ(wIn[3], (cmsUInt16Number)src[0]);

    return TEST_END();
}


// Test 2: Alternative data set to ensure consistent reversal across different byte values.
// Verifies the same reversal behavior with a distinct byte sequence.
// Candidate Keywords: Unroll4BytesSwap, accum, wIn[]
static bool Test_Unroll4BytesSwap_AltDataset() {
    TEST_BEGIN();

    cmsUInt8Number src[8] = { 0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44 };
    cmsUInt16Number wIn[4] = { 0, 0, 0, 0 };

    cmsUInt8Number* accum = src;
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* end = Unroll4BytesSwap(info, wIn, accum, Stride);

    // Expectation 1: accum advanced by 4 bytes
    EXPECT_TRUE(end == src + 4);

    // Expectation 2: reversed 4 bytes into 16-bit slots
    EXPECT_EQ(wIn[0], (cmsUInt16Number)src[3]); // 0xDD
    EXPECT_EQ(wIn[1], (cmsUInt16Number)src[2]); // 0xCC
    EXPECT_EQ(wIn[2], (cmsUInt16Number)src[1]); // 0xBB
    EXPECT_EQ(wIn[3], (cmsUInt16Number)src[0]); // 0xAA

    return TEST_END();
}


// Main - run tests and report status
int main() {
    printf("Starting tests for Unroll4BytesSwap...\n");

    bool t1 = Test_Unroll4BytesSwap_Basic();
    printf("Test_Unroll4BytesSwap_Basic: %s\n", t1 ? "PASS" : "FAIL");

    bool t2 = Test_Unroll4BytesSwap_AltDataset();
    printf("Test_Unroll4BytesSwap_AltDataset: %s\n", t2 ? "PASS" : "FAIL");

    int failures = (!t1) + (!t2);
    if (failures == 0) {
        printf("All tests PASSED.\n");
    } else {
        printf("Total failures: %d\n", failures);
    }

    return failures;
}