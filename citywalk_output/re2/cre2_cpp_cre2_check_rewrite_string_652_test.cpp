#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


/*
  Unit test 1: Valid rewrite string for a simple capturing group.
  - Regex: "(a)" which defines 1 capturing group.
  - Rewrite: "$1" should be valid.
  - Expected: function returns 1, errmsg is NULL/length 0.
*/
bool test_rewrite_string_valid() {
    // Compile a simple regex with one capturing group
    const char *pattern = "(a)";
    int pattern_len = 3; // length of "(a)"
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, NULL);
    if (!rex) {
        std::cerr << "test_rewrite_string_valid: Failed to compile regex." << std::endl;
        return false;
    }

    // Prepare a valid rewrite string using the capturing group
    cre2_string_t rewrite;
    rewrite.data = (char *)"$1";   // cast away const for test data field
    rewrite.length = 2;

    cre2_string_t errmsg;
    errmsg.data = NULL;
    errmsg.length = 0;

    int retval = cre2_check_rewrite_string(rex, &rewrite, &errmsg);

    // Cleanup resources
    cre2_delete(rex);

    bool ok = (retval == 1) && (errmsg.data == NULL) && (errmsg.length == 0);
    if (!ok) {
        std::cerr << "test_rewrite_string_valid: Expected success with empty errmsg." << std::endl;
    }

    // In case errmsg.data was allocated by a previous failed run (not in this path),
    // ensure no memory is leaked here.
    if (errmsg.data) {
        free(errmsg.data);
        errmsg.data = NULL;
        errmsg.length = 0;
    }

    return ok;
}

/*
  Unit test 2: Invalid rewrite string for the same regex.
  - Regex: "(a)" which defines 1 capturing group.
  - Rewrite: "$2" references a non-existent capturing group.
  - Expected: function returns 0, errmsg is allocated with non-zero length.
*/
bool test_rewrite_string_invalid() {
    // Compile a simple regex with one capturing group
    const char *pattern = "(a)";
    int pattern_len = 3;
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, NULL);
    if (!rex) {
        std::cerr << "test_rewrite_string_invalid: Failed to compile regex." << std::endl;
        return false;
    }

    // Prepare an invalid rewrite string referencing non-existent group $2
    cre2_string_t rewrite;
    rewrite.data = (char *)"$2";
    rewrite.length = 2;

    cre2_string_t errmsg;
    errmsg.data = NULL;
    errmsg.length = 0;

    int retval = cre2_check_rewrite_string(rex, &rewrite, &errmsg);

    // Cleanup resources
    cre2_delete(rex);

    bool ok = (retval == 0) && (errmsg.data != NULL) && (errmsg.length > 0);
    if (!ok) {
        std::cerr << "test_rewrite_string_invalid: Expected failure with non-empty errmsg." << std::endl;
        // If errmsg was allocated, print its length for debugging
        if (errmsg.data) {
            std::cerr << "test_rewrite_string_invalid: errmsg.length = " << errmsg.length
                      << std::endl;
        }
    }

    // Validate that errmsg.data contains a string of expected length
    if (errmsg.data) {
        // Ensure the length matches the actual C-string length
        size_t actual_len = std::strlen(errmsg.data);
        if ((size_t)errmsg.length != actual_len) {
            std::cerr << "test_rewrite_string_invalid: Warning - documented length ("
                      << errmsg.length << ") != actual strlen(errmsg.data) (" << actual_len << ")." << std::endl;
        }
        // Free allocated error message buffer
        free(errmsg.data);
        errmsg.data = NULL;
        errmsg.length = 0;
    }

    return ok;
}

int main() {
    bool v = test_rewrite_string_valid();
    std::cout << "Summary - test_rewrite_string_valid: " << (v ? "PASS" : "FAIL") << std::endl;

    bool iv = test_rewrite_string_invalid();
    std::cout << "Summary - test_rewrite_string_invalid: " << (iv ? "PASS" : "FAIL") << std::endl;

    bool all_passed = v && iv;
    if (all_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}