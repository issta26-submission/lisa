// C++11 test harness for the focal C function: testArgSelectX
// This test suite exercises various scenarios of the argument-selector
// implemented in lsmtest_util.c, focusing on coverage of all branches.
//
// Notes:
// - We declare the C function with extern "C" to ensure proper linkage from C++.
// - The test data layout mirrors the internal struct Entry { const char *zName; } used by
//   testArgSelectX. Each test builds an array of Entry with a null zName sentinel.
// - The test cases cover: no matches, exact match, single longer match, multiple matches,
//   and exact match located after other potential matches (ensures correct iOut handling).
// - We only rely on the C standard library and the provided function under test (no GTest).

#include <string.h>
#include <lsmtest.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the C function under test.
// It is implemented in lsmtest_util.c and linked during build.
extern "C" int testArgSelectX(void *aData, const char *zType, int sz, const char *zArg, int *piOut);

// Helper macro for simple test result reporting
#define REPORT_RESULT(testName, passed) \
    do { \
        totalTests++; \
        if (passed) { \
            ++passedTests; \
            std::cout << "[PASS] " << testName << "\n"; \
        } else { \
            std::cout << "[FAIL] " << testName << "\n"; \
        } \
    } while (0)

int main() {
    int totalTests = 0;
    int passedTests = 0;

    // Local mirror of the expected Entry layout used by testArgSelectX
    struct Entry { const char *zName; };

    // Common zType to pass to the function (the value itself is not verified by tests)
    const char *zType = "ENTRY_TYPE";

    // Test Case 1: No matches found (zArg does not prefix any zName)
    // Expected: function returns non-zero (1) and piOut remains unchanged.
    {
        const Entry data[] = {
            { "apple" },
            { "banana" },
            { nullptr } // sentinel
        };
        int piOut = -123456789; // sentinel to detect unintended writes
        int res = testArgSelectX((void*)data, zType, (int)sizeof(Entry), "car", &piOut);

        bool pass = (res == 1) && (piOut == -123456789);
        REPORT_RESULT("Test 1: No matches -> return 1, no piOut assignment", pass);
    }

    // Test Case 2: Exact single match (zArg exactly matches one zName)
    // Expected: function returns 0 and piOut equals the index of the exact match (0)
    {
        const Entry data[] = {
            { "foo" },
            { nullptr }
        };
        int piOut = -1;
        int res = testArgSelectX((void*)data, zType, (int)sizeof(Entry), "foo", &piOut);

        bool pass = (res == 0) && (piOut == 0);
        REPORT_RESULT("Test 2: Exact single match -> return 0, piOut == 0", pass);
    }

    // Test Case 3: One longer match (zArg prefixes a single zName longer than zArg)
    // Expected: function returns 0 and piOut equals the index of that single (longer) match (0)
    {
        const Entry data[] = {
            { "foobar" }, // length > strlen("foo")
            { nullptr }
        };
        int piOut = -1;
        int res = testArgSelectX((void*)data, zType, (int)sizeof(Entry), "foo", &piOut);

        bool pass = (res == 0) && (piOut == 0);
        REPORT_RESULT("Test 3: One longer match -> return 0, piOut == 0", pass);
    }

    // Test Case 4: Multiple matches (two zName entries share the zArg prefix)
    // Expected: function returns non-zero (1) and piOut is not written (remains sentinel)
    {
        const Entry data[] = {
            { "abcde" },
            { "abcef" },
            { nullptr }
        };
        int piOut = -9999;
        int res = testArgSelectX((void*)data, zType, (int)sizeof(Entry), "abc", &piOut);

        bool pass = (res == 1) && (piOut == -9999);
        REPORT_RESULT("Test 4: Multiple matches -> return 1, piOut unchanged", pass);
    }

    // Test Case 5: Exact match after a preceding non-exact potential match
    // Data: first entry does not match, second entry matches exactly
    // Expected: function returns 0 and piOut equals index of exact match (1)
    {
        const Entry data[] = {
            { "abx" },   // does not match "abc"
            { "abc" },   // exact match at index 1
            { nullptr }
        };
        int piOut = -1;
        int res = testArgSelectX((void*)data, zType, (int)sizeof(Entry), "abc", &piOut);

        bool pass = (res == 0) && (piOut == 1);
        REPORT_RESULT("Test 5: Exact match after non-matching entry -> return 0, piOut == 1", pass);
    }

    // Summary
    std::cout << "\nTest Summary: " << passedTests << " / " << totalTests << " tests passed.\n";

    return (passedTests == totalTests) ? 0 : 1;
}