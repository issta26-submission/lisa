/*
Unit Test Suite for the focal method:
  int sqlite3_column_bytes(sqlite3_stmt *pStmt, int i)

Context and Plan:
- The focal method delegates to sqlite3_value_bytes(columnMem(pStmt,i)) and then calls columnMallocFailure(pStmt) before returning the value.
- We will test that sqlite3_column_bytes returns the expected byte length for various blob/non-blob/null cases by driving a small in-memory SQLite database.
- No GTest; a lightweight in-file test harness is provided with non-terminating assertions (they log failures but continue executing).
- Tests cover:
  1) A real non-empty blob (length should match)
  2) An empty blob (length should be 0)
  3) A non-blob value (integer) in the blob column (length should be 0 or not treated as blob; we expect 0)
  4) NULL value (length should be 0)

Candidate Keywords (Step 1):
- sqlite3_column_bytes
- sqlite3_stmt (pStmt)
- columnMem(pStmt,i)  [indirect Mem* access]
- sqlite3_value_bytes
- columnMallocFailure(pStmt)
- pStmt, i (statement and column index)
- BLOB, NULL, INTEGER data types
- In-memory database (":memory:")
- sqlite3_bind_blob, sqlite3_bind_null, sqlite3_bind_int
- sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize
- sqlite3_open, sqlite3_close
These form the core dependencies and behavior surface the tests exercise.

Note on static and dependencies:
- The tests rely on the public SQLite C API to exercise sqlite3_column_bytes. We avoid direct access to any private or static helper internals.
- The test suite uses only the C API and standard C++11 library features.

Test harness behavior:
- Non-terminating assertions log failures and continue, enabling broad coverage per domain guidelines.
- A small test registry is used to collect failures and print a summary at the end.
- Each test creates its own in-memory database instance to isolate tests and avoid cross-test side effects.

Build/Run:
- Compile with C++11 and link against sqlite3 (e.g., g++ -std=c++11 test_sqlite_column_bytes.cpp -lsqlite3 -o test_sqlite_column_bytes)
- Run: ./test_sqlite_column_bytes
- Output provides a summary and detailed failure messages if any.

Code (C++11, single file, with explanatory comments per unit test):

*/

#include <vector>
#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Candidate Keywords mapping (for reference in-code)
//
// - sqlite3_column_bytes: focal method under test
// - sqlite3_value_bytes: used inside the focal method to obtain blob length
// - columnMem: internal helper (static) to fetch Mem* for a given column
// - columnMallocFailure: internal memory failure handler invoked by the focal method
// - sqlite3_stmt: statement handle passed to the focal method
// - blob handling: tests cover non-empty blob, empty blob, non-blob (e.g., integer), and NULL
//
// The tests operate via the public C API to avoid direct access to static/file-scope internals.

static std::vector<std::string> gFailures;

// Log a non-terminating failure message
static void logFailure(const std::string& msg) {
    gFailures.push_back(msg);
}

// Convenience macro for integer equality with location info
#define EXPECT_INT_EQ(actual, expected, message) do { \
    int a = (actual); \
    int e = (expected); \
    if (a != e) { \
        logFailure("EXPECT_INT_EQ FAILED at " __FILE__ ":" + std::to_string(__LINE__) + " | " + (message) + \
                   " | actual=" + std::to_string(a) + " expected=" + std::to_string(e)); \
    } \
} while(0)

// Convenience macro for size_t equality with location info
#define EXPECT_SIZE_T_EQ(actual, expected, message) do { \
    size_t a = (actual); \
    size_t e = (expected); \
    if (a != e) { \
        logFailure("EXPECT_SIZE_T_EQ FAILED at " __FILE__ ":" + std::to_string(__LINE__) + " | " + (message) + \
                   " | actual=" + std::to_string(a) + " expected=" + std::to_string(e)); \
    } \
} while(0)

// Test 1: Blob length should be 6 for a 6-byte blob
static void test_sqlite_column_bytes_blob_len() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *pStmt = nullptr;
    const unsigned char blob[] = { 'A','B','C','D','E','F' }; // length 6
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        logFailure("test_blob_len: cannot open in-memory DB");
        return;
    }

    rc = sqlite3_exec(db, "CREATE TABLE t(a BLOB);", 0, 0, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_blob_len: CREATE TABLE failed");
        sqlite3_close(db);
        return;
    }

    // Insert a real blob of length 6
    rc = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES (?)", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_blob_len: prepare INSERT failed");
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_blob(pStmt, 1, blob, sizeof(blob), SQLITE_TRANSIENT);
    rc = sqlite3_step(pStmt);
    sqlite3_finalize(pStmt);
    if (rc != SQLITE_DONE) {
        logFailure("test_blob_len: INSERT failed to finalize");
        sqlite3_close(db);
        return;
    }

    // Query back the blob and verify sqlite3_column_bytes reports 6
    rc = sqlite3_prepare_v2(db, "SELECT a FROM t", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_blob_len: prepare SELECT failed");
        sqlite3_close(db);
        return;
    }
    rc = sqlite3_step(pStmt);
    if (rc == SQLITE_ROW) {
        int bytes = sqlite3_column_bytes(pStmt, 0);
        EXPECT_INT_EQ(bytes, 6, "Blob length should equal 6");
    } else {
        logFailure("test_blob_len: expected a row, got rc=" + std::to_string(rc));
    }
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

// Test 2: Empty blob should yield length 0
static void test_sqlite_column_bytes_empty_blob() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *pStmt = nullptr;
    const unsigned char emptyBlob[] = {}; // length 0
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        logFailure("test_empty_blob: cannot open in-memory DB");
        return;
    }

    rc = sqlite3_exec(db, "CREATE TABLE t(a BLOB);", 0, 0, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_empty_blob: CREATE TABLE failed");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES (?)", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_empty_blob: prepare INSERT failed");
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_blob(pStmt, 1, emptyBlob, 0, SQLITE_TRANSIENT);
    rc = sqlite3_step(pStmt);
    sqlite3_finalize(pStmt);
    if (rc != SQLITE_DONE) {
        logFailure("test_empty_blob: INSERT failed to finalize");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_prepare_v2(db, "SELECT a FROM t", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_empty_blob: prepare SELECT failed");
        sqlite3_close(db);
        return;
    }
    rc = sqlite3_step(pStmt);
    if (rc == SQLITE_ROW) {
        int bytes = sqlite3_column_bytes(pStmt, 0);
        EXPECT_INT_EQ(bytes, 0, "Blob length should be 0 for empty blob");
    } else {
        logFailure("test_empty_blob: expected a row, got rc=" + std::to_string(rc));
    }
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

// Test 3: Non-blob value (INTEGER) in the blob column should yield length 0
static void test_sqlite_column_bytes_non_blob() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *pStmt = nullptr;
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        logFailure("test_non_blob: cannot open in-memory DB");
        return;
    }

    rc = sqlite3_exec(db, "CREATE TABLE t(a BLOB);", 0, 0, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_non_blob: CREATE TABLE failed");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES (?)", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_non_blob: prepare INSERT failed");
        sqlite3_close(db);
        return;
    }

    // Bind an INTEGER value to a BLOB column
    sqlite3_bind_int(pStmt, 1, 12345);
    rc = sqlite3_step(pStmt);
    sqlite3_finalize(pStmt);
    if (rc != SQLITE_DONE) {
        logFailure("test_non_blob: INSERT failed to finalize");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_prepare_v2(db, "SELECT a FROM t", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_non_blob: prepare SELECT failed");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(pStmt);
    if (rc == SQLITE_ROW) {
        int bytes = sqlite3_column_bytes(pStmt, 0);
        // If the value is not a blob, we expect 0 bytes for sqlite3_column_bytes
        EXPECT_INT_EQ(bytes, 0, "Blob bytes for non-blob INTEGER should be 0");
    } else {
        logFailure("test_non_blob: expected a row, got rc=" + std::to_string(rc));
    }
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

// Test 4: NULL value should yield length 0
static void test_sqlite_column_bytes_null() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *pStmt = nullptr;
    int rc;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        logFailure("test_null: cannot open in-memory DB");
        return;
    }

    rc = sqlite3_exec(db, "CREATE TABLE t(a BLOB);", 0, 0, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_null: CREATE TABLE failed");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES (?)", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_null: prepare INSERT failed");
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_null(pStmt, 1);
    rc = sqlite3_step(pStmt);
    sqlite3_finalize(pStmt);
    if (rc != SQLITE_DONE) {
        logFailure("test_null: INSERT failed to finalize");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_prepare_v2(db, "SELECT a FROM t", -1, &pStmt, 0);
    if (rc != SQLITE_OK) {
        logFailure("test_null: prepare SELECT failed");
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(pStmt);
    if (rc == SQLITE_ROW) {
        int bytes = sqlite3_column_bytes(pStmt, 0);
        EXPECT_INT_EQ(bytes, 0, "Blob bytes should be 0 for NULL value");
    } else {
        logFailure("test_null: expected a row, got rc=" + std::to_string(rc));
    }
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
}

// Run all tests (Step 3: test case refinement and execution)
static void runAllTests() {
    test_sqlite_column_bytes_blob_len();
    test_sqlite_column_bytes_empty_blob();
    test_sqlite_column_bytes_non_blob();
    test_sqlite_column_bytes_null();
}

// Entry point
int main() {
    runAllTests();

    if (gFailures.empty()) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << gFailures.size() << " TEST(S) FAILED:\n";
        for (size_t i = 0; i < gFailures.size(); ++i) {
            std::cerr << i + 1 << ") " << gFailures[i] << "\n";
        }
        return 1;
    }
}