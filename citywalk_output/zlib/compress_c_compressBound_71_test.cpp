// Standalone C++11 test suite for the focal method: compressBound
// This test suite is designed to be run without Google Test (GTest).
// It uses simple non-terminating assertions and a minimal test harness.

#include <iostream>
#include <zlib.h>
#include <cstdint>


// Step 1 (Contextual): Candidate Keywords extracted from the focal method and dependencies
// - compressBound
// - sourceLen
// - bit shifts: (sourceLen >> 12), (sourceLen >> 14), (sourceLen >> 25)
// - constants: 13
// - zlib types/macros: ZLIB_INTERNAL, ZEXPORT, uLong
// - Related functions in the dependency set: compress2, compress, compressBound
//
// Note: The compressBound function is declared in the zlib API as:
//     uLong ZEXPORT compressBound(uLong sourceLen);
// In this test, we declare the C signature to bind to the compiled object
extern "C" unsigned long compressBound(unsigned long sourceLen);

// Global lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Report test result without terminating the program
static void reportTestResult(const char* testName, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::cout << "PASS: " << testName << "\n";
    } else {
        ++g_failed_tests;
        std::cerr << "FAIL: " << testName << "\n";
    }
}

// Helper to run a single test case: input -> expected, identified by testName
static void test_compressBound_entry(const char* testName, unsigned long input, unsigned long expected) {
    unsigned long actual = compressBound(input);
    bool ok = (actual == expected);
    reportTestResult(testName, ok);
    if (!ok) {
        std::cerr << "  input=" << input
                  << ", expected=" << expected
                  << ", got=" << actual << "\n";
    }
}

// Domain-knowledge-informed test cases
// These cases cover the expected behavior of the bit-shift based formula:
// result = sourceLen + (sourceLen >> 12) + (sourceLen >> 14) + (sourceLen >> 25) + 13

int main() {
    // Documented scenarios (non-exhaustive but targeted for coverage)
    // 1) Zero input: expect 0 + 0 + 0 + 0 + 13 = 13
    test_compressBound_entry("zero_input", 0, 13);

    // 2) Small value where all shifts are 0: expect sourceLen + 13
    //    for 1 => 1 + 0 + 0 + 0 + 13 = 14
    test_compressBound_entry("small_input_one", 1, 14);

    // 3) Edge just below 4096 to verify (>>12) is still 0
    //    4095 + 0 + 0 + 0 + 13 = 4108
    test_compressBound_entry("edge_4095", 4095, 4108);

    // 4) Exactly 4096 to verify (>>12) contributes 1
    //    4096 + 1 + 0 + 0 + 13 = 4110
    test_compressBound_entry("edge_4096", 4096, 4110);

    // 5) Edge around 16383 to verify (>>12) and (>>14) interplay
    //    16383 + 3 + 0 + 0 + 13 = 16399
    test_compressBound_entry("edge_16383", 16383, 16399);

    // 6) Exactly 16384 to verify (>>12) and (>>14) both contribute
    //    16384 + 4 + 1 + 0 + 13 = 16402
    test_compressBound_entry("edge_16384", 16384, 16402);

    // 7) Max testable with 2^25-1 to verify (>>12) and (>>14) non-zero, (>>25) = 0
    //    33554431 + 8191 + 2047 + 0 + 13 = 33564682
    test_compressBound_entry("edge_2pow25_minus1", 33554431, 33564682);

    // 8) Exactly 2^25 to verify (>>25) becomes 1
    //    33554432 + 8192 + 2048 + 1 + 13 = 33564686
    test_compressBound_entry("edge_2pow25", 33554432, 33564686);

    // 9) Mid-range value to validate typical behavior
    //    12345 + (12345>>12) + (12345>>14) + (12345>>25) + 13
    //    = 12345 + 3 + 0 + 0 + 13 = 12361
    test_compressBound_entry("mid_input_12345", 12345, 12361);

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << "\n";

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}