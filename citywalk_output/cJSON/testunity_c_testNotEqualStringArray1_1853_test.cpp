/*
 * Lightweight C++11 test suite for the focal method/testNotEqualStringArray1
 * (adapted from the Unity-based test in testunity.c)
 *
 * Notes:
 * - This is a standalone, GTest-free test harness implemented in C++11.
 * - It mirrors the intent of Unity's testNotEqualStringArray1: verify that two
 *   string arrays are not equal, and thus the test should indicate a failure
 *   if equality is incorrectly detected.
 * - For simplicity and portability in C++11, we implement a tiny string-array
 *   comparison utility and a small test framework (no external dependencies).
 * - All tests are non-terminating (they return a boolean success/failure).
 * - This harness can be extended to cover more scenarios around string-array
 *   comparisons (null pointers, length variations, etc.).
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


// Utility: compare two arrays of C-strings (may contain NULLs) element-wise.
// Behavior matches intuitive notion of string array equality:
// - NULL == NULL is considered equal for that element.
// - NULL != non-NULL is not equal.
// - Non-NULL strings are compared using strcmp.
static bool string_arrays_equal(const char* const* a, const char* const* b, size_t len)
{
    if (a == nullptr && b == nullptr) return true;
    if ((a == nullptr) != (b == nullptr)) return false;
    for (size_t i = 0; i < len; ++i)
    {
        const char* sa = a[i];
        const char* sb = b[i];
        if (sa == nullptr && sb == nullptr) continue;
        if ((sa == nullptr) != (sb == nullptr)) return false;
        if (strcmp(sa, sb) != 0) return false;
    }
    return true;
}

// Convenience: wrappers around the focal data from testNotEqualStringArray1
// (replicating the structure used by the Unity test).
static bool focal_testNotEqualStringArray1_logic()
{
    // Candidate Keywords: testStrings, expStrings, array length 4
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    // Expectation: arrays are NOT equal (last element differs: "moo" vs "zoo")
    bool areEqual = string_arrays_equal(expStrings, testStrings, 4);
    // In the original Unity test, this would abort/FAIL if equal.
    // Here we return true to indicate the test "should fail" if equal,
    // but since we know they are not equal, the test should pass.
    return !areEqual;
}

// Test 1: Not equal because the last element differs ("moo" vs "zoo").
// Mirrors the focal testNotEqualStringArray1 behavior.
static bool testNotEqualStringArray1()
{
    // This mirrors the exact scenario from <FOCAL_METHOD>.
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    bool equal = string_arrays_equal(expStrings, testStrings, 4);
    // The focal test expects inequality, so the test passes if not equal.
    return !equal;
}

// Test 2: Not equal due to first element mismatch
static bool testNotEqualStringArray2()
{
    const char *testStrings[] = { "zoo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "moo" };
    bool equal = string_arrays_equal(expStrings, testStrings, 4);
    return !equal;
}

// Test 3: Not equal due to a NULL in last element of actual array
static bool testNotEqualStringArray3()
{
    const char *testStrings[] = { "foo", "boo", "woo", NULL };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };
    bool equal = string_arrays_equal(expStrings, testStrings, 4);
    return !equal;
}

// Test 4: Not equal due to a NULL in expStrings last element
static bool testNotEqualStringArray4()
{
    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", NULL, "woo", "moo" };
    bool equal = string_arrays_equal(expStrings, testStrings, 4);
    return !equal;
}

// Test 5: Not equal because actual testStrings is NULL while expStrings is non-NULL
static bool testNotEqualStringArray5()
{
    const char **testStrings = nullptr;
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };
    bool equal = string_arrays_equal(expStrings, testStrings, 4);
    return !equal;
}

// Test 6: Not equal because expStrings is NULL while testStrings is non-NULL
static bool testNotEqualStringArray6()
{
    const char *testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char **expStrings = nullptr;
    bool equal = string_arrays_equal(expStrings, testStrings, 4);
    return !equal;
}

// Test 7: Equal when both arrays are NULL (edge case)
static bool testEqualStringArrayIfBothNulls()
{
    const char *const* testStrings = nullptr;
    const char *const* expStrings  = nullptr;
    bool equal = string_arrays_equal(expStrings, testStrings, 4);
    return equal;
}

// Extra: Equal when both arrays are non-NULL and identical
static bool testEqualStringArrayIfIdentical()
{
    const char *testStrings[] = { "foo", "bar", "baz" };
    const char *expStrings[]  = { "foo", "bar", "baz" };
    bool equal = string_arrays_equal(expStrings, testStrings, 3);
    return equal;
}

// Simple test harness
struct TestCase {
    const char* name;
    std::function<bool()> func;
};

int main()
{
    std::vector<TestCase> tests = {
        { "testNotEqualStringArray1", testNotEqualStringArray1 },
        { "testNotEqualStringArray2", testNotEqualStringArray2 },
        { "testNotEqualStringArray3", testNotEqualStringArray3 },
        { "testNotEqualStringArray4", testNotEqualStringArray4 },
        { "testNotEqualStringArray5", testNotEqualStringArray5 },
        { "testNotEqualStringArray6", testNotEqualStringArray6 },
        { "testEqualStringArrayIfBothNulls", testEqualStringArrayIfBothNulls },
        { "testEqualStringArrayIfIdentical", testEqualStringArrayIfIdentical }
    };

    int passed = 0;
    int failed = 0;

    std::cout << "Running " << tests.size() << " tests for focal method: testNotEqualStringArray1 and related scenarios\n";

    for (const auto& t : tests)
    {
        bool result = false;
        try
        {
            result = t.func();
        }
        catch (...)
        {
            result = false;
        }

        if (result)
        {
            std::cout << "[PASS]  " << t.name << "\n";
            ++passed;
        }
        else
        {
            std::cout << "[FAIL]  " << t.name << "\n";
            ++failed;
        }
    }

    std::cout << "\nSummary: " << passed << " passed, " << failed << " failed\n";

    // Return non-zero if any test failed
    return failed > 0 ? 1 : 0;
}