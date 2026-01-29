// C++11 test suite for Fn8D1 from testcms2.c
// This test harness avoids GoogleTest and uses a small, non-terminating assertion framework.
// It imports the C declarations from testcms2.h and calls the Fn8D1 function directly.
//
// Domain notes:
// - Fn8D1 computes: return ((a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8) / m) cast to cmsUInt16Number.
// - All inputs are unsigned 16-bit values (a1..a8) and an unsigned 32-bit divisor m.
// - No internal branching in Fn8D1, so tests target correct arithmetic and cast behavior (including overflow wrap).
//
// Test execution approach:
// - Define several explicit test cases with fixed inputs and known expected outputs.
// - Use a tiny test harness that logs PASS/FAIL but continues execution (non-terminating assertions).
// - Report a final pass/fail summary and return non-zero if any test failed.

#include <iostream>
#include <string>
#include <cstdint>
#include <testcms2.h>


#ifdef __cplusplus
extern "C" {
#endif
// Ensure C linkage for the focal function and types when included from C++
#ifdef __cplusplus
}
#endif

// Lightweight test harness state
static int g_passed = 0;
static int g_failed = 0;

// Non-terminating assertion helper
static void assert_eq(cmsUInt16Number got, cmsUInt16Number expected, const char* testName) {
    if (got == expected) {
        std::cout << "[PASS] " << testName << "\n";
        ++g_passed;
    } else {
        // Display numeric values as unsigned for clarity
        std::cout << "[FAIL] " << testName
                  << " - expected: " << static_cast<unsigned>(expected)
                  << ", got: " << static_cast<unsigned>(got) << "\n";
        ++g_failed;
    }
}

// Test 1: All inputs are 1, m = 1
// Rationale: Simple, exact average with no wrapping.
static void test_Fn8D1_allOnes_m1() {
    // Candidate Keywords: a1..a8 = 1, m = 1
    cmsUInt16Number a1 = 1, a2 = 1, a3 = 1, a4 = 1;
    cmsUInt16Number a5 = 1, a6 = 1, a7 = 1, a8 = 1;
    cmsUInt32Number m  = 1;

    cmsUInt16Number res = Fn8D1(a1, a2, a3, a4, a5, a6, a7, a8, m);
    // Expected: (1+1+1+1+1+1+1+1)/1 = 8
    const cmsUInt16Number expected = 8;

    assert_eq(res, expected, "Fn8D1_allOnes_m1");
}

// Test 2: All inputs are max (65535), m = 1
// Rationale: Overflow wrap occurs when casting 524280 to 16-bit.
static void test_Fn8D1_allMax_m1() {
    // Candidate Keywords: a1..a8 = 65535, m = 1
    cmsUInt16Number a = 65535;
    cmsUInt16Number a1 = a, a2 = a, a3 = a, a4 = a;
    cmsUInt16Number a5 = a, a6 = a, a7 = a, a8 = a;
    cmsUInt32Number m  = 1;

    cmsUInt16Number res = Fn8D1(a1, a2, a3, a4, a5, a6, a7, a8, m);
    // Expected: (8 * 65535) / 1 -> 524280 -> cast to 16-bit -> 524280 mod 65536 = 65528
    const cmsUInt16Number expected = 65528;

    assert_eq(res, expected, "Fn8D1_allMax_m1");
}

// Test 3: Mixed small values with m such that result is a small integer
// Rationale: Normal non-wrapping result (no overflow) and simple division.
static void test_Fn8D1_mixed_m9() {
    // Candidate Keywords: a = {1,2,3,4,5,6,7,8}, m = 9
    cmsUInt16Number a1 = 1, a2 = 2, a3 = 3, a4 = 4;
    cmsUInt16Number a5 = 5, a6 = 6, a7 = 7, a8 = 8;
    cmsUInt32Number m  = 9;

    cmsUInt16Number res = Fn8D1(a1, a2, a3, a4, a5, a6, a7, a8, m);
    // Expected: (1+2+3+4+5+6+7+8) = 36; 36/9 = 4
    const cmsUInt16Number expected = 4;

    assert_eq(res, expected, "Fn8D1_mixed_m9");
}

// Test 4: Large single non-zero value with half the maximum, to trigger wrap when needed
// Rationale: sum = 0 + 0 + 0 + 0 + 0 + 0 + 0 + 65535; m = 2; 65535/2 = 32767
static void test_Fn8D1_singleLarge_m2() {
    // Candidate Keywords: a8 = 65535, others 0; m = 2
    cmsUInt16Number a1 = 0, a2 = 0, a3 = 0, a4 = 0;
    cmsUInt16Number a5 = 0, a6 = 0, a7 = 0, a8 = 65535;
    cmsUInt32Number m  = 2;

    cmsUInt16Number res = Fn8D1(a1, a2, a3, a4, a5, a6, a7, a8, m);
    // Expected: (0+...+65535) / 2 = 32767 (since 65535/2 = 32767)
    const cmsUInt16Number expected = 32767;

    assert_eq(res, expected, "Fn8D1_singleLarge_m2");
}

// Test 5: All zeros with a large m
// Rationale: Sum is zero, result should be zero regardless of m (as long as m != 0)
static void test_Fn8D1_allZeros_largeM() {
    // Candidate Keywords: all a's = 0, m = 1000
    cmsUInt16Number a1 = 0, a2 = 0, a3 = 0, a4 = 0;
    cmsUInt16Number a5 = 0, a6 = 0, a7 = 0, a8 = 0;
    cmsUInt32Number m  = 1000;

    cmsUInt16Number res = Fn8D1(a1, a2, a3, a4, a5, a6, a7, a8, m);
    const cmsUInt16Number expected = 0;

    assert_eq(res, expected, "Fn8D1_allZeros_largeM");
}

// Runner to execute all tests and print a summary
static void run_all_tests() {
    std::cout << "Running Fn8D1 unit tests (C++ harness, no GTest):\n";
    test_Fn8D1_allOnes_m1();
    test_Fn8D1_allMax_m1();
    test_Fn8D1_mixed_m9();
    test_Fn8D1_singleLarge_m2();
    test_Fn8D1_allZeros_largeM();

    std::cout << "\nTest Summary: " << g_passed << " passed, " << g_failed << " failed.\n";
}

// Entry point
int main() {
    run_all_tests();
    // Return non-zero if any test failed to aid integration with build systems
    return (g_failed == 0) ? 0 : 1;
}