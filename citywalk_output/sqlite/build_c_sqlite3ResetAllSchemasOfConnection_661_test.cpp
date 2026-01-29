// Unit test suite for sqlite3ResetAllSchemasOfConnection
// - This test suite is written in C++11 without GoogleTest.
// - It uses lightweight, self-contained mocks for the dependent SQLite internals.
// - The goal is to validate both true/false branches of key predicates within
//   sqlite3ResetAllSchemasOfConnection as exercised by <FOCAL_METHOD>.
// - All tests run from main() and use non-terminating assertions to maximize coverage.

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// -----------------------------------------------------------------------------
// Minimal type definitions to model the subset of sqlite3 and Db used by the
// focal function. These are intentionally lightweight and tailored for testing.
// -----------------------------------------------------------------------------

// Forward declaration for C-style linkage compatibility with the focal function.
extern "C" {
  // Mocked dependency functions used by sqlite3ResetAllSchemasOfConnection
  void sqlite3BtreeEnterAll(struct sqlite3 *db);
  void sqlite3BtreeLeaveAll(struct sqlite3 *db);
  void sqlite3VtabUnlockList(struct sqlite3 *db);
  void sqlite3CollapseDatabaseArray(struct sqlite3 *db);
  void sqlite3SchemaClear(void *pSchema);
  void DbSetProperty(struct sqlite3 *db, int iDb, int mask);
}

// Minimal representation of Db and sqlite3 structures sufficient for tests.
typedef struct Db {
  void *pSchema; // pointer to a schema object; nullptr if no schema
} Db;

typedef struct sqlite3 {
  int nDb;            // number of databases in the array aDb
  Db *aDb;            // array of Db
  int nSchemaLock;    // if 0 => clear schemas, else request reset
  int mDbFlags;        // bit flags (we care about two bits for test)
} sqlite3;

// Bitmasks used by the focal function
static const int DB_ResetWanted = 1;
static const int DBFLAG_SchemaChange   = 1 << 0;
static const int DBFLAG_SchemaKnownOk  = 1 << 1;

// -----------------------------------------------------------------------------
// Test harness: lightweight, deterministic mocks to observe calls
// -----------------------------------------------------------------------------
// Global counters to observe how the focal method interacts with its dependencies.
static int g_btreeEnterAll_calls = 0;
static int g_btreeLeaveAll_calls = 0;
static int g_vtabUnlockList_calls = 0;
static int g_collapseDatabaseArray_calls = 0;
static int g_schemaClear_calls = 0;     // counts successful SchemaClear invocations
static int g_dbSetProperty_calls = 0;   // counts DbSetProperty invocations

// Per-test helper to reset counters
static void reset_call_logs() {
  g_btreeEnterAll_calls = 0;
  g_btreeLeaveAll_calls = 0;
  g_vtabUnlockList_calls = 0;
  g_collapseDatabaseArray_calls = 0;
  g_schemaClear_calls = 0;
  g_dbSetProperty_calls = 0;
}

// Mocks implementing the dependencies called by sqlite3ResetAllSchemasOfConnection

extern "C" {

// sqlite3BtreeEnterAll: record the call
void sqlite3BtreeEnterAll(struct sqlite3 *db) {
  (void)db;
  ++g_btreeEnterAll_calls;
}

// sqlite3BtreeLeaveAll: record the call
void sqlite3BtreeLeaveAll(struct sqlite3 *db) {
  (void)db;
  ++g_btreeLeaveAll_calls;
}

// sqlite3VtabUnlockList: record the call
void sqlite3VtabUnlockList(struct sqlite3 *db) {
  (void)db;
  ++g_vtabUnlockList_calls;
}

// sqlite3CollapseDatabaseArray: record the call
void sqlite3CollapseDatabaseArray(struct sqlite3 *db) {
  (void)db;
  ++g_collapseDatabaseArray_calls;
}

// sqlite3SchemaClear: simulate clearing a schema; count only non-null pSchema cases
void sqlite3SchemaClear(void *pSchema) {
  (void)pSchema;
  // Count only when a real schema pointer is provided
  if (pSchema != NULL) {
    ++g_schemaClear_calls;
  }
}

// DbSetProperty: simulate requesting a reset of a specific schema
void DbSetProperty(struct sqlite3 *db, int iDb, int mask) {
  (void)db;
  (void)iDb;
  (void)mask;
  // In tests we only need to know that this function was invoked.
  ++g_dbSetProperty_calls;
}
} // extern "C"

// -----------------------------------------------------------------------------
// Helper utilities to construct sqlite3 and Db instances for tests
// -----------------------------------------------------------------------------

// Create a sqlite3 instance with nDb databases. Each database i has pSchema[i].
// hasSchema[i] indicates whether a non-null schema pointer is assigned.
static sqlite3* create_db(int nDb, const bool* hasSchema) {
  sqlite3* db = new sqlite3();
  db->nDb = nDb;
  db->nSchemaLock = 0;
  db->mDbFlags = DBFLAG_SchemaChange | DBFLAG_SchemaKnownOk; // initial flags
  if (nDb > 0) {
    db->aDb = new Db[nDb];
    for (int i = 0; i < nDb; ++i) {
      if (hasSchema && hasSchema[i]) {
        db->aDb[i].pSchema = reinterpret_cast<void*>(0x1); // non-null dummy
      } else {
        db->aDb[i].pSchema = nullptr;
      }
    }
  } else {
    db->aDb = nullptr;
  }
  return db;
}

static void destroy_db(sqlite3* db) {
  if (db) {
    delete[] db->aDb;
    delete db;
  }
}

// Simple test naming and assertion helpers (non-terminating)
static std::string currentTestName;
static std::vector<std::string> g_failure_messages;

#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    g_failure_messages.push_back(currentTestName + " - EXPECT_TRUE failed: " + (msg)); \
  } \
} while(0)

#define REQUIRE_TRUE(cond, msg) do { \
  if(!(cond)) { \
    g_failure_messages.push_back(currentTestName + " - REQUIRE_TRUE failed: " + (msg)); \
  } \
} while(0)

// A helper to run a test and print results
static void report_test_result(bool passed) {
  if (passed) {
    std::cout << "[ PASS ] " << currentTestName << std::endl;
  } else {
    std::cout << "[ FAIL ] " << currentTestName << std::endl;
  }
}

// -----------------------------------------------------------------------------
// Prototypes for the focal method (to be linked with the actual implementation
// in the project under test). We declare the function here so tests can call it.
// -----------------------------------------------------------------------------

extern "C" void sqlite3ResetAllSchemasOfConnection(sqlite3 *db);

// -----------------------------------------------------------------------------
// Test cases
// -----------------------------------------------------------------------------

// Test 1: When nSchemaLock == 0, clear all schemas with non-null pSchema and
// verify that seed calls are made appropriately and flags are cleared.
// Expected:
// - sqlite3BtreeEnterAll called once
// - sqlite3BtreeLeaveAll called once
// - sqlite3VtabUnlockList called once
// - sqlite3CollapseDatabaseArray called once
// - sqlite3SchemaClear called for each non-null pSchema (e.g., 2)
// - DbSetProperty not called
// - db->mDbFlags has the schema-change bits cleared
static void test_ClearSchemas_nSchemaLockZero() {
  reset_call_logs();
  currentTestName = "ClearSchemas_nSchemaLockZero";

  // Setup: 3 databases, two with schemas
  const int nDb = 3;
  bool hasSchema[3] = { true, false, true };
  sqlite3* db = create_db(nDb, hasSchema);
  db->nSchemaLock = 0; // trigger SchemaClear path

  // Call the focal function (the actual implementation is expected to be linked)
  sqlite3ResetAllSchemasOfConnection(db);

  // Validate expectations
  bool passed = true;
  EXPECT_TRUE(g_btreeEnterAll_calls == 1, "sqlite3BtreeEnterAll should be called once");
  EXPECT_TRUE(g_btreeLeaveAll_calls == 1, "sqlite3BtreeLeaveAll should be called once");
  EXPECT_TRUE(g_vtabUnlockList_calls == 1, "sqlite3VtabUnlockList should be called once");
  EXPECT_TRUE(g_collapseDatabaseArray_calls == 1, "sqlite3CollapseDatabaseArray should be called once");
  EXPECT_TRUE(g_schemaClear_calls == 2, "sqlite3SchemaClear should be invoked for each non-null pSchema (2)");
  EXPECT_TRUE(g_dbSetProperty_calls == 0, "DbSetProperty should not be invoked when nSchemaLock == 0");
  // Flags should be cleared of the two schema-related bits
  EXPECT_TRUE((db->mDbFlags & (DBFLAG_SchemaChange | DBFLAG_SchemaKnownOk)) == 0,
              "db->mDbFlags bits SchemaChange/SchemaKnownOk should be cleared");

  destroy_db(db);

  // Report
  report_test_result(passed);
}

// Test 2: When nSchemaLock > 0, do not clear schemas; instead, request reset via DbSetProperty.
// Expected:
// - sqlite3BtreeEnterAll / sqlite3BtreeLeaveAll / sqlite3VtabUnlockList called
// - sqlite3SchemaClear not called
// - DbSetProperty called for each non-null pSchema (once per non-null)
static void test_ResetRequested_when_nSchemaLockNonZero() {
  reset_call_logs();
  currentTestName = "ResetRequested_when_nSchemaLockNonZero";

  // Setup: 3 databases, two with schemas
  const int nDb = 3;
  bool hasSchema[3] = { true, true, false };
  sqlite3* db = create_db(nDb, hasSchema);
  db->nSchemaLock = 1; // trigger DbSetProperty path

  sqlite3ResetAllSchemasOfConnection(db);

  bool passed = true;
  EXPECT_TRUE(g_btreeEnterAll_calls == 1, "sqlite3BtreeEnterAll should be called once");
  EXPECT_TRUE(g_btreeLeaveAll_calls == 1, "sqlite3BtreeLeaveAll should be called once");
  EXPECT_TRUE(g_vtabUnlockList_calls == 1, "sqlite3VtabUnlockList should be called once");
  EXPECT_TRUE(g_collapseDatabaseArray_calls == 0, "sqlite3CollapseDatabaseArray should NOT be called when nSchemaLock != 0");
  EXPECT_TRUE(g_schemaClear_calls == 0, "sqlite3SchemaClear should NOT be invoked when nSchemaLock != 0");
  EXPECT_TRUE(g_dbSetProperty_calls == 2, "DbSetProperty should be invoked for each non-null pSchema (2)");
  EXPECT_TRUE((db->mDbFlags & (DBFLAG_SchemaChange | DBFLAG_SchemaKnownOk)) == 0,
              "db->mDbFlags bits SchemaChange/SchemaKnownOk should be cleared");

  destroy_db(db);
  report_test_result(passed);
}

// Test 3: No schemas present (all pSchema == nullptr). Validate no clearing and no DbSetProperty calls.
// Expected both branches to be no-ops, but still perform enter/leave and unlock, and potentially collapse when nSchemaLock == 0.
static void test_NoSchemas_present() {
  reset_call_logs();
  currentTestName = "NoSchemas_present";

  const int nDb = 2;
  bool hasSchema[2] = { false, false };
  sqlite3* db = create_db(nDb, hasSchema);
  db->nSchemaLock = 0; // path with SchemaClear would not clear anything due to null pointers

  sqlite3ResetAllSchemasOfConnection(db);

  bool passed = true;
  EXPECT_TRUE(g_btreeEnterAll_calls == 1, "enter/leave should still occur");
  EXPECT_TRUE(g_btreeLeaveAll_calls == 1, "enter/leave should still occur");
  EXPECT_TRUE(g_vtabUnlockList_calls == 1, "unlock list should still occur");
  // CollapseDatabaseArray should be called since nSchemaLock == 0
  EXPECT_TRUE(g_collapseDatabaseArray_calls == 1, "collapse should be called when nSchemaLock == 0");
  EXPECT_TRUE(g_schemaClear_calls == 0, "no SchemaClear should occur when no schemas present");
  EXPECT_TRUE(g_dbSetProperty_calls == 0, "DbSetProperty should not be invoked when no schemas present");
  EXPECT_TRUE((db->mDbFlags & (DBFLAG_SchemaChange | DBFLAG_SchemaKnownOk)) == 0,
              "db->mDbFlags bits SchemaChange/SchemaKnownOk should be cleared");

  destroy_db(db);
  report_test_result(passed);
}

// Test 4: Edge case with zero databases (nDb == 0). Validate that the function handles empty array gracefully.
// Expected: enters/leaves, unlocks, and collapses (since nSchemaLock == 0) but does not touch any schemas.
static void test_ZeroDatabases() {
  reset_call_logs();
  currentTestName = "ZeroDatabases";

  sqlite3* db = create_db(0, nullptr);
  db->nSchemaLock = 0;
  sqlite3ResetAllSchemasOfConnection(db);

  bool passed = true;
  EXPECT_TRUE(g_btreeEnterAll_calls == 1, "enter should be called even if nDb == 0");
  EXPECT_TRUE(g_btreeLeaveAll_calls == 1, "leave should be called even if nDb == 0");
  EXPECT_TRUE(g_vtabUnlockList_calls == 1, "unlock list should be called even if nDb == 0");
  EXPECT_TRUE(g_collapseDatabaseArray_calls == 1, "collapse should be called when nSchemaLock == 0, even if nDb == 0");
  EXPECT_TRUE(g_schemaClear_calls == 0, "no SchemaClear should be invoked with no schemas");
  EXPECT_TRUE(g_dbSetProperty_calls == 0, "no DbSetProperty should be invoked with no schemas");
  EXPECT_TRUE((db->mDbFlags & (DBFLAG_SchemaChange | DBFLAG_SchemaKnownOk)) == 0,
              "db->mDbFlags bits SchemaChange/SchemaKnownOk should be cleared");

  destroy_db(db);
  report_test_result(passed);
}

// -----------------------------------------------------------------------------
// Main entry: run all tests
// -----------------------------------------------------------------------------

int main() {
  // Run tests
  test_ClearSchemas_nSchemaLockZero();
  test_ResetRequested_when_nSchemaLockNonZero();
  test_NoSchemas_present();
  test_ZeroDatabases();

  // Summary
  if (!g_failure_messages.empty()) {
    std::cout << "\nTest Summary: " << g_failure_messages.size() << " failure(s) detected.\n";
    for (const auto &s : g_failure_messages) {
      std::cout << " - " << s << "\n";
    }
    return 2; // indicate failures
  } else {
    std::cout << "\nAll tests completed. No failures detected.\n";
    return 0;
  }
}

// Note:
// - This test harness relies on the focal function's implementation and its internal
//   dependencies being linked in the test build. The mocked functions above replace
//   the real SQLite internals with deterministic counters suitable for unit testing.
// - The tests focus on verifying true/false branches of the key predicates and
//   ensuring proper side-effects on flags and calls.
// - If the test environment uses a larger build system, you may need to adapt
//   linkage (e.g., marking mocks as weak or using a dedicated test build).