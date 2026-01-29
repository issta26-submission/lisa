// Test suite for the focal method Pack3WordsAndSkip1SwapSwapFirst
// This test suite is designed to be compiled with a C++11 compiler without GoogleTest.
// It provides lightweight, non-terminating assertions and a small harness to
// validate the behavior of the focal function as implemented in cmsPack.c.
//
// Step 1: Program Understanding (summary in comments)
// - The function Pack3WordsAndSkip1SwapSwapFirst takes an input array of 3
//   16-bit words (wOut[0..2]) and writes them into the output buffer in reverse
//   order as 16-bit values: wOut[2], wOut[1], wOut[0].
// - It writes 2 bytes per word, advancing by 2 after each write, and finally
//   advances output by an additional 2 bytes before returning (total 8 bytes
//   written/consumed from the base pointer). The returned pointer equals base
//   output pointer + 8.
// - info and Stride are unused within the function (they are marked as unused
//   parameters).
// - We test for correct content (in 16-bit units) and correct return address.
// Step 2: Unit Test Generation (covering key behavior)
// - Test 1: Basic non-zero values, verify reversed write order, and return pointer.
// - Test 2: All zeros to ensure content handling is correct for zero inputs.
// - Test 3: Non-zero Stride and distinct values to exercise typical usage.
// - Additionally verify that bytes at positions 6-7 (the trailing two bytes not written) remain unchanged.
// Step 3: Test Case Refinement (domain knowledge)
// - Use direct memory interpretation to check 16-bit words written into the buffer.
// - Use non-terminating assertions that print failures but continue running subsequent tests.
// - Do not rely on any GoogleTest; provide a minimal test harness in main().
//
// In order to make this test self-contained for demonstration, we declare minimal
// compatible types and an extern "C" prototype for the focal function, assuming
// the actual implementation is provided by the cmsPack.c linkage in the test
// environment.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight compatibility layer to mimic cms types used by the focal function.
#define CMSREGISTER
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Opaque transform struct to satisfy the function signature.
// The actual definition is not needed for these tests.
struct _cmsTRANSFORM {};

// Export-compatible prototype (C linkage) for the focal function under test.
extern "C" {
    cmsUInt8Number* Pack3WordsAndSkip1SwapSwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                                    CMSREGISTER cmsUInt16Number wOut[],
                                                    CMSREGISTER cmsUInt8Number* output,
                                                    CMSREGISTER cmsUInt32Number Stride);
}

// Simple non-terminating assertion helper.
// It prints a message on failure but does not abort the test suite.
static int g_testsFailed = 0;
#define CHECK_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << desc \
                  << " | Expected: " << +static_cast<int>(expected) \
                  << ", Actual: " << +static_cast<int>(actual) << std::endl; \
        ++g_testsFailed; \
    } else { \
        std::cout << "[PASS] " << desc << std::endl; \
    } \
} while(0)

#define CHECK_PTR_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << desc \
                  << " | Expected pointer: " << static_cast<void*>(expected) \
                  << ", Actual pointer: " << static_cast<void*>(actual) << std::endl; \
        ++g_testsFailed; \
    } else { \
        std::cout << "[PASS] " << desc << std::endl; \
    } \
} while(0)

// Test 1: Basic non-zero values; verify reversed write order and return value.
static void test_Pack3WordsAndSkip1SwapSwapFirst_Basic() {
    // Arrange
    cmsUInt16Number wOut[3] = { 0x1122, 0x3344, 0x5566 }; // wOut[0], wOut[1], wOut[2]
    cmsUInt8Number buffer[8];
    // Pre-fill the trailing two bytes to detect inadvertent writes beyond the required region
    buffer[6] = 0xAA;
    buffer[7] = 0xAA;
    // Ensure the rest is known
    std::memset(buffer, 0x00, 6);

    // Act
    cmsUInt8Number* ret = Pack3WordsAndSkip1SwapSwapFirst(nullptr, wOut, buffer, 0);

    // Assert
    // Interpret first 3 words written as 16-bit units in memory order.
    cmsUInt16Number* writtenWords = reinterpret_cast<cmsUInt16Number*>(buffer);
    // Expect: buffer[0..1] == wOut[2], buffer[2..3] == wOut[1], buffer[4..5] == wOut[0]
    CHECK_EQ(writtenWords[0], wOut[2], "Word 0 should be wOut[2]");
    CHECK_EQ(writtenWords[1], wOut[1], "Word 1 should be wOut[1]");
    CHECK_EQ(writtenWords[2], wOut[0], "Word 2 should be wOut[0]");

    // Return pointer should be base + 8
    CHECK_PTR_EQ(ret, buffer + 8, "Return pointer should be buffer + 8");

    // Trailing bytes (positions 6 and 7) should remain unchanged (0xAA)
    CHECK_EQ(buffer[6], 0xAA, "Trailing byte 6 should remain unchanged");
    CHECK_EQ(buffer[7], 0xAA, "Trailing byte 7 should remain unchanged");
}

// Test 2: All zeros; verify content and return pointer behavior with zero inputs.
static void test_Pack3WordsAndSkip1SwapSwapFirst_AllZeros() {
    // Arrange
    cmsUInt16Number wOut[3] = { 0x0000, 0x0000, 0x0000 };
    cmsUInt8Number buffer[8];
    // Pre-fill trailing bytes
    buffer[6] = 0x13;
    buffer[7] = 0x37;
    std::memset(buffer, 0x00, 6);

    // Act
    cmsUInt8Number* ret = Pack3WordsAndSkip1SwapSwapFirst(nullptr, wOut, buffer, 0);

    // Assert
    cmsUInt16Number* writtenWords = reinterpret_cast<cmsUInt16Number*>(buffer);
    CHECK_EQ(writtenWords[0], (cmsUInt16Number)0x0000, "Word 0 should be 0x0000");
    CHECK_EQ(writtenWords[1], (cmsUInt16Number)0x0000, "Word 1 should be 0x0000");
    CHECK_EQ(writtenWords[2], (cmsUInt16Number)0x0000, "Word 2 should be 0x0000");

    CHECK_PTR_EQ(ret, buffer + 8, "Return pointer should be buffer + 8 for AllZeros");

    // Trailing bytes should remain unchanged
    CHECK_EQ(buffer[6], 0x13, "Trailing byte 6 should remain unchanged");
    CHECK_EQ(buffer[7], 0x37, "Trailing byte 7 should remain unchanged");
}

// Test 3: Non-zero Stride and distinct values; ensure proper behavior with Stride != 0.
static void test_Pack3WordsAndSkip1SwapSwapFirst_WithStride() {
    // Arrange
    cmsUInt16Number wOut[3] = { 0x0102, 0x0304, 0x0506 };
    cmsUInt8Number buffer[8];
    buffer[6] = 0xFF;
    buffer[7] = 0xEE;
    std::memset(buffer, 0x11, 6);

    // Act
    cmsUInt8Number* ret = Pack3WordsAndSkip1SwapSwapFirst(nullptr, wOut, buffer, 7); // Stride is unused, but non-zero

    // Assert
    cmsUInt16Number* writtenWords = reinterpret_cast<cmsUInt16Number*>(buffer);
    CHECK_EQ(writtenWords[0], wOut[2], "Word 0 should be wOut[2] for WithStride");
    CHECK_EQ(writtenWords[1], wOut[1], "Word 1 should be wOut[1] for WithStride");
    CHECK_EQ(writtenWords[2], wOut[0], "Word 2 should be wOut[0] for WithStride");

    CHECK_PTR_EQ(ret, buffer + 8, "Return pointer should be buffer + 8 for WithStride");

    // Trailing bytes should remain unchanged
    CHECK_EQ(buffer[6], 0xFF, "Trailing byte 6 should remain unchanged with stride");
    CHECK_EQ(buffer[7], 0xEE, "Trailing byte 7 should remain unchanged with stride");
}

int main() {
    // Run tests
    std::cout << "Starting Pack3WordsAndSkip1SwapSwapFirst test suite (C++11)..." << std::endl;

    test_Pack3WordsAndSkip1SwapSwapFirst_Basic();
    test_Pack3WordsAndSkip1SwapSwapFirst_AllZeros();
    test_Pack3WordsAndSkip1SwapSwapFirst_WithStride();

    if (g_testsFailed == 0) {
        std::cout << "[ALL TESTS PASSED] Pack3WordsAndSkip1SwapSwapFirst" << std::endl;
        return 0;
    } else {
        std::cerr << "[TOTAL FAILURES] " << g_testsFailed << " test(s) failed." << std::endl;
        return 1;
    }
}