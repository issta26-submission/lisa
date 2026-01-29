// Test suite for Pack1ByteReversed in cmspack.c
// This test is designed to be compiled alongside the real cmspack.c
// in a C++11 compliant environment without using GoogleTest.
// The tests are self-contained and rely on a minimal external
// interface that mirrors the expected function signature used by
// the original codebase.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// -----------------------------------------------------------------------------
// Lightweight, test-friendly type and prototype definitions
// These mimic only what's needed to link against the real function in cmspack.c.
// Do not rely on these in production; they are placeholders to enable testing.
// -----------------------------------------------------------------------------

typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;

// Forward declaration of the transform struct as used by the API.
// The real cmsTRANSFORM type is opaque in the tested function.
struct _cmsTRANSFORM;
typedef struct _cmsTRANSFORM cmsTRANSFORM;

// The CMSREGISTER macro is used in the original code; in the test harness
// we define it as empty (it's typically used for compiler hints and can be ignored).
#define CMSREGISTER

extern "C" {

// Prototype of the focal function under test.
// Note: The real implementation is in cmspack.c; this prototype must match.
// We intentionally avoid pulling in cmspack's internal headers here.
cmsUInt8Number* Pack1ByteReversed(CMSREGISTER cmsTRANSFORM* info,
                                 CMSREGISTER cmsUInt16Number wOut[],
                                 CMSREGISTER cmsUInt8Number* output,
                                 CMSREGISTER cmsUInt32Number Stride);
}

// -----------------------------------------------------------------------------
// Test helpers and test cases
// -----------------------------------------------------------------------------

// Simple test harness to accumulate failures without aborting on first error.
struct TestRunner {
    int failures;

    TestRunner() : failures(0) {}

    static void logFailure(const char* testName, int caseNum, const char* detail) {
        std::cerr << "Test failed: " << testName << " (case " << caseNum << ")"
                  << " - " << detail << std::endl;
    }

    static void logSuccess(const char* testName, int caseNum) {
        std::cout << "Test passed: " << testName << " (case " << caseNum << ")" << std::endl;
    }

    void expect(bool condition, const char* testName, int caseNum, const char* detail) {
        if (condition) {
            logSuccess(testName, caseNum);
        } else {
            ++failures;
            logFailure(testName, caseNum, detail);
        }
    }
};

// Helper: compute the expected 8-bit value according to the effective behavior
// of Pack1ByteReversed as implemented in cms code:
// expected byte = low byte of the 16-bit value after byte-reversal.
// For a 16-bit x = [MSB: x>>8, LSB: x & 0xFF], reversed 16-bit is (LSB<<8) | (MSB).
// The low byte of the reversed value is (x >> 8) & 0xFF.
static cmsUInt8Number expectedByteFromWord(cmsUInt16Number word) {
    return static_cast<cmsUInt8Number>((word >> 8) & 0xFF);
}

// Test 1: Basic value mapping for a representative word
// - Verifies that Pack1ByteReversed writes the correct reversed-byte-derived 8-bit value
// - Verifies returned pointer equals original output pointer + 1
void test_Pack1ByteReversed_BasicValue(TestRunner& runner) {
    // Prepare input
    cmsUInt16Number wOut[1] = { 0x1234 }; // 0x1234 -> reversed 0x3412 -> low byte 0x12
    cmsUInt8Number outputBuffer[4] = { 0xAA, 0xBB, 0xCC, 0xDD }; // pre-fill to detect modifications
    cmsUInt8Number* origOutPtr = outputBuffer;

    // Call the function (info is unused in the test; pass nullptr)
    cmsUInt8Number* ret = Pack1ByteReversed(nullptr, wOut, outputBuffer, 0);

    // Expected result
    cmsUInt8Number expected = expectedByteFromWord(wOut[0]);
    // Validate: first byte written equals expected
    runner.expect(outputBuffer[0] == expected,
                  "Pack1ByteReversed_BasicValue",
                  1,
                  "Output byte does not match expected reversed-byte result.");

    // Validate: returned pointer points to the byte after the written one
    runner.expect(ret == origOutPtr + 1,
                  "Pack1ByteReversed_BasicValue",
                  2,
                  "Returned pointer should be advanced by one element from the output buffer.");

    // Validate that no other bytes were modified (beyond the written position)
    runner.expect(outputBuffer[1] == 0xBB && outputBuffer[2] == 0xCC && outputBuffer[3] == 0xDD,
                  "Pack1ByteReversed_BasicValue",
                  3,
                  "Bytes after the written position should remain unchanged.");
}

// Test 2: Different input value to ensure consistent behavior
// - Uses 0xABCD to verify the mapping: (0xABCD >> 8) & 0xFF == 0xAB
void test_Pack1ByteReversed_DifferentValue(TestRunner& runner) {
    cmsUInt16Number wOut[1] = { 0xABCD };
    cmsUInt8Number outputBuffer[4] = { 0x00, 0x00, 0x00, 0x00 };
    cmsUInt8Number* ret = Pack1ByteReversed(nullptr, wOut, outputBuffer, 0);

    cmsUInt8Number expected = expectedByteFromWord(wOut[0]);
    runner.expect(outputBuffer[0] == expected,
                  "Pack1ByteReversed_DifferentValue",
                  1,
                  "Output byte for 0xABCD should be 0xAB.");
    runner.expect(ret == outputBuffer + 1,
                  "Pack1ByteReversed_DifferentValue",
                  2,
                  "Returned pointer should point to next position.");
}

// Test 3: Zero input value
void test_Pack1ByteReversed_ZeroValue(TestRunner& runner) {
    cmsUInt16Number wOut[1] = { 0x0000 };
    cmsUInt8Number outputBuffer[2] = { 0x11, 0x22 };
    cmsUInt8Number* ret = Pack1ByteReversed(nullptr, wOut, outputBuffer, 0);

    cmsUInt8Number expected = expectedByteFromWord(wOut[0]); // 0x00
    runner.expect(outputBuffer[0] == expected,
                  "Pack1ByteReversed_ZeroValue",
                  1,
                  "Output byte for 0x0000 should be 0x00.");
    runner.expect(ret == outputBuffer + 1,
                  "Pack1ByteReversed_ZeroValue",
                  2,
                  "Returned pointer should point to next position.");
}

// Test 4: Maximum byte value input
void test_Pack1ByteReversed_MaxValue(TestRunner& runner) {
    cmsUInt16Number wOut[1] = { 0xFFFF };
    cmsUInt8Number outputBuffer[2] = { 0xAA, 0xBB };
    cmsUInt8Number* ret = Pack1ByteReversed(nullptr, wOut, outputBuffer, 0);

    cmsUInt8Number expected = expectedByteFromWord(wOut[0]); // (0xFFFF >> 8) & 0xFF = 0xFF
    runner.expect(outputBuffer[0] == expected,
                  "Pack1ByteReversed_MaxValue",
                  1,
                  "Output byte for 0xFFFF should be 0xFF.");
    runner.expect(ret == outputBuffer + 1,
                  "Pack1ByteReversed_MaxValue",
                  2,
                  "Returned pointer should point to next position.");
}

// -----------------------------------------------------------------------------
// Entry point
// -----------------------------------------------------------------------------

int main() {
    TestRunner runner;

    // Run the test suite for Pack1ByteReversed
    // Explanatory comments accompany each test to clarify intent and coverage.
    test_Pack1ByteReversed_BasicValue(runner);
    test_Pack1ByteReversed_DifferentValue(runner);
    test_Pack1ByteReversed_ZeroValue(runner);
    test_Pack1ByteReversed_MaxValue(runner);

    if (runner.failures == 0) {
        std::cout << "All tests passed for Pack1ByteReversed." << std::endl;
        return 0;
    } else {
        std::cerr << "Total failures: " << runner.failures
                  << " out of 4 tests for Pack1ByteReversed." << std::endl;
        return 1;
    }
}