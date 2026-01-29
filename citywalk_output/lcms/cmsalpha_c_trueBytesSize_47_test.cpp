// Unit Tests for cmsUInt32Number trueBytesSize(cmsUInt32Number Format)
// This test suite targets the focal function trueBytesSize and exercises
// both branches of the internal conditional: when T_BYTES(Format) != 0 and when it == 0.
// The tests are written in C++11 without Google Test; a minimal in-house test harness is used.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Include the internal CMS headers to gain access to:
 // - The cmsUInt32Number type
 // - The T_BYTES macro
 // - The declaration of trueBytesSize
// Note: The exact include paths may vary depending on the project setup.
// Ensure these headers are available in the build environment.
extern "C" {
}

// The function under test is defined in the FOCAL_CLASS_DEP region as a
// non-static function with C linkage. We declare it here for linking.
extern "C" cmsUInt32Number trueBytesSize(cmsUInt32Number Format);

// Simple in-house test harness (non-terminating assertions) to maximize coverage.
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Non-terminating assertion macro: logs failures but continues execution
#define EXPECT_EQ(actual, expected) do {                                  \
    ++g_total_tests;                                                         \
    if (static_cast<uint64_t>(actual) != static_cast<uint64_t>(expected)) { \
        ++g_failed_tests;                                                    \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__              \
                  << "  Expected: " << static_cast<uint64_t>(expected)       \
                  << ", Actual: " << static_cast<uint64_t>(actual) << "\n";\
    }                                                                        \
} while(0)

// Test 1: Validate that trueBytesSize returns the same value as T_BYTES(Format)
// for a variety of candidate Format values. If T_BYTES(Format) yields 0, the
// function should return sizeof(double) per the documented behavior.
void test_trueBytesSize_matches_T_BYTES_for_various_formats() {
    // Candidate formats (including a mix of arbitrary values and edge-like values)
    // The exact numeric values are intentionally chosen to exercise macro expansion
    // and ensure we don't rely on a single constant.
    const cmsUInt32Number candidates[] = {
        1u, 2u, 3u, 0x12345678u, 0xFFFFFFFFu, 0u
    };
    const size_t count = sizeof(candidates) / sizeof(candidates[0]);

    for (size_t i = 0; i < count; ++i) {
        cmsUInt32Number fmt = candidates[i];
        // Expected relies on the library's T_BYTES macro: if zero, expect sizeof(double);
        cmsUInt32Number expected = T_BYTES(fmt);
        if (expected == 0) {
            expected = sizeof(double);
        }
        cmsUInt32Number actual = trueBytesSize(fmt);
        // The test is non-terminating: it will report all mismatches but continue.
        EXPECT_EQ(actual, expected);
    }
}

// Test 2: Specifically exercise the zero-branch of the trueBytesSize logic if possible.
// The comment in the focal method states that for the "double" format, T_BYTES returns zero.
// We assume a Format value of 0 corresponds to that double-like format.
// If T_BYTES(0) == 0, we expect trueBytesSize(0) == sizeof(double).
void test_trueBytesSize_zero_branch_if_double_format() {
    const cmsUInt32Number fmt = 0u; // Intended as the "double" format sentinel in comments
    cmsUInt32Number expected = T_BYTES(fmt);
    if (expected == 0) expected = sizeof(double);
    cmsUInt32Number actual = trueBytesSize(fmt);
    EXPECT_EQ(actual, expected);
}

// Entry point for the test suite.
// Executes all tests and reports a final summary.
// The program returns non-zero if any test failed to aid integration with CI systems.
int main() {
    test_trueBytesSize_matches_T_BYTES_for_various_formats();
    test_trueBytesSize_zero_branch_if_double_format();

    std::cout << "Test summary: total=" << g_total_tests
              << ", failed=" << g_failed_tests << "\n";

    return (g_failed_tests == 0) ? 0 : 1;
}