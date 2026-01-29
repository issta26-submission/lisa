// Test suite for the focal method: cmsINLINE cmsBool is_surrogate(cmsUInt32Number uc)
// This unit test is designed to compile in a C++11 environment without GoogleTest.
// It uses a small, non-terminating assertion framework to maximize coverage.
// The test assumes the focal function is exposed with C linkage:
//
//     extern "C" cmsBool is_surrogate(cmsUInt32Number uc);
//
// where cmsBool is an unsigned/bool-like type and cmsUInt32Number is a 32-bit unsigned integer.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Step 1 (Test Understanding): The core logic tested here is:
//     is_surrogate(uc) -> returns true if uc is in the UTF-16 surrogate range [0xD800, 0xDFFF]
// The condition is effectively: (uc - 0xD800) < 2048 when uc is treated as an unsigned value.
//
// Delegate the actual function to the library (extern "C" to ensure proper linkage).

extern "C" {
    // Define compatible type aliases mirroring the library's typedefs.
    // These should align with the library's actual definitions (cmsBool and cmsUInt32Number).
    typedef unsigned int cmsBool;
    typedef uint32_t     cmsUInt32Number;

    // Focal method under test (extern to link with the real implementation in cmstypes.c)
    cmsBool is_surrogate(cmsUInt32Number uc);
}

// Lightweight non-terminating test harness
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        ++tests_run; \
        if(!(cond)) { \
            ++tests_failed; \
            std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        } \
    } while(0)

#define EXPECT_FALSE(cond) \
    do { \
        ++tests_run; \
        if((cond)) { \
            ++tests_failed; \
            std::cerr << "EXPECT_FALSE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        } \
    } while(0)

// Candidate Keywords (Step 1): core dependent components
// - uc: input code unit to evaluate surrogate status
// - 0xD800: start of surrogate range
// - 0xDFFF: end of surrogate range
// - 2048: width of surrogate window (as used in uc - 0xD800 < 2048)
// - true/false branches of the condition
// - unsigned arithmetic semantics (no sign extension)

// Test cases to cover both branches and boundary conditions
void test_is_surrogate_false_below_lower_bound() {
    // uc below the surrogate range should return false
    // Example: 0x0000
    EXPECT_FALSE(is_surrogate(0x0000u));

    // A value just below the surrogate start
    // Example: 0xD7FF
    EXPECT_FALSE(is_surrogate(0xD7FFu));
}

void test_is_surrogate_true_at_lower_bound() {
    // uc exactly at the start of the surrogate range should return true
    // Example: 0xD800
    EXPECT_TRUE(is_surrogate(0xD800u));
}

void test_is_surrogate_true_within_range_boundaries() {
    // Values clearly within the surrogate range should return true
    // Example: 0xD900
    EXPECT_TRUE(is_surrogate(0xD900u));

    // Example: 0xDFFF (end of surrogate range)
    EXPECT_TRUE(is_surrogate(0xDFFFu));
}

void test_is_surrogate_false_at_upper_bound() {
    // uc just above the surrogate range should return false
    // Example: 0xE000
    EXPECT_FALSE(is_surrogate(0xE000u));
}

void test_is_surrogate_true_for_mid_range() {
    // Additional mid-range checks to exercise the path
    // Example: 0xDC00 (within surrogate range)
    EXPECT_TRUE(is_surrogate(0xDC00u));
    // Example: 0xDBFF (upper surrogate boundary)
    EXPECT_TRUE(is_surrogate(0xDBFFu));
}

void test_is_surrogate_extreme_values() {
    // Extreme values to ensure robustness with unsigned arithmetic
    // Example: max 32-bit value should yield false (not in surrogate range)
    EXPECT_FALSE(is_surrogate(0xFFFFFFFFu));

    // Example: 0xFFFFFFFF - 0xD800 wrap behavior check
    EXPECT_FALSE(is_surrogate(0xFFFFFEFFu));
}

// Main function to execute tests (Step 3: Test Case Refinement)
int main() {
    // Run individual test cases
    test_is_surrogate_false_below_lower_bound();
    test_is_surrogate_true_at_lower_bound();
    test_is_surrogate_true_within_range_boundaries();
    test_is_surrogate_false_at_upper_bound();
    test_is_surrogate_true_for_mid_range();
    test_is_surrogate_extreme_values();

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;

    // Return non-zero if any test failed
    return tests_failed;
}