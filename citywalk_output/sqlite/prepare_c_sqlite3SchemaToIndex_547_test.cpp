// Test suite for sqlite3SchemaToIndex (Step 2 & Step 3: generate and refine tests)
// This test suite is written in C++11 without GTest.
// It relies on the internal SQLite structures (sqlite3, Db, Schema) exposed via
// sqliteInt.h and public interfaces (sqlite3_mutex_*). It aims to verify the
// behavior of sqlite3SchemaToIndex for key cases:
// - pSchema == nullptr -> returns -32768
// - pSchema matches the first entry in db->aDb -> returns 0
// - pSchema matches a later entry (e.g., index 2) -> returns that index
// Notes:
// - The internal asserts in sqlite3SchemaToIndex are present; tests try to keep
//   them from triggering by ensuring a proper mutex is held before invocation.
// - Compile-time linking with SQLite library is required (e.g., -lsqlite3) and
//   include paths must expose sqliteInt.h for internal types.
// - This test is designed to run in an environment where NDEBUG is not forcibly defined
//   to ensure internal asserts behave as in normal builds, but we explicitly acquire
//   the mutex to keep the path safe when asserts are enabled.
// - For demonstration purposes, tests print PASS/FAIL statuses and explanatory comments.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstring>


// Include SQLite internal headers to access sqlite3, Db, Schema and mutex APIs.
// Depending on the project layout, you may need to adjust include paths.
extern "C" {
  // Public API to obtain a mutex allocator and related operations
  #include <sqlite3.h>
  // Internal definitions used by sqlite3SchemaToIndex
  // This header is typically located in the SQLite source tree.
  #include "sqliteInt.h"

  // Function under test (C linkage)
  int sqlite3SchemaToIndex(sqlite3 *db, Schema *pSchema);
}

// Helper function for printing test results
static void printTestResult(const std::string &testName, bool pass, const std::string &details = "") {
  std::cout << (pass ? "[PASS] " : "[FAIL] ") << testName;
  if (!details.empty()) {
    std::cout << " -- " << details;
  }
  std::cout << std::endl;
}

// Helper to safely allocate and prepare a sqlite3 test database instance
// This function creates a sqlite3 object with 'nDb' backends and aDb array,
// and holds a valid mutex so sqlite3SchemaToIndex's mutex-ownership assert passes.
static sqlite3* createTestDbWithN(int nDb) {
  // Allocate a zero-initialized sqlite3 structure
  sqlite3 *db = (sqlite3*)std::calloc(1, sizeof(sqlite3));
  if (!db) return nullptr;

  db->nDb = nDb;

  // Allocate a Db array (aDb) with 'nDb' elements.
  // Db is defined in sqliteInt.h and contains at least a member 'pSchema'.
  db->aDb = (Db*)std::calloc(nDb, sizeof(Db));
  if (!db->aDb) {
    std::free(db);
    return nullptr;
  }

  // Initialize all pSchema to nullptr
  for (int i = 0; i < nDb; ++i) {
    db->aDb[i].pSchema = nullptr;
  }

  // Acquire a real mutex so that sqlite3Mutex_held(db->mutex) can be true.
  // sqlite3_mutex_alloc, sqlite3_mutex_enter/held are part of the SQLite internals.
  // We choose FAST mutex type for typical usage.
  db->mutex = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
  if (db->mutex) {
    sqlite3_mutex_enter(db->mutex);
  }

  return db;
}

// Helper to clean up a test db instance
static void destroyTestDb(sqlite3 *db) {
  if (!db) return;
  // Note: We intentionally avoid calling sqlite3_mutex_leave/free to keep test simple,
  // relying on process exit to reclaim resources in this test harness.
  // Free aDb array
  if (db->aDb) {
    // If pSchema was allocated by tests, ensure they are freed by the test itself:
    // However, since test may pass external Schema pointers, we only free the array here.
    std::free(db->aDb);
  }
  std::free(db);
}

int main() {
  // Test 1: pSchema == nullptr should return -32768
  {
    // Arrange
    sqlite3 *db = createTestDbWithN(2);
    if (!db) {
      printTestResult("Test 1: setup", false, "failed to allocate test db");
      return 1;
    }

    // Ensure both aDb entries are present but with no schemas
    // (they are already nullptr in createTestDbWithN)

    // Act
    int result = sqlite3SchemaToIndex(db, nullptr);

    // Assert
    const int expected = -32768;
    bool pass = (result == expected);
    std::string details = "returned " + std::to_string(result) + ", expected " + std::to_string(expected);
    printTestResult("Test 1: pSchema == nullptr returns -32768", pass, details);

    // Cleanup
    destroyTestDb(db);
  }

  // Test 2: pSchema matches the first entry (index 0)
  {
    // Arrange
    sqlite3 *db = createTestDbWithN(3);
    if (!db) {
      printTestResult("Test 2: setup", false, "failed to allocate test db");
      return 1;
    }

    // Create a Schema instance to represent the first schema.
    Schema *s0 = new Schema();

    // Put s0 into aDb[0]
    db->aDb[0].pSchema = s0;

    // Ensure aDb[1] and aDb[2] have no schemas
    db->aDb[1].pSchema = nullptr;
    db->aDb[2].pSchema = nullptr;

    // Act
    int result = sqlite3SchemaToIndex(db, s0);

    // Assert
    const int expected = 0;
    bool pass = (result == expected);
    std::string details = "returned " + std::to_string(result) + ", expected " + std::to_string(expected);
    printTestResult("Test 2: pSchema matches index 0 -> 0", pass, details);

    // Cleanup
    delete s0;
    destroyTestDb(db);
  }

  // Test 3: pSchema matches a non-first entry (e.g., index 2)
  {
    // Arrange
    sqlite3 *db = createTestDbWithN(3);
    if (!db) {
      printTestResult("Test 3: setup", false, "failed to allocate test db");
      return 1;
    }

    // Create a Schema instance and place it at index 2
    Schema *s2 = new Schema();
    db->aDb[2].pSchema = s2;

    // Act
    int result = sqlite3SchemaToIndex(db, s2);

    // Assert
    const int expected = 2;
    bool pass = (result == expected);
    std::string details = "returned " + std::to_string(result) + ", expected " + std::to_string(expected);
    printTestResult("Test 3: pSchema matches index 2 -> 2", pass, details);

    // Cleanup
    delete s2;
    destroyTestDb(db);
  }

  // All tests completed
  std::cout << "Test suite completed." << std::endl;
  return 0;
}