// A self-contained C++11 test harness for the focal method logic.
// Note: This test mirrors the behavior of sqlite3_bind_double's core logic
// (as seen in the provided focal method) in order to enable unit testing
// without requiring the full SQLite build. It uses lightweight stubs and
// a small, non-GTest testing framework with non-terminating assertions.

#include <opcodes.h>
#include <cmath>
#include <iomanip>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// ---------------------------
// Lightweight Test Framework
// ---------------------------

static int g_fail_count = 0;
static int g_tests_run = 0;

// Non-terminating expectation macro: prints message on failure but continues.
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[EXPECT_TRUE] " << msg << "  (file: " << __FILE__ \
                  << ", line: " << __LINE__ << ")" << std::endl; \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual, msg) do { \
    if(!((expected) == (actual))) { \
        std::cerr << "[EXPECT_EQ] " << msg \
                  << "  Expected: " << (expected) \
                  << ", Actual: " << (actual) \
                  << "  (file: " << __FILE__ << ", line: " << __LINE__ << ")" \
                  << std::endl; \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_DOUBLE_EQ(expected, actual, tol, msg) do { \
    if(!(std::fabs((expected) - (actual)) <= (tol))) { \
        std::cerr << "[EXPECT_DOUBLE_EQ] " << msg \
                  << "  Expected: " << (expected) \
                  << "  Actual: " << (actual) \
                  << "  Tol: " << (tol) \
                  << "  (file: " << __FILE__ << ", line: " << __LINE__ << ")" \
                  << std::endl; \
        ++g_fail_count; \
    } \
} while(0)

static void test_report_summary() {
    std::cout << "\nTest results: " << g_tests_run << " run, "
              << g_fail_count << " failures." << std::endl;
    if(g_fail_count == 0) {
        std::cout << "All tests passed." << std::endl;
    }
}

// ---------------------------
// Minimal In-Test Runtime (Stubs)
// ---------------------------

// Reproduced/simplified types to model the focal function's environment.

typedef struct Mem {
  int flags;
  double rValue; // used to store double binding result
} Mem;

typedef struct sqlite3 {
  void *mutex; // opaque mutex placeholder
} sqlite3;

typedef struct Vdbe {
  sqlite3 *db;
  Mem aVar[64]; // array for bound values, size 64 aligns with typical sqlite usage
} Vdbe;

// The opaque sqlite3_stmt in the real code is cast to Vdbe* by the focal method.
// Here, we simply alias sqlite3_stmt to Vdbe for test simplicity.
typedef void sqlite3_stmt;

// Global instrumentation for tests:
static int g_vdbeUnbind_rc_for_test = 0; // 0 => SQLITE_OK, non-zero => error path
static bool g_mutex_leave_called = false;

// Forward declarations of helper "public" testing utilities to mirror real API style
// (these implementations are test-local and do not come from the real SQLite library).

#define SQLITE_OK 0

static void sqlite3VdbeMemSetDouble(Mem *pMem, double rValue) {
  // Mimics sqlite3VdbeMemSetDouble: stores the given double into Mem
  pMem->rValue = rValue;
  pMem->flags = 0; // clear any fake flags
}

// Simulated mutex leave function: records that it was invoked.
static void sqlite3_mutex_leave(void *mutex) {
  (void)mutex; // unused in test, but matches real API signature
  g_mutex_leave_called = true;
}

// Test-time replacement for the static vdbeUnbind in the real code.
// The real function is static in vdbeapi.c; here we expose a test hook.
static int vdbeUnbind_for_test(Vdbe *p, unsigned int i) {
  (void)p; // p is not used by test hook beyond shape
  (void)i;
  return g_vdbeUnbind_rc_for_test;
}

// The focal method under test (re-implemented for unit testing in isolation).
// This mirrors the logic in the provided FOCAL_METHOD precisely, but uses
// test-time hooks to control behavior.
static int sqlite3_bind_double_like(sqlite3_stmt *pStmt, int i, double rValue) {
  int rc;
  Vdbe *p = (Vdbe *)pStmt;
  rc = vdbeUnbind_for_test(p, (unsigned int)(i - 1));
  if (rc == SQLITE_OK) {
    sqlite3VdbeMemSetDouble(&p->aVar[i - 1], rValue);
    sqlite3_mutex_leave(p->db->mutex);
  }
  return rc;
}

// ---------------------------
// Test Suite: Covering sqlite3_bind_double-like behavior
// ---------------------------

// Helper to initialize a Vdbe with a dummy sqlite3 and zeroed memory array.
static void init_test_environment(Vdbe *p) {
  static sqlite3 db;
  db.mutex = (void*)0x1; // non-null mutex to simulate successful lock/unlock
  p->db = &db;
  // Clear all bound values
  for (int idx = 0; idx < 64; ++idx) {
    p->aVar[idx].flags = 0;
    p->aVar[idx].rValue = 0.0;
  }
  g_mutex_leave_called = false;
}

// Test 1: rc == SQLITE_OK path should set the double value and release the mutex
static void test_bind_double_ok_path_sets_value_and_releases_mutex() {
  g_tests_run++;
  Vdbe p;
  init_test_environment(&p);

  // Configure test hook to return OK
  g_vdbeUnbind_rc_for_test = SQLITE_OK;

  int rc = sqlite3_bind_double_like((sqlite3_stmt*)&p, 1, 3.14159265358979323846);

  EXPECT_EQ(SQLITE_OK, rc, "Return code should be SQLITE_OK on success path");
  EXPECT_DOUBLE_EQ(3.14159265358979323846, p.aVar[0].rValue, 1e-12,
                   "Bound value at aVar[0] should be set to the provided rValue");
  EXPECT_TRUE(g_mutex_leave_called, "sqlite3_mutex_leave should be called on success path");
}

// Test 2: rc != SQLITE_OK path should not modify memory nor release mutex
static void test_bind_double_error_path_should_not_modify_mem_or_mutex() {
  g_tests_run++;
  Vdbe p;
  init_test_environment(&p);

  // Initialize with a known non-zero value to detect unintended changes
  p.aVar[0].rValue = 9.8765;

  // Configure test hook to return an error
  g_vdbeUnbind_rc_for_test = -1; // non-zero indicates error

  int rc = sqlite3_bind_double_like((sqlite3_stmt*)&p, 1, 1.2345);

  // Expect error path
  EXPECT_TRUE(rc != SQLITE_OK, "Return code should indicate error when vdbeUnbind fails");
  // Value should remain unchanged
  EXPECT_DOUBLE_EQ(9.8765, p.aVar[0].rValue, 1e-12,
                   "Bound value should remain unchanged when rc != SQLITE_OK");
  // Mutex should not be released in error path
  EXPECT_TRUE(!g_mutex_leave_called, "sqlite3_mutex_leave should NOT be called on error path");
}

// Test 3: Binding to a different index (i = 2) updates the correct slot without affecting others
static void test_bind_double_binds_to_correct_index() {
  g_tests_run++;
  Vdbe p;
  init_test_environment(&p);

  g_vdbeUnbind_rc_for_test = SQLITE_OK;
  int rc = sqlite3_bind_double_like((sqlite3_stmt*)&p, 2, -6.28);

  EXPECT_EQ(SQLITE_OK, rc, "Return code should be SQLITE_OK for valid index");
  // Check index 1 (i-1) updated
  EXPECT_DOUBLE_EQ(-6.28, p.aVar[1].rValue, 1e-12,
                   "Bound value should be stored at aVar[i-1]");
  // Other slots should remain at initial 0
  EXPECT_DOUBLE_EQ(0.0, p.aVar[0].rValue, 1e-12,
                   "Other slots should remain unaffected");
  EXPECT_TRUE(g_mutex_leave_called, "Mutex should be released for successful bind");
}

// Test 4: Boundary condition: binding to the last slot (i = 64) does not overflow
static void test_bind_double_bound_index_at_end() {
  g_tests_run++;
  Vdbe p;
  init_test_environment(&p);

  g_vdbeUnbind_rc_for_test = SQLITE_OK;
  int rc = sqlite3_bind_double_like((sqlite3_stmt*)&p, 64, 0.123456);

  EXPECT_EQ(SQLITE_OK, rc, "Return code should be SQLITE_OK for end boundary");
  EXPECT_DOUBLE_EQ(0.123456, p.aVar[63].rValue, 1e-12,
                   "Bound value should be stored at the last valid index aVar[63]");
  EXPECT_TRUE(g_mutex_leave_called, "Mutex should be released on successful bind");
}

// ---------------------------
// Main: Run all tests
// ---------------------------

int main() {
  // Run test suite
  test_bind_double_ok_path_sets_value_and_releases_mutex();
  test_bind_double_error_path_should_not_modify_mem_or_mutex();
  test_bind_double_binds_to_correct_index();
  test_bind_double_bound_index_at_end();

  test_report_summary();
  return g_fail_count != 0 ? 1 : 0;
}