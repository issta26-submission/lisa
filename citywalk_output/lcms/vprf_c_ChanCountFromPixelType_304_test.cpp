// Unit tests for ChanCountFromPixelType in vprf.c
// This test suite is designed for C++11, without GoogleTest.
// It relies on the project's existing Pixel Type macros (PT_*) and the
// function prototype for ChanCountFromPixelType declared in vprf.h (or an equivalent header).

#include <iostream>
#include <cstdint>
#include <utils.h>
#include <vprf.h>


// Include the project's definitions for pixel types and the function to test.
// It is expected that vprf.h declares:
 //   extern "C" int ChanCountFromPixelType(int ColorChannels);
 //   and defines the PT_* constants used below.

#ifdef __cplusplus
extern "C" {
#endif
// If vprf.h doesn't declare the function with C linkage, provide a minimal forward
// declaration to ensure linking with the C implementation in vprf.c.
int ChanCountFromPixelType(int ColorChannels);
#ifdef __cplusplus
}
#endif

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Non-terminating assertion macro: reports failure but continues execution.
#define EXPECT_EQ(expected, actual) do {                                  \
    ++g_tests_run;                                                           \
    if ((expected) == (actual)) {                                           \
        ++g_tests_passed;                                                   \
    } else {                                                                \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__              \
                  << " - expected: " << (expected)                          \
                  << ", actual: " << (actual) << std::endl;               \
    }                                                                       \
} while (0)

// Test 1: Verify ChanCountFromPixelType returns 1 for PT_GRAY
// Rationale: Gray images should have a single color channel.
static void test_PT_GRAY_returns_1() {
    int result = ChanCountFromPixelType(PT_GRAY);
    EXPECT_EQ(1, result);
}

// Test 2-17: Comprehensive coverage of all supported pixel types
// Those tests ensure each Pixel Type constant maps to the expected channel count.
// The exact PT_* macros are defined in the project's headers and must match the
// switch-case labels inside ChanCountFromPixelType.
static void test_all_supported_pixel_types() {
    struct Case {
        int input;
        int expected;
        const char* label;
    } cases[] = {
        { PT_GRAY, 1,   "PT_GRAY" },
        { PT_RGB,  3,   "PT_RGB" },
        { PT_CMY,  3,   "PT_CMY" },
        { PT_Lab,  3,   "PT_Lab" },
        { PT_YUV,  3,   "PT_YUV" },
        { PT_YCbCr,3,   "PT_YCbCr" },
        { PT_CMYK, 4,   "PT_CMYK" },
        { PT_MCH2, 2,   "PT_MCH2" },
        { PT_MCH3, 3,   "PT_MCH3" },
        { PT_MCH4, 4,   "PT_MCH4" },
        { PT_MCH5, 5,   "PT_MCH5" },
        { PT_MCH6, 6,   "PT_MCH6" },
        { PT_MCH7, 7,   "PT_MCH7" },
        { PT_MCH8, 8,   "PT_MCH8" },
        { PT_MCH9, 9,   "PT_MCH9" },
        { PT_MCH10,10,  "PT_MCH10" },
        { PT_MCH11,11,  "PT_MCH11" },
        { PT_MCH12,12,  "PT_MCH12" },
        { PT_MCH13,12,  "PT_MCH13" },
        { PT_MCH14,14,  "PT_MCH14" },
        { PT_MCH15,15,  "PT_MCH15" }
    };

    const int n = sizeof(cases) / sizeof(cases[0]);
    for (int i = 0; i < n; ++i) {
        const Case &c = cases[i];
        int result = ChanCountFromPixelType(c.input);
        // Each entry is expected to match the mapping in the switch-case.
        // We reuse the same EXPECT_EQ macro to accumulate results.
        EXPECT_EQ(c.expected, result);
    }

    // Test 18: Default/unsupported color space should trigger FatalError and return -1.
    // The FatalError function in the test's class dependencies is a no-op,
    // allowing the test to continue safely.
    int invalidInput = -9999; // outside defined PT_* range
    int defaultResult = ChanCountFromPixelType(invalidInput);
    EXPECT_EQ(-1, defaultResult);
}

// Runner
int main() {
    // Run tests with explanatory comments to aid maintainability.
    // 1) Basic correctness for PT_GRAY
    test_PT_GRAY_returns_1();

    // 2) Exhaustive coverage across all supported pixel types and an invalid value
    test_all_supported_pixel_types();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed
              << ", Failed: " << (g_tests_run - g_tests_passed) << std::endl;

    // Nonzero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}