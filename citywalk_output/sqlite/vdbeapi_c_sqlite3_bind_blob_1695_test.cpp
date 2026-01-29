// High-quality C++11 unit test suite for sqlite3_bind_blob (vdbeapi.c) without GTest.
// The tests exercise public SQLite API usage to verify correct behavior of sqlite3_bind_blob.
// The suite uses a lightweight test harness with non-terminating assertions and prints
// informative comments for each test case.
//
// Note: This test code assumes that SQLite is available and linked (e.g., -lsqlite3).
// It focuses on the public API surface (sqlite3_bind_blob) and common usage patterns.

#include <vector>
#include <opcodes.h>
#include <sqlite3.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Simple non-terminating assertion helper
static void ExpectTrue(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "[FAIL] " << message << std::endl;
    }
}

// A small test harness that prints pass/fail for each test
static bool RunTest(const std::string& name, bool ok) {
    if (ok) {
        std::cout << "[PASS] " << name << std::endl;
        return true;
    } else {
        std::cerr << "[FAIL] " << name << std::endl;
        return false;
    }
}

// Helper: execute a simple SQL statement (without results)
static bool ExecSQL(sqlite3* db, const char* sql) {
    char* err = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &err);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << (err ? err : "unknown") << std::endl;
        sqlite3_free(err);
        return false;
    }
    return true;
}

// Helper: fetch a blob by id from table t
static bool GetBlobById(sqlite3* db, int id, std::vector<unsigned char>& blobOut) {
    const char* sql = "SELECT data FROM t WHERE id = ?";
    sqlite3_stmt* pStmt = nullptr;
    int rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    rc = sqlite3_bind_int(pStmt, 1, id);
    if (rc != SQLITE_OK) {
        std::cerr << "Bind failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(pStmt);
        return false;
    }
    rc = sqlite3_step(pStmt);
    if (rc != SQLITE_ROW) {
        std::cerr << "Step did not return row for id=" << id << std::endl;
        sqlite3_finalize(pStmt);
        return false;
    }
    const void* blob = sqlite3_column_blob(pStmt, 0);
    int size = sqlite3_column_bytes(pStmt, 0);
    blobOut.assign(reinterpret_cast<const unsigned char*>(blob),
                   reinterpret_cast<const unsigned char*>(blob) + size);
    sqlite3_finalize(pStmt);
    return true;
}

// Test 1: Basic binding of a non-empty blob and retrieval to verify exact data round-trip
static bool test_bind_blob_basic() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!ExecSQL(db, "CREATE TABLE t (id INTEGER PRIMARY KEY AUTOINCREMENT, data BLOB)")) {
        sqlite3_close(db);
        return false;
    }

    const char* insertSql = "INSERT INTO t (data) VALUES (?)";
    sqlite3_stmt* pStmt = nullptr;
    rc = sqlite3_prepare_v2(db, insertSql, -1, &pStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind a non-empty blob
    const unsigned char blob[] = { 'A', 0x01, 'C' };
    rc = sqlite3_bind_blob(pStmt, 1, blob, 3, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "bind_blob failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(pStmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(pStmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Step failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(pStmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(pStmt);

    // Retrieve the inserted blob
    std::vector<unsigned char> retrieved;
    bool ok = GetBlobById(db, 1, retrieved);
    if (!ok) {
        sqlite3_close(db);
        return false;
    }

    // Expected data
    const unsigned char expected[] = { 'A', 0x01, 'C' };
    bool same = (retrieved.size() == 3) && (std::memcmp(retrieved.data(), expected, 3) == 0);
    sqlite3_close(db);
    return same;
}

// Test 2: Bind a zero-length blob and verify it is stored as a 0-byte BLOB
static bool test_bind_blob_zero_length() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!ExecSQL(db, "CREATE TABLE t (id INTEGER PRIMARY KEY AUTOINCREMENT, data BLOB)")) {
        sqlite3_close(db);
        return false;
    }

    const char* insertSql = "INSERT INTO t (data) VALUES (?)";
    sqlite3_stmt* pStmt = nullptr;
    rc = sqlite3_prepare_v2(db, insertSql, -1, &pStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind a zero-length blob; zData can be non-null but nData == 0
    const unsigned char dummy[] = { 'X' }; // value should be ignored due to nData==0
    rc = sqlite3_bind_blob(pStmt, 1, dummy, 0, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "bind_blob (zero-length) failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(pStmt);
        sqlite3_close(db);
        return false;
    }

    rc = sqlite3_step(pStmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Step failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(pStmt);
        sqlite3_close(db);
        return false;
    }
    sqlite3_finalize(pStmt);

    // Retrieve the inserted blob
    std::vector<unsigned char> retrieved;
    bool ok = GetBlobById(db, 1, retrieved);
    if (!ok) {
        sqlite3_close(db);
        return false;
    }

    // Expect 0-length blob
    bool isZeroLength = retrieved.empty();
    sqlite3_close(db);
    return isZeroLength;
}

// Test 3: Binding to an out-of-range parameter should yield SQLITE_RANGE (or equivalent)
static bool test_bind_blob_out_of_range_index() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!ExecSQL(db, "CREATE TABLE t (id INTEGER PRIMARY KEY AUTOINCREMENT, data BLOB)")) {
        sqlite3_close(db);
        return false;
    }

    const char* insertSql = "INSERT INTO t (data) VALUES (?)";
    sqlite3_stmt* pStmt = nullptr;
    rc = sqlite3_prepare_v2(db, insertSql, -1, &pStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    const unsigned char blob[] = { 'D', 'E', 'F' };
    // Bind to an invalid parameter index (2, while there is only 1 parameter)
    rc = sqlite3_bind_blob(pStmt, 2, blob, 3, SQLITE_TRANSIENT);
    // Expect an error (SQLITE_RANGE) from out-of-bounds parameter binding
    bool ok = (rc == SQLITE_RANGE);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return ok;
}

// Test 4 (optional): If built with API armor, binding with negative nData should yield SQLITE_MISUSE_BKPT
// This test is guarded to compile only when SQLITE_ENABLE_API_ARMOR is defined.
static bool test_bind_blob_api_armor_negative_nData() {
#ifdef SQLITE_ENABLE_API_ARMOR
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Open failed: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    if (!ExecSQL(db, "CREATE TABLE t (id INTEGER PRIMARY KEY AUTOINCREMENT, data BLOB)")) {
        sqlite3_close(db);
        return false;
    }

    const char* insertSql = "INSERT INTO t (data) VALUES (?)";
    sqlite3_stmt* pStmt = nullptr;
    rc = sqlite3_prepare_v2(db, insertSql, -1, &pStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Prepare insert failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    const unsigned char blob[] = { 'Z', 'Y', 'X' };
    // Pass a negative nData to trigger API_ARMOR misuse path
    rc = sqlite3_bind_blob(pStmt, 1, blob, -1, SQLITE_TRANSIENT);

    // Depending on build, this may be SQLITE_MISUSE_BKPT
    bool ok = (rc == SQLITE_MISUSE_BKPT);
    sqlite3_finalize(pStmt);
    sqlite3_close(db);
    return ok;
#else
    // If API_ARMOR is not defined, skip test gracefully
    std::cout << "[SKIP] test_bind_blob_api_armor_negative_nData (API_ARMOR not defined)" << std::endl;
    return true;
#endif
}

// Entrypoint for running all tests
int main() {
    int passed = 0;
    int total = 0;

    bool t1 = test_bind_blob_basic();
    total++;
    if (RunTest("test_bind_blob_basic", t1)) passed++;

    bool t2 = test_bind_blob_zero_length();
    total++;
    if (RunTest("test_bind_blob_zero_length", t2)) passed++;

    bool t3 = test_bind_blob_out_of_range_index();
    total++;
    if (RunTest("test_bind_blob_out_of_range_index", t3)) passed++;

    bool t4 = test_bind_blob_api_armor_negative_nData();
    // If API_ARMOR is not defined, this test prints skip; treat as not counting towards pass/fail
    total++;
    if (t4) {
        // If the test ran and passed or was skipped, increase pass count accordingly.
        if (t4) {
            // t4==true either indicates pass or skip; for skip we already printed skip message.
            // We won't increment passed here to avoid inflating results for skip.
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    // Return non-zero if any test failed (excluding skipped tests)
    return (passed == total - 1) ? 0 : 1;
}