// C++11 compatible unit test harness for cre2_find_and_consume (no GTest, no mocks)
// Focus: validate behavior of the FindAndConsumeN wrapper defined in cre2.cpp
// Notes:
// - This test suite uses a lightweight in-process assertion mechanism to avoid
//   terminating the test run on first failure, enabling higher coverage.
// - We rely on the cre2_string_t structure (data and length) as used throughout cre2.cpp.
// - We assume the function prototype for FindAndConsumeN is:
//     int cre2_find_and_consume(const char* pattern, cre2_string_t* text, cre2_string_t* match)
//   based on the <FOCAL_METHOD> section and surrounding wrappers.
// - If your actual prototype differs, please adjust the extern declaration accordingly.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the CRE2 wrapper header from the project

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define TEST_FAIL(msg) do { \
    std::cerr << "TEST_FAIL: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
    ++g_failures; \
} while (0)

#define TEST_EQ(expected, actual, msg) do { \
    if (!((expected) == (actual))) { \
        TEST_FAIL(std::string("ASSERTION FAILED: ") + (msg) + \
                  " | expected: " + std::to_string(static_cast<long long>(expected)) + \
                  " actual: " + std::to_string(static_cast<long long>(actual))); \
    } \
} while (0)

#define TEST_STR_EQ(expected, actual_ptr, actual_len, msg) do { \
    std::string _exp = (expected); \
    std::string _act = std::string(actual_ptr, actual_len); \
    if (_exp != _act) { \
        TEST_FAIL(std::string("ASSERTION FAILED: ") + (msg) + \
                  " | expected: \"" + _exp + "\" actual: \"" + _act + "\""); \
    } \
} while (0)

using cre2_string_t = /* forward-declared in cre2.h; use directly */ struct cre2_string_t;

// Helper: create a cre2_string_t from a std::string (allocates memory for data)
static cre2_string_t make_string(const std::string &s) {
    cre2_string_t str;
    // allocate memory for data; the tests are tolerant to whether the API
    // reuses, mutates, or reallocates internal buffers
    str.data = std::strdup(s.c_str());
    str.length = static_cast<size_t>(s.size());
    return str;
}

// Helper: free the data buffer if allocated by tests
static void free_string(cre2_string_t &str) {
    if (str.data) {
        free(str.data);
        str.data = nullptr;
        str.length = 0;
    }
}

// Forward declare the focal function signature (as implied by macros in cre2.cpp)
extern "C" {
    // The test assumes the following prototype based on the "FindAndConsumeN" macro usage:
    // int cre2_find_and_consume(const char* pattern, cre2_string_t* text, cre2_string_t* match);
    int cre2_find_and_consume(const char* pattern, cre2_string_t* text, cre2_string_t* match);
}

// Individual test cases

// Test 1: Simple match at the beginning of the text
// Input: text = "abcdef", pattern = "abc"
// Expect: match = "abc", text updated to "def", return value == 1
static void test_find_and_consume_start() {
    // Prepare input
    cre2_string_t text = make_string("abcdef");
    cre2_string_t match = { nullptr, 0 };

    int ret = cre2_find_and_consume("abc", &text, &match);

    TEST_EQ(1, ret, "FindAndConsume should return 1 on a successful match at start");
    // verify consumed part
    TEST_STR_EQ("abc", match.data, match.length, "Matched portion should be 'abc'");

    // verify remaining text after consumption
    TEST_STR_EQ("def", text.data, text.length, "Remaining text after consumption should be 'def'");

    // Cleanup
    free_string(text);
    // match may be owned by API; we avoid freeing if uncertain
}

// Test 2: No match should leave text unchanged and return 0
static void test_find_and_consume_no_match() {
    cre2_string_t text = make_string("abcdef");
    cre2_string_t match = { nullptr, 0 };

    int ret = cre2_find_and_consume("gh", &text, &match);

    TEST_EQ(0, ret, "FindAndConsume should return 0 when no match is found");
    // ensure text unchanged
    TEST_STR_EQ("abcdef", text.data, text.length, "Text should remain unchanged when no match is found");
    // match should be empty
    TEST_EQ(0, match.length, "No match should yield empty consumed string");

    free_string(text);
}

// Test 3: Match at end of text (consumes trailing segment)
// Input: text = "123xyz", pattern = "xyz" -> remaining text should be ""
static void test_find_and_consume_end() {
    cre2_string_t text = make_string("123xyz");
    cre2_string_t match = { nullptr, 0 };

    int ret = cre2_find_and_consume("xyz", &text, &match);

    TEST_EQ(1, ret, "FindAndConsume should return 1 when pattern found at end");
    TEST_STR_EQ("xyz", match.data, match.length, "Matched portion should be 'xyz'");
    TEST_EQ(0, text.length, "Remaining text should be empty after consuming end match");

    free_string(text);
}

// Test 4: Match at middle of text
// Input: text = "123abc456", pattern = "abc" -> remaining text "456"
static void test_find_and_consume_middle() {
    cre2_string_t text = make_string("123abc456");
    cre2_string_t match = { nullptr, 0 };

    int ret = cre2_find_and_consume("abc", &text, &match);

    TEST_EQ(1, ret, "FindAndConsume should return 1 for a middle match");
    TEST_STR_EQ("abc", match.data, match.length, "Matched portion should be 'abc'");
    TEST_STR_EQ("456", text.data, text.length, "Remaining text after consuming middle match should be '456'");

    free_string(text);
}

// Test 5: Empty input should return 0 (no match possible)
static void test_find_and_consume_empty_input() {
    cre2_string_t text = make_string("");
    cre2_string_t match = { nullptr, 0 };

    int ret = cre2_find_and_consume("anything", &text, &match);

    TEST_EQ(0, ret, "FindAndConsume should return 0 when input is empty");
    TEST_EQ(0, text.length, "Input length should remain 0 for empty input");
    TEST_EQ(0, match.length, "Match length should be 0 for empty input");

    free_string(text);
}

// Run all tests and report summary
static void run_all_tests() {
    test_find_and_consume_start();
    test_find_and_consume_no_match();
    test_find_and_consume_end();
    test_find_and_consume_middle();
    test_find_and_consume_empty_input();

    if (g_failures == 0) {
        std::cout << "All FindAndConsume tests passed." << std::endl;
    } else {
        std::cout << "FindAndConsume tests completed with " << g_failures << " failure(s)." << std::endl;
    }
}

int main() {
    // Domain knowledge constraints:
    // - Use only standard library and provided headers (cre2.h).
    // - Access static/global functions via their public interfaces (no private access).
    // - Use a simple custom test harness (no GTest).

    run_all_tests();
    // Exit code indicates number of failures (0 means success)
    return g_failures;
}