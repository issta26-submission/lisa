/*
Step 1 - Program Understanding (Keywords)
Candidate Keywords: sqlite3_expanded_sql, sqlite3_sql, Vdbe, sqlite3_mutex_enter, sqlite3_mutex_leave, sqlite3VdbeExpandSql, sqlite3_stmt, sqlite3, sqlite3_open, sqlite3_prepare_v2, sqlite3_bind_int, sqlite3_bind_text, sqlite3_expanded_sql result handling, sqlite3_free, SQLITE_TRANSIENT, sqlite3_finalize, sqlite3_close

This test suite targets the focal method sqlite3_expanded_sql as implemented in vdbeapi.c and exercises its behavior using the public SQLite C API from C++. The tests aim to cover typical usage paths (with and without bound parameters) and a concurrent usage scenario to ensure thread-safety aspects do not regress.
*/

#include <vector>
#include <atomic>
#include <cstdio>
#include <opcodes.h>
#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <thread>


// Simple, non-terminating-like test harness (keeps executing to maximize coverage)
static int g_total_failures = 0;
#define TN_PRINT(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

// Utility: simple contains (case-sensitive)
static bool contains(const char* hay, const char* needle) {
    if (!hay || !needle) return false;
    return strstr(hay, needle) != nullptr;
}

// Test 1: Simple statement expansion without any bound parameters
static bool test_expanded_sql_simple() {
    bool ok = true;
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TN_PRINT("Test 1: Failed to open in-memory DB\n");
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "SELECT 1";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        TN_PRINT("Test 1: Failed to prepare statement\n");
        ok = false;
        sqlite3_close(db);
        return ok;
    }

    char *z = sqlite3_expanded_sql(stmt);
    if (!z) {
        TN_PRINT("Test 1: Expanded SQL returned NULL\n");
        ok = false;
    } else {
        // Expect the expanded SQL to contain the original query
        if (!contains(z, "SELECT 1")) {
            TN_PRINT("Test 1: Expanded SQL does not contain 'SELECT 1': %s\n", z);
            ok = false;
        }
        sqlite3_free(z);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Test 2: Expanded SQL with an integer bound parameter
static bool test_expanded_sql_bind_int() {
    bool ok = true;
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TN_PRINT("Test 2: Failed to open in-memory DB\n");
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "SELECT ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        TN_PRINT("Test 2: Failed to prepare statement\n");
        ok = false;
        sqlite3_close(db);
        return ok;
    }

    rc = sqlite3_bind_int(stmt, 1, 42);
    if (rc != SQLITE_OK) {
        TN_PRINT("Test 2: Failed to bind integer parameter\n");
        ok = false;
    }

    char *z = sqlite3_expanded_sql(stmt);
    if (!z) {
        TN_PRINT("Test 2: Expanded SQL returned NULL\n");
        ok = false;
    } else {
        // Expect the bound value to appear in the expanded SQL
        if (!contains(z, "SELECT 42")) {
            TN_PRINT("Test 2: Expanded SQL does not contain 'SELECT 42': %s\n", z);
            ok = false;
        }
        sqlite3_free(z);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Test 3: Expanded SQL with a text bound parameter
static bool test_expanded_sql_bind_text() {
    bool ok = true;
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TN_PRINT("Test 3: Failed to open in-memory DB\n");
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "SELECT ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        TN_PRINT("Test 3: Failed to prepare statement\n");
        ok = false;
        sqlite3_close(db);
        return ok;
    }

    rc = sqlite3_bind_text(stmt, 1, "world", -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        TN_PRINT("Test 3: Failed to bind text parameter\n");
        ok = false;
    }

    char *z = sqlite3_expanded_sql(stmt);
    if (!z) {
        TN_PRINT("Test 3: Expanded SQL returned NULL\n");
        ok = false;
    } else {
        // Expect the bound value to appear quoted in expanded SQL
        if (!contains(z, "'world'")) {
            // Some environments may escape differently, but "'world'" is the typical form
            TN_PRINT("Test 3: Expanded SQL does not contain \"'world'\": %s\n", z);
            ok = false;
        }
        sqlite3_free(z);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Test 4: Expanded SQL with multiple parameters (int and text)
static bool test_expanded_sql_two_params() {
    bool ok = true;
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TN_PRINT("Test 4: Failed to open in-memory DB\n");
        return false;
    }

    sqlite3_stmt *stmt = nullptr;
    const char *sql = "SELECT ?, ?";
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        TN_PRINT("Test 4: Failed to prepare statement\n");
        ok = false;
        sqlite3_close(db);
        return ok;
    }

    rc = sqlite3_bind_int(stmt, 1, 10);
    if (rc != SQLITE_OK) ok = false;
    rc = sqlite3_bind_text(stmt, 2, "abc", -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) ok = false;

    char *z = sqlite3_expanded_sql(stmt);
    if (!z) {
        TN_PRINT("Test 4: Expanded SQL returned NULL\n");
        ok = false;
    } else {
        // Expect bound values to appear in order
        if (!contains(z, "SELECT 10") || !contains(z, "'abc'")) {
            TN_PRINT("Test 4: Expanded SQL does not contain expected bound values: %s\n", z);
            ok = false;
        }
        sqlite3_free(z);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Test 5: Concurrent expansion to exercise thread-safety and mutex behavior
static bool test_expanded_sql_concurrent() {
    const int THREADS = 4;
    std::atomic<int> failures(0);
    auto worker = [&](int idx) {
        sqlite3 *db = nullptr;
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            failures.fetch_add(1);
            return;
        }
        sqlite3_stmt *stmt = nullptr;
        const char *sql = "SELECT ?";
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (rc != SQLITE_OK || stmt == nullptr) {
            failures.fetch_add(1);
            sqlite3_close(db);
            return;
        }

        // Bind different values per thread
        int value = 7 + idx;
        rc = sqlite3_bind_int(stmt, 1, value);
        if (rc != SQLITE_OK) {
            failures.fetch_add(1);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }

        char *z = sqlite3_expanded_sql(stmt);
        if (!z) {
            failures.fetch_add(1);
        } else {
            // Verify that the bound value shows up
            // Expect "SELECT <value>"
            char expect[64];
            snprintf(expect, sizeof(expect), "SELECT %d", value);
            if (!contains(z, expect)) {
                // In some environments, ensure at least the value appears
                failures.fetch_add(1);
            }
            sqlite3_free(z);
        }

        sqlite3_finalize(stmt);
        sqlite3_close(db);
    };

    std::vector<std::thread> threads;
    for (int i = 0; i < THREADS; ++i) {
        threads.emplace_back(worker, i);
    }
    for (auto &t : threads) t.join();

    return failures.load() == 0;
}

int main() {
    TN_PRINT("Starting sqlite3_expanded_sql unit tests (C++11, non-GTest) ...\n");

    int total = 0;
    int passed = 0;

    struct TestCase { const char* name; bool (*fn)(); };
    TestCase tests[] = {
        {"test_expanded_sql_simple", test_expanded_sql_simple},
        {"test_expanded_sql_bind_int", test_expanded_sql_bind_int},
        {"test_expanded_sql_bind_text", test_expanded_sql_bind_text},
        {"test_expanded_sql_two_params", test_expanded_sql_two_params},
        {"test_expanded_sql_concurrent", test_expanded_sql_concurrent},
    };

    const int N = sizeof(tests)/sizeof(tests[0]);
    for (int i = 0; i < N; ++i) {
        bool result = tests[i].fn();
        total++;
        if (result) {
            passed++;
            TN_PRINT("PASS: %s\n", tests[i].name);
        } else {
            g_total_failures++;
            TN_PRINT("FAIL: %s\n", tests[i].name);
        }
    }

    TN_PRINT("Summary: %d/%d tests passed, %d failures total\n", passed, total, g_total_failures);
    return (g_total_failures == 0) ? 0 : 1;
}