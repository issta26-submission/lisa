// SQLite NextStmt Unit Test Suite (C++11, no GoogleTest)
// This test suite focuses on sqlite3_next_stmt behavior as implemented in the
// provided FOCAL_METHOD. We create a minimal, self-contained mock environment to
// exercise true/false branches of the internal predicate and the two input-paths
// (pStmt == 0 and pStmt != 0). The tests are designed to be non-terminating and
// report pass/fail via stdout for easy integration in environments without GTest.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <opcodes.h>


// Domain/Abstraction: provide a minimal set of types and functions needed by
// sqlite3_next_stmt. We mirror just enough of the SQLite internal API for testing.

#define SQLITE_ENABLE_API_ARMOR 1
#define SQLITE_MISUSE_BKPT 0

// Forward declare to enable flexible memory layout for sqlite3_stmt and Vdbe
struct sqlite3_stmt;
struct Vdbe;

// Vdbe is the internal structure that stores pVNext, which links to the next statement.
// To allow sqlite3_next_stmt to cast a sqlite3_stmt* to Vdbe*, we arrange the
// memory layout so that Vdbe is the first member of sqlite3_stmt.
struct Vdbe {
  sqlite3_stmt *pVNext;
};

// sqlite3_stmt is defined as containing a Vdbe as its first member.
struct sqlite3_stmt {
  Vdbe v;
};

// Minimal sqlite3 structure used by sqlite3_next_stmt
struct sqlite3 {
  void *mutex;          // dummy mutex handle (no real threading in tests)
  sqlite3_stmt *pVdbe;    // pointer to the head (first) Vdbe/stmt
};

// Global test instrumentation and stubs for the required API

// Safety check flag used to simulate SQLITE_ENABLE_API_ARMOR behavior
static int g_safety_ok = 1;

// Mutex call counters to verify proper mutex usage
static int g_mutex_enter_calls = 0;
static int g_mutex_leave_calls = 0;

// Stub implementations to mimic the SQLite internal API (no real locking)
static inline int sqlite3SafetyCheckOk(sqlite3 *p) {
  return g_safety_ok;
}
static inline void sqlite3_mutex_enter(void *p) {
  (void)p; // unused
  g_mutex_enter_calls++;
}
static inline void sqlite3_mutex_leave(void *p) {
  (void)p; // unused
  g_mutex_leave_calls++;
}

// The actual function under test. Implemented here exactly as in the focal method,
// including the API_ARMOR path, for a faithful test.
static inline sqlite3_stmt* sqlite3_next_stmt(sqlite3 *pDb, sqlite3_stmt *pStmt) {
  sqlite3_stmt *pNext;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(pDb) ){
    (void)SQLITE_MISUSE_BKPT;
    return 0;
  }
#endif
  sqlite3_mutex_enter(pDb->mutex);
  if( pStmt==0 ){
    pNext = (sqlite3_stmt*)pDb->pVdbe;
  }else{
    pNext = (sqlite3_stmt*)((Vdbe*)pStmt)->pVNext;
  }
  sqlite3_mutex_leave(pDb->mutex);
  return pNext;
}

// Helper: reset global test state
static void reset_test_state() {
  g_safety_ok = 1;
  g_mutex_enter_calls = 0;
  g_mutex_leave_calls = 0;
}

// Lightweight test harness (non-terminating, prints per-test result)
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void log_result(const char* test_name, bool passed, const char* detail = "") {
  g_total_tests++;
  if (passed) {
    ++g_passed_tests;
    std::cout << "[PASS] " << test_name << "\n";
  } else {
    std::cout << "[FAIL] " << test_name;
    if (detail && detail[0] != '\0') {
      std::cout << " - " << detail;
    }
    std::cout << "\n";
  }
}

// Test 1: Armor enabled, safety ok, pStmt == 0
static void test_next_stmt_arm_ok_pstmt_null() {
  // Setup
  reset_test_state();
  sqlite3 db;
  static sqlite3_stmt pdb_vdbe;
  pdb_vdbe.v.pVNext = nullptr;
  pdb_vdbe.v.pVNext = (sqlite3_stmt*)nullptr;
  db.mutex = (void*)nullptr;
  db.pVdbe = &pdb_vdbe;
  g_safety_ok = 1;

  // Expected: returns the first statement in the DB (pVdbe)
  sqlite3_stmt *ret = sqlite3_next_stmt(&db, nullptr);

  bool passed = (ret == &pdb_vdbe) && (g_mutex_enter_calls == 1) && (g_mutex_leave_calls == 1);
  log_result("test_next_stmt_arm_ok_pstmt_null", passed,
             "Expected return of pDb->pVdbe when pStmt==0 and safety ok; mutex entered/leaved exactly once.");
}

// Test 2: Armor enabled, safety fail, pStmt == 0
static void test_next_stmt_arm_fail_pstmt_null() {
  reset_test_state();
  sqlite3 db;
  db.mutex = (void*)nullptr;
  static sqlite3_stmt pdb_vdbe;
  pdb_vdbe.v.pVNext = nullptr;
  db.pVdbe = &pdb_vdbe;
  g_safety_ok = 0; // trigger misuse path

  sqlite3_stmt *ret = sqlite3_next_stmt(&db, nullptr);

  bool passed = (ret == nullptr) && (g_mutex_enter_calls == 0) && (g_mutex_leave_calls == 0);
  log_result("test_next_stmt_arm_fail_pstmt_null", passed,
             "Expected NULL return and no mutex activity when safety check fails.");
}

// Test 3: Armor enabled, safety ok, pStmt != 0 -> uses pStmt->v.pVNext
static void test_next_stmt_arm_ok_pstmt_nonnull() {
  reset_test_state();
  sqlite3 db;
  db.mutex = (void*)nullptr;
  static sqlite3_stmt pdb_vdbe_main;
  pdb_vdbe_main.v.pVNext = nullptr;

  // Next statement to be returned
  static sqlite3_stmt pdb_next;
  pdb_next.v.pVNext = nullptr;

  // Prepare current statement with pVNext pointing to pdb_next
  sqlite3_stmt cur_stmt;
  cur_stmt.v.pVNext = &pdb_next;

  db.pVdbe = &pdb_vdbe_main;
  g_safety_ok = 1;

  sqlite3_stmt *ret = sqlite3_next_stmt(&db, &cur_stmt);

  bool passed = (ret == &pdb_next) && (g_mutex_enter_calls == 1) && (g_mutex_leave_calls == 1);
  log_result("test_next_stmt_arm_ok_pstmt_nonnull", passed,
             "Expected to return pStmt->v.pVNext when pStmt != 0 and safety ok; mutex usage verified.");
}

// Test 4: Armor enabled, safety ok, pStmt != 0 and pVNext is NULL -> returns NULL
static void test_next_stmt_arm_ok_pstmt_nonnull_nullnext() {
  reset_test_state();
  sqlite3 db;
  db.mutex = (void*)nullptr;
  static sqlite3_stmt cur_stmt;
  cur_stmt.v.pVNext = nullptr;

  sqlite3_stmt pdb_vdbe;
  pdb_vdbe.v.pVNext = nullptr;
  db.pVdbe = &pdb_vdbe;
  g_safety_ok = 1;

  sqlite3_stmt *ret = sqlite3_next_stmt(&db, &cur_stmt);

  bool passed = (ret == nullptr) && (g_mutex_enter_calls == 1) && (g_mutex_leave_calls == 1);
  log_result("test_next_stmt_arm_ok_pstmt_nonnull_nullnext", passed,
             "Expected NULL when pStmt->pVNext is NULL; mutex usage verified.");
}

// Main entry: run all tests
int main() {
  // Run tests
  test_next_stmt_arm_ok_pstmt_null();
  test_next_stmt_arm_fail_pstmt_null();
  test_next_stmt_arm_ok_pstmt_nonnull();
  test_next_stmt_arm_ok_pstmt_nonnull_nullnext();

  // Summary
  std::cout << "\nTest suite completed: " << g_passed_tests << " / " << g_total_tests << " tests passed.\n";
  return (g_total_tests == g_passed_tests) ? 0 : 1;
}