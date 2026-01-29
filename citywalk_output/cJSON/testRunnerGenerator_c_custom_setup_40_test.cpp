/*
Step 1 - Program Understanding (Candidate Keywords)
- CounterSetup: a global state modified by custom_setup
- custom_setup(void): focal method under test; assigns CounterSetup = 2
- testRunnerGenerator.c: file where custom_setup is defined
- Dependencies: external/global CounterSetup, external C function custom_setup
- Static/file-scope considerations: CounterSetup is treated as a global state; access via external linkage
- Testing approach: verify that calling custom_setup reliably sets CounterSetup to 2 from various initial states

Step 2 - Unit Test Generation (C++11, no GTest)
- Implement a small, self-contained test harness in C++11 that calls the C function custom_setup()
- Access C symbol CounterSetup and function custom_setup via extern "C"
- Create multiple test cases to ensure true coverage of how custom_setup affects CounterSetup
- Since no GTest is used, tests are run from main() and results printed to stdout

Step 3 - Test Case Refinement (Domain Knowledge)
- Cover scenarios: initial values before custom_setup, repeated invocations (idempotence)
- Ensure tests are independent and do not rely on Unity/GTest
- Use straightforward assertions (return true/false) to accumulate results without terminating on first failure
- Provide explanatory comments for each unit test
*/

#include <unity.h>
#include <iostream>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


// Step 2: Provide C linkage for the focal function and global variable
extern "C" {
    // The focal method under test located in testRunnerGenerator.c
    void custom_setup(void);

    // The global state manipulated by custom_setup
    extern int CounterSetup;
}

// Lightweight test harness (non-terminating assertions) for C++11
static int tests_run = 0;
static int tests_failed = 0;

using TestFn = bool(*)();

// Helper to run a single test and report result
static void runTest(const char* testName, TestFn testFunc) {
    ++tests_run;
    bool ok = testFunc();
    if (ok) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        ++tests_failed;
        std::cout << "[FAIL] " << testName << std::endl;
    }
}

/*
Test 1: custom_setup_sets_counter_to_2
- Explanation: Starting from a known value (0), ensure that calling custom_setup() sets CounterSetup to 2.
*/
bool test_custom_setup_sets_counter_to_2() {
    CounterSetup = 0;
    custom_setup();
    return CounterSetup == 2;
}

/*
Test 2: custom_setup_sets_counter_to_2_from_various_initial_values
- Explanation: Ensure that starting from a variety of initial values, custom_setup() always results in CounterSetup == 2.
  This checks the function's robustness across plausible domain values.
*/
bool test_custom_setup_sets_counter_to_2_from_various_initial_values() {
    int initial_values[] = { -10, -1, 0, 1, 2, 5, 100, 9999 };
    const int n = sizeof(initial_values) / sizeof(initial_values[0]);
    for (int i = 0; i < n; ++i) {
        CounterSetup = initial_values[i];
        custom_setup();
        if (CounterSetup != 2) {
            return false;
        }
    }
    return true;
}

/*
Test 3: custom_setup_is_idempotent_over_multiple_calls
- Explanation: Repeatedly calling custom_setup() should not change the invariant CounterSetup == 2 after the first call.
  This confirms there are no unintended side effects when invoked multiple times.
*/
bool test_custom_setup_is_idempotent_over_multiple_calls() {
    int values[] = { -100, 0, 7, 42 };
    for (int i = 0; i < 4; ++i) {
        CounterSetup = values[i];
        // First call
        custom_setup();
        if (CounterSetup != 2) return false;
        // Second call (should remain 2)
        custom_setup();
        if (CounterSetup != 2) return false;
        // Third call (still 2)
        custom_setup();
        if (CounterSetup != 2) return false;
    }
    return true;
}

/*
Test 4: custom_setup_resets_from_random_sequence_to_2
- Explanation: Validate that dynamic sequences of calls yield 2 regardless of the starting value,
  simulating a more stress-like scenario with multiple resets.
*/
bool test_custom_setup_resets_from_random_sequence_to_2() {
    int sequence[] = { 15, -7, 0, 3, 999, -123 };
    const int n = sizeof(sequence) / sizeof(sequence[0]);
    for (int i = 0; i < n; ++i) {
        CounterSetup = sequence[i];
        custom_setup();
        if (CounterSetup != 2) return false;
        // Call again to ensure stability
        custom_setup();
        if (CounterSetup != 2) return false;
    }
    return true;
}

/*
Test 5: custom_setup_main_entry_point_presence
- Explanation: Simple consistency check to ensure the function linkage is intact by verifying
  the global CounterSetup is a plain int and can be read/written as expected after calling custom_setup.
  (This test is lightweight but ensures linkage correctness in environments where symbols must be exported.)
*/
bool test_custom_setup_linkage_and_type_consistency() {
    CounterSetup = -1;     // arbitrary non-2 value
    custom_setup();
    // Expect 2 after setup; if not, linkage/type issues are likely present
    return CounterSetup == 2;
}

int main() {
    std::cout << "Running custom_setup() test suite (C++ harness calling C function)" << std::endl;

    // Step 2: Run tests
    runTest("custom_setup_sets_counter_to_2", test_custom_setup_sets_counter_to_2);
    runTest("custom_setup_sets_counter_to_2_from_various_initial_values", test_custom_setup_sets_counter_to_2_from_various_initial_values);
    runTest("custom_setup_is_idempotent_over_multiple_calls", test_custom_setup_is_idempotent_over_multiple_calls);
    runTest("custom_setup_resets_from_random_sequence_to_2", test_custom_setup_resets_from_random_sequence_to_2);
    runTest("custom_setup_linkage_and_type_consistency", test_custom_setup_linkage_and_type_consistency);

    std::cout << "Test results: " << tests_run << " run, " << tests_failed << " failed." << std::endl;
    return (tests_failed == 0) ? 0 : 1;
}