// Unit test suite for cre2_extract (cre2.cpp) without GoogleTest
// This test suite uses a lightweight, non-terminating assertion framework.
// It exercises both true/false branches of the predicate by comparing against
// independent RE2::Extract results and validating the target buffer content.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>




// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (msg) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (msg) \
                  << " | Expected: " << (b) << ", Actual: " << (a) \
                  << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
} while(0)

// Helper: run a single extraction test and validate against independent RE2::Extract
static void test_cre2_extract_against_re2(const std::string& test_name,
                                          const std::string& text_str,
                                          const std::string& pattern_str,
                                          const std::string& rewrite_str)
{
    // Prepare mutable buffers to mimic cre2_string_t usage
    // Keep buffers alive for the duration of the call
    const size_t MAX_BUF = 1024;
    char text_buf[MAX_BUF];
    char rewrite_buf[MAX_BUF];

    // Copy text
    std::size_t tlen = text_str.size();
    if (tlen >= MAX_BUF) tlen = MAX_BUF - 1;
    std::memcpy(text_buf, text_str.data(), tlen);
    text_buf[tlen] = '\0';

    // Copy rewrite
    std::size_t rlen = rewrite_str.size();
    if (rlen >= MAX_BUF) rlen = MAX_BUF - 1;
    std::memcpy(rewrite_buf, rewrite_str.data(), rlen);
    rewrite_buf[rlen] = '\0';

    // Prepare cre2_string_t inputs
    cre2_string_t text;
    text.data = text_buf;
    text.length = static_cast<int>(tlen);

    cre2_string_t rewrite;
    rewrite.data = rewrite_buf;
    rewrite.length = static_cast<int>(rlen);

    // Target output from cre2_extract
    cre2_string_t target;
    target.data = nullptr;
    target.length = 0;

    // Pattern (const char*) is acceptable; the function uses it to build a StringPiece
    const char* pattern_c = pattern_str.c_str();

    // Call the focal function
    int ret_from_cre2_extract = cre2_extract(pattern_c, &text, &rewrite, &target);

    // Independent expected result using RE2::Extract
    // Build StringPiece pieces for independent evaluation
    re2::StringPiece T2(text_buf, tlen);
    re2::StringPiece P2(pattern_c, pattern_str.size());
    re2::StringPiece R2(rewrite_buf, rlen);

    std::string expected_O;
    bool expected_ret = RE2::Extract(T2, P2, R2, &expected_O);

    // Assertions:
    // 1) Return value should match expected (0/1)
    EXPECT_EQ(static_cast<int>(expected_ret), ret_from_cre2_extract,
              test_name + " - return value mismatch");

    // 2) target.length should equal expected_O.length()
    EXPECT_EQ(target.length, static_cast<int>(expected_O.length()),
              test_name + " - target length mismatch");

    // 3) target.data content should match expected_O
    if (expected_O.length() > 0) {
        // Build string from target buffer
        std::string actual(target.data, target.length);
        EXPECT_TRUE(actual == expected_O,
                    test_name + " - target content mismatch");
    } else {
        // When no match, RE2 may produce empty O. Ensure target is an empty string.
        std::string actual(target.data ? target.data : "");
        // Ensure buffer exists and content is empty
        EXPECT_TRUE(actual.empty(),
                    test_name + " - target content should be empty when no match");
    }

    // Cleanup allocated memory
    if (target.data) {
        free(target.data);
        target.data = nullptr;
        target.length = 0;
    }
}

// Public test cases

// Test 1: No match scenario (pattern not found in text)
static void test_no_match_case()
{
    // Text has no digits; pattern seeks digits; rewrite is arbitrary
    std::string test_name = "test_no_match_case";
    test_cre2_extract_against_re2(test_name,
                                 "abcdef",        // text
                                 "(\\d+)",         // pattern (no match)
                                 "$0");            // rewrite
}

// Test 2: Simple match with digits
static void test_simple_match_case()
{
    // Text contains a digit sequence; pattern captures digits; rewrite repeats the match
    std::string test_name = "test_simple_match_case";
    test_cre2_extract_against_re2(test_name,
                                 "abc123def",       // text
                                 "(\\d+)",           // pattern
                                 "$0");              // rewrite (full match)
}

// Test 3: Complex capture with named-like groups (practice with captures)
static void test_complex_capture_case()
{
    // Text contains a name and an ID; rewrite combines captures
    std::string test_name = "test_complex_capture_case";
    test_cre2_extract_against_re2(test_name,
                                 "Name: John Doe, ID: 42", // text
                                 "Name: ([A-Za-z ]+), ID: (\\d+)", // pattern
                                 "$1|$2");                      // rewrite
}

// Test 4: Empty text case to verify edge behavior
static void test_empty_text_case()
{
    std::string test_name = "test_empty_text_case";
    test_cre2_extract_against_re2(test_name,
                                 "",                 // empty text
                                 "a",                // pattern that won't match
                                 "$0");              // rewrite
}

// Main: run all tests and report summary
int main() {
    test_no_match_case();
    test_simple_match_case();
    test_complex_capture_case();
    test_empty_text_case();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}