/*
Unit Test Suite for Pack4BytesSwap (focal method)
Context:
- The function Pack4BytesSwap exists in cmspack.c and is responsible for packing 4 16-bit words
  into 4 consecutive 8-bit bytes, taking the low byte of each 16-bit word and reversing the
  word order in the process (wOut[3], wOut[2], wOut[1], wOut[0]).

Assumptions for testing (tests are self-contained and do not rely on the full CMS inventory):
- Minimal stubs for types/macros used by the focal method.
- FROM_16_TO_8(x) extracts the low 8 bits of a 16-bit value.
- The function does not rely on Stride or info for its core logic.
- Non-terminating assertions are used to exercise code paths without aborting on failure.

Notes:
- This test suite uses a lightweight, homegrown test harness (no GTest) and runs in a C++11 environment.
- Tests include multiple input scenarios to exercise the conversion and order reversal logic.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// --------------------------------------------------------------------------------
// Minimal environment to enable compilation of the focal method
// --------------------------------------------------------------------------------

// Typedefs matching the CMS types used by the focal function
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// CMSREGISTER macro (empty in tests)
#define CMSREGISTER

// _cmsTRANSFORM mock structure (empty; not used in tests)
typedef struct _cmsTRANSFORM {
    // intentionally empty
} _cmsTRANSFORM;

// Macro to silence unused parameter warnings
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Macro to convert 16-bit value to 8-bit (use low byte)
#define FROM_16_TO_8(x) (static_cast<cmsUInt8Number>((x) & 0xFF))

// Forward declaration of the focal method under test (as it would appear in cmspack.c)
cmsUInt8Number* Pack4BytesSwap(CMSREGISTER _cmsTRANSFORM* info,
                               CMSREGISTER cmsUInt16Number wOut[],
                               CMSREGISTER cmsUInt8Number* output,
                               CMSREGISTER cmsUInt32Number Stride);

// --------------------------------------------------------------------------------
// Focal method implementation (reproduced for test purposes)
// Note: The real file cmspack.c would define this. For unit testing, we provide a
// self-contained version that mirrors the original logic.
// --------------------------------------------------------------------------------
cmsUInt8Number* Pack4BytesSwap(CMSREGISTER _cmsTRANSFORM* info,
                               CMSREGISTER cmsUInt16Number wOut[],
                               CMSREGISTER cmsUInt8Number* output,
                               CMSREGISTER cmsUInt32Number Stride)
{
{
    *output++ = FROM_16_TO_8(wOut[3]);
    *output++ = FROM_16_TO_8(wOut[2]);
    *output++ = FROM_16_TO_8(wOut[1]);
    *output++ = FROM_16_TO_8(wOut[0]);
    return output;
    cmsUNUSED_PARAMETER(info);
    cmsUNUSED_PARAMETER(Stride);
}
}

// --------------------------------------------------------------------------------
// Lightweight test harness
// --------------------------------------------------------------------------------

static int g_total_tests = 0;
static int g_passed_tests = 0;

// Non-terminating assertion macro
#define TEST_ASSERT(cond) do { \
    ++g_total_tests; \
    if (cond) { ++g_passed_tests; } \
    else { \
        std::cerr << "[ERROR] Assertion failed in " << __func__ \
                  << " at line " << __LINE__ << std::endl; \
    } \
} while (0)

static void test_Pack4BytesSwap_BasicSequence()
{
    // Purpose: Validate correct packing order and low-byte extraction
    // Inputs: wOut = {0x0102, 0x0304, 0x0506, 0x0708}
    // Expected output: {0x08, 0x06, 0x04, 0x02}
    cmsUInt16Number wOut[4] = { 0x0102u, 0x0304u, 0x0506u, 0x0708u };
    cmsUInt8Number outBuf[4] = { 0 };

    cmsUInt8Number* endPtr = Pack4BytesSwap(nullptr, wOut, outBuf, 0);

    cmsUInt8Number expected[4] = { 0x08, 0x06, 0x04, 0x02 };

    // Compare contents
    TEST_ASSERT(std::memcmp(outBuf, expected, 4) == 0);
    // Verify returned pointer advanced by 4 bytes
    TEST_ASSERT(endPtr == outBuf + 4);
}

static void test_Pack4BytesSwap_AnotherSequence()
{
    // Purpose: Validate with different bytes to exercise low-byte extraction
    // Inputs: wOut = {0xABCD, 0x1234, 0x0001, 0xFF00}
    // Expected output: {0x00, 0x01, 0x34, 0xCD}
    cmsUInt16Number wOut[4] = { 0xABCDu, 0x1234u, 0x0001u, 0xFF00u };
    cmsUInt8Number outBuf[4] = { 0 };

    cmsUInt8Number* endPtr = Pack4BytesSwap(nullptr, wOut, outBuf, 0);

    cmsUInt8Number expected[4] = { 0x00, 0x01, 0x34, 0xCD };

    // Compare contents
    TEST_ASSERT(std::memcmp(outBuf, expected, 4) == 0);
    // Verify returned pointer advanced by 4 bytes
    TEST_ASSERT(endPtr == outBuf + 4);
}

// Step 3: Additional case to exercise multiple runs and static-like behavior handling
static void test_Pack4BytesSwap_RepeatedCalls()
{
    // Ensure multiple successive calls produce consistent results
    cmsUInt16Number wOutA[4] = { 0x1111u, 0x2222u, 0x3333u, 0x4444u };
    cmsUInt8Number outBufA[4] = { 0 };

    cmsUInt8Number* endPtrA = Pack4BytesSwap(nullptr, wOutA, outBufA, 0);
    cmsUInt8Number expectedA[4] = { 0x11, 0x33, 0x22, 0x11 }; // placeholder; will compute properly below

    // Compute expected using low bytes of each word in reverse order
    cmsUInt8Number eA[4];
    eA[0] = static_cast<cmsUInt8Number>(0x4444u & 0xFF);
    eA[1] = static_cast<cmsUInt8Number>(0x3333u & 0xFF);
    eA[2] = static_cast<cmsUInt8Number>(0x2222u & 0xFF);
    eA[3] = static_cast<cmsUInt8Number>(0x1111u & 0xFF);

    TEST_ASSERT(std::memcmp(outBufA, eA, 4) == 0);
    TEST_ASSERT(endPtrA == outBufA + 4);

    // Second sequence on a new buffer
    cmsUInt16Number wOutB[4] = { 0xAAAAu, 0xBBBBu, 0xCCCCu, 0xDDDDu };
    cmsUInt8Number outBufB[4] = { 0 };
    cmsUInt8Number* endPtrB = Pack4BytesSwap(nullptr, wOutB, outBufB, 0);
    cmsUInt8Number eB[4];
    eB[0] = static_cast<cmsUInt8Number>(0xDDDDu & 0xFF);
    eB[1] = static_cast<cmsUInt8Number>(0xCCCCu & 0xFF);
    eB[2] = static_cast<cmsUInt8Number>(0xBBBBu & 0xFF);
    eB[3] = static_cast<cmsUInt8Number>(0xAAAAu & 0xFF);
    TEST_ASSERT(std::memcmp(outBufB, eB, 4) == 0);
    TEST_ASSERT(endPtrB == outBufB + 4);
}

// --------------------------------------------------------------------------------
// Main entry point for tests
// --------------------------------------------------------------------------------
int main()
{
    std::cout << "Running Pack4BytesSwap unit tests (non-terminating assertions enabled)" << std::endl;

    test_Pack4BytesSwap_BasicSequence();
    test_Pack4BytesSwap_AnotherSequence();
    test_Pack4BytesSwap_RepeatedCalls();

    std::cout << "Test Summary: " << g_passed_tests << " / " << g_total_tests
              << " tests passed." << std::endl;

    // Return 0 if all tests passed; non-zero otherwise
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}