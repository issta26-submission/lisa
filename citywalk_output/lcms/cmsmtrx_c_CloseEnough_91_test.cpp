// Minimal C++11 test harness for the C function CloseEnough defined in cmsmtrx.c
// This test does not rely on any external testing framework (GTest, etc.)

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>


// Provide C linkage for the target function and dependent types.
// We reuse common type aliases that are typically defined in the library header.
// If the real project uses different typedefs, adjust accordingly, but keep linkage C.
extern "C" {
    typedef unsigned int cmsBool;        // Common alias in CMS/LCMS libraries
    typedef double cmsFloat64Number;     // 64-bit floating point
    cmsBool CloseEnough(cmsFloat64Number a, cmsFloat64Number b);
}

// Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;

// Expectation macros that do not terminate the test on failure.
// They simply record results and print diagnostic messages to aid debugging.
#define EXPECT_TRUE(cond) \
    do { \
        ++tests_run; \
        if (cond) { ++tests_passed; } \
        else { \
            std::cerr << "Test failed: " #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while(0)

#define EXPECT_FALSE(cond) \
    do { \
        ++tests_run; \
        if (!(cond)) { ++tests_passed; } \
        else { \
            std::cerr << "Test failed: " "!(" #cond ")" << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } \
    } while(0)

// Test 1: True result when both arguments are identical
void test_CloseEnough_same_values() {
    // True: diff is 0
    EXPECT_TRUE(CloseEnough(1.0, 1.0));
    // Symmetric case
    EXPECT_TRUE(CloseEnough(-5.0, -5.0));
}

// Test 2: True result for differences strictly within the threshold (1/65535)
void test_CloseEnough_within_threshold() {
    const cmsFloat64Number TH = 1.0 / 65535.0;
    // Difference smaller than threshold
    EXPECT_TRUE(CloseEnough(0.0, TH / 2.0));

    // Another within-threshold example with non-zero values
    EXPECT_TRUE(CloseEnough(2.5, 2.5 + TH / 3.0));
}

// Test 3: False result when the difference is exactly the threshold or greater
void test_CloseEnough_at_threshold() {
    const cmsFloat64Number TH = 1.0 / 65535.0;
    // Exactly at threshold: should be false due to strict inequality
    EXPECT_FALSE(CloseEnough(0.0, TH));

    // Slightly above the threshold
    EXPECT_FALSE(CloseEnough(-1.0, -1.0 + TH));
}

// Test 4: False results for values clearly above threshold across larger magnitudes
void test_CloseEnough_above_threshold() {
    const cmsFloat64Number TH = 1.0 / 65535.0;
    // Above threshold by a measurable amount
    EXPECT_FALSE(CloseEnough(0.0, TH + 1e-6));

    // Another large-magnitude example
    EXPECT_FALSE(CloseEnough(100.0, 100.0 + TH + 1e-6));
}

int main() {
    // Run all tests
    test_CloseEnough_same_values();
    test_CloseEnough_within_threshold();
    test_CloseEnough_at_threshold();
    test_CloseEnough_above_threshold();

    // Summary
    std::cout << "CloseEnough tests: " << tests_passed << " / " << tests_run << " passed." << std::endl;
    return (tests_run == tests_passed) ? 0 : 1;
}