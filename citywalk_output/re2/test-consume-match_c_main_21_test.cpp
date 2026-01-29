/*
 * Lightweight C++11 test suite for the focal method: main (void) in test-consume-match.c
 *
 * Goals:
 * - Exercise cre2_consume and cre2_consume_re with a variety of patterns/texts
 * - Cover true/false branches of condition predicates
 * - Use only C/C++ standard facilities plus the provided cre2 API
 * - Avoid GTest; implement a small non-terminating assertion mechanism
 * - Keep tests self-contained and compilable under C++11
 *
 * Notes:
 * - The original C tests rely on designated initializers (e.g., { .data = text, .length = ... }),
 *   which are not valid in C++. We adapt initialization accordingly.
 * - VLAs are not allowed in standard C++11; we allocate match arrays dynamically.
 * - We only rely on cre2.h API; no private fields or non-exported methods are accessed.
 * - Test functions are designed to be non-throwing; failures are recorded but do not abort tests.
 *
 * Candidate Keywords (from Step 1 understanding):
 * - pattern, text, input, result, match, nmatch, rex, cre2_consume, cre2_consume_re, cre2_new, cre2_delete
 * - strncmp, strlen, data, length
 *
 * Test plan (mapped to the original blocks):
 * - Consume tests (non-regex and regex-like usage via cre2_consume and cre2_consume_re)
 * - Success paths: full buffer consumed, partial consumption, one/two subexpressions, with/without matches
 * - Failure paths: pattern cannot be satisfied, incorrect subexpression counts, etc.
 *
 * Explanatory comments precede each unit test to describe intent and expected branching.
 */

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <cstdlib>
#include <cre2-test.h>
#include <iostream>
#include <cstdio>


extern "C" {
}

// Lightweight non-terminating test harness
static int g_failures = 0;

// Log a non-fatal assertion failure
#define ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "ASSERTION FAILED in " << __FILE__ << ":" << __LINE__ \
                  << " - " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define TEST_NAME __func__

/*
 * Helper utilities to create cre2_string_t in C++ (no designated initializers)
 */
static cre2_string_t make_input(const char* data) {
    cre2_string_t s;
    s.data = data;
    s.length = std::strlen(data);
    return s;
}

/*
 * Helper to create a dynamic array of cre2_string_t for matches.
 * Returns pointer; caller must delete[].
 */
static cre2_string_t* alloc_matches(int n) {
    return new cre2_string_t[n];
}

/*
 * Test 1: success, no parentheses, full consumed buffer using cre2_consume
 * Corresponds to first block in focal method.
 */
TEST_NAME test_consume_basic_full_buffer() {
    const char * pattern = "ci.*ut";
    const char * text    = "ciao salut";
    cre2_string_t input = make_input(text);
    int result = cre2_consume(pattern, &input, NULL, 0);
    ASSERT(result != 0, "consume should succeed for full buffer");
    // input should be fully consumed: compared to empty string
    int cmp = std::strncmp("", input.data, input.length);
    ASSERT(cmp == 0, "input buffer should be fully consumed (empty remaining)");
}

/*
 * Test 2: success, no parentheses, partially consumed buffer
 */
TEST_NAME test_consume_basic_partial_buffer() {
    const char * pattern = "ci.*ut";
    const char * text    = "ciao salut hello";
    cre2_string_t input  = make_input(text);
    int result = cre2_consume(pattern, &input, NULL, 0);
    ASSERT(result != 0, "consume should succeed for partial buffer");
    int cmp = std::strncmp(" hello", input.data, input.length);
    ASSERT(cmp == 0, "remaining buffer should be ' hello' after consumption");
}

/*
 * Test 3: success, one parenthetical subexpression, one match entry
 */
TEST_NAME test_consume_one_subexpr_one_match() {
    const char * pattern = "(ciao) salut";
    const char * text    = "ciao salut hello";
    cre2_string_t input  = make_input(text);
    int nmatch = 1;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume(pattern, &input, match, nmatch);
    ASSERT(result != 0, "consume should succeed with one subexpression");
    int cmpInput = std::strncmp(" hello", input.data, input.length);
    ASSERT(cmpInput == 0, "input should be ' hello' after consumption");
    int cmpMatch0 = std::strncmp("ciao", match[0].data, match[0].length);
    ASSERT(cmpMatch0 == 0, "match[0] should be 'ciao'");
    delete[] match;
}

/*
 * Test 4: success, two parenthetical subexpressions, two match entries
 */
TEST_NAME test_consume_two_subexpr_two_matches() {
    const char * pattern = "(ciao) (salut)";
    const char * text    = "ciao salut hello";
    cre2_string_t input  = make_input(text);
    int nmatch = 2;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume(pattern, &input, match, nmatch);
    ASSERT(result != 0, "consume should succeed with two subexpressions and two matches");
    int cmpInput = std::strncmp(" hello", input.data, input.length);
    ASSERT(cmpInput == 0, "input should be ' hello' after consumption");
    int cmp0 = std::strncmp("ciao", match[0].data, match[0].length);
    int cmp1 = std::strncmp("salut", match[1].data, match[1].length);
    ASSERT(cmp0 == 0, "match[0] should be 'ciao'");
    ASSERT(cmp1 == 0, "match[1] should be 'salut'");
    delete[] match;
}

/*
 * Test 5: failure, no parentheses
 * Pattern ci.*ut should fail on "ciao hello" (no trailing "ut" substring after match)
 */
TEST_NAME test_consume_failure_no_parentheses() {
    const char * pattern = "ci.*ut";
    const char * text    = "ciao hello";
    cre2_string_t input  = make_input(text);
    int result = cre2_consume(pattern, &input, NULL, 0);
    ASSERT(result == 0, "consume should fail for unmatched input");
}

/*
 * Test 6: failure, one parenthetical subexpression
 * Pattern "(ciao) salut" on "ciao hello" should fail
 */
TEST_NAME test_consume_failure_one_subexpr() {
    const char * pattern = "(ciao) salut";
    const char * text    = "ciao hello";
    cre2_string_t input  = make_input(text);
    int nmatch = 1;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume(pattern, &input, match, nmatch);
    ASSERT(result == 0, "consume should fail when text does not match subexpression");
    // On failure, input should remain unchanged
    int cmpInput = std::strncmp(text, input.data, input.length);
    ASSERT(cmpInput == 0, "input should be unchanged on failure");
    delete[] match;
}

/*
 * Test 7: success, one parenthetical subexpression, no match entries
 * Ensure consumption still advances and no crash when matches array is NULL
 */
TEST_NAME test_consume_one_subexpr_no_matches() {
    const char * pattern = "(ciao) salut";
    const char * text    = "ciao salut hello";
    cre2_string_t input  = make_input(text);
    int result = cre2_consume(pattern, &input, NULL, 0);
    ASSERT(result != 0, "consume should succeed with no matches array (NULL)");
    int cmpInput = std::strncmp(" hello", input.data, input.length);
    ASSERT(cmpInput == 0, "input should be ' hello' after consumption");
}

/*
 * Test 8: failure, one subexpression with two match entries
 * Should fail if the caller supplies two matches when only one is expected
 */
TEST_NAME test_consume_failure_one_subexpr_two_matches() {
    const char * pattern = "(ciao) salut";
    const char * text    = "ciao salut hello";
    cre2_string_t input  = make_input(text);
    int nmatch = 2;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume(pattern, &input, match, nmatch);
    // Depending on library semantics, this can fail; replicate original: expect failure (0)
    ASSERT(result == 0, "consume should fail when providing too many matches for pattern");
    delete[] match;
}

/*
 * Test 9: success, two subexpressions, one match entry
 * pattern "(ciao) (salut)" on "ciao salut hello"
 */
TEST_NAME test_consume_two_subexpr_one_match() {
    const char * pattern = "(ciao) (salut)";
    const char * text    = "ciao salut hello";
    cre2_string_t input  = make_input(text);
    int nmatch = 1;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume(pattern, &input, match, nmatch);
    ASSERT(result != 0, "consume should succeed with one match entry");
    int cmpMatch0 = std::strncmp("ciao", match[0].data, match[0].length);
    int cmpInput = std::strncmp(" hello", input.data, input.length);
    ASSERT(cmpMatch0 == 0, "match[0] should be 'ciao'");
    ASSERT(cmpInput == 0, "input should be ' hello' after consumption");
    delete[] match;
}

/*
 * The following tests exercise the "regex-like" path using cre2_consume_re
 * Test 10: full buffer consumption via cre2_consume_re
 * Test 11: partial consumption via cre2_consume_re
 * Test 12: one subexpression via cre2_consume_re
 * Test 13: two subexpressions via cre2_consume_re
 * Test 14: various failure paths via cre2_consume_re
 */
TEST_NAME test_consume_re_full_buffer() {
    const char * pattern = "ci.*ut";
    const char * text    = "ciao salut";
    cre2_regexp_t * rex = cre2_new(pattern, std::strlen(pattern), NULL);
    cre2_string_t input = make_input(text);
    int result = cre2_consume_re(rex, &input, NULL, 0);
    cre2_delete(rex);
    ASSERT(result != 0, "consume_re should succeed for full buffer");
    int cmp = std::strncmp("", input.data, input.length);
    ASSERT(cmp == 0, "input should be empty after full consume_re");
}

TEST_NAME test_consume_re_partial_buffer() {
    const char * pattern = "ci.*ut";
    const char * text    = "ciao salut hello";
    cre2_regexp_t * rex = cre2_new(pattern, std::strlen(pattern), NULL);
    cre2_string_t input = make_input(text);
    int result = cre2_consume_re(rex, &input, NULL, 0);
    cre2_delete(rex);
    ASSERT(result != 0, "consume_re should succeed for partial buffer");
    int cmp = std::strncmp(" hello", input.data, input.length);
    ASSERT(cmp == 0, "input should be ' hello' after consume_re");
}

TEST_NAME test_consume_re_one_subexpr() {
    const char * pattern = "(ciao) salut";
    const char * text    = "ciao salut hello";
    cre2_regexp_t * rex = cre2_new(pattern, std::strlen(pattern), NULL);
    cre2_string_t input = make_input(text);
    int nmatch = 1;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume_re(rex, &input, match, nmatch);
    cre2_delete(rex);
    ASSERT(result != 0, "consume_re should succeed with one subexpression");
    int cmpInput = std::strncmp(" hello", input.data, input.length);
    int cmpMatch0 = std::strncmp("ciao", match[0].data, match[0].length);
    ASSERT(cmpInput == 0, "input should be ' hello' after consume_re");
    ASSERT(cmpMatch0 == 0, "match[0] should be 'ciao'");
    delete[] match;
}

TEST_NAME test_consume_re_two_subexpr_two_matches() {
    const char * pattern = "(ciao) (salut)";
    const char * text    = "ciao salut hello";
    cre2_regexp_t * rex = cre2_new(pattern, std::strlen(pattern), NULL);
    cre2_string_t input = make_input(text);
    int nmatch = 2;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume_re(rex, &input, match, nmatch);
    cre2_delete(rex);
    ASSERT(result != 0, "consume_re should succeed with two subexpressions");
    int cmpInput = std::strncmp(" hello", input.data, input.length);
    int cmp0 = std::strncmp("ciao", match[0].data, match[0].length);
    int cmp1 = std::strncmp("salut", match[1].data, match[1].length);
    ASSERT(cmpInput == 0, "input should be ' hello' after consume_re");
    ASSERT(cmp0 == 0, "match[0] should be 'ciao'");
    ASSERT(cmp1 == 0, "match[1] should be 'salut'");
    delete[] match;
}

TEST_NAME test_consume_re_failure_no_parentheses() {
    const char * pattern = "ci.*ut";
    const char * text    = "ciao hello";
    cre2_regexp_t * rex = cre2_new(pattern, std::strlen(pattern), NULL);
    cre2_string_t input = make_input(text);
    int result = cre2_consume_re(rex, &input, NULL, 0);
    cre2_delete(rex);
    ASSERT(result == 0, "consume_re should fail when pattern doesn't match input");
}

TEST_NAME test_consume_re_failure_one_subexpr() {
    const char * pattern = "(ciao) salut";
    const char * text    = "ciao hello";
    cre2_regexp_t * rex = cre2_new(pattern, std::strlen(pattern), NULL);
    cre2_string_t input = make_input(text);
    int nmatch = 1;
    cre2_string_t* match = alloc_matches(nmatch);
    int result = cre2_consume_re(rex, &input, match, nmatch);
    cre2_delete(rex);
    ASSERT(result == 0, "consume_re should fail when text does not contain full 'salut'");
    // input should remain unchanged
    int cmpInput = std::strncmp(text, input.data, input.length);
    ASSERT(cmpInput == 0, "input should be unchanged after failed consume_re");
    delete[] match;
}

/*
 * Runner: execute all test cases
 */
int main(void) {
    // Basic consume tests
    test_consume_basic_full_buffer();
    test_consume_basic_partial_buffer();
    test_consume_one_subexpr_one_match();
    test_consume_two_subexpr_two_matches();
    test_consume_failure_no_parentheses();
    test_consume_failure_one_subexpr();
    test_consume_one_subexpr_no_matches();
    test_consume_failure_one_subexpr_two_matches();
    test_consume_two_subexpr_one_match();
    // Regex-based tests
    test_consume_re_full_buffer();
    test_consume_re_partial_buffer();
    test_consume_re_one_subexpr();
    test_consume_re_two_subexpr_two_matches();
    test_consume_re_failure_no_parentheses();
    test_consume_re_failure_one_subexpr();

    if (g_failures) {
        std::cerr << "Total failures: " << g_failures << std::endl;
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}