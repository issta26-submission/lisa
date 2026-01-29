// Unit test suite for sqlite3_prepare16_v2
// Focus: generate a small, executable C++11 test harness without GTest.
// The tests exercise the focal method via the real SQLite library if available.

#include <functional>
#include <vector>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// Global failure counter to allow non-terminating tests.
static int g_failed = 0;

// Lightweight, non-terminating test assertions (do not abort on failure).
#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { std::cerr << "EXPECT_TRUE FAILED: " << (msg) << std::endl; ++g_failed; } } while(0)

#define EXPECT_FALSE(cond, msg) \
  do { if((cond))  { std::cerr << "EXPECT_FALSE FAILED: " << (msg) << std::endl; ++g_failed; } } while(0)

#define EXPECT_EQ(actual, expected, msg) \
  do { if(!((actual) == (expected))) { std::cerr << "EXPECT_EQ FAILED: " << (msg) \
        << " (actual=" << (actual) << ", expected=" << (expected) << ")" << std::endl; ++g_failed; } } while(0)

#define EXPECT_NOT_NULL(ptr, msg) \
  do { if((ptr) == nullptr) { std::cerr << "EXPECT_NOT_NULL FAILED: " << (msg) << std::endl; ++g_failed; } } while(0)

// Test 1: Basic successful path
// - Open in-memory database
// - Prepare a valid UTF-16 SQL statement
// - Expect SQLITE_OK and non-null sqlite3_stmt*
void test_prepare16_v2_basic() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_NOT_NULL(db, "sqlite3_open should yield non-null db handle");
  EXPECT_EQ(rc, SQLITE_OK, "sqlite3_open(:memory:) should return SQLITE_OK");

  if (db != nullptr && rc == SQLITE_OK) {
    // UTF-16 SQL string:  "SELECT 1;"
    std::u16string sql = u"SELECT 1;";
    int nBytes = static_cast<int>(sql.size()) * static_cast<int>(sizeof(char16_t));
    sqlite3_stmt* stmt = nullptr;
    const void* tail_out = nullptr;
    const void** ppTail = &tail_out;

    rc = sqlite3_prepare16_v2(db, sql.data(), nBytes, &stmt, ppTail);
    EXPECT_EQ(rc, SQLITE_OK, "sqlite3_prepare16_v2 should succeed for valid UTF-16 SQL");
    EXPECT_NOT_NULL(stmt, "sqlite3_prepare16_v2 should output a non-null statement on success");

    if (stmt) {
      sqlite3_finalize(stmt);
    }
  }

  if (db) {
    sqlite3_close(db);
  }
}

// Test 2: Invalid SQL should produce a non-OK return code
// - Open in-memory DB
// - Provide obviously invalid SQL
// - Expect rc != SQLITE_OK and that stmt is NULL (typical behavior)
void test_prepare16_v2_invalid_sql() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_EQ(rc, SQLITE_OK, "sqlite3_open(:memory:) should return SQLITE_OK");
  if (db != nullptr && rc == SQLITE_OK) {
    // Invalid UTF-16 SQL
    std::u16string sql = u"SELEC X 1;"; // misspelled SELECT
    int nBytes = static_cast<int>(sql.size()) * static_cast<int>(sizeof(char16_t));
    sqlite3_stmt* stmt = nullptr;
    const void* tail_out = nullptr;
    const void** ppTail = &tail_out;

    rc = sqlite3_prepare16_v2(db, sql.data(), nBytes, &stmt, ppTail);
    EXPECT_TRUE(rc != SQLITE_OK, "sqlite3_prepare16_v2 should fail for invalid SQL");
    EXPECT_TRUE(stmt == nullptr, "On failure, statement should be NULL");
  }

  if (db) {
    sqlite3_close(db);
  }
}

// Test 3: Verify that pzTail (tail pointer) is set for valid statements
// - Open DB
// - Prepare a valid UTF-16 SQL with a non-null tail pointer
// - Ensure tail pointer is non-null after parsing
void test_prepare16_v2_tail() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_EQ(rc, SQLITE_OK, "sqlite3_open(:memory:) should return SQLITE_OK");
  if (db != nullptr && rc == SQLITE_OK) {
    std::u16string sql = u"SELECT 1;";
    int nBytes = static_cast<int>(sql.size()) * static_cast<int>(sizeof(char16_t));
    sqlite3_stmt* stmt = nullptr;
    const void* tail_out = nullptr;
    const void** ppTail = &tail_out;

    rc = sqlite3_prepare16_v2(db, sql.data(), nBytes, &stmt, ppTail);
    EXPECT_EQ(rc, SQLITE_OK, "sqlite3_prepare16_v2 should succeed for valid SQL with tail");
    EXPECT_NOT_NULL(stmt, "stmt should be non-null on success");
    EXPECT_NOT_NULL(tail_out, "pzTail should be set (non-null) after parsing valid SQL");
    if (stmt) {
      sqlite3_finalize(stmt);
    }
  }
  if (db) {
    sqlite3_close(db);
  }
}

// Simple test harness runner
int main() {
  std::cout << "Starting sqlite3_prepare16_v2 unit tests (no GTest, C++11)..." << std::endl;

  test_prepare16_v2_basic();
  test_prepare16_v2_invalid_sql();
  test_prepare16_v2_tail();

  if (g_failed > 0) {
    std::cerr << "1 or more tests FAILED: " << g_failed << " failure(s)" << std::endl;
    return 1;
  } else {
    std::cout << "All tests PASSED" << std::endl;
    return 0;
  }
}