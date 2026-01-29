/*
  Lightweight C++11 test suite for the focal Unity test method
 : testNotEqualDoubleEachEqual1

  Context and rationale
  - The provided focal method is a Unity test asserting that every element in an array
    equals an expected double value:
      TEST_ASSERT_EACH_EQUAL_DOUBLE(expected, array, count);
    In the original testNotEqualDoubleEachEqual1, the array contains a non-matching
    element, so the assertion would fail (i.e., the test would abort).

  - This suite emulates the core dependency (a small assertion helper for doubles)
    and provides non-terminating tests (no longjmp/abort) to enable executable
    test cases in a plain C++11 harness without GTest.

  - The Candidate Keywords below are derived from the focal method and its referenced
    symbols, to help guide test coverage mapping between the original C Unity test
    and this C++ translation.

  - Note: This suite does not depend on Unity. It uses a tiny, self-contained
    assertion helper to validate behavior, suitable for inclusion alongside
    the original test when needed.

  Structure:
  - Candidate Keywords
  - Minimal assertion helper: testNotEqualDoubleEachEqualImpl
  - Test cases:
      test_NotEqualDoubleEachEqual1_ReturnsFalse   (verifies mismatch detected)
      test_EqualDoubleEachEqual_AllMatch_ReturnsTrue (verifies all-match case)
      test_NotEqualDoubleEachEqualWithNaN_ReturnsFalse (verifies NaN handling)
  - Lightweight test runner (no external frameworks)
*/

#include <cmath>
#include <cstddef>
#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords (mapped from <FOCAL_METHOD> and <FOCAL_CLASS_DEP> context)
// - TEST_ASSERT_EACH_EQUAL_DOUBLE
// - UNITY_EXCLUDE_DOUBLE
// - p0 (array under test)
// - 0.253 (expected value)
// - testNotEqualDoubleEachEqual1 (focal test name)
// - startPutcharSpy / endPutcharSpy (Unity I/O spies in original test)
// - VERIFY_FAILS_END, TEST_ABORT_BEGIN (Unity fail/abort scaffolding)
// - Unity.CurrentTestFailed / CurrentTestLineNumber (test state in Unity)

// 1. Minimal assertion helper that mimics the essence of
//    TEST_ASSERT_EACH_EQUAL_DOUBLE(expected, array, count)
//    It returns true if all elements equal 'expected', false otherwise.
static bool testNotEqualDoubleEachEqualImpl(const double* arr, std::size_t count, double expected)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        if (arr[i] != expected)
        {
            return false; // Found a non-matching element
        }
    }
    return true; // All elements matched
}

// 2. Test case: NotEqualDoubleEachEqual1
//    Mirrors the focal test: an array with a single non-matching element should fail.
//    We treat "fail" as returning false from the assertion helper.
static bool test_NotEqualDoubleEachEqual1_ReturnsFalse(void)
{
    // Original test data: {0.253, 8.0, 0.253, 0.253}
    // Expected: all elements equal to 0.253
    const double p0[] = {0.253, 8.0, 0.253, 0.253};
    bool allMatch = testNotEqualDoubleEachEqualImpl(p0, 4, 0.253);
    // Expectation: not all elements match (since 8.0 != 0.253)
    return (allMatch == false);
}

// 3. Test case: EqualDoubleEachEqual_AllMatch
//    A positive control: all elements match the expected value.
static bool test_EqualDoubleEachEqual_AllMatch_ReturnsTrue(void)
{
    const double p[] = {0.253, 0.253, 0.253, 0.253};
    bool allMatch = testNotEqualDoubleEachEqualImpl(p, 4, 0.253);
    return allMatch == true;
}

// 4. Test case: NotEqualDoubleEachEqualWithNaN
//    NaN should cause the comparison to fail since NaN != 0.0 for the expected value.
static bool test_NotEqualDoubleEachEqualWithNaN_ReturnsFalse(void)
{
    // Include a NaN within the array
    const double nanVal = std::numeric_limits<double>::quiet_NaN();
    const double p[] = {0.0, nanVal, 0.0, 0.0};
    bool allMatch = testNotEqualDoubleEachEqualImpl(p, 4, 0.0);
    // NaN != 0.0 makes the assertion fail
    return (allMatch == false);
}

// 5. Lightweight test harness
//    A tiny runner that executes each test, reports PASS/FAIL, and returns an exit code.
//    This runner is self-contained and does not rely on external testing frameworks.
int main(void)
{
    using TestFn = std::function<bool(void)>;
    struct TestCase {
        const char* name;
        TestFn func;
    };

    // Assemble test suite (non-Unity, purely C++11)
    std::vector<TestCase> tests = {
        { "NotEqualDoubleEachEqual1_ReturnsFalse",  test_NotEqualDoubleEachEqual1_ReturnsFalse },
        { "EqualDoubleEachEqual_AllMatch_ReturnsTrue", test_EqualDoubleEachEqual_AllMatch_ReturnsTrue },
        { "NotEqualDoubleEachEqualWithNaN_ReturnsFalse", test_NotEqualDoubleEachEqualWithNaN_ReturnsFalse }
    };

    int total = 0;
    int passed = 0;

    std::cout << "Running unit test suite for focal method: testNotEqualDoubleEachEqual1 (not using Unity GTest)\n";
    std::cout << "=============================================================\n";

    for (const auto& tc : tests)
    {
        ++total;
        bool result = false;
        try
        {
            result = tc.func();
        }
        catch (...)
        {
            result = false;
        }

        if (result)
        {
            ++passed;
            std::cout << "[PASS]  " << tc.name << "\n";
        }
        else
        {
            std::cout << "[FAIL]  " << tc.name << "\n";
        }
    }

    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Test results: " << passed << " / " << total << " passed\n";

    // Exit code semantics: 0 if all tests pass, non-zero otherwise
    return (passed == total) ? 0 : 1;
}