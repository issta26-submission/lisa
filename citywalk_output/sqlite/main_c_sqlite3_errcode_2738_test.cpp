// Minimal test scaffolding for sqlite3_errcode to run under C++11 without GTest.
// This file provides a lightweight test harness and a small stub of the SQLite-like
// environment necessary to exercise the focal function in main.c.

// Provide a minimal sqlite3 API surface compatible with the focal function.
// We avoid pulling real SQLite headers; instead we supply just what's needed.

#include <iostream>
#include <sqliteInt.h>


// Minimal sqlite3 stub type and constants
typedef struct sqlite3 sqlite3;
struct sqlite3 {
  int mallocFailed;
  int errCode;
  int errMask;
};

// Prototypes of the focal function under test and its dependency
#ifdef __cplusplus
extern "C" {
#endif
int sqlite3_errcode(sqlite3 *db);
int sqlite3SafetyCheckSickOrOk(sqlite3 *db);
#ifdef __cplusplus
}
#endif

// Constants used by sqlite3_errcode (mocked for testing)
#define SQLITE_MISUSE_BKPT 1
#define SQLITE_NOMEM_BKPT 2

// Global hook to control the return value of sqlite3SafetyCheckSickOrOk.
// The actual implementation in the real project is static/complex; for testing
// we provide a controllable override.
static int g_SafetyCheckReturn = 1;
extern "C" int sqlite3SafetyCheckSickOrOk(sqlite3 *db) {
  // Return value can be toggled by tests to exercise true/false branches.
  (void)db; // suppress unused warning in case TESTS don't read db
  return g_SafetyCheckReturn;
}

// Simple test harness
static int tests_run = 0;
static int tests_passed = 0;

static void logTest(const char* name, bool ok) {
  std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << std::endl;
  ++tests_run;
  if (ok) ++tests_passed;
}

// --------------------------------------------------------------------------------------
// Test 1: MISUSE_BKPT path when db is non-null and sqlite3SafetyCheckSickOrOk returns false.
// Expected: sqlite3_errcode should return SQLITE_MISUSE_BKPT.
// --------------------------------------------------------------------------------------
bool testMisusePath() {
  g_SafetyCheckReturn = 0; // simulate sick/unsafe state
  sqlite3 db;
  db.mallocFailed = 0;
  db.errCode = 0x12;
  db.errMask = 0xFF;

  int rc = sqlite3_errcode(&db);
  bool ok = (rc == SQLITE_MISUSE_BKPT);
  logTest("sqlite3_errcode should return SQLITE_MISUSE_BKPT when db is valid and SafetyCheck fails", ok);
  return ok;
}

// --------------------------------------------------------------------------------------
// Test 2: NOMEM_BKPT path when db is null (db == nullptr).
// Expected: sqlite3_errcode should return SQLITE_NOMEM_BKPT.
// --------------------------------------------------------------------------------------
bool testNomemNull() {
  g_SafetyCheckReturn = 1; // doesn't matter since db is null
  sqlite3* db = nullptr;

  int rc = sqlite3_errcode(db);
  bool ok = (rc == SQLITE_NOMEM_BKPT);
  logTest("sqlite3_errcode should return SQLITE_NOMEM_BKPT when db is NULL", ok);
  return ok;
}

// --------------------------------------------------------------------------------------
// Test 3: NOMEM_BKPT path when mallocFailed is set on a non-null db object.
// Expected: sqlite3_errcode should return SQLITE_NOMEM_BKPT.
// --------------------------------------------------------------------------------------
bool testNomemMallocFailed() {
  g_SafetyCheckReturn = 1; // SafetyCheck passes, but malloc failed triggers NOMEM
  sqlite3 db;
  db.mallocFailed = 1;
  db.errCode = 0xAB;
  db.errMask = 0x0F;

  int rc = sqlite3_errcode(&db);
  bool ok = (rc == SQLITE_NOMEM_BKPT);
  logTest("sqlite3_errcode should return SQLITE_NOMEM_BKPT when db mallocFailed is true", ok);
  return ok;
}

// --------------------------------------------------------------------------------------
// Test 4: Success path when db is non-null, SafetyCheck passes, and mallocFailed is false.
// Expected: sqlite3_errcode should return (errCode & errMask).
// --------------------------------------------------------------------------------------
bool testSuccessPath() {
  g_SafetyCheckReturn = 1; // Safety check passes
  sqlite3 db;
  db.mallocFailed = 0;
  db.errCode = 0xAB;
  db.errMask = 0x0F;

  int rc = sqlite3_errcode(&db);
  int expected = db.errCode & db.errMask;
  bool ok = (rc == expected);
  logTest("sqlite3_errcode should return (errCode & errMask) on success path", ok);
  return ok;
}

// --------------------------------------------------------------------------------------
// Test 5: Another success path with different values to ensure bitwise handling.
// --------------------------------------------------------------------------------------
bool testSuccessPathVariant() {
  g_SafetyCheckReturn = 1;
  sqlite3 db;
  db.mallocFailed = 0;
  db.errCode = 0x7A; // 0111 1010
  db.errMask = 0x0F; // 0000 1111
  int rc = sqlite3_errcode(&db);
  int expected = 0x0A; // 0x7A & 0x0F = 0x0A
  bool ok = (rc == expected);
  logTest("sqlite3_errcode success path variant computes correct bitwise AND", ok);
  return ok;
}

// --------------------------------------------------------------------------------------
// Main: execute all tests and print a summary.
// --------------------------------------------------------------------------------------
int main() {
  testMisusePath();
  testNomemNull();
  testNomemMallocFailed();
  testSuccessPath();
  testSuccessPathVariant();

  std::cout << "Tests run: " << tests_run << ", Passed: " << tests_passed
            << ", Failed: " << (tests_run - tests_passed) << std::endl;
  return (tests_run == tests_passed) ? 0 : 1;
}

// Note:
// - The sqlite3_errcode implementation under test resides in main.c and is linked with this test.
// - The sqlite3.h-like stub above provides just enough structure and constants to compile and run
//   the tests in isolation without depending on the full SQLite source tree. The tests cover:
//   * MISUSE path when SafetyCheck fails
//   * NOMEM path for NULL db
//   * NOMEM path when mallocFailed is set
//   * Successful path with bitwise AND of errCode and errMask
// - All test checks are non-terminating and report their results to stdout.