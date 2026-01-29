// C++11 unit test suite for the focal method: testPrngValue
// This test suite follows the instructions to not use GTest and to provide
// explanatory comments for each test case. It uses a lightweight, non-terminating
// assertion mechanism to execute all tests and report a summary.

#include <string.h>
#include <lsmtest.h>
#include <cstdio>
#include <cstddef>
#include <stdio.h>
#include <stdarg.h>
#include <cstdint>


// Import the required C function and the PRNG state from the project under test.
// We declare the external C function with C linkage to avoid name mangling in C++.
extern "C" unsigned int testPrngValue(unsigned int iVal);

// The PRNG state 'r' is assumed to be exposed with a known layout in the test
// environment. We declare an anonymous struct type for extern access to its fields.
// This mirrors the usage in testPrngValue where:
//    r.aRand1[iVal & 0x000007FF]  ^  r.aRand2[(iVal>>11) & 0x000007FF]  ^  r.aRand3[(iVal>>22) & 0x000003FF]
extern "C" struct {
  unsigned int aRand1[2048];
  unsigned int aRand2[2048];
  unsigned int aRand3[1024];
} r;

// Lightweight test harness: track total tests and failures using non-terminating asserts
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating assertion macro suitable for C/C++ without external testing frameworks.
// If the assertion fails, we log the failure but continue executing remaining tests.
#define EXPECT_EQ_U32(actual, expected, msg) do {                          \
    ++g_tests_run;                                                           \
    if ((static_cast<unsigned int>(actual)) != (static_cast<unsigned int>(expected))) { \
        ++g_tests_failed;                                                    \
        fprintf(stderr, "TEST FAIL: %s | Expected: %u, Actual: %u\n",     \
                (msg), static_cast<unsigned int>(expected), static_cast<unsigned int>(actual)); \
    }                                                                      \
} while(0)

// Initialize the PRNG arrays in a deterministic way so that tests have a known baseline.
// We purposefully overwrite the external symbol 'r' arrays. If the actual project uses
// different initialization, this still provides deterministic data for testing.
static void init_prng_arrays_for_test(void) {
    for (size_t i = 0; i < 2048; ++i) {
        r.aRand1[i] = static_cast<unsigned int>(i * 7 + 1);
    }
    for (size_t i = 0; i < 2048; ++i) {
        r.aRand2[i] = static_cast<unsigned int>(i * 11 + 3);
    }
    for (size_t i = 0; i < 1024; ++i) {
        r.aRand3[i] = static_cast<unsigned int>(i * 13 + 7);
    }
}

// Unit Test 1: Verify testPrngValue computes the XOR of the three slices exactly as written.
// We exercise a set of representative iVal values, including boundary conditions and a large value
// to ensure proper indexing into all three arrays.
static void testPrngValue_basic(void) {
    init_prng_arrays_for_test();

    // Representative test vectors to cover typical, boundary, and large-index scenarios
    const unsigned int testVals[] = {
        0,              // all lower indices
        1,              // small non-zero indices
        2047,           // max index for aRand1
        2048,            // boundary that changes aRand2 and aRand1 indices
        4096,           // further shift into aRand2/index space
        0x7FFFFFFF,     // large value to exercise all three index slices
        0xFFFFFFFF      // maximum 32-bit unsigned value
    };
    const size_t nVals = sizeof(testVals) / sizeof(testVals[0]);

    for (size_t k = 0; k < nVals; ++k) {
        unsigned int iVal = testVals[k];
        unsigned int actual = testPrngValue(iVal);
        unsigned int expected = r.aRand1[iVal & 0x000007FFU]
                              ^ r.aRand2[(iVal >> 11) & 0x000007FFU]
                              ^ r.aRand3[(iVal >> 22) & 0x000003FFU];
        EXPECT_EQ_U32(actual, expected,
                      "testPrngValue should equal manual XOR of r.aRand1/2/3 slices");
    }
}

// Unit Test 2: Boundary coverage test for multiple random values to further validate slice calculations.
// This test supplements Test 1 by explicitly varying iVal to exercise edge-cases in bit slicing.
static void testPrngValue_boundary_coverage(void) {
    init_prng_arrays_for_test();

    // More boundary-oriented values, including extreme bit patterns
    const unsigned int testVals[] = {
        0x00000000,     // all slices index into 0
        0x000007FF,     // only aRand1 index changes
        0x00000800,     // aRand1 index 0, aRand2 index 1
        0x001FFFFF,     // high aRand2 index, aRand3 index 0
        0x3FFFFF00,     // high aRand2 and aRand3 indices
        0xFFFFFFFF      // max
    };
    const size_t nVals = sizeof(testVals) / sizeof(testVals[0]);

    for (size_t k = 0; k < nVals; ++k) {
        unsigned int iVal = testVals[k];
        unsigned int actual = testPrngValue(iVal);
        unsigned int expected = r.aRand1[iVal & 0x000007FFU]
                              ^ r.aRand2[(iVal >> 11) & 0x000007FFU]
                              ^ r.aRand3[(iVal >> 22) & 0x000003FFU];
        EXPECT_EQ_U32(actual, expected,
                      "Boundary test: testPrngValue slices calculation matches expected");
    }
}

// Runner that invokes all tests and prints a summary.
// Tests are designed to be non-terminating; we continue regardless of individual failures.
static void run_all_tests(void) {
    testPrngValue_basic();
    testPrngValue_boundary_coverage();

    printf("Test Summary: ran=%d, failed=%d\n", g_tests_run, g_tests_failed);
}

// Main entry point for the test executable.
// Returns non-zero if any test failed to aid automation.
int main(void) {
    run_all_tests();
    return g_tests_failed ? 1 : 0;
}