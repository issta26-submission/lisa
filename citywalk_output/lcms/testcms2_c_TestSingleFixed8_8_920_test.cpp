/*
  Test Suite for cmsInt32Number TestSingleFixed8_8(cmsFloat64Number d)

  Step 1 - Program Understanding (Cand. Keywords)
  - _cmsDoubleTo8Fixed8, _cms8Fixed8toDouble: fixed-point conversion helpers
  - cmsS15Fixed16Number, cmsUInt16Number: fixed-point and 16-bit types
  - FIXED_PRECISION_8_8: comparison threshold for fixed-point 8.8 precision
  - TestSingleFixed8_8: converts double to 8.8 fixed-point, back to double, checks error
  - Dependencies/Helpers in testcms2.h (stubbed in the focal class dependency list)

  Step 2 - Unit Test Generation
  - Target: cmsInt32Number TestSingleFixed8_8(cmsFloat64Number d)
  - Coverage goals: exercise true and false branches of the comparison
  - Strategy: select values within representable range (expect true) and out of range (expect false)

  Step 3 - Test Case Refinement
  - Use a lightweight C++11 test harness (no GTest)
  - Non-terminating assertions: accumulate failures and report at end
  - Keep tests independent and deterministic
  - Access the C function via extern "C" prototype
*/

#include <cstdint>
#include <iostream>
#include <cmath>
#include <testcms2.h>


// Lightweight interface to the focal function.
// The actual implementation is in the C test file (testcms2.c) or linked library.
// We declare the minimal types and the function prototype for linking.
typedef int cmsInt32Number;
typedef double cmsFloat64Number;

// Ensure C linkage for the focal function
extern "C" cmsInt32Number TestSingleFixed8_8(cmsFloat64Number d);

// Simple non-terminating assertion framework
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " << (msg) \
                      << " | In " << __FILE__ << ":" << __LINE__ \
                      << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { \
        if (!((a) == (b))) { \
            std::cerr << "EXPECT_EQ failed: " << (msg) \
                      << " | Expected: " << (b) << " Got: " << (a) \
                      << " | In " << __FILE__ << ":" << __LINE__ \
                      << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Test cases

// Test that 0.0 is represented exactly in 8.8 fixed-point and round-trips with zero error.
void test_TestSingleFixed8_8_zero() {
    cmsFloat64Number d = 0.0;
    cmsInt32Number res = TestSingleFixed8_8(d);
    EXPECT_TRUE(res == 1, "TestSingleFixed8_8(0.0) should return true (zero error)");
}

// Test that a small positive value is accurately represented within fixed precision.
void test_TestSingleFixed8_8_small_positive() {
    cmsFloat64Number d = 1.0; // integer value should round-trip exactly
    cmsInt32Number res = TestSingleFixed8_8(d);
    EXPECT_TRUE(res == 1, "TestSingleFixed8_8(1.0) should return true (exact round-trip)");
}

// Test that a moderate negative value still fits within 8.8 fixed range and rounds correctly.
void test_TestSingleFixed8_8_negative_moderate() {
    cmsFloat64Number d = -3.5; // negative fractional value
    cmsInt32Number res = TestSingleFixed8_8(d);
    EXPECT_TRUE(res == 1, "TestSingleFixed8_8(-3.5) should return true (within range, small error)");
}

// Test that a large positive value likely outside 8.8 fixed range yields a larger error (false branch).
void test_TestSingleFixed8_8_large_positive_out_of_range() {
    cmsFloat64Number d = 1000.0; // far outside typical 8.8 range
    cmsInt32Number res = TestSingleFixed8_8(d);
    // Expect false since the round-trip cannot represent such large magnitude accurately
    EXPECT_TRUE(res == 0, "TestSingleFixed8_8(1000.0) should return false (out of range)");
}

// Test that a large negative value outside 8.8 fixed range yields a larger error (false branch).
void test_TestSingleFixed8_8_large_negative_out_of_range() {
    cmsFloat64Number d = -1000.0; // far outside typical 8.8 range
    cmsInt32Number res = TestSingleFixed8_8(d);
    // Expect false since the round-trip cannot represent such large magnitude accurately
    EXPECT_TRUE(res == 0, "TestSingleFixed8_8(-1000.0) should return false (out of range)");
}

// Run all tests and report a summary
void run_all_tests() {
    test_TestSingleFixed8_8_zero();
    test_TestSingleFixed8_8_small_positive();
    test_TestSingleFixed8_8_negative_moderate();
    test_TestSingleFixed8_8_large_positive_out_of_range();
    test_TestSingleFixed8_8_large_negative_out_of_range();

    if (g_failures == 0) {
        std::cout << "All TestSingleFixed8_8 test cases PASSED." << std::endl;
    } else {
        std::cout << "TestSingleFixed8_8 test cases FAILED: " << g_failures << " failure(s)." << std::endl;
    }
}

// Main: entry point for the test suite
int main() {
    run_all_tests();
    return g_failures == 0 ? 0 : 1;
}