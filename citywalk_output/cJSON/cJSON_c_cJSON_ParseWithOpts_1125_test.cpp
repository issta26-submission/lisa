// Unit test suite for cJSON_ParseWithOpts (CJSON project) using C++11
// This test harness avoids any external testing framework (per instructions) and uses
// lightweight assertions that do not abort on failure (non-terminating).
// The tests exercise different branches of cJSON_ParseWithOpts by supplying various inputs
// and verifying the resulting cJSON structures and the return_parse_end behavior.
// Explanatory comments are provided above each test function.

#include <locale.h>
#include <math.h>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <limits.h>


// Lightweight, non-terminating assertion helper
static void expect_true(bool condition, const std::string &message, int &passed, int &failed)
{
    if (condition)
    {
        ++passed;
    }
    else
    {
        ++passed;
        ++failed;
        std::cerr << "ASSERTION FAILED: " << message << std::endl;
    }
}

// Test 1: Null input should return NULL (hit the early-return branch)
bool test_ParseWithOpts_null_input(int &passed, int &failed)
{
    // Explain test purpose
    std::cout << "Test 1: CJSON_ParseWithOpts should return NULL when input value is NULL" << std::endl;

    const char *value = NULL;
    const char *end = NULL;
    // When value is NULL, the function should return NULL regardless of end parameter
    cJSON *item = cJSON_ParseWithOpts(value, &end, /*require_null_terminated*/ 1);

    bool ok = (item == NULL);
    if (item) cJSON_Delete(item); // Safe to call only if not NULL

    expect_true(ok, "Expected NULL return when value is NULL", passed, failed);
    return ok;
}

// Test 2: Parse a simple JSON array with require_null_terminated = true
// Verifies basic parsing path and end-to-end parsing for a well-formed input.
bool test_ParseWithOpts_array_true_terminator(int &passed, int &failed)
{
    // Explain test purpose
    std::cout << "Test 2: Parse a simple array with require_null_terminated = true" << std::endl;

    const char *value = "[1,2]";
    const char *end = NULL;

    cJSON *item = cJSON_ParseWithOpts(value, &end, /*require_null_terminated*/ 1);
    bool ok = true;
    if (!item) ok = false;
    if (item && !cJSON_IsArray(item)) ok = false;
    if (item && cJSON_IsArray(item))
    {
        int size = cJSON_GetArraySize(item);
        if (size != 2) ok = false;
    }

    // Clean up
    if (item) cJSON_Delete(item);

    expect_true(ok, "Expected array [1,2] parsed with size 2 and array type", passed, failed);
    // Also ensure end pointer is non-null (though exact position is implementation-defined)
    expect_true(end != NULL, "Expected end pointer to be set for non-NULL input (array, true terminator)", passed, failed);

    return ok;
}

// Test 3: Parse with require_null_terminated = false and trailing data
// Verifies that end points to the end of the parsed content when extra trailing data exists.
bool test_ParseWithOpts_trailing_data_non_terminator(int &passed, int &failed)
{
    // Explain test purpose
    std::cout << "Test 3: Parse with trailing data and require_null_terminated = false" << std::endl;

    const char *value = "[1,2]X"; // trailing character 'X' after valid JSON
    const char *end = NULL;

    cJSON *item = cJSON_ParseWithOpts(value, &end, /*require_null_terminated*/ 0);
    bool ok = true;
    if (!item) ok = false;
    if (item && !cJSON_IsArray(item)) ok = false;
    if (item && cJSON_IsArray(item))
    {
        int size = cJSON_GetArraySize(item);
        if (size != 2) ok = false;
    }

    // Validate end position: it should point to the first trailing character after the parsed content.
    // For "[1,2]X", the parsed content length is 5, so end should point to value + 5, i.e., at 'X'.
    if (end == NULL)
    {
        ok = false;
        expect_true(false, "end pointer should not be NULL when trailing data exists (non-terminating)", passed, failed);
    }
    else
    {
        size_t offset = static_cast<size_t>(end - value);
        if (offset != 5) // "[1,2]" is 5 chars long
        {
            ok = false;
            std::string msg = "Expected end offset 5 after parsing, got " + std::to_string(offset);
            expect_true(false, msg, passed, failed);
        }
        // Additional sanity: the next character should be 'X' in this test vector
        if (end && *end != 'X')
        {
            ok = false;
            expect_true(false, "Expected end to point at trailing 'X' after parsed content", passed, failed);
        }
    }

    if (item) cJSON_Delete(item);

    expect_true(ok, "Expected trailing data to be tolerated when require_null_terminated is false", passed, failed);
    return ok;
}

// Test 4: Parse a simple number
bool test_ParseWithOpts_number(int &passed, int &failed)
{
    // Explain test purpose
    std::cout << "Test 4: Parse a simple numeric JSON value" << std::endl;

    const char *value = "12345";
    const char *end = NULL;

    cJSON *item = cJSON_ParseWithOpts(value, &end, /*require_null_terminated*/ 1);
    bool ok = true;
    if (!item) ok = false;
    if (item && !cJSON_IsNumber(item)) ok = false;
    if (item && cJSON_IsNumber(item))
    {
        double num = cJSON_GetNumberValue(item);
        if (num != 12345) ok = false;
    }

    if (item) cJSON_Delete(item);
    expect_true(ok, "Expected numeric value 12345 to be parsed correctly", passed, failed);
    return ok;
}

// Test 5: Parse a string value
bool test_ParseWithOpts_string(int &passed, int &failed)
{
    // Explain test purpose
    std::cout << "Test 5: Parse a JSON string value" << std::endl;

    const char *value = "\"hello world\"";
    const char *end = NULL;

    cJSON *item = cJSON_ParseWithOpts(value, &end, /*require_null_terminated*/ 1);
    bool ok = true;
    if (!item) ok = false;
    if (item && !cJSON_IsString(item)) ok = false;
    if (item && cJSON_IsString(item))
    {
        const char *str = cJSON_GetStringValue(item);
        if (str == NULL || std::string(str) != "hello world") ok = false;
    }

    if (item) cJSON_Delete(item);
    expect_true(ok, "Expected string value \"hello world\" to be parsed correctly", passed, failed);
    return ok;
}

// Test 6: Ensure NULL-check behavior with require_null_terminated true by parsing a valid value
// This complements Test 1 by ensuring normal parsing path is still exercised when input is valid.
bool test_ParseWithOpts_valid_simple_string(int &passed, int &failed)
{
    // Explain test purpose
    std::cout << "Test 6: Valid parsing path for a simple string with require_null_terminated = true" << std::endl;

    const char *value = "\"abc\"";
    const char *end = NULL;

    cJSON *item = cJSON_ParseWithOpts(value, &end, /*require_null_terminated*/ 1);
    bool ok = true;
    if (!item) ok = false;
    if (item && !cJSON_IsString(item)) ok = false;
    if (item && cJSON_IsString(item))
    {
        const char *str = cJSON_GetStringValue(item);
        if (str == NULL || std::string(str) != "abc") ok = false;
    }

    if (item) cJSON_Delete(item);
    expect_true(ok, "Expected string value \"abc\" parsed with require_null_terminated = true", passed, failed);
    return ok;
}

// Main test harness
int main()
{
    int passed = 0;
    int failed = 0;

    // Run tests
    test_ParseWithOpts_null_input(passed, failed);
    test_ParseWithOpts_array_true_terminator(passed, failed);
    test_ParseWithOpts_trailing_data_non_terminator(passed, failed);
    test_ParseWithOpts_number(passed, failed);
    test_ParseWithOpts_string(passed, failed);
    test_ParseWithOpts_valid_simple_string(passed, failed);

    // Summary
    int total = passed; // all tests contribute to 'passed' in our scheme
    // Note: In this lightweight harness, 'passed' counts both successes and failures identically to total checks.
    // To provide a clearer summary, we recompute total and final result directly.
    // Re-run a compact summary:
    // We'll simply indicate total tests = number of test_* calls above
    const int TOTAL_TESTS = 6;
    std::cout << "\nTest Summary: " << TOTAL_TESTS << " tests executed, "
              << failed << " failures." << std::endl;

    // Return non-zero if any test failed
    return (failed > 0) ? 1 : 0;
}