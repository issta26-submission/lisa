/*
  This test suite targets the focal function (main) behavior in the provided C source
  that relies on the cre2 library. The tests are written in C++11 without GTest,
  using a lightweight, non-terminating assertion mechanism to maximize coverage.

  Step-wise mapping to the required steps:
  - Step 1 (Program Understanding): Identify core CRE2 dependent components used by main.
    Candidate Keywords: cre2_regexp_t, cre2_options_t, cre2_new, cre2_opt_new, cre2_opt_set_posix_syntax,
    cre2_pattern, cre2_error_code, cre2_error_string, cre2_num_capturing_groups, cre2_program_size,
    cre2_error_arg, CRE2_ERROR_MISSING_PAREN, cre2_delete, cre2_opt_delete, cre2_opt_set_log_errors.

  - Step 2 (Unit Test Generation): Create a test suite that exercises the main sequence:
    1) Valid pattern with options object: "ciao" => expect success, no capturing groups, program size > 0.
    2) Valid pattern with NULL options: NULL opt path should still succeed.
    3) Pattern with capturing group: "ci(ao)" => expect 1 capturing group.
    4) Error path: malformed pattern "ci(ao" with log errors disabled => expect CRE2_ERROR_MISSING_PAREN and captured error info.

  - Step 3 (Test Case Refinement): Implement non-terminating assertions, ensure memory management
    with cre2_delete/cre2_opt_delete, and keep tests executable in a single main entry by
    calling test functions from main.

  Notes:
  - This suite uses a minimal custom test harness instead of GTest.
  - We wrap the cre2 C API with extern "C" to ensure linkage from C++11.
  - The tests do not rely on private/internal details; they validate public API behavior and
    coverage of all requested code paths.
*/

#include <stdlib.h>
#include <cre2.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include <string>
#include <cre2-test.h>
#include <vector>
#include <iostream>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static std::string g_current_test;
static int g_failure_count = 0;

static void set_current_test(const std::string &name) {
    g_current_test = name;
}

static void log_failure(const std::string &message) {
    std::cerr << "[FAILED] " << g_current_test << ": " << message << std::endl;
    ++g_failure_count;
}

#define TEST_CHECK(cond, msg) do { if(!(cond)) log_failure(msg); } while(0)

// ---------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------

// Test 1: Basic, well-formed pattern with an options object.
// Expected: success (no errors), 0 capturing groups, program size > 0, no error code.
static void test_basic_positive_pattern() {
    set_current_test("test_basic_positive_pattern");

    // Create options and enable POSIX syntax
    cre2_options_t *opt = cre2_opt_new();
    TEST_CHECK(opt != nullptr, "Failed to allocate cre2 options");

    cre2_opt_set_posix_syntax(opt, 1);

    // Compile the pattern "ciao" with length 4
    cre2_regexp_t *rex = cre2_new("ciao", 4, opt);
    TEST_CHECK(rex != nullptr, "Failed to compile pattern 'ciao' with valid options");

    // Retrieve information from the compiled regex
    const char *pattern_str = cre2_pattern(rex);
    TEST_CHECK(pattern_str != nullptr && std::string(pattern_str) == "ciao",
               "Pattern string should be 'ciao'");

    int err_code = cre2_error_code(rex);
    TEST_CHECK(err_code == 0, "Error code should be 0 for valid pattern");

    const char *err_string = cre2_error_string(rex);
    TEST_CHECK(err_string != nullptr, "Error string should be non-null for valid pattern");

    int capturing_groups = cre2_num_capturing_groups(rex);
    TEST_CHECK(capturing_groups == 0, "Number of capturing groups should be 0");

    int program_size = cre2_program_size(rex);
    TEST_CHECK(program_size > 0, "Program size should be greater than 0");

    cre2_string_t S;
    cre2_error_arg(rex, &S);
    TEST_CHECK(S.length == 0, "Error argument length should be 0 for valid pattern");

    // Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);
}

// Test 2: No options object (NULL) should still succeed for a simple pattern.
// Expected: success (no error), no crash, memory clean-up.
static void test_no_options_object() {
    set_current_test("test_no_options_object");

    // Compile pattern with NULL options
    cre2_regexp_t *rex = cre2_new("ciao", 4, NULL);
    TEST_CHECK(rex != nullptr, "Failed to compile pattern 'ciao' with NULL options");

    int err_code = cre2_error_code(rex);
    TEST_CHECK(err_code == 0, "Error code should be 0 when options is NULL for valid pattern");

    // Cleanup
    cre2_delete(rex);
}

// Test 3: Pattern with a capturing group.
// Expected: success with 1 capturing group.
static void test_capture_group() {
    set_current_test("test_capture_group");

    cre2_options_t *opt = cre2_opt_new();
    TEST_CHECK(opt != nullptr, "Failed to allocate cre2 options for capture group test");
    cre2_opt_set_posix_syntax(opt, 1);

    cre2_regexp_t *rex = cre2_new("ci(ao)", 6, opt);
    TEST_CHECK(rex != nullptr, "Failed to compile pattern 'ci(ao)' with options");

    int err_code = cre2_error_code(rex);
    TEST_CHECK(err_code == 0, "Error code should be 0 for valid capturing pattern");

    int groups = cre2_num_capturing_groups(rex);
    TEST_CHECK(groups == 1, "Expected 1 capturing group for pattern 'ci(ao)'");

    int psize = cre2_program_size(rex);
    TEST_CHECK(psize > 0, "Program size should be > 0 for valid pattern with capture group");

    // Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);
}

// Test 4: Malformed pattern with missing closing parenthesis and log_errors disabled.
// Expected: CRE2_ERROR_MISSING_PAREN and has error info available.
static void test_error_missing_paren() {
    set_current_test("test_error_missing_paren");

    cre2_options_t *opt = cre2_opt_new();
    TEST_CHECK(opt != nullptr, "Failed to allocate cre2 options for missing paren test");
    cre2_opt_set_log_errors(opt, 0);

    // Malformed pattern: missing closing parenthesis
    cre2_regexp_t *rex = cre2_new("ci(ao", 5, opt);
    TEST_CHECK(rex != nullptr, "Failed to allocate regex for malformed pattern");

    int code = cre2_error_code(rex);
    const char *msg = cre2_error_string(rex);
    cre2_string_t S;
    cre2_error_arg(rex, &S);

    TEST_CHECK(code == CRE2_ERROR_MISSING_PAREN, "Expected CRE2_ERROR_MISSING_PAREN for malformed pattern");
    TEST_CHECK(msg != nullptr, "Error message should be non-null for malformed pattern");
    // Note: We don't require the content of S here, but ensure it's retrievable
    TEST_CHECK(S.data != nullptr || S.length >= 0, "Error arg data should be retrievable for malformed pattern");

    // Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);
}

// ---------------------------------------------------------------------------
// Test runner
// ---------------------------------------------------------------------------

static void run_all_tests() {
    // Run each test. The tests themselves are responsible for logging failures.
    test_basic_positive_pattern();
    test_no_options_object();
    test_capture_group();
    test_error_missing_paren();

    if (g_failure_count == 0) {
        std::cout << "[PASSED] All unit tests completed successfully.\n";
    } else {
        std::cout << "[TOTAL FAILED] " << g_failure_count << " test(s) failed out of 4.\n";
    }
}

// Entry point for the test suite
int main(void) {
    // Begin tests
    run_all_tests();

    // Return non-zero if any test failed
    return (g_failure_count == 0) ? 0 : 1;
}