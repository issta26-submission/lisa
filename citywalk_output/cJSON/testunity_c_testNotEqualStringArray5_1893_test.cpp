// Lightweight C++11 test harness for the focal Unity-based testNotEqualStringArray5 scenario.
// This harness recreates a minimal non-terminating assertion mechanism to exercise
// TEST_ASSERT_EQUAL_STRING_ARRAY-like behavior without pulling in the Unity framework.
// The goal is to validate the specific behavior of comparing a NULL actual strings array
// against an expected array of strings, ensuring the comparison reports failure as in the
// original focal test.
//
// Notes:
// - We implement a small UnityAssertEqualStringArray-like function to compare arrays.
// - We provide three unit tests focusing on the focal method's intention:
//   1) Should fail when actual is NULL (as in the provided focal test).
//   2) Should pass when actual matches the expected strings.
//   3) Should fail when there is any mismatch between corresponding elements.
// - The tests are registered and executed by a tiny test harness. Outputs indicate PASS/FAIL.

#include <unity.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Lightweight replacement for Unity's TEST_ASSERT_EQUAL_STRING_ARRAY.
// Returns true if arrays are equal for the given length; false otherwise.
static bool UnityAssertEqualStringArray(const char* const* expected, const char* const* actual, size_t length)
{
    if (actual == nullptr) {
        // Actual array is NULL -> failure (as per focal test expectation)
        return false;
    }
    for (size_t i = 0; i < length; ++i)
    {
        const char* exp = expected[i];
        const char* act = actual[i];
        // If one is NULL and the other isn't, they are not equal
        if ((exp == nullptr) != (act == nullptr)) {
            return false;
        }
        // If both non-NULL, compare strings
        if (exp != nullptr && act != nullptr)
        {
            if (std::strcmp(exp, act) != 0) {
                return false;
            }
        }
        // If both NULL, consider equal for this position (aligns with Unity semantics for NULL entries)
    }
    return true;
}

// Simple test result tracking
static int gTotalFailures = 0;
static void testFail(const std::string& testName, const std::string& reason)
{
    std::cerr << "[FAIL] " << testName << ": " << reason << std::endl;
    ++gTotalFailures;
}
static void testPass(const std::string& testName)
{
    std::cout << "[PASS] " << testName << std::endl;
}

// Minimal test framework
struct TestCase {
    const char* name;
    void (*fn)();
};
static std::vector<TestCase> gTests;

static void RegisterTest(const char* name, void (*fn)())
{
    gTests.push_back({name, fn});
}
static void RunAllTests()
{
    for (auto &t : gTests) {
        std::cout << "[RUN] " << t.name << std::endl;
        t.fn();
    }
}

// FOCAL TESTS (adapted to non-terminating, C++11 harness)

// Test 1: The focal test asserts that TEST_ASSERT_EQUAL_STRING_ARRAY(expStrings, testStrings, 4) should FAIL
// when testStrings is NULL. This should be a failed assertion in Unity, which in turn means
// our test should treat it as PASS when the assertion reports failure (i.e., returns false).
static void testNotEqualStringArray5_ShouldFail()
{
    // Arrange: testStrings is NULL, expStrings has 4 elements
    const char **testStrings = NULL;
    const char *expStrings[] = { "foo", "boo", "woo", "zoo" };

    // Act: simulate Unity's assertion
    bool result = UnityAssertEqualStringArray(expStrings, testStrings, 4);

    // Assert: the assertion should fail (result == false)
    if (result)
    {
        testFail("testNotEqualStringArray5_ShouldFail", "Expected assertion to fail but it passed.");
    }
    else
    {
        testPass("testNotEqualStringArray5_ShouldFail");
    }
}

// Test 2: Positive case - actual array exactly matches expected array; should pass.
static void testNotEqualStringArray5_ShouldPass()
{
    // Arrange: actual equals expected
    const char *testStrings[] = { "foo", "boo", "woo", "zoo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };

    // Act
    bool result = UnityAssertEqualStringArray(expStrings, testStrings, 4);

    // Assert: should pass (result == true)
    if (result)
    {
        testPass("testNotEqualStringArray5_ShouldPass");
    }
    else
    {
        testFail("testNotEqualStringArray5_ShouldPass", "Expected assertion to pass but it failed.");
    }
}

// Test 3: Mismatch case - one element differs; should fail.
static void testNotEqualStringArray5_ShouldFailMismatch()
{
    // Arrange: last element differs ("baz" vs "zoo")
    const char *testStrings[] = { "foo", "boo", "woo", "baz" };
    const char *expStrings[]  = { "foo", "boo", "woo", "zoo" };

    // Act
    bool result = UnityAssertEqualStringArray(expStrings, testStrings, 4);

    // Assert: should fail
    if (result)
    {
        testFail("testNotEqualStringArray5_ShouldFailMismatch", "Expected assertion to fail but it passed.");
    }
    else
    {
        testPass("testNotEqualStringArray5_ShouldFailMismatch");
    }
}

// Main
int main()
{
    // Register tests
    RegisterTest("NotEqualStringArray5_ShouldFail", testNotEqualStringArray5_ShouldFail);
    RegisterTest("NotEqualStringArray5_ShouldPass", testNotEqualStringArray5_ShouldPass);
    RegisterTest("NotEqualStringArray5_ShouldFailMismatch", testNotEqualStringArray5_ShouldFailMismatch);

    // Run
    RunAllTests();

    // Summary
    if (gTotalFailures == 0) {
        std::cout << "[SUMMARY] All tests passed." << std::endl;
    } else {
        std::cout << "[SUMMARY] Total failures: " << gTotalFailures << std::endl;
    }

    return gTotalFailures;
}