// Unit test suite for sqlite3AddInt64 (C function) using a lightweight, non-terminating test harness.
// The test harness is written in C++11 and does not rely on GTest or any external framework.
// It calls the C function sqlite3AddInt64 and verifies expected behavior for various edge and normal cases.
// Explanatory comments are provided for each test case to describe the scenario being validated.

#include <math.h>
#include <limits>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// Declaration of the focal function from util.c (C linkage to avoid name mangling when linked with C++).
extern "C" int sqlite3AddInt64(long long *pA, long long iB);

// Global counter for test failures (non-terminating; we continue running tests).
static int g_failures = 0;

// Lightweight assertion macros that do not terminate the test program on failure.
#define CHECK_EQ(expected, actual, msg) do { \
    if ((long long)(expected) != (long long)(actual)) { \
        ++g_failures; \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } \
} while(0)

#define CHECK_TRUE(cond, msg) do { \
    if (!(cond)) { \
        ++g_failures; \
        std::cerr << "ASSERTION FAILED: " << (msg) << std::endl; \
    } \
} while(0)


// Test 1: No overflow case with small positive values; pA should update to iB and return 0.
static void test_AddInt64_NoOverflow_SmallPositive()
{
    // Setup
    long long a = 0;
    long long b = 5;

    // Action
    int res = sqlite3AddInt64(&a, b);

    // Verification
    CHECK_EQ(0, res, "No overflow should occur for 0 + 5");
    CHECK_EQ(5, a, "pA should be updated to 5 after adding 5");
}

// Test 2: No overflow at boundary near LARGEST_INT64; a = MAX-1, add 1 -> MAX
static void test_AddInt64_NoOverflow_BoundaryMaxPlusOne()
{
    long long a = std::numeric_limits<long long>::max() - 1;
    long long b = 1;
    int res = sqlite3AddInt64(&a, b);
    CHECK_EQ(0, res, "No overflow should occur when adding 1 to MAX-1");
    CHECK_EQ(std::numeric_limits<long long>::max(), a, "Result should be MAX");
}

// Test 3: Overflow path for positive addend when iB >= 0; a = 1, b = MAX -> should overflow (return 1)
// In this case, pA should remain unchanged.
static void test_AddInt64_Overflow_PositiveAddend()
{
    long long a = 1;
    long long b = std::numeric_limits<long long>::max();
    long long a_before = a;

    int res = sqlite3AddInt64(&a, b);

    CHECK_EQ(1, res, "Overflow should be detected for 1 + MAX");
    CHECK_EQ(a_before, a, "pA should remain unchanged on overflow (positive addend)");
}

// Test 4: Overflow path for negative addend with iB == -1 from a negative value; no overflow in this specific case
// Example: a = -5, b = -1 -> result -6, no overflow
static void test_AddInt64_NoOverflow_NegativeAddend_NoOverflowCase()
{
    long long a = -5;
    long long b = -1;
    int res = sqlite3AddInt64(&a, b);
    CHECK_EQ(0, res, "No overflow should occur for -5 + (-1)");
    CHECK_EQ(-6, a, "Result should be -6 after adding -1 to -5");
}

// Test 5: Negative path overflow scenario with a negative value and extreme MIN input
// a = -2, b = MIN -> should overflow according to the negative-branch condition
// The function should return 1 and not modify pA.
static void test_AddInt64_Overflow_NegativeAddend_ExtremeMIN()
{
    long long a = -2;
    long long b = std::numeric_limits<long long>::min();
    long long a_before = a;

    int res = sqlite3AddInt64(&a, b);

    CHECK_EQ(1, res, "Overflow should be detected for -2 + MIN (negative addend)");
    CHECK_EQ(a_before, a, "pA should remain unchanged on overflow (negative addend)");
}

// Test 6: Negative path near boundary where adding -1 results in MIN boundary, no overflow
// a = -LLONG_MAX, b = -1 -> a becomes MIN (no overflow in signed arithmetic within C98/C99 range)
static void test_AddInt64_NoOverflow_NegativeBoundaryToMin()
{
    long long a = -std::numeric_limits<long long>::max();
    long long b = -1;
    int res = sqlite3AddInt64(&a, b);
    CHECK_EQ(0, res, "No overflow when -LLONG_MAX + (-1) -> MIN");
    CHECK_EQ(std::numeric_limits<long long>::min(), a, "Result should be MIN after addition");
}

// Test 7: Overflow path for negative addend with MIN edge-case (iA = -2, iB = MIN) -> overflow
static void test_AddInt64_Overflow_NegativeAddend_MinEdge()
{
    long long a = -2;
    long long b = std::numeric_limits<long long>::min();
    int res = sqlite3AddInt64(&a, b);
    CHECK_EQ(1, res, "Overflow should be detected for -2 + MIN (negative, extreme case)");
    CHECK_EQ(-2, a, "pA should remain unchanged on overflow (negative edge case)");
}

// Test 8: Overflow path for large positive addend when iA > 0 and LARGEST_INT64 - iA < iB
// a = 1, b = MAX -> should overflow
// This ensures the overflow check condition LARGEST_INT64 - iA < iB is exercised.
static void test_AddInt64_Overflow_OverflowCheckClassic()
{
    long long a = 1;
    long long b = std::numeric_limits<long long>::max();
    int res = sqlite3AddInt64(&a, b);
    CHECK_EQ(1, res, "Overflow should occur for 1 + MAX due to positive overflow check");
    CHECK_EQ(1, a, "pA should remain unchanged on overflow (in fast-path semantics)");
}

// Test 9: Confirm simple non-overflow when adding zero
static void test_AddInt64_NoOverflow_AddZero()
{
    long long a = 12345;
    long long b = 0;
    int res = sqlite3AddInt64(&a, b);
    CHECK_EQ(0, res, "Adding zero should not overflow");
    CHECK_EQ(12345, a, "pA should remain unchanged after adding zero");
}


// Run all tests and report summary
static void run_all_tests()
{
    test_AddInt64_NoOverflow_SmallPositive();
    test_AddInt64_NoOverflow_BoundaryMaxPlusOne();
    test_AddInt64_Overflow_PositiveAddend();
    test_AddInt64_NoOverflow_NegativeAddend_NoOverflowCase();
    test_AddInt64_Overflow_NegativeAddend_ExtremeMIN();
    test_AddInt64_NoOverflow_NegativeBoundaryToMin();
    test_AddInt64_Overflow_NegativeAddend_MinEdge();
    test_AddInt64_Overflow_OverflowCheckClassic();
    test_AddInt64_NoOverflow_AddZero();

    // Summary
    std::cerr << "sqlite3AddInt64 test suite finished. Failures: " << g_failures << std::endl;
}


int main()
{
    run_all_tests();
    // Return nonzero if any test failed to aid integration with simple scripts.
    return g_failures;
}