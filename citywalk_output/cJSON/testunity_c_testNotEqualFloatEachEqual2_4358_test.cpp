/*
  Candidate Keywords extracted from the focal method and its dependencies:
  - UNITY_EXCLUDE_FLOAT, TEST_ASSERT_EACH_EQUAL_FLOAT
  - Floating-point test scenario: 8.0f vs 0.253f in an array
  - p0: {8.0f, 8.0f, 8.0f, 0.253f}
  - testNotEqualFloatEachEqual2: intended to fail because not all elements equal 8.0f
  - Domain focus: non-GTest C++ test without GTest, using std::fabs for tolerance
  - Static/dependency notes: UNITY-like testing macros are not used here; we implement a minimal C++11 test harness
  - Namespace and accessibility: adhere to standard C++11; avoid private/public API concerns of focal C code
  - No GMock/GTest; only standard library
*/

#include <cmath>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight, self-contained test harness focused on the focal scenario.
// This mirrors the behavior of the Unity test's intention: verify that not all
// elements in the float array are equal to the expected value (8.0f in this case).

// Test 1: NotEqualFloatEachEqual2_ShouldFail
// Rationale: In the original Unity test, the array p0 contains a non-matching
// value (0.253f). Therefore, TEST_ASSERT_EACH_EQUAL_FLOAT(8.0f, p0, 4) should fail.
// The function returns true to indicate that the test correctly detected a failure.
static bool test_NotEqualFloatEachEqual2_ShouldFail(void)
{
    // Focal test data replicated from the Unity test:
    // float p0[] = {8.0f, 8.0f, 8.0f, 0.253f};
    float p0[] = {8.0f, 8.0f, 8.0f, 0.253f};
    const float expected = 8.0f;
    const float tol = 1e-6f;

    bool all_equal = true;
    for (std::size_t i = 0; i < 4; ++i)
    {
        if (std::fabs(p0[i] - expected) > tol)
        {
            all_equal = false;
            break;
        }
    }

    // The test should fail (i.e., not all values equal) => return true to indicate expected failure.
    return !all_equal;
}

// Test 2: NotEqualFloatEachEqual2_ShouldPassWhenAllElementsAreEqual
// Rationale: A complementary test where all elements are equal to 8.0f.
// This verifies the "pass" path for the same predicate (all elements equal).
static bool test_NotEqualFloatEachEqual2_ShouldPassIfAllEqual(void)
{
    float p0[] = {8.0f, 8.0f, 8.0f, 8.0f};
    const float expected = 8.0f;
    const float tol = 1e-6f;

    bool all_equal = true;
    for (std::size_t i = 0; i < 4; ++i)
    {
        if (std::fabs(p0[i] - expected) > tol)
        {
            all_equal = false;
            break;
        }
    }

    // All elements equal: test passes => return true.
    return all_equal;
}

// Simple test runner
int main()
{
    int total = 0;
    int passed = 0;
    int failed = 0;

    // Run Test 1: Expect a failure path (not all elements equal)
    ++total;
    if (test_NotEqualFloatEachEqual2_ShouldFail())
    {
        // The focal test expected a failure; receiving a "true" here means we detected the failure as intended.
        ++passed;
        std::cout << "[PASS] test_NotEqualFloatEachEqual2_ShouldFail" << std::endl;
    }
    else
    {
        ++failed;
        std::cout << "[FAIL] test_NotEqualFloatEachEqual2_ShouldFail" << std::endl;
        std::cout << "  Expected: test to indicate a failure (not all elements equal) but it did not." << std::endl;
    }

    // Run Test 2: Complementary pass-path (all elements equal)
    ++total;
    if (test_NotEqualFloatEachEqual2_ShouldPassIfAllEqual())
    {
        ++passed;
        std::cout << "[PASS] test_NotEqualFloatEachEqual2_ShouldPassIfAllEqual" << std::endl;
    }
    else
    {
        ++failed;
        std::cout << "[FAIL] test_NotEqualFloatEachEqual2_ShouldPassIfAllEqual" << std::endl;
        std::cout << "  Expected: all elements equal; test should pass but reported failure." << std::endl;
    }

    // Summary
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nTest Summary: "
              << "Total=" << total
              << ", Passed=" << passed
              << ", Failed=" << failed
              << std::endl;

    // Return non-zero if any test failed
    return failed == 0 ? 0 : 1;
} 

// Explanatory comments for each unit test:
// - test_NotEqualFloatEachEqual2_ShouldFail
//   Reproduces the focal scenario from testNotEqualFloatEachEqual2 in testunity.c.
//   It uses the exact array {8.0f, 8.0f, 8.0f, 0.253f} and checks if all elements are equal to 8.0f
//   within a small tolerance. Since one element differs, the "all equal" condition is false,
//   and the test reports a failure as expected (returns true).
// - test_NotEqualFloatEachEqual2_ShouldPassIfAllEqual
//   A complementary scenario where all elements match the expected value (8.0f).
//   This ensures the "pass" path of the same predicate is exercised.