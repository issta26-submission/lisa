// Unit test suite for the focal function: FindFunction_WhichIsBroken
// This test suite is crafted for C++11 compilation without Google Test.
// It relies on the external global array NumbersToFind that is typically defined
// in ProductionCode.c and accessible here via extern int NumbersToFind[].
// Each test is annotated with comments explaining its purpose and approach.

#include <algorithm>
#include <ProductionCode.h>
#include <cstdio>


// Extern declaration of the dependency array to allow test to manipulate its values.
// Assumes NumbersToFind is a non-static global array defined in the production code.
extern int NumbersToFind[];

//
// Simple lightweight test harness (non-terminating assertions).
//
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT_EQ_INT(EXPECTED, ACTUAL) do { \
    ++g_tests_run; \
    if ((EXPECTED) == (ACTUAL)) { \
        ++g_tests_passed; \
    } else { \
        ++g_tests_failed; \
        std::fprintf(stderr, "Test failed at %s:%d - expected %d, got %d\n", __FILE__, __LINE__, (EXPECTED), (ACTUAL)); \
    } \
} while(0)

//
// Helper to print a summary at the end of tests
//
static void print_summary() {
    std::printf("Tests run: %d, Passed: %d, Failed: %d\n",
                g_tests_run, g_tests_passed, g_tests_failed);
}

//
// Test 1: Verify that when NumbersToFind[9] equals the target number,
// the function returns 9 (demonstrating the post-loop compare path).
// This checks the true-path of the final if statement after the loop.
// This also exercises the behavior caused by the mis-braced while loop (the
// actual loop body is effectively just i++, and the if executes after the loop).
//
static bool test_returns_9_when_number_found_at_9() {
    // Prepare a 10-element array to safely address index 9.
    int local[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 42};
    for (int i = 0; i < 10; ++i) {
        NumbersToFind[i] = local[i];
    }

    // NumberToFind is chosen to match NumbersToFind[9]
    int target = 42;
    int result = FindFunction_WhichIsBroken(target);

    // Expect the result to be 9 due to the post-loop if comparison
    TEST_ASSERT_EQ_INT(9, result);
    return (9 == result);
}

//
// Test 2: Verify that when NumbersToFind[9] does not equal the target number,
// the function returns 0 (false path of the final if statement).
//
static bool test_returns_0_when_number_not_found_at_9() {
    // Prepare a 10-element array where the last element does not equal the target
    int local[10] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 99};
    for (int i = 0; i < 10; ++i) {
        NumbersToFind[i] = local[i];
    }

    int target = 42;
    int result = FindFunction_WhichIsBroken(target);

    // Expect the result to be 0 since NumbersToFind[9] != target
    TEST_ASSERT_EQ_INT(0, result);
    return (result == 0);
}

//
// Additional exploratory test (optional sanity check):
// Test with a different target that matches NumbersToFind[9] as well.
// This reinforces that the post-loop if logic is used regardless of the
// actual number passed, since the check is performed after the loop.
// This is effectively similar to Test 1 but with a different number.
//
static bool test_returns_9_for_another_matching_value() {
    int local[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, -7}; // 9th index initialized to -7
    for (int i = 0; i < 10; ++i) {
        NumbersToFind[i] = local[i];
    }

    int target = -7;
    int result = FindFunction_WhichIsBroken(target);

    TEST_ASSERT_EQ_INT(9, result);
    return (result == 9);
}

int main() {
    // Run tests
    bool t1 = test_returns_9_when_number_found_at_9();
    bool t2 = test_returns_0_when_number_not_found_at_9();
    bool t3 = test_returns_9_for_another_matching_value();

    // Optionally print a summary for quick feedback
    print_summary();

    // Return non-zero if any test failed (helps CI detect failures)
    if (g_tests_failed > 0) {
        return g_tests_failed;
    }
    // Normal exit
    return 0;
}