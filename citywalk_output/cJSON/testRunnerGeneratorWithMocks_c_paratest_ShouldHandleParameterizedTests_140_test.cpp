#include <unity.h>
#include <mockMock.h>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// The focal method under test, declared with C linkage to match the original TU.
void paratest_ShouldHandleParameterizedTests(int Num);
#ifdef __cplusplus
}
#endif

// Avoid redefining setUp/tearDown if USE_CEXCEPTION is defined in the target TU.
// This prevents linkage conflicts across translation units.
#ifndef USE_CEXCEPTION
void setUp(void) { }
void tearDown(void) { }
#endif

/*
  Test 1: Validate the true branch of the modulo predicate
  - Purpose: Ensure that the function behaves correctly when Num is divisible by 5.
  - Coverage: Covers the true case of (Num % 5 == 0) which should leads to a passing assertion.
*/
static void test_paratest_ShouldHandleParameterizedTests_AllMultiplesOfFive(void)
{
    // True branch scenarios
    paratest_ShouldHandleParameterizedTests(0);   // 0 % 5 == 0
    paratest_ShouldHandleParameterizedTests(5);   // 5 % 5 == 0
    paratest_ShouldHandleParameterizedTests(10);  // 10 % 5 == 0
}

/*
  Test 2: Validate the false branch of the modulo predicate
  - Purpose: Ensure that the function executes the false branch when Num is not divisible by 5.
  - Coverage: Exercise numbers not divisible by 5 to exercise the failure path of the assertion.
  - Note: Unity assertions are non-fatal in this project configuration, allowing subsequent calls to run.
*/
static void test_paratest_ShouldHandleParameterizedTests_NonMultiplesOfFive(void)
{
    // False branch scenarios (expected to fail the assertion inside the focal method)
    paratest_ShouldHandleParameterizedTests(1);
    paratest_ShouldHandleParameterizedTests(2);
    paratest_ShouldHandleParameterizedTests(3);
    paratest_ShouldHandleParameterizedTests(4);
    paratest_ShouldHandleParameterizedTests(-1);   // negative non-multiple
    paratest_ShouldHandleParameterizedTests(-6);   // -6 % 5 == -1 (non-zero remainder)
}

/*
  Optional: Mixed coverage to ensure both branches are exercised within a single test.
  This is not strictly necessary since the two tests above cover true/false branches,
  but including this helps ensure a broader range of inputs are tested in one pass.
*/
static void test_paratest_ShouldHandleParameterizedTests_MixedInputs(void)
{
    paratest_ShouldHandleParameterizedTests(0);   // true
    paratest_ShouldHandleParameterizedTests(7);   // false
    paratest_ShouldHandleParameterizedTests(15);  // true
    paratest_ShouldHandleParameterizedTests(-2);  // false
}

int main(void)
{
    UNITY_BEGIN();
    // Run tests that exercise the focal method under different inputs
    RUN_TEST(test_paratest_ShouldHandleParameterizedTests_AllMultiplesOfFive);
    RUN_TEST(test_paratest_ShouldHandleParameterizedTests_NonMultiplesOfFive);
    RUN_TEST(test_paratest_ShouldHandleParameterizedTests_MixedInputs);
    return UNITY_END();
}