/*
Step 1: Program Understanding
- Focused method: sqlite3_str_length(sqlite3_str *p)
- Core logic: returns p ? p->nChar : 0
- Dependent components: sqlite3_str structure (nChar field) and Null-pointer handling
- Key {Candidate Keywords}: sqlite3_str, nChar, null check, return value, pointer handling

Step 2: Unit Test Generation
- Tests cover true and false branches of the condition p ? p->nChar : 0
- Test scenarios:
  1) p is null -> expect 0
  2) p is non-null with a positive nChar -> expect that exact value
  3) p is non-null with nChar = 0 -> expect 0
- Static helpers internal to the file are not touched; tests interact only via the public (C) API
- Static members: test only uses sqlite3_str_length with the real sqlite3_str type provided by sqliteInt.h
- Assertions: non-terminating EXPECT_ style to maximize code execution

Step 3: Test Case Refinement
- Use C++11 compatible test harness (no GTest)
- Implement lightweight EXPECT_EQ macro that logs failures but does not abort
- Access sqlite3_str via the real type defined in sqliteInt.h to ensure correct memory layout
- Main function invokes test cases directly as required (no test framework dependence)
*/

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Import SQLite internal definitions. These headers provide the sqlite3_str type
// and the declaration of sqlite3_str_length. The tests are written in C++
// but link against the C implementation of sqlite3_str_length.
extern "C" {
}

// Lightweight non-terminating test assertions to maximize coverage while continuing
static int g_failures = 0;

#define EXPECT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " - expected: " << (expected) << ", actual: " << (actual) << "\n"; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
                  << " - condition is false: " #cond "\n"; \
        ++g_failures; \
    } \
} while (0)

// Test 1: Null pointer should yield 0
static void test_sqlite3_str_length_null_pointer() {
    // When p is NULL, function should return 0
    int result = sqlite3_str_length(nullptr);
    EXPECT_EQ(0, result);
}

// Test 2: Non-null pointer with positive nChar
static void test_sqlite3_str_length_non_null_positive() {
    sqlite3_str s;
    s.nChar = 5;
    int result = sqlite3_str_length(&s);
    EXPECT_EQ(5, result);
}

// Test 3: Non-null pointer with zero nChar
static void test_sqlite3_str_length_non_null_zero() {
    sqlite3_str s;
    s.nChar = 0;
    int result = sqlite3_str_length(&s);
    EXPECT_EQ(0, result);
}

int main() {
    // Run tests
    test_sqlite3_str_length_null_pointer();
    test_sqlite3_str_length_non_null_positive();
    test_sqlite3_str_length_non_null_zero();

    if (g_failures == 0) {
        std::cout << "ALL tests PASSED for sqlite3_str_length\n";
        return 0;
    } else {
        std::cerr << g_failures << " test failure(s) detected for sqlite3_str_length\n";
        return g_failures;
    }
}