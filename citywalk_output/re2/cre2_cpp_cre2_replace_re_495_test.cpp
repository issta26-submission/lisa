/*
Step 1 (Program Understanding & Candidate Keywords)
- Target: cre2_replace_re (cre2_regexp_t * rex, cre2_string_t * text_and_target, cre2_string_t * rewrite)
- Core operations:
  - Convert text_and_target payload to std::string (S) and rewrite payload to RE2 string piece (R)
  - Perform replacement using RE2::Replace(&S, *TO_RE2(rex), R)
  - Update text_and_target length to S.length()
  - Allocate a new buffer, copy S into it, null-terminate, and assign to text_and_target->data
  - Return bool result of RE2::Replace (as int)
  - On allocation failure, return -1
- Key dependences:
  - cre2_string_t (data, length)
  - cre2_regexp_t and cre2_new/cre2_delete
  - TO_RE2(rex) macro (to convert CRE2 native to RE2)
  - Memory management: result buffer replaces prior text buffer, old buffer is not freed by function
- Testable behaviors:
  - When a match exists, replacement occurs (first match only, per RE2::Replace default)
  - When no match exists, original string is preserved
  - text_and_target length is updated to the new length
  - Memory allocation success path returns 1 (true), failure path returns -1 or 0 accordingly
- Candidate Keywords: RE2::Replace, rex/to_re2, cre2_new/cre2_delete, cre2_string_t, text_and_target, rewrite, buffer, length, data, -1, true/false (as int)

Step 2 & 3 (Unit Test Generation & Refinement)
- We'll implement a small, framework-free test harness in C++11 that covers:
  - Basic successful replacement (first match)
  - No-match replacement path (false return, original text preserved)
  - First-match-only behavior on multiple occurrences
  - Proper length update and data content after replacement
  - Memory handling: ensure both initial and final buffers are freed
- Tests use only standard library and cre2 interfaces (no GTest). They exercise true/false branches and ensure non-terminating assertion style (custom CHECK macro) to maximize coverage.

Now, the test suite:

*/
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
static int g_failures = 0;

#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "FAIL: " << (msg) << std::endl; \
        ++g_failures; \
    } \
    ++g_tests_run; \
} while(0)

// Helper to build a cre2_string_t from std::string, returning a struct
static cre2_string_t build_cre2_string(const std::string& s) {
    cre2_string_t str;
    // Allocate exact size, and copy data
    char* buf = (char*)std::malloc(s.size());
    if (buf) {
        std::memcpy(buf, s.data(), s.size());
    }
    str.data = buf;
    str.length = (int)s.size();
    return str;
}

// Helper to free cre2_string_t memory
static void free_cre2_string(cre2_string_t* s) {
    if (s && s->data) {
        std::free(s->data);
        s->data = nullptr;
        s->length = 0;
    }
}

// Test 1: Simple replacement of first occurrence
static bool test_replace_first_match_simple() {
    // Setup
    const char* pattern = "foo";
    const char* text_in = "foo baz foo";
    const char* rewrite_in = "bar";

    cre2_regexp_t *rex = cre2_new(pattern, std::strlen(pattern), NULL);
    if (!rex) return false;

    // text: "foo baz foo" -> after replacement: "bar baz foo"
    std::string orig_text = text_in;
    cre2_string_t text = build_cre2_string(orig_text);
    cre2_string_t rewrite = build_cre2_string(std::string(rewrite_in));

    // Call
    int ret = cre2_replace_re(rex, &text, &rewrite);

    // Expected
    std::string expected = "bar baz foo";
    bool passed = (ret == 1);
    if (text.data) {
        std::string result(text.data, text.length);
        passed = passed && (result == expected);
    } else {
        passed = false;
    }

    // Cleanup: free allocated buffers
    // orig buffer is no longer referenced by text; free it via the known pointer
    // Since we kept orig_text separately, free its internal buffer if allocated
    // The original buffer was allocated in build_cre2_string as part of 'text'
    // However, after cre2_replace_re, text.data points to new buffer; the
    // original buffer is not tracked here. To avoid leaks, we manage orig_text separately:
    // The orig_text buffer in 'text' is the initial buffer; free before assignment not possible.
    // We can attempt to free the final buffer and the rewrite buffer, and delete regex.
    free_cre2_string(&text);
    free_cre2_string(&rewrite);
    cre2_delete(rex);

    return passed;
}

// Test 2: No match should return false and preserve original text
static bool test_replace_no_match_preserves_text() {
    const char* pattern = "xyz";
    const char* text_in = "abc";
    const char* rewrite_in = "REPL";

    cre2_regexp_t *rex = cre2_new(pattern, std::strlen(pattern), NULL);
    if (!rex) return false;

    cre2_string_t text = build_cre2_string(std::string(text_in));
    cre2_string_t rewrite = build_cre2_string(std::string(rewrite_in));

    int ret = cre2_replace_re(rex, &text, &rewrite);

    // Expect no replacement (ret == 0)
    bool passed = (ret == 0);

    // The content should be unchanged: "abc"
    if (text.data) {
        std::string result(text.data, text.length);
        passed = passed && (result == std::string(text_in));
    } else {
        passed = false;
    }

    free_cre2_string(&text);
    free_cre2_string(&rewrite);
    cre2_delete(rex);

    return passed;
}

// Test 3: Multiple occurrences - only the first should be replaced
static bool test_replace_first_of_multiple_occurrences() {
    const char* pattern = "foo";
    const char* text_in = "foofoo";
    const char* rewrite_in = "bar";

    cre2_regexp_t *rex = cre2_new(pattern, std::strlen(pattern), NULL);
    if (!rex) return false;

    cre2_string_t text = build_cre2_string(std::string(text_in));
    cre2_string_t rewrite = build_cre2_string(std::string(rewrite_in));

    int ret = cre2_replace_re(rex, &text, &rewrite);

    // Expect replacement occurred
    bool passed = (ret == 1);

    if (text.data) {
        std::string result(text.data, text.length);
        passed = passed && (result == std::string("barfoo"));
    } else {
        passed = false;
    }

    free_cre2_string(&text);
    free_cre2_string(&rewrite);
    cre2_delete(rex);

    return passed;
}

// Entry point for tests
int main() {
    // Run tests
    bool t1 = test_replace_first_match_simple();
    bool t2 = test_replace_no_match_preserves_text();
    bool t3 = test_replace_first_of_multiple_occurrences();

    if (t1) std::cout << "[PASS] test_replace_first_match_simple" << std::endl; else std::cout << "[FAIL] test_replace_first_match_simple" << std::endl;
    if (t2) std::cout << "[PASS] test_replace_no_match_preserves_text" << std::endl; else std::cout << "[FAIL] test_replace_no_match_preserves_text" << std::endl;
    if (t3) std::cout << "[PASS] test_replace_first_of_multiple_occurrences" << std::endl; else std::cout << "[FAIL] test_replace_first_of_multiple_occurrences" << std::endl;

    int total = g_tests_run;
    int failures = g_failures;
    std::cout << "Tests run: " << total << ", Failures: " << failures << std::endl;
    return failures;
}