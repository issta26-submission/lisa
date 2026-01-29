// Test suite for the focal behavior: HEX32 array equality checks.
// This is implemented in C++11 (no GTest) and is designed to be run standalone
// by invoking main. It mirrors the scenarios in the Unity-based testEqualHEX32Arrays
// from testunity.c, but uses a lightweight C++ harness to maximize portability
// and coverage without depending on Unity.
//
// NOTE: This test suite focuses on functional equivalence of 32-bit hex values
// in arrays, independent of the Unity test framework. It provides true/false
// outcomes via explicit boolean returns to allow continued execution of other tests.

#include <cstdint>
#include <unity.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


// Lightweight helper to compare two HEX32 arrays element-by-element.
// - a and b may point to arrays of length 'len' elements.
// - Returns true if all corresponding elements are equal; false otherwise.
// - No reliance on Unity; pure C++11 logic.
static bool equalHEX32Arrays(const uint32_t* a, const uint32_t* b, size_t len)
{
    if (a == nullptr || b == nullptr) {
        // If both are NULL and length is zero, treat as equal in a minimal sense.
        // Our tests below avoid NULL pointers for non-zero-length comparisons.
        if (a == nullptr && b == nullptr && len == 0) return true;
        // For non-zero length, treat NULL pointers as not equal.
        return (a == b) && (len == 0);
    }

    for (size_t i = 0; i < len; ++i)
    {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// 1) Test: identical array (same pointer) with length 4 should be equal.
static bool test_equal_case_A(void)
{
    uint32_t p0[] = {1, 8, 987, 65132u};
    // Compare p0 with itself for length 4
    return equalHEX32Arrays(p0, p0, 4);
}

// 2) Test: two separate but content-equal arrays with length 4 should be equal.
static bool test_equal_case_B(void)
{
    uint32_t p0[] = {1, 8, 987, 65132u};
    uint32_t p1[] = {1, 8, 987, 65132u};
    return equalHEX32Arrays(p0, p1, 4);
}

// 3) Test: arrays equal for first 3 elements even if the 4th differs, using length 3.
static bool test_equal_case_C(void)
{
    uint32_t p0[] = {1, 8, 987, 65132u};
    uint32_t p2[] = {1, 8, 987, 2};
    return equalHEX32Arrays(p0, p2, 3);
}

// 4) Test: equal for length 1 (only the first element matters here).
static bool test_equal_case_D(void)
{
    uint32_t p0[] = {1, 8, 987, 65132u};
    uint32_t p3[] = {1, 500, 600, 700};
    return equalHEX32Arrays(p0, p3, 1);
}

// 5) Negative test: not equal when comparing full arrays (length 4).
// This exercises the false branch of the equality predicate.
static bool test_not_equal_case_E(void)
{
    uint32_t p0[] = {1, 8, 987, 65132u};
    uint32_t p3[] = {1, 500, 600, 700};
    return !equalHEX32Arrays(p0, p3, 4);
}

// Simple test harness: runs all tests and reports results.
// Per instructions, tests are invoked from main (no GTest). Explanatory
// comments accompany each test function.
int main(void)
{
    // Define test cases with their names and function pointers.
    struct TestCase {
        const char* name;
        bool (*func)(void);
    };

    // Forward declare a local lambda-capable wrapper to hold test pointers.
    // We will populate a static array of TestCase.
    static const TestCase tests[] = {
        { "test_equal_case_A", test_equal_case_A },
        { "test_equal_case_B", test_equal_case_B },
        { "test_equal_case_C", test_equal_case_C },
        { "test_equal_case_D", test_equal_case_D },
        { "test_not_equal_case_E", test_not_equal_case_E }
    };

    const size_t testCount = sizeof(tests) / sizeof(tests[0]);
    int failed = 0;

    std::cout << "Running HEX32 array equality tests (no GTest Unity)..." << std::endl;

    for (size_t i = 0; i < testCount; ++i)
    {
        bool result = tests[i].func();
        if (result)
        {
            std::cout << "[PASSED] " << tests[i].name << std::endl;
        }
        else
        {
            std::cout << "[FAILED] " << tests[i].name << std::endl;
            ++failed;
        }
    }

    std::cout << "Summary: " << (testCount - failed) << " passed, " << failed << " failed." << std::endl;

    // Return non-zero if any test failed, suitable for simple CI checks.
    return failed;
}