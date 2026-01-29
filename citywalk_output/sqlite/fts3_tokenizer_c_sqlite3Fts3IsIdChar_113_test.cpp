/*
Step 1 - Program Understanding and Candidate Keywords
- Core function under test: sqlite3Fts3IsIdChar(char c)
- Key behaviors:
  - Uses a static 128-entry isFtsIdChar lookup table to classify ASCII chars 0x00..0x7F
  - Returns nonzero if either the high bit of c is set (c & 0x80) or isFtsIdChar[(int)c] is nonzero
  - Thus: all characters with high bit set are considered valid ID chars; otherwise, only those flagged in the table are valid
- Candidate Keywords (representing dependences and logic): isFtsIdChar, high-bit check (c & 0x80), 128-element mapping, ASCII subset (digits, letters, underscore, dollar, etc.), return semantics (int used as boolean), static-local state

Step 2 - Unit Test Generation (no GTest)
- Focus: create a small, self-contained C++11 test harness that exercises both true and false branches of the predicate
- Tests cover:
  - ASCII digits '0'..'9' -> true
  - Uppercase letters 'A'..'Z' -> true
  - Lowercase letters 'a'..'z' -> true
  - Underscore '_' (likely true) and dollar '$' (true via table)
  - Common punctuation like ':' and space ' ' -> false
  - High-bit values (e.g., 0x80 cast to char) -> true (high-bit path)
  - 0x7F (DEL) -> false (from table)
  - Dash '-' and '@' -> false (not in table)
- Approach: create a minimal test harness that does not terminate on failure but records failures and prints them. Call the focal function via extern "C" declaration to ensure proper linkage with the C symbol when linked with fts3_tokenizer.c.

Step 3 - Test Case Refinement (Domain knowledge applied)
- Use a compact, explicit test list to maximize coverage, including both low-byte ASCII cases and high-bit edge cases
- Use a lightweight, non-terminating assertion approach to maximize code execution and test coverage
- The tests do not rely on private/internal details beyond the public signature of sqlite3Fts3IsIdChar
- All tests are implemented in a single translation unit to ease compilation in environments without GTest/GMock

Now the test code (C++11, no GTest), with explanatory comments above each test block:

*/

#include <fts3Int.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <cstdint>


// Step 2/3: Declare the focal function with C linkage to allow linking with the C implementation.
extern "C" int sqlite3Fts3IsIdChar(char c);

/*
 Lightweight test harness:
 - Non-terminating assertions: do not abort on failure, but record and report
 - At the end, report total tests and failures
 - Provide per-test messages to aid debugging
*/
static int g_total_tests = 0;
static int g_failures = 0;

#define TEST_FAIL_MSG(msg) \
    do { std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << " - " << (msg) << "\n"; } while(0)

#define TEST_ASSERT_INT_EQ(actual, expected, desc) \
    do { \
        int _act = (int)(actual); \
        int _exp = (int)(expected); \
        ++g_total_tests; \
        if(_act != _exp) { \
            std::cerr << "Test failed: " << (desc) \
                      << " (actual=" << _act << ", expected=" << _exp << ")" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define TEST_ASSERT_TRUE(cond, desc) \
    TEST_ASSERT_INT_EQ((cond) ? 1 : 0, 1, (desc))

#define TEST_ASSERT_FALSE(cond, desc) \
    TEST_ASSERT_INT_EQ((cond) ? 1 : 0, 0, (desc))

// Step 2/3: Test suite for sqlite3Fts3IsIdChar
static void test_sqlite3Fts3IsIdChar_basic() {
    // Test a representative set of characters, covering both table-based and high-bit logic
    struct Case {
        char ch;
        int expected; // 1 for true, 0 for false
        const char* note;
    } cases[] = {
        // Basic ASCII in-table characters (digits, letters, underscore, dollar)
        {'A', 1, "uppercase ASCII letter should be ID char"},
        {'a', 1, "lowercase ASCII letter should be ID char"},
        {'0', 1, "digit zero should be ID char"},
        {'_', 1, "underscore should be ID char (likely)"},
        {'$', 1, "dollar sign should be ID char (likely)"},
        // In-table negatives
        {':', 0, "colon should NOT be an ID char"},
        {' ', 0, "space should NOT be an ID char"},
        {'-', 0, "dash should NOT be an ID char"},
        {'@', 0, "at sign should NOT be an ID char"},
        // Boundary: high-bit path
        static_cast<char>(0x80), 1, "high-bit character should be ID char (high-bit path)"},
        static_cast<char>(0x7F), 0, "DEL 0x7F should NOT be ID char (per table)"},
        // Additional check: lowercase z
        {'z', 1, "lowercase z should be ID char"},
        // Additional check: uppercase Z
        {'Z', 1, "uppercase Z should be ID char"},
    };

    const int nCases = sizeof(cases) / sizeof(cases[0]);
    for(int i = 0; i < nCases; ++i){
        int actual = sqlite3Fts3IsIdChar(cases[i].ch);
        int exp = cases[i].expected ? 1 : 0;
        // Use the note as the test description
        TEST_ASSERT_INT_EQ((actual != 0) ? 1 : 0, exp, cases[i].note);
    }
}

// Entry point
int main() {
    // Run the tests
    test_sqlite3Fts3IsIdChar_basic();

    // Summary
    std::cout << "Total tests run: " << g_total_tests << ", Failures: " << g_failures << std::endl;

    // Return nonzero if any test failed
    return (g_failures == 0) ? 0 : 1;
}