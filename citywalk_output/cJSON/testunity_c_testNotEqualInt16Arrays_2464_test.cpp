// Test suite: Not-Equal Int16 Arrays - focused on the focal method concept
// This suite is designed in C++11 without GTest. It mirrors the intent of the
// Unity test testNotEqualInt16Arrays from testunity.c by validating the underlying
// array comparison logic that would cause a failure when two int16 arrays differ.
// It provides multiple scenarios to improve coverage and uses explanatory comments
// for each test case.
//
// Notes aligned with the provided guidance:
// - Candidate Keywords (high level): UNITY_INT16, TEST_ASSERT_EQUAL_INT16_ARRAY, p0, p1, len
// - Core dependency concepts: 16-bit signed arrays, element-wise comparison, failure when not equal
// - Static concepts not used here; tests are standalone and run from main
// - No external test framework; simple in-process assertions with pass/fail reporting

#include <unity.h>
#include <iostream>
#include <vector>
#include <stdint.h>
#include <string.h>
#include <string>
#include <cstdint>
#include <stdio.h>

using std::int16_t;
using std::vector;
using std::string;
using std::cout;
using std::endl;

// Candidate Keywords (informational)
/*
Candidate Keywords:
- UNITY_INT16 -> int16_t
- p0, p1 -> test data arrays
- len -> length of arrays
- TEST_ASSERT_EQUAL_INT16_ARRAY -> concept: equality check between int16 arrays
- testNotEqualInt16Arrays -> focal method equivalence (not directly invoked here)
*/

// Helper: compare two int16_t arrays for exact equality
static bool arraysEqualInt16(const int16_t* a, const int16_t* b, size_t len)
{
    if (a == nullptr || b == nullptr) {
        // Consider null pointers not equal unless both are null and len == 0
        return (a == nullptr && b == nullptr && len == 0);
    }
    for (size_t i = 0; i < len; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

// Wrappers for test-style behavior (no aborts; report pass/fail)
static int gTotal = 0;
static int gFailed = 0;

static void runTest(const string& name, bool (*testFunc)())
{
    ++gTotal;
    bool ok = testFunc();
    if (ok) {
        cout << "[PASS] " << name << endl;
    } else {
        cout << "[FAIL] " << name << endl;
        ++gFailed;
    }
}

// Test 1: Primary Not-Equal Case (matches focal scenario: last element differs)
static bool test_NotEqualInt16Arrays_PrimaryCase()
{
    // p0 and p1 differ at the last element: 3 vs 2
    int16_t p0[] = {1, 8, 127, 3};
    int16_t p1[] = {1, 8, 127, 2};
    const size_t len = 4;

    // Expectation: arrays are NOT equal -> arraysEqualInt16 should return false
    bool equal = arraysEqualInt16(p0, p1, len);
    return !equal;
}

// Test 2: Equal Case for sanity (arrays identical -> should be considered equal)
static bool test_EqualInt16Arrays_Identical()
{
    int16_t p0[] = {1, 8, 117, 3};
    int16_t p1[] = {1, 8, 117, 3};
    const size_t len = 4;

    bool equal = arraysEqualInt16(p0, p1, len);
    return equal;
}

// Test 3: Not-Equal Case: Different Last Element
static bool test_NotEqualInt16Arrays_DifferentLast()
{
    int16_t p0[] = {1, 2, 3, 4};
    int16_t p1[] = {1, 2, 3, 5}; // last element differs
    const size_t len = 4;

    bool equal = arraysEqualInt16(p0, p1, len);
    return !equal;
}

// Test 4: Not-Equal Case: Different First Element
static bool test_NotEqualInt16Arrays_DifferentFirst()
{
    int16_t p0[] = {0, 2, 3, 4};
    int16_t p1[] = {1, 2, 3, 4}; // first element differs
    const size_t len = 4;

    bool equal = arraysEqualInt16(p0, p1, len);
    return !equal;
}

// Test 5: Zero-Length Arrays: should be considered equal (no elements to compare)
static bool test_ZeroLengthInt16Arrays_AreEqual()
{
    int16_t p0[] = {};
    int16_t p1[] = {};
    const size_t len = 0;

    bool equal = arraysEqualInt16(p0, p1, len);
    return equal;
}

// Test 6: Not-Equal Case with negative values
static bool test_NotEqualInt16Arrays_NegativeValues()
{
    int16_t p0[] = {-16383, -1, 0, 32767};
    int16_t p1[] = {-16383, -1, 0, -32768}; // last element differs in sign
    const size_t len = 4;

    bool equal = arraysEqualInt16(p0, p1, len);
    return !equal;
}

// Main: run all tests and print summary
int main()
{
    runTest("test_NotEqualInt16Arrays_PrimaryCase", test_NotEqualInt16Arrays_PrimaryCase);
    runTest("test_EqualInt16Arrays_Identical", test_EqualInt16Arrays_Identical);
    runTest("test_NotEqualInt16Arrays_DifferentLast", test_NotEqualInt16Arrays_DifferentLast);
    runTest("test_NotEqualInt16Arrays_DifferentFirst", test_NotEqualInt16Arrays_DifferentFirst);
    runTest("test_ZeroLengthInt16Arrays_AreEqual", test_ZeroLengthInt16Arrays_AreEqual);
    runTest("test_NotEqualInt16Arrays_NegativeValues", test_NotEqualInt16Arrays_NegativeValues);

    // Summary
    cout << "\nTest suite finished. Total: " << gTotal << ", Passed: "
         << (gTotal - gFailed) << ", Failed: " << gFailed << endl;

    return gFailed == 0 ? 0 : 1;
}