// Test suite: Focused coverage for the focal method testEqualPtrEachEqual
// This suite is designed to be compiled with a C11-compatible Unity framework
// (no GTest). It exercises both the success path and failure paths of
// TEST_ASSERT_EACH_EQUAL_PTR by supplying various pointer-arrays and
// edge-case scenarios.
//
// Step 1: Program Understanding highlights (conceptual keywords)
// - Candidate Keywords: A, p0, p1, p2, p3, &A, TEST_ASSERT_EACH_EQUAL_PTR, char*,",
 //   pointers, pointer-array comparisons, length parameter, pass cases, fail cases,
//   EXPECT_ABORT_BEGIN, VERIFY_FAILS_END
//
// Step 2: Unit Test Generation
// - Added tests to cover:
//   - Passing scenarios identical to the focal method (positive path)
//   - A single mismatch within the array (should abort) (negative path)
//   - Null actual pointer array (should abort)
//   - Null expected pointer (should abort)

#include <cstddef>
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

// Forward declarations of tests
void testEqualPtrEachEqual_PassCases(void);
void testEqualPtrEachEqual_FailsOnLastMismatch(void);
void testEqualPtrEachEqual_NullActual(void);
void testEqualPtrEachEqual_NullExpected(void);

// Main to run the focused tests
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(testEqualPtrEachEqual_PassCases);
    RUN_TEST(testEqualPtrEachEqual_FailsOnLastMismatch);
    RUN_TEST(testEqualPtrEachEqual_NullActual);
    RUN_TEST(testEqualPtrEachEqual_NullExpected);

    return UNITY_END();
}

// Test 1: Reproduce the exact passing scenarios from the focal method.
// This validates that TEST_ASSERT_EACH_EQUAL_PTR behaves correctly when all
// elements in the actual array match the expected pointer.
void testEqualPtrEachEqual_PassCases(void)
{
    // Candidate keywords and setup: A, B, C are distinct chars; p0..p3 arrays
    // are constructed to mirror the focal test data.
    char A = 1;
    char B = 2;
    char C = 3;

    char* p0[] = {&A, &A, &A};        // all elements point to A
    char* p1[] = {&A, &B, &C, &A};    // first and last are A; middle are B and C
    char* p2[] = {&B, &B};            // all elements point to B
    char* p3[] = {&C};                 // single-element array pointing to C

    // All of these should pass: each array is checked against the given expected pointer
    // The length parameters match the array lengths used
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 1);
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p0, 3);

    TEST_ASSERT_EACH_EQUAL_PTR(&A, p1, 1);
    TEST_ASSERT_EACH_EQUAL_PTR(&B, p2, 2);
    TEST_ASSERT_EACH_EQUAL_PTR(&C, p3, 1);
}

// Test 2: Failure path where a mismatch exists within the array.
// The macro should abort, which we catch with EXPECT_ABORT_BEGIN / VERIFY_FAILS_END.
void testEqualPtrEachEqual_FailsOnLastMismatch(void)
{
    char A = 1;
    char X = 2;

    // Last element is a different pointer value, causing a mismatch
    char* p_bad[] = {&A, &A, &A, &X};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p_bad, 4);
    VERIFY_FAILS_END
}

// Test 3: Failure path - NULL actual pointer (p is NULL).
// The assertion compares each element of a NULL array, which should fail.
void testEqualPtrEachEqual_NullActual(void)
{
    char A = 1;
    char* arr[] = {&A, &A};

    char** p_null = NULL;

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(&A, p_null, 2);
    VERIFY_FAILS_END
}

// Test 4: Failure path - NULL expected pointer.
// This should fail since no non-NULL actual pointers would equal NULL.
void testEqualPtrEachEqual_NullExpected(void)
{
    char A = 1;
    char* arr[] = {&A, &A};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_PTR(NULL, arr, 2);
    VERIFY_FAILS_END
}