// testparameterized_suite.cpp
// This test suite exercises the focal method:
//   void test_TheseShouldAllFail(int Num)
// The test uses Unity's parameterized style by wrapping the C function
// and running it with different Num values from main.
//
// Key ideas implemented:
// - Parameterized wrapper to call the focal method with different Num.
// - Unity test runner invoked from main (via RUN_TEST) to capture
//   expected aborts and branch coverage.
// - Values chosen to cover both sides of the predicate Num > 100 (i.e., <=100 and >100).
//
// Note: This file is written for a C/C++ project using Unity (no GTest).

#include <setjmp.h>
#include <unity.h>
#include <cstdio>
#include <stdio.h>


extern "C" {
}

// Forward declaration of the focal method (from testparameterized.c)
void test_TheseShouldAllFail(int Num);

// Wrapper function to adapt the parameterized test to Unity's RUN_TEST
static int g_Num;                   // global parameter accessible to the wrapper

static void test_TheseShouldAllFail_Wrapper(void)
{
    // Call the focal method with the current parameter
    test_TheseShouldAllFail(g_Num);
}

// Additional helper to run a single test case with a given Num
static void RunWithParam(int Num)
{
    g_Num = Num;
    RUN_TEST(test_TheseShouldAllFail_Wrapper);
}

// Main function acts as the test harness (no separate GTest/GMock runner required)
int main(void)
{
    // Initialize Unity test framework
    UnityBegin("testparameterized.c");

    // Domain-informed test values:
    // - 50 and 100 cover the false/edge condition (Num > 100 is false)
    // - 150 and 101 cover the true condition (Num > 100 is true)
    // - Additional values included to ensure broader coverage
    int testValues[] = { 50, 100, 150, 101, -5, 0 };
    const int count = sizeof(testValues) / sizeof(testValues[0]);

    // Run each parameterized case
    for (int i = 0; i < count; ++i)
    {
        RunWithParam(testValues[i]);
        // Explanation:
        // For Num <= 100, TEST_ASSERT_TRUE(Num > 100) will fail inside the focal test
        // and Unity's EXPECT_ABORT_BEGIN/VERIFY_FAILS_END will handle the abort, letting
        // the test pass in terms of coverage of the abort path.
        // For Num > 100, the assertion would pass, the focal test would not abort,
        // and the test will fail (which is intentional here to exercise both branches
        // of the predicate and the abort handling in the test harness).
        // The overall suite captures both outcomes as part of coverage.
    }

    // Finalize Unity test run
    return UnityEnd();
}