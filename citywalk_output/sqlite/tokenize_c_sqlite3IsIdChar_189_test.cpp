// Unit tests for sqlite3IsIdChar in tokenize.c
// This test suite is designed to be compiled with C++11 (no GTest) and
// links against the sqlite source that provides sqlite3IsIdChar.
// We use a lightweight, non-terminating assertion mechanism to maximize
// code coverage.

#include <stdlib.h>
#include <iomanip>
#include <keywordhash.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the C function under test.
// The function signature uses unsigned char (u8 alias in sqlite code).
extern "C" int sqlite3IsIdChar(unsigned char c);

// Lightweight test harness (non-terminating assertions)
static int g_total = 0;
static int g_failed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if(!(cond)) { \
        ++g_failed; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected true but got false: " #cond "\n"; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    ++g_total; \
    if(cond) { \
        ++g_failed; \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << " - Expected false but got true: " #cond "\n"; \
    } \
} while(0)

// Helper: wrap C function call for readability
static inline bool isIdChar(unsigned char c) {
    return sqlite3IsIdChar(c) != 0;
}

/*
Step 2: Unit Test Generation
We design tests to exercise the true/false branches of sqlite3IsIdChar.
Based on typical SQLite character class mappings (letters and underscore are valid
identifier characters; digits and most punctuation are not starting-identifier)
we implement the following test groups:

- Alphabetic characters (a-z, A-Z) should yield non-zero (true).
- Underscore '_' should yield non-zero (true) as an identifier character.
- Numeric digits '0'-'9' should yield zero (false) as starting identifier chars.
- Punctuation and whitespace should yield zero (false).
- Non-ASCII bytes (0x80, 0xFF) should yield zero (false) on ASCII-targeted
  builds.

Step 3: Test Case Refinement
Each test includes explanatory comments describing intent and expected behavior to
aid maintenance and future coverage improvements.
*/

// Test 1: Alphabetic characters should be valid identifier characters
// Rationale: In SQL, identifiers commonly start with a letter.
// We expect 'a' and 'Z' to be accepted by sqlite3IsIdChar.
void test_IsIdChar_AlphabeticCharacters() {
    // Lowercase alphabet
    EXPECT_TRUE(isIdChar('a'));
    EXPECT_TRUE(isIdChar('m'));
    // Uppercase alphabet
    EXPECT_TRUE(isIdChar('A'));
    EXPECT_TRUE(isIdChar('Z'));
}

// Test 2: Underscore is typically allowed in identifiers
// Rationale: SQLite allows underscores in identifiers; this should be treated as an IdChar.
void test_IsIdChar_Underscore() {
    EXPECT_TRUE(isIdChar('_'));
}

// Test 3: Digits should not be considered identifier characters for starting tokens
// Rationale: A leading digit typically starts a numeric literal, not an identifier.
// We expect digits to be not IdChar.
void test_IsIdChar_DigitsNotAllowed() {
    EXPECT_FALSE(isIdChar('0'));
    EXPECT_FALSE(isIdChar('5'));
    EXPECT_FALSE(isIdChar('9'));
}

// Test 4: Punctuation and whitespace should not be IdChar
// Rationale: Tokens like spaces and punctuation delimit tokens, not part of identifiers.
void test_IsIdChar_PunctuationAndSpace() {
    EXPECT_FALSE(isIdChar('!'));
    EXPECT_FALSE(isIdChar(' ')); // space
    EXPECT_FALSE(isIdChar(','));
    EXPECT_FALSE(isIdChar('.'));
    EXPECT_FALSE(isIdChar('\t'));
}

// Test 5: Non-ASCII bytes should generally not be treated as IdChar in ASCII mode
// Rationale: In ASCII-mode builds, non-ASCII values are not valid identifier characters.
void test_IsIdChar_NonASCII() {
    EXPECT_FALSE(isIdChar(0x80)); // 128
    EXPECT_FALSE(isIdChar(0xFF)); // 255
}

// Test 6: Boundary check for low-values (NUL character should not be IdChar)
void test_IsIdChar_NUL() {
    EXPECT_FALSE(isIdChar('\0'));
}

// Main runner: execute all tests and print a summary.
// The program returns a non-zero exit code if any test failed.
int main() {
    // Run tests with explanatory comments above
    test_IsIdChar_AlphabeticCharacters();
    test_IsIdChar_Underscore();
    test_IsIdChar_DigitsNotAllowed();
    test_IsIdChar_PunctuationAndSpace();
    test_IsIdChar_NonASCII();
    test_IsIdChar_NUL();

    std::cout << std::dec;
    std::cout << "sqlite3IsIdChar test results: "
              << g_total << " checks, "
              << g_failed << " failures." << std::endl;

    return (g_failed != 0) ? 1 : 0;
}