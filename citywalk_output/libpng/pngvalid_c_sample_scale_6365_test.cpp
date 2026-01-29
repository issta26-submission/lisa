// Test suite for the focal method: sample_scale
// This C++11 test harness calls the C function sample_scale (defined in pngvalid.c)
// and verifies its behavior across a range of inputs.
// No GTest is used; a lightweight, non-terminating test harness is implemented.

#include <vector>
#include <signal.h>
#include <string.h>
#include <string>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


// Forward declaration of the C function under test.
// Ensure C linkage to avoid name mangling when linking with the C source.
extern "C" unsigned int sample_scale(double sample_value, unsigned int scale);

// Simple non-terminating test assertion macro.
// It prints detailed information on failure but does not exit the program.
// Returns true if the assertion passes.
#define EXPECT_EQ(actual, expected, description)                         \
    do {                                                               \
        if ((actual) != (expected)) {                                \
            std::cerr << "[FAIL] " << (description) << "\n"         \
                      << "  Expected: " << (expected) << "\n"        \
                      << "  Actual  : " << (actual) << "\n";       \
            ++g_fail_count;                                          \
        } else {                                                       \
            ++g_pass_count;                                          \
        }                                                              \
    } while (0)

// Global counters to track test results (non-terminating).
static int g_pass_count = 0;
static int g_fail_count = 0;

// Candidate Keywords (Step 1): core components of the focal method.
// - floor operation on the scaled value
// - rounding via +0.5 before flooring
// - NaN/negative handling: any non-positive intermediate result becomes 0
// - clamping to the maximum value 'scale'
// - final cast to unsigned int
//
// These keywords guided the test cases to cover true/false branches:
// floor/rounding decisions, NaN/非-positive handling, upper bound clamping.

struct TestCase {
    double sample_value;
    unsigned int scale;
    unsigned int expected;
    std::string description;
};

int main() {
    // Step 2 & 3: Generate and run a comprehensive test suite for sample_scale.
    // The tests target:
    // - Zero and negative inputs
    // - Small positives producing values below 'scale'
    // - Values that round to exactly 'scale'
    // - Values that exceed 'scale' and trigger clamping to 'scale'
    // - Edge case with scale == 0
    // - Boundary rounding near 0 and near the next integer

    std::vector<TestCase> tests = {
        // Basic zero input
        { 0.0, 10, 0, "sample_value = 0.0, scale = 10 -> expect 0" },
        // Small positive values below scale
        { 0.4, 10, 4, "sample_value = 0.4, scale = 10 -> expect 4" },
        { 0.6, 10, 6, "sample_value = 0.6, scale = 10 -> expect 6" },
        // Negative value should yield 0 due to NaN-as-0 handling
        { -1.0, 10, 0, "sample_value = -1.0, scale = 10 -> expect 0" },
        // Rounding up to exceed scale should clamp to scale
        { 1.1, 10, 10, "sample_value = 1.1, scale = 10 -> expect clamp to 10" },
        // Large input should clamp to scale
        { 12.0, 10, 10, "sample_value = 12.0, scale = 10 -> expect clamp to 10" },
        // Scale zero must yield 0 (anything * 0 -> floor(0.5) -> 0)
        { 5.0, 0, 0, "sample_value = 5.0, scale = 0 -> expect 0" },
        // Non-trivial rounding near small scale: ensures non-zero but small result
        { 0.17, 3, 1, "sample_value = 0.17, scale = 3 -> expect 1" },
        // Boundary where value equals exactly scale after rounding
        { 1.0, 10, 10, "sample_value = 1.0, scale = 10 -> expect 10" },
        // Another clamping scenario with approximate threshold
        { 1.05, 10, 10, "sample_value = 1.05, scale = 10 -> expect clamp to 10" },
        // Very small negative near zero should still yield 0
        { -0.01, 3, 0, "sample_value = -0.01, scale = 3 -> expect 0" }
    };

    // Run all test cases
    for (size_t i = 0; i < tests.size(); ++i) {
        const TestCase& tc = tests[i];
        unsigned int result = sample_scale(tc.sample_value, tc.scale);
        // Use description to provide context about the test case.
        EXPECT_EQ(result, tc.expected, tc.description);
    }

    // Summary
    std::cout << "Test Summary: "
              << g_pass_count << " passed, "
              << g_fail_count << " failed.\n";

    // Return non-zero if any test failed to signal issues to automation,
    // but do not terminate tests early.
    return (g_fail_count == 0) ? 0 : 1;
}