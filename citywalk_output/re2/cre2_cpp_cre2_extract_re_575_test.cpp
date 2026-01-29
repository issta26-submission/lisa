// This file implements a small C++11 test suite (no GoogleTest) for the focal
// function cre2_extract_re defined in cre2.cpp. It exercises the behavior of
// extracting a rewritten substring from a text using a compiled CRE2 regular
// expression. The tests rely only on the standard library and the CRE2 headers
// included with the project (cre2.h, etc.). Link against the CRE2 library when
// compiling this test.
//
// Notes:
// - cre2_string_t is assumed to be a struct with fields: char* data; int length;
//   this matches usage in the focal method.
// - The test suite does not rely on private methods; it uses the public API from
//   cre2.h and cre2.cpp as provided.
// - The tests create the text and rewrite buffers as std::vector<char> to obtain
//   stable mutable data() pointers for the cre2_string_t inputs.
// - Each test cleans up allocated memory to avoid leaks.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>



// Helper to safely free target.data if allocated
static void free_target_data(cre2_string_t *target) {
    if (target && target->data) {
        free(target->data);
        target->data = NULL;
        target->length = 0;
    }
}

// Test 0: No-match scenario
// - Pattern: "notexist" (will not match the text)
// - Text: "hello world"
// - Rewrite: "$1" (not used since there is no match, but needs to be valid)
// Expected:
// - ret == 0 (false)
// - target->length == 0
// - target->data is allocated and contains empty string (terminator at [0])
static bool test_cre2_extract_re_no_match() {
    const char *pattern = "notexist";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (!rex) {
        std::cerr << "[Test 0] Failed to compile regex pattern\n";
        return false;
    }

    std::string text_str = "hello world";
    std::vector<char> text_buf(text_str.begin(), text_str.end());
    cre2_string_t text;
    text.data = text_buf.data();
    text.length = (int)text_buf.size();

    std::string rewrite_str = "$1";
    std::vector<char> rewrite_buf(rewrite_str.begin(), rewrite_str.end());
    cre2_string_t rewrite;
    rewrite.data = rewrite_buf.data();
    rewrite.length = (int)rewrite_buf.size();

    cre2_string_t target;
    target.data = NULL;
    target.length = 0;

    int ret = cre2_extract_re(rex, &text, &rewrite, &target);

    // Expect no match: ret == 0, and target should be an empty string with a terminating null.
    bool ok = (ret == 0) &&
              (target.length == 0) &&
              (target.data != NULL) &&
              (target.data[0] == '\0');

    // Cleanup
    free_target_data(&target);
    cre2_delete(rex);

    if (!ok) {
        std::cerr << "[Test 0] Failure: expected no match behavior. ret=" << ret
                  << " target.len=" << target.length << " target.data=" 
                  << (target.data ? target.data : (char*)nullptr) << "\n";
    }
    return ok;
}

// Test 1: Simple match with group extraction
// - Pattern: "([0-9]+)" (capturing group 1 contains digits)
// - Text: "abc 123 def"
// - Rewrite: "$1" (extract the captured digits)
// Expected:
// - ret == 1 (true)
// - target->length == 3
// - target->data == "123" (null-terminated)
static bool test_cre2_extract_re_simple_capture() {
    const char *pattern = "([0-9]+)";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (!rex) {
        std::cerr << "[Test 1] Failed to compile regex pattern\n";
        return false;
    }

    std::string text_str = "abc 123 def";
    std::vector<char> text_buf(text_str.begin(), text_str.end());
    cre2_string_t text;
    text.data = text_buf.data();
    text.length = (int)text_buf.size();

    std::string rewrite_str = "$1";
    std::vector<char> rewrite_buf(rewrite_str.begin(), rewrite_str.end());
    cre2_string_t rewrite;
    rewrite.data = rewrite_buf.data();
    rewrite.length = (int)rewrite_buf.size();

    cre2_string_t target;
    target.data = NULL;
    target.length = 0;

    int ret = cre2_extract_re(rex, &text, &rewrite, &target);

    // Expected
    std::string expected = "123";

    bool ok = (ret == 1) &&
              (target.length == (int)expected.size()) &&
              (target.data != NULL) &&
              (std::strncmp(target.data, expected.c_str(), target.length) == 0) &&
              (target.data[target.length] == '\0');

    // Cleanup
    free_target_data(&target);
    cre2_delete(rex);

    if (!ok) {
        std::cerr << "[Test 1] Failure: expected capture '" << expected
                  << "' got '" << (target.data ? std::string(target.data, target.length) : "") 
                  << "'; ret=" << ret << " len=" << target.length << "\n";
    }
    return ok;
}

// Test 2: Capture with a literal in rewrite (prefix)
 // - Pattern: "([0-9]+)"
 // - Text: "prefix 456 suffix"
 // - Rewrite: "ID_$1"  -> expected "ID_456"
 static bool test_cre2_extract_re_rewrite_with_literal() {
    const char *pattern = "([0-9]+)";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), NULL);
    if (!rex) {
        std::cerr << "[Test 2] Failed to compile regex pattern\n";
        return false;
    }

    std::string text_str = "prefix 456 suffix";
    std::vector<char> text_buf(text_str.begin(), text_str.end());
    cre2_string_t text;
    text.data = text_buf.data();
    text.length = (int)text_buf.size();

    std::string rewrite_str = "ID_$1";
    std::vector<char> rewrite_buf(rewrite_str.begin(), rewrite_str.end());
    cre2_string_t rewrite;
    rewrite.data = rewrite_buf.data();
    rewrite.length = (int)rewrite_buf.size();

    cre2_string_t target;
    target.data = NULL;
    target.length = 0;

    int ret = cre2_extract_re(rex, &text, &rewrite, &target);

    std::string expected = "ID_456";

    bool ok = (ret == 1) &&
              (target.length == (int)expected.size()) &&
              (target.data != NULL) &&
              (std::strncmp(target.data, expected.c_str(), target.length) == 0) &&
              (target.data[target.length] == '\0');

    // Cleanup
    free_target_data(&target);
    cre2_delete(rex);

    if (!ok) {
        std::cerr << "[Test 2] Failure: expected rewrite '" << expected
                  << "' but got '" << (target.data ? std::string(target.data, target.length) : "") 
                  << "'; ret=" << ret << " len=" << target.length << "\n";
    }
    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    // Run Test 0
    ++total;
    if (test_cre2_extract_re_no_match()) {
        ++passed;
    }

    // Run Test 1
    ++total;
    if (test_cre2_extract_re_simple_capture()) {
        ++passed;
    }

    // Run Test 2
    ++total;
    if (test_cre2_extract_re_rewrite_with_literal()) {
        ++passed;
    }

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero on failure to indicate test suite did not pass entirely.
    return (passed == total) ? 0 : 1;
}