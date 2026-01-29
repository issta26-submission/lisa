// Lightweight C++11 unit test suite for cre2_num_capturing_groups
// Purpose: verify that the wrapper correctly reports the number of capturing groups
// in various regular expression patterns by leveraging the CRE2 wrappers provided
// in cre2.h, without relying on a test framework like GoogleTest.

// This test suite uses a simple non-terminating assertion style.
// On failure, it reports details to std::cerr but continues executing other tests.

#include <cre2.h>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>



// Helper: run a single test case and report failure details without aborting the test run.
// test_name: human-readable name of the test
// pattern: regex pattern to compile
// expected: expected number of capturing groups
static bool run_and_check(const std::string& test_name,
                          const std::string& pattern,
                          int expected)
{
    // Create default CRE2 options
    cre2_options_t* opt = cre2_opt_new();
    if (!opt) {
        std::cerr << "[ERR] " << test_name
                  << ": failed to allocate CRE2 options for pattern '"
                  << pattern << "'\n";
        return false;
    }

    // Compile the regular expression
    cre2_regexp_t* re = cre2_new(pattern.c_str(), (int)pattern.size(), opt);
    if (!re) {
        std::cerr << "[ERR] " << test_name
                  << ": failed to compile pattern '" << pattern << "'\n";
        cre2_opt_delete(opt);
        return false;
    }

    // Invoke the focal function under test
    int got = cre2_num_capturing_groups(re);

    // Clean up resources
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Non-terminating assertion: report mismatch but do not abort
    bool ok = (got == expected);
    if (!ok) {
        std::cerr << "[FAIL] " << test_name
                  << ": pattern='" << pattern
                  << "' expected capturing groups=" << expected
                  << " but got " << got << "\n";
    }

    return ok;
}

/*
  Test 1: No capturing groups
  Pattern "abc" contains zero capturing parentheses.
*/
static bool test_no_capturing_groups()
{
    // Expected: 0 capturing groups
    return run_and_check("NoCapturingGroups", "abc", 0);
}

/*
  Test 2: Exactly one capturing group
  Pattern "(abc)" has a single capturing group.
*/
static bool test_one_group()
{
    return run_and_check("OneCapturingGroup", "(abc)", 1);
}

/*
  Test 3: Two capturing groups
  Pattern "(a)(b)" has two separate capturing groups.
*/
static bool test_two_groups()
{
    return run_and_check("TwoCapturingGroups", "(a)(b)", 2);
}

/*
  Test 4: Nested capturing groups
  Pattern "((a)(b))" has an outer group and two inner groups: total 3.
*/
static bool test_nested_groups()
{
    return run_and_check("NestedCapturingGroups", "((a)(b))", 3);
}

/*
  Test 5: Non-capturing groups
  Pattern "(?:abc)" uses a non-capturing group; should not increment count.
*/
static bool test_non_capturing_groups()
{
    return run_and_check("NonCapturingGroupsOnly", "(?:abc)", 0);
}

/*
  Test 6: Mixed capturing and non-capturing groups
  Pattern "((a)(?:b))(c)" contains 3 capturing groups in total:
  - outer: 1
  - inner "(a)": 1
  - inner "(?:b)": non-capturing
  - trailing "(c)": 1
*/
static bool test_mixed_groups()
{
    return run_and_check("MixedCapturingNonCapturing", "((a)(?:b))(c)", 3);
}

/*
  Test 7: Zero-length capturing group
  Pattern "()" is a capturing group with zero length.
*/
static bool test_empty_capture_group()
{
    return run_and_check("EmptyCapturingGroup", "()", 1);
}

// Main: run all tests and provide a final summary.
int main()
{
    std::cout << "Starting cre2_num_capturing_groups unit tests (C++11, no GTest)\n";

    int failures = 0;

    if (!test_no_capturing_groups()) ++failures;
    if (!test_one_group()) ++failures;
    if (!test_two_groups()) ++failures;
    if (!test_nested_groups()) ++failures;
    if (!test_non_capturing_groups()) ++failures;
    if (!test_mixed_groups()) ++failures;
    if (!test_empty_capture_group()) ++failures;

    if (failures == 0) {
        std::cout << "[ALL TESTS PASSED]\n";
        return 0;
    } else {
        std::cerr << "[TESTS FAILED] " << failures << " test(s) did not pass.\n";
        return 1;
    }
}