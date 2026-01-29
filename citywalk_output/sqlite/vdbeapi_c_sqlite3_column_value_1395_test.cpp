// Test suite for sqlite3_column_value (focal method) using the public SQLite C API.
// This test is written in C++11 without GTest. A lightweight non-terminating assertion
// framework is used to maximize code execution paths for coverage.
// The tests exercise: (i) the value retrieval path through sqlite3_column_value via
// a prepared statement, (ii) retrieval of integers, (iii) retrieval of text, and
// (iv) retrieval of blob data. The internal MEM_Static -> MEM_Ephem behavior is a
// private detail of the original implementation; we validate only the observable
// API behavior and data integrity via standard sqlite3_value_* accessors.

#include <vector>
#include <cstdio>
#include <opcodes.h>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Lightweight test framework (non-terminating assertions)
static int gTotalAssertions = 0;
static int gFailedAssertions = 0;

#define EXPECT_TRUE(cond, desc) do { \
    ++gTotalAssertions; \
    if(!(cond)) { \
        std::cerr << "FAIL: " << (desc) << "\n"; \
        ++gFailedAssertions; \
    } else { \
        /* Pass: do not terminate, continue to next assertion */ \
        std::cout << "PASS: " << (desc) << "\n"; \
    } \
} while(0)

// Helper to print a test header
static void printTestHeader(const std::string &title) {
    std::cout << "\n=== Test: " << title << " ===\n";
}

// Test 1: sqlite3_column_value returns a non-null sqlite3_value* for a simple integer column.
// We verify that sqlite3_value_type() returns SQLITE_INTEGER and sqlite3_value_int() yields the
// expected value. This exercises the typical code path inside sqlite3_column_value when the
// underlying Mem represents a static integer converted to a sqlite3_value.
static void test_column_value_returns_integer() {
    printTestHeader("Column value retrieval for INTEGER column");

    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "Open in-memory DB");

    // Create a simple table and insert an integer value.
    rc = sqlite3_exec(db, "CREATE TABLE t(i INTEGER); INSERT INTO t VALUES(42);",
                      nullptr, nullptr, nullptr);
    EXPECT_TRUE(rc == SQLITE_OK, "Create table and insert integer 42");

    // Prepare statement to read the integer value.
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT i FROM t", -1, &stmt, nullptr);
    EXPECT_TRUE(rc == SQLITE_OK && stmt != nullptr, "Prepare statement for integer column");

    // Step to first (and only) row.
    rc = sqlite3_step(stmt);
    EXPECT_TRUE(rc == SQLITE_ROW, "Step to first row");

    // Retrieve the value using sqlite3_column_value (the focal method under test).
    sqlite3_value *pVal = sqlite3_column_value(stmt, 0);
    EXPECT_TRUE(pVal != nullptr, "sqlite3_column_value should not return NULL for valid column");

    // Validate the type and content through public APIs.
    int type = sqlite3_value_type(pVal);
    int intVal = sqlite3_value_int(pVal);
    EXPECT_TRUE(type == SQLITE_INTEGER, "Value type should be SQLITE_INTEGER");
    EXPECT_TRUE(intVal == 42, "Value should be integer 42");

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test 2: sqlite3_column_value retrieves TEXT data correctly.
// This validates public API behavior when the column contains text data.
static void test_column_value_returns_text() {
    printTestHeader("Column value retrieval for TEXT column");

    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "Open in-memory DB for TEXT test");

    rc = sqlite3_exec(db, "CREATE TABLE t(s TEXT); INSERT INTO t VALUES('hello world');",
                      nullptr, nullptr, nullptr);
    EXPECT_TRUE(rc == SQLITE_OK, "Create table and insert text 'hello world'");

    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT s FROM t", -1, &stmt, nullptr);
    EXPECT_TRUE(rc == SQLITE_OK && stmt != nullptr, "Prepare statement for TEXT column");

    rc = sqlite3_step(stmt);
    EXPECT_TRUE(rc == SQLITE_ROW, "Step to first row for TEXT");

    sqlite3_value *pVal = sqlite3_column_value(stmt, 0);
    EXPECT_TRUE(pVal != nullptr, "sqlite3_column_value should not return NULL for TEXT");

    int type = sqlite3_value_type(pVal);
    const unsigned char *text = sqlite3_value_text(pVal);
    std::string gotText = text ? reinterpret_cast<const char*>(text) : "";

    EXPECT_TRUE(type == SQLITE_TEXT, "Value type should be SQLITE_TEXT");
    EXPECT_TRUE(gotText == "hello world", "Text content should be 'hello world'");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Test 3: sqlite3_column_value retrieves BLOB data correctly.
// This exercises the blob path and ensures data integrity via sqlite3_value_blob.
static void test_column_value_returns_blob() {
    printTestHeader("Column value retrieval for BLOB column");

    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(db != nullptr && rc == SQLITE_OK, "Open in-memory DB for BLOB test");

    // Insert a known blob: 0x01 0x02 0x03
    rc = sqlite3_exec(db, "CREATE TABLE t(b BLOB); INSERT INTO t VALUES(X'010203');",
                      nullptr, nullptr, nullptr);
    EXPECT_TRUE(rc == SQLITE_OK, "Create table and insert blob X'010203'");

    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT b FROM t", -1, &stmt, nullptr);
    EXPECT_TRUE(rc == SQLITE_OK && stmt != nullptr, "Prepare statement for BLOB column");

    rc = sqlite3_step(stmt);
    EXPECT_TRUE(rc == SQLITE_ROW, "Step to first row for BLOB");

    sqlite3_value *pVal = sqlite3_column_value(stmt, 0);
    EXPECT_TRUE(pVal != nullptr, "sqlite3_column_value should not return NULL for BLOB");

    int type = sqlite3_value_type(pVal);
    const void *blob = sqlite3_value_blob(pVal);
    int size = sqlite3_value_bytes(pVal);

    // Validate type and size, and examine raw bytes
    EXPECT_TRUE(type == SQLITE_BLOB, "Value type should be SQLITE_BLOB");
    const unsigned char expected[] = {0x01, 0x02, 0x03};
    std::string hex = "";
    if(blob && size > 0) {
        const unsigned char *b = static_cast<const unsigned char*>(blob);
        // Copy bytes to hex-like representation for comparison
        for(int i = 0; i < size; ++i) {
            char buf[4];
            snprintf(buf, sizeof(buf), "%02X", b[i]);
            hex += buf;
        }
    }
    // Expect hex string to match "010203"
    EXPECT_TRUE(size == 3, "Blob size should be 3 bytes");
    EXPECT_TRUE(hex == "010203", "Blob content should be 01 02 03");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int main() {
    // Run tests
    test_column_value_returns_integer();
    test_column_value_returns_text();
    test_column_value_returns_blob();

    // Summary
    std::cout << "\nTest summary: " << gTotalAssertions << " assertions, "
              << gFailedAssertions << " failures.\n";

    // Return non-zero if any test failed
    return (gFailedAssertions == 0) ? 0 : 1;
}