// Test suite for the focal method testPrngArray in lsmtest_util.c
// This test suite is written for C++11, without Google Test.
// It relies on the C implementations provided in lsmtest_util.c, including testPrngArray and testPrngValue.
// The tests use a lightweight, non-terminating assertion mechanism to maximize code coverage.

#include <string.h>
#include <lsmtest.h>
#include <cstdio>
#include <cstddef>
#include <stdio.h>
#include <stdarg.h>
#include <cstdint>


// Declare the C functions from the target file using C linkage.
extern "C" {
    // The actual PRNG value generator provided elsewhere in the project.
    unsigned int testPrngValue(unsigned int iVal);

    // The function under test: fills aOut[0 .. nOut-1] with testPrngValue(iVal+i)
    void testPrngArray(unsigned int iVal, unsigned int *aOut, int nOut);
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Expectation macros that preserve execution flow on failure.
#define EXPECT_EQ(expected, actual) do {                                      \
    ++g_tests_run;                                                             \
    if ((expected) != (actual)) {                                              \
        ++g_tests_failed;                                                      \
        std::fprintf(stderr, "EXPECT_EQ failed at %s:%d: expected %llu, got %llu\n",\
                     __FILE__, __LINE__, (unsigned long long)(expected),             \
                     (unsigned long long)(actual));                              \
    }                                                                          \
} while (0)

#define EXPECT_TRUE(condition) do {                                             \
    ++g_tests_run;                                                               \
    if (!(condition)) {                                                            \
        ++g_tests_failed;                                                          \
        std::fprintf(stderr, "EXPECT_TRUE failed at %s:%d: condition is false\n",   \
                     __FILE__, __LINE__);                                        \
    }                                                                            \
} while (0)

#define EXPECT_FALSE(condition) do {                                            \
    ++g_tests_run;                                                               \
    if ((condition)) {                                                             \
        ++g_tests_failed;                                                          \
        std::fprintf(stderr, "EXPECT_FALSE failed at %s:%d: condition is true\n",  \
                     __FILE__, __LINE__);                                        \
    }                                                                            \
} while (0)

// Helper: print final summary
static void print_summary() {
    std::fprintf(stdout, "LSMTestUtil::testPrngArray suite: tests=%d, failures=%d\n",
                 g_tests_run, g_tests_failed);
}

// Test 1: Basic correctness
// - Purpose: verify that testPrngArray writes exactly testPrngValue(iVal+i) for each i in [0, nOut-1].
// - Rationale: Since testPrngArray delegates to testPrngValue, the outputs must match
//   the independent computation using the same deterministic function.
static void testPrngArray_basic_correctness() {
    const unsigned int iVal = 10;
    const int N = 8;
    unsigned int aOut[N];

    // Execute function under test
    testPrngArray(iVal, aOut, N);

    // Validate each position against a direct computation of testPrngValue(iVal + i)
    for (int i = 0; i < N; ++i) {
        unsigned int expected = testPrngValue(iVal + static_cast<unsigned int>(i));
        EXPECT_EQ(expected, aOut[i]);
    }
}

// Test 2: Zero-length output array (nOut == 0)
// - Purpose: ensure that the function does not write when nOut is zero.
// - Rationale: The function should perform no writes and should be side-effect free for aOut elements.
static void testPrngArray_zero_length() {
    // Prepare aOut with sentinel values to detect any unintended writes beyond nOut
    const int CAPACITY = 5;
    unsigned int aOut[CAPACITY];
    for (int i = 0; i < CAPACITY; ++i) {
        aOut[i] = 0xA5A5A5A5u;
    }

    // Call with nOut = 0; no elements should be modified
    testPrngArray(123u, aOut, 0);

    // Verify that all elements remain unchanged
    for (int i = 0; i < CAPACITY; ++i) {
        EXPECT_EQ(0xA5A5A5A5u, aOut[i]);
    }
}

// Test 3: Wrap-around behavior of iVal
// - Purpose: ensure that unsigned int arithmetic handles wrap-around correctly.
// - Rationale: iVal + i may overflow; C unsigned arithmetic wraps modulo 2^32.
// - Validation: compare against testPrngValue(iVal + i) for i in a small range.
static void testPrngArray_wraparound() {
    const unsigned int iVal = 0xFFFFFFFFu - 1; // near max, to force wrap
    const int N = 4;
    unsigned int aOut[N];
    testPrngArray(iVal, aOut, N);
    for (int i = 0; i < N; ++i) {
        unsigned int idx = iVal + static_cast<unsigned int>(i); // wraps around naturally
        unsigned int expected = testPrngValue(idx);
        EXPECT_EQ(expected, aOut[i]);
    }
}

// Test 4: Negative nOut (nOut < 0) should result in no writes
// - Purpose: verify behavior when nOut is negative (no iterations).
static void testPrngArray_negative_length() {
    const unsigned int iVal = 55;
    const int N = -5;
    const int CAPACITY = 4;
    unsigned int aOut[CAPACITY];
    // Initialize with distinct values
    for (int i = 0; i < CAPACITY; ++i) {
        aOut[i] = 0xDEADBEEFu;
    }
    testPrngArray(iVal, aOut, N);

    // Since no iterations should occur, array should remain unchanged
    for (int i = 0; i < CAPACITY; ++i) {
        EXPECT_EQ(0xDEADBEEFu, aOut[i]);
    }
}

// Test 5: Multiple sequential blocks to stress multiple calls
// - Purpose: ensure repeated invocations with varying iVal values behave consistently.
// - Rationale: The function should independently populate aOut for each call without cross-contamination.
static void testPrngArray_multiple_calls_consistency() {
    const int N = 16;
    unsigned int aOut[N];

    // First call
    testPrngArray(0, aOut, N);
    for (int i = 0; i < N; ++i) {
        unsigned int expected = testPrngValue(static_cast<unsigned int>(i));
        EXPECT_EQ(expected, aOut[i]);
    }

    // Second call with different base iVal
    testPrngArray(1000u, aOut, N);
    for (int i = 0; i < N; ++i) {
        unsigned int expected = testPrngValue(1000u + static_cast<unsigned int>(i));
        EXPECT_EQ(expected, aOut[i]);
    }
}

// Runner that executes all tests
static void run_all_tests() {
    testPrngArray_basic_correctness();
    testPrngArray_zero_length();
    testPrngArray_wraparound();
    testPrngArray_negative_length();
    testPrngArray_multiple_calls_consistency();

    print_summary();
}

// Entry point
int main() {
    // Run all test cases
    run_all_tests();

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}