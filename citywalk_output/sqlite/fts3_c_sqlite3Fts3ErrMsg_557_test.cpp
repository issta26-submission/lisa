// test_fts3_errmsg.cpp
// Unit tests for sqlite3Fts3ErrMsg in fts3.c
// Notes:
// - This test provides lightweight, self-contained mocks for sqlite3_free and sqlite3_vmprintf
//   so that sqlite3Fts3ErrMsg can be exercised without requiring a full SQLite build.
// - The tests are written in plain C++11 (no GoogleTest/GTest) and use a small, non-terminating
//   assertion style to maximize coverage.
// - The test suite focuses on ensuring: proper free of previous error string, correct new formatting,
//   handling of NULL previous error pointer, and formatting with multiple arguments.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstdarg>
#include <stdarg.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Forward declaration of the focal function (C linkage to avoid name mangling)
extern "C" void sqlite3Fts3ErrMsg(char **pzErr, const char *zFormat, ...);

// Mocks for sqlite3_free and sqlite3_vmprintf to support sqlite3Fts3ErrMsg without SQLite.
// We provide simple, deterministic behavior suitable for unit tests.

extern "C" {

// Global to observe which pointer is freed by sqlite3_free
static void* g_last_freed_ptr = nullptr;

// sqlite3_free implementation: records the freed pointer (even if NULL) and frees memory if non-NULL
void sqlite3_free(void *p) {
    g_last_freed_ptr = p;
    if (p != nullptr) {
        free(p);
    }
}

// Helper to format a string using a va_list (subset of sqlite3_vmprintf behavior)
static char* vsmprintf(const char *zFormat, va_list ap) {
    va_list ap_copy;
    va_copy(ap_copy, ap);
    int n = vsnprintf(nullptr, 0, zFormat, ap_copy);
    va_end(ap_copy);
    if (n < 0) n = 0;
    char* zOut = (char*)malloc((size_t)n + 1);
    if (zOut) {
        va_list ap_copy2;
        va_copy(ap_copy2, ap);
        vsnprintf(zOut, (size_t)n + 1, zFormat, ap_copy2);
        va_end(ap_copy2);
    }
    return zOut;
}

// sqlite3_vmprintf implementation using the helper above
char* sqlite3_vmprintf(const char *zFormat, va_list ap) {
    return vsmprintf(zFormat, ap);
}
} // extern "C"

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::cerr << "TEST FAILURE: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
            ++g_failed_tests; \
        } \
    } while(0)

#define EXPECT_STREQ(a, b, msg) \
    do { \
        ++g_total_tests; \
        if (((a) == nullptr && (b) != nullptr) || ((a) != nullptr && (b) == nullptr) || \
            ((a) && (b) && strcmp((a), (b)) != 0)) { \
            std::cerr << "TEST FAILURE: " << (msg) \
                      << " | Expected: \"" << (b ? b : "NULL") \
                      << "\" Actual: \"" << (a ? a : "NULL") << "\"" \
                      << " (line " << __LINE__ << ")" << std::endl; \
            ++g_failed_tests; \
        } \
    } while(0)

static void test_basic_formatting_frees_old_and_sets_new_string() {
    // Arrange: old error message exists
    char* old = (char*)malloc(16);
    if (old) strcpy(old, "old");
    char* pzErr = old;

    g_last_freed_ptr = nullptr;

    // Act: call the function with formatting
    sqlite3Fts3ErrMsg(&pzErr, "Hello %s %d", "world", 42);

    // Assert: previous string freed
    EXPECT_TRUE(g_last_freed_ptr == old, "sqlite3_free should be called on the previous *pzErr");

    // Assert: new string correctly formed
    EXPECT_STREQ(pzErr, "Hello world 42", "Formatted string mismatch after sqlite3Fts3ErrMsg");

    // Cleanup: free newly allocated error string
    if (pzErr) free(pzErr);
}

static void test_null_previous_pointer_is_handled() {
    // Arrange: pzErr is NULL
    char* pzErr = nullptr;
    g_last_freed_ptr = nullptr;

    // Act: call with no prior allocated string
    sqlite3Fts3ErrMsg(&pzErr, "Static message");

    // Assert: sqlite3_free should have been invoked with NULL (recorded as NULL)
    EXPECT_TRUE(g_last_freed_ptr == nullptr, "sqlite3_free should be invoked with NULL when *pzErr is NULL");

    // Assert: new string set correctly
    EXPECT_STREQ(pzErr, "Static message", "Expected static message formatting when no prior error string");

    if (pzErr) free(pzErr);
}

static void test_no_argument_formatting() {
    // Arrange: previous string exists
    char* old = (char*)malloc(20);
    if (old) strcpy(old, "to_be_freed");
    char* pzErr = old;
    g_last_freed_ptr = nullptr;

    // Act: format with no additional arguments
    sqlite3Fts3ErrMsg(&pzErr, "Static message only");

    // Assert: previous string freed
    EXPECT_TRUE(g_last_freed_ptr == old, "sqlite3_free should free the previous error string when no args provided");

    // Assert: new string content
    EXPECT_STREQ(pzErr, "Static message only", "Expected exact formatting without args");

    if (pzErr) free(pzErr);
}

static void test_multiple_arguments_formatting() {
    // Arrange: old error message
    char* old = (char*)malloc(8);
    if (old) strcpy(old, "old!");
    char* pzErr = old;
    g_last_freed_ptr = nullptr;

    // Act: formatting with multiple arguments
    sqlite3Fts3ErrMsg(&pzErr, "Num=%d, Str=%s", 7, "ok");

    // Assert: freed old pointer
    EXPECT_TRUE(g_last_freed_ptr == old, "sqlite3_free should be invoked on the previous error string for multi-arg formatting");

    // Assert: new string content
    EXPECT_STREQ(pzErr, "Num=7, Str=ok", "Formatted string with multiple arguments");

    if (pzErr) free(pzErr);
}

static void run_all_tests() {
    test_basic_formatting_frees_old_and_sets_new_string();
    test_null_previous_pointer_is_handled();
    test_no_argument_formatting();
    test_multiple_arguments_formatting();

    std::cout << "Total tests run: " << g_total_tests << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << "TESTS FAILED: " << g_failed_tests << std::endl;
    }
}

// Entrypoint
int main() {
    run_all_tests();
    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}