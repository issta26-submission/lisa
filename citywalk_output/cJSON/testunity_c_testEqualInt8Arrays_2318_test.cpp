/*
Step 1 - Program Understanding (Candidate Keywords)
- Core components: int8/int8_t arrays, equality comparison, length-based comparison
- Dependencies: standard C++11 headers for vector, iostream, cstdint
- Focus: replicate the behavior of testEqualInt8Arrays from testunity.c in a standalone C++11 test harness
- Approach: implement a minimal, static test suite class that provides an equality checker and five test cases mirroring the Unity test cases
- Static members: use static methods for helper and tests to reflect the static nature of test utilities
- Non-terminating assertions: this harness reports PASS/FAIL without terminating the process to maximize coverage
- Namespace: no external dependencies; keep in global scope but encapsulated in a class
*/

/* Lightweight standalone test harness for testEqualInt8Arrays behavior
   This does not depend on Unity; it exercises equivalent logic using C++11. */

#include <cstddef>
#include <unity.h>
#include <iostream>
#include <stdint.h>
#include <string.h>
#include <cstdint>
#include <stdio.h>


class TestEqualInt8ArraysSuite
{
public:
    // Core utility: compare two int8 arrays up to length 'len'
    // Handles null pointers gracefully by considering two NULL pointers equal
    static bool equalInt8Arrays(const int8_t* a, const int8_t* b, size_t len)
    {
        if (a == nullptr || b == nullptr)
            return (a == nullptr) && (b == nullptr);
        for (size_t i = 0; i < len; ++i)
        {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }

    // Test Case 1
    // Mirrors: TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p0, 1);
    // Expectation: first element of p0 equals itself for a length of 1
    static bool test_case_1_passes()
    {
        int8_t p0[] = {1, 8, 117, -2};
        return equalInt8Arrays(p0, p0, 1);
    }

    // Test Case 2
    // Mirrors: TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p0, 4);
    // Expectation: entire p0 compared to itself for length 4 should be equal
    static bool test_case_2_passes()
    {
        int8_t p0[] = {1, 8, 117, -2};
        return equalInt8Arrays(p0, p0, 4);
    }

    // Test Case 3
    // Mirrors: TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p1, 4);
    // Expectation: p0 equals p1 when both are {1,8,117,-2} for length 4
    static bool test_case_3_passes()
    {
        int8_t p0[] = {1, 8, 117, -2};
        int8_t p1[] = {1, 8, 117, -2};
        return equalInt8Arrays(p0, p1, 4);
    }

    // Test Case 4
    // Mirrors: TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p2, 3);
    // Expectation: first three elements of p0 and p2 are equal
    static bool test_case_4_passes()
    {
        int8_t p0[] = {1, 8, 117, -2};
        int8_t p2[] = {1, 8, 117, 2};
        return equalInt8Arrays(p0, p2, 3);
    }

    // Test Case 5
    // Mirrors: TEST_ASSERT_EQUAL_INT8_ARRAY(p0, p3, 1);
    // Expectation: first element of p0 equals first element of p3 for length 1
    static bool test_case_5_passes()
    {
        int8_t p0[] = {1, 8, 117, -2};
        int8_t p3[] = {1, 50, 60, 70};
        return equalInt8Arrays(p0, p3, 1);
    }

    // Additional negative scenario to broaden coverage (not part of focal method)
    // Verifies that arrays with a mismatch are detected when length covers mismatch
    static bool test_case_not_equal_for_length_4()
    {
        int8_t p0[] = {1, 8, 36, -2};
        int8_t p1[] = {1, 8, 36, 2}; // last element differs
        return !equalInt8Arrays(p0, p1, 4);
    }

    // Run all tests; returns number of failed tests
    static int runAll()
    {
        int failures = 0;

        if (!test_case_1_passes()) {
            std::cout << "[Fail] test_case_1_passes\n";
            ++failures;
        } else {
            std::cout << "[Pass] test_case_1_passes\n";
        }

        if (!test_case_2_passes()) {
            std::cout << "[Fail] test_case_2_passes\n";
            ++failures;
        } else {
            std::cout << "[Pass] test_case_2_passes\n";
        }

        if (!test_case_3_passes()) {
            std::cout << "[Fail] test_case_3_passes\n";
            ++failures;
        } else {
            std::cout << "[Pass] test_case_3_passes\n";
        }

        if (!test_case_4_passes()) {
            std::cout << "[Fail] test_case_4_passes\n";
            ++failures;
        } else {
            std::cout << "[Pass] test_case_4_passes\n";
        }

        if (!test_case_5_passes()) {
            std::cout << "[Fail] test_case_5_passes\n";
            ++failures;
        } else {
            std::cout << "[Pass] test_case_5_passes\n";
        }

        // Negative coverage
        if (!test_case_not_equal_for_length_4()) {
            std::cout << "[Fail] test_case_not_equal_for_length_4\n";
            ++failures;
        } else {
            std::cout << "[Pass] test_case_not_equal_for_length_4\n";
        }

        return failures;
    }
};

// Entry point to execute the test suite
int main()
{
    std::cout << "Running TestEqualInt8ArraysSuite (standalone C++11 harness)" << std::endl;
    int fails = TestEqualInt8ArraysSuite::runAll();
    if (fails == 0)
    {
        std::cout << "All tests passed." << std::endl;
    }
    else
    {
        std::cout << "Total failures: " << fails << std::endl;
    }
    return fails;
}