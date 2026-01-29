/*
  Unit test suite for cre2_named_groups_iter_next (focal method)
  - No GTest or GMock usage (standalone test executable)
  - Uses C++11 standard library only
  - Builds with the provided cre2.cpp / cre2.h (focal method relies on RE2-like API)
  - Tests:
      1) End condition when there are no named capturing groups
      2) Iteration over named capturing groups for a pattern with two named groups
  - Explanatory comments accompany each test
*/

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Helper macro for lightweight assertion that logs but does not abort program immediately
#define ASSERT_LOG(cond, msg)                            \
    do {                                                 \
        if (!(cond)) {                                   \
            std::cerr << "Assertion failed: " << (msg)  \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
            return false;                                \
        }                                                \
    } while (0)

// Test 1: End condition when there are no named capturing groups
// - Create a regex with no named groups (e.g., "abc")
// - Create an iterator over named groups
// - The first call to cre2_named_groups_iter_next should return false
//   and set name to NULL and index to -1
static bool test_no_named_groups_end_condition() {
    // Pattern with no named capturing groups
    const char *pattern = "abc";
    int pattern_len = std::strlen(pattern);

    // Create a compiled regex object
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, NULL);
    ASSERT_LOG(re != NULL, "cre2_new returned NULL for pattern with no named groups");

    // Create an iterator for named groups
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    ASSERT_LOG(iter != NULL, "cre2_named_groups_iter_new returned NULL");

    // Call next: should be end immediately
    const char *name = NULL;
    int index = -1;
    bool has_next = cre2_named_groups_iter_next(iter, &name, &index);

    // Validate end condition
    ASSERT_LOG(has_next == false, "Expected false on end condition for no named groups");
    ASSERT_LOG(name == NULL, "Expected name to be NULL on end condition");
    ASSERT_LOG(index == -1, "Expected index to be -1 on end condition");

    // Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    return true;
}

// Test 2: Iteration over two named capturing groups
// - Pattern includes two named groups using RE2-compatible syntax
//   e.g., "(?P<first>a)(?P<second>b)" (two named groups)
// - The first call should return true with name == "first" and index == 1
// - The second call should return true with name == "second" and index == 2
// - The third call should return false (end of iteration)
static bool test_named_groups_iteration_two_entries() {
    // Pattern with two named capturing groups
    // Note: The syntax here assumes RE2-compatible named groups like (?P<name>...).
    const char *pattern = "(?P<first>a)(?P<second>b)";
    int pattern_len = std::strlen(pattern);

    // Create a compiled regex object
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, NULL);
    ASSERT_LOG(re != NULL, "cre2_new failed for pattern with two named groups");

    // Create an iterator over named groups
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    ASSERT_LOG(iter != NULL, "cre2_named_groups_iter_new returned NULL for two named groups");

    // First iteration
    const char *name1 = NULL;
    int index1 = -1;
    bool has_next1 = cre2_named_groups_iter_next(iter, &name1, &index1);
    ASSERT_LOG(has_next1 == true, "First call to cre2_named_groups_iter_next should return true");
    ASSERT_LOG(name1 != NULL, "First name should not be NULL");
    ASSERT_LOG(index1 == 1, "First named group index should be 1 (for first group)");
    if (name1) {
        std::cout << "First named group: " << name1 << ", index: " << index1 << "\n";
        // Optional: validate content matches expected
        ASSERT_LOG(std::strcmp(name1, "first") == 0, "First named group name should be 'first'");
    }

    // Second iteration
    const char *name2 = NULL;
    int index2 = -1;
    bool has_next2 = cre2_named_groups_iter_next(iter, &name2, &index2);
    ASSERT_LOG(has_next2 == true, "Second call to cre2_named_groups_iter_next should return true");
    ASSERT_LOG(name2 != NULL, "Second name should not be NULL");
    ASSERT_LOG(index2 == 2, "Second named group index should be 2 (for second group)");
    if (name2) {
        std::cout << "Second named group: " << name2 << ", index: " << index2 << "\n";
        // Optional: validate content matches expected
        ASSERT_LOG(std::strcmp(name2, "second") == 0, "Second named group name should be 'second'");
    }

    // Third iteration should be end
    const char *name3 = NULL;
    int index3 = -1;
    bool has_next3 = cre2_named_groups_iter_next(iter, &name3, &index3);
    ASSERT_LOG(has_next3 == false, "Third call should indicate end of named groups");
    ASSERT_LOG(name3 == NULL, "Third name should be NULL at end");
    ASSERT_LOG(index3 == -1, "Third index should be -1 at end");

    // Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);

    return true;
}

int main() {
    int total = 0;
    int failures = 0;

    std::cout << "Running cre2_named_groups_iter_next unit tests (C++11, no GTest)..." << std::endl;

    total = 2;

    // Run Test 1
    std::cout << "Test 1: End condition with no named groups... ";
    bool t1 = test_no_named_groups_end_condition();
    if (t1) {
        std::cout << "PASSED\n";
    } else {
        std::cout << "FAILED\n";
        ++failures;
    }

    // Run Test 2
    std::cout << "Test 2: Iteration over two named groups... ";
    bool t2 = test_named_groups_iteration_two_entries();
    if (t2) {
        std::cout << "PASSED\n";
    } else {
        std::cout << "FAILED\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests PASSED (" << total << " tests).\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED out of " << total << " tests.\n";
        return 1;
    }
}