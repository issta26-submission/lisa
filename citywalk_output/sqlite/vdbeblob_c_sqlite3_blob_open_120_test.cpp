// C++11 test suite for sqlite3_blob_open (vdbeblob.c) using public SQLite API.
// This test avoids GTest and relies on the standard library only.
// It creates temporary in-memory databases, exercises positive and negative paths
// to cover multiple branches of sqlite3_blob_open, and includes explanatory
// comments for each test case.

#include <vector>
#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Simple test harness: non-terminating checks that accumulate failures.
static int gFailures = 0;
#define TEST_EXPECT(cond, msg) \
    if(!(cond)) { std::cerr << "Test failure: " << msg << std::endl; ++gFailures; }

// Helper to print a short summary at the end
static void printSummary() {
    if(gFailures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << gFailures << " test(s) failed." << std::endl;
    }
}

// Test 1: Basic blob_open in read-only mode for a simple table with a blob column.
// Steps:
// - Create in-memory DB with a table (id PRIMARY KEY, data BLOB)
// - Insert a small blob
// - Open the blob with wrFlag = 0 (read-only)
// - Read back the blob and verify content and size
// - Close and cleanup
static void test_blob_open_basic_readonly() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_EXPECT(rc == SQLITE_OK, "sqlite3_open in-memory");

    if(rc != SQLITE_OK || db == nullptr) {
        if(db) sqlite3_close(db);
        return;
    }

    const char *createSQL = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, createSQL, 0, 0, 0);
    TEST_EXPECT(rc == SQLITE_OK, "CREATE TABLE t (id, data BLOB)");

    // Prepare and bind blob data
    const unsigned char blobData[] = { 'H','e','l','l','o' }; // "Hello"
    int blobSize = (int)sizeof(blobData);

    sqlite3_stmt *stmt = 0;
    rc = sqlite3_prepare_v2(db, "INSERT INTO t(data) VALUES(?)", -1, &stmt, 0);
    TEST_EXPECT(rc == SQLITE_OK && stmt != 0, "prepare INSERT");
    if(rc != SQLITE_OK || stmt == 0) {
        sqlite3_close(db);
        return;
    }
    sqlite3_bind_blob(stmt, 1, blobData, blobSize, SQLITE_STATIC);
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    TEST_EXPECT(rc == SQLITE_DONE, "insert blob row");
    if(rc != SQLITE_DONE) {
        sqlite3_close(db);
        return;
    }

    // Open blob
    sqlite3_blob *pBlob = 0;
    rc = sqlite3_blob_open(db, "main", "t", "data", 1, 0, &pBlob);
    TEST_EXPECT(rc == SQLITE_OK && pBlob != 0, "sqlite3_blob_open read-only success");
    if(rc != SQLITE_OK || pBlob == 0) {
        sqlite3_close(db);
        return;
    }

    // Verify size and content
    int openedSize = sqlite3_blob_bytes(pBlob);
    TEST_EXPECT(openedSize == blobSize, "blob size matches inserted size");

    std::vector<char> readBuf(blobSize, 0);
    rc = sqlite3_blob_read(pBlob, readBuf.data(), blobSize, 0);
    TEST_EXPECT(rc == SQLITE_OK, "blob_read success");
    if(rc == SQLITE_OK) {
        TEST_EXPECT(std::memcmp(readBuf.data(), blobData, blobSize) == 0, "blob data matches expected");
    }

    // Cleanup
    sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 2: Opening a blob from a non-existent table should return SQLITE_ERROR.
static void test_blob_open_missing_table() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_EXPECT(rc == SQLITE_OK, "sqlite3_open in-memory (missing-table test)");
    if(rc != SQLITE_OK || db == nullptr) {
        if(db) sqlite3_close(db);
        return;
    }

    sqlite3_blob *pBlob = 0;
    rc = sqlite3_blob_open(db, "main", "no_table", "data", 1, 0, &pBlob);
    TEST_EXPECT(rc == SQLITE_ERROR && pBlob == 0, "blob_open on missing table yields SQLITE_ERROR");

    if(pBlob) sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 3: Opening a blob with a non-existent column should return SQLITE_ERROR.
static void test_blob_open_missing_column() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_EXPECT(rc == SQLITE_OK, "sqlite3_open in-memory (missing-column test)");
    if(rc != SQLITE_OK || db == nullptr) {
        if(db) sqlite3_close(db);
        return;
    }

    // Create table with a blob column
    rc = sqlite3_exec(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);", 0, 0, 0);
    TEST_EXPECT(rc == SQLITE_OK, "CREATE TABLE t (id, data BLOB) for missing-column test");

    // Insert a blob
    sqlite3_stmt *stmt = 0;
    const unsigned char blobData[] = { 0x01, 0x02, 0x03 };
    rc = sqlite3_prepare_v2(db, "INSERT INTO t(data) VALUES(?)", -1, &stmt, 0);
    if(rc == SQLITE_OK && stmt) {
        sqlite3_bind_blob(stmt, 1, blobData, (int)sizeof(blobData), SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    TEST_EXPECT(rc == SQLITE_DONE, "insert blob for missing-column test");

    sqlite3_blob *pBlob = 0;
    rc = sqlite3_blob_open(db, "main", "t", "not_exist_column", 1, 0, &pBlob);
    TEST_EXPECT(rc == SQLITE_ERROR && pBlob == 0, "blob_open with missing column yields SQLITE_ERROR");

    if(pBlob) sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 4: Opening a blob on a WITHOUT ROWID table should return SQLITE_ERROR (no rowid).
static void test_blob_open_without_rowid() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_EXPECT(rc == SQLITE_OK, "sqlite3_open in-memory (without-rowid test)");
    if(rc != SQLITE_OK || db == nullptr) {
        if(db) sqlite3_close(db);
        return;
    }

    // Create WITHOUT ROWID table. Having a blob column here will ensure HasRowid() is false.
    rc = sqlite3_exec(db, "CREATE TABLE t_noid(id INTEGER PRIMARY KEY, data BLOB) WITHOUT ROWID;", 0, 0, 0);
    TEST_EXPECT(rc == SQLITE_OK, "CREATE WITHOUT ROWID table");

    // Insert a blob (not strictly necessary as the test should fail before seeking the row)
    sqlite3_stmt *stmt = 0;
    rc = sqlite3_prepare_v2(db, "INSERT INTO t_noid(data) VALUES(?)", -1, &stmt, 0);
    if(rc == SQLITE_OK && stmt) {
        const unsigned char blobData[] = { 0x10, 0x20, 0x30 };
        sqlite3_bind_blob(stmt, 1, blobData, (int)sizeof(blobData), SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    // Try to open; should fail due to no rowid
    sqlite3_blob *pBlob = 0;
    rc = sqlite3_blob_open(db, "main", "t_noid", "data", 1, 0, &pBlob);
    TEST_EXPECT(rc == SQLITE_ERROR && pBlob == 0, "blob_open on WITHOUT ROWID table yields SQLITE_ERROR");

    if(pBlob) sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 5: Writing into an indexed blob column should be rejected (cannot open for writing).
static void test_blob_open_write_blocked_by_index() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    TEST_EXPECT(rc == SQLITE_OK, "sqlite3_open in-memory (write-blocked-index test)");
    if(rc != SQLITE_OK || db == nullptr) {
        if(db) sqlite3_close(db);
        return;
    }

    // Create table and insert a blob
    rc = sqlite3_exec(db, "CREATE TABLE t_idx(id INTEGER PRIMARY KEY, data BLOB);", 0, 0, 0);
    TEST_EXPECT(rc == SQLITE_OK, "CREATE TABLE t_idx (id, data BLOB)");
    if(rc != SQLITE_OK) { sqlite3_close(db); return; }

    sqlite3_stmt *stmt = 0;
    const unsigned char blobData[] = { 'A','B','C' };
    rc = sqlite3_prepare_v2(db, "INSERT INTO t_idx(data) VALUES(?)", -1, &stmt, 0);
    if(rc == SQLITE_OK && stmt) {
        sqlite3_bind_blob(stmt, 1, blobData, (int)sizeof(blobData), SQLITE_STATIC);
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    TEST_EXPECT(rc == SQLITE_DONE, "insert blob for indexed-write test");

    // Create an index on the blob column
    rc = sqlite3_exec(db, "CREATE INDEX idx_blob ON t_idx(data);", 0, 0, 0);
    TEST_EXPECT(rc == SQLITE_OK, "CREATE INDEX on blob column");

    // Attempt to open with write access; should fail due to being indexed
    sqlite3_blob *pBlob = 0;
    rc = sqlite3_blob_open(db, "main", "t_idx", "data", 1, 1, &pBlob);
    TEST_EXPECT(rc == SQLITE_ERROR && pBlob == 0, "blob_open with indexed column cannot open for writing");

    if(pBlob) sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

int main() {
    // Run all tests
    test_blob_open_basic_readonly();
    test_blob_open_missing_table();
    test_blob_open_missing_column();
    test_blob_open_without_rowid();
    test_blob_open_write_blocked_by_index();

    // Print summary and return non-zero if any failures occurred
    printSummary();
    return gFailures;
}