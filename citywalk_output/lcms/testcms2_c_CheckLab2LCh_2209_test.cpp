/*
Unit Test Suite for CheckLab2LCh (from testcms2.c)

Overview:
- Target: cmsInt32Number CheckLab2LCh(void)
- Approach: Provide a lightweight C++11 test harness (no GTest) that calls the C function
  from testcms2.c. Use non-terminating assertions that log failures but continue executing
  to maximize coverage of the code path within the focal function.
- Key dependencies exercised by the test:
  - L, a, b (Lab components)
  - LCh conversion (cmsLab2LCh, cmsLCh2Lab) and DeltaE (cmsDeltaE)
  - Round-trip behaviour through Lab <-> LCh conversions
- Notes:
  - We only declare the function with a compatible C signature to avoid needing the
    full littleCMS headers in the test code.
  - This test focuses on runtime behavior of the focal function and basic return value
    properties (non-zero indicates "true" in the original API convention).
  - True/false branch coverage within CheckLab2LCh is inherently linked to the internal
    dist > Max branch and the final Max < 1e-12 check; the test validates that the function
    returns a boolean-like result and does not crash.

Usage:
- Build with C file testcms2.c and link against the produced C++ test binary.
- Ensure the platform uses a 32-bit int for cmsInt32Number (typical in such libraries).

Note: If the environment provides exact littleCMS headers, replacing the forward declaration
with the proper include would be possible. Here we minimize dependencies to guarantee
compilation in isolation.

*/

#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Minimal forward declaration for the focal C function from testcms2.c.
// We mirror the return type as cmsInt32Number using a portable alias.
typedef int cmsInt32Number;
extern "C" cmsInt32Number CheckLab2LCh(void);

// Lightweight test framework (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
    g_total++; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "ASSERTION FAILED [" << __FILE__ << ":" << __LINE__ << "]" \
                  << " - expected true: (" #cond ")" << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    g_total++; \
    if((a) != (b)) { \
        ++g_failed; \
        std::cerr << "ASSERTION FAILED [" << __FILE__ << ":" << __LINE__ << "]" \
                  << " - expected " << (a) << " == " << (b) << std::endl; \
    } \
} while(0)

// Test 1: Validate that CheckLab2LCh returns a non-zero value (interpreted as true by the API convention).
// This also exercises the most common path where at least one deltaE is larger than the initial Max=0.
void test_CheckLab2LCh_ReturnsNonZero() {
    // Explanation:
    // The function iterates over Lab values, converts to LCh and back, computes DeltaE,
    // tracks the maximum delta, and returns true if Max < 1e-12.
    // In a typical color space implementation, some delta will exceed 0, so we expect a non-zero
    // boolean result in practice. This test asserts the function does not return 0 (false),
    // indicating successful computation and no crash.
    cmsInt32Number res = CheckLab2LCh();

    // We interpret non-zero as "true" for this API shape.
    EXPECT_TRUE(res != 0);
}

// Test 2: Validate the return value is a plausible boolean (0 or 1).
// Rationale: The function returns a boolean-like cmsInt32Number. This test ensures we
// do not receive an invalid value (e.g., negative or a value outside {0,1}).
void test_CheckLab2LCh_BoolRange() {
    cmsInt32Number res = CheckLab2LCh();
    // Acceptable values are 0 or 1. If a different integer value is returned, mark as failure.
    EXPECT_TRUE((res == 0) || (res == 1));
    // Additional check to ensure consistent type expectations
    EXPECT_EQ(res == 0, 0); // Just a placeholder to exercise the integer path
}

// Test 3: Sanity test to ensure repeated invocations do not crash and return a valid boolean.
// This helps confirm stability across multiple calls in a single test run.
void test_CheckLab2LCh_RepeatedCalls() {
    for (int i = 0; i < 3; ++i) {
        cmsInt32Number res = CheckLab2LCh();
        EXPECT_TRUE((res == 0) || (res == 1));
    }
}

int main() {
    // Run tests
    test_CheckLab2LCh_ReturnsNonZero();
    test_CheckLab2LCh_BoolRange();
    test_CheckLab2LCh_RepeatedCalls();

    // Summary
    std::cout << "Test Summary: total=" << g_total << ", failures=" << g_failed << std::endl;
    // Return non-zero if any test failed
    return g_failed;
}