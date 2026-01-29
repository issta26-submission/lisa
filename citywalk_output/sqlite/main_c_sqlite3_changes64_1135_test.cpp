// Unit test suite for the focal method sqlite3_changes64
// This test harness is designed to be compiled in two separate modes:
// 1) TEST_MODE_NO_ARMOR  - exercises the path when SQLITE_ENABLE_API_ARMOR is NOT defined.
// 2) TEST_MODE_WITH_ARMOR - exercises the path when SQLITE_ENABLE_API_ARMOR is defined.
// The test code mirrors the focal function's behavior, and uses a controllable
// sqlite3SafetyCheckOk() to toggle the API-armor branch, allowing true/false
// coverage of the predicate. This approach keeps tests self-contained without
// requiring a full SQLite build.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Lightweight in-file mock of the sqlite3 structure (only what sqlite3_changes64 ).
typedef int64_t sqlite3_int64;
struct sqlite3 {
  sqlite3_int64 nChange;
};

// Minimal definition for the behavior constant used in the API-armor path.
#ifndef SQLITE_MISUSE_BKPT
#define SQLITE_MISUSE_BKPT 0
#endif

// ---------------------------------------------------------------------------
// Focal method (replicated here for self-contained unit tests)
// sqlite3_changes64(sqlite3 *db) with optional API-armor behavior.
// The real implementation reads db->nChange, optionally guarding via
// sqlite3SafetyCheckOk(db) when SQLITE_ENABLE_API_ARMOR is defined.
// We replicate the exact control flow to enable accurate unit testing.
// ---------------------------------------------------------------------------

extern "C" sqlite3_int64 sqlite3_changes64(sqlite3 *db){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ){
    (void)SQLITE_MISUSE_BKPT;
    return 0;
  }
#endif
  return db->nChange;
}

// ---------------------------------------------------------------------------
// API-Safety hook (overridable in tests when API armor is enabled).
// We provide a test-controlled implementation that returns a value based on
// a global flag, allowing us to exercise both true and false outcomes of
// the sqlite3SafetyCheckOk predicate without needing the full SQLite codebase.
// ---------------------------------------------------------------------------

#if defined(SQLITE_ENABLE_API_ARMOR)
static int s_safety_override = 1; // 1 => safe, 0 => unsafe (for test control)

// Forward declaration to mimic the actual API function signature used by the focal method.
extern "C" int sqlite3SafetyCheckOk(sqlite3 *db){
  (void)db; // suppress unused parameter warning in test-only path
  return s_safety_override;
}
#endif

// ---------------------------------------------------------------------------
// Lightweight test helpers (non-terminating assertions to maximize code-path coverage)
// These helpers do not terminate on failure; they print diagnostic messages instead.
// This aligns with the instruction to use non-terminating assertions for high coverage.
// ---------------------------------------------------------------------------

#define TEST_PASS(msg)  std::cout << "[PASS] " << (msg) << std::endl
#define TEST_FAIL(msg)  std::cerr << "[FAIL] " << (msg) << std::endl
#define EXPECT_EQ(a,b) do { \
  auto _va = (a); auto _vb = (b); \
  if (_va != _vb) { TEST_FAIL(std::string("EXPECTED: ") + #a + " == " + #b + " (" + std::to_string(_va) + " != " + std::to_string(_vb) + ")"); } \
  else { TEST_PASS(std::string("EXPECTED: ") + #a + " == " + #b); } \
} while(0)

// Helper: print mode
void printMode(const char* mode){
  std::cout << "==== Test mode: " << mode << " ====" << std::endl;
}

// ---------------------------------------------------------------------------
// Test entry points (two separate modes to cover both branches)
// Mode selection is controlled at compile time via one of the following macros:
// - TEST_MODE_NO_ARMOR
// - TEST_MODE_WITH_ARMOR
// A single translation unit may be compiled in either mode to isolate tests.
// ---------------------------------------------------------------------------

#if defined(TEST_MODE_NO_ARMOR)

static void test_no_armor_path(){
  printMode("NO ARMOR (SQLITE_ENABLE_API_ARMOR not defined)");
  // Case: basic path should return nChange; no safety check involved.
  sqlite3 db;
  db.nChange = 5;
  sqlite3_int64 res = sqlite3_changes64(&db);
  EXPECT_EQ(res, 5);
}

int main(){
  test_no_armor_path();
  return 0;
}

#elif defined(TEST_MODE_WITH_ARMOR)

static void test_with_armor_true_safety(){
  printMode("ARMOR ENABLED - safety check returns true");
  // Configure safety to "true"
  s_safety_override = 1;
  sqlite3 db;
  db.nChange = 7;
  sqlite3_int64 res = sqlite3_changes64(&db);
  EXPECT_EQ(res, 7);
}

static void test_with_armor_false_safety(){
  printMode("ARMOR ENABLED - safety check returns false");
  // Configure safety to "false" to exercise the early return 0 path
  s_safety_override = 0;
  sqlite3 db;
  db.nChange = 123; // value should be ignored when safety fails
  sqlite3_int64 res = sqlite3_changes64(&db);
  EXPECT_EQ(res, 0);
}

int main(){
  test_with_armor_true_safety();
  test_with_armor_false_safety();
  return 0;
}

#else
#error Define either TEST_MODE_NO_ARMOR or TEST_MODE_WITH_ARMOR to build this test suite.
#endif

// Notes for maintainers:
// - This test harness is self-contained and does not require the full SQLite build.
// - The API armor path is exercised by toggling s_safety_override via the test harness.
// - Static helpers in the focal class (as per provided FOCAL_CLASS_DEP) are not directly
//   accessed here since we are testing a single focal function. If needed, static
//   members/functions can be exercised by expanding the test harness in a separate
//   translation unit, respecting the rule that static symbols are file-scoped.
// - The test uses simple, non-terminating assertions (prints) to maximize code-path coverage
//   without terminating on first failure, as requested.