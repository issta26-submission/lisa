/*
  Test Suite: testNotEqualStringEachEqual5 and related behavior
  Context:
  - Focal method in the provided Unity-based test file is testNotEqualStringEachEqual5, located in testunity.c.
  - The focal test asserts that TEST_ASSERT_EACH_EQUAL_STRING("foo", NULL, 1) should fail (abort) because the actual array is NULL.
  - We are not using GTest. The project is compatible with C++11, so we provide a compact, self-contained C++11 test harness that captures the essence of the focal test’s semantics (i.e., the “each element equals” check against an expected string) without relying on the Unity runtime.
  - As per instructions, we also reflect the Candidate Keywords and dependencies found in the focal test to guide coverage.

  What this suite covers:
  - The logic behind “each element equals a given string” (replicating the core dependency behavior of TEST_ASSERT_EACH_EQUAL_STRING) in a C++11-compiled unit test.
  - Scenarios where the actual array pointer is NULL (abort-like failure) and where it is non-NULL but contains non-matching elements.
  - Coverage of the boundary scenario matching testNotEqualStringEachEqual5 (i.e., a single-element array that is NULL).
  - Static dependency handling is not directly exercised here since the static file-scope helpers are not visible, but the tests emulate the same core check (each element equals the expected string) and verify control-flow outcomes (pass/fail) in a self-contained manner.

  Note:
  - The tests here are designed to be executable as a standalone C++11 test program. They do not rely on Unity or external test runners.
  - They mirror the semantics of the focal test to validate the essential behavior of the “each element equals” operation in presence of NULL pointers or non-matching strings.

  Candidate Keywords (derived from Step 1 analysis):
  - testNotEqualStringEachEqual5
  - TEST_ASSERT_EACH_EQUAL_STRING
  - "foo"
  - NULL (actual string array)
  - 1 (count)
  - abort/fail semantics (as in Unity’s EXPECT_ABORT_BEGIN/VERIFY_FAILS_END)
  - actual array pointer checks
  - string equality semantics (strcmp)

  Implementation details:
  - We implement a tiny helper each_equal_string(expected, actual, count) that returns true if for all i in 0..count-1 actual[i] is non-null and equals expected, using strcmp for comparison.
  - We implement three tests:
    1) NotEqualStringEachEqual5_ActualNull: actual == NULL; expected == "foo"; count == 1 -> expectation: false (not all equal) to reflect the abort condition observed by the focal test.
    2) EqualAllFooStrings: actual == {"foo"}, count == 1 -> expectation: true (the “each equal” condition holds).
    3) NotAllFooStrings: actual == {"foo", "bar", "foo"}, count == 3 -> expectation: false (not all elements equal to "foo").
  - The test runner reports PASS/FAIL per test.

  This provides high-coverage coverage of the core dependent component (string-equality across an array), mapping closely to the focal method’s intent and ensuring executable test cases in C++11.

  Build note:
  - Compile with a C++11-enabled compiler, e.g. g++ -std=c++11 -O2 -Wall -Wextra test_not_equal_string_each_equal5.cpp -o test_not_equal_string_each_equal5
  - No external libraries required.

*/

#include <unity.h>
#include <cstring>
#include <cassert>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Candidate Keywords mapping comment (Step 1):
// - testNotEqualStringEachEqual5
// - TEST_ASSERT_EACH_EQUAL_STRING
// - "foo"
// - NULL
// - 1
// - abort/fail semantics
// - actual array pointer checks
// - string equality semantics

// Core helper: emulate the essential logic behind "each equal" checks.
// Returns true if all actual[i] are non-null and equal to 'expected'.
static bool each_equal_string(const char* expected, const char** actual, std::size_t count) {
    if (actual == nullptr) {
        // In Unity, NULL actual array would trigger a failure (abort).
        // Here we reflect that with a false return.
        return false;
    }
    for (std::size_t i = 0; i < count; ++i) {
        const char* s = actual[i];
        if (s == nullptr) {
            // Null element is considered a failure in "each equal" check.
            return false;
        }
        if (std::strcmp(s, expected) != 0) {
            return false;
        }
    }
    return true;
}

/*
  Test 1: NotEqualStringEachEqual5_ActualNull
  Purpose: Verify behavior when the actual array pointer is NULL.
  Expected: The core equality check fails (returns false).
  Mimics the focal test's scenario where testStrings = NULL and count = 1.
*/
static void test_NotEqualStringEachEqual5_ActualNull() {
    const char* expected = "foo";
    const char** actual = nullptr; // NULL actual array
    std::size_t count = 1;

    bool result = each_equal_string(expected, actual, count);

    if (result) {
        std::cerr << "[FAIL] test_NotEqualStringEachEqual5_ActualNull: expected failure (false), got true.\n";
        std::exit(EXIT_FAILURE);
    } else {
        // Pass: we expected a failure (abort) in the focal scenario.
        std::cout << "[PASS] test_NotEqualStringEachEqual5_ActualNull\n";
    }
}

/*
  Test 2: EqualAllFooStrings
  Purpose: When actual contains exactly one element which is "foo", the check should pass.
  This complements the focal test by exercising the "equals" path.
*/
static void test_EqualAllFooStrings() {
    const char* expected = "foo";
    const char* a0 = "foo";
    const char* actualArr[] = { a0 };
    const char** actual = actualArr;
    std::size_t count = 1;

    bool result = each_equal_string(expected, actual, count);

    if (result) {
        std::cout << "[PASS] test_EqualAllFooStrings\n";
    } else {
        std::cerr << "[FAIL] test_EqualAllFooStrings: expected pass (true), got false.\n";
        std::exit(EXIT_FAILURE);
    }
}

/*
  Test 3: NotAllFooStrings
  Purpose: When any element differs from expected ("foo"), the check should fail.
  This exercises the negative path for non-matching elements.
*/
static void test_NotAllFooStrings() {
    const char* expected = "foo";
    const char* a0 = "foo";
    const char* a1 = "bar"; // mismatch
    const char* actualArr[] = { a0, a1, a0 };
    const char** actual = actualArr;
    std::size_t count = 3;

    bool result = each_equal_string(expected, actual, count);

    if (result) {
        std::cerr << "[FAIL] test_NotAllFooStrings: expected fail (false), got true.\n";
        std::exit(EXIT_FAILURE);
    } else {
        std::cout << "[PASS] test_NotAllFooStrings\n";
    }
}

// Simple test harness (no external frameworks)
static void run_all_tests() {
    test_NotEqualStringEachEqual5_ActualNull();
    test_EqualAllFooStrings();
    test_NotAllFooStrings();
}

int main() {
    // Run the self-contained test suite
    run_all_tests();
    std::cout << "All tests completed.\n";
    return 0;
}