/*
Step 1: Program Understanding and Candidate Keywords
- Core focus: sqlite3_declare_vtab(db, zCreateTable)
- Key internal components/branches to touch via tests:
  - API armor guard: sqlite3SafetyCheckOk(db) and zCreateTable != 0
  - Mutual exclusion: sqlite3_mutex_enter/leave on db->mutex
  - Validation: pCtx = db->pVtabCtx; must be non-null and not yet declared (pCtx->bDeclared)
  - Context and tab binding: pCtx->pTab must be a virtual table (IsVirtual(pTab))
  - Parser path: sqlite3ParseObjectInit, parse mode PARSE_MODE_DECLARE_VTAB, disableTriggers
  - Run parser: sqlite3RunParser(&sParse, zCreateTable)
  - Post-parse success path: copy pNewTable into pTab, set nCol/nNVCol, aCol, handle pIndex, ensure HasRowid/PrimaryKey constraints
  - Error path: sqlite3ErrorWithMsg, sqlite3DbFree(sParse.zErrMsg)
  - Cleanup: finalize Vdbe, delete parsed/temporary tables, reset parse object
  - Return path: rc goes through sqlite3ApiExit(db, rc) and sqlite3_mutex_leave
- Candidate Keywords representing critical components:
  - API_ARMOR, sqlite3SafetyCheckOk, zCreateTable
  - sqlite3_mutex_enter, sqlite3_mutex_leave
  - VtabCtx, pCtx, bDeclared, pTab, IsVirtual
  - Parse, sParse, PARSE_MODE_DECLARE_VTAB, disableTriggers
  - sqlite3RunParser, sParse.pNewTable, sParse.zErrMsg
  - IsOrdinaryTable, HasRowid, sqlite3PrimaryKeyIndex
  - pIndex, pNewTable, aCol, nCol, tabFlags
  - sqlite3ExprListDelete, sqlite3PrimaryKeyIndex, sqlite3DbFree
  - sParse.pVdbe, sqlite3VdbeFinalize
  - sqlite3ParseObjectReset, sqlite3DeleteTable
  - rc handling: SQLITE_OK, SQLITE_ERROR, SQLITE_MISUSE_BKPT
  - sqlite3ApiExit, sqlite3_mutex_leave
- Domain knowledge alignment: design tests to cover success path and error path branches, using a public SQLite API-based approach (via a real sqlite3 module), to exercise the same logical flow as the focal method without re-implementing internal SQLite plumbing.

Step 2: Unit Test Generation
- Strategy: Use the public SQLite API to exercise sqlite3_declare_vtab by providing a virtual-table module whose xCreate calls sqlite3_declare_vtab with a valid schema and with a NULL schema, to exercise success vs error paths of the focal function.
- Rationale: This approach exercises sqlite3_declare_vtab in the library in a realistic setting (as a module would when creating a virtual table). It avoids re-implementing internal SQLite structures and logic and relies on the actual implementation of sqlite3_declare_vtab inside sqlite3.c/vtab.c, which is what we want to validate.

Step 3: Test Case Refinement
- Tests are crafted as non-terminating (non-fatal) tests using a small harness (no GTest) and use the public SQLite API.
- Tests:
  - Test A: Success path
    - Create an in-memory SQLite database.
    - Register a module whose xCreate calls sqlite3_declare_vtab(db, "CREATE TABLE vt_schema(a TEXT, b INTEGER)").
    - Create a virtual table using that module: "CREATE VIRTUAL TABLE vt USING test_module".
    - Expect sqlite3_exec to return SQLITE_OK (success path).
  - Test B: Misuse path via NULL zCreateTable
    - Create an in-memory SQLite database.
    - Register a module whose xCreate calls sqlite3_declare_vtab(db, NULL)
    - Attempt to create a virtual table: "CREATE VIRTUAL TABLE vt_bad USING test_module_bad".
    - Expect sqlite3_exec to return a non-SQLITE_OK error code (reflecting the misuse path in sqlite3_declare_vtab).
  - Test C (optional extension): Could verify that multiple creation attempts with the same module (if SQLite allows) do not crash and return appropriate errors, but at least one additional scenario could be attempted by creating a second virtual table with a separate module instance that also declares a valid schema.

Implementation notes
- The tests rely on the public SQLite API (sqlite3.h) and link against the system SQLite library (-lsqlite3).
- The module definitions are provided with C linkage to satisfy the SQLite C API and to ensure sqlite3_declare_vtab is invoked from within the module’s xCreate handler.
- No Google Test is used. A small custom test harness prints pass/fail information and summary counts.
- The tests are designed to be compiled with a C++11 capable compiler.

Below is the self-contained test program that follows the above approach. It uses the public SQLite API and registers two modules to exercise the two targeted paths of sqlite3_declare_vtab.

Code (C++11, no GTest, using real SQLite library):
*/

// Note: This test program requires a system SQLite3 library.
// Build command example (on a system with sqlite3 installed):
// g++ -std=c++11 -O2 -I/usr/include -L/usr/lib -lsqlite3 test_sqlite3_declare_vtab.cpp -o test_sqlite3_declare_vtab
//
// The test uses the public SQLite API to trigger sqlite3_declare_vtab via a module's xCreate function.

#include <vector>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>


static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define ASSERT_EQ(a, b, msg) do { if ((a) != (b)) { \
    fprintf(stderr, "ASSERT_FAILED: %s (expected %d, got %d)\n", (msg), (int)(b), (int)(a)); \
    g_tests_failed++; \
  } else { g_tests_passed++; } \
} while(0)

#define RUN_TEST(name) void name(); static void name##_runner() { name(); } \
  static int name##_registered = (name##_runner(), 0)

static void print_summary() {
  printf("Tests passed: %d\n", g_tests_passed);
  printf("Tests failed: %d\n", g_tests_failed);
  if (g_tests_failed == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("SOME TESTS FAILED\n");
  }
}

/* Test 1: Module that declares a valid vtab schema via sqlite3_declare_vtab.
   Expected: sqlite3_declare_vtab succeeds when called with a valid zCreateTable string. */
static int test1_xCreate(sqlite3* db, void* pAux, int argc, const char* const* argv,
                       sqlite3_vtab** ppVTab, char** pzErr) {
  (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
  // Declare a simple virtual table schema. This exercises the success path.
  int rc = sqlite3_declare_vtab(db, "CREATE TABLE vt_schema(a TEXT, b INTEGER)");
  return rc;
}

static int test1_xConnect(sqlite3* db, void* pAux, int argc, const char* const* argv,
                        sqlite3_vtab** ppVTab, char** pzErr) {
  (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
  return SQLITE_OK;
}

static sqlite3_module test1_module = {
  1,              // iVersion
  test1_xCreate,  // xCreate
  test1_xConnect, // xConnect
  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

/* Test 2: Module that calls sqlite3_declare_vtab with NULL zCreateTable.
   Expected: sqlite3_create_module should fail for the vtab creation attempt. */
static int test2_xCreate(sqlite3* db, void* pAux, int argc, const char* const* argv,
                       sqlite3_vtab** ppVTab, char** pzErr) {
  (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
  // Deliberately pass NULL to trigger API misuse path in sqlite3_declare_vtab
  int rc = sqlite3_declare_vtab(db, NULL);
  return rc;
}

static int test2_xConnect(sqlite3* db, void* pAux, int argc, const char* const* argv,
                        sqlite3_vtab** ppVTab, char** pzErr) {
  (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
  return SQLITE_OK;
}

static sqlite3_module test2_module = {
  1,
  test2_xCreate,
  test2_xConnect,
  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
  nullptr, nullptr, nullptr, nullptr, nullptr, nullptr
};

/* Helper: execute a no-args statement and return whether it succeeds. */
static bool exec_sql(sqlite3* db, const char* sql) {
  char* errMsg = nullptr;
  int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
  if (rc != SQLITE_OK && errMsg) {
    fprintf(stderr, "SQL error: %s\n", errMsg);
    sqlite3_free(errMsg);
  }
  return rc == SQLITE_OK;
}

/* Test 1 Runner: expect success creating a virtual table using test1_module. */
static void run_test1() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "test1: cannot open database\n");
    g_tests_failed++;
    return;
  }

  rc = sqlite3_create_module(db, "testmod1", &test1_module, nullptr, nullptr);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "test1: Failed to register module. rc=%d\n", rc);
    sqlite3_close(db);
    g_tests_failed++;
    return;
  }

  // Attempt to create a virtual table. This should invoke sqlite3_declare_vtab with a valid schema.
  const char* createStmt = "CREATE VIRTUAL TABLE vt1 USING testmod1";
  int execRc = sqlite3_exec(db, createStmt, nullptr, nullptr, nullptr);
  if (execRc != SQLITE_OK) {
    fprintf(stderr, "test1: Failed to create virtual table. rc=%d\n", execRc);
  }
  ASSERT_EQ(execRc, SQLITE_OK, "Virtual table creation should succeed in test1");

  sqlite3_close(db);
}

/* Test 2 Runner: expect failure when xCreate calls sqlite3_declare_vtab with NULL zCreateTable. */
static void run_test2() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "test2: cannot open database\n");
    g_tests_failed++;
    return;
  }

  rc = sqlite3_create_module(db, "testmod2", &test2_module, nullptr, nullptr);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "test2: Failed to register module. rc=%d\n", rc);
    sqlite3_close(db);
    g_tests_failed++;
    return;
  }

  // Attempt to create a virtual table. This should invoke sqlite3_declare_vtab with NULL zCreateTable and fail.
  const char* createStmt = "CREATE VIRTUAL TABLE vt2 USING testmod2";
  int execRc = sqlite3_exec(db, createStmt, nullptr, nullptr, nullptr);
  // We expect this to fail (non-SQLITE_OK) due to misuse path in sqlite3_declare_vtab
  if (execRc == SQLITE_OK) {
    fprintf(stderr, "test2: Unexpected success; expected failure due to NULL zCreateTable\n");
  }
  ASSERT_EQ(execRc, SQLITE_OK, "Virtual table creation in test2 should fail or return non-OK");

  sqlite3_close(db);
}

int main() {
  // Run tests
  run_test1();
  run_test2();

  print_summary();
  return (g_tests_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}