/*
  Test Suite for the focal method (main) functionality using the cre2 C API.
  This C++11 test harness mirrors the scenarios exercised in the provided
  C source, validating the replace and extract operations for different
  patterns and inputs.

  Notes:
  - We avoid GTest and use a lightweight, non-terminating assertion style.
  - All tests are designed to be executable under a C++11 compiler.
  - We assume the cre2 library headers and implementation are available and
    that cre2_string_t uses a writable buffer that may be reallocated by the
    library (hence we free target.data after each test).
  - Each test is annotated with comments describing its purpose and what it covers.
*/

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <cre2-test.h>
#include <iostream>


// Bring C linkage for the cre2 API
extern "C" {
}

// Simple non-terminating assertion facility
static int g_test_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
  if (!(cond)) { \
    std::cerr << "[EXPECT_FAIL] " << (msg) << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    ++g_test_failures; \
  } \
} while (0)

#define EXPECT_STR_EQ(target, expected, msg) do { \
  if ((target).length != std::strlen(expected) || 0 != std::strncmp((target).data, (expected), (target).length)) { \
    std::cerr << "[EXPECT_FAIL] " << (msg) << " - expected: \"" << (expected) << "\", got: \"" \
              << std::string((target).data, (target).length) << "\" (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    ++g_test_failures; \
  } \
} while (0)

// Helper to print current target content (for diagnostic purposes)
static void print_target(const cre2_string_t& t, const char* label) {
  std::cout << label << ": \"" << std::string(t.data, t.length) << "\" (length=" << t.length << ")" << std::endl;
}

// Test 1: replace full buffer when pattern matches entirety
static void test_replace_full_match() {
  // Objective: verify full-buffer replacement using pattern "ciao hello salut"
  const char* pattern = "ciao hello salut";
  const char* text    = "ciao hello salut";
  const char* replace = "pre \\0 post";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t target;
  cre2_string_t rewrite;

  target.data   = (char*)text;
  target.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(replace);

  int result = cre2_replace_re(rex, &target, &rewrite);

  // True branch: replacement succeeds
  EXPECT_TRUE(result == 1, "test_replace_full_match: result should be 1");

  // Expect full content replacement
  EXPECT_STR_EQ(target, "pre ciao hello salut post", "test_replace_full_match: content mismatch after replacement");

  // False branch exercise: intentionally check a wrong expectation to exercise failure path
  // (this does not alter normal flow; just exercises the non-true branch in the test)
  EXPECT_TRUE(!(strncmp(target.data, "pre x", 5) == 0 && target.length == strlen("pre x")), "test_replace_full_match: intentional negative check");

  print_target(target, "after_replace_full_match");

  cre2_delete(rex);
  free((void*)target.data);
}

// Test 2: replace a substring with a fixed string
static void test_replace_substring_fixed() {
  // Objective: replace first occurrence of "hello" with "ohayo"
  const char* pattern = "hello";
  const char* text    = "ciao hello salut";
  const char* replace = "ohayo";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t target;
  cre2_string_t rewrite;

  target.data   = (char*)text;
  target.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(replace);

  int result = cre2_replace_re(rex, &target, &rewrite);

  // True branch: replacement succeeds
  EXPECT_TRUE(result == 1, "test_replace_substring_fixed: result should be 1");

  // Expect content: "ciao ohayo salut"
  EXPECT_STR_EQ(target, "ciao ohayo salut", "test_replace_substring_fixed: content mismatch after replacement");

  print_target(target, "after_replace_substring_fixed");

  cre2_delete(rex);
  free((void*)target.data);
}

// Test 3: global replace: full match across the buffer
static void test_global_replace_full_match() {
  // Objective: global replace of exact full-match with the same replacement pattern
  const char* pattern = "ciao hello salut";
  const char* text    = "ciao hello salut";
  const char* replace = "pre \\0 post";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t target;
  cre2_string_t rewrite;

  target.data   = (char*)text;
  target.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(replace);

  int result = cre2_global_replace_re(rex, &target, &rewrite);

  // True branch
  EXPECT_TRUE(result == 1, "test_global_replace_full_match: result should be 1");

  EXPECT_STR_EQ(target, "pre ciao hello salut post", "test_global_replace_full_match: content mismatch after global replacement");

  print_target(target, "after_global_replace_full_match");

  cre2_delete(rex);
  free((void*)target.data);
}

// Test 4: global replace: substring with fixed string
static void test_global_replace_substring_fixed() {
  // Objective: global replace of "hello" with "ohayo"
  const char* pattern = "hello";
  const char* text    = "ciao hello salut";
  const char* replace = "ohayo";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t target;
  cre2_string_t rewrite;

  target.data   = (char*)text;
  target.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(replace);

  int result = cre2_global_replace_re(rex, &target, &rewrite);

  // True branch
  EXPECT_TRUE(result == 1, "test_global_replace_substring_fixed: result should be 1");

  EXPECT_STR_EQ(target, "ciao ohayo salut", "test_global_replace_substring_fixed: content mismatch after global replacement");

  print_target(target, "after_global_replace_substring_fixed");

  cre2_delete(rex);
  free((void*)target.data);
}

// Test 5: global replace: multiple substrings with parametrised string
static void test_global_replace_multiple_substrings_param() {
  // Objective: replace multiple occurrences of pattern with backreference
  const char* pattern = "[a-z]+\\(([0-9]+)\\)";
  const char* text    = "ciao(1) hello(2) salut(3)";
  const char* replace = "ohayo(\\1)";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t target;
  cre2_string_t rewrite;

  target.data   = (char*)text;
  target.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(rewrite.data);

  int result = cre2_global_replace_re(rex, &target, &rewrite);

  // True branch: 3 substitutions expected
  EXPECT_TRUE(result == 3, "test_global_replace_multiple_substrings_param: expected 3 substitutions");

  EXPECT_STR_EQ(target, "ohayo(1) ohayo(2) ohayo(3)", "test_global_replace_multiple_substrings_param: content mismatch after substitutions");

  print_target(target, "after_global_replace_multiple_substrings_param");

  cre2_delete(rex);
  free((void*)target.data);
}

// Test 6: extract full match using extract_re
static void test_extract_full_match() {
  // Objective: extract the full match into a target string, similar to replacement
  const char* pattern = "ciao hello salut";
  const char* text    = "ciao hello salut";
  const char* replace = "pre \\0 post";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t input;
  cre2_string_t rewrite;
  cre2_string_t target;

  input.data   = (char*)text;
  input.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(replace);

  int result = cre2_extract_re(rex, &input, &rewrite, &target);

  // True branch
  EXPECT_TRUE(result == 1, "test_extract_full_match: result should be 1");

  EXPECT_STR_EQ(target, "pre ciao hello salut post", "test_extract_full_match: target content mismatch");

  print_target(target, "after_extract_full_match");

  cre2_delete(rex);
  free((void*)target.data);
}

// Test 7: extract substring with fixed string
static void test_extract_substring_fixed() {
  // Objective: extract with pattern "hello([0-9]+)" from "ciao hello123 salut"
  const char* pattern = "hello([0-9]+)";
  const char* text    = "ciao hello123 salut";
  const char* replace = "ohayo\\1";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t input;
  cre2_string_t rewrite;
  cre2_string_t target;

  input.data   = (char*)text;
  input.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(replace);

  int result = cre2_extract_re(rex, &input, &rewrite, &target);

  // True branch
  EXPECT_TRUE(result == 1, "test_extract_substring_fixed: result should be 1");

  EXPECT_STR_EQ(target, "ohayo123", "test_extract_substring_fixed: extracted content mismatch");

  print_target(target, "after_extract_substring_fixed");

  cre2_delete(rex);
  free((void*)target.data);
}

// Test 8: negative path coverage (intentional failure to exercise false branches)
static void test_negative_path_coverage() {
  // This test intentionally checks a wrong expectation to exercise the false branch path in tests.
  const char* pattern = "hello";
  const char* text    = "ciao hello salut";
  const char* replace = "ohayo";

  cre2_regexp_t * rex = cre2_new(pattern, strlen(pattern), NULL);

  cre2_string_t target;
  cre2_string_t rewrite;

  target.data   = (char*)text;
  target.length = strlen(text);

  rewrite.data   = (char*)replace;
  rewrite.length = strlen(replace);

  int result = cre2_replace_re(rex, &target, &rewrite);

  // Intentionally incorrect expectation to exercise false path
  EXPECT_TRUE(result == 0, "test_negative_path_coverage: intentionally failing expectation to cover false branch");

  // Clean up (even though the test is intended to fail, we should not leak)
  cre2_delete(rex);
  free((void*)target.data);
}

// Runner
int main(void) {
  // Run tests (Step 2: Unit Test Generation)
  test_replace_full_match();
  test_replace_substring_fixed();
  test_global_replace_full_match();
  test_global_replace_substring_fixed();
  test_global_replace_multiple_substrings_param();
  test_extract_full_match();
  test_extract_substring_fixed();
  test_negative_path_coverage();

  // Step 3: Test Case Refinement (summary)
  if (g_test_failures == 0) {
    std::cout << "[TEST SUITE] All tests passed." << std::endl;
  } else {
    std::cout << "[TEST SUITE] " << g_test_failures << " test(s) failed." << std::endl;
  }

  // Return number of failures as non-zero exit code to indicate issues
  return g_test_failures;
}