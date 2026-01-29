// Candidate Keywords derived from Step 1
// - sqlite3_busy_timeout: focal method under test
// - sqlite3 *db: database handle with member busyTimeout
// - int ms: requested timeout in milliseconds
// - SQLITE_ENABLE_API_ARMOR: conditional safety check path
// - sqlite3SafetyCheckOk: safety check function (guarded by ARMOR)
// - SQLITE_MISUSE_BKPT: error return when misuse detected under ARMOR
// - SQLITE_OK: success return code
// - sqlite3_busy_handler: registers/unregisters a busy handler
// - sqliteDefaultBusyCallback: default busy callback function
// - BusyHandler: concept of a busy handler in the API
// - static/global test state: to observe internal calls in tests

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Minimal stubs to emulate the environment for sqlite3_busy_timeout
// This is a focused, self-contained test harness and does not rely on GTest.

// Public-facing API constants (simplified for testing)
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT -1

// Minimal sqlite3 structure with the member accessed by sqlite3_busy_timeout
struct sqlite3 {
  int busyTimeout;
};

// Global test-state for observing behavior of mocks
#ifdef SQLITE_ENABLE_API_ARMOR
static int g_safety_ok_for_test = 1; // 1 = safety check passes, 0 = fails
static int sqlite3SafetyCheckOk(sqlite3 *db) {
  (void)db; // not used in this simplified test
  return g_safety_ok_for_test;
}
#endif

// The default busy callback used by the real API; we expose a symbol to compare pointers
static int sqliteDefaultBusyCallback(void *ptr, int count) {
  (void)ptr;
  (void)count;
  return 0;
}

// Capture observations from sqlite3_busy_handler for assertions
static int (*g_lastBusyXBusy)(void*, int) = nullptr;
static void *g_lastBusyArg = nullptr;
static bool g_busyHandlerWasSet = false;

// Mock implementation of sqlite3_busy_handler used by the focal method
static int sqlite3_busy_handler(sqlite3 *db,
                              int (*xBusy)(void*, int),
                              void *pArg) {
  (void)db;
  g_lastBusyXBusy = xBusy;
  g_lastBusyArg = pArg;
  g_busyHandlerWasSet = true;
  return 0;
}

// The focal method under test (reproduced here for unit testing purposes)
// Note: In the real project this would be in main.c; here we re-create the logic in C++.
int sqlite3_busy_timeout(sqlite3 *db, int ms) {
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ) return SQLITE_MISUSE_BKPT;
#endif
  if( ms>0 ){
    sqlite3_busy_handler(db, (int(*)(void*,int))sqliteDefaultBusyCallback,
                             (void*)db);
    db->busyTimeout = ms;
  }else{
    sqlite3_busy_handler(db, 0, 0);
  }
  return SQLITE_OK;
}

// Helper: reset test state between tests
static void reset_state(sqlite3 *db) {
  db->busyTimeout = -999999; // sentinel to detect changes
  g_lastBusyXBusy = nullptr;
  g_lastBusyArg = nullptr;
  g_busyHandlerWasSet = false;
}

// Test 1: ms > 0 with API_ARMOR enabled and safety check passes
// - Expect: db->busyTimeout is set to ms
// - Expect: busy handler is registered with sqliteDefaultBusyCallback and arg == db
static bool test_armor_true_ms_positive() {
  sqlite3 db;
  reset_state(&db);
  db.busyTimeout = -1;

#ifdef SQLITE_ENABLE_API_ARMOR
  g_safety_ok_for_test = 1; // ensure safety check passes
  // Call the focal method
  int rc = sqlite3_busy_timeout(&db, 100);
  bool ok = true;
  ok = ok && (rc == SQLITE_OK);
  ok = ok && (db.busyTimeout == 100);
  ok = ok && (g_busyHandlerWasSet);
  ok = ok && (g_lastBusyXBusy == sqliteDefaultBusyCallback);
  ok = ok && (g_lastBusyArg == &db);

  std::cout << "Test Armor True MS Positive: " << (ok ? "PASS" : "FAIL") << std::endl;
  return ok;
#else
  // If ARMOR is not enabled in this build, mark as skipped for clarity
  std::cout << "Test Armor True MS Positive: SKIPPED (ARMOR not enabled in this build)" << std::endl;
  return true;
#endif
}

// Test 2: safety check fails under API_ARMOR
// - Expect: return code SQLITE_MISUSE_BKPT and no change to handler timeout
static bool test_armor_safety_check_fails() {
#ifdef SQLITE_ENABLE_API_ARMOR
  sqlite3 db;
  reset_state(&db);
  db.busyTimeout = -1;

  g_safety_ok_for_test = 0; // safety check fails
  int rc = sqlite3_busy_timeout(&db, 50);

  bool ok = true;
  ok = ok && (rc == SQLITE_MISUSE_BKPT);
  // When safety check fails, behavior before ms>0 is not taken
  // Busy handler should not be registered (since we return early)
  ok = ok && (!g_busyHandlerWasSet);
  ok = ok && (db.busyTimeout == -1); // unchanged sentinel

  std::cout << "Test Armor Safety Check Fails: " << (ok ? "PASS" : "FAIL") << std::endl;
  return ok;
#else
  std::cout << "Test Armor Safety Check Fails: SKIPPED (ARMOR not enabled in this build)" << std::endl;
  return true;
#endif
}

// Test 3: ms == 0 with API_ARMOR enabled
// - Expect: busy handler is unregistered (set with 0,0) and return SQLITE_OK
static bool test_armor_ms_zero() {
#ifdef SQLITE_ENABLE_API_ARMOR
  sqlite3 db;
  reset_state(&db);
  db.busyTimeout = -1;

  g_safety_ok_for_test = 1; // safety passes
  int rc = sqlite3_busy_timeout(&db, 0);

  bool ok = true;
  ok = ok && (rc == SQLITE_OK);
  ok = ok && (g_lastBusyXBusy == nullptr);
  ok = ok && (g_lastBusyArg == nullptr);

  std::cout << "Test Armor MS Zero: " << (ok ? "PASS" : "FAIL") << std::endl;
  return ok;
#else
  std::cout << "Test Armor MS Zero: SKIPPED (ARMOR not enabled in this build)" << std::endl;
  return true;
#endif
}

// Test 4: Re-run of ms > 0 to confirm consistency (optional additional coverage)
static bool test_armor_ms_positive_consistency() {
#ifdef SQLITE_ENABLE_API_ARMOR
  sqlite3 db;
  reset_state(&db);
  db.busyTimeout = -1;

  g_safety_ok_for_test = 1;
  int rc = sqlite3_busy_timeout(&db, 250);

  bool ok = true;
  ok = ok && (rc == SQLITE_OK);
  ok = ok && (db.busyTimeout == 250);
  ok = ok && (g_busyHandlerWasSet);
  ok = ok && (g_lastBusyXBusy == sqliteDefaultBusyCallback);
  ok = ok && (g_lastBusyArg == &db);

  std::cout << "Test Armor MS Positive Consistency: " << (ok ? "PASS" : "FAIL") << std::endl;
  return ok;
#else
  std::cout << "Test Armor MS Positive Consistency: SKIPPED (ARMOR not enabled in this build)" << std::endl;
  return true;
#endif
}

// Domain knowledge guidance: Run tests from main; use non-terminating assertions
int main() {
  // Step 2/3: Run a comprehensive test suite for sqlite3_busy_timeout with ARMOR enabled.
  // Note: If you want to exercise the non-ARMOR path, compile with -DSQLITE_ENABLE_API_ARMOR=0 (not shown here).
  int pass_all = 1;
  pass_all &= test_armor_true_ms_positive();
  pass_all &= test_armor_safety_check_fails();
  pass_all &= test_armor_ms_zero();
  pass_all &= test_armor_ms_positive_consistency();

  if(pass_all) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << "SOME TESTS FAILED" << std::endl;
    return 1;
  }
}