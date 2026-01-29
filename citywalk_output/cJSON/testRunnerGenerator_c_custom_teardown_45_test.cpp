/*
Step 1: Candidate Keywords (core dependencies inferred from the focal method)
- CounterTeardown: a global counter mutated by custom_teardown
- custom_teardown: focal method under test (sets CounterTeardown to 2)
- External linkage: custom_teardown is defined in a C translation unit (testRunnerGenerator.c)
- extern CounterTeardown: the test harness must access the same global
Notes:
- The focal method contains a single assignment; there are no branches to cover.
- This test suite focuses on ensuring custom_teardown reliably sets CounterTeardown to 2 regardless of its prior value.
*/

/*
Domain knowledge considerations applied:
- Use C++11 compatible code without GTest (no dependencies on GoogleTest).
- Access C symbols from C++ using extern "C" declarations.
- Use non-terminating assertions by printing pass/fail statuses rather than aborting.
- Keep tests lightweight and self-contained; do not rely on private members or internal state beyond CounterTeardown.
- Provide explanatory comments for each test.
*/

#include <unity.h>
#include <iostream>
#include <Defs.h>
#include <CException.h>
#include <string>
#include <stdio.h>


// Step 2/3: Declare C linkage for the focal function and its dependent global.
extern "C" {
    void custom_teardown(void);     // Focal method under test
    extern int CounterTeardown;      // Global counter mutated by the focal method
}

// Simple, non-terminating assertion helper
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void expect_eq(const char* test_name, int got, int expected) {
    ++g_total_tests;
    if (got == expected) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        std::cerr << "[FAIL] " << test_name << ": expected " << expected
                  << ", got " << got << std::endl;
        ++g_failed_tests;
    }
}

/*
Test 1: Ensure custom_teardown sets CounterTeardown to 2 when starting from 0.
Rationale: Validates the primary behavior of the focal method.
*/
static void test_custom_teardown_sets_2_from_zero(void) {
    CounterTeardown = 0;   // Precondition
    custom_teardown();     // Exercise
    expect_eq("custom_teardown_sets_2_from_zero", CounterTeardown, 2);
}

/*
Test 2: Ensure custom_teardown sets CounterTeardown to 2 when starting from a non-zero value.
Rationale: The method should always set the value to 2 regardless of previous state.
*/
static void test_custom_teardown_sets_2_from_nonzero(void) {
    CounterTeardown = 7;   // Precondition
    custom_teardown();     // Exercise
    expect_eq("custom_teardown_sets_2_from_nonzero", CounterTeardown, 2);
}

/*
Test 3: Idempotence test across multiple invocations with varying initial states.
Rationale: Verify repeated calls keep the counter at the expected value (stable result).
*/
static void test_custom_teardown_idempotence_multiple_calls(void) {
    CounterTeardown = 3;
    custom_teardown();
    expect_eq("custom_teardown_idempotence_first_call", CounterTeardown, 2);

    CounterTeardown = 9;
    custom_teardown();
    expect_eq("custom_teardown_idempotence_second_call", CounterTeardown, 2);
}

int main(void) {
    // Run tests
    test_custom_teardown_sets_2_from_zero();
    test_custom_teardown_sets_2_from_nonzero();
    test_custom_teardown_idempotence_multiple_calls();

    // Summary
    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}