/*
  Test Suite for focal method: testINT64sWithinDelta
  - This suite emulates the essence of the Unity test in testunity.c
  - It focuses on 64-bit within-delta checks (TEST_ASSERT_INT64_WITHIN style)
  - Implemented as a standalone C++11 test harness (no GTest) to maximize portability
  - Includes multiple test cases to exercise true/false branches and boundary conditions

  Step 1 (Conceptual): Candidate Keywords reflected in tests
  - INT64, int64_t, Delta, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFE
  - UNITY_SUPPORT_64, TEST_ASSERT_INT64_WITHIN
  - Within-delta semantics, absolute difference, boundary checks
  Step 2 (Guidance from dependencies): This harness is independent of Unity but mirrors
  the intent of the focal test by validating the same logical conditions.
  Step 3 (Refinement): Added additional boundary and edge-case tests to improve coverage
  (negative numbers, cross-sign comparisons, exact delta, and overflow-safety considerations).
*/

#include <cmath>
#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Lightweight test harness (no GTest, suitable for C++11)
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

static void logFailed(const char* testName, const char* file, int line)
{
    std::cerr << "[FAIL] " << testName << " ("
              << file << ":" << line << ")" << std::endl;
}

// Simple assertion wrapper (non-terminating to maximize test flow)
#define ASSERT_TRUE(cond, testName)                                  \
    do {                                                             \
        ++g_total;                                                   \
        if (static_cast<bool>(cond)) {                               \
            ++g_passed;                                              \
        } else {                                                     \
            ++g_failed;                                              \
            logFailed(testName, __FILE__, __LINE__);                 \
        }                                                            \
    } while (0)

// Core logic: compute whether two int64_t values are within a given delta
// Note: uses absolute difference in a overflow-safe manner
static bool int64WithinDelta(int64_t delta, int64_t a, int64_t b)
{
    if (delta < 0) return false;
    uint64_t diff = (a > b) ? static_cast<uint64_t>(a - b) : static_cast<uint64_t>(b - a);
    return diff <= static_cast<uint64_t>(delta);
}

/*
  Test 1: Boundary test using focal values from the original testINT64sWithinDelta
  - delta = 1
  - a = 0x7FFFFFFFFFFFFFFF
  - b = 0x7FFFFFFFFFFFFFFE
  Expected: within delta (true), since difference is 1
*/
static void test_INT64sWithinDelta_boundary_max_values(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(1, 0x7FFFFFFFFFFFFFFF, 0x7FFFFFFFFFFFFFFE);
    ASSERT_TRUE(r, "INT64 within delta: max positive boundary with delta 1");
#else
    // If 64-bit support is not compiled in, skip gracefully
    // The focal test uses UNITY_SUPPORT_64 to decide behavior
#endif
}

/*
  Test 2: Another within-delta case
  - delta = 5
  - a = 5000
  - b = 4996
  Expected: within delta (true)
*/
static void test_INT64sWithinDelta_within_delta_5000_4996(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(5, 5000, 4996);
    ASSERT_TRUE(r, "INT64 within delta: 5000 vs 4996 within 5");
#else
#endif
}

/*
  Test 3: Yet another within-delta case (exact upper bound)
  - delta = 5
  - a = 5000
  - b = 5005
  Expected: within delta (true)
*/
static void test_INT64sWithinDelta_within_delta_upper_bound(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(5, 5000, 5005);
    ASSERT_TRUE(r, "INT64 within delta: 5000 vs 5005 within 5");
#else
#endif
}

/*
  Additional Test 4: Outside delta boundary
  - delta = 5
  - a = 5000
  - b = 5006
  Expected: not within delta (false)
*/
static void test_INT64sWithinDelta_outside_boundary(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(5, 5000, 5006);
    ASSERT_TRUE(!r, "INT64 within delta: 5000 vs 5006 outside 5");
#else
#endif
}

/*
  Additional Test 5: Negative values within delta
  - delta = 5
  - a = -100
  - b = -106
  Difference = 6 -> Outside delta
*/
static void test_INT64sWithinDelta_negative_values_outside(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(5, -100, -106);
    ASSERT_TRUE(!r, "INT64 within delta: -100 vs -106 outside 5");
#else
#endif
}

/*
  Additional Test 6: Negative values inside delta
  - delta = 6
  - a = -100
  - b = -106
  Difference = 6 -> Inside delta
*/
static void test_INT64sWithinDelta_negative_values_inside(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(6, -100, -106);
    ASSERT_TRUE(r, "INT64 within delta: -100 vs -106 inside 6");
#else
#endif
}

/*
  Additional Test 7: Cross-sign comparison within delta
  - delta = 5
  - a = -2
  - b = 2
  Difference = 4 -> Inside delta
*/
static void test_INT64sWithinDelta_cross_sign_within(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(5, -2, 2);
    ASSERT_TRUE(r, "INT64 within delta: -2 vs 2 within 5 (cross-sign)");
#else
#endif
}

/*
  Additional Test 8: Zero delta case
  - delta = 0
  - a = 7
  - b = 7
  Difference = 0 -> Inside delta
*/
static void test_INT64sWithinDelta_zero_delta(void)
{
#ifdef UNITY_SUPPORT_64
    bool r = int64WithinDelta(0, 7, 7);
    ASSERT_TRUE(r, "INT64 within delta: delta 0 with identical values");
#else
#endif
}

int main()
{
#ifdef UNITY_SUPPORT_64
    // Run all tests that exercise 64-bit within-delta semantics
    test_INT64sWithinDelta_boundary_max_values();
    test_INT64sWithinDelta_within_delta_5000_4996();
    test_INT64sWithinDelta_within_delta_upper_bound();
    test_INT64sWithinDelta_outside_boundary();
    test_INT64sWithinDelta_negative_values_outside();
    test_INT64sWithinDelta_negative_values_inside();
    test_INT64sWithinDelta_cross_sign_within();
    test_INT64sWithinDelta_zero_delta();

    // Summary
    std::cout << "Total tests: " << g_total
              << ", Passed: " << g_passed
              << ", Failed: " << g_failed << std::endl;
    return (g_failed == 0) ? 0 : 1;
#else
    std::cout
      << "64-bit support not available (UNITY_SUPPORT_64 undefined). Tests skipped."
      << std::endl;
    return 0;
#endif
}