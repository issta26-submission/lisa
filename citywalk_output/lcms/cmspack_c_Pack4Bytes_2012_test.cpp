// Test suite for Pack4Bytes in cmspack.c
// This test focuses on validating the behavior of the focal method:
// cmsUInt8Number* Pack4Bytes(CMSREGISTER _cmsTRANSFORM* info,
//                            CMSREGISTER cmsUInt16Number wOut[],
//                            CMSREGISTER cmsUInt8Number* output,
//                            CMSREGISTER cmsUInt32Number Stride);
 
// Notes and assumptions based on the provided source:
// - Pack4Bytes writes four 8-bit values derived from four 16-bit inputs.
// - The conversion macro FROM_16_TO_8 is assumed to extract the high byte
//   ((w >> 8) & 0xFF). If the upstream macro differs, adjust expectations accordingly.
// - The function should return a pointer advanced by 4 bytes (the end pointer after writes).
// - The info and Stride parameters are unused (via cmsUNUSED_PARAMETER) but must be
//   accepted by the signature for correct linkage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Domain-specific type and macro stubs to enable compilation of the test harness.
// These mirror the likely definitions found in cms runtime headers.

#define CMSREGISTER
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

struct _cmsTRANSFORM;                 // Forward declaration for opaque type
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Macro used to convert 16-bit to 8-bit according to assumed upstream behavior.
// If the real macro uses a different conversion, adjust accordingly.
#define FROM_16_TO_8(x) ((cmsUInt8Number)((x) >> 8))

// Unused parameter helper to avoid warnings in C++.
#define cmsUNUSED_PARAMETER(x) (void)(x)

// Access to the actual function under test (C linkage).
extern "C" cmsUInt8Number* Pack4Bytes(CMSREGISTER _cmsTRANSFORM* info,
                                    CMSREGISTER cmsUInt16Number wOut[],
                                    CMSREGISTER cmsUInt8Number* output,
                                    CMSREGISTER cmsUInt32Number Stride);

// Minimal test framework (non-terminating assertions) to maximize coverage.
// It reports failures but continues executing subsequent tests.

static int g_failureCount = 0;

static void reportFailure(const char* file, int line, const char* cond) {
    std::cerr << "TEST FAILURE: " << file << ":" << line
              << " - Condition failed: " << cond << std::endl;
    ++g_failureCount;
}

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { reportFailure(__FILE__, __LINE__, #cond); } \
} while(false)

#define EXPECT_EQ_U8(a, b) do { \
    if(((cmsUInt8Number)(a)) != ((cmsUInt8Number)(b))) { \
        reportFailure(__FILE__, __LINE__, "EXPECT_EQ_U8"); \
    } \
} while(false)

#define EXPECT_EQ_PTR(a, b) do { \
    if((void*)(a) != (void*)(b)) { \
        reportFailure(__FILE__, __LINE__, "EXPECT_EQ_PTR"); \
    } \
} while(false)

// Test 1: Basic correctness - four inputs mapped to four bytes via high-byte extraction.
// Verifies the end pointer and the content of the 4-byte output.
void test_Pack4Bytes_basic() {
    // Arrange
    cmsUInt16Number wOut[4] = { 0x1234, 0xABCD, 0x00FF, 0xF0F0 };
    cmsUInt8Number output[4] = { 0, 0, 0, 0 };

    // Act
    cmsUInt8Number* endPtr = Pack4Bytes(nullptr, wOut, output, 0);

    // Assert
    // End pointer should point exactly after the 4 written bytes
    EXPECT_TRUE(endPtr == output + 4);

    // Expected high-byte extraction: 0x12, 0xAB, 0x00, 0xF0
    EXPECT_EQ_U8(output[0], 0x12);
    EXPECT_EQ_U8(output[1], 0xAB);
    EXPECT_EQ_U8(output[2], 0x00);
    EXPECT_EQ_U8(output[3], 0xF0);
}

// Test 2: Verify behavior with a non-zero Stride (parameter is unused in Pack4Bytes).
// Ensures output integrity is independent of Stride value.
void test_Pack4Bytes_withStride() {
    // Arrange
    cmsUInt16Number wOut[4] = { 0x0102, 0x0304, 0x0506, 0x0708 };
    cmsUInt8Number output[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

    // Act
    cmsUInt8Number* endPtr = Pack4Bytes(nullptr, wOut, output, 12345U);

    // Assert
    EXPECT_TRUE(endPtr == output + 4);

    // High-byte extraction for inputs: 0x01, 0x03, 0x05, 0x07
    EXPECT_EQ_U8(output[0], 0x01);
    EXPECT_EQ_U8(output[1], 0x03);
    EXPECT_EQ_U8(output[2], 0x05);
    EXPECT_EQ_U8(output[3], 0x07);
}

// Test 3: Mixed inputs to ensure no assumptions about output buffer content carry over.
// Validates that a subsequent call writes anew without relying on previous state.
void test_Pack4Bytes_multiple_calls_separate_buffers() {
    // Arrange
    cmsUInt16Number wOutA[4] = { 0xA1B2, 0xC3D4, 0xE5F6, 0x0708 };
    cmsUInt8Number outA[4] = { 0, 0, 0, 0 };

    cmsUInt16Number wOutB[4] = { 0x1111, 0x2222, 0x3333, 0x4444 };
    cmsUInt8Number outB[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

    // Act
    cmsUInt8Number* endA = Pack4Bytes(nullptr, wOutA, outA, 0);
    cmsUInt8Number* endB = Pack4Bytes(nullptr, wOutB, outB, 0);

    // Assert for first buffer
    EXPECT_TRUE(endA == outA + 4);
    EXPECT_EQ_U8(outA[0], 0xA1);
    EXPECT_EQ_U8(outA[1], 0xC3);
    EXPECT_EQ_U8(outA[2], 0xE5);
    EXPECT_EQ_U8(outA[3], 0x07);

    // Assert for second buffer
    EXPECT_TRUE(endB == outB + 4);
    EXPECT_EQ_U8(outB[0], 0x11);
    EXPECT_EQ_U8(outB[1], 0x22);
    EXPECT_EQ_U8(outB[2], 0x33);
    EXPECT_EQ_U8(outB[3], 0x44);
}

// Main test runner
int main() {
    // Run tests
    test_Pack4Bytes_basic();
    test_Pack4Bytes_withStride();
    test_Pack4Bytes_multiple_calls_separate_buffers();

    // Summary
    if (g_failureCount == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failureCount << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}