/*
 * Comprehensive Unity-based test suite for the focal method:
 *   test_TheseAreEveryOther(int Num)
 *
 * Context:
 * - The focal method is defined in testparameterized.c and uses Unity
 *   test macros (EXPECT_ABORT_BEGIN, TEST_ASSERT_TRUE, etc.).
 * - We cannot rely on GTest; thus we use Unity's framework to drive tests.
 * - This suite focuses on exercising the two main predicates within
 *   test_TheseAreEveryOther:
 *     1) Oddness (Num & 1) and the nested assertion (Num > 100)
 *     2) Even path where the predicate (Num < 100) must hold
 *
 * The tests are designed to cover:
 * - True and false branches of the inner predicates as far as possible
 *   given the structure of the focal function.
 * - Use of abort-catching blocks for cases that are expected to fail
 *   (i.e., trigger Unity's assertion aborts).
 * - Normal pass/fail scaffolding via the provided testNormal helpers.
 *
 * Build note:
 * - This file should be compiled and linked together with testparameterized.c
 *   and Unity's library (unity.h + Unity source) under a C++11 capable toolchain.
 * - The tests expose a main() that explicitly runs the test suite.
 *
 * Important: The testcases rely on the focal function's internal use of Unity
 *                 macros. We invoke the focal function from within test cases
 *                 that expect aborts where appropriate.
 */

// If compiling as C++, ensure C linkage for the focal function.
#ifdef __cplusplus
extern "C" {
#endif

// Prototype of the focal method under test (defined in testparameterized.c)
void test_TheseAreEveryOther(int Num);

#ifdef __cplusplus
}
#endif

// Include Unity header for macros and test harness
#include <setjmp.h>
#include <unity.h>
#include <stdio.h>


// Forward declarations of Unity-style test cases
void test_OddUnder100_ShouldAbort(void);
void test_EvenUnder100_ShouldPass(void);
void test_EvenGe100_ShouldAbort(void);
void test_NormalTestsMixin(void);

// Helper helper: utility to run a single test function with multiple inputs.
// Note: Each test case uses Unity's EXPECT_ABORT_BEGIN/END inside the focal
//       method for the respective abort scenarios. We wrap the focal call in
//       its own abort expectations to clearly capture intended aborts.
void test_RunWithAbortWrapper(void (*testFunc)(void));

// Implementation of test cases

// Test the odd-number branch for values <= 100. The focal method uses
// an abort-guard around TEST_ASSERT_TRUE(Num > 100). For odd Num <= 100,
// this assertion will fail (Num > 100 is false) and should trigger an abort.
// We wrap the focal call with Unity's abort-capture macros to ensure the
// expected abort occurs and the test passes.
void test_OddUnder100_ShouldAbort(void)
{
    // Odd values that should trigger the abort inside the nested assertion
    int oddUnder100[] = {1, 3, 5, 99};
    const int count = sizeof(oddUnder100) / sizeof(oddUnder100[0]);
    for (int i = 0; i < count; ++i)
    {
        // Expect an abort due to the inner assertion failing
        EXPECT_ABORT_BEGIN
        test_TheseAreEveryOther(oddUnder100[i]);
        VERIFY_FAILS_END
    }
}

// Test the even branch for values less than 100. The focal else path should
// simply assert TEST_ASSERT_TRUE(Num < 100), which should pass for these values.
void test_EvenUnder100_ShouldPass(void)
{
    int evenUnder100[] = {0, 2, 4, 98};
    const int count = sizeof(evenUnder100) / sizeof(evenUnder100[0]);
    for (int i = 0; i < count; ++i)
    {
        // This path should not abort; the assertion should pass.
        test_TheseAreEveryOther(evenUnder100[i]);
    }
}

// Test the even branch for values >= 100. For these, Num < 100 is false and would
// trigger an assertion failure (abort). We capture this via Unity's abort macro.
void test_EvenGe100_ShouldAbort(void)
{
    int evenGe100[] = {100, 102, 200, 250};
    const int count = sizeof(evenGe100) / sizeof(evenGe100[0]);
    for (int i = 0; i < count; ++i)
    {
        // Expect an abort due to the condition Num < 100 being false
        EXPECT_ABORT_BEGIN
        test_TheseAreEveryOther(evenGe100[i]);
        VERIFY_FAILS_END
    }
}

// Optional: include normal passing/failing behavior tests for overall coverage.
// These are placeholders to ensure the test suite exercises non-aborting paths.
void test_NormalTestsMixin(void)
{
    // If there are static or normal passes/failures in the library, ensure
    // they still pass under the test harness. These calls rely on the
    // existing functions defined in the focal test file.
    // For the provided skeleton, these are intentionally simple no-ops.
    // They serve as anchors for coverage and demonstration.
    // Example (if implemented in the focal file):
    // test_NormalPassesStillWork();
    // test_NormalFailsStillWork();
}

// Helper: Run a test function that expects aborts within. Not strictly required
// for Unity in this particular scenario since each test function is already
// written to drive multiple sub-cases, but provided for completeness.
void test_RunWithAbortWrapper(void (*testFunc)(void))
{
    testFunc();
}

// Unity setup and teardown (empty as per the original skeleton)
void setUp(void) { }
void tearDown(void) { }

// Main function to drive the tests without GTest.
// If Unity provides RUN_TEST macro usage, we can still compile with a standard
// Unity test runner embedded in main. This ensures compatibility with
// environments where tests are invoked from main explicitly.
int main(void)
{
    UNITY_BEGIN();

    // Register and run tests
    RUN_TEST(test_OddUnder100_ShouldAbort);
    RUN_TEST(test_EvenUnder100_ShouldPass);
    RUN_TEST(test_EvenGe100_ShouldAbort);
    RUN_TEST(test_NormalTestsMixin);

    // End Unity test session
    return UNITY_END();
}