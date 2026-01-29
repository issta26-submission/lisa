/*
Test suite for the focal method: custom_setup(void)

Step 1 recap (for reference in comments):
- Core action performed by custom_setup: CounterSetup = 2;
- Core dependent component: CounterSetup (likely a global variable defined in Defs.h/.c)
- No branches inside custom_setup; the test should verify the effect on CounterSetup.
- Relevant dependencies: Defs.h (exposes CounterSetup), Unity (test framework), and the function prototype for custom_setup.

Step 2 & 3 Implementation notes:
- We create a small Unity-based test suite in testRunnerGeneratorSmall.c that targets custom_setup.
- Tests cover: initial assignment to 2 and subsequent calls preserving the value (idempotency).
- We keep tests simple and iterable, suitable for C++11 compilation (as a C file tested under C/C++ build).
- We expose the function prototype with C linkage when compiled as C++.
- Tests are designed to be executable from a main function (Unity style) to avoid GoogleTest requirements and conform to the given domain knowledge.

Usage notes:
- Ensure the build links with the object file that defines custom_setup() and with Defs.h/.c that declare CounterSetup.
- If CounterSetup has internal/static linkage, this test assumes external visibility via Defs.h; otherwise adjust to match your project’s linkage.

Code (testRunnerGeneratorSmall.c):
*/
#include <unity.h>
#include <Defs.h>
#include <stdio.h>


/* Ensure C linkage when compiled under C++11 */
#ifdef __cplusplus
extern "C" {
#endif
void custom_setup(void);
#ifdef __cplusplus
}
#endif

/* Unity setUp/tearDown hooks (not used by these tests, but provided for Unity compatibility) */
void setUp(void)
{
    // Intentionally empty: setup handled within each test to isolate cases
}
void tearDown(void)
{
    // Intentionally empty
}

/*
Test 1: Custom setup should set CounterSetup to 2 from a known initial value.
- Rationale: Validates the primary side-effect of the focal method.
*/
void test_CustomSetup_SetsCounterToTwo(void)
{
    // Arrange: start from a known state
    CounterSetup = 0;

    // Act: invoke the focal function
    custom_setup();

    // Assert: CounterSetup is exactly 2
    TEST_ASSERT_EQUAL_INT(2, CounterSetup);
}

/*
Test 2: Custom setup should overwrite any non-2 value to 2 on subsequent calls.
- Rationale: Ensure the function is idempotent with respect to the value 2 (i.e., it always ends up as 2).
*/
void test_CustomSetup_SecondCallKeepsTwo(void)
{
    // Arrange: set to a non-2 value before first call
    CounterSetup = 7;

    // Act: first call
    custom_setup();

    // Assert after first call
    TEST_ASSERT_EQUAL_INT(2, CounterSetup);

    // Arrange for second call (simulate another setup cycle)
    CounterSetup = 9;

    // Act: second call
    custom_setup();

    // Assert after second call: should still be 2
    TEST_ASSERT_EQUAL_INT(2, CounterSetup);
}

/*
Optional: If you want to run tests in a single main (as requested by domain knowledge for non-GTest environments),
the following main demonstrates a Unity-based test harness invocation.
*/
int main(void)
{
    UNITY_BEGIN();
    // Run the test suite for custom_setup
    RUN_TEST(test_CustomSetup_SetsCounterToTwo);
    RUN_TEST(test_CustomSetup_SecondCallKeepsTwo);
    return UNITY_END();
}