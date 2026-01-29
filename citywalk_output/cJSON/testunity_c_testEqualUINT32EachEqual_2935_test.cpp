// Unit test suite for the focal method: testEqualUINT32EachEqual
// This suite is implemented in C++11 without using GoogleTest.
// It re-implements the essential behavior of the Unity macro TEST_ASSERT_EACH_EQUAL_UINT32
// in a small, self-contained test harness to achieve high coverage of the core logic.
//
// The focal method under test conceptually checks that all elements in an array are equal
// to a given value. We create a lightweight tester that mirrors that behavior and write
// test cases that exercise both the passing and failing scenarios from the original code.

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>


// Lightweight helper that emulates TEST_ASSERT_EACH_EQUAL_UINT32 behavior.
// Returns true if every element in 'arr' of length 'len' equals 'expected'.
static bool each_equal_uint32(const uint32_t* arr, size_t len, uint32_t expected)
{
    if (arr == nullptr) {
        // In Unity, passing a NULL pointer to the macro would typically fail.
        // For our simple harness, treat NULL as failure (no elements to compare).
        return false;
    }
    for (size_t i = 0; i < len; ++i)
    {
        if (arr[i] != expected)
        {
            return false;
        }
    }
    return true;
}

// Simple test harness infrastructure
static int tests_run = 0;
static int tests_passed = 0;

static void run_test(bool condition, const std::string& testName, const std::string& reason = "")
{
    ++tests_run;
    if (condition)
    {
        ++tests_passed;
        std::cout << "PASS: " << testName << "\n";
    }
    else
    {
        std::cout << "FAIL: " << testName;
        if (!reason.empty()) std::cout << " - " << reason;
        std::cout << "\n";
    }
}

// 1) Focal test: testEqualUINT32EachEqual
// Reproduces the passing scenarios from the Unity test by verifying that
// all elements in the given arrays match the expected value.

void test_equalUINT32EachEqual_passes(void)
{
    // a) Single-element array: length 1, all elements equal to 65132
    {
        uint32_t p0[] = {65132u};
        run_test(each_equal_uint32(p0, 1, 65132u) == true,
                 "test_equalUINT32EachEqual_passes - single element matches",
                 "Expected true when all elements equal the value");
    }

    // b) Four-element array: all elements equal to 65132
    {
        uint32_t p0[] = {65132u, 65132u, 65132u, 65132u};
        run_test(each_equal_uint32(p0, 4, 65132u) == true,
                 "test_equalUINT32EachEqual_passes - four elements all 65132",
                 "Expected true when all four elements equal the value");
    }

    // c) Four-element array with another uniform value (987)
    {
        uint32_t p1[] = {987u, 987u, 987u, 987u};
        run_test(each_equal_uint32(p1, 4, 987u) == true,
                 "test_equalUINT32EachEqual_passes - four elements all 987",
                 "Expected true when all four elements equal the value");
    }

    // d) First three elements equal to 8, fourth element differs
    // We only check the first 3 elements (len = 3)
    {
        uint32_t p2[] = {8u, 8u, 8u, 2u};
        run_test(each_equal_uint32(p2, 3, 8u) == true,
                 "test_equalUINT32EachEqual_passes - first three elements equal 8",
                 "Expected true when first three elements equal the value");
    }

    // e) First element matches, others are different but length is 1
    {
        uint32_t p3[] = {1u, 500u, 600u, 700u};
        run_test(each_equal_uint32(p3, 1, 1u) == true,
                 "test_equalUINT32EachEqual_passes - first element equals 1",
                 "Expected true when length is 1 and first element matches");
    }
}

// 2) Negative tests: ensure that not-all-equal scenarios return false.
// The original Unity tests expect a failure; in this harness we verify that
// the predicate detects mismatches without aborting execution.

void test_equalUINT32EachEqual_failures(void)
{
    // a) {65132, 65132, 987, 65132} -> not all equal to 65132
    {
        uint32_t arr[] = {65132u, 65132u, 987u, 65132u};
        run_test(each_equal_uint32(arr, 4, 65132u) == false,
                 "test_notEqualUINT32EachEqual1 - not all elements equal",
                 "Expected false when a non-matching element is present");
    }

    // b) {1, 987, 987, 987} -> not all equal to 987
    {
        uint32_t arr[] = {1u, 987u, 987u, 987u};
        run_test(each_equal_uint32(arr, 4, 987u) == false,
                 "test_notEqualUINT32EachEqual2 - first element differs",
                 "Expected false when at least one element differs");
    }

    // c) {1, 1, 1, 65132} -> not all equal to 1
    {
        uint32_t arr[] = {1u, 1u, 1u, 65132u};
        run_test(each_equal_uint32(arr, 4, 1u) == false,
                 "test_notEqualUINT32EachEqual3 - last element differs",
                 "Expected false when last element differs");
    }
}

// Main entry point to run the test suite
int main()
{
    std::cout << "Starting test suite for testEqualUINT32EachEqual (simulated Unity behavior)\n";

    test_equalUINT32EachEqual_passes();
    test_equalUINT32EachEqual_failures();

    // Summary
    std::cout << "=================================\n";
    std::cout << "Total tests run: " << tests_run << "\n";
    std::cout << "Tests passed:  " << tests_passed << "\n";
    std::cout << "Tests failed:  " << (tests_run - tests_passed) << "\n";

    // Return non-zero if any test failed
    return (tests_run == tests_passed) ? 0 : 1;
}