// Unit test suite for the focal method: db_define_rm (located in c-pp.c)
// This test harness uses a lightweight, non-GTest approach suitable for C++11.
// It relies on the real project environment exposing the following C functions:
//   - void db_define_rm(const char * zKey);
//   - void db_define_add(const char * zKey);
//   - int  db_define_has(const char * zName);
// The tests exercise the interaction with the dependency surface (db_define_add,
// db_define_has) to verify correct behavior of db_define_rm under various inputs.
// Explanatory comments are provided for each test case.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <cstring>
#include <iostream>


extern "C" {
    // Focal method under test
    void db_define_rm(const char * zKey);

    // Supporting dependencies (from the class dependency surface)
    void db_define_add(const char * zKey);
    int  db_define_has(const char * zName);
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_pass = 0;

#define EXPECT_TRUE(cond)                                                      \
    do {                                                                         \
        g_tests_run++;                                                           \
        if (cond) {                                                              \
            g_tests_pass++;                                                    \
        } else {                                                                 \
            std::cerr << "Test FAILED: " #cond " at " __FILE__ ":" << __LINE__ \
                      << "\n";                                                 \
        }                                                                        \
    } while (0)

#define EXPECT_EQ(a, b)                                                         \
    do {                                                                          \
        g_tests_run++;                                                            \
        if ((a) == (b)) {                                                        \
            g_tests_pass++;                                                     \
        } else {                                                                  \
            std::cerr << "Test FAILED: " #a " == " #b " (" << (a) << " != " << (b)  \
                      << ") at " __FILE__ ":" << __LINE__ << "\n";               \
        }                                                                         \
    } while (0)

static void report_summary() {
    std::cout << "\nTest Summary: " << g_tests_pass << " / " << g_tests_run
              << " tests passed.\n";
}

// Test 1: Remove an existing key that does not contain '='
//
// Rationale:
// - Ensures the standard deletion path works when zKey has no '='.
// - Validates that db_define_rm interacts with db_define_add/db_define_has as expected.
// - Confirms that db_define_has reports the key as absent after removal.
static void test_define_rm_removes_existing_key_no_equals() {
    // Setup: ensure the key exists
    db_define_add("existing_key_no_eq");

    // Act: remove the key
    db_define_rm("existing_key_no_eq");

    // Assert: key should no longer be present
    int exists = db_define_has("existing_key_no_eq");
    EXPECT_TRUE(exists == 0);

    // Cleanup: remove any residual entry if present (defensive)
    db_define_rm("existing_key_no_eq"); // idempotent cleanup
}

// Test 2: Remove a key that includes an '=' character in the key string
//
// Rationale:
// - Verifies that the logic counting n (characters before the first '=') does not
//   affect the deletion, and that the key itself is still deleted correctly.
// - Ensures compatibility with keys that may be composed or derived from other tooling.
static void test_define_rm_key_with_equals() {
    // Setup: create a key that contains '='
    db_define_add("split=key");

    // Act: remove the key that includes an '='
    db_define_rm("split=key");

    // Assert: key should no longer be present
    int exists = db_define_has("split=key");
    EXPECT_TRUE(exists == 0);

    // Cleanup
    db_define_rm("split=key");
}

// Test 3: Remove a non-existing key to ensure the function handles non-existent entries gracefully
//
// Rationale:
// - Covers the "false" path where no row matches the provided key.
// - Verifies that no crash occurs and that the function remains stable under no-op deletion.
static void test_define_rm_non_existing_key_noop() {
    // Ensure the key does not exist
    db_define_rm("not_a_real_key_abcdef");

    // Assert: still not present
    int exists = db_define_has("not_a_real_key_abcdef");
    EXPECT_TRUE(exists == 0);
}

// Test 4: Remove an empty key string
//
// Rationale:
// - Edge case to ensure the function handles empty input gracefully.
// - Validates that the function does not crash or misbehave when zKey is an empty string.
static void test_define_rm_empty_key() {
    // Act: attempt to remove with empty key
    db_define_rm("");

    // Assert: empty key should not be considered present
    int exists = db_define_has("");
    EXPECT_TRUE(exists == 0);
}

// Test 5: Confirm that multiple consecutive removals for the same key are safe
//
// Rationale:
// - Verifies that subsequent removals after the first do not cause side effects.
// - Ensures stability when the defDel statement may already be prepared on a second call.
static void test_define_rm_multiple_calls_same_key() {
    // Setup: ensure key exists initially
    db_define_add("multi_call_key");

    // First removal
    db_define_rm("multi_call_key");
    int after_first = db_define_has("multi_call_key");
    EXPECT_TRUE(after_first == 0);

    // Second removal (should be harmless)
    db_define_rm("multi_call_key");
    int after_second = db_define_has("multi_call_key");
    EXPECT_TRUE(after_second == 0);
}

// Main function: run all unit tests
int main(int argc, char const * argv[]) {
    // Run tests
    test_define_rm_removes_existing_key_no_equals();
    test_define_rm_key_with_equals();
    test_define_rm_non_existing_key_noop();
    test_define_rm_empty_key();
    test_define_rm_multiple_calls_same_key();

    // Report summary
    report_summary();

    // Return status: 0 if all tests pass, 1 otherwise
    return (g_tests_run == g_tests_pass) ? 0 : 1;
}