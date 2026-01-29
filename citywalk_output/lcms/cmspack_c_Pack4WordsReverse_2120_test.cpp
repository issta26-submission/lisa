/*
Unit test suite for Pack4WordsReverse in cmspack.c

Notes:
- This test suite is written for C++11 and does not depend on GoogleTest.
- A tiny in-file test framework is implemented to support non-terminating assertions
  (logging failures without aborting the test run).
- The tests assume the presence of the real Pack4WordsReverse function (linked from cmspack.c)
  and use minimal stub types/macros to simulate the CMS environment.
- The test focuses on verifying the exact 8-byte output produced by Pack4WordsReverse when
  given 4 input 16-bit words, each written with REVERSE_FLAVOR_16 semantics.
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific type aliases (simulate CMS types in a minimal way)
using cmsUInt8Number  = uint8_t;
using cmsUInt16Number = uint16_t;
using cmsUInt32Number = uint32_t;
using cmsBool         = bool;

// CMSREGISTER is a macro used in the original CMS codebase; we provide a neutral placeholder.
typedef void* CMSREGISTER;

// Forward declaration of the focal function from cmspack.c
extern "C" cmsUInt8Number* Pack4WordsReverse(CMSREGISTER _cmsTRANSFORM* info,
                                           CMSREGISTER cmsUInt16Number wOut[],
                                           CMSREGISTER cmsUInt8Number* output,
                                           CMSREGISTER cmsUInt32Number Stride);

// Minimal TRANSFORM struct to satisfy the function signature in tests.
// The real implementation would use a detailed struct; for testing this signature is sufficient.
struct _cmsTRANSFORM { int dummy; };

// Simple in-file assertion framework (non-terminating)
static int g_total_tests  = 0;
static int g_failed_tests = 0;

#define TST_ASSERT(cond, msg) do {                               \
    ++g_total_tests;                                               \
    if(!(cond)) {                                                  \
        ++g_failed_tests;                                          \
        std::cerr << "[FAIL] " << __func__ << ": " << msg << "\n"; \
    }                                                              \
} while(0)

// Helper to compare memory buffers
static bool memory_equal(const uint8_t* a, const uint8_t* b, size_t len) {
    return std::memcmp(a, b, len) == 0;
}

// Helper constant to mirror REVERSE_FLAVOR_16 behavior used by Pack4WordsReverse.
// The original macro flips the two bytes in a 16-bit word.
static inline cmsUInt16Number REVERSE_FLAVOR_16(cmsUInt16Number x) {
    return static_cast<cmsUInt16Number>(((x & 0xFF00) >> 8) | ((x & 0x00FF) << 8));
}

// Test 1: Basic reversal of four words with known values
static void test_Pack4WordsReverse_basic_case1() {
    // Arrange
    cmsUInt16Number wOut[4] = { 0x1234, 0xABCD, 0x0F0F, 0xFFFF };
    cmsUInt8Number  outputBuf[8] = {0};
    cmsUInt8Number*  outputPtr = outputBuf;
    _cmsTRANSFORM   info; // unused in this focused test (Pack4WordsReverse ignores it)

    // Act
    cmsUInt8Number* ret = Pack4WordsReverse(reinterpret_cast<CMSREGISTER &_cmsTRANSFORM>(info),
                                           wOut, outputPtr, 0);

    // Expected (assuming REVERSE_FLAVOR_16 flips bytes):
    // wOut[0] = 0x1234 -> 0x3412 -> bytes 0x12, 0x34
    // wOut[1] = 0xABCD -> 0xCDAB -> bytes 0xAB, 0xCD
    // wOut[2] = 0x0F0F -> 0x0F0F -> bytes 0x0F, 0x0F
    // wOut[3] = 0xFFFF -> 0xFFFF -> bytes 0xFF, 0xFF
    uint8_t expected[8] = { 0x12, 0x34, 0xAB, 0xCD, 0x0F, 0x0F, 0xFF, 0xFF };

    // Assert
    TST_ASSERT(ret == outputPtr + 8, "Return pointer should advance by 8 bytes.");
    TST_ASSERT(memory_equal(outputBuf, expected, 8),
             "Output bytes should be the reversed 16-bit words in correct order.");
}

// Test 2: Basic reversal with different values including zero and mixed patterns
static void test_Pack4WordsReverse_basic_case2() {
    // Arrange
    cmsUInt16Number wOut[4] = { 0x0001, 0x0203, 0x0405, 0x0607 };
    cmsUInt8Number  outputBuf[8] = {0};
    cmsUInt8Number*  outputPtr = outputBuf;
    _cmsTRANSFORM   info;

    // Act
    cmsUInt8Number* ret = Pack4WordsReverse(reinterpret_cast<CMSREGISTER &_cmsTRANSFORM>(info),
                                           wOut, outputPtr, 0);

    // Expected:
    // 0x0001 -> 0x0100 -> bytes 0x00, 0x01
    // 0x0203 -> 0x0302 -> bytes 0x02, 0x03
    // 0x0405 -> 0x0504 -> bytes 0x04, 0x05
    // 0x0607 -> 0x0706 -> bytes 0x06, 0x07
    uint8_t expected[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

    // Assert
    TST_ASSERT(ret == outputPtr + 8, "Return pointer should advance by 8 bytes (second run).");
    TST_ASSERT(memory_equal(outputBuf, expected, 8),
             "Output bytes should reflect proper reversed values for second input set.");
}

// Test 3: All-ones edge case (0xFFFF) to ensure full-range handling
static void test_Pack4WordsReverse_basic_case3() {
    // Arrange
    cmsUInt16Number wOut[4] = { 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF };
    cmsUInt8Number  outputBuf[8] = {0};
    cmsUInt8Number*  outputPtr = outputBuf;
    _cmsTRANSFORM   info;

    // Act
    cmsUInt8Number* ret = Pack4WordsReverse(reinterpret_cast<CMSREGISTER &_cmsTRANSFORM>(info),
                                           wOut, outputPtr, 0);

    uint8_t expected[8] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    // Assert
    TST_ASSERT(ret == outputPtr + 8, "Return pointer should advance by 8 bytes (all-FFFF case).");
    TST_ASSERT(memory_equal(outputBuf, expected, 8),
             "Output bytes should be all 0xFFFF written as reversed 0xFFFF words.");
}

// Test 4: Ensure multiple successive calls produce correct, independent outputs
static void test_Pack4WordsReverse_multiple_calls() {
    // Arrange first input
    cmsUInt16Number wOut1[4] = { 0x1111, 0x2222, 0x3333, 0x4444 };
    cmsUInt8Number  outBuf1[8] = {0};
    cmsUInt8Number*  p1 = Pack4WordsReverse(nullptr, wOut1, outBuf1, 0);

    // Expected 1
    uint8_t expected1[8] = { 0x11, 0x11, 0x22, 0x22, 0x33, 0x33, 0x44, 0x44 };
    // Arrange second input
    cmsUInt16Number wOut2[4] = { 0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD };
    cmsUInt8Number  outBuf2[8] = {0};
    cmsUInt8Number*  p2 = Pack4WordsReverse(nullptr, wOut2, outBuf2, 0);
    uint8_t expected2[8] = { 0xAA, 0xAA, 0xBB, 0xBB, 0xCC, 0xCC, 0xDD, 0xDD };

    // Assert for first call
    TST_ASSERT(p1 == outBuf1 + 8, "First call should advance by 8 bytes.");
    TST_ASSERT(memory_equal(outBuf1, expected1, 8),
             "First call output bytes should match expected reversed values.");

    // Assert for second call
    TST_ASSERT(p2 == outBuf2 + 8, "Second call should advance by 8 bytes.");
    TST_ASSERT(memory_equal(outBuf2, expected2, 8),
             "Second call output bytes should match expected reversed values.");
}

// Entry point to run all tests
static void run_all_tests() {
    test_Pack4WordsReverse_basic_case1();
    test_Pack4WordsReverse_basic_case2();
    test_Pack4WordsReverse_basic_case3();
    test_Pack4WordsReverse_multiple_calls();
}

int main() {
    // Run tests
    run_all_tests();

    // Summary
    int total = g_total_tests;
    int failed = g_failed_tests;
    std::cout << "Pack4WordsReverse test summary: "
              << total << " tests, "
              << failed << " failures.\n";

    // Return non-zero if any test failed
    return (failed > 0) ? 1 : 0;
}