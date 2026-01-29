/*
Unit test suite for Pack4WordsSwap in cmspack.c

Step 1: Program Understanding (in-code notes)
- Pack4WordsSwap takes an input array of four 16-bit words (wOut[4])
- It writes them to the output buffer in reverse order as 4 x 16-bit words:
  output[0..1] = wOut[3]
  output[2..3] = wOut[2]
  output[4..5] = wOut[1]
  output[6..7] = wOut[0]
- It returns the output pointer advanced by 8 bytes (output + 8)
- The function uses a CMSTRANSFORM-like pointer (info) and a Stride parameter that are unused
- This test uses a small, self-contained C++11 test harness (no Google Test), focusing on true/false branches and coverage of data flow

Step 2: Unit Test Generation
- Core Candidate Keywords: Pack4WordsSwap, wOut (input), output (buffer), Stride (unused), _cmsTRANSFORM (type), CMSREGISTER (macro)
- Tests cover: basic behavior, large values, mixed values, Stride ignored, and multiple invocations
- Dependencies (mocked as forward declarations in test): _cmsTRANSFORM, CMSREGISTER, cmsUInt8Number, cmsUInt16Number, cmsUInt32Number

Step 3: Test Case Refinement
- Use host-endianness-agnostic checks by comparing 16-bit words after the call
- Ensure output buffer alignment via alignas to safely read 16-bit chunks
- Validate both pointer return value and content correctness
- Include explanatory comments for each test case
*/

// Candidate keywords and understanding reflected in tests below

#include <vector>
#include <cstddef>
#include <iostream>
#include <string>
#include <iomanip>
#include <type_traits>
#include <algorithm>
#include <lcms2_internal.h>
#include <cstdint>


// Minimal compatibility layer to declare the target function for linking.
// We declare the function with C linkage and use forward declarations for the transform type.
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Forward declare the transform structure as the real type is opaque to the test harness.
struct _cmsTRANSFORM;

// Typedefs matching the CMS types (commonly 8/16/32-bit)
typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// Forward declare the function from cmspack.c using C linkage
extern "C" cmsUInt8Number* Pack4WordsSwap(CMSREGISTER struct _cmsTRANSFORM* info,
                               CMSREGISTER cmsUInt16Number wOut[],
                               CMSREGISTER cmsUInt8Number* output,
                               CMSREGISTER cmsUInt32Number Stride);

// Global test failure collector
static std::vector<std::string> g_failures;

// Simple failure logger
static void logFailure(const std::string& msg) {
    g_failures.push_back(msg);
}

// Helper: compare two arrays of cmsUInt16Number
static bool equal16(const cmsUInt16Number* a, const cmsUInt16Number* b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Test 1: Basic order reversal with small, distinct values
// - Verifies the returned pointer is offset by 8 bytes
// - Verifies that the 16-bit words are written in reverse order: [wOut3, wOut2, wOut1, wOut0]
static void Test_Pack4WordsSwap_BasicOrder() {
    alignas(2) cmsUInt8Number output[16];
    cmsUInt16Number wOut[4] = {0, 1, 2, 3}; // wOut[0]=0, wOut[1]=1, wOut[2]=2, wOut[3]=3
    struct _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;
    cmsUInt8Number* ret = Pack4WordsSwap(info, wOut, output, Stride);

    // Expected: first 4 bytes contain 3,0 (wOut[3]), then 2,0 (wOut[2]), then 1,0 (wOut[1]), then 0,0 (wOut[0])
    // We read back as 16-bit words to avoid endianness assumptions
    cmsUInt16Number* out16 = reinterpret_cast<cmsUInt16Number*>(output);
    cmsUInt16Number expected[4] = { wOut[3], wOut[2], wOut[1], wOut[0] };

    bool pass = (ret == output + 8) && equal16(out16, expected, 4);
    if (!pass) {
        logFailure("Test_Pack4WordsSwap_BasicOrder: content or return pointer mismatch");
        logFailure("  Expected 16-bit words: [" +
                   std::to_string(expected[0]) + ", " +
                   std::to_string(expected[1]) + ", " +
                   std::to_string(expected[2]) + ", " +
                   std::to_string(expected[3]) + "]");
        logFailure("  Got 16-bit words: [" +
                   std::to_string(out16[0]) + ", " +
                   std::to_string(out16[1]) + ", " +
                   std::to_string(out16[2]) + ", " +
                   std::to_string(out16[3]) + "]");
        logFailure("  Returned pointer: " + std::to_string(reinterpret_cast<uintptr_t>(ret)) +
                   " vs expected " + std::to_string(reinterpret_cast<uintptr_t>(output) + 8));
    }
}

// Test 2: Basic order reversal with larger 16-bit values
// - Ensures function handles non-zero 16-bit values correctly
static void Test_Pack4WordsSwap_LargeValues() {
    alignas(2) cmsUInt8Number output[16];
    cmsUInt16Number wOut[4] = {0x1234, 0x5678, 0x9ABC, 0xDEF0};
    struct _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;
    cmsUInt8Number* ret = Pack4WordsSwap(info, wOut, output, Stride);

    cmsUInt16Number* out16 = reinterpret_cast<cmsUInt16Number*>(output);
    cmsUInt16Number expected[4] = { wOut[3], wOut[2], wOut[1], wOut[0] };

    bool pass = (ret == output + 8) && equal16(out16, expected, 4);
    if (!pass) {
        logFailure("Test_Pack4WordsSwap_LargeValues: content or return pointer mismatch");
        logFailure("  Expected 16-bit words: [" +
                   std::to_string(expected[0]) + ", " +
                   std::to_string(expected[1]) + ", " +
                   std::to_string(expected[2]) + ", " +
                   std::to_string(expected[3]) + "]");
        logFailure("  Got 16-bit words: [" +
                   std::to_string(out16[0]) + ", " +
                   std::to_string(out16[1]) + ", " +
                   std::to_string(out16[2]) + ", " +
                   std::to_string(out16[3]) + "]");
        logFailure("  Returned pointer: " + std::to_string(reinterpret_cast<uintptr_t>(ret)) +
                   " vs expected " + std::to_string(reinterpret_cast<uintptr_t>(output) + 8));
    }
}

// Test 3: Stride parameter is unused by the implementation
// - Change Stride and verify behavior remains correct
static void Test_Pack4WordsSwap_StrideIgnored() {
    alignas(2) cmsUInt8Number output[16];
    cmsUInt16Number wOut[4] = {0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD};
    struct _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 12345; // non-zero, but should be ignored
    cmsUInt8Number* ret = Pack4WordsSwap(info, wOut, output, Stride);

    cmsUInt16Number* out16 = reinterpret_cast<cmsUInt16Number*>(output);
    cmsUInt16Number expected[4] = { wOut[3], wOut[2], wOut[1], wOut[0] };

    bool pass = (ret == output + 8) && equal16(out16, expected, 4);
    if (!pass) {
        logFailure("Test_Pack4WordsSwap_StrideIgnored: content or return pointer mismatch");
        logFailure("  Stride value did not affect operation (as expected)");
    }
}

// Test 4: Multiple invocations on same buffer with varying data
static void Test_Pack4WordsSwap_MultipleCalls() {
    alignas(2) cmsUInt8Number output[16];
    cmsUInt16Number wOut1[4] = {1, 2, 3, 4};
    cmsUInt16Number wOut2[4] = {10, 20, 30, 40};
    struct _cmsTRANSFORM* info = nullptr;

    // First call
    Pack4WordsSwap(info, wOut1, output, 0);
    cmsUInt16Number* out16 = reinterpret_cast<cmsUInt16Number*>(output);
    cmsUInt16Number expected1[4] = { wOut1[3], wOut1[2], wOut1[1], wOut1[0] };
    bool pass1 = equal16(out16, expected1, 4);

    // Second call on same buffer with different data
    cmsUInt8Number* ret2 = Pack4WordsSwap(info, wOut2, output, 0);
    cmsUInt16Number* out16b = reinterpret_cast<cmsUInt16Number*>(output);
    cmsUInt16Number expected2[4] = { wOut2[3], wOut2[2], wOut2[1], wOut2[0] };
    bool pass2 = (ret2 == output + 8) && equal16(out16b, expected2, 4);

    if (!pass1) {
        logFailure("Test_Pack4WordsSwap_MultipleCalls: first call content mismatch");
    }
    if (!pass2) {
        logFailure("Test_Pack4WordsSwap_MultipleCalls: second call content mismatch");
    }
}

// Runner
static void RunAllTests() {
    Test_Pack4WordsSwap_BasicOrder();
    Test_Pack4WordsSwap_LargeValues();
    Test_Pack4WordsSwap_StrideIgnored();
    Test_Pack4WordsSwap_MultipleCalls();
}

// Entry point
int main() {
    RunAllTests();

    if (g_failures.empty()) {
        std::cout << "All Pack4WordsSwap tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << g_failures.size() << " Pack4WordsSwap test(s) FAILED:" << std::endl;
        for (const auto& f : g_failures) {
            std::cout << "- " << f << std::endl;
        }
        return static_cast<int>(g_failures.size());
    }
}