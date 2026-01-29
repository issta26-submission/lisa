/*
  Unit test suite for the focal method:
  CJSON_PUBLIC(const char *) cJSON_GetErrorPtr(void)

  Notes aligned with the provided guidance:
  - The tests target how cJSON_GetErrorPtr behaves in error scenarios by leveraging
    cJSON_Parse to induce parse errors (which populate the internal global_error state).
  - We do not rely on private/static internals; instead, we exercise the public API.
  - Tests are written in plain C++11 (no Google Test), with a lightweight, self-contained test harness.
  - Each test includes explanatory comments describing intent and expectations.

  Candidate Keywords (from Step 1):
  - global_error, json, position (as parts of the error state used by cJSON_GetErrorPtr)
  - cJSON_Parse, cJSON_GetErrorPtr
  - error pointer semantics: pointer into input string at error location
  - input strings used to provoke errors (e.g., "}", "{\"a\":1,")
*/

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <cstdlib>
#include <limits.h>


// Include the cJSON public API.
// The header is expected to provide C linkage for C functions when included from C++.
extern "C" {
}

// Lightweight test harness
static int tests_run = 0;
static int tests_passed = 0;

// Helper: simple assertion with descriptive message
static void assert_true(bool condition, const char* message) {
    tests_run++;
    if (condition) {
        tests_passed++;
        // Optional: print per-test success message
        // printf("PASS: %s\n", message);
    } else {
        printf("FAIL: %s\n", message);
    }
}

/*
  Test 1: Trigger a parsing error at the very first character.
  Input: "}"
  Rationale: A single closing brace is invalid JSON; parsing should fail,
  and cJSON_GetErrorPtr() is expected to return a pointer to the location of the error
  within the input string. For this simple invalid input, we expect the error pointer
  to point at the start of the input.
*/
static int test_GetErrorPtr_error_at_start() {
    const char* input = "}";

    // Attempt to parse invalid JSON; should fail
    cJSON* root = cJSON_Parse(input);
    assert_true(root == NULL, "Parsing invalid single-character JSON should fail");

    // If parse failed, GetErrorPtr should return a non-null pointer into the original input.
    const char* errPtr = cJSON_GetErrorPtr();
    assert_true(errPtr != NULL, "GetErrorPtr should return a non-null pointer after a parse error");

    // Cleanup (cJSON_Parse may allocate on failure; if root is NULL, nothing to free)
    if (root) {
        cJSON_Delete(root);
    }

    // Expect the error pointer to point to the start of the input (the first character)
    // i.e., errPtr should equal input in this simple case.
    bool pointer_matches_input = (errPtr == input) || (std::strcmp(errPtr, input) == 0);
    assert_true(pointer_matches_input, "Error pointer should reference the input's start for a first-character error");

    return 0;
}

/*
  Test 2: Trigger a parse error due to a trailing comma in an object.
  Input: "{\"a\":1,"
  Rationale: Trailing comma is invalid in JSON objects; parsing should fail.
  GetErrorPtr should return a pointer that lies within the original input string
  (i.e., the location of the error). We verify that the pointer is within the input bounds.
*/
static int test_GetErrorPtr_error_with_trailing_comma() {
    const char* input = "{\"a\":1,";

    // Attempt to parse invalid JSON; should fail
    cJSON* root = cJSON_Parse(input);
    assert_true(root == NULL, "Parsing JSON with trailing comma should fail");

    // If parse failed, GetErrorPtr should return a pointer into the input string
    const char* errPtr = cJSON_GetErrorPtr();
    assert_true(errPtr != NULL, "GetErrorPtr should return a non-null pointer after a parse error");

    // Cleanup
    if (root) {
        cJSON_Delete(root);
    }

    // Verify errPtr lies within the bounds of the original input string
    size_t len = std::strlen(input);
    bool in_range = (errPtr >= input) && (errPtr < input + len);
    assert_true(in_range, "Error pointer should point inside the original input buffer for a trailing-comma error");

    return 0;
}

int main() {
    // Run tests
    int initial_passes = tests_passed;

    if (true) {
        test_GetErrorPtr_error_at_start();
    }
    if (true) {
        test_GetErrorPtr_error_with_trailing_comma();
    }

    // Summary
    printf("Test results: %d/%d tests passed\n", tests_passed, tests_run);
    // Return non-zero if any test failed
    return (tests_passed == tests_run) ? 0 : 1;
}