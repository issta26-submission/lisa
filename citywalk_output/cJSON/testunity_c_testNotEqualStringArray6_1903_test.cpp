/*
  This is a lightweight C++11 test harness that focuses on validating the focal behavior
  of the Unity-style testNotEqualStringArray6 scenario from testunity.c without pulling
  in the GTest framework. It provides a minimal, non-terminating assertion style to
  exercise the core dependency: comparing two string-pointer arrays for equality.

  Step 1 (Program Understanding summary): The focal test asserts that TEST_ASSERT_EQUAL_STRING_ARRAY
  fails (aborts) when the expected array (expStrings) is NULL while the actual testStrings is non-NULL.
  Core dependent components (Candidate Keywords) include:
  - testStrings: array of const char* containing the actual strings
  - expStrings: expected array pointer (const char**), intentionally NULL in the focal test
  - TEST_ASSERT_EQUAL_STRING_ARRAY: equality check between two string-pointer arrays
  - The length parameter (4 in the focal test)
  - ABORT-like behavior: we emulate non-terminating tests by returning a boolean status instead of exiting

  Step 2 (Test Suite): We implement three tests that mirror the critical aspects of the focal case and two common edge cases
  using a small manual comparison function:
  - NotEqualStringArray6_NullExpected_ShouldFail: expected == NULL, actual non-NULL -> should fail
  - NotEqualStringArray6_NullActual_ShouldFail: actual == NULL, expected non-NULL -> should fail
  - NotEqualStringArray6_Different_ShouldFail: both non-NULL but an element differs -> should fail

  Step 3 (Refinement): The tests use plain C++11 facilities from the standard library and
  provide explanatory comments for each unit test. They avoid terminating the test process
  and instead return a boolean indicating pass/fail for each scenario.
*/

#include <unity.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


/*
  Candidate Keywords (for reference during test generation)
  - testStrings, expStrings
  - TEST_ASSERT_EQUAL_STRING_ARRAY semantics (equality check for arrays of C-strings)
  - NULL vs non-NULL pointer handling
  - array length parameter (4 in focal test)
  - non-terminating assertion style (returning status instead of aborting)
  - comparison utilities for C-strings
*/

// Lightweight comparator that mimics Unity's string-array equality checks.
// Returns true if arrays are element-wise equal for all positions [0..len-1], false otherwise.
// Behavior mirrors Unity's strict inequality when any element is NULL while the other is non-NULL,
// or when any corresponding strings differ.
static bool compareStringArray(const char** expected, const char** actual, size_t len)
{
    if (expected == nullptr || actual == nullptr)
        return false; // cannot compare when either side is NULL (mimics abort condition)

    for (size_t i = 0; i < len; ++i)
    {
        const char* e = expected[i];
        const char* a = actual[i];

        // Handle NULL elements to mimic behavior where NULL vs non-NULL causes failure.
        if (e == nullptr || a == nullptr)
        {
            if (e != a) // one NULL and the other non-NULL -> not equal
                return false;
            else
                continue; // both NULL -> considered equal for this position
        }

        // Compare actual string content
        if (std::strcmp(e, a) != 0)
            return false;
    }
    return true;
}

/*
  Test 1:
  NotEqualStringArray6_NullExpected_ShouldFail
  - Purpose: Validate that when expStrings is NULL and testStrings is a non-empty array,
             the string-array equality check fails (as in the focal test).
  - Expected: The comparison reports inequality (false), hence the test passes.
*/
bool NotEqualStringArray6_NullExpected_ShouldFail()
{
    // Candidate Keywords:
    // testStrings: actual values
    // expStrings: intentionally NULL to simulate expected failure
    const char* testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char** expStrings = NULL;

    // Cast to match the expected function signature utility (to simulate Unity's macro usage)
    bool result = compareStringArray((const char**)expStrings, (const char* const*)testStrings, 4);

    // We expect a failure (inequality), so a false result means the test passes.
    return (result == false);
}

/*
  Test 2:
  NotEqualStringArray6_NullActual_ShouldFail
  - Purpose: Validate that when testStrings (actual) is NULL and expStrings is non-NULL,
             the string-array equality check fails.
  - Expected: The comparison reports inequality (false), hence the test passes.
*/
bool NotEqualStringArray6_NullActual_ShouldFail()
{
    const char* testStrings[] = { "foo", "boo", "woo", "zoo" }; // non-NULL actual array
    const char** expStrings = testStrings;                    // non-NULL expected array
    const char* const* actual = NULL;                        // actual is NULL to simulate failure

    bool result = compareStringArray((const char**)expStrings, actual, 4);

    // Expect failure (false)
    return (result == false);
}

/*
  Test 3:
  NotEqualStringArray6_Different_ShouldFail
  - Purpose: Validate that when both arrays are non-NULL but contain different content
             (e.g., last element differs), the equality check fails.
  - Expected: The comparison reports inequality (false), hence the test passes.
*/
bool NotEqualStringArray6_Different_ShouldFail()
{
    const char* testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char* expStrings[] = { "foo", "boo", "woo", "bar" }; // last element differs

    bool result = compareStringArray((const char**)expStrings, (const char* const*)testStrings, 4);

    // Expect failure (false)
    return (result == false);
}

/*
  Simple test runner:
  - Executes each test and reports PASS/FAIL.
  - Non-terminating style: tests return a boolean rather than aborting the process.
*/
int main()
{
    // Register tests
    std::vector<std::pair<std::string, std::function<bool()>>> tests;
    tests.emplace_back("NotEqualStringArray6_NullExpected_ShouldFail", NotEqualStringArray6_NullExpected_ShouldFail);
    tests.emplace_back("NotEqualStringArray6_NullActual_ShouldFail", NotEqualStringArray6_NullActual_ShouldFail);
    tests.emplace_back("NotEqualStringArray6_Different_ShouldFail", NotEqualStringArray6_Different_ShouldFail);

    int passed = 0;
    int total = 0;

    // Run tests
    for (auto &tc : tests)
    {
        ++total;
        bool ok = tc.second();
        if (ok)
        {
            ++passed;
            std::cout << "PASS: " << tc.first << "\n";
        }
        else
        {
            std::cout << "FAIL: " << tc.first << "\n";
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}