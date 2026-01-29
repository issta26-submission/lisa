/*
  Minimal C++11 test harness to validate the behavior of the focal method equivalent
  to testEqualUint64Arrays from testunity.c without depending on GTest or Unity.

  Notes:
  - This harness provides a light-weight replacement for Unity-style tests focused on
    64-bit unsigned array equality.
  - It covers true and false branches, including edge cases like zero length and null pointers.
  - Static members: demonstrated via a small static counter in test execution flow.
  - All tests are non-terminating: failures are recorded but do not abort the test execution,
    allowing multiple assertions within a single test to run.
  - The test suite can be compiled with a C++11 compiler.
  - If you want to disable 64-bit specific tests, define SUPPORTED_UINT64 to 0.
*/

/* Candidate Keywords extracted from the focal method context:
   - UINT64 type: uint64_t (UNITY_UINT64 alias in the original code)
   - Function under test (conceptual): equal_uint64_arrays / TEST_ASSERT_EQUAL_UINT64_ARRAY
   - Array literals and element-wise comparison semantics
   - Null pointer handling and length-based comparison
   - Test harness macros/structures to mimic Unity-like assertions
*/

/* Includes and basic test framework */
#include <cstddef>
#include <unity.h>
#include <cstring>
#include <vector>
#include <cstdio>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Enable/disable 64-bit test coverage (default: enabled)
#ifndef SUPPORTED_UINT64
#define SUPPORTED_UINT64 1
#endif

// Lightweight test state
static const char* gCurrentTestName = nullptr;
static int gCurrentTestFailed = 0;

// Simple equality check for two uint64_t arrays of given length
// Handles edge cases similarly to Unity's expectations:
// - If length is 0, arrays are considered equal
// - If either pointer is NULL and length > 0, consider not equal
static bool equal_uint64_arrays(const uint64_t* a, const uint64_t* b, size_t len)
{
    if (len == 0) {
        return true;
    }
    if (a == nullptr || b == nullptr) {
        return false;
    }
    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

// Macro to emulate Unity's TEST_ASSERT_EQUAL_UINT64_ARRAY behavior in a non-terminating way
#define TEST_ASSERT_EQUAL_UINT64_ARRAY(A, B, LEN) do {                                               \
    if (!equal_uint64_arrays((A), (B), (LEN))) {                                                  \
        printf("Assertion FAILED in test '%s' at line %d: arrays differ for length %zu\n",      \
               gCurrentTestName, __LINE__, (size_t)(LEN));                                       \
        gCurrentTestFailed = 1;                                                                   \
    }                                                                                               \
} while (0)

// Helper to run a test function and report result
static void run_test(const char* testName, void (*testFunc)())
{
    gCurrentTestName = testName;
    gCurrentTestFailed = 0;
    testFunc();
    if (gCurrentTestFailed) {
        printf("Test %s: FAILED\n", testName);
    } else {
        printf("Test %s: PASSED\n", testName);
    }
}

// Domain knowledge comment: The focal class dependencies would be something like Unity's
// UNITY_UINT64 type and the TEST_ASSERT_EQUAL_UINT64_ARRAY assertion.
// In this minimal harness we implement the essential behavior for correctness checking.

#if SUPPORTED_UINT64
// Test 1: True branches of array equality with 64-bit elements
// Replicates the core scenarios of the original testEqualUint64Arrays function when 64-bit support is available.
void testEqualUint64Arrays_trueBranches(void)
{
    uint64_t p0[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    uint64_t p1[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    uint64_t p2[] = {1ULL, 8ULL, 987ULL, 2ULL};
    uint64_t p3[] = {1ULL, 500ULL, 600ULL, 700ULL};

    // Cases that should pass
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p0, 1);  // first element equal (size 1)
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p0, 4);  // full equality with itself
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p1, 4);  // identical arrays
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p2, 3);  // first 3 elements equal
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p3, 1);  // first element equal
}
#endif

#if SUPPORTED_UINT64
// Test 2: Negative path - arrays not equal for a given length
void testEqualUint64Arrays_notEqualLastElement(void)
{
    uint64_t p0[] = {1ULL, 8ULL, 987ULL, 65132ULL};
    uint64_t p1[] = {1ULL, 8ULL, 987ULL, 0ULL}; // last element differs

    // This should fail the assertion for length 4
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p1, 4);
}
#endif

#if SUPPORTED_UINT64
// Test 3: Edge case - zero length should always be considered equal
void testEqualUint64Arrays_zeroLength(void)
{
    uint64_t p0[] = {1ULL, 2ULL, 3ULL, 4ULL};

    // len = 0 should pass regardless of contents
    TEST_ASSERT_EQUAL_UINT64_ARRAY(p0, p0, 0);
}
#endif

#if SUPPORTED_UINT64
// Test 4: Null pointer handling
void testEqualUint64Arrays_nullPointerCases(void)
{
    uint64_t valid[] = {1ULL, 2ULL, 3ULL, 4ULL};

    // a is NULL, b is valid, len > 0 -> should fail
    TEST_ASSERT_EQUAL_UINT64_ARRAY(nullptr, valid, 4);

    // both NULL with len 0 -> should pass
    TEST_ASSERT_EQUAL_UINT64_ARRAY(nullptr, nullptr, 0);
}
#endif

int main(void)
{
    printf("Running test suite for focal method: testEqualUint64Arrays equivalent\n");

#if SUPPORTED_UINT64
    run_test("testEqualUint64Arrays_trueBranches", testEqualUint64Arrays_trueBranches);
    // Note: The next test demonstrates a failing case; in a real harness we would separate
    // passing vs failing tests. Here we still execute to ensure coverage of negative path.
    run_test("testEqualUint64Arrays_notEqualLastElement", testEqualUint64Arrays_notEqualLastElement);
    run_test("testEqualUint64Arrays_zeroLength", testEqualUint64Arrays_zeroLength);
    run_test("testEqualUint64Arrays_nullPointerCases", testEqualUint64Arrays_nullPointerCases);
#else
    printf("64-bit unsigned integer support is disabled (SUPPORTED_UINT64 == 0). Skipping tests.\n");
#endif

    printf("Test suite execution completed.\n");
    return 0;
}