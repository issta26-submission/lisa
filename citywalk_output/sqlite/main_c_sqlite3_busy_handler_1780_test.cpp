// Unit test suite for the focal method: sqlite3_busy_handler
// This test is crafted to be self-contained and compiles with C++11.
// It provides a minimal stub environment for the sqlite3-like types and
// functions used by sqlite3_busy_handler, then exercises the function's
// core logic, including the API_ARMOR guarded path.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Domain-specific macros/constants (simplified for the test harness)
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT 1

// Enable the API armor code path to exercise the safety check branch.
#define SQLITE_ENABLE_API_ARMOR

// ------------------------------------------------------------
// Minimal type stubs to emulate the parts of sqlite3 used by
// sqlite3_busy_handler in the focal code.
// ------------------------------------------------------------

// Forward declaration of our test sqlite3 struct
struct BusyHandler {
  int (*xBusyHandler)(void*, int);
  void *pBusyArg;
  int nBusy;
};

struct sqlite3 {
  void *mutex;           // dummy mutex handle
  BusyHandler busyHandler;
  int busyTimeout;
};

// ------------------------------------------------------------
// Test helpers and global state to observe behavior

// Global counters to verify that mutex enter/leave are called
static int g_mutex_enter_count = 0;
static int g_mutex_leave_count = 0;

// Safety check control (to simulate SQLITE_ENABLE_API_ARMOR path)
static int g_safety_check_ok = 1; // 1 = ok, 0 = fail

// Expose a tiny fake safety check function used by the focal code
extern "C" int sqlite3SafetyCheckOk(sqlite3 *db) {
  // Return the current simulated safety check result
  return g_safety_check_ok;
}

// Fake mutex enter/leave implementations to observe sequence
extern "C" void sqlite3_mutex_enter(void *);

/* The function signature is invoked by sqlite3_busy_handler(db, xBusy, pArg) */
extern "C" void sqlite3_mutex_enter(void * /*mtx*/) {
  g_mutex_enter_count++;
  // No real locking required for the unit test
}
extern "C" void sqlite3_mutex_leave(void * /*mtx*/) {
  g_mutex_leave_count++;
  // No real unlocking required for the unit test
}

// Make sure the compiler doesn't mangle the function we are testing
extern "C" int sqlite3_busy_handler(
  sqlite3 *db,
  int (*xBusy)(void*,int),
  void *pArg
);

// Forward-declare a dummy busy function used as a non-null xBusy
extern "C" int dummyBusy(void *ctx, int n);

// A small helper to reset global test state between tests
static void reset_test_state() {
  g_mutex_enter_count = 0;
  g_mutex_leave_count = 0;
  g_safety_check_ok = 1;
}

// ------------------------------------------------------------
// Implementation of the focal method under test
// We reproduce the key behavior from the provided focal method even though
// in a real project this would come from the actual sqlite3 source.
// This implementation mirrors:
// - Optional safety check when SQLITE_ENABLE_API_ARMOR is defined
// - Enter/leave of the db mutex
// - Assignment of busy handler and reset of counters
// - Return SQLITE_OK on success
// ------------------------------------------------------------
extern "C" int sqlite3_busy_handler(
  sqlite3 *db,
  int (*xBusy)(void*,int),
  void *pArg
){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ) return SQLITE_MISUSE_BKPT;
#endif
  sqlite3_mutex_enter(db->mutex);
  db->busyHandler.xBusyHandler = xBusy;
  db->busyHandler.pBusyArg = pArg;
  db->busyHandler.nBusy = 0;
  db->busyTimeout = 0;
  sqlite3_mutex_leave(db->mutex);
  return SQLITE_OK;
}

// ------------------------------------------------------------
// TESTS

// Tiny assertion framework (non-terminating, prints errors but continues)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const char* file, int line, const char* cond) {
  fprintf(stderr, "Test failure in %s:%d: %s\n", file, line, cond);
  ++g_failed_tests;
}

template <typename T, typename U>
static void expect_eq(const char* file, int line, const T& a, const U& b, const char* expr) {
  if (! (a == b) ) {
    fprintf(stderr, "Assertion failed at %s:%d: %s\n", file, line, expr);
    ++g_failed_tests;
  }
}

#define EXPECT_TRUE(cond) do { \
  ++g_total_tests; \
  if(!(cond)) log_failure(__FILE__, __LINE__, #cond); \
} while(0)

#define EXPECT_EQ(a,b) expect_eq(__FILE__, __LINE__, (a), (b), #a " == " #b)

static int test_busy_handler_updates_fields_on_safe_path() {
  // Test 1: API_ARMOR path is active and safety check passes.
  // We verify that:
  // - xBusyHandler is assigned
  // - pBusyArg is stored
  // - nBusy is reset to 0
  // - busyTimeout is reset to 0
  // - mutex enter/leave are invoked exactly once each
  g_total_tests = 0;
  g_failed_tests = 0;

  reset_test_state();

  // Prepare a dummy db and a non-null mutex handle
  sqlite3 db;
  db.mutex = (void*)0xDEADBEEF;
  db.busyTimeout = 12345; // ensure it gets reset to 0

  // Initialize busyHandler to known non-zero values to ensure they get overwritten
  db.busyHandler.xBusyHandler = nullptr;
  db.busyHandler.pBusyArg = (void*)0xCAFEBABE;
  db.busyHandler.nBusy = 7;

  // Clear safety-check flag to success
  g_safety_check_ok = 1;

  int rc = sqlite3_busy_handler(&db, dummyBusy, (void*)0x12345678);

  // Assertions
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(db.busyHandler.xBusyHandler, dummyBusy);
  EXPECT_EQ(db.busyHandler.pBusyArg, (void*)0x12345678);
  EXPECT_EQ(db.busyHandler.nBusy, 0);
  EXPECT_EQ(db.busyTimeout, 0);
  EXPECT_EQ(g_mutex_enter_count, 1);
  EXPECT_EQ(g_mutex_leave_count, 1);

  fprintf(stderr, "Test test_busy_handler_updates_fields_on_safe_path completed with %d checks, %d failures.\n",
          g_total_tests, g_failed_tests);
  return g_failed_tests;
}

// Test 2: API_ARMOR safety check fails, function should return MISUSE_BKPT
// and should not touch the mutex or busy handler fields.
static int test_busy_handler_safety_check_fails() {
  g_total_tests = 0;
  g_failed_tests = 0;
  reset_test_state();

  sqlite3 db;
  db.mutex = (void*)0xBADC0DE;
  db.busyTimeout = 999;

  // Set initial values to detect that they aren't overwritten on failure
  db.busyHandler.xBusyHandler = (int (*)(void*, int))0x11111111;
  db.busyHandler.pBusyArg = (void*)0x22222222;
  db.busyHandler.nBusy = 42;
  int initial_busyTimeout = db.busyTimeout;

  // Simulate safety check failure
  g_safety_check_ok = 0;

  int rc = sqlite3_busy_handler(&db, dummyBusy, nullptr);

  // Assertions
  EXPECT_EQ(rc, SQLITE_MISUSE_BKPT);
  // Ensure no changes occurred due to early return
  EXPECT_EQ(db.busyHandler.xBusyHandler, (int (*)(void*, int))0x11111111);
  EXPECT_EQ(db.busyHandler.pBusyArg, (void*)0x22222222);
  EXPECT_EQ(db.busyHandler.nBusy, 42);
  EXPECT_EQ(db.busyTimeout, initial_busyTimeout);
  // Mutex should not have been entered or left
  EXPECT_EQ(g_mutex_enter_count, 0);
  EXPECT_EQ(g_mutex_leave_count, 0);

  // Reset safety for any potential further tests
  g_safety_check_ok = 1;

  fprintf(stderr, "Test test_busy_handler_safety_check_fails completed with %d checks, %d failures.\n",
          g_total_tests, g_failed_tests);
  return g_failed_tests;
}

// ------------------------------------------------------------
// Minimal busy function implementation for test (does nothing)
extern "C" int dummyBusy(void *ctx, int n) {
  (void)ctx;
  (void)n;
  return 0;
}

// ------------------------------------------------------------
// Entry point for the test executable
int main(void) {
  int failures = 0;

  fprintf(stderr, "Starting sqlite3_busy_handler unit tests (standalone C++ harness)\n");

  // Run tests
  failures += test_busy_handler_updates_fields_on_safe_path();
  failures += test_busy_handler_safety_check_fails();

  if (failures == 0) {
    fprintf(stderr, "All tests passed.\n");
    return 0;
  } else {
    fprintf(stderr, "Total failures: %d\n", failures);
    return 1;
  }
}