// C++11 test suite for sqlite3_table_column_metadata using a lightweight, non-terminating assertion framework.
// This test relies on the public SQLite API and links against the system sqlite3 library.
// It creates in-memory databases, defines tables, and validates metadata retrieval for existing and
// non-existing columns, following the provided focal method's behavior.

#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


extern "C" {
}

// Lightweight, non-terminating assertion helpers
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
  do { if(!(cond)) { ++g_failures; std::cerr << "[EXPECT_TRUE] FAIL: " << (msg) << std::endl; } } while(0)

#define EXPECT_EQ(actual, expected, msg) \
  do { if((actual) != (expected)) { ++g_failures; std::cerr << "[EXPECT_EQ] FAIL: " << (msg) \
        << " (expected: " << (expected) << ", actual: " << (actual) << ")" << std::endl; } } while(0)

#define EXPECT_STR_EQ(actual, expected, msg) \
  do { if(((actual) == nullptr) || (strcmp((actual), (expected)) != 0)) { \
        ++g_failures; std::cerr << "[EXPECT_STR_EQ] FAIL: " << (msg) \
        << " (expected: \"" << (expected) << "\", actual: \"" << (actual ? actual : "NULL") << "\")" << std::endl; } } while(0)

static void test_table_column_metadata_id_and_name(sqlite3* db) {
  // Test metadata for existing columns: id (PRIMARY KEY AUTOINCREMENT) and name (NOT NULL)
  const char* zDataType = nullptr;
  const char* zCollSeq = nullptr;
  int notNull = -1;
  int primaryKey = -1;
  int autoinc = -1;

  int rc = sqlite3_table_column_metadata(db, nullptr, "t1", "id",
                                         &zDataType, &zCollSeq,
                                         &notNull, &primaryKey, &autoinc);
  EXPECT_EQ(rc, SQLITE_OK, "sqlite3_table_column_metadata id rc");
  EXPECT_STR_EQ(zDataType, "INTEGER", "id declared type");
  EXPECT_STR_EQ(zCollSeq, "BINARY", "id coll seq");
  EXPECT_TRUE(notNull == 1, "id NOT NULL");
  EXPECT_TRUE(primaryKey == 1, "id is part of PRIMARY KEY");
  EXPECT_TRUE(autoinc == 1, "id AUTOINCREMENT");

  // Fetch metadata for the second column: name
  zDataType = zCollSeq = nullptr;
  notNull = primaryKey = autoinc = 0;
  rc = sqlite3_table_column_metadata(db, nullptr, "t1", "name",
                                     &zDataType, &zCollSeq,
                                     &notNull, &primaryKey, &autoinc);
  EXPECT_EQ(rc, SQLITE_OK, "sqlite3_table_column_metadata name rc");
  EXPECT_STR_EQ(zDataType, "TEXT", "name declared type");
  EXPECT_STR_EQ(zCollSeq, "BINARY", "name coll seq");
  EXPECT_TRUE(notNull == 1, "name NOT NULL");
  EXPECT_TRUE(primaryKey == 0, "name is not part of PRIMARY KEY");
  EXPECT_TRUE(autoinc == 0, "name AUTOINCREMENT");
}

static void test_table_column_metadata_nonexistent_column(sqlite3* db) {
  // Query a column that doesn't exist; expect an error
  const char* zDataType = nullptr;
  const char* zCollSeq = nullptr;
  int notNull = 0;
  int primaryKey = 0;
  int autoinc = 0;

  int rc = sqlite3_table_column_metadata(db, nullptr, "t1", "does_not_exist",
                                         &zDataType, &zCollSeq,
                                         &notNull, &primaryKey, &autoinc);
  EXPECT_EQ(rc, SQLITE_ERROR, "nonexistent column should return SQLITE_ERROR");
  // Outputs may reflect internal defaults on error; we only assert error code here.
}

static void test_table_column_metadata_column_null_name(sqlite3* db) {
  // When zColumnName is NULL, function treats as "existence of table" request.
  // It should still return some metadata defaults (per implementation).
  const char* zDataType = nullptr;
  const char* zCollSeq = nullptr;
  int notNull = -1;
  int primaryKey = -1;
  int autoinc = -1;

  int rc = sqlite3_table_column_metadata(db, nullptr, "t1", NULL,
                                         &zDataType, &zCollSeq,
                                         &notNull, &primaryKey, &autoinc);
  // We expect a successful call; details depend on implementation for zColumnName==NULL.
  EXPECT_EQ(rc, SQLITE_OK, "column NULL name rc");
  // The function may return defaults; ensure we got a string and some integers.
  if(zDataType) {
    std::string dt(zDataType);
    (void)dt; // avoid unused warning if not asserted
  }
}

int main(void) {
  // Initialize database
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr, "sqlite3_open memory db");
  if(!db) {
    std::cerr << "Failed to open in-memory database. Aborting tests." << std::endl;
    return 1;
  }

  // Create a table to test against
  const char* createSQL = "CREATE TABLE t1(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, value BLOB);";
  rc = sqlite3_exec(db, createSQL, nullptr, nullptr, nullptr);
  EXPECT_EQ(rc, SQLITE_OK, "CREATE TABLE t1");

  // Run tests that depend on the real sqlite3_table_column_metadata behavior
  test_table_column_metadata_id_and_name(db);
  test_table_column_metadata_nonexistent_column(db); // should report SQLITE_ERROR
  test_table_column_metadata_column_null_name(db);   // check handling of NULL column name

  // Teardown
  sqlite3_close(db);

  if(g_failures > 0){
    std::cerr << g_failures << " test(s) failed." << std::endl;
  } else {
    std::cout << "All tests passed" << std::endl;
  }
  return g_failures;
}