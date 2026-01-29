// Unit test suite for unzStringFileNameCompare (C function) using pure C++11 (no GTest).
// This test suite is designed to be compile-and-run in a C++11 project.
// It links against the C implementation in unzip.c (extern "C").
// The tests cover the three code paths inside unzStringFileNameCompare:
//  - iCaseSensitivity == 0: uses CASESENSITIVITYDEFAULTVALUE
//  - iCaseSensitivity == 1: uses strcmp (case-sensitive)
//  - else: uses STRCMPCASENOSENTIVEFUNCTION (case-insensitive)

#include <string.h>
#include <string>
#include <stdio.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <unzip.h>


// Declaration of the focal C function. We force C linkage to avoid name mangling.
extern "C" int unzStringFileNameCompare(const char* fileName1,
                                      const char* fileName2,
                                      int iCaseSensitivity);

// Test 1: Exercise the iCaseSensitivity == 0 branch by using identical strings.
// Rationale: When both strings are identical (including case), the comparison should be 0
// regardless of the default case-sensitivity value set internally.
bool test_case_zero_branch_identical_strings()
{
    int result = unzStringFileNameCompare("same_string", "same_string", 0);
    return (result == 0);
}

// Test 2: Exercise the iCaseSensitivity == 2 branch (case-insensitive path).
// Rationale: This path should perform a case-insensitive comparison.
// We expect "File.txt" vs "file.TXT" to be equal ignoring case, hence 0.
bool test_case_insensitive_path_equal_ignoring_case()
{
    int result = unzStringFileNameCompare("File.txt", "file.TXT", 2);
    return (result == 0);
}

// Test 3: Exercise the iCaseSensitivity == 1 branch (case-sensitive path using strcmp).
// Rationale: "abc" vs "Abc" differ in the first character; in ASCII, 'a' > 'A', so result > 0.
bool test_case_sensitive_path_strict_lexicographic()
{
    int result = unzStringFileNameCompare("abc", "Abc", 1);
    // Expect a positive value since 'a'(97) - 'A'(65) > 0
    return (result > 0);
}

// Test 4: Exercise the else branch (case-insensitive) with clearly different strings.
// Rationale: For case-insensitive comparison, "apple" should come before "banana" (ignoring case),
// so the result should be negative.
bool test_case_insensitive_path_negative_order()
{
    int result = unzStringFileNameCompare("apple", "banana", 2);
    return (result < 0);
}

// Simple non-terminating test harness: track and report results without aborting on first failure.
int main()
{
    int failures = 0;
    std::cout << "Running tests for unzStringFileNameCompare...\n";

    if (test_case_zero_branch_identical_strings()) {
        std::cout << "[PASSED] test_case_zero_branch_identical_strings\n";
    } else {
        std::cout << "[FAILED] test_case_zero_branch_identical_strings\n";
        ++failures;
    }

    if (test_case_insensitive_path_equal_ignoring_case()) {
        std::cout << "[PASSED] test_case_insensitive_path_equal_ignoring_case\n";
    } else {
        std::cout << "[FAILED] test_case_insensitive_path_equal_ignoring_case\n";
        ++failures;
    }

    if (test_case_sensitive_path_strict_lexicographic()) {
        std::cout << "[PASSED] test_case_sensitive_path_strict_lexicographic\n";
    } else {
        std::cout << "[FAILED] test_case_sensitive_path_strict_lexicographic\n";
        ++failures;
    }

    if (test_case_insensitive_path_negative_order()) {
        std::cout << "[PASSED] test_case_insensitive_path_negative_order\n";
    } else {
        std::cout << "[FAILED] test_case_insensitive_path_negative_order\n";
        ++failures;
    }

    std::cout << "Total failures: " << failures << "\n";
    // Return non-zero if any test failed to aid integration with CI scripts.
    return failures;
}

/*
Notes and rationale aligned with the provided domain knowledge:

- Candidate Keywords (Step 1) identified: unzStringFileNameCompare, CASESENSITIVITYDEFAULTVALUE,
  STRCMPCASENOSENTIVEFUNCTION, strcmp. These represent the core dependent components and
  branches within the focal method.

- Step 2: The test suite targets the function's critical decision points:
  1) iCaseSensitivity == 0 -> path taken (default sensitivity value used).
  2) iCaseSensitivity == 1 -> strcmp branch (case-sensitive).
  3) else -> STRCMPCASENOSENTIVEFUNCTION path (case-insensitive).

  Tests cover both true/false branches for these predicates by using representative input
  pairs for each scenario.

- Step 3: Test refinement uses domain knowledge:
  - All tests are pure C function calls from C++ without GTest.
  - Tests do not rely on private/internal state; they exercise public behavior of the function.
  - Assertions are non-terminating in the sense that the test harness continues after failures
    and reports a summary at the end (via the return code).
  - Only standard library usage; no mocking required, since only a C function is under test.

- Static and class-related notes:
  - The focal code uses a number of static helper functions and macros; tests here only rely on
    the externally visible function unzStringFileNameCompare. Static/internal details are not
    accessed, respecting the encapsulation described in the domain guidance.
*/