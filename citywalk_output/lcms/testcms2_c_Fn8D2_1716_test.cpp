// C++11 test suite for Fn8D2 in testcms2.c
// This harness uses a lightweight, non-terminating assertion style.
// It does not rely on GTest and runs from main() as requested.
// The tests cover various input scenarios to validate the arithmetic in Fn8D2.

#include <iostream>
#include <string>
#include <cstdint>
#include <testcms2.h>


// Bring in the C declarations for the CMS library test components.
// Fn8D2 is defined in testcms2.c with C linkage.
extern "C" {
}

// Prototype to ensure correct linkage in C++ translation unit.
extern "C" cmsUInt16Number Fn8D2(cmsUInt16Number a1, cmsUInt16Number a2, cmsUInt16Number a3, cmsUInt16Number a4,
                               cmsUInt16Number a5, cmsUInt16Number a6, cmsUInt16Number a7, cmsUInt16Number a8,
                               cmsUInt32Number m);

// Simple non-terminating assertion helper for unit tests.
// It prints a message on failure but does not abort test execution.
static int g_totalTests = 0;
static int g_failedTests = 0;

static void EXPECT_EQ(const std::string& testName,
                      cmsUInt16Number actual,
                      cmsUInt16Number expected)
{
    ++g_totalTests;
    if (actual != expected) {
        ++g_failedTests;
        std::cout << "[FAILED] " << testName
                  << " - Expected: " << static_cast<uint32_t>(expected)
                  << ", Actual: " << static_cast<uint32_t>(actual) << std::endl;
    } else {
        std::cout << "[PASSED] " << testName << std::endl;
    }
}

// Test Case 1
// Scenario: Small numbers with zero values for a4..a8 and m=0.
// Expected numerator = 1 + 3*1 + 3*1 + 0 + 0 + 0 + 0 + 0 = 7
// Denominator = m + 4 = 4
// Expected result = 7 / 4 = 1 (integer division)
static void test_case_basic_small_all_zero_rest()
{
    cmsUInt16Number a1 = 1;
    cmsUInt16Number a2 = 1;
    cmsUInt16Number a3 = 1;
    cmsUInt16Number a4 = 0;
    cmsUInt16Number a5 = 0;
    cmsUInt16Number a6 = 0;
    cmsUInt16Number a7 = 0;
    cmsUInt16Number a8 = 0;
    cmsUInt32Number m = 0;

    cmsUInt16Number result = Fn8D2(a1, a2, a3, a4, a5, a6, a7, a8, m);

    // Expected value computed as described above.
    cmsUInt32Number numerator = a1 + 3*a2 + 3*a3 + a4 + a5 + a6 + a7 + a8;
    cmsUInt16Number expected = (cmsUInt16Number)(numerator / (m + 4));

    EXPECT_EQ("Case1_BasicSmall", result, expected);
}

// Test Case 2
// Scenario: All eight inputs are 1, m=2.
// Expected numerator = 12, denominator = 6, result = 2
static void test_case_all_ones_m2()
{
    cmsUInt16Number a1 = 1, a2 = 1, a3 = 1, a4 = 1, a5 = 1, a6 = 1, a7 = 1, a8 = 1;
    cmsUInt32Number m = 2;

    cmsUInt16Number result = Fn8D2(a1, a2, a3, a4, a5, a6, a7, a8, m);

    cmsUInt32Number numerator = a1 + 3*a2 + 3*a3 + a4 + a5 + a6 + a7 + a8;
    cmsUInt16Number expected = (cmsUInt16Number)(numerator / (m + 4));

    EXPECT_EQ("Case2_AllOnes_M2", result, expected);
}

// Test Case 3
// Scenario: All inputs are 1, m=3 to test truncation behavior.
// Numerator = 12, Denominator = 7, Result = 1
static void test_case_all_ones_m3()
{
    cmsUInt16Number a1 = 1, a2 = 1, a3 = 1, a4 = 1, a5 = 1, a6 = 1, a7 = 1, a8 = 1;
    cmsUInt32Number m = 3;

    cmsUInt16Number result = Fn8D2(a1, a2, a3, a4, a5, a6, a7, a8, m);

    cmsUInt32Number numerator = a1 + 3*a2 + 3*a3 + a4 + a5 + a6 + a7 + a8;
    cmsUInt16Number expected = (cmsUInt16Number)(numerator / (m + 4));

    EXPECT_EQ("Case3_AllOnes_M3", result, expected);
}

// Test Case 4
// Scenario: All inputs are maximum 65535 to test 16-bit cast behavior.
// Denominator = 4, numerator = 12 * 65535, quotient = 196605, cast to 16-bit => 196605 mod 65536 = 65533
static void test_case_all_max_values_wraparound()
{
    cmsUInt16Number a1 = 65535, a2 = 65535, a3 = 65535, a4 = 65535;
    cmsUInt16Number a5 = 65535, a6 = 65535, a7 = 65535, a8 = 65535;
    cmsUInt32Number m = 0;

    cmsUInt16Number result = Fn8D2(a1, a2, a3, a4, a5, a6, a7, a8, m);

    // Compute expected value with the same arithmetic and cast as in Fn8D2
    uint32_t numerator = a1 + 3u*a2 + 3u*a3 + a4 + a5 + a6 + a7 + a8;
    cmsUInt16Number expected = (cmsUInt16Number)(numerator / (m + 4));

    EXPECT_EQ("Case4_MaxInputs_Wraparound", result, expected);
}

// Test Case 5
// Scenario: All zeros, m=0.
// Expected = 0
static void test_case_all_zeros()
{
    cmsUInt16Number a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0, a8 = 0;
    cmsUInt32Number m = 0;

    cmsUInt16Number result = Fn8D2(a1, a2, a3, a4, a5, a6, a7, a8, m);
    cmsUInt16Number expected = 0;

    EXPECT_EQ("Case5_AllZeros", result, expected);
}

// Test Case 6
// Scenario: Mixed inputs with moderate m to verify standard division behavior.
// a1=5, a2=2, a3=3, a4=4, a5=6, a6=7, a7=8, a8=9, m=10
// Numerator = 5 + 3*2 + 3*3 + 4 + 6 + 7 + 8 + 9 = 54
// Denominator = 14, Result = 54 / 14 = 3
static void test_case_mixed_values_m10()
{
    cmsUInt16Number a1 = 5, a2 = 2, a3 = 3, a4 = 4, a5 = 6, a6 = 7, a7 = 8, a8 = 9;
    cmsUInt32Number m = 10;

    cmsUInt16Number result = Fn8D2(a1, a2, a3, a4, a5, a6, a7, a8, m);
    cmsUInt32Number numerator = a1 + 3*a2 + 3*a3 + a4 + a5 + a6 + a7 + a8;
    cmsUInt16Number expected = (cmsUInt16Number)(numerator / (m + 4));

    EXPECT_EQ("Case6_MixedValues_M10", result, expected);
}

// Helper to run all tests
static void run_all_tests()
{
    test_case_basic_small_all_zero_rest();
    test_case_all_ones_m2();
    test_case_all_ones_m3();
    test_case_all_max_values_wraparound();
    test_case_all_zeros();
    test_case_mixed_values_m10();

    std::cout << "Total tests run: " << g_totalTests
              << ", Passed: " << (g_totalTests - g_failedTests)
              << ", Failed: " << g_failedTests << std::endl;
}

int main()
{
    // Run the curated test suite for Fn8D2
    // All tests are designed to be self-contained and use only the standard library for reporting.
    run_all_tests();

    // Return non-zero if any test failed to signal a problem.
    return (g_failedTests != 0) ? 1 : 0;
}