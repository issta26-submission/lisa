// Test suite: Focused tests for the focal method testNotEqualStringLen1
// Context: The focal method exercises Unity's TEST_ASSERT_EQUAL_STRING_LEN macro
//          which compares the first 'len' characters of two C-strings.
//          The focal test expects an abort when the strings differ within the
//          provided length. This suite adds additional scenarios to improve
//          coverage of both aborting (false predicate) and passing (true predicate)
//          paths, including edge cases with NULL pointers and equality within the len.

#include <stddef.h>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Step A: Bring in Unity framework (C API) with C linkage for compatibility with C++11
#ifdef __cplusplus
extern "C" {
#endif
// Prototypes from the focal class dependency (Unity helpers used by the macros)
void startPutcharSpy(void);
void endPutcharSpy(void);
char* getBufferPutcharSpy(void);
#ifdef __cplusplus
}
#endif

// Step B: Provide minimal spy implementations to satisfy linker when not linked with testunity.c
#ifdef __cplusplus
extern "C" {
#endif
void startPutcharSpy(void) { /* no-op spy */ }
void endPutcharSpy(void)   { /* no-op spy */ }
char* getBufferPutcharSpy(void) { return nullptr; }
#ifdef __cplusplus
}
#endif

// SetUp/TearDown for Unity tests (no per-test state needed here)
void setUp(void)  { }
void tearDown(void) { }

// Test 1: Fails path - repro the focal behavior: two strings differ within the given length (6)
static void test_NotEqualStringLen1_FailsOnDifferentStrings(void)
{
    // This test mirrors the focal behavior: abort expected when first 6 chars differ
    // Expected: abort happens, test passes as a negative case
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foobar", "foobaz", 6);
    VERIFY_FAILS_END
}

// Test 2: Pass path - identical strings within the given length should not abort
static void test_NotEqualStringLen1_PassOnIdenticalStrings(void)
{
    // No abort is expected; the two strings are exactly equal for len = 6
    TEST_ASSERT_EQUAL_STRING_LEN("foobar", "foobar", 6);
}

// Test 3: Pass path - strings differ only after the provided length; should pass
static void test_NotEqualStringLen1_PassWhenDifferenceOutsideLen(void)
{
    // Different after 5th char; within len=5 they are identical.
    TEST_ASSERT_EQUAL_STRING_LEN("foobar", "foobaz", 5);
}

// Test 4: Abort path - null as expected string should fail (ExpectedStringIsNull)
static void test_NotEqualStringLen1_ExpectedNull_Fails(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN(NULL, "bar", 6);
    VERIFY_FAILS_END
}

// Test 5: Abort path - null as actual string should fail (ActualStringIsNull)
static void test_NotEqualStringLen1_ActualNull_Fails(void)
{
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EQUAL_STRING_LEN("foo", NULL, 3);
    VERIFY_FAILS_END
}

// Test 6: Additional edge - both strings NULL with zero length should abort or pass appropriately
static void test_NotEqualStringLen1_BothNullWithZeroLen(void)
{
    // If both are NULL and len is 0, the comparison may be considered equal; expect pass (no abort)
    TEST_ASSERT_EQUAL_STRING_LEN(NULL, NULL, 0);
}

// Test 7: Another edge - both strings equal within a larger len (sanity check)
static void test_NotEqualStringLen1_EqualWithinLargerLen(void)
{
    // Equal strings within a larger len should pass (no abort)
    TEST_ASSERT_EQUAL_STRING_LEN("abc", "abc", 5); // Compare first 5 chars; strings shorter but equal in covered portion
}

// Test execution entry point
int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Register tests
    RUN_TEST(test_NotEqualStringLen1_FailsOnDifferentStrings, __LINE__);
    RUN_TEST(test_NotEqualStringLen1_PassOnIdenticalStrings, __LINE__);
    RUN_TEST(test_NotEqualStringLen1_PassWhenDifferenceOutsideLen, __LINE__);
    RUN_TEST(test_NotEqualStringLen1_ExpectedNull_Fails, __LINE__);
    RUN_TEST(test_NotEqualStringLen1_ActualNull_Fails, __LINE__);
    RUN_TEST(test_NotEqualStringLen1_BothNullWithZeroLen, __LINE__);
    RUN_TEST(test_NotEqualStringLen1_EqualWithinLargerLen, __LINE__);

    // End Unity tests
    return UNITY_END();
}