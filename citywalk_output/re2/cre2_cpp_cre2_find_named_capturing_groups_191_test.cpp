// Test suite for cre2_find_named_capturing_groups in cre2.cpp
// This test suite uses the provided C API (no GTest) and is designed for C++11.
// It focuses on verifying the behavior of locating named capturing groups by name
// within compiled regular expressions processed by CRE2/RE2 bridging utilities.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the Cre2 C API header for the testable function and helpers.
// Adjust the include path if your build environment places cre2.h elsewhere.

/*
Candidate Keywords (from analysis):
- NamedCapturingGroups
- cre2_new / cre2_delete
- cre2_opt_new / cre2_opt_delete
- cre2_find_named_capturing_groups (target function)
- Pattern strings with named groups: (?P<name>...), multiple named groups
- Group index semantics: named group index corresponds to capture position
- Patterns with and without unnamed groups
- Error handling: return -1 for missing group name
*/

// Helper macro for simple assertions with failure messages
#define ASSERT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "ASSERTION FAILED: " << (msg) << " (file " << __FILE__ \
                      << ", line " << __LINE__ << ")" << std::endl; \
            return false; \
        } \
    } while(false)

// Test 1: Simple named group - expect index 1
static bool test_find_named_group_single() {
    // Arrange
    const char* pattern = "(?P<name>abc)";
    int pat_len = static_cast<int>(std::strlen(pattern));

    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (re == nullptr) {
        std::cerr << "Failed to compile pattern for test_find_named_group_single" << std::endl;
        cre2_opt_delete(opt);
        return false;
    }

    // Act
    int idx = cre2_find_named_capturing_groups(re, "name");

    // Assert
    ASSERT_TRUE(idx == 1, "Expected named group 'name' to have index 1, got " << idx);

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    return true;
}

// Test 2: Two named groups - verify both indices
static bool test_find_named_group_two_names() {
    // Arrange
    const char* pattern = "(?P<g1>a)(?P<g2>b)";
    int pat_len = static_cast<int>(std::strlen(pattern));

    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (re == nullptr) {
        std::cerr << "Failed to compile pattern for test_find_named_group_two_names" << std::endl;
        cre2_opt_delete(opt);
        return false;
    }

    // Act
    int idx_g1 = cre2_find_named_capturing_groups(re, "g1");
    int idx_g2 = cre2_find_named_capturing_groups(re, "g2");

    // Assert
    ASSERT_TRUE(idx_g1 == 1, "Expected named group 'g1' to have index 1, got " << idx_g1);
    ASSERT_TRUE(idx_g2 == 2, "Expected named group 'g2' to have index 2, got " << idx_g2);

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    return true;
}

// Test 3: Unnamed capture before named group - ensure index reflects position
static bool test_find_named_group_with_unnamed_before() {
    // Arrange
    // First capturing group is unnamed, second is named 'id'
    const char* pattern = "([0-9]+)(?P<id>abc)";
    int pat_len = static_cast<int>(std::strlen(pattern));

    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (re == nullptr) {
        std::cerr << "Failed to compile pattern for test_find_named_group_with_unnamed_before" << std::endl;
        cre2_opt_delete(opt);
        return false;
    }

    // Act
    int idx_id = cre2_find_named_capturing_groups(re, "id");

    // Assert
    // The unnamed group before should push the named group's index to 2
    ASSERT_TRUE(idx_id == 2, "Expected named group 'id' to have index 2, got " << idx_id);

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    return true;
}

// Test 4: No named groups present - should return -1 for any lookup
static bool test_find_named_group_no_named() {
    // Arrange
    const char* pattern = "abc"; // no capturing groups at all
    int pat_len = static_cast<int>(std::strlen(pattern));

    cre2_options_t *opt = cre2_opt_new();
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (re == nullptr) {
        std::cerr << "Failed to compile pattern for test_find_named_group_no_named" << std::endl;
        cre2_opt_delete(opt);
        return false;
    }

    // Act
    int idx = cre2_find_named_capturing_groups(re, "anything");

    // Assert
    ASSERT_TRUE(idx == -1, "Expected -1 for missing named group, got " << idx);

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    return true;
}

// Main: Run all tests and report summary
int main() {
    int failures = 0;

    if(!test_find_named_group_single()) {
        ++failures;
        std::cerr << "[Test 1] test_find_named_group_single failed." << std::endl;
    } else {
        std::cout << "[Test 1] Passed: test_find_named_group_single" << std::endl;
    }

    if(!test_find_named_group_two_names()) {
        ++failures;
        std::cerr << "[Test 2] test_find_named_group_two_names failed." << std::endl;
    } else {
        std::cout << "[Test 2] Passed: test_find_named_group_two_names" << std::endl;
    }

    if(!test_find_named_group_with_unnamed_before()) {
        ++failures;
        std::cerr << "[Test 3] test_find_named_group_with_unnamed_before failed." << std::endl;
    } else {
        std::cout << "[Test 3] Passed: test_find_named_group_with_unnamed_before" << std::endl;
    }

    if(!test_find_named_group_no_named()) {
        ++failures;
        std::cerr << "[Test 4] test_find_named_group_no_named failed." << std::endl;
    } else {
        std::cout << "[Test 4] Passed: test_find_named_group_no_named" << std::endl;
    }

    if(failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << failures << " test(s) failed." << std::endl;
    }

    return failures;
}