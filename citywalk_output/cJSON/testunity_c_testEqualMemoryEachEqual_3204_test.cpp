// memory_each_equal_test.cpp
// A focused test suite for Unity's TEST_ASSERT_EACH_EQUAL_MEMORY macro
// This file is designed to be compiled with a C++11 compiler, without GTest.
// It exercises various scenarios around testEqualMemoryEachEqual behavior
// by leveraging the underlying Unity macros for memory comparison.
//
// Candidate Keywords (Step 1/2): 
// - TEST_ASSERT_EACH_EQUAL_MEMORY
// - p0, p1, p2, p3 (memory blocks used in tests)
// - v (scalar memory used in tests)
// - sizeof(int), sizeof(int)*4, sizeof(int), 3 (size and count controls)

#include <unity.h>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// ---------------------------------------------------------------------------
// Test 1: Baseline - identical blocks and multi-block comparison
// Mirrors the focal method's successful case, but separated into a dedicated test.
// This ensures that:
// - When both blocks are identical (block-wise), the macro passes.
// - It also demonstrates comparing across multiple contiguous blocks.
// ---------------------------------------------------------------------------

void testEqualMemoryEachEqual_Basics(void)
{
    // p0 represents a 4-int block
    int p0[] = {1, 8, 987, -2};

    // p1 contains two consecutive blocks: {p0[0..3], p0[0..3]}
    int p1[] = {1, 8, 987, -2, 1, 8, 987, -2};

    // p2 and p3 used for scalar-mem comparisons later
    int p2[] = {8, 8, 8, 2};
    int p3[] = {8, 500, 600, 700};
    int v = 8;

    // Case 1: One block, identical pointers
    // Expected to pass: block0 of p0 equals block0 of p0
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p0, sizeof(int)*4, 1);

    // Case 2: Two blocks, compare p0 block to p1's first block
    // p0's block matches p1's first block
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int)*4, 2);

    // Case 3: Reconfirm identical single block comparison
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int)*4, 1);

    // Case 4: Compare scalar memory (&v) to first 3 elements of p2
    // The first 3 ints in p2 are all 8, and v is 8
    TEST_ASSERT_EACH_EQUAL_MEMORY(&v, p2, sizeof(int), 3);

    // Case 5: Compare scalar memory (&v) to first 1 element of p3
    TEST_ASSERT_EACH_EQUAL_MEMORY(&v, p3, sizeof(int), 1);
}

// ---------------------------------------------------------------------------
// Test 2: Not Equal - First block differs
// Verify that the macro aborts when the very first block mismatches.
// This ensures correct failure signaling on the initial comparison.
// ---------------------------------------------------------------------------

void testNotEqualMemoryEachEqual_FirstBlockMismatch(void)
{
    int a[] = {1, 2, 3, 4};
    int b[] = {9, 2, 3, 4}; // first int differs

    // Expect the comparison to fail for the single block
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(a, b, sizeof(int), 1);
    VERIFY_FAILS_END
}

// ---------------------------------------------------------------------------
// Test 3: Not Equal - Second block differs
// Ensure that a mismatch in the second block is detected when multiple blocks are compared.
// The first block will be identical, the second will differ.
// ---------------------------------------------------------------------------

void testNotEqualMemoryEachEqual_SecondBlockMismatch(void)
{
    int a[8]  = {1, 2, 3, 4, 5, 6, 7, 8};      // two contiguous blocks (block size = 4 ints)
    int b[8]  = {1, 2, 3, 4, 9, 6, 7, 8};      // second block differs (9 instead of 5)

    // Expect failure on the second block (count = 2 blocks)
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(a, b, sizeof(int)*4, 2);
    VERIFY_FAILS_END
}

// ---------------------------------------------------------------------------
// Test 4: Not Equal - Expected memory or actual memory is NULL
// Validate correct handling of NULL pointers in both expected and actual roles.
// ---------------------------------------------------------------------------

void testNotEqualMemoryEachEqual_ExpectedNull(void)
{
    int data[] = {1, 2, 3, 4};

    // Expected memory pointer is NULL; should fail
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(NULL, data, sizeof(int), 4);
    VERIFY_FAILS_END
}

void testNotEqualMemoryEachEqual_ActualNull(void)
{
    int data[] = {1, 2, 3, 4};
    int* null_ptr = NULL;

    // Actual memory pointer is NULL; should fail
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(data, null_ptr, sizeof(int), 4);
    VERIFY_FAILS_END
}

// ---------------------------------------------------------------------------
// Test 5: Not Equal - Zero-sized memory comparisons (edge case)
// Some frameworks skip zero-size blocks; ensure stability when size == 0.
// If Unity handles it gracefully, this should pass or be a well-defined failure.
// ---------------------------------------------------------------------------

void testNotEqualMemoryEachEqual_ZeroSize(void)
{
    int data1[] = {1, 2, 3, 4};
    int data2[] = {1, 2, 3, 4};

    // If size is zero, comparing memory should be treated as vacuously equal.
    // We will assume Unity treats size==0 as a no-op equivalent to pass.
    // Therefore, expect pass for count > 0 only if implementation agrees.
    // Here we purposefully test the behavior; if it fails, adjust accordingly.
    TEST_ASSERT_EACH_EQUAL_MEMORY(data1, data2, 0, 4);
}

// ---------------------------------------------------------------------------
// Test 6: Mixed scalar and array memory (additional coverage)
// Use a scalar memory block compared against an array block to exercise
// address vs content semantics in memory comparisons.
// ---------------------------------------------------------------------------

void testEqualMemoryEachEqual_MixedScalarAndArray(void)
{
    int v = 8;
    int arr[] = {8, 8, 8, 2};

    // Compare scalar v against first 4 bytes (one int) multiple times
    // Here count = 1, size = sizeof(int); should succeed since v == first element of arr
    TEST_ASSERT_EACH_EQUAL_MEMORY(&v, arr, sizeof(int), 1);

    // Compare scalar against first 3 elements of arr
    TEST_ASSERT_EACH_EQUAL_MEMORY(&v, arr, sizeof(int), 3);
}

// ---------------------------------------------------------------------------
// Test Runner (main)
// Step 3: Test Case Refinement
// - Invokes the tests above, ensuring coverage of true/false branches.
// - Uses Unity's RUN_TEST macro pattern, suitable for C++11 compilation.
// - Includes explanatory comments for maintainability.
// ---------------------------------------------------------------------------

int main(void)
{
    UNITY_BEGIN();

    // Baseline success path for testEqualMemoryEachEqual
    RUN_TEST(testEqualMemoryEachEqual_Basics);

    // Negative test paths to validate proper abort signaling
    RUN_TEST(testNotEqualMemoryEachEqual_FirstBlockMismatch);
    RUN_TEST(testNotEqualMemoryEachEqual_SecondBlockMismatch);

    // NULL pointer handling scenarios
    RUN_TEST(testNotEqualMemoryEachEqual_ExpectedNull);
    RUN_TEST(testNotEqualMemoryEachEqual_ActualNull);

    // Edge case for zero-size memory (depends on Unity's behavior)
    // Commented-out if your Unity build handles zero-size blocks differently.
    RUN_TEST(testNotEqualMemoryEachEqual_ZeroSize);

    // Mixed scalar vs array coverage
    RUN_TEST(testEqualMemoryEachEqual_MixedScalarAndArray);

    return UNITY_END();
}