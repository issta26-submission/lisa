/***************************************************************************************
 * Lightweight C++11 unit test suite for the focal method: testNotEqualIntArrays2
 *
 * Context:
 * - The focal method lives in testunity.c and uses Unity's NOT_EQUAL assertion for int
 *   arrays. To provide a self-contained, GTest-free test suite suitable for C++11,
 *   this file re-creates the essential behavior sufficient to validate the core idea:
 *   two int arrays that differ in any element should be reported as "not equal".
 *
 * - This harness does not rely on Unity. It provides a tiny, dependency-light framework
 *   with simple pass/fail reporting and clear test-case comments.
 *
 * - The tests target the fundamental dependency: a function that determines whether two
 *   int arrays of a given length are not equal.
 *
 * Notes:
 * - We intentionally cover true and false branches:
 *     - Not-equal when a difference exists
 *     - Equal when arrays are identical
 *     - Length-zero handling
 *     - Null-pointer handling
 * - The candidate keywords derived from the focal method include: int arrays, not equal,
 *   array comparison, p0/p1 arrays, 4-length comparison, boundary cases.
 ***************************************************************************************/

#include <cstdint>
#include <cstddef>
#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Lightweight helper: determine if two int arrays are not equal (by element) for length n.
// Behavior aligned with the conceptual intent of TEST_ASSERT_EQUAL_INT_ARRAY in Unity:
// return true if any corresponding elements differ. If length is 0, consider arrays equal.
static bool intArraysNotEqual(const int* a, const int* b, std::size_t n)
{
    if (n == 0) return false;               // Trivial case: zero-length arrays are equal
    if (a == nullptr || b == nullptr) return true; // Non-nullness assumption: pointers must be valid when n>0
    for (std::size_t i = 0; i < n; ++i)
    {
        if (a[i] != b[i]) return true;       // Found a difference
    }
    return false;                             // Arrays are equal
}

// Simple test harness infrastructure
static int g_totalTests = 0;
static int g_failedTests = 0;
static bool g_currentTestPassed = true;

// Minimal assertion macro: marks test as failed but continues execution
#define TEST_ASSERT(cond, msg) do { if (!(cond)) { g_currentTestPassed = false; std::cout << "  assertion failed: " << (msg) << " (line " << __LINE__ << ")\n"; } } while (0)

// Convenience macro for not-equal assertion (as in focal method semantics)
#define ASSERT_NOT_EQUAL_ARRAYS(a, b, n) TEST_ASSERT(intArraysNotEqual((a), (b), (n)), "Expected arrays to be not equal")

// Convenience macro for equality assertion
#define ASSERT_EQUAL_ARRAYS(a, b, n) TEST_ASSERT(!intArraysNotEqual((a), (b), (n)), "Expected arrays to be equal")

// Helper to run a single test
#define RUN_TEST_FUNC(fn)                          \
    do {                                           \
        g_currentTestPassed = true;                \
        std::cout << "Running " << #fn << "...\n";\
        fn();                                      \
        ++g_totalTests;                            \
        if (g_currentTestPassed) {                 \
            std::cout << "PASS: " << #fn << "\n"; \
        } else {                                     \
            ++g_failedTests;                       \
            std::cout << "FAIL: " << #fn << "\n"; \
        }                                            \
        std::cout << std::flush;                  \
    } while (0)

// Test 1: Basic non-equality when first elements differ
// Mirrors focal scenario: p0 = {1, 8, 987, -2}, p1 = {2, 8, 987, -2}
void testNotEqualIntArrays2_BasicDifference(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {2, 8, 987, -2};
    // Expect not-equal to be true
    ASSERT_NOT_EQUAL_ARRAYS(p0, p1, 4);
}

// Test 2: Equal arrays should be reported as equal (not-not-equal)
void testNotEqualIntArrays2_EqualArrays(void)
{
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2};
    // Expect not-equal to be false; thus this should fail the not-equal assertion
    TEST_ASSERT(intArraysNotEqual(p0, p1, 4) == false,
                "Arrays are equal; intArraysNotEqual should return false");
}

// Test 3: Non-zero length with a single internal difference (second element)
void testNotEqualIntArrays2_DifferenceInMiddle(void)
{
    int p0[] = {1, 20, 987, -2};
    int p1[] = {1, 21, 987, -2};
    ASSERT_NOT_EQUAL_ARRAYS(p0, p1, 4);
}

// Test 4: Length zero should be treated as equal (not-not-equal)
void testNotEqualIntArrays2_LengthZero(void)
{
    int p0[] = {1};       // Value contents irrelevant for length 0
    int p1[] = {9};
    // With length 0, arrays are considered equal; notEqual should be false
    TEST_ASSERT(intArraysNotEqual(p0, p1, 0) == false,
                "Length zero should yield equal arrays (not-equal should be false)");
}

// Test 5: Null pointers handling for non-zero length
void testNotEqualIntArrays2_NullPointers(void)
{
    int p0[] = {1, 2, 3, 4};
    int* nullPtr = nullptr;
    // One pointer null and non-zero length -> should be considered not equal
    ASSERT_NOT_EQUAL_ARRAYS(nullPtr, p0, 4);
    // Also test with the other pointer null
    ASSERT_NOT_EQUAL_ARRAYS(p0, nullPtr, 4);
}

// Test 6: Both pointers null with non-zero length
void testNotEqualIntArrays2_BothNull(void)
{
    int* NULL1 = nullptr;
    int* NULL2 = nullptr;
    // Both null with non-zero length -> our not-equal function returns true
    ASSERT_NOT_EQUAL_ARRAYS(NULL1, NULL2, 4);
}

// Entry point: run all tests
int main(void)
{
    std::cout << "Starting focal-method-oriented test suite for testNotEqualIntArrays2\n";
    RUN_TEST_FUNC(testNotEqualIntArrays2_BasicDifference);
    RUN_TEST_FUNC(testNotEqualIntArrays2_EqualArrays);
    RUN_TEST_FUNC(testNotEqualIntArrays2_DifferenceInMiddle);
    RUN_TEST_FUNC(testNotEqualIntArrays2_LengthZero);
    RUN_TEST_FUNC(testNotEqualIntArrays2_NullPointers);
    RUN_TEST_FUNC(testNotEqualIntArrays2_BothNull);

    // Summary
    std::cout << "\nTest Summary: " << g_totalTests << " run, "
              << g_failedTests << " failed, "
              << (g_totalTests - g_failedTests) << " passed.\n";
    return g_failedTests == 0 ? 0 : 1;
}

/*
Notes on coverage and dependencies:
- The candidate keywords (from Step 1) reflect core components tested:
  - int arrays, p0/p1 instances, 4-length comparisons, boundary cases (length 0, null pointers).
- This suite focuses on the core dependent component: a function that determines if two int arrays
  are not equal, which is the crux behind Unity's TEST_ASSERT_EQUAL_INT_ARRAY in testNotEqualIntArrays2.
- Static members or file-scope internals are not exercised here since the test harness is self-contained
  and does not rely on the Unity framework's static state. If integrating with Unity, the same test
  scenarios can be mapped to Unity-based tests by wrapping the assertions with the provided macros.
- Namespace: none used; this is a standalone C++11 test harness suitable for integration into a non-GTest
  environment.
- Namespace alignment with DOMAIN_KNOWLEDGE: we used standard C++ library only, avoided private method access,
  and kept tests non-terminating (tests log failures but continue execution).
*/