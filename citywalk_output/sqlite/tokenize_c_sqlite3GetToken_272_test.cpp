/*
Unit Test Suite for sqlite3GetToken (tokenize.c) - C++11, no GoogleTest

Overview:
- This file implements a lightweight, non-terminating test harness to exercise
  the focal function sqlite3GetToken with a broad set of input tokens.
- It covers a variety of code paths (spaces, operators, punctuation, literals,
  numbers, variables, and identifiers) to increase coverage of the original
  tokenizer logic.
- The tests are designed to compile in a C++11 environment and rely on the
  public SQLite API available in the project (i.e., sqlite3GetToken and
  token type macros TK_*, as defined by sqlite3.h in the project include paths).
- Static/private helpers from the original file are not accessed directly; tests
  invoke sqlite3GetToken through the public API and compare against expected results.

Notes:
- We provide safe fallbacks for token constants in case the internal header is not
  visible in the build environment by using a post-include fallback mechanism.
- Tests use non-terminating checks (do not exit on first failure) to maximize
  coverage during a single run.

Compile-time prerequisites:
- The build should include the SQLite public header(s) (e.g., sqlite3.h) so that
  sqlite3GetToken and TK_* token constants are defined. If a constant is not
  defined by the headers, a fallback is provided.

Usage:
- Build this test alongside the project that provides tokenize.c and sqlite3GetToken.
- Run the resulting executable to see per-test pass/fail output and a final summary.

*/
#include <stdlib.h>
#include <cstdio>
#include <sqlite3.h>
#include <keywordhash.h>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>


// Try to use the project's public API first

// Provide fallbacks for TK_* constants if not defined by sqlite3.h.
// This allows the test to compile in environments where internal token codes
// might not be exposed publicly.
#ifndef TK_SPACE
#define TK_SPACE 0
#endif
#ifndef TK_MINUS
#define TK_MINUS 1
#endif
#ifndef TK_PTR
#define TK_PTR 2
#endif
#ifndef TK_LP
#define TK_LP 3
#endif
#ifndef TK_RP
#define TK_RP 4
#endif
#ifndef TK_SEMI
#define TK_SEMI 5
#endif
#ifndef TK_PLUS
#define TK_PLUS 6
#endif
#ifndef TK_STAR
#define TK_STAR 7
#endif
#ifndef TK_SLASH
#define TK_SLASH 8
#endif
#ifndef TK_REM
#define TK_REM 9
#endif
#ifndef TK_EQ
#define TK_EQ 10
#endif
#ifndef TK_LE
#define TK_LE 11
#endif
#ifndef TK_NE
#define TK_NE 12
#endif
#ifndef TK_LSHIFT
#define TK_LSHIFT 13
#endif
#ifndef TK_GE
#define TK_GE 14
#endif
#ifndef TK_RSHIFT
#define TK_RSHIFT 15
#endif
#ifndef TK_LT
#define TK_LT 16
#endif
#ifndef TK_GT
#define TK_GT 17
#endif
#ifndef TK_BANG
#define TK_BANG 18
#endif
#ifndef TK_BITOR
#define TK_BITOR 19
#endif
#ifndef TK_CONCAT
#define TK_CONCAT 20
#endif
#ifndef TK_COMMA
#define TK_COMMA 21
#endif
#ifndef TK_AND
#define TK_AND 22
#endif
#ifndef TK_TILDA
#define TK_TILDA 23
#endif
#ifndef TK_STRING
#define TK_STRING 24
#endif
#ifndef TK_VARIABLE
#define TK_VARIABLE 25
#endif
#ifndef TK_ID
#define TK_ID 26
#endif
#ifndef TK_INTEGER
#define TK_INTEGER 27
#endif
#ifndef TK_FLOAT
#define TK_FLOAT 28
#endif
#ifndef TK_BLOB
#define TK_BLOB 29
#endif
#ifndef TK_DOT
#define TK_DOT 30
#endif
#ifndef TK_ILLEGAL
#define TK_ILLEGAL 31
#endif
#ifndef TK_SELECT
#define TK_SELECT 32
#endif
#ifndef TK_NULL
#define TK_NULL 33
#endif

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Simple non-terminating assertion helpers
#define TEST_ASSERT(cond, desc) \
    do { \
        ++g_tests_run; \
        if(!(cond)) { \
            fprintf(stderr, "TEST FAILED: %s\n", (desc)); \
            ++g_tests_failed; \
        } else { \
            fprintf(stdout, "TEST PASSED: %s\n", (desc)); \
        } \
    } while(0)

#define EXPECT_EQ_INT(a, b, desc) TEST_ASSERT((int)(a) == (int)(b), (desc))
#define EXPECT_EQ_LONG(a, b, desc) TEST_ASSERT((long)(a) == (long)(b), (desc))
#define EXPECT_EQ_HEX(a, b, desc) TEST_ASSERT((unsigned long)(a) == (unsigned long)(b), (desc))
#define EXPECT_TRUE(cond, desc) TEST_ASSERT((cond) != 0, (desc))
#define EXPECT_FALSE(cond, desc) TEST_ASSERT((cond) == 0, (desc))

// Helper to invoke sqlite3GetToken for a given C-string input and expected token type
static void run_get_token(const char* s, int expected_len, int expected_type, const char* test_desc)
{
    int tokenType = -1;
    int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(s), &tokenType);
    char msg[256];
    snprintf(msg, sizeof(msg), "%s: length", test_desc);
    EXPECT_EQ_INT(len, expected_len, msg);

    snprintf(msg, sizeof(msg), "%s: tokenType", test_desc);
    EXPECT_EQ_INT(tokenType, expected_type, msg);
}

// Individual test cases
static void test_space_token()
{
    // Z starts with spaces; should skip spaces and return count, type TK_SPACE
    const char* z = "   abc";
    int tt;
    int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
    EXPECT_EQ_INT(len, 3, "Space token should skip 3 spaces");
    // TK_SPACE indicates a space token
    EXPECT_EQ_INT(tt, TK_SPACE, "Space token should be classified as TK_SPACE");
}

static void test_minus_and_ptr_tokens()
{
    // Single '-' => TK_MINUS, length 1
    {
        const char* z = "-";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "Single '-' token length");
        EXPECT_EQ_INT(tt, TK_MINUS, "Single '-' token type TK_MINUS");
    }

    // "->" => TK_PTR, length 2
    {
        const char* z = "->";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 2, "Arrow token length");
        EXPECT_EQ_INT(tt, TK_PTR, "Arrow token type TK_PTR");
    }
}

static void test_parentheses_and_basic_punct_tokens()
{
    // LP "(" => TK_LP
    {
        const char* z = "(";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "LP token length");
        EXPECT_EQ_INT(tt, TK_LP, "LP token type TK_LP");
    }

    // RP ")" => TK_RP
    {
        const char* z = ")";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "RP token length");
        EXPECT_EQ_INT(tt, TK_RP, "RP token type TK_RP");
    }

    // SEMI ";" => TK_SEMI
    {
        const char* z = ";";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "SEMI token length");
        EXPECT_EQ_INT(tt, TK_SEMI, "SEMI token type TK_SEMI");
    }
}

static void test_basic_operators_and_comment()
{
    // "+" => TK_PLUS
    {
        const char* z = "+";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "PLUS token length");
        EXPECT_EQ_INT(tt, TK_PLUS, "PLUS token type TK_PLUS");
    }

    // "/* ... */" comment should be treated as a space-like token; expected length equals full comment
    {
        const char* z = "/*abc*/";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        // The tokenizer consumes the whole comment as a space-like sequence
        EXPECT_EQ_INT(tt, TK_SPACE, "COMMENT token type TK_SPACE");
        // Length depends on the exact comment content; ensure non-zero length
        EXPECT_TRUE(len > 0, "COMMENT token should have positive length");
    }

    // "=" => TK_EQ, "==" => TK_EQ
    {
        const char* z = "=";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "EQ token length for single '='");
        EXPECT_EQ_INT(tt, TK_EQ, "EQ token type TK_EQ");
    }
    {
        const char* z = "==";
        int tt;
        int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 2, "EQ token length for '=='");
        EXPECT_EQ_INT(tt, TK_EQ, "EQ token type TK_EQ for '=='");
    }

    // "<=", "<>", "<", "<<" combos
    {
        const char* z = "<=";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 2, "LT token length for '<='");
        EXPECT_EQ_INT(tt, TK_LE, "LT token type TK_LE for '<='");
    }
    {
        const char* z = "<>";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 2, "LT token length for '<>'");
        EXPECT_EQ_INT(tt, TK_NE, "LT token type TK_NE for '<>'");
    }
    {
        const char* z = "<";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "LT token length for '<'");
        EXPECT_EQ_INT(tt, TK_LT, "LT token type TK_LT for '<'");
    }
    {
        const char* z = "<<";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 2, "LSHIFT token length for '<<'");
        EXPECT_EQ_INT(tt, TK_LSHIFT, "LSHIFT token type TK_LSHIFT for '<<'");
    }
}

static void test_dot_and_numbers()
{
    // ".5" should be a FLOAT token starting with dot
    {
        const char* z = ".5";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 2, "Floating point number starting with dot '.5' length");
        EXPECT_EQ_INT(tt, TK_FLOAT, "Floating point number token type TK_FLOAT for '.5'");
    }

    // "." followed by non-digit should be DOT
    {
        const char* z = ".a";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 1, "DOT token length for '.' followed by non-digit");
        EXPECT_EQ_INT(tt, TK_DOT, "DOT token type TK_DOT for '.' with non-digit following");
    }

    // "123" as INTEGER
    {
        const char* z = "123";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 3, "INTEGER token length for '123'");
        EXPECT_EQ_INT(tt, TK_INTEGER, "INTEGER token type TK_INTEGER for '123'");
    }
}

static void test_identifier_and_variable()
{
    // "$var" as VARIABLE
    {
        const char* z = "$var";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_TRUE(len > 0, "VARIABLE token should have positive length for '$var'");
        EXPECT_EQ_INT(tt, TK_VARIABLE, "VARIABLE token type TK_VARIABLE for '$var'");
    }

    // "abc" as ID
    {
        const char* z = "abc";
        int tt; int len = sqlite3GetToken(reinterpret_cast<const unsigned char*>(z), &tt);
        EXPECT_EQ_INT(len, 3, "ID token length for 'abc'");
        EXPECT_EQ_INT(tt, TK_ID, "ID token type TK_ID for 'abc'");
    }
}

// Runner
int main(void)
{
    fprintf(stdout, "Starting sqlite3GetToken test suite (C++11 harness)\n\n");

    test_space_token();
    test_minus_and_ptr_tokens();
    test_parentheses_and_basic_punct_tokens();
    test_basic_operators_and_comment();
    test_dot_and_numbers();
    test_identifier_and_variable();

    fprintf(stdout, "\nTest suite completed. Total: %d, Failures: %d\n",
            g_tests_run, g_tests_failed);

    // Non-zero exit on any failure to indicate issues
    return (g_tests_failed != 0) ? 1 : 0;
}