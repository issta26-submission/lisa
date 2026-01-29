// Test suite for cre2_named_groups_iter_new
// Purpose: exercise the focal method and basic related API surface
// Notes:
// - This test avoids GTest and uses a small in-code test harness.
// - It relies on the CRE2 API provided in cre2.h and linked against the RE2 library.
// - The tests focus on allocating the iterator and ensuring basic interactions
//   with a pattern that contains a named capturing group and a pattern without
//   any named capturing groups.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Simple test harness helpers
static bool tests_passed = true;

#define RUN_TEST(name) do {                                                      \
    if (!(name())) {                                                              \
        std::cerr << "[FAIL] " #name "\n";                                       \
        tests_passed = false;                                                     \
    } else {                                                                      \
        std::cout << "[PASS] " #name "\n";                                      \
    }                                                                             \
} while (0)

// Test 1: Named groups present
// - Compile a pattern that contains a named capturing group.
// - Call cre2_named_groups_iter_new and ensure it returns a non-null iterator.
// - Additionally, verify that the underlying regex reports at least one capturing
//   group (sanity check against miscompiled patterns).
// - Cleanup: delete the iterator and the compiled regex.
static bool test_named_groups_iter_with_named_group() {
    // Pattern containing a named capturing group. This format is supported by RE2:
    // (?P<name>pattern)
    const char pattern[] = "(?P<name>abc)";

    // Use default options (NULL)
    cre2_options_t *opt = NULL;

    // Compile the regex
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);
    if (re == NULL) {
        std::cerr << "Error: cre2_new failed for pattern with named group.\n";
        return false;
    }

    // Sanity check: number of capturing groups should be >= 1 when a named group exists.
    int ng = cre2_num_capturing_groups(re);
    if (ng < 1) {
        std::cerr << "Error: Expected >= 1 capturing group for pattern with named group, got "
                  << ng << "\n";
        cre2_delete(re);
        return false;
    }

    // Create the named groups iterator
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter == nullptr) {
        std::cerr << "Error: cre2_named_groups_iter_new returned NULL for pattern with named group.\n";
        cre2_delete(re);
        return false;
    }

    // If we reach here, cre2_named_groups_iter_new executed the focal method's code path.
    // We do not rely on cre2_named_groups_iter_next for this test to keep the test
    // robust against variations in the next() semantics. We ensure we can clean up properly.
    cre2_named_groups_iter_delete(iter);

    // Cleanup
    cre2_delete(re);

    return true;
}

// Test 2: No named groups present
// - Compile a pattern with no named capturing groups.
// - Call cre2_named_groups_iter_new and ensure it returns a non-null iterator.
// - Cleanup: delete the iterator and the compiled regex.
// Rationale: ensures the code path works when there are zero named groups and thus
// the NamedCapturingGroups vector is empty.
static bool test_named_groups_iter_without_named_group() {
    const char pattern[] = "abc"; // no capturing groups at all

    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);
    if (re == NULL) {
        std::cerr << "Error: cre2_new failed for pattern without named group.\n";
        return false;
    }

    int ng = cre2_num_capturing_groups(re);
    if (ng < 0) {
        std::cerr << "Error: Invalid capturing groups count encountered.\n";
        cre2_delete(re);
        return false;
    }

    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter == nullptr) {
        std::cerr << "Error: cre2_named_groups_iter_new returned NULL for pattern without named group.\n";
        cre2_delete(re);
        return false;
    }

    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    return true;
}

int main() {
    // Run the targeted unit tests
    RUN_TEST(test_named_groups_iter_with_named_group);
    RUN_TEST(test_named_groups_iter_without_named_group);

    // Exit code: 0 on success, non-zero on failure
    if (tests_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Some tests failed.\n";
        return 1;
    }
}