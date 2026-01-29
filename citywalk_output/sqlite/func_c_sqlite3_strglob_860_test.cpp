// Lightweight C++11 unit tests for sqlite3_strglob (no Google Test, no GMock)
// This test harness calls the C function sqlite3_strglob and verifies its behavior
// across key branches described in the focal method.
//
// Notes:
// - We declare the function with C linkage to call from C++.
// - Tests are non-terminating: failures are logged, but do not abort execution.
// - Explanatory comments accompany each unit test to document intent and expected behavior.

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Forward declaration of the C function under test.
// The actual implementation (along with its static dependencies) is expected
// to be provided in the build (e.g., in func.c as shown in the focal class).
extern "C" int sqlite3_strglob(const char *zGlobPattern, const char *zString);

// Simple non-terminating assertion macro suitable for this lightweight tester
// It reports failures but allows subsequent tests to run.
static int g_sqlite_strglob_failures = 0;
#define EXPECT_EQ(actual, expected, description) do { \
    auto _actual = (actual); \
    auto _expected = (expected); \
    if(_actual != _expected) { \
        std::cerr << "FAILED: " << (description) \
                  << " | expected: " << _expected \
                  << ", actual: " << _actual << std::endl; \
        ++g_sqlite_strglob_failures; \
    } \
} while(0)

// -------------- Unit Tests for sqlite3_strglob --------------
// Each test is designed to cover a specific branch or behavior of the focal method.

// Test 1: zString == NULL and zGlobPattern != NULL
// This should return 1 according to: if (zString==0) return zGlobPattern!=0;
static void test_null_zString_with_nonnull_pattern() {
    // true branch: zString is NULL, zGlobPattern is non-NULL
    int res = sqlite3_strglob("pattern", nullptr);
    EXPECT_EQ(res, 1, "zString==NULL with non-null zGlobPattern should return 1");
}

// Test 2: zString == NULL and zGlobPattern == NULL
// This should return 0 (false) according to the same branch logic.
static void test_both_null_strings_and_patterns() {
    int res = sqlite3_strglob(nullptr, nullptr);
    EXPECT_EQ(res, 0, "Both zString and zGlobPattern NULL should return 0");
}

// Test 3: zString != NULL and zGlobPattern == NULL
// This should return 1 according to: else if (zGlobPattern==0) { return 1; }
static void test_null_pattern_with_nonnull_string() {
    int res = sqlite3_strglob(nullptr, "some string");
    EXPECT_EQ(res, 1, "zGlobPattern==NULL with non-null zString should return 1");
}

// Test 4: Both non-null with a simple wildcard pattern that should match
// "*" should match any string, so expect non-zero (true).
static void test_star_pattern_matches_any_string() {
    int res = sqlite3_strglob("*", "abc");
    EXPECT_EQ(res, 1, "Pattern '*' should match any string (e.g., 'abc')");
}

// Test 5: Basic non-match scenario with exact strings
static void test_exact_non_match() {
    int res = sqlite3_strglob("abc", "def");
    EXPECT_EQ(res, 0, "Pattern 'abc' should not match 'def'");
}

// Test 6: Single-character wildcard '?' matching
static void test_question_mark_matches_single_character() {
    int res = sqlite3_strglob("a?c", "abc");
    EXPECT_EQ(res, 1, "Pattern 'a?c' should match 'abc' where '?' matches one char");
}

// Test 7: Character class matching with simple class [bc]
static void test_character_class_matching() {
    int res1 = sqlite3_strglob("a[bc]d", "abd"); // 'b' matches [bc]
    EXPECT_EQ(res1, 1, "Pattern 'a[bc]d' should match 'abd' where [bc] matches 'b'");

    int res2 = sqlite3_strglob("a[bc]d", "aad"); // 'a' does not match [bc]
    EXPECT_EQ(res2, 0, "Pattern 'a[bc]d' should not match 'aad' since [bc] != 'a'");
}

// -------------- Test Runner --------------
int main() {
    // Run all test cases
    test_null_zString_with_nonnull_pattern();
    test_both_null_strings_and_patterns();
    test_null_pattern_with_nonnull_string();
    test_star_pattern_matches_any_string();
    test_exact_non_match();
    test_question_mark_matches_single_character();
    test_character_class_matching();

    // Summary
    if(g_sqlite_strglob_failures == 0) {
        std::cout << "All sqlite3_strglob tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_sqlite_strglob_failures << " sqlite3_strglob test(s) failed." << std::endl;
        return 1;
    }
}