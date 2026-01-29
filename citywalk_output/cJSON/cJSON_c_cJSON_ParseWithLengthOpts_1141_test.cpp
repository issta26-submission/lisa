// Unit test suite for cJSON_ParseWithLengthOpts (CJSON C API) using only C++11 standard library.
// No Google Test or third-party test frameworks are used. A lightweight, non-terminating
// assertion mechanism is implemented to maximize code coverage across tests.
//
// Notes:
// - This test suite focuses on the focal method cJSON_ParseWithLengthOpts, exercising
//   various success and failure branches, including NULL input, zero length, BOM handling,
//   null-terminated enforcement, and end-pointer reporting via return_parse_end.
// - Tests are written in C++11 but call into the C API provided by cJSON.c / cJSON.h.
// - Static helpers inside cJSON.c (like static parse helpers) are not touched directly; tests
//   exercise only the public API as requested.
// - Each test includes explanatory comments describing intent and expected behavior.

#include <locale.h>
#include <math.h>
#include <cstring>
#include <cJSON.h>
#include <stdio.h>
#include <iostream>
#include <ctype.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


// Import the C API of cJSON. Ensure C linkage when included from C++.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

// EXPECT macro: non-terminating assertion with diagnostic output
#define EXPECT(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[TEST FAILURE] " << msg << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
    } \
} while (0)

// Helper to print summary at the end
static void print_summary() {
    std::cout << "Test summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failures.\n";
}

// Test A: Parse a simple valid JSON object with require_null_terminated = true
// Expect a non-NULL item and the end pointer to point to the terminating null terminator.
static void test_ParseWithLengthOpts_SimpleValid_NoTrailing()
{
    // Arrange
    const char* value = "{\"a\":1}";
    size_t len = std::strlen(value);
    const char* end_ptr = nullptr;

    // Act
    cJSON *item = cJSON_ParseWithLengthOpts(value, len, &end_ptr, /*require_null_terminated=*/1);

    // Assert
    EXPECT(item != nullptr, "Test A: item should be non-NULL for valid JSON");
    if (item != nullptr)
    {
        const char* expected_end = value + len;
        EXPECT(end_ptr == expected_end,
               "Test A: end_ptr should point to end of input (value+len)");
        cJSON_Delete(item);
    }  
}

// Test B: Valid JSON with trailing garbage and require_null_terminated = true
// Expect a NULL return and global_error populated with the parse position.
static void test_ParseWithLengthOpts_TrailingGarbageRequiresTerminated()
{
    // Arrange: valid JSON followed by extra garbage data
    const char* value = "{\"a\":1} garbage";
    size_t len = std::strlen(value);
    const char* end_ptr = nullptr;

    // Act
    cJSON *item = cJSON_ParseWithLengthOpts(value, len, &end_ptr, /*require_null_terminated=*/1);

    // Assert
    EXPECT(item == nullptr, "Test B: item should be NULL due to trailing garbage when terminated");
    // If parsing failed, global_error should be set (static in the C file)
    // We can't reset it here, but we can at least verify that an error occurred via non-NULL global_error.json if set.
    // Accessing the structure directly via the library's symbol is acceptable here as a diagnostic.
    // Note: We only assert that an error position is reported if available.
    // The code path guarantees global_error to be set on fail.
    // We check that the error context (global_error.json) is non-NULL and refers to the input.
    // Cast to verify non-NULL without relying on internal types beyond the header.
    // We rely on the API to expose global_error as part of the implementation; if not linked, this check is benign.
    extern error global_error;
    // Note: The actual type name 'error' is used in cJSON.c; ensure linking matches. If not accessible, skip assertion.
    // To remain robust across builds, perform a best-effort check:
    if (global_error.json != NULL)
    {
        EXPECT(reinterpret_cast<const void*>(global_error.json) == reinterpret_cast<const void*>(value),
               "Test B: global_error.json should point to input value on failure");
    }
}

// Test C: BOM prefix handling. Ensure that a leading UTF-8 BOM is skipped gracefully.
static void test_ParseWithLengthOpts_BOMPrefix()
{
    // Arrange: BOM followed by a simple JSON object
    const char bom_json[] = "\xEF\xBB\xBF{\"a\":1}";
    const size_t len = std::strlen(bom_json);
    const char* end_ptr = nullptr;

    // Act
    cJSON *item = cJSON_ParseWithLengthOpts(bom_json, len, &end_ptr, /*require_null_terminated=*/1);

    // Assert
    EXPECT(item != nullptr, "Test C: item should be non-NULL when parsing BOM-prefixed JSON");
    if (item != nullptr)
    {
        const char* expected_end = bom_json + len;
        EXPECT(end_ptr == expected_end,
               "Test C: end_ptr should point to end of input for BOM-prefixed JSON");
        cJSON_Delete(item);
    }
}

// Test D: NULL value input should fail gracefully
static void test_ParseWithLengthOpts_NullValue()
{
    // Arrange
    const char* value = NULL;
    size_t len = 10;
    const char* end_ptr = nullptr;

    // Act
    cJSON *item = cJSON_ParseWithLengthOpts(value, len, &end_ptr, /*require_null_terminated=*/1);

    // Assert
    EXPECT(item == nullptr, "Test D: item should be NULL when value is NULL");
    // end_ptr should remain unchanged (since we pass NULL value, function should goto fail early)
}

// Test E: Zero length should fail gracefully
static void test_ParseWithLengthOpts_ZeroLength()
{
    // Arrange
    const char* value = "{\"a\":1}";
    size_t len = 0;
    const char* end_ptr = nullptr;

    // Act
    cJSON *item = cJSON_ParseWithLengthOpts(value, len, &end_ptr, /*require_null_terminated=*/1);

    // Assert
    EXPECT(item == nullptr, "Test E: item should be NULL when buffer_length is zero");
}

// Test F: Validate that returning end pointer is not required (return_parse_end = NULL) but parsing succeeds
static void test_ParseWithLengthOpts_EndPtrOptional()
{
    // Arrange
    const char* value = "{\"a\":1}";
    size_t len = std::strlen(value);

    // Act
    cJSON *item = cJSON_ParseWithLengthOpts(value, len, NULL, /*require_null_terminated=*/1);

    // Assert
    EXPECT(item != nullptr, "Test F: item should be non-NULL when return_parse_end is NULL");
    if (item != nullptr)
    {
        cJSON_Delete(item);
    }
}

// Test G: With return_parse_end and no strict null termination (require_null_terminated = false)
// Ensure that end pointer lies within the input after parsing.
static void test_ParseWithLengthOpts_EndPtrWhenNotTerminated()
{
    // Arrange: valid JSON with trailing data, but not requiring null-termination
    const char* value = "{\"a\":1}X";
    size_t len = std::strlen(value);
    const char* end_ptr = nullptr;

    // Act
    cJSON *item = cJSON_ParseWithLengthOpts(value, len, &end_ptr, /*require_null_terminated=*/0);

    // Assert
    EXPECT(item != nullptr, "Test G: item should be non-NULL when trailing data exists but termination not required");
    if (item != nullptr)
    {
        // The end_ptr should point somewhere within the input, not past it
        EXPECT(end_ptr != nullptr, "Test G: end_ptr should be non-NULL when parsing succeeds with trailing data");
        EXPECT(end_ptr >= value && end_ptr <= value + len,
               "Test G: end_ptr should point into the input range");
        cJSON_Delete(item);
    }
}

// Runner to execute all tests
static void run_all_tests()
{
    test_ParseWithLengthOpts_SimpleValid_NoTrailing();
    test_ParseWithLengthOpts_TrailingGarbageRequiresTerminated();
    test_ParseWithLengthOpts_BOMPrefix();
    test_ParseWithLengthOpts_NullValue();
    test_ParseWithLengthOpts_ZeroLength();
    test_ParseWithLengthOpts_EndPtrOptional();
    test_ParseWithLengthOpts_EndPtrWhenNotTerminated();
}

int main()
{
    // Run the suite
    run_all_tests();

    // Emit a final summary
    print_summary();

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}