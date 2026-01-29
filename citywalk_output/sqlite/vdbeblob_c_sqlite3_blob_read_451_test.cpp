// This test suite exercises the focal method sqlite3_blob_read via the public SQLite blob API.
// It uses a small in-house test harness (no Google Test) and prints PASS/FAIL per test.
// Prerequisites: SQLite3 library must be available and linked (e.g., -lsqlite3).
// The tests rely on the public API: sqlite3_open, sqlite3_blob_open, sqlite3_blob_read,
// sqlite3_blob_bytes, sqlite3_blob_close, sqlite3_bind_blob, sqlite3_prepare_v2, sqlite3_step, etc.

#include <vector>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_totalTests; \
    if(!(cond)) { \
        ++g_failedTests; \
        std::cerr << "[FAIL] " #cond " at line " << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual) do { \
    ++g_totalTests; \
    if(!((expected) == (actual))) { \
        ++g_failedTests; \
        std::cerr << "[FAIL] Expected (" #expected ") == Actual (" #actual "), values: " \
                  << (expected) << " vs " << (actual) << " at line " << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_NEQ(expected, actual) do { \
    ++g_totalTests; \
    if((expected) == (actual)) { \
        ++g_failedTests; \
        std::cerr << "[FAIL] Expected (" #expected ") != Actual (" #actual "), value: " \
                  << (actual) << " at line " << __LINE__ << std::endl; \
    } \
} while(0)

// Helper to create a blob in memory
static std::vector<unsigned char> makeBlobContent(const unsigned char* src, size_t len) {
    if(src == nullptr || len == 0) return std::vector<unsigned char>();
    return std::vector<unsigned char>(src, src + len);
}

// Test 1: Read an entire blob in one go and verify contents.
// This tests that sqlite3_blob_read forwards to the underlying blobReadWrite path and
// that we can retrieve the exact bytes originally stored in the blob.
static void test_blob_read_entire_blob() {
    // Original blob data
    const unsigned char original[] = { 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
                                     0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88 };
    const size_t blobLen = sizeof(original);

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* pBlob = nullptr;

    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(rc == SQLITE_OK && db != nullptr);

    // Create table
    const char* createSQL = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, createSQL, nullptr, nullptr, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);

    // Insert blob
    const char* insertSQL = "INSERT INTO t(data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);
    // Bind blob; use SQLITE_TRANSIENT to copy data
    rc = sqlite3_bind_blob(stmt, 1, original, (int)blobLen, SQLITE_TRANSIENT);
    EXPECT_EQ(SQLITE_OK, rc);
    rc = sqlite3_step(stmt);
    EXPECT_EQ(SQLITE_DONE, rc);
    sqlite3_finalize(stmt);

    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);

    // Open blob for reading
    rc = sqlite3_blob_open(db, "main", "t", "data", rowid, 0, &pBlob);
    EXPECT_EQ(SQLITE_OK, rc);
    EXPECT_TRUE(pBlob != nullptr);

    // Prepare buffer and read
    std::vector<unsigned char> buffer(blobLen);
    rc = sqlite3_blob_read(pBlob, buffer.data(), (int)blobLen, 0);
    EXPECT_EQ(SQLITE_OK, rc);
    // Validate content
    EXPECT_EQ(0, std::memcmp(buffer.data(), original, blobLen));

    // Cleanup
    sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 2: Read blob in two steps (partial reads) and verify combined result.
// This checks that subsequent reads correctly advance the offset and return the expected data.
static void test_blob_read_in_two_steps() {
    // Original blob data
    const unsigned char original[] = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
        0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,
        0x11,0x12,0x13,0x14
    };
    const size_t blobLen = sizeof(original);

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* pBlob = nullptr;

    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(rc == SQLITE_OK && db != nullptr);

    rc = sqlite3_exec(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);", nullptr, nullptr, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);

    const char* insertSQL = "INSERT INTO t(data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);
    rc = sqlite3_bind_blob(stmt, 1, original, (int)blobLen, SQLITE_TRANSIENT);
    EXPECT_EQ(SQLITE_OK, rc);
    rc = sqlite3_step(stmt);
    EXPECT_EQ(SQLITE_DONE, rc);
    sqlite3_finalize(stmt);

    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);

    rc = sqlite3_blob_open(db, "main", "t", "data", rowid, 0, &pBlob);
    EXPECT_EQ(SQLITE_OK, rc);
    EXPECT_TRUE(pBlob != nullptr);

    // Read first half
    size_t half = blobLen / 2;
    std::vector<unsigned char> firstHalf(half);
    rc = sqlite3_blob_read(pBlob, firstHalf.data(), (int)half, 0);
    EXPECT_EQ(SQLITE_OK, rc);

    // Read second half
    std::vector<unsigned char> secondHalf(blobLen - half);
    rc = sqlite3_blob_read(pBlob, secondHalf.data(), (int)(blobLen - half), (int)half);
    EXPECT_EQ(SQLITE_OK, rc);

    // Combine and compare
    std::vector<unsigned char> combined;
    combined.reserve(blobLen);
    combined.insert(combined.end(), firstHalf.begin(), firstHalf.end());
    combined.insert(combined.end(), secondHalf.begin(), secondHalf.end());

    EXPECT_EQ(0, std::memcmp(combined.data(), original, blobLen));

    sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 3: Attempt to read with an offset beyond blob length and verify error handling.
// The API should return a non-SQLITE_OK status for invalid offset.
static void test_blob_read_invalid_offset() {
    // Simple 4-byte blob
    const unsigned char original[] = { 0xAA, 0xBB, 0xCC, 0xDD };
    const size_t blobLen = sizeof(original);

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* pBlob = nullptr;

    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(rc == SQLITE_OK && db != nullptr);

    rc = sqlite3_exec(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);", nullptr, nullptr, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);

    rc = sqlite3_prepare_v2(db, "INSERT INTO t(data) VALUES (?);", -1, &stmt, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);
    rc = sqlite3_bind_blob(stmt, 1, original, (int)blobLen, SQLITE_TRANSIENT);
    EXPECT_EQ(SQLITE_OK, rc);
    rc = sqlite3_step(stmt);
    EXPECT_EQ(SQLITE_DONE, rc);
    sqlite3_finalize(stmt);

    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
    rc = sqlite3_blob_open(db, "main", "t", "data", rowid, 0, &pBlob);
    EXPECT_EQ(SQLITE_OK, rc);
    EXPECT_TRUE(pBlob != nullptr);

    // Read with an invalid offset (beyond blob length)
    std::vector<unsigned char> buf(blobLen);
    rc = sqlite3_blob_read(pBlob, buf.data(), (int)blobLen, (int)blobLen + 1);
    EXPECT_NEQ(SQLITE_OK, rc);

    sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 4: Read zero bytes; expect SQLITE_OK and zero bytes read (buffer unused).
static void test_blob_read_zero_bytes() {
    const unsigned char original[] = { 0xDE, 0xAD, 0xBE, 0xEF };
    const size_t blobLen = sizeof(original);

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* pBlob = nullptr;

    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(rc == SQLITE_OK && db != nullptr);

    rc = sqlite3_exec(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);", nullptr, nullptr, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);

    rc = sqlite3_prepare_v2(db, "INSERT INTO t(data) VALUES (?);", -1, &stmt, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);
    rc = sqlite3_bind_blob(stmt, 1, original, (int)blobLen, SQLITE_TRANSIENT);
    EXPECT_EQ(SQLITE_OK, rc);
    rc = sqlite3_step(stmt);
    EXPECT_EQ(SQLITE_DONE, rc);
    sqlite3_finalize(stmt);

    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);
    rc = sqlite3_blob_open(db, "main", "t", "data", rowid, 0, &pBlob);
    EXPECT_EQ(SQLITE_OK, rc);
    EXPECT_TRUE(pBlob != nullptr);

    // Read zero bytes
    std::vector<unsigned char> buf(blobLen);
    rc = sqlite3_blob_read(pBlob, buf.data(), 0, 0);
    EXPECT_EQ(SQLITE_OK, rc);
    // Buffer should remain unchanged (not required, but ensure no crash)

    sqlite3_blob_close(pBlob);
    sqlite3_close(db);
}

// Test 5: Attempt to open a blob with a non-existent row and verify error is returned.
// This ensures the API responds correctly to invalid row identifiers.
static void test_blob_open_invalid_row() {
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    EXPECT_TRUE(rc == SQLITE_OK && db != nullptr);

    rc = sqlite3_exec(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);", nullptr, nullptr, nullptr);
    EXPECT_EQ(SQLITE_OK, rc);

    // No rows inserted; attempt to open a blob for a non-existent row
    sqlite3_blob* pBlob = nullptr;
    rc = sqlite3_blob_open(db, "main", "t", "data", 999999, 0, &pBlob);
    // Expect an error since the row doesn't exist
    EXPECT_NEQ(SQLITE_OK, rc);
    EXPECT_TRUE(pBlob == nullptr);

    sqlite3_close(db);
}

int main() {
    std::cout << "Starting sqlite3_blob_read unit tests (C++11, no GTest)..." << std::endl;

    test_blob_read_entire_blob();
    test_blob_read_in_two_steps();
    test_blob_read_invalid_offset();
    test_blob_read_zero_bytes();
    test_blob_open_invalid_row();

    std::cout << "Tests completed. Total: " << g_totalTests
              << ", Failed: " << g_failedTests << std::endl;

    // Return 0 to indicate tests completed; non-zero if any failures occurred.
    return (g_failedTests == 0) ? 0 : 1;
}