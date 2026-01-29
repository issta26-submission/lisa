/*
Unit Test Suite for Pack3BytesSwap
Focal method location: cmspack.c
Domain assumptions:
- Pack3BytesSwap writes the least-significant byte of wOut[2], then wOut[1], then wOut[0]
- Output pointer is advanced by 3 and returned (end-of-output pointer)
- The function signatures use: CMSREGISTER _cmsTRANSFORM* info, CMSREGISTER cmsUInt16Number wOut[], CMSREGISTER cmsUInt8Number* output, CMSREGISTER cmsUInt32Number Stride
- For testing, we link against the real cmspack.c (extern "C" linkage)
- We provide a lightweight test harness (no GTest) with non-terminating assertions
- Static/public test code is used; we expose minimal types to match the focal function
Candidate Keywords extracted from the focal method and its dependencies:
- Pack3BytesSwap, wOut, output, Stride, _cmsTRANSFORM, FROM_16_TO_8
- cmsUInt8Number, cmsUInt16Number, cmsUInt32Number
- CMSREGISTER, _cmsTRANSFORM (structure)
- NULL / nullptr (info)
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Candidate Keywords (for reference/documentation)
// - Pack3BytesSwap
// - wOut (array of cmsUInt16Number)
// - output (buffer to write to)
// - Stride (unused in focal method, but part of signature)
// - _cmsTRANSFORM (structure type for info parameter)
// - FROM_16_TO_8 macro (convert 16-bit to 8-bit)
// - cmsUInt8Number, cmsUInt16Number, cmsUInt32Number
// - CMSREGISTER (macro for parameter qualifiers)

// Domain knowledge helpers
// - We will not mock private members; only public, function-level behavior is tested
// - We will call test cases from main (no GTest)

// Typedefs and declarations to align with the focal function's signature.
// We assume the real implementation exists in cmspack.c and will be linked accordingly.

typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Macro used in the production code (expected to exist). We'll define a local equivalent for compilation compatibility,
// but the function itself is linked from cmspack.c, so the macro is defined in that translation unit.
// We implement a simple inline conversion here for consistency if needed in test-only contexts.
#ifndef FROM_16_TO_8
#define FROM_16_TO_8(x) (static_cast<cmsUInt8Number>((x) & 0xFFu))
#endif

// Forward-declare the _cmsTRANSFORM type as an incomplete type to match the production signature.
// The real cmspack.c uses a typedef struct _cmsTRANSFORM; we mirror that roughly for linkage compatibility.
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// CMSREGISTER macro is commonly defined as register or empty. For test linkage, we define it as empty.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Prototype of the focal function (extern "C" to match the C linkage of the production code)
extern "C" cmsUInt8Number* Pack3BytesSwap(CMSREGISTER _cmsTRANSFORM* info,
                                         CMSREGISTER cmsUInt16Number wOut[],
                                         CMSREGISTER cmsUInt8Number* output,
                                         CMSREGISTER cmsUInt32Number Stride);

// Lightweight test framework (non-terminating assertions)
static int g_TestFailures = 0;

#define EXPECT_EQ(a,b) do { \
    if (!((a) == (b))) { \
        std::cerr << "[Test Failed] " << __FUNCTION__ << ": " << #a << " != " << #b \
                  << " (" << (a) << " != " << (b) << ")" << std::endl; \
        ++g_TestFailures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
    if ((void*)(a) != (void*)(b)) { \
        std::cerr << "[Test Failed] " << __FUNCTION__ << ": pointer " << #a << " != " << #b \
                  << " (" << (void*)(a) << " != " << (void*)(b) << ")" << std::endl; \
        ++g_TestFailures; \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "[Test Failed] " << __FUNCTION__ << ": Expected true but got false." << std::endl; \
        ++g_TestFailures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if (cond) { \
        std::cerr << "[Test Failed] " << __FUNCTION__ << ": Expected false but got true." << std::endl; \
        ++g_TestFailures; \
    } \
} while(0)

// Test Case 1: Basic behavior with known inputs
// - Verifies that Pack3BytesSwap writes the least significant byte of wOut[2], then wOut[1], then wOut[0]
// - Verifies end pointer is advanced by exactly 3 bytes
// - Verifies the content order matches expected FROM_16_TO_8 behavior
void test_Pack3BytesSwap_basic_ordering() {
    // Dummy info (unused by focal function)
    _cmsTRANSFORM* info = nullptr;

    cmsUInt16Number wOut[3] = { 0x0001, 0x00FF, 0x1234 }; // [0]=0x0001, [1]=0x00FF, [2]=0x1234
    cmsUInt8Number outBuf[16];
    std::memset(outBuf, 0xAA, sizeof(outBuf)); // fill with sentinel values for visibility

    cmsUInt8Number* endPtr = Pack3BytesSwap(info, wOut, outBuf, 0);

    // Expected: lower byte of wOut[2] (0x1234 -> 0x34), then wOut[1] (0x00FF -> 0xFF), then wOut[0] (0x0001 -> 0x01)
    EXPECT_EQ(outBuf[0], static_cast<cmsUInt8Number>(0x34));
    EXPECT_EQ(outBuf[1], static_cast<cmsUInt8Number>(0xFF));
    EXPECT_EQ(outBuf[2], static_cast<cmsUInt8Number>(0x01));

    // End pointer should be exactly 3 bytes past the original output
    EXPECT_PTR_EQ(endPtr, outBuf + 3);
    
    // Ensure buffer beyond written region remains unchanged (sanity)
    EXPECT_EQ(outBuf[3], 0xAA);
    EXPECT_EQ(outBuf[4], 0xAA);
}

// Test Case 2: Another input combination to verify correctness across different values
void test_Pack3BytesSwap_second_input_set() {
    _cmsTRANSFORM* info = nullptr;

    cmsUInt16Number wOut[3] = { 0xABCD, 0x1234, 0x00AA }; // [0]=0xABCD, [1]=0x1234, [2]=0x00AA
    cmsUInt8Number outBuf[16];
    std::memset(outBuf, 0x00, sizeof(outBuf)); // zeroed

    cmsUInt8Number* endPtr = Pack3BytesSwap(info, wOut, outBuf, 0);

    // Expected: wOut[2] low byte 0xAA, wOut[1] low byte 0x34, wOut[0] low byte 0xCD
    EXPECT_EQ(outBuf[0], static_cast<cmsUInt8Number>(0xAA));
    EXPECT_EQ(outBuf[1], static_cast<cmsUInt8Number>(0x34));
    EXPECT_EQ(outBuf[2], static_cast<cmsUInt8Number>(0xCD));

    // End pointer check
    EXPECT_PTR_EQ(endPtr, outBuf + 3);
}

// Test Case 3: Verify that only the first three bytes are modified and no buffer overflow occurs
void test_Pack3BytesSwap_no_overflow() {
    _cmsTRANSFORM* info = nullptr;

    cmsUInt16Number wOut[3] = { 0xFFFF, 0xEEEE, 0xDDDD };
    cmsUInt8Number outBuf[10] = { 0 }; // start with zeros for visibility
    // Pre-fill after the first three bytes to ensure no accidental writes beyond
    outBuf[3] = 0x11; outBuf[4] = 0x22;

    cmsUInt8Number* endPtr = Pack3BytesSwap(info, wOut, outBuf, 0);

    // Check only first three bytes modified
    EXPECT_EQ(outBuf[0], static_cast<cmsUInt8Number>(0xDD)); // wOut[2] low byte
    EXPECT_EQ(outBuf[1], static_cast<cmsUInt8Number>(0xEE)); // wOut[1] low byte
    EXPECT_EQ(outBuf[2], static_cast<cmsUInt8Number>(0xFF)); // wOut[0] low byte

    // The rest should remain unchanged (as initialized)
    EXPECT_EQ(outBuf[3], 0x11);
    EXPECT_EQ(outBuf[4], 0x22);

    // End pointer check
    EXPECT_PTR_EQ(endPtr, outBuf + 3);
}

// Test Case 4: Edge case with zero values to ensure correct zero-byte handling
void test_Pack3BytesSwap_zero_values() {
    _cmsTRANSFORM* info = nullptr;

    cmsUInt16Number wOut[3] = { 0x0000, 0x0000, 0x0000 };
    cmsUInt8Number outBuf[3];

    cmsUInt8Number* endPtr = Pack3BytesSwap(info, wOut, outBuf, 0);

    EXPECT_EQ(outBuf[0], static_cast<cmsUInt8Number>(0x00));
    EXPECT_EQ(outBuf[1], static_cast<cmsUInt8Number>(0x00));
    EXPECT_EQ(outBuf[2], static_cast<cmsUInt8Number>(0x00));
    EXPECT_PTR_EQ(endPtr, outBuf + 3);
}

// Main test runner
int main() {
    std::cout << "Running Pack3BytesSwap test suite (non-GTest)\n";

    test_Pack3BytesSwap_basic_ordering();
    test_Pack3BytesSwap_second_input_set();
    test_Pack3BytesSwap_no_overflow();
    test_Pack3BytesSwap_zero_values();

    if (g_TestFailures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_TestFailures << " test(s) failed." << std::endl;
        return 1;
    }
}