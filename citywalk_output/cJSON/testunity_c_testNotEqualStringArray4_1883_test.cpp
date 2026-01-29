/*
Step 1 – Program Understanding (Candidate Keywords)
- testStrings: the actual string array being tested
- expStrings: the expected string array to compare against
- 4: the length of the arrays in the test
- TEST_ASSERT_EQUAL_STRING_ARRAY: the Unity assertion style being emulated
- NULL: used as a sentinel/NULL element within expStrings or testStrings
- testNotEqualStringArray4: focal test illustrating a failure when a NULL is present in expected vs actual mismatch
- equal_string_array: core dependent function implemented for unit testing outside Unity
- Strings comparison relies on strcmp for content equality when non-NULL
*/

/*
Step 2 & 3 – Unit Test Generation and Refinement (C++11, no GTest)
We implement a lightweight, non-terminating test harness to verify the behavior of a
string-array equality checker that mirrors the intent of TEST_ASSERT_EQUAL_STRING_ARRAY.
We focus on reproducing the scenario of testNotEqualStringArray4 from testunity.c and
add supplementary tests to ensure broader coverage of the function’s behavior.
*/

#include <cstddef>
#include <unity.h>
#include <cstring>
#include <iostream>
#include <functional>
#include <vector>
#include <string.h>
#include <string>
#include <stdint.h>
#include <stdio.h>


// Core function under test: compares two arrays of C-strings for equality up to 'len' elements.
static bool equal_string_array(const char* expected[], const char* actual[], size_t len)
{
    for (size_t i = 0; i < len; ++i)
    {
        const char* e = (expected == nullptr) ? nullptr : expected[i];
        const char* a = (actual   == nullptr) ? nullptr : actual[i];

        if (e == a) {
            // both NULL or same pointer, consider equal at this index
            continue;
        }
        if (e == nullptr || a == nullptr) {
            // one is NULL and the other isn't
            return false;
        }
        if (std::strcmp(e, a) != 0) {
            // different content
            return false;
        }
    }
    return true;
}

// Lightweight test harness (non-terminating assertions)
static std::string g_currentTestName;
static int g_failuresInCurrentTest = 0;
static int g_totalFailures = 0;

static void logFailure(const std::string& message)
{
    ++g_failuresInCurrentTest;
    ++g_totalFailures;
    std::cout << "[FAIL] " << g_currentTestName << ": " << message << "\n";
}

#define EXPECT_TRUE(cond, msg) do { if(!(cond)) logFailure(msg); } while(0)
#define EXPECT_FALSE(cond, msg) do { if(cond)  logFailure(msg); } while(0)

static void reportTestResult(const std::string& testName)
{
    if (g_failuresInCurrentTest == 0)
        std::cout << "[PASS] " << testName << "\n";
    else
        std::cout << "[RESULT] " << testName << " - Failures: " << g_failuresInCurrentTest << "\n";

    // Prepare for next test
    g_failuresInCurrentTest = 0;
}

// Step 2: Focal test variants (mirroring testNotEqualStringArray4 and additional coverage)
void testNotEqualStringArray4_CPP() {
    g_currentTestName = "testNotEqualStringArray4_CPP";

    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", NULL,  "woo", "moo" };

    bool equal = equal_string_array(expStrings, testStrings, 4);

    // Expecting failure (not equal)
    EXPECT_FALSE(equal, "Arrays should NOT be equal due to NULL in expected[1]");
    reportTestResult(g_currentTestName);
}

void testEqualStringArray_CPP() {
    g_currentTestName = "testEqualStringArray_CPP";

    const char *testStrings[] = { "foo", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", "boo", "woo", "moo" };

    bool equal = equal_string_array(expStrings, testStrings, 4);
    EXPECT_TRUE(equal, "Arrays should be equal for exact same content");
    reportTestResult(g_currentTestName);
}

void testNotEqualStringArray4_NULLActual_AllNulls(void) {
    g_currentTestName = "testNotEqualStringArray4_NULLActual_AllNulls";

    const char *testStrings[] = { NULL, NULL, NULL, NULL };
    const char *expStrings[]  = { NULL, NULL, NULL, NULL };

    bool equal = equal_string_array(expStrings, testStrings, 4);
    EXPECT_TRUE(equal, "Both arrays are all NULLs, should be considered equal");
    reportTestResult(g_currentTestName);
}

void testNotEqualStringArray4_NullMismatchAgainstNonNull(void) {
    g_currentTestName = "testNotEqualStringArray4_NullMismatchAgainstNonNull";

    const char *testStrings[] = { "bar", "boo", "woo", "moo" };
    const char *expStrings[]  = { "foo", NULL,  "woo", "moo" };

    bool equal = equal_string_array(expStrings, testStrings, 4);
    EXPECT_FALSE(equal, "Mismatch due to NULL vs non-NULL at index 0 or 1");
    reportTestResult(g_currentTestName);
}

void testNotEqualStringArray4_LengthZero(void) {
    g_currentTestName = "testNotEqualStringArray4_LengthZero";

    const char *testStrings[] = { "foo" };
    const char *expStrings[]  = { NULL }; // content irrelevant for length 0

    bool equal = equal_string_array(expStrings, testStrings, 0);
    EXPECT_TRUE(equal, "Length zero should yield equality (no elements compared)");
    reportTestResult(g_currentTestName);
}

// Step 3 – Main to run the tests (as per requirement to call tests from main when no test framework is used)
int main() {
    std::cout << "Starting lightweight C++11 test suite for NOT_EQUAL_STRING_ARRAY4 scenario...\n";

    testNotEqualStringArray4_CPP();
    testEqualStringArray_CPP();
    testNotEqualStringArray4_NULLActual_AllNulls();
    testNotEqualStringArray4_NullMismatchAgainstNonNull();
    testNotEqualStringArray4_LengthZero();

    std::cout << "Total failures across all tests: " << g_totalFailures << "\n";
    return (g_totalFailures == 0) ? 0 : 1;
}