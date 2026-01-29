// Candidate Keywords derived from the focal method and its context
// - UNITY_INT8 / int8_t
// - TEST_ASSERT_EACH_EQUAL_INT8
// - testEqualInt8EachEqual (focal test)
// - Array-based equality assertions over 8-bit integers
// - Non-terminating test style vs. abort-on-failure style
// - Null pointer handling for arrays and length-based checks
// - Basic domain-driven test coverage (true/false branches)

#include <cstdint>
#include <unity.h>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Minimal runtime harness to validate the concept of
// the focal Unity-style test: TEST_ASSERT_EACH_EQUAL_INT8.
// We implement a small helper that checks that all elements in an
// int8_t array are equal to the given value for a specified length.

static bool test_equalInt8EachEqual_impl(int8_t expected, const int8_t* arr, size_t len)
{
    // This mimics the semantics of TEST_ASSERT_EACH_EQUAL_INT8 with respect to
    // the provided length. For len == 0, Unity would treat as a no-op/pass.
    if (len == 0) {
        return true;
    }
    if (arr == nullptr) {
        return false;
    }
    for (size_t i = 0; i < len; ++i) {
        if (arr[i] != expected) {
            return false;
        }
    }
    return true;
}

// Lightweight test harness
struct TestCase {
    std::string name;
    std::function<bool()> func;
};

// Individual test cases (explanatory comments provided before each)

// Case 1: All array elements equal to 1, length 1
// This corresponds to TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 1) where p0 = {1, 1, 1, 1}
bool tc_case1_basic_single_element_equal()
{
    int8_t p0[] = {1, 1, 1, 1};
    // Only the first element checked (length = 1)
    return test_equalInt8EachEqual_impl(1, p0, 1);
}

// Case 2: All elements equal to 1, length 4
// This corresponds to TEST_ASSERT_EACH_EQUAL_INT8(1, p0, 4)
bool tc_case2_basic_all_elements_equal()
{
    int8_t p0[] = {1, 1, 1, 1};
    return test_equalInt8EachEqual_impl(1, p0, 4);
}

// Case 3: Elements equal to 117 for first 3 items
// p1 = {117, 117, 117, -2}; length = 3
bool tc_case3_three_elements_equal117()
{
    int8_t p1[] = {117, 117, 117, -2};
    return test_equalInt8EachEqual_impl(117, p1, 3);
}

// Case 4: First two elements are -1, -1; length = 2
// p2 = {-1, -1, 117, 2}
bool tc_case4_two_elements_equalMinusOne()
{
    int8_t p2[] = {-1, -1, 117, 2};
    return test_equalInt8EachEqual_impl(-1, p2, 2);
}

// Case 5: Single element equal to 1
// p3 = {1, 50, 60, 70}; length = 1
bool tc_case5_single_element_equalOne()
{
    int8_t p3[] = {1, 50, 60, 70};
    return test_equalInt8EachEqual_impl(1, p3, 1);
}

// Case 6: Failure case - not all first 2 elements equal to 1
// p_fail = {1, -1, 2}; length = 2 -> should fail
bool tc_case6_failure_not_all_equal()
{
    int8_t p_fail[] = {1, -1, 2};
    return !test_equalInt8EachEqual_impl(1, p_fail, 2);
}

// Case 7: Failure case - NULL array with non-zero length
// arr == NULL and len > 0 should fail
bool tc_case7_failure_null_actual()
{
    int8_t* null_arr = nullptr;
    return !test_equalInt8EachEqual_impl(1, null_arr, 4);
}

// Case 8: Length zero should pass regardless of array pointer
// len == 0 is treated as a pass (no elements to compare)
bool tc_case8_pass_length_zero()
{
    int8_t dummy[] = {1, 2, 3};
    return test_equalInt8EachEqual_impl(0, nullptr, 0);
}

// Helper to run tests and report status
static void run_test_cases(const std::vector<TestCase>& tests)
{
    int total = static_cast<int>(tests.size());
    int passed = 0;

    std::cout << "Running test cases for focal method: testEqualInt8EachEqual\n";
    for (const auto& t : tests) {
        bool result = t.func();
        std::cout << std::setw(40) << std::left << t.name
                  << (result ? "PASS" : "FAIL") << "\n";
        if (result) ++passed;
    }
    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";
}

int main()
{
    // Register test cases with descriptive names and explanatory comments
    std::vector<TestCase> tests = {
        {"Case1_BasicSingleElementEqual", tc_case1_basic_single_element_equal},
        {"Case2_BasicAllElementsEqual4", tc_case2_basic_all_elements_equal},
        {"Case3_ThreeElementsEqual117", tc_case3_three_elements_equal117},
        {"Case4_TwoElementsEqualMinusOne", tc_case4_two_elements_equalMinusOne},
        {"Case5_SingleElementEqualOne", tc_case5_single_element_equalOne},
        {"Case6_FailureNotAllEqual", tc_case6_failure_not_all_equal},
        {"Case7_FailureNullActual", tc_case7_failure_null_actual},
        {"Case8_PassLengthZero", tc_case8_pass_length_zero}
    };

    run_test_cases(tests);

    // Return status: 0 if all tests passed, 1 otherwise
    // (to mimic a non-aborting test harness, we return non-zero if any test failed)
    // We recompute the pass rate:
    int total = static_cast<int>(tests.size());
    int passed = 0;
    for (const auto& t : tests) {
        if (t.func()) ++passed;
    }
    return (passed == total) ? 0 : 1;
}