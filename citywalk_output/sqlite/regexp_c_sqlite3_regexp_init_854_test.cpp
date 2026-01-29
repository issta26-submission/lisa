/*
  Unit test suite for the focal method: sqlite3_regexp_init
  - Target: verify correct registration of regexp/regexpi functions in an in-memory SQLite DB.
  - Language: C++11 (no GTest). A lightweight test harness is implemented with non-terminating assertions.
  - Notes:
    * The tests rely on the presence of the extension code providing sqlite3_regexp_init and its dependencies.
    * We include explanatory comments for each test case.
    * Some tests are guarded with SQLITE_DEBUG to accommodate the conditional compilation path.
*/

#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <iostream>
#include <cstdlib>


extern "C" int sqlite3_regexp_init(sqlite3*, char**pzErrMsg, const sqlite3_api_routines *pApi);

// Simple non-terminating test assertion macros
static int g_failures = 0;
#define TEST_OK(cond, msg) do { \
    if (cond) { \
        std::cout << "PASS: " << msg << "\n"; \
    } else { \
        std::cerr << "FAIL: " << msg << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define TEST_SKIP(msg) do { \
    std::cout << "SKIP: " << msg << "\n"; \
} while(0)

#define TEST_ASSERT(cond, msg) TEST_OK((cond), (msg))

// Helper to execute a SQL statement and fetch an integer result
static bool execIntResult(sqlite3* db, const char* sql, int expected, bool* pOutValue) {
    sqlite3_stmt* stmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "prepare failed: " << sqlite3_errmsg(db) << " | SQL: " << sql << "\n";
        return false;
    }
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        std::cerr << "step did not yield a row. rc=" << rc << " | SQL: " << sql << "\n";
        sqlite3_finalize(stmt);
        return false;
    }
    int val = sqlite3_column_int(stmt, 0);
    if (pOutValue) *pOutValue = true;
    sqlite3_finalize(stmt);
    return val == expected;
}

// Test 1: sqlite3_regexp_init should initialize on a valid in-memory database
static void test_init_on_memory_db() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_OK(db != nullptr && rc == SQLITE_OK, "Open in-memory SQLite DB");

    char* errMsg = nullptr;
    rc = sqlite3_regexp_init(db, &errMsg, nullptr);
    if (errMsg) {
        // If the extension attempted to set an error message, print it (non-fatal)
        std::cerr << "Warning: sqlite3_regexp_init reported error: " << errMsg << "\n";
        sqlite3_free(errMsg);
    }
    TEST_OK(rc == SQLITE_OK, "sqlite3_regexp_init returns SQLITE_OK on in-memory DB");
    if (db) sqlite3_close(db);
}

// Test 2: Validate that the registered 'regexp' function behaves as expected for a true match
static void test_regexp_true_match() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_OK(db != nullptr && rc == SQLITE_OK, "Open in-memory SQLite DB for true-match test");

    char* errMsg = nullptr;
    rc = sqlite3_regexp_init(db, &errMsg, nullptr);
    TEST_OK(rc == SQLITE_OK, "sqlite3_regexp_init succeeds for true-match test");

    // Prepare and execute a query that uses the registered regexp function
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT regexp('^foo$', 'foo')", -1, &stmt, NULL);
    TEST_OK(rc == SQLITE_OK, "Prepare statement using regexp function (true match)");
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int res = sqlite3_column_int(stmt, 0);
        TEST_OK(res == 1, "regexp returns true (1) for a matching pattern");
    } else {
        TEST_OK(false, "Expected a row from regexp evaluation (true match)");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test 3: Validate that the registered 'regexp' function behaves as expected for a false match
static void test_regexp_false_match() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_OK(db != nullptr && rc == SQLITE_OK, "Open in-memory SQLite DB for false-match test");

    char* errMsg = nullptr;
    rc = sqlite3_regexp_init(db, &errMsg, nullptr);
    TEST_OK(rc == SQLITE_OK, "sqlite3_regexp_init succeeds for false-match test");

    // Prepare and execute a query that uses the registered regexp function (false match)
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT regexp('^bar$', 'foo')", -1, &stmt, NULL);
    TEST_OK(rc == SQLITE_OK, "Prepare statement using regexp function (false match)");
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int res = sqlite3_column_int(stmt, 0);
        TEST_OK(res == 0, "regexp returns false (0) for a non-matching pattern");
    } else {
        TEST_OK(false, "Expected a row from regexp evaluation (false match)");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test 4: Validate the case-insensitive variant 'regexpi' when compiled (requires SQLITE_DEBUG for path)
static void test_regexpi_true_match_case_insensitive() {
#ifdef SQLITE_DEBUG
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_OK(db != nullptr && rc == SQLITE_OK, "Open in-memory SQLite DB for regexpi test");

    char* errMsg = nullptr;
    rc = sqlite3_regexp_init(db, &errMsg, nullptr);
    TEST_OK(rc == SQLITE_OK, "sqlite3_regexp_init succeeds for regexpi true-match test");

    // Case-insensitive match: pattern '^foo$' should match 'FOO' or 'Foo'
    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT regexpi('(?i)^foo$', 'FOO')", -1, &stmt, NULL);
    // Note: Using (?i) inline flag for case-insensitive; if the actual implementation uses a different approach, adapt accordingly.
    if (rc != SQLITE_OK) {
        // If the inline flag isn't supported, try a straightforward pattern with lower-case input
        sqlite3_finalize(stmt);
        rc = sqlite3_prepare_v2(db, "SELECT regexpi('^foo$', 'Foo')", -1, &stmt, NULL);
    }
    TEST_OK(rc == SQLITE_OK, "Prepare statement using regexpi function (case-insensitive true-match)");
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int res = sqlite3_column_int(stmt, 0);
        TEST_OK(res == 1, "regexpi returns true (1) for a case-insensitive match");
    } else {
        TEST_OK(false, "Expected a row from regexpi evaluation (true match)");
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
#else
    TEST_SKIP("Test skipped because SQLITE_DEBUG is not defined (regexp_bytecode path not enabled).");
#endif
}

// Test 5: If compiled with SQLITE_DEBUG, ensure regexp_bytecode is registered and callable.
// We do a best-effort check by attempting to call the function via SQL; if not registered, the test is skipped.
static void test_regexp_bytecode_presence() {
#ifdef SQLITE_DEBUG
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_OK(db != nullptr && rc == SQLITE_OK, "Open in-memory SQLite DB for regexp_bytecode test");

    char* errMsg = nullptr;
    rc = sqlite3_regexp_init(db, &errMsg, nullptr);
    TEST_OK(rc == SQLITE_OK, "sqlite3_regexp_init succeeds for regexp_bytecode test (DEBUG)");

    sqlite3_stmt* stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT regexp_bytecode('^foo$')", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        // If the function returns a scalar, a row should be produced; we allow any type as a successful invocation.
        TEST_OK(rc == SQLITE_ROW, "regexp_bytecode invocation returned a row");
        // Clean up value
        sqlite3_finalize(stmt);
    } else {
        TEST_OK(false, "regexp_bytecode is not available or failed to prepare (DEBUG)");
    }
    sqlite3_close(db);
#else
    TEST_SKIP("regexp_bytecode test skipped (SQLITE_DEBUG not defined).");
#endif
}

// Entry point for the test suite
int main() {
    std::cout << "Starting sqlite3_regexp_init unit tests (C++11, no GTest)\n";

    test_init_on_memory_db();
    test_regexp_true_match();
    test_regexp_false_match();
    test_regexpi_true_match_case_insensitive();
    test_regexp_bytecode_presence();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED\n";
        return 1;
    }
}