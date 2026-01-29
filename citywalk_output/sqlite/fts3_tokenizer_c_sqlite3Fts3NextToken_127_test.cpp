/*
Unit test suite for sqlite3Fts3NextToken (fts3_tokenizer.c)

Overview:
- This test suite targets the focal method: sqlite3Fts3NextToken.
- It exercises the tokenization rules implemented in sqlite3Fts3NextToken, including:
  - Identifier tokens composed of characters recognized by sqlite3Fts3IsIdChar
  - Quoted tokens delimited by ', ", or `
  - Bracket-delimited tokens prefixed by [
- It relies on the actual implementation of sqlite3Fts3NextToken and sqlite3Fts3IsIdChar
  (i.e., this test must be linked with the fts3_tokenizer.c source file from the project).
  Do not compile sqlite3Fts3NextToken and sqlite3Fts3IsIdChar in this test file;
  link against the existing object/file that contains their definitions.

Notes:
- This test suite is written in plain C++11 without GoogleTest. It uses a small
  in-house assertion mechanism that continues execution after a failed assertion
  to maximize coverage across tests.
- The tests focus on correctness of token boundaries and handling of the different
  tokenization branches: identifiers, quotes, and brackets.
- The test harness does not access private/static internals; it exercises only the
  public C interface as intended.

Candidate Keywords (from Step 1):
- sqlite3Fts3NextToken, sqlite3Fts3IsIdChar
- Token types: IDENT (letters/digits per IsIdChar), QUOTE (' " `), BRACKET '[' ... ']'
- Token boundaries: quoted tokens include the quotes themselves, bracket tokens include brackets
- Non-token characters: whitespace and punctuation are skipped

Usage:
- Compile this test file together with fts3_tokenizer.c (the file containing
  sqlite3Fts3NextToken and sqlite3Fts3IsIdChar), e.g.:
  g++ -std=c++11 test_fts3_next_token.cpp fts3_tokenizer.c -o test_fts3
- Run ./test_fts3

*/

#include <fts3Int.h>
#include <string.h>
#include <vector>
#include <assert.h>
#include <string>
#include <cstring>
#include <iostream>


extern "C" {
  // Declarations of the focal function and its dependency used by the test.
  // They must be resolved by linking against the project's fts3_tokenizer.c
  // (or equivalent object/file containing their implementations).
  const char *sqlite3Fts3NextToken(const char *zStr, int *pn);
  int sqlite3Fts3IsIdChar(char c);
}

// Simple in-house test harness
static int g_failures = 0;
static std::vector<std::string> g_failedMessages;

// Helper macro for assertions that records failures but does not abort
#define ASSERT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    g_failures++; \
    g_failedMessages.push_back(std::string("ASSERT_TRUE failed: ") + (msg)); \
  } \
} while(0)

#define ASSERT_EQ(expected, actual, msg) do { \
  if(!((expected) == (actual))) { \
    g_failures++; \
    g_failedMessages.push_back(std::string("ASSERT_EQ failed: ") + (msg) + \
      " | expected: " + std::to_string(static_cast<long long>(expected)) + \
      " actual: " + std::to_string(static_cast<long long>(actual)); \
  } \
} while(0)


// Helper to tokenize a string using sqlite3Fts3NextToken
static std::vector<std::string> tokenize(const std::string &s) {
  std::vector<std::string> tokens;
  const char *z = s.c_str();
  int n = 0;
  while (true) {
    const char *tok = sqlite3Fts3NextToken(z, &n);
    if (tok == nullptr) break;
    tokens.emplace_back(tok, tok + n);
    z = tok + n;
  }
  return tokens;
}

// Test 1: Basic identifier tokens separated by whitespace
static void test_basic_identifier_tokens() {
  // Scenario: "abc123 def456" -> tokens: "abc123", "def456"
  std::string input = "abc123 def456";
  auto toks = tokenize(input);
  // Expect 2 tokens
  ASSERT_EQ(2u, toks.size(), "test_basic_identifier_tokens: token count");
  if (toks.size() == 2) {
    ASSERT_EQ(std::string("abc123"), toks[0], "test_basic_identifier_tokens: first token");
    ASSERT_EQ(std::string("def456"), toks[1], "test_basic_identifier_tokens: second token");
  }
}

// Test 2: Quoted token (single quotes), followed by an unquoted token
static void test_quoted_single_token() {
  // Scenario: "'hello' world" -> tokens: "'hello'", "world"
  std::string input = "'hello' world";
  auto toks = tokenize(input);
  ASSERT_TRUE(toks.size() >= 2, "test_quoted_single_token: at least 2 tokens");
  if (toks.size() >= 2) {
    ASSERT_EQ(std::string("'hello'"), toks[0], "test_quoted_single_token: first token (quoted)");
    ASSERT_EQ(std::string("world"), toks[1], "test_quoted_single_token: second token (unquoted)");
  }
}

// Test 3: Bracket-delimited token
static void test_bracket_token() {
  // Scenario: " [tok] rest" -> tokens: "[tok]", "rest"
  std::string input = " [tok] rest";
  auto toks = tokenize(input);
  ASSERT_TRUE(toks.size() >= 2, "test_bracket_token: at least 2 tokens");
  if (toks.size() >= 2) {
    ASSERT_EQ(std::string("[tok]"), toks[0], "test_bracket_token: first token (bracketed)");
    ASSERT_EQ(std::string("rest"), toks[1], "test_bracket_token: second token (unquoted)");
  }
}

// Test 4: Mixed sequence covering IDENT, BRACKET, QUOTE, and nested patterns
static void test_mixed_sequence() {
  // Scenario: "a[bc] 'd e'\"f g\" h" -> tokens: "a", "[bc]", "'d e'", "\"f g\"", "h"
  std::string input = "a[bc] 'd e'\"f g\" h";
  auto toks = tokenize(input);
  std::vector<std::string> expected = {"a", "[bc]", "'d e'", "\"f g\"", "h"};
  ASSERT_TRUE(toks.size() == expected.size(), "test_mixed_sequence: token count");
  if (toks.size() == expected.size()) {
    for (size_t i = 0; i < expected.size(); ++i) {
      ASSERT_EQ(expected[i], toks[i], "test_mixed_sequence: token " + std::to_string(i));
    }
  }
}

// Test 5: Empty string yields no tokens
static void test_empty_string_has_no_tokens() {
  std::string input = "";
  auto toks = tokenize(input);
  ASSERT_TRUE(toks.empty(), "test_empty_string_has_no_tokens: no tokens for empty input");
}

// Run all tests
static void run_all_tests() {
  test_basic_identifier_tokens();
  test_quoted_single_token();
  test_bracket_token();
  test_mixed_sequence();
  test_empty_string_has_no_tokens();
}

int main() {
  run_all_tests();
  if(!g_failedMessages.empty()) {
    std::cerr << "FOCAL TESTS FAILED: " << g_failedMessages.size() << " failure(s)\n";
    for(const auto &m : g_failedMessages) {
      std::cerr << m << "\n";
    }
    return 1;
  }
  std::cout << "FOCAL TESTS PASSED: all tests succeeded.\n";
  return 0;
}