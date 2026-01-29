// Lightweight C++11 unit tests for cre2_set_match (no GoogleTest)
// This test suite targets cre2_set_match defined in cre2.cpp using the
// cre2 C-API wrappers provided by cre2.h and the underlying RE2::Set backend.
// It uses a small custom test harness to avoid terminating on failure and
// to maximize code coverage by exercising true/false branches and boundary behavior.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the project's CRE2 wrappers (C API)
extern "C" {
}

// Simple non-terminating test harness
static int g_failed_tests = 0;

static void log_fail(const std::string& test_name, const char* msg) {
    std::cerr << "[FAIL] " << test_name << ": " << msg << std::endl;
    ++g_failed_tests;
}

// Test 1: No match scenario
// - Create a set with a single pattern "foo"
// - Query with text that does not contain "foo"
// - Expect return value 0 (no matches)
static bool test_no_match() {
    const char* test_name = "test_no_match";
    // Create a new CRE2 set with a benign anchor (0 cast to cre2_anchor_t to avoid enum name specificity)
    cre2_set *set = cre2_set_new(nullptr, (cre2_anchor_t)0);
    if (!set) {
        log_fail(test_name, "Failed to create CRE2 set");
        return false;
    }

    // Add a simple pattern
    cre2_set_add_simple(set, "foo");

    // Compile the set
    cre2_set_compile(set);

    // Prepare output buffer (2 slots) and initialize to sentinel value to detect overwrites
    int match_out[2] = { -1, -1 };

    // Try text that does not contain "foo"
    const char* text = "bar baz qux";
    size_t text_len = std::strlen(text);

    int matches_found = cre2_set_match(set, text, text_len, match_out, 2);

    bool ok = (matches_found == 0) && (match_out[0] == -1) && (match_out[1] == -1);
    if (!ok) {
        log_fail(test_name, "Expected 0 matches and no writes to output buffer");
    }

    cre2_set_delete(set);
    return ok;
}

// Test 2: Single match with copy boundary
// - Create a set with a single pattern "foo"
// - Query with text that contains "foo"
// - Use match_len = 2 and verify that only the first element is copied
//   and the second slot remains untouched (-1 sentinel)
static bool test_single_match_copy_boundary() {
    const char* test_name = "test_single_match_copy_boundary";

    cre2_set *set = cre2_set_new(nullptr, (cre2_anchor_t)0);
    if (!set) {
        log_fail(test_name, "Failed to create CRE2 set");
        return false;
    }

    cre2_set_add_simple(set, "foo");
    cre2_set_compile(set);

    int match_out[2] = { -1, -1 };

    const char* text = "this text contains foo";
    int ret = cre2_set_match(set, text, std::strlen(text), match_out, 2);

    bool ok = (ret == 1) && (match_out[0] == 0) && (match_out[1] == -1);
    if (!ok) {
        log_fail(test_name, "Expected one match (index 0) and second slot untouched (-1)");
    }

    cre2_set_delete(set);
    return ok;
}

// Test 3: Multiple matches with full copy when match_len >= number_of_matches
// - Create a set with two patterns: "foo" and "foo.*"
// - Query with text that matches both patterns (e.g., "foo bar")
// - Expect two matches and exact set of indices {0,1} in any order
static bool test_two_matches_full_copy() {
    const char* test_name = "test_two_matches_full_copy";

    cre2_set *set = cre2_set_new(nullptr, (cre2_anchor_t)0);
    if (!set) {
        log_fail(test_name, "Failed to create CRE2 set");
        return false;
    }

    // Add two patterns
    cre2_set_add_simple(set, "foo");     // pattern index 0
    cre2_set_add_simple(set, "foo.*");   // pattern index 1
    cre2_set_compile(set);

    int match_out[2] = { -1, -1 };

    const char* text = "foo bar";
    int ret = cre2_set_match(set, text, std::strlen(text), match_out, 2);

    bool ok = (ret == 2);
    if (ok) {
        // Sort the two results to compare irrespective of order
        std::vector<int> got = { match_out[0], match_out[1] };
        std::sort(got.begin(), got.end());
        std::vector<int> expected = {0, 1};
        if (got != expected) {
            ok = false;
            log_fail(test_name, "Expected match indices {0,1} in any order");
        }
    } else {
        log_fail(test_name, "Expected 2 matches for input 'foo bar'");
    }

    cre2_set_delete(set);
    return ok;
}

// Entry point: run all tests and summarize
int main() {
    // Run tests
    bool t1 = test_no_match();
    bool t2 = test_single_match_copy_boundary();
    bool t3 = test_two_matches_full_copy();

    // Summary
    int total = 3;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);

    std::cout << "Test results: " << passed << " / " << total << " tests passed." << std::endl;
    if (g_failed_tests > 0) {
        std::cerr << g_failed_tests << " failure(s) detected." << std::endl;
    }

    // Return non-zero if any test failed
    return g_failed_tests > 0 ? 1 : 0;
}