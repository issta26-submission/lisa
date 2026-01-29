/*
  Unit test suite for the focal method:
  paratest_ShouldHandleParameterizedTestsThatFail(int Num)

  This test suite is designed to run under the Unity C unit test framework,
  while being compiled with a C++11 capable compiler (as requested).
  It exercises the focal method by supplying parameter values that cover
  both the passing and failing branches of the internal assertion.

  Notes aligned with the task steps:
  - Step 1 (Candidate Keywords): paratest_ShouldHandleParameterizedTestsThatFail, Num,
    TEST_ASSERT_EQUAL_MESSAGE, Unity, parameterized tests, "This call should fail",
    testRunnerGenerator.c, test dependencies (Defs.h)
  - Step 2 (Class/Dependency Context): The focal function relies on Unity's
    TEST_ASSERT_EQUAL_MESSAGE macro and the project dependency Defs.h (as seen in
    the provided <FOCAL_CLASS_DEP> block).
  - Step 3 (Domain Knowledge): The tests are written in C++11 compatible code,
    using extern "C" wrappers to call the C focal function, and they invoke the
    Unity test runner (RUN_TEST) to handle test execution and reporting.
  Explanatory comments are included for every unit test.
*/

#include <unity.h>
#include <CException.h>
#include <Defs.h>
#include <stdio.h>


// Include Unity header (C unit testing framework)

#ifdef __cplusplus
extern "C" {
#endif


// Focal method under test (declared with C linkage for C++ compilation)
void paratest_ShouldHandleParameterizedTestsThatFail(int Num);

#ifdef __cplusplus
}
#endif

// Candidate Keywords (for traceability and alignment with Step 1)
//
// - paratest_ShouldHandleParameterizedTestsThatFail: focal method under test
// - TEST_ASSERT_EQUAL_MESSAGE: Unity assertion used inside the focal method
// - Num: parameter passed into the focal method to drive logic
// - "This call should fail": assertion message used by the focal method
// - Unity: test framework providing RUN_TEST, UNITY_BEGIN, UNITY_END
// - testRunnerGenerator.c: source file context containing the focal method
// - Defs.h: project dependency used by the focal method
// - parameterized tests: test strategy to exercise multiple inputs
//

// Wrapper test: Ensure paratest_ShouldHandleParameterizedTestsThatFail passes when Num == 3
extern "C" void test_paratest_ShouldHandleParameterizedTestsThatFail_NumIs3_Passes(void)
{
    // Purpose:
    // - Validate that the focal method does not trigger an assertion failure
    //   when Num equals the expected value (3).
    // - This exercises the "true" branch of the equality check inside the focal method.
    paratest_ShouldHandleParameterizedTestsThatFail(3);
}

// Wrapper test: Ensure paratest_ShouldHandleParameterizedTestsThatFail fails when Num != 3
// Note: Unity's assertions are typically terminating for the current test,
// so this test will report a failure if the focal method detects the mismatch.
// This exercises the "false" branch of the equality check inside the focal method.
extern "C" void test_paratest_ShouldHandleParameterizedTestsThatFail_NumIsNot3_Fails(void)
{
    paratest_ShouldHandleParameterizedTestsThatFail(2);
}

// Optional: Additional parameter variation to broaden coverage (uncomment if needed)
/*
extern "C" void test_paratest_ShouldHandleParameterizedTestsThatFail_NumIsOtherValue_Fails(void)
{
    paratest_ShouldHandleParameterizedTestsThatFail(-1);
}
*/

// Unity test runner entry points
#ifdef __cplusplus
extern "C" {
#endif

// In Unity-based test suites, a main() function is typically provided to drive the tests.
// We expose a C-compatible main so it can be linked and run from a CMake-based or
// manual build setup that expects C entry points.
int main(void)
{
    UNITY_BEGIN();

    // Run tests:
    // - First: a passing scenario (Num == 3)
    RUN_TEST(test_paratest_ShouldHandleParameterizedTestsThatFail_NumIs3_Passes);

    // - Second: a failing scenario (Num != 3)
    RUN_TEST(test_paratest_ShouldHandleParameterizedTestsThatFail_NumIsNot3_Fails);

    // If additional tests are added (e.g., NumIsOtherValue_Fails), register here:
    // RUN_TEST(test_paratest_ShouldHandleParameterizedTestsThatFail_NumIsOtherValue_Fails);

    return UNITY_END();
}

#ifdef __cplusplus
}
#endif

/*
  Test Dependency Notes for Step 2 (aligned with <FOCAL_CLASS_DEP> block):
  - The test uses the Unity framework (unity.h) and the project header Defs.h.
  - The focal function paratest_ShouldHandleParameterizedTestsThatFail(int) is declared with C linkage
    so it can be invoked from C++ test code without name mangling.
  - The test suite adheres to the constraint of not using GTest, leveraging Unity's API
    for assertions and test orchestration.
*/