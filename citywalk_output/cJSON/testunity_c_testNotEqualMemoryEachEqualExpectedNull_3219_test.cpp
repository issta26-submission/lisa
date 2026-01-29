#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


// This test file adds focused coverage around the focal scenario:
// testNotEqualMemoryEachEqualExpectedNull. The provided Unity style tests
// exercise expectation failures when comparing memory blocks where the
// expected pointer is NULL. The added tests explore variations of NULL vs
// non-NULL pointers and memory equality to improve coverage across related
// branches of the assertion macro TEST_ASSERT_EACH_EQUAL_MEMORY.

// Ensure C linkage for the Unity test harness in case of C++ compilation.
#ifdef __cplusplus
extern "C" {
#endif

// Forward declaration of the focal test function from the existing test file.
// This is kept here for completeness if the test harness expects it to be
// discoverable, though we are primarily adding complementary test cases.
void testNotEqualMemoryEachEqualExpectedNull(void);

#ifdef __cplusplus
}
#endif

// No setup required for these tests.
void setUp(void) {}

// No teardown required for these tests.
void tearDown(void) {}

// Test 1: Expected pointer is NULL and actual memory is non-NULL.
// This mirrors the focal case and should abort (fail the test).
// Rationale: Validates that a NULL expected memory block is treated as a mismatch.
void test_NotEqualMemoryEachEqualExpectedNull_ExpectedNullActualNonNull(void)
{
    // NULL expected and a non-empty actual array should trigger a failure.
    int* pExpected = NULL;
    int actual[] = {1, 8, 987, 2};

    // Expect the assertion to fail and abort the test.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(pExpected, actual, sizeof(int), 4);
    VERIFY_FAILS_END
}

// Test 2: Expected pointer is non-NULL and actual memory is NULL.
// This variant ensures the macro also aborts when the actual block is NULL.
void test_NotEqualMemoryEachEqualExpectedNull_ActualNull(void)
{
    int expected[] = {1, 8, 987, 2};
    int* pActual = NULL;

    // Expect failure because actual memory is NULL while expected is non-NULL.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(expected, pActual, sizeof(int), 4);
    VERIFY_FAILS_END
}

// Test 3: Both pointers are NULL with a non-zero count.
// This case probes the behavior when both sides are NULL for a non-zero block.
// Depending on the implementation, this could be treated as a mismatch or an edge
// case; we expect a failure to ensure robust handling in edge scenarios.
void test_NotEqualMemoryEachEqualExpectedNull_BothNullNonZeroCount(void)
{
    int* pExpected = NULL;
    int* pActual = NULL;

    // Non-zero count with both pointers NULL should not silently pass.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(pExpected, pActual, sizeof(int), 4);
    VERIFY_FAILS_END
}

// Test 4: Both pointers non-NULL but memory contents differ.
// This confirms that non-NULL pointers are still compared by contents and aborts
// when a mismatch exists, even though NULL cases are also covered by focal tests.
void test_NotEqualMemoryEachEqualExpectedNull_NonNullPointersDifferentMemory(void)
{
    int expected[] = {1, 2, 3, 4};
    int actual[]   = {1, 2, 3, 5}; // last element differs

    // Expect failure due to memory mismatch.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(expected, actual, sizeof(int), 4);
    VERIFY_FAILS_END
}