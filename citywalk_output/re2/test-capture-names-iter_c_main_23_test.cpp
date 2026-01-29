/*
 * C++11 test suite for the focal main() functionality provided in the
 * C source (libcre2 based) using the CRE2 API.
 *
 * The tests exercise the following core components (Candidate Keywords):
 * - cre2_new, cre2_delete
 * - cre2_named_groups_iter_new, cre2_named_groups_iter_next
 * - cre2_match (for a broader integration test)
 * - Handling of named groups and positional indices
 *
 * Note: This test suite is designed to be compiled and linked against the
 * CRE2 C library in a C++11 project without using Google Test.
 * It provides a small, lightweight test harness that runs a set of tests
 * and reports results to stdout/stderr.
 */

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <cre2-test.h>
#include <iostream>
#include <cstdio>



// Lightweight test harness helpers
static bool test_empty_group_iterator_no_groups();
static bool test_named_group_dot_iterator();
static bool test_named_groups_sequence_positions();
static bool test_large_pattern_match_and_group_iteration();

// Utility to report test results
static bool run_one(const char* name, bool (*test_func)()) {
    bool ok = test_func();
    if (ok) {
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cerr << "[FAIL] " << name << "\n";
    }
    return ok;
}

int main(void) {
    int overall = 0;

    overall += run_one("test_empty_group_iterator_no_groups", test_empty_group_iterator_no_groups);
    overall += run_one("test_named_group_dot_iterator", test_named_group_dot_iterator);
    overall += run_one("test_named_groups_sequence_positions", test_named_groups_sequence_positions);
    overall += run_one("test_large_pattern_match_and_group_iteration", test_large_pattern_match_and_group_iteration);

    if (overall == 4) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Some tests failed. (" << (4 - overall) << "/4) \n";
        return 1;
    }
}

/*
 * Test 1: Empty named groups iterator on a literal pattern.
 * - Build a regex with pattern "ciao" (no named groups).
 * - Create a named groups iterator.
 * - Expect that the first call to cre2_named_groups_iter_next returns false
 *   (no groups), and that name == NULL and pos == -1.
 *
 * This tests the false-branch behavior and proper initialization/cleanup.
 */
static bool test_empty_group_iterator_no_groups() {
    cre2_regexp_t * rex = cre2_new("ciao", 4, NULL);
    if (!rex) {
        std::cerr << "Failed to create rex for test_empty_group_iterator_no_groups\n";
        return false;
    }
    cre2_named_groups_iter_t * it = cre2_named_groups_iter_new(rex);
    if (!it) {
        std::cerr << "Failed to create named groups iterator for test_empty_group_iterator_no_groups\n";
        cre2_delete(rex);
        return false;
    }

    const char * name = nullptr;
    int pos = -2;
    bool ret = cre2_named_groups_iter_next(it, &name, &pos);
    // Expect no groups -> ret should be false
    if (ret) {
        std::cerr << "Unexpected capture in test_empty_group_iterator_no_groups: ret=true\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (name != nullptr) {
        std::cerr << "Unexpected non-null name in test_empty_group_iterator_no_groups\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (pos != -1) {
        std::cerr << "Unexpected pos in test_empty_group_iterator_no_groups: pos=" << pos << "\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }

    cre2_named_groups_iter_delete(it);
    cre2_delete(rex);
    return true;
}

/*
 * Test 2: Named groups iterator on a simple capturing pattern with a named group "dot".
 * - Pattern: "((.)(?P<dot>.))"
 * - First next() should succeed, yielding pos == 3 and name == "dot".
 * - Second next() should fail, with name == NULL and pos == -1.
 *
 * This validates correct extraction and ordering of named groups.
 */
static bool test_named_group_dot_iterator() {
    const char pat[] = "((.)(?P<dot>.))";
    cre2_regexp_t * rex = cre2_new(pat, sizeof(pat), NULL);
    if (!rex) {
        std::cerr << "Failed to create rex for test_named_group_dot_iterator\n";
        return false;
    }
    cre2_named_groups_iter_t * it = cre2_named_groups_iter_new(rex);
    if (!it) {
        std::cerr << "Failed to create named groups iterator for test_named_group_dot_iterator\n";
        cre2_delete(rex);
        return false;
    }

    const char * name = nullptr;
    int pos = -2;
    bool ret = cre2_named_groups_iter_next(it, &name, &pos);
    if (!ret) {
        std::cerr << "Expected first group in test_named_group_dot_iterator but got none\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (pos != 3) {
        std::cerr << "Unexpected pos in test_named_group_dot_iterator: pos=" << pos << "\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (name == nullptr || std::strcmp(name, "dot") != 0) {
        std::cerr << "Expected named group 'dot' in test_named_group_dot_iterator, got '" 
                  << (name ? name : "nullptr") << "'\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }

    // Second call should yield no more groups
    ret = cre2_named_groups_iter_next(it, &name, &pos);
    if (ret) {
        std::cerr << "Expected no more groups in test_named_group_dot_iterator, but ret=true\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (name != nullptr) {
        std::cerr << "Expected name to be NULL on end of groups in test_named_group_dot_iterator\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (pos != -1) {
        std::cerr << "Expected pos to be -1 on end of groups in test_named_group_dot_iterator, got " << pos << "\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }

    cre2_named_groups_iter_delete(it);
    cre2_delete(rex);
    return true;
}

/*
 * Test 3: Named groups over a pattern with mixed named and unnamed groups.
 * - Pattern: "(unnamed_cap): (?P<year>\\d{4})-(?P<month>\\d{2})(-)(?P<day>\\d{2})"
 * - Iterate three times, storing group names by their positional index (pos-1).
 * - Validate that positions 1 -> "year", 2 -> "month", and 4 -> "day" exist.
 * - After three iterations, ensure there are no more groups (ret==false, name==NULL, pos==-1).
 *
 * This tests the correct handling of positional indexing and named groups.
 */
static bool test_named_groups_sequence_positions() {
    const char pat[] = "(unnamed_cap): (?P<year>\\d{4})-(?P<month>\\d{2})(-)(?P<day>\\d{2})";
    cre2_regexp_t * rex = cre2_new(pat, sizeof(pat), NULL);
    if (!rex) {
        std::cerr << "Failed to create rex for test_named_groups_sequence_positions\n";
        return false;
    }
    cre2_named_groups_iter_t * it = cre2_named_groups_iter_new(rex);
    if (!it) {
        std::cerr << "Failed to create named groups iterator for test_named_groups_sequence_positions\n";
        cre2_delete(rex);
        return false;
    }

    const char * name = nullptr;
    int pos = -2;
    const char* names[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

    // First group
    bool ret = cre2_named_groups_iter_next(it, &name, &pos);
    if (!ret) {
        std::cerr << "First group missing in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (pos < 1 || pos > 5) {
        std::cerr << "First group has invalid pos in test_named_groups_sequence_positions: pos=" << pos << "\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    names[pos - 1] = name;

    // Second group
    ret = cre2_named_groups_iter_next(it, &name, &pos);
    if (!ret) {
        std::cerr << "Second group missing in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (pos < 1 || pos > 5) {
        std::cerr << "Second group has invalid pos in test_named_groups_sequence_positions: pos=" << pos << "\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    names[pos - 1] = name;

    // Third group
    ret = cre2_named_groups_iter_next(it, &name, &pos);
    if (!ret) {
        std::cerr << "Third group missing in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (pos < 1 || pos > 5) {
        std::cerr << "Third group has invalid pos in test_named_groups_sequence_positions: pos=" << pos << "\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    names[pos - 1] = name;

    // Next call should yield no more groups
    ret = cre2_named_groups_iter_next(it, &name, &pos);
    if (ret) {
        std::cerr << "Expected end of groups after three iterations in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (name != nullptr || pos != -1) {
        std::cerr << "Expected end state (name==NULL, pos==-1) after three iterations in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }

    // Verify specific named groups by their expected positions
    if (names[1] == nullptr || std::strcmp(names[1], "year") != 0) {
        std::cerr << "Expected position 2 to be named 'year' in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (names[2] == nullptr || std::strcmp(names[2], "month") != 0) {
        std::cerr << "Expected position 3 to be named 'month' in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }
    if (names[4] == nullptr || std::strcmp(names[4], "day") != 0) {
        std::cerr << "Expected position 5 to be named 'day' in test_named_groups_sequence_positions\n";
        cre2_named_groups_iter_delete(it);
        cre2_delete(rex);
        return false;
    }

    cre2_named_groups_iter_delete(it);
    cre2_delete(rex);
    return true;
}

/*
 * Test 4: Large pattern with many named groups to ensure end-to-end path is safe.
 * - Build a large rex_pattern with months and many named groups.
 * - Run cre2_match on the sample text to ensure a match is found.
 * - Create an iterator and walk all named groups, asserting that at least one group is returned.
 *
 * This tests integration of match() with the named groups iterator.
 */
static bool test_large_pattern_match_and_group_iteration() {
    // Reuse the sample months pattern (as in the original code) with many named groups.
    const char rex_pattern[] =
        "January:[[:blank:]]+(?P<january>[[:digit:]]+)\\n\
February:[[:blank:]]+(?P<january>[[:digit:]]+)\\n\
March:[[:blank:]]+(?P<march>[[:digit:]]+)\\n\
April:[[:blank:]]+(?P<april>[[:digit:]]+)\\n\
May:[[:blank:]]+(?P<may>[[:digit:]]+)\\n\
June:[[:blank:]]+(?P<june>[[:digit:]]+)\\n\
July:[[:blank:]]+(?P<july>[[:digit:]]+)\\n\
August:[[:blank:]]+(?P<august>[[:digit:]]+)\\n\
September:[[:blank:]]+(?P<september>[[:digit:]]+)\\n\
October:[[:blank:]]+(?P<october>[[:digit:]]+)\\n\
November:[[:blank:]]+(?P<november>[[:digit:]]+)\\n\
December:[[:blank:]]+(?P<december>[[:digit:]]+)\\n";

    const char text[] =
        "\
January: 8\\n\
February: 3\\n\
March: 3\\n\
April: 4\\n\
May: 9\\n\
June: 4\\n\
July: 7\\n\
August: 5\\n\
September: 9\\n\
October: 2\\n\
November: 1\\n\
December: 6\\n";

    int text_len = std::strlen(text);
    int nmatch = 20;
    cre2_string_t match[nmatch];

    cre2_regexp_t * rex = cre2_new(rex_pattern, std::strlen(rex_pattern), NULL);
    if (!rex) {
        std::cerr << "Failed to build rex in test_large_pattern_match_and_group_iteration\n";
        return false;
    }
    if (cre2_error_code(rex)) {
        std::cerr << "Error code set after building rex in test_large_pattern_match_and_group_iteration\n";
        cre2_delete(rex);
        return false;
    }

    int rv = cre2_match(rex, text, text_len, 0, text_len, CRE2_ANCHOR_BOTH, match, nmatch);
    if (!rv) {
        std::cerr << "Pattern did not match input in test_large_pattern_match_and_group_iteration\n";
        cre2_delete(rex);
        return false;
    }

    cre2_named_groups_iter_t * iter = cre2_named_groups_iter_new(rex);
    if (!iter) {
        std::cerr << "Failed to create named groups iterator in test_large_pattern_match_and_group_iteration\n";
        cre2_delete(rex);
        return false;
    }

    const char * gname = nullptr;
    int gindex = -1;
    int count = 0;
    while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
        // Just ensure we can iterate; record count for basic coverage
        if (gname) {
            ++count;
            // Optional: print for debugging (commented out to avoid noisy tests)
            // std::cout << "Group: " << gindex << ", " << gname << "\n";
        } else {
            // If a group name is NULL, still counts? We only count non-null names here.
        }
    }

    // Ensure at least one named group was iterated
    if (count <= 0) {
        std::cerr << "No named groups iterated in test_large_pattern_match_and_group_iteration\n";
        cre2_named_groups_iter_delete(iter);
        cre2_delete(rex);
        return false;
    }

    cre2_named_groups_iter_delete(iter);
    cre2_delete(rex);
    return true;
}