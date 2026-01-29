// C++11 test suite for sqlite3Fts5ParseSetDistance (ftS5 expression parsing)
// This test uses only the C headers from the FTS5 project and the standard library.
// It is designed to compile in a C++11 environment without GoogleTest or other frameworks.

#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>


extern "C" {
}

// Simple non-terminating test harness (mini EXPECT_* macros)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
  do { \
    if(!(cond)) { \
      std::cerr << "EXPECT_TRUE failed: " << (msg) \
                << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
      ++g_failures; \
    } \
  } while(0)

#define EXPECT_EQ(a, b, msg) \
  do { \
    if((a) != (b)) { \
      std::cerr << "EXPECT_EQ failed: " << (msg) \
                << " - got " << (a) << ", expected " << (b) \
                << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
      ++g_failures; \
    } \
  } while(0)


// Candidate Keywords (from Step 1) that define the core behavior of the focal method
// - Fts5Parse *pParse
// - Fts5ExprNearset *pNear
// - Fts5Token *p
// - p->n, p->p
// - FTS5_DEFAULT_NEARDIST
// - sqlite3Fts5ParseError
//
// These keywords map to the test scenarios below.


// Test 1: pNear is provided and p->n > 0 with digits only.
// Expectation: nNear is parsed as a decimal number from the token.
void test_parseSetDistance_digits_parses_correct() {
    Fts5Parse pParse;
    // Zero/initialize to simulate clean parse state
    std::memset(&pParse, 0, sizeof(Fts5Parse));

    Fts5ExprNearset near;
    near.nNear = 0xDEADBEEF; // sentinel to ensure it gets overwritten
    // Token containing digits "123"
    const char *digits = "123";
    Fts5Token tok;
    tok.p = (unsigned char*)digits;
    tok.n = 3;

    // Call under test
    sqlite3Fts5ParseSetDistance(&pParse, &near, &tok);

    // Expectation: near.nNear == 123
    EXPECT_EQ(near.nNear, 123, "Digits token should be parsed to 123");
}

// Test 2: pNear is provided and p->n == 0 (empty token).
// Expectation: nNear is set to FTS5_DEFAULT_NEARDIST.
void test_parseSetDistance_empty_token_defaults() {
    Fts5Parse pParse;
    std::memset(&pParse, 0, sizeof(Fts5Parse));

    Fts5ExprNearset near;
    near.nNear = -1; // sentinel to ensure it changes to default value
    Fts5Token tok;
    tok.p = (unsigned char*)""; // empty token
    tok.n = 0;                    // empty

    sqlite3Fts5ParseSetDistance(&pParse, &near, &tok);

    EXPECT_EQ(near.nNear, FTS5_DEFAULT_NEARDIST, "Empty token should set default neardist");
}

// Test 3: pNear is provided and p->n > 0 with a non-digit character.
// Expectation: Since there is a non-digit, the function should early return
// and not modify nNear from its initial value.
void test_parseSetDistance_invalid_digit_no_assignment() {
    Fts5Parse pParse;
    std::memset(&pParse, 0, sizeof(Fts5Parse));

    Fts5ExprNearset near;
    near.nNear = 0xCAFEBABE; // sentinel to detect modification

    // Token containing non-digit "12a"
    const char *tokstr = "12a";
    Fts5Token tok;
    tok.p = (unsigned char*)tokstr;
    tok.n = 3;

    sqlite3Fts5ParseSetDistance(&pParse, &near, &tok);

    // Expectation: near.nNear remains unchanged
    EXPECT_EQ(near.nNear, 0xCAFEBABE, "Invalid digit should not modify nNear");
}

// Test 4: pNear is NULL (distance should be ignored; function should be a no-op).
void test_parseSetDistance_null_near_no_op() {
    Fts5Parse pParse;
    std::memset(&pParse, 0, sizeof(Fts5Parse));

    // Token with digits "456" to exercise parsing path, but near is NULL
    const char *digits = "456";
    Fts5Token tok;
    tok.p = (unsigned char*)digits;
    tok.n = 3;

    // Call with pNear = NULL; nothing should be written anywhere
    sqlite3Fts5ParseSetDistance(&pParse, (Fts5ExprNearset*)NULL, &tok);

    // If we reached here without crash, this test passes for non-op scenario.
    EXPECT_TRUE(true, "Call with NULL pNear should be a no-op and not crash");
}


// Main function to run all tests
int main() {
    test_parseSetDistance_digits_parses_correct();
    test_parseSetDistance_empty_token_defaults();
    test_parseSetDistance_invalid_digit_no_assignment();
    test_parseSetDistance_null_near_no_op();

    if(g_failures){
        std::cerr << "Total test failures: " << g_failures << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}