/*
 * Test Suite: testNotEqualMemory3-suite
 * Environment: C++11 compatible, Unity test framework (no GTest)
 * Purpose: Extend coverage around the focal testNotEqualMemory3 method
 *          which verifies that TEST_ASSERT_EQUAL_MEMORY(NULL, "food", 4) aborts.
 * Rationale: Provide additional tests to cover both failing and passing
 *            scenarios related to memory comparisons, using the same Unity
 *            macros and non-terminating assertions where applicable.
 * Notes:
 *  - This file is designed to compile with a project that already includes
 *    the Unity headers (unity.h) and the Unity test runner artifacts.
 *  - The tests use only standard C/C++ compatible constructs; do not rely on GTest.
 *  - If integrating into an existing test harness, ensure UnityBegin/UnityEnd
 *    or RUN_TEST are wired as in your project.
 */

#include <stddef.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif

// Forward declarations (not strictly required, but explicit for clarity)
void setUp(void);
void tearDown(void);

#ifdef __cplusplus
}
#endif

// Minimal setup/teardown to keep Unity state consistent
void setUp(void) { /* no-op setup for tests */ }
void tearDown(void) { /* no-op teardown for tests */ }

/*
 * Test 1: Fails as in the focal method
 * Purpose: Ensure that the focal scenario still aborts when memory pointers
 *          are NULL vs a non-null memory region of 4 bytes ("food").
 * This directly mirrors the provided testNotEqualMemory3 in testunity.c.
 */
void testNotEqualMemory3_FailsWhenFirstArgNull(void)
{
    // This test should abort due to non-equality, captured by EXPECT_ABORT_BEGIN/END
    {
        EXPECT_ABORT_BEGIN
        TEST_ASSERT_EQUAL_MEMORY(NULL, "food", 4);
        VERIFY_FAILS_END
    }
}

/*
 * Test 2: Passes when both memory blocks are identical (non-NULL)
 * Purpose: Validate that when memory regions are equal, the assertion does not abort.
 * This complements the focal test by confirming a non-failing path for similar APIs.
 */
void testNotEqualMemory3_PassesWhenMemoryIsIdenticalNonNull(void)
{
    // Compare two identical memory blocks of 4 bytes: "food"
    // Cast to void* to match the generic memory pointer type expected by the macro.
    TEST_ASSERT_EQUAL_MEMORY((void*)"food", (void*)"food", 4);
}

/*
 * Test 3: Passes when comparing a local array against a string literal
 * Purpose: Ensure that memory equality works across different address sources
 *          (stack-allocated array vs literal).
 */
void testNotEqualMemory3_PassesWhenComparingLocalArrayAgainstLiteral(void)
{
    char localBuf[4] = { 'f', 'o', 'o', 'd' }; // 4-byte memory region
    // Compare local array against a literal; both have the same 4 bytes.
    TEST_ASSERT_EQUAL_MEMORY((void*)localBuf, (void*)"food", 4);
}

/*
 * Test Runner
 * Note: This harness uses Unity's RUN_TEST macro to execute each test.
 * If your project uses a different invocation pattern, adapt accordingly.
 */

int main(void)
{
    UnityBegin("testNotEqualMemory3-suite");

    // Register tests
    RUN_TEST(testNotEqualMemory3_FailsWhenFirstArgNull);
    RUN_TEST(testNotEqualMemory3_PassesWhenMemoryIsIdenticalNonNull);
    RUN_TEST(testNotEqualMemory3_PassesWhenComparingLocalArrayAgainstLiteral);

    return UnityEnd();
}