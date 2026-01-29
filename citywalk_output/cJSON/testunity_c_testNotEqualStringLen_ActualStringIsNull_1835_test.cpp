#include <unity.h>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Minimal, self-contained test harness (no external frameworks like GTest/GMock).
// This harness is designed to simulate a tiny subset of Unity's behaviour
// focusing on the focal scenario: when the "actual" string pointer is NULL
// in an equality-with-length check (TEST_ASSERT_EQUAL_STRING_LEN).
//
// The goal is to validate that the function under test correctly handles
// the NULL ActualString scenario by failing the comparison, as per the
// focal test: testNotEqualStringLen_ActualStringIsNull in testunity.c.

// Simple assertion function emulating Unity's behavior for equality check
// of two strings up to a given length. Returns true if equal, false if not.
// Important: If either pointer is NULL, this returns false to indicate a failure.
static bool TEST_ASSERT_EQUAL_STRING_LEN(const char* expected, const char* actual, size_t len)
{
    if (expected == nullptr || actual == nullptr) {
        return false;
    }
    for (size_t i = 0; i < len; ++i) {
        char ce = expected[i];
        char ca = actual[i];
        if (ce != ca) {
            return false;
        }
        // If end of string reached in expected, no further chars matter.
        if (ce == '\0') {
            break;
        }
    }
    return true;
}

// Forward declarations of test scenarios (each returns whether the test passes).
// The focal test lives in testunity.c and checks that asserting equality with
// an ActualString that is NULL should fail. We mimic that behavior here.

static bool testNotEqualStringLen_ActualStringIsNull_variant1();
static bool testNotEqualStringLen_ActualStringIsNull_variant2();
static bool testNotEqualStringLen_ActualStringIsNull_variant3();

// Variant 1:
// Domain knowledge: vary the expected string length predicate with a non-empty
// expected string and a NULL actual string. This should fail the comparison.
static bool testNotEqualStringLen_ActualStringIsNull_variant1()
{
    // <FOCAL_METHOD> TEST_ASSERT_EQUAL_STRING_LEN("foo", NULL, 1); </FOCAL_METHOD>
    // Expected: Failure because actual is NULL.
    const char* expected = "foo";
    const char* actual = nullptr;
    size_t len = 1;

    bool result = TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len);
    // The focal test expects a failure (i.e., result == false).
    return !result;
}

// Variant 2:
// Extend the scenario with a longer expected string and a NULL actual string.
// This further exercises the NULL handling path across a different length.
static bool testNotEqualStringLen_ActualStringIsNull_variant2()
{
    const char* expected = "foobar";
    const char* actual = nullptr;
    size_t len = 6;

    bool result = TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len);
    return !result;
}

// Variant 3:
// Edge case: empty expected string with a NULL actual string and len 0.
// This checks the len==0 path with NULL actual to ensure consistent failure.
static bool testNotEqualStringLen_ActualStringIsNull_variant3()
{
    const char* expected = "";
    const char* actual = nullptr;
    size_t len = 0;

    bool result = TEST_ASSERT_EQUAL_STRING_LEN(expected, actual, len);
    return !result;
}

int main()
{
    // Collect tests (name, function returning pass/fail).
    using TestFn = std::function<bool()>;
    std::vector<std::pair<std::string, TestFn>> tests;

    tests.emplace_back(
        "testNotEqualStringLen_ActualStringIsNull_variant1",
        []() -> bool { return testNotEqualStringLen_ActualStringIsNull_variant1(); });

    tests.emplace_back(
        "testNotEqualStringLen_ActualStringIsNull_variant2",
        []() -> bool { return testNotEqualStringLen_ActualStringIsNull_variant2(); });

    tests.emplace_back(
        "testNotEqualStringLen_ActualStringIsNull_variant3",
        []() -> bool { return testNotEqualStringLen_ActualStringIsNull_variant3(); });

    // Run tests and report results.
    int total = 0;
    int passed = 0;
    for (const auto& t : tests) {
        ++total;
        bool testPass = t.second();
        if (testPass) {
            ++passed;
            std::cout << "[PASS] " << t.first << std::endl;
        } else {
            std::cout << "[FAIL] " << t.first
                      << " (expected failure in focal assertion)" << std::endl;
        }
    }

    std::cout << "\nTest results: " << passed << " / " << total << " passed." << std::endl;
    // Return non-zero if any test failed to satisfy test suite convention.
    return (passed == total) ? 0 : 1;
}