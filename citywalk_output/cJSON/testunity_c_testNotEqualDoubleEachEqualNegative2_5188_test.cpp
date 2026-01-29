/*
  Lightweight C++11 test suite reproducing the focal Unity test:
  testNotEqualDoubleEachEqualNegative2 located in testunity.c

  Notes:
  - This harness does not rely on Unity. It provides a minimal, non-terminating
    assertion mechanism suitable for C++11 compilation.
  - The focal test under test asserts that TEST_ASSERT_EACH_EQUAL_DOUBLE(-25.4, p0, 4)
    should fail for a mixed array { -25.4, -8.0, -25.4, -25.4 }.
  - We implement a tiny replica of the core behavior: a function that checks all
    elements equal to an expected value and return whether that check would pass.
  - We provide two tests:
    1) testNotEqualDoubleEachEqualNegative2: exercises the negative/false path.
    2) testEqualDoubleEachEqual: exercises the positive/true path for completeness.
  - The tests are structured to be run from main() and report pass/fail status.

  This file can be compiled with a C++11 compiler (e.g., g++ -std=c++11).
*/

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (from Step 1): TEST_ASSERT_EACH_EQUAL_DOUBLE, double, array,
// comparison, failure, negative, UNITY_EXCLUDE_DOUBLE, testNotEqualDoubleEachEqualNegative2

// Lightweight assertion helper: checks if all elements in arr of length len
// are exactly equal to 'expected'. Used to model the core logic of
// TEST_ASSERT_EACH_EQUAL_DOUBLE without pulling in the Unity framework.
static bool test_assert_each_equal_double(double expected, const double* arr, std::size_t len)
{
    for (std::size_t i = 0; i < len; ++i)
    {
        if (arr[i] != expected)
        {
            return false;
        }
    }
    return true;
}

// Focal test translated into C++: not equal among elements should cause a failure
// in the Unity test (i.e., the assertion should abort). In this harness, we
// return 'true' if the behavior matches the expected failure (i.e., not all
// elements are equal).
static bool testNotEqualDoubleEachEqualNegative2_impl()
{
    // Array from the original focal method
    double p0[] = { -25.4, -8.0, -25.4, -25.4 };

    // Modeling: all elements equal to -25.4? No (one is -8.0).
    bool allEqual = test_assert_each_equal_double(-25.4, p0, 4);

    // The Unity test expects this to fail, so we return true if it indeed fails.
    return !allEqual; // true because not all elements are equal
}

// Additional supportive test: all elements equal (positive path)
static bool testEqualDoubleEachEqual_impl()
{
    double p[] = { -25.4, -25.4, -25.4, -25.4 };
    bool allEqual = test_assert_each_equal_double(-25.4, p, 4);
    // Expect the path to succeed (no abort)
    return allEqual;
}

// Simple test harness
struct TestCase
{
    std::string name;
    std::function<bool()> func;
};

// Entry point
int main()
{
    std::vector<TestCase> tests = {
        // Test that the not-equal scenario correctly triggers a failure (positive for the suite)
        { "testNotEqualDoubleEachEqualNegative2", testNotEqualDoubleEachEqualNegative2_impl },
        // Complementary test: ensure the equal scenario passes (additional coverage)
        { "testEqualDoubleEachEqual", testEqualDoubleEachEqual_impl }
    };

    int total = 0;
    int passed = 0;
    int failed = 0;

    std::cout << "Running test suite for focal method: testNotEqualDoubleEachEqualNegative2 (and related cases)\n";
    std::cout << "-------------------------------------------------------------\n";

    for (const auto& t : tests)
    {
        ++total;
        bool result = t.func();
        std::cout << (result ? "PASS: " : "FAIL: ") << t.name << "\n";
        if (result) ++passed; else ++failed;
    }

    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Total:   " << total << "\n";
    std::cout << "Passed:  " << passed << "\n";
    std::cout << "Failed:  " << failed << "\n";

    // Return non-zero if any test failed
    return (failed == 0) ? 0 : 1;
}

/*
  Explanatory comments for the unit tests:

  - testNotEqualDoubleEachEqualNegative2:
    Intent: Verify that TEST_ASSERT_EACH_EQUAL_DOUBLE would abort when the
    provided array does not contain all equal elements to the expected value -25.4.
    This mirrors the focal test's scenario where the array is [-25.4, -8.0, -25.4, -25.4].
    The function returns true to indicate the observed behavior matches the expected
    failure condition (i.e., not all elements are equal).

  - testEqualDoubleEachEqual:
    Intent: Provide coverage for the positive path where all elements are equal to -25.4.
    This ensures the "true" branch (no abort) is exercised in the same core logic.

  These tests rely only on standard C++11 facilities and avoid external test
  frameworks, respecting the constraints from the domain knowledge.
*/