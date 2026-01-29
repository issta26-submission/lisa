/***************************************************************
 * Extended Test Suite for testNotEqualStringArray2
 * 
 * This C++11 compatible test file adds additional coverage for
 * the Unity-based string-array equality assertion used in
 * testNotEqualStringArray2. It exercises various mismatch
 * scenarios and null-pointer edge cases to maximize path
 * coverage without modifying the original testunity.c file.
 *
 * Notes:
 * - The tests rely on Unity's EXPECT_ABORT_BEGIN / VERIFY_FAILS_END
 *   to capture expected assertion failures.
 * - Minimal stubs for putchar spying are provided to satisfy
 *   the EXPECT_ABORT_BEGIN macro requirements when linking with
 *   Unity's runtime.
 * - This file is intended to be compiled and linked with unity.c
 *   to provide the Unity framework (make sure to include both in
 *   the build).
 *
 * Domain considerations:
 * - Use non-terminating assertions appropriately, but for the
 *   NOT_EQUAL scenarios we intentionally wrap in EXPECT_ABORT_BEGIN
 *   to verify failure paths.
 * - Arrays are of length 4 to mirror the original focal method.
 * - All tests are self-contained and do not rely on private/static
 *   members of the focal code.
 ***************************************************************/

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

#ifdef __cplusplus
}
#endif

// Forward declarations for Unity's abort spy utilities (provided in the class dep).
// Implement minimal stubs to satisfy EXPECT_ABORT_BEGIN / VERIFY_FAILS_END.
void startPutcharSpy(void);
void endPutcharSpy(void);
char* getBufferPutcharSpy(void);

static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;

// Empty setUp/tearDown to keep Unity happy between tests.
void setUp(void) { (void)SetToOneToFailInTearDown; (void)SetToOneMeanWeAlreadyCheckedThisGuy; }
void tearDown(void) { /* no teardown actions required for these tests */ }

// Stub implementations for the stdout spy used by Unity's failure macros
void startPutcharSpy(void) { /* no-op: placeholder for test harness */ }
void endPutcharSpy(void) { /* no-op: placeholder for test harness */ }
char* getBufferPutcharSpy(void) { return nullptr; }

// Test 1: First element mismatch should abort as in original testNotEqualStringArray2
// This validates that a mismatch at index 0 is correctly detected.
static void testNotEqualStringArray2_FirstMismatch_Extended(void)
{
    // Arrangements: actual vs expected differ at index 0 ("zoo" != "foo")
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

// Test 2: Mismatch at index 1 (middle element)
static void testNotEqualStringArray2_MismatchAtIndex1_Extended(void)
{
    // Actual: "zoo","boo","woo","moo"
    // Expected: "zoo","bar","woo","moo" -> mismatch at index 1
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "zoo", "bar", "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

// Test 3: Mismatch at index 2 (second half)
static void testNotEqualStringArray2_MismatchAtIndex2_Extended(void)
{
    // Actual: "zoo","boo","woo","moo"
    // Expected: "zoo","boo","bar","moo" -> mismatch at index 2
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "zoo", "boo", "bar", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

// Test 4: Mismatch at index 3 (last element)
static void testNotEqualStringArray2_MismatchAtIndex3_Extended(void)
{
    // Actual: "zoo","boo","woo","moo"
    // Expected: "zoo","boo","woo","bar" -> mismatch at index 3
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "zoo", "boo", "woo", "bar" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4);
    VERIFY_FAILS_END
}

// Test 5: Null actual array should abort when length > 0
static void testNotEqualStringArray2_NullActualArray_Extended(void)
{
    const char **testStringsPtr = NULL;
    const char *expStrings[] = { "foo", "boo", "woo", "moo" };

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStringsPtr, 4);
    VERIFY_FAILS_END
}

// Test 6: Null expected array should abort when length > 0
static void testNotEqualStringArray2_NullExpectedArray_Extended(void)
{
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char **expStringsPtr = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_ARRAY(expStringsPtr, testStrings, 4);
    VERIFY_FAILS_END
}

// Test runner
int main(void)
{
    UNITY_BEGIN("testNotEqualStringArray2_Extended.cpp");

    // Run extended tests
    RUN_TEST(testNotEqualStringArray2_FirstMismatch_Extended);
    RUN_TEST(testNotEqualStringArray2_MismatchAtIndex1_Extended);
    RUN_TEST(testNotEqualStringArray2_MismatchAtIndex2_Extended);
    RUN_TEST(testNotEqualStringArray2_MismatchAtIndex3_Extended);
    RUN_TEST(testNotEqualStringArray2_NullActualArray_Extended);
    RUN_TEST(testNotEqualStringArray2_NullExpectedArray_Extended);

    return UNITY_END();
}