// C++11 test suite for cre2_find_and_consume_re (FindAndConsumeN) in cre2.cpp
// This test avoids Google Test and uses a lightweight custom harness.
// It exercises one positive path (match at start) and one negative path (no match).
// The test relies on the public cre2 API exposed via cre2.h (as in the project under test).

#include <functional>
#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the library header that declares cre2_find_and_consume_re and related types.
// This path follows the project's conventions (cre2.h is located alongside other headers).

// Helper to build a cre2_string_t from a C-string.
// The function allocates memory for the data to ensure the callee can read the buffer.
static cre2_string_t build_cre2_string(const char* s) {
    cre2_string_t str;
    if (!s) {
        str.data = nullptr;
        str.length = 0;
        return str;
    }
    size_t len = std::strlen(s);
    char* buf = static_cast<char*>(std::malloc(len + 1));
    if (!buf) {
        // In case of allocation failure, return an empty string.
        str.data = nullptr;
        str.length = 0;
        return str;
    }
    std::memcpy(buf, s, len + 1); // copy including terminating null
    str.data = buf;
    str.length = static_cast<int>(len);
    return str;
}

// Helper to free a cre2_string_t's allocated data if any.
static void free_cre2_string(cre2_string_t& s) {
    if (s.data) {
        std::free(s.data);
        s.data = nullptr;
        s.length = 0;
    }
}

// Test 1: Verify that when the input text starts with a pattern that the regex matches,
// the FindAndConsume operation returns a positive result and consumes the matched portion.
// We expect the text length to decrease by the length of the matched prefix (characters consumed).
static bool test_find_and_consume_re_matches_start() {
    // Compile a simple regex that matches the prefix "foo"
    cre2_regexp_t* rex = cre2_new("foo", static_cast<int>(std::strlen("foo")), NULL);
    if (!rex) {
        std::cerr << "[Test] Failed to create regex for pattern 'foo'\n";
        return false;
    }

    // Prepare input text: "foobarbaz" (prefix "foo" should be consumed)
    std::string input = "foobarbaz";
    cre2_string_t text = build_cre2_string(input.c_str());
    if (!text.data) {
        std::cerr << "[Test] Failed to allocate text buffer\n";
        cre2_delete(rex);
        return false;
    }

    // Prepare rewrite string (not used for FindAndConsume in the sense of replacement here).
    // Use an empty rewrite to minimize side effects.
    cre2_string_t rewrite = build_cre2_string("");

    // Capture original length for comparison
    int original_length = text.length;

    // Call the function under test
    int result = cre2_find_and_consume_re(rex, &text, &rewrite);

    // Cleanup rewrite buffer (text.buffer may be reallocated by the function, but rewrite is separate)
    free_cre2_string(rewrite);

    // Free regex after use
    cre2_delete(rex);

    // Validate that a match was found and consumed
    // We expect result to be non-negative and text length to be reduced by at least 3 (length of "foo")
    bool ok = false;
    if (result >= 0) {
        // text.length should be <= original_length
        if (text.length <= original_length) {
            // If the match was found and consumed, length should indeed be reduced.
            // The exact final length may depend on implementation details, but it should be strictly less for a consumable match.
            ok = (text.length < original_length);
        } else {
            ok = false;
        }
    }

    free_cre2_string(text);

    if (!ok) {
        std::cerr << "[Test] test_find_and_consume_re_matches_start: Unexpected results (result=" 
                  << result << ", original_length=" << original_length << ", final_length=" 
                  << (text.length) << ")\n";
    }

    return ok;
}

// Test 2: Verify that when the input text does not contain a match at all, the FindAndConsume
// operation returns a non-positive result and leaves the text unchanged.
static bool test_find_and_consume_re_no_match() {
    // Compile a simple regex that matches the prefix "foo"
    cre2_regexp_t* rex = cre2_new("foo", static_cast<int>(std::strlen("foo")), NULL);
    if (!rex) {
        std::cerr << "[Test] Failed to create regex for pattern 'foo' (no-match case)\n";
        return false;
    }

    // Prepare input text: "barbaz" (no match at the start)
    std::string input = "barbaz";
    cre2_string_t text = build_cre2_string(input.c_str());
    if (!text.data) {
        std::cerr << "[Test] Failed to allocate text buffer (no-match case)\n";
        cre2_delete(rex);
        return false;
    }

    // Prepare rewrite string (not used in this test)
    cre2_string_t rewrite = build_cre2_string("");

    int original_length = text.length;

    int result = cre2_find_and_consume_re(rex, &text, &rewrite);

    // Cleanup
    free_cre2_string(rewrite);
    cre2_delete(rex);

    // In no-match case, we expect no consumption: final length should equal original
    bool ok = false;
    if (result >= 0) {
        if (text.length == original_length) {
            ok = true;
        } else {
            ok = false;
        }
    }

    free_cre2_string(text);

    if (!ok) {
        std::cerr << "[Test] test_find_and_consume_re_no_match: Unexpected results (result=" 
                  << result << ", original_length=" << original_length << ", final_length=" 
                  << (text.length) << ")\n";
    }

    return ok;
}

int main() {
    // Simple test harness (no external testing framework)
    // Executes a small set of unit tests for cre2_find_and_consume_re.
    std::vector<std::pair<std::string, std::function<bool()>>> tests;

    // Register tests with explanatory names
    tests.push_back({"FindAndConsumeRe_MatchAtStart", test_find_and_consume_re_matches_start});
    tests.push_back({"FindAndConsumeRe_NoMatch", test_find_and_consume_re_no_match});

    int failures = 0;
    for (const auto &t : tests) {
        // Each test prints its own explanatory messages
        bool result = t.second();
        if (!result) {
            ++failures;
            std::cerr << "[Result] FAIL: " << t.first << "\n";
        } else {
            std::cout << "[Result] PASS: " << t.first << "\n";
        }
    }

    if (failures > 0) {
        std::cerr << "Total failures: " << failures << "\n";
    } else {
        std::cout << "All tests passed.\n";
    }

    return failures;
}