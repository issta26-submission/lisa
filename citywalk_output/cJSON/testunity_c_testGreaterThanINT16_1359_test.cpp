/*
 * Test Suite: Focused validation for testGreaterThanINT16 (focal method)
 * 
 * Approach:
 * - Expose and invoke the focal test function testGreaterThanINT16 (defined in testunity.c)
 * - Also add additional false-path tests to exercise negative branches of the predicate
 * - Use Unity framework (no GTest) with C linkage to integrate into a C++11 test executable
 * - Compile under C++11 (g++/clang++) with Unity's sources linked
 * 
 * Notes on Candidate Keywords (Step 1):
 * - UNITY_INT16, TEST_ASSERT_GREATER_THAN_INT16
 * - Pointers: v0, v1, p0, p1
 * - Edge values: -32768, 32767
 * - Macros: EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
 * - Static/extern C linkage for cross-translation-unit calls
 * 
 * This suite ensures:
 * - True-branch coverage by executing the focal test as-is
 * - False-branch coverage by dedicated tests that expect assertion failures
 * - Access to static helpers (startPutcharSpy/endPutcharSpy) via extern "C" declarations
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test (defined in testunity.c)
void testGreaterThanINT16(void);

// Unity I/O helpers (needed by EXPECT_ABORT macros)
void startPutcharSpy(void);
void endPutcharSpy(void);
char* getBufferPutcharSpy(void);
#ifdef __cplusplus
}
#endif

// Prototypes for wrapper test functions (C linkage for RUN_TEST)
#ifdef __cplusplus
extern "C" {
#endif
// True-path wrapper: simply invokes the focal test
void testGreaterThanINT16_trueWrapper(void);

// False-path wrappers: exercise negative outcomes of the predicate
void testGreaterThanINT16_falseWrapper1(void);
void testGreaterThanINT16_falseWrapper2(void);
void testGreaterThanINT16_falseWrapper3(void);
void testGreaterThanINT16_falseWrapper4(void);
#ifdef __cplusplus
}
#endif

// Implementation of wrappers with C linkage to be consumed by RUN_TEST
#ifdef __cplusplus
extern "C" {
#endif

void testGreaterThanINT16_trueWrapper(void)
{
    // Execute the focal test which contains four assertions
    testGreaterThanINT16();
}

void testGreaterThanINT16_falseWrapper1(void)
{
    // Case: a > b should be false; expect abort on assertion
    // a = -1, b = 0
    UNITY_INT16 a = -1;
    UNITY_INT16 b = 0;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT16(a, b);
    VERIFY_FAILS_END
}

void testGreaterThanINT16_falseWrapper2(void)
{
    // Case: *pa > b should be false
    UNITY_INT16 a = -5;
    UNITY_INT16 b = 1;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT16(*pa, b);
    VERIFY_FAILS_END
}

void testGreaterThanINT16_falseWrapper3(void)
{
    // Case: a > *pb should be false
    UNITY_INT16 a = -32768;
    UNITY_INT16 b = -32767;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT16(a, *pb);
    VERIFY_FAILS_END
}

void testGreaterThanINT16_falseWrapper4(void)
{
    // Case: *pa > *pb should be false
    UNITY_INT16 a = -3000;
    UNITY_INT16 b = -2999;
    UNITY_INT16 *pa = &a;
    UNITY_INT16 *pb = &b;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_GREATER_THAN_INT16(*pa, *pb);
    VERIFY_FAILS_END
}

#ifdef __cplusplus
}
#endif

int main(void)
{
    // Initialize Unity test framework
    UNITY_BEGIN();

    // Run focal test (true path)
    RUN_TEST(testGreaterThanINT16_trueWrapper);

    // Run negative-path tests to cover false branches of the predicate
    RUN_TEST(testGreaterThanINT16_falseWrapper1);
    RUN_TEST(testGreaterThanINT16_falseWrapper2);
    RUN_TEST(testGreaterThanINT16_falseWrapper3);
    RUN_TEST(testGreaterThanINT16_falseWrapper4);

    // Finish Unity test run
    return UNITY_END();
}