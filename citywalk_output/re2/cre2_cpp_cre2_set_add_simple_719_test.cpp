// Test suite for cre2_set_add_simple in cre2.cpp
// This test suite does not rely on GoogleTest/GMock and uses a small in-file harness.
// It validates true/false behavior of the underlying RE2 Set::Add via cre2_set_add_simple.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the C API of CRE2 (wrapped for C++ compilation)
extern "C" {
}

// Simple internal test harness to avoid terminating tests on failure
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Report test results without aborting the process
static void report_test(const char* name, bool passed) {
    ++g_total_tests;
    if (passed) {
        ++g_passed_tests;
        std::cout << "[PASSED] " << name << std::endl;
    } else {
        std::cout << "[FAILED] " << name << std::endl;
    }
}

// Helper macro to capture test name automatically for readability
#define RUN_TEST(name) (test_##name())

/*
 Candidate Keywords (Step 1 mapping):
 - cre2_set_add_simple: the focal function under test
 - cre2_set: the set type used to store regex patterns
 - RE2::Set and Add: underlying dependency (returns int id or -1 on failure)
 - TO_RE2_SET(set): conversion macro to RE2 set pointer
 - re2::StringPiece: wrapper for pattern string
 - pattern validity: valid regex vs invalid regex impacts Add result
 - duplicate handling: adding an existing pattern
 - static linkage and file-scope behavior: access via provided API
*/

// Test 1: Valid pattern should be added successfully (expected id >= 0)
// This validates the true path of the Add call within cre2_set_add_simple.
// Rationale: Basic use-case ensuring correct interaction with RE2 Set.
static bool test_add_simple_valid_pattern() {
    // Create a new CRE2 set (no specific options, anchored with default)
    cre2_set *set = cre2_set_new(nullptr, 0);
    if (!set) {
        std::cerr << "Error: cre2_set_new returned null\n";
        report_test("test_add_simple_valid_pattern", false);
        return false;
    }

    // Add a simple, valid regex pattern
    int id = cre2_set_add_simple(set, "abc");
    bool ok = (id >= 0);

    // Cleanup
    cre2_set_delete(set);

    // Explanatory comment:
    // - Ensures the true branch of Add is exercised for a valid pattern.
    return ok;
}

// Test 2: Invalid pattern should fail to add (expected id == -1)
// This validates the false path when the provided pattern cannot be parsed.
// Rationale: The Add function should gracefully report failure for invalid regex.
static bool test_add_simple_invalid_pattern() {
    cre2_set *set = cre2_set_new(nullptr, 0);
    if (!set) {
        std::cerr << "Error: cre2_set_new returned null\n";
        report_test("test_add_simple_invalid_pattern", false);
        return false;
    }

    // Add an invalid regex pattern (unbalanced parenthesis)
    int id = cre2_set_add_simple(set, "(");
    bool ok = (id == -1);

    cre2_set_delete(set);

    // Explanatory comment:
    // - Ensures the false path of Add is exercised for an invalid pattern.
    return ok;
}

// Test 3: Duplicate pattern should fail to add (expected id == -1 on second add)
// This validates handling of duplicates; Set::Add should not re-add an existing pattern.
// Rationale: Confirm proper duplicate behavior as part of the Add contract.
static bool test_add_simple_duplicate_pattern() {
    cre2_set *set = cre2_set_new(nullptr, 0);
    if (!set) {
        std::cerr << "Error: cre2_set_new returned null\n";
        report_test("test_add_simple_duplicate_pattern", false);
        return false;
    }

    int id1 = cre2_set_add_simple(set, "duplicate_pattern");
    bool first_ok = (id1 >= 0);

    int id2 = cre2_set_add_simple(set, "duplicate_pattern");
    bool second_fail = (id2 == -1);

    cre2_set_delete(set);

    // Explanatory comment:
    // - Verifies that adding the same pattern twice does not succeed the second time.
    return first_ok && second_fail;
}

int main() {
    // Run tests and report results
    report_test("test_add_simple_valid_pattern", test_add_simple_valid_pattern());
    report_test("test_add_simple_invalid_pattern", test_add_simple_invalid_pattern());
    report_test("test_add_simple_duplicate_pattern", test_add_simple_duplicate_pattern());

    // Summary
    std::cout << "\nTest summary: " << g_passed_tests << " / " << g_total_tests
              << " tests PASSED." << std::endl;

    // Return non-zero if any test failed to ease integration with CI tools
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}