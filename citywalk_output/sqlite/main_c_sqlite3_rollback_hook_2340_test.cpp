// Test suite for the sqlite3_rollback_hook function.
// This test is designed to be compiled in a C++11 (or compatible) environment.
// It mocks the necessary sqlite3 internal dependencies to exercise the focal function.
// No external testing framework is used (GTest is avoided as required).

#include <cstdio>
#include <cinttypes>
#include <cstring>
#include <sqliteInt.h>
#include <cstdint>


// -----------------------------------------------------------------------------
// Minimal surface area to mimic the real sqlite3 structure used by sqlite3_rollback_hook
// and provide the hooks that the focal function relies on.
// Note: This is a lightweight mock aligned with the function's usage in the file.
// -----------------------------------------------------------------------------

typedef struct sqlite3 {
  void *mutex;                       // dummy mutex handle
  void (*xRollbackCallback)(void*);  // currently installed rollback callback
  void *pRollbackArg;                // argument for the rollback callback
} sqlite3;

// -----------------------------------------------------------------------------
// Tight control over the API_ARMOR branch used by sqlite3_rollback_hook.
// The real code uses SQLITE_ENABLE_API_ARMOR to gate a safety check.
// We emulate that behavior in test by providing a controllable sqlite3SafetyCheckOk.
// -----------------------------------------------------------------------------

#define SQLITE_MISUSE_BKPT 0  // dummy constant used in the (void) cast in the code above

// Enable the API armor behavior in tests
#define SQLITE_ENABLE_API_ARMOR 1

extern "C" {

// Forward declaration of the focal function under test (expected to be defined in main.c)
void *sqlite3_rollback_hook(sqlite3 *db,
                          void (*xCallback)(void*),
                          void *pArg);

// Safety check control (will be overridden by test harness)
int sqlite3SafetyCheckOk(sqlite3 *db);

// Mutex enter/leave hooks (will be overridden by test harness)
void sqlite3_mutex_enter(void *mutex);
void sqlite3_mutex_leave(void *mutex);

} // extern "C"

// -----------------------------------------------------------------------------
// Test harness control: provide controllable behavior for safety check and mutex ops.
// This mirrors the minimal behavior required for the focal method under test.
// -----------------------------------------------------------------------------

static int g_safety_ok = 1;                 // toggles the safety check result
static int g_mutex_enter_calls = 0;         // counts mutex enter invocations
static int g_mutex_leave_calls = 0;         // counts mutex leave invocations
static int g_mutex_locked = 0;                // simple flag to reflect lock state

extern "C" {

// Override safety check logic to be controllable from tests
int sqlite3SafetyCheckOk(sqlite3 *db) {
  (void)db; // unused, but provided for completeness
  return g_safety_ok;
}

// Track mutex enter calls
void sqlite3_mutex_enter(void *mutex) {
  (void)mutex;
  ++g_mutex_enter_calls;
  g_mutex_locked = 1;
}

// Track mutex leave calls
void sqlite3_mutex_leave(void *mutex) {
  (void)mutex;
  ++g_mutex_leave_calls;
  g_mutex_locked = 0;
}

} // extern "C"

// -----------------------------------------------------------------------------
// Unit test helpers
// -----------------------------------------------------------------------------

static inline void* toPtr(uintptr_t v) {
  return reinterpret_cast<void*>(v);
}

static const char* toBoolStr(bool b) {
  return b ? "true" : "false";
}

// Simple non-terminating assertion helper
static inline bool expect_bool(bool cond, const char* expr, const char* testName) {
  if (cond) {
    std::printf("[PASS] %s: %s\n", testName, expr);
    return true;
  } else {
    std::printf("[FAIL] %s: %s\n", testName, expr);
    return false;
  }
}

// Public tests begin here

// Helper to print a separator for readability
static inline void printSeparator() {
  std::printf("------------------------------------------------------------\n");
}

// Test 1: Basic path - swap old pRollbackArg with new and install new callback
// Preconditions:
// - db initially has a non-null pRollbackArg and a non-null xRollbackCallback
// - We call sqlite3_rollback_hook with a non-null new callback and a non-null new arg
// Expectations:
// - Return value equals the original pRollbackArg
// - db.pRollbackArg is updated to the new arg
// - db.xRollbackCallback is updated to the new callback
// - Mutex enter/leave should be invoked exactly once each
static void test_basic_path_swap() {
  printSeparator();
  std::printf("Test 1: Basic path - swap old arg and install new callback\n");

  sqlite3 db;
  db.mutex = toPtr(0x1);
  // Old values
  void* oldArg = toPtr(0xDEADBEEF);
  void (*oldCallback)(void*) = [](void*){};
  db.pRollbackArg = oldArg;
  db.xRollbackCallback = oldCallback;

  // New values to install
  void* newArg = toPtr(0x1234);
  void (*newCallback)(void*) = [](void*){};

  void *ret = sqlite3_rollback_hook(&db, newCallback, newArg);

  bool ok = true;
  ok &= (ret == oldArg);
  ok &= (db.pRollbackArg == newArg);
  ok &= (db.xRollbackCallback == newCallback);
  ok &= (g_mutex_enter_calls == 1);
  ok &= (g_mutex_leave_calls == 1);

  expect_bool(ok, "Return oldArg; updated arg and callback; mutex usage", "test_basic_path_swap");
}

// Test 2: Null previous arg - ensure NULL is returned and updated correctly
// Preconditions:
// - initial pRollbackArg is NULL
// - a new callback and arg are provided
// Expectations:
// - Return value is NULL
// - pRollbackArg is updated to the new arg
// - xRollbackCallback is updated to the new callback
static void test_null_previous_arg() {
  printSeparator();
  std::printf("Test 2: Null previous arg - return NULL and update values\n");

  sqlite3 db;
  db.mutex = toPtr(0x2);
  db.pRollbackArg = nullptr;
  db.xRollbackCallback = nullptr;

  void* newArg = toPtr(0xABC);
  void (*newCallback)(void*) = [](void*){};

  void *ret = sqlite3_rollback_hook(&db, newCallback, newArg);

  bool ok = true;
  ok &= (ret == nullptr);
  ok &= (db.pRollbackArg == newArg);
  ok &= (db.xRollbackCallback == newCallback);

  expect_bool(ok, "Return NULL; updated arg and callback", "test_null_previous_arg");
}

// Test 3: API_ARMOR safety check fails - should return 0 and not modify db
// Preconditions:
// - Simulate safety check failure by toggling g_safety_ok to 0
// - initial values of pRollbackArg and xRollbackCallback are set to known values
// Expectations:
// - Return value is 0
// - pRollbackArg and xRollbackCallback remain unchanged
static void test_safety_check_fails() {
  printSeparator();
  std::printf("Test 3: Safety check fails - should return 0 and not modify db\n");

  g_safety_ok = 0; // force safety check to fail

  sqlite3 db;
  db.mutex = toPtr(0x3);
  void* oldArg = toPtr(0x101112);
  void (*oldCallback)(void*) = [](void*){};
  db.pRollbackArg = oldArg;
  db.xRollbackCallback = oldCallback;

  void* newArg = toPtr(0x999);
  void (*newCallback)(void*) = [](void*){};

  void *ret = sqlite3_rollback_hook(&db, newCallback, newArg);

  bool ok = true;
  ok &= (ret == 0);
  ok &= (db.pRollbackArg == oldArg);
  ok &= (db.xRollbackCallback == oldCallback);

  expect_bool(ok, "Return 0; no changes to db when safety check fails", "test_safety_check_fails");

  // Reset safety for potential further tests
  g_safety_ok = 1;
}

// Test 4: Callback can be NULL - ensure function accepts NULL callback
// Preconditions:
// - initial pRollbackArg is a non-null value
// - pass NULL as xCallback
// Expectations:
// - Return value is the original pRollbackArg
// - pRollbackArg is updated to the newArg
// - xRollbackCallback becomes NULL
static void test_null_callback_allowed() {
  printSeparator();
  std::printf("Test 4: NULL callback is allowed - ensure field is set to NULL\n");

  sqlite3 db;
  db.mutex = toPtr(0x4);
  void* oldArg = toPtr(0xFEEDBEAD);
  void (*oldCallback)(void*) = [](void*){};
  db.pRollbackArg = oldArg;
  db.xRollbackCallback = oldCallback;

  void* newArg = toPtr(0xBEEF);
  void (*newCallback)(void*) = nullptr; // explicitly NULL

  void *ret = sqlite3_rollback_hook(&db, newCallback, newArg);

  bool ok = true;
  ok &= (ret == oldArg);
  ok &= (db.pRollbackArg == newArg);
  ok &= (db.xRollbackCallback == newCallback);

  expect_bool(ok, "Return oldArg; update with NULL callback", "test_null_callback_allowed");
}

// Entry point for running all tests
int main() {
  // Initialize/clear global counters
  g_mutex_enter_calls = 0;
  g_mutex_leave_calls = 0;
  g_mutex_locked = 0;
  g_safety_ok = 1;

  test_basic_path_swap();
  test_null_previous_arg();
  test_safety_check_fails();
  test_null_callback_allowed();

  // Summary
  std::printf("Summary: mutex_enter=%d, mutex_leave=%d, safety_ok=%d\n",
              g_mutex_enter_calls, g_mutex_leave_calls, g_safety_ok);

  return 0;
}