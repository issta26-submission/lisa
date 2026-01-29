/*******************************************************
 * Test Suite: testNotEqualStringEachEqual1 - Unity C
 * Purpose: Validate behavior around TEST_ASSERT_EACH_EQUAL_STRING
 *          when one element in the array differs from the expected
 *          string (i.e., the focal method testNotEqualStringEachEqual1).
 *
 * Notes:
 * - The focal test relies on Unity's abort-wrapping macros
 *   EXPECT_ABORT_BEGIN / VERIFY_FAILS_END to verify a failure.
 * - This suite mirrors that behavior, providing:
 *     - A failing scenario for testNotEqualStringEachEqual1
 *     - A passing scenario to ensure the macro behaves correctly on success
 * - This file is designed to compile with C++11 as required, using
 *   Unity runtime (no GoogleTest).
 *
 * Candidate Keywords (Step 1 insights):
 * - testStrings array of const char*
 * - TEST_ASSERT_EACH_EQUAL_STRING macro
 * - Expected vs Actual string comparison
 * - 4-element array length (testStrings, 4)
 * - "foo" as the expected string
 * - Failure path indication via Unity abort behavior
 * - Macros: EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
 * - Helper stubs: startPutcharSpy, endPutcharSpy, getBufferPutcharSpy
 * - Static tearDown/flag hooks to simulate Unity test flow
 *******************************************************/

#include <stddef.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Forward declarations required by the EXPECT_ABORT macros used in the focal test
void startPutcharSpy(void);
void endPutcharSpy(void);
char* getBufferPutcharSpy(void);

// Minimal stubs to satisfy macro expectations (no-ops)
void startPutcharSpy(void) { /* no-op spy for putchar */ }
void endPutcharSpy(void)   { /* no-op spy for putchar */ }
char* getBufferPutcharSpy(void) { return NULL; }

// Static flags used by the tearDown/abort handling in the original suite
static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;

// Typical Unity setup/teardown hooks
void setUp(void)
{
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}
void tearDown(void)
{
    // Minimal tearDown to avoid interfering with abort-wrapped tests
    (void)SetToOneToFailInTearDown;
    (void)SetToOneMeanWeAlreadyCheckedThisGuy;
}

// Test 1 (Focal): Expected to Abort because one element differs ("moo" != "foo")
void testNotEqualStringEachEqual1(void)
{
    // Core scenario from the focal method:
    // testStrings = { "foo", "foo", "foo", "moo" }
    // Expect TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4) to FAIL
    const char *testStrings[] = { "foo", "foo", "foo", "moo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

// Test 2 (Non-focal): All strings equal; should PASS without abort
void testNotEqualStringEachEqual1_Pass(void)
{
    // All entries match "foo" -> should pass
    const char *testStrings[] = { "foo", "foo", "foo", "foo" };
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
}

// Note: Additional safety test to ensure behavior when one element differs early (index 1)
void testNotEqualStringEachEqual1_DifferentAtIndex1(void)
{
    const char *testStrings[] = { "foo", "bar", "foo", "foo" };
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_STRING("foo", testStrings, 4);
    VERIFY_FAILS_END
}

// Main runner to execute the tests
int main(void)
{
    UnityBegin("test_not_equal_string_each_equal1_suite.c");

    // Failing scenario (must abort and be reported as a failure for this test)
    RUN_TEST(testNotEqualStringEachEqual1);

    // Passing scenario (should pass cleanly)
    RUN_TEST(testNotEqualStringEachEqual1_Pass);

    // Additional failing scenario with a different index
    RUN_TEST(testNotEqualStringEachEqual1_DifferentAtIndex1);

    return UnityEnd();
}