// Minimal C++11 unit tests for sqlite3DbNameToBtree
// This test suite provides self-contained stubs for the minimal
// sqlite3 database structure and functions necessary to exercise the
// focal function sqlite3DbNameToBtree without requiring the full SQLite project.
//
// The tests cover:
// - Null zDbName (should default to first database slot)
// - Existing database name (should return corresponding Btree pointer)
// - Non-existent database name (should return null)
// - Existing database name at index 0 (edge case sanity)

// Note: This file is self-contained and does not depend on external testing libraries.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Minimal Btree stub
struct Btree {
  int id; // simple identifier to distinguish bt instances
};

// Database entry holding a Btree pointer and a database name
struct sqlite3_dummy_DbEntry {
  Btree *pBt;
  const char *zDbName;
};

// Minimal sqlite3 stub containing an array of database entries
struct sqlite3_dummy {
  int nDb;
  sqlite3_dummy_DbEntry *aDb;
};

// Forward declaration of helper used by focal function
static int sqlite3FindDbName(sqlite3_dummy *db, const char *zName);

// Focal function under test (reimplemented to be self-contained for unit testing)
static Btree *sqlite3DbNameToBtree(sqlite3_dummy *db, const char *zDbName){
  int iDb = zDbName ? sqlite3FindDbName(db, zDbName) : 0;
  return iDb<0 ? 0 : db->aDb[iDb].pBt;
}

// Helper: mimic behavior of sqlite3FindDbName used by the focal function
static int sqlite3FindDbName(sqlite3_dummy *db, const char *zName){
  if (!db || !db->aDb) return -1;
  if (!zName) return -1; // shouldn't be called with NULL in our usage, but guard anyway
  for (int i = 0; i < db->nDb; ++i){
    const char *name = db->aDb[i].zDbName;
    if (name && std::strcmp(name, zName) == 0){
      return i;
    }
  }
  return -1;
}

// Simple test harness (non-terminating expectations)
static int gFailCount = 0;
#define EXPECT_EQ_PTR(a, b, msg) do { \
  if ((a) != (b)) { \
    ++gFailCount; \
    std::cerr << "[FAIL] " << (msg) << " | Expected ptr: " << (void*)(b) \
              << ", Got: " << (void*)(a) << std::endl; \
  } \
} while(0)

#define EXPECT_EQ_INT(a, b, msg) do { \
  if ((a) != (b)) { \
    ++gFailCount; \
    std::cerr << "[FAIL] " << (msg) << " | Expected int: " << (b) \
              << ", Got: " << (a) << std::endl; \
  } \
} while(0)

#define EXPECT_NOT_NULL(a, msg) do { \
  if ((a) == nullptr) { \
    ++gFailCount; \
    std::cerr << "[FAIL] " << (msg) << " | Received null pointer" << std::endl; \
  } \
} while(0)

// Test 1: Null zDbName should return the first database's Btree pointer
void test_sqlite3DbNameToBtree_nullName(){
  // Arrange
  Btree bt0{0};
  Btree bt1{1};
  sqlite3_dummy db;
  db.nDb = 2;
  sqlite3_dummy_DbEntry entries[2];
  entries[0].pBt = &bt0; entries[0].zDbName = "main";
  entries[1].pBt = &bt1; entries[1].zDbName = "secondary";
  db.aDb = entries;

  // Act
  Btree *res = sqlite3DbNameToBtree(&db, nullptr);

  // Assert
  EXPECT_EQ_PTR(res, &bt0, "Null zDbName should map to index 0 (first database)");
  // Clean up not necessary for stack-allocated test data
}

// Test 2: Existing database name should return the corresponding Btree
void test_sqlite3DbNameToBtree_existingName(){
  // Arrange
  Btree bt0{0};
  Btree bt1{1};
  sqlite3_dummy db;
  db.nDb = 2;
  sqlite3_dummy_DbEntry entries[2];
  entries[0].pBt = &bt0; entries[0].zDbName = "main";
  entries[1].pBt = &bt1; entries[1].zDbName = "audit";
  db.aDb = entries;

  // Act
  Btree *res = sqlite3DbNameToBtree(&db, "audit");

  // Assert
  EXPECT_EQ_PTR(res, &bt1, "Existing database name 'audit' should map to its Btree");
}

// Test 3: Non-existent database name should return null
void test_sqlite3DbNameToBtree_nonexistentName(){
  // Arrange
  Btree bt0{0};
  sqlite3_dummy db;
  db.nDb = 1;
  sqlite3_dummy_DbEntry entries[1];
  entries[0].pBt = &bt0; entries[0].zDbName = "main";
  db.aDb = entries;

  // Act
  Btree *res = sqlite3DbNameToBtree(&db, "does_not_exist");

  // Assert
  EXPECT_EQ_PTR(res, nullptr, "Non-existent database name should return null");
}

// Test 4 (edge case): Existing name at index 0 should still map correctly
void test_sqlite3DbNameToBtree_nameAtIndexZero(){
  // Arrange
  Btree bt0{0};
  Btree bt1{1};
  sqlite3_dummy db;
  db.nDb = 2;
  sqlite3_dummy_DbEntry entries[2];
  entries[0].pBt = &bt0; entries[0].zDbName = "main";
  entries[1].pBt = &bt1; entries[1].zDbName = "temp";
  db.aDb = entries;

  // Act
  Btree *res = sqlite3DbNameToBtree(&db, "main");

  // Assert
  EXPECT_EQ_PTR(res, &bt0, "Database name 'main' at index 0 should map to bt0");
}

// Main function to run tests
int main() {
  std::cout << "Running sqlite3DbNameToBtree unit tests (self-contained)..." << std::endl;

  test_sqlite3DbNameToBtree_nullName();
  test_sqlite3DbNameToBtree_existingName();
  test_sqlite3DbNameToBtree_nonexistentName();
  test_sqlite3DbNameToBtree_nameAtIndexZero();

  if (gFailCount == 0) {
    std::cout << "All tests PASSED." << std::endl;
    return 0;
  } else {
    std::cout << gFailCount << " test(s) FAILED." << std::endl;
    return 1;
  }
}