/*
   Unit test suite for cre2_set_add (C++11, no GoogleTest)
   - Uses a lightweight in-file test harness with non-terminating EXPECT macros.
   - Exercises true/false branches of cre2_set_add:
     * Success path with error == NULL and error_len == 0
     * Failure path with error buffer provided (error != NULL, error_len > 0)
     * Edge case: error_len == 1 yields empty error string
     * Edge case: empty pattern
     * Optional: verify error buffer length boundary handling (last char reset to '\\0')
   - Relies on cre2.h wrappers provided by the project (cre2_set_new, cre2_set_add, cre2_set_delete).
   - Assumes RE2 library linkage is available during build/link stage.
*/

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the project's CRE2 wrapper header.
// This provides cre2_set_new, cre2_set_add, cre2_set_delete, and related types.

// Simple non-terminating test assertions
static int g_failures = 0;

#define LOG_FAIL(msg) \
    do { std::cerr << "[FAIL] " << (msg) \
                   << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; } while (0)

#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { LOG_FAIL(msg); ++g_failures; } } while (0)

#define EXPECT_EQ_INT(expected, actual, msg) \
    do { if ((expected) != (actual)) { \
            std::cerr << "[FAIL] " << (msg) \
                      << " | Expected: " << (expected) \
                      << "  Actual: " << (actual) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_STR_NOT_EMPTY(str, max_len, msg) \
    do { if ((str) == NULL) { LOG_FAIL(msg); ++g_failures; } \
         else { if (std::strlen(str) == 0) { LOG_FAIL(msg); ++g_failures; } } } while (0)


// Test 1: Success path when error is NULL and error_len is 0
// - Expect a non-negative index returned by cre2_set_add for a valid pattern.
static bool test_set_add_success_no_error() {
    cre2_set *set = cre2_set_new(NULL, 0);
    if (!set) {
        LOG_FAIL("Failed to create CRE2 set in test_set_add_success_no_error");
        ++g_failures;
        return false;
    }

    int idx = cre2_set_add(set, "abc", 3, NULL, 0);
    EXPECT_TRUE(idx >= 0, "cre2_set_add should succeed for valid pattern when error buffer is unused");

    cre2_set_delete(set);
    return (g_failures == 0);
}

// Test 2: Failure path with error buffer provided (error != NULL, error_len > 0)
// - Use an invalid regex to trigger an error, ensure a non-empty error message is written.
static bool test_set_add_invalid_with_error_buffer() {
    cre2_set *set = cre2_set_new(NULL, 0);
    if (!set) {
        LOG_FAIL("Failed to create CRE2 set in test_set_add_invalid_with_error_buffer");
        ++g_failures;
        return false;
    }

    char errorBuf[128];
    int idx = cre2_set_add(set, "(", 1, errorBuf, sizeof(errorBuf));
    EXPECT_TRUE(idx < 0, "Invalid regex should return negative index when error buffer is provided");
    EXPECT_TRUE(errorBuf[0] != '\0', "Error buffer should be populated for invalid regex");

    cre2_set_delete(set);
    return (g_failures == 0);
}

// Test 3: Edge case where error_len == 1
// - Expect an empty error string (only the terminating null character is present).
static bool test_set_add_invalid_error_len_one() {
    cre2_set *set = cre2_set_new(NULL, 0);
    if (!set) {
        LOG_FAIL("Failed to create CRE2 set in test_set_add_invalid_error_len_one");
        ++g_failures;
        return false;
    }

    char errorBuf[1]; // error_len == 1
    int idx = cre2_set_add(set, "(", 1, errorBuf, sizeof(errorBuf));
    EXPECT_TRUE(idx < 0, "Invalid regex should return negative index even with minimal error buffer");
    EXPECT_TRUE(errorBuf[0] == '\0', "Error buffer should be empty when error_len == 1");

    cre2_set_delete(set);
    return (g_failures == 0);
}

// Test 4: Empty pattern should be accepted (pattern_len == 0)
static bool test_set_add_empty_pattern() {
    cre2_set *set = cre2_set_new(NULL, 0);
    if (!set) {
        LOG_FAIL("Failed to create CRE2 set in test_set_add_empty_pattern");
        ++g_failures;
        return false;
    }

    int idx = cre2_set_add(set, "", 0, NULL, 0);
    EXPECT_TRUE(idx >= 0, "Empty pattern should be accepted by set Add");

    cre2_set_delete(set);
    return (g_failures == 0);
}

// Test 5: Error buffer boundary behavior
// - Ensure the last byte in the provided buffer is '\0' when error_len > 0.
static bool test_set_add_error_buffer_boundary() {
    cre2_set *set = cre2_set_new(NULL, 0);
    if (!set) {
        LOG_FAIL("Failed to create CRE2 set in test_set_add_error_buffer_boundary");
        ++g_failures;
        return false;
    }

    char errorBuf[5]; // error_len = 5, last index 4 should be set to '\0'
    int idx = cre2_set_add(set, "(", 1, errorBuf, sizeof(errorBuf));
    EXPECT_TRUE(idx < 0, "Invalid regex should return negative index for boundary test");
    // The function enforces error[error_len-1] = '\0'
    EXPECT_STR_NOT_EMPTY(errorBuf, sizeof(errorBuf), "Error buffer should be null-terminated within bounds");
    // Specifically verify boundary behavior
    EXPECT_EQ_INT(0, errorBuf[4] == '\0' ? 1 : 0, "Last buffer byte should be '\\0' per boundary rule");

    cre2_set_delete(set);
    return (g_failures == 0);
}


// Helper to run all tests
static void RunAllTests() {
    test_set_add_success_no_error();
    test_set_add_invalid_with_error_buffer();
    test_set_add_invalid_error_len_one();
    test_set_add_empty_pattern();
    test_set_add_error_buffer_boundary();
}

int main() {
    std::cout << "Running cre2_set_add unit tests (no GTest)..." << std::endl;
    RunAllTests();

    if (g_failures == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED." << std::endl;
        return 1;
    }
}