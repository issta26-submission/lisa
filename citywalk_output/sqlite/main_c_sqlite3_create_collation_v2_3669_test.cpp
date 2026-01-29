// Test suite for the focal method sqlite3_create_collation_v2
// This is a self-contained, stand-alone test harness written in C++11
// It provides lightweight mocks/stubs for the necessary SQLite-like APIs
// and validates the behavior of the focal function under different scenarios.
//
// Notes:
// - This test recreates a minimal environment to exercise true/false branches.
// - It uses non-terminating assertions (EXPECT-like macros) to continue execution.
// - Explanatory comments are provided for each test case.
// - The test is designed to be compiled and linked in a typical C++11 toolchain.

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Enable the API armor branch as in the original code
#define SQLITE_ENABLE_API_ARMOR 1

// Basic SQLite-like return codes (subset for testing)
#define SQLITE_OK 0
#define SQLITE_MISUSE_BKPT 1

// Type aliases to mirror sqlite3's primitive types for the test
typedef unsigned char u8;

// Forward declarations of the functions/structures used by the focal method.
// We implement lightweight mocks for the necessary interactions.

struct sqlite3_mutex; // opaque mutex type (mock)
struct sqlite3 {
  sqlite3_mutex* mutex;
  int mallocFailed;
};

// Global test state to observe interactions
static bool g_safety_ok = true;                   // controls sqlite3SafetyCheckOk(db)
static int g_collation_rc = SQLITE_OK;            // return code from createCollation
static const char* g_last_name = nullptr;         // captured zName in createCollation
static u8 g_last_enc = 0;                          // captured enc in createCollation
static void* g_last_pCtx = nullptr;                // captured pCtx in createCollation
static int(*g_last_xCompare)(void*,int,const void*,int,const void*) = nullptr; // captured xCompare
static void(*g_last_xDel)(void*) = nullptr;        // captured xDel
static int g_enter_count = 0;
static int g_leave_count = 0;
static bool g_api_exit_called = false;
static int g_api_exit_rc = -1;

// Volatile counters to verify static/behavior
static int g_safety_checked_calls = 0;

// Dummy mutex enter/leave implementations
static void sqlite3_mutex_enter(sqlite3_mutex* /*m*/) {
  g_enter_count++;
}
static void sqlite3_mutex_leave(sqlite3_mutex* /*m*/) {
  g_leave_count++;
}

// Mocked safety check used by the focal method
extern "C" int sqlite3SafetyCheckOk(sqlite3* /*db*/) {
  g_safety_checked_calls++;
  return g_safety_ok ? 1 : 0;
}

// Mocked ApiExit to mimic sqlite3ApiExit behavior
extern "C" int sqlite3ApiExit(sqlite3* /*db*/, int rc) {
  g_api_exit_called = true;
  g_api_exit_rc = rc;
  return rc;
}

// A minimal, dummy mutex type (only required for type compatibility in tests)
struct sqlite3_mutex {
  // empty - acts as a placeholder
};

// Minimal sqlite3 instance for testing
static sqlite3 g_test_db;

// The focal function is implemented here in a self-contained way for testing.
// This mirrors the logic of the provided focal method, but uses our test stubs.
extern "C" int sqlite3_create_collation_v2(
  sqlite3* db,
  const char *zName,
  int enc,
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*),
  void(*xDel)(void*)
){
  int rc;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) || zName==nullptr ) return SQLITE_MISUSE_BKPT;
#endif
  sqlite3_mutex_enter(db->mutex);
  assert( !db->mallocFailed );
  // Call into a test-scoped createCollation to observe inputs
  rc = 0;
  // Forward to the test-scoped "static" implementation
  extern int createCollation(sqlite3* db, const char *zName, u8 enc, void* pCtx,
                             int(*xCompare)(void*,int,const void*,int,const void*),
                             void(*xDel)(void*));
  rc = createCollation(db, zName, (u8)enc, pCtx, xCompare, xDel);
  rc = sqlite3ApiExit(db, rc);
  sqlite3_mutex_leave(db->mutex);
  return rc;
}

// Test-scoped static implementation to capture inputs
static int createCollation(
  sqlite3* db,
  const char *zName,
  u8 enc,
  void* pCtx,
  int(*xCompare)(void*,int,const void*,int,const void*),
  void(*xDel)(void*)
){
  g_last_name = zName;
  g_last_enc = enc;
  g_last_pCtx = pCtx;
  g_last_xCompare = xCompare;
  g_last_xDel = xDel;
  return g_collation_rc;
}

// Dummy xCompare function
static int dummyCompare(void* a, int n1, const void* b, int n2, const void* c) {
  (void)a; (void)n1; (void)b; (void)n2; (void)c;
  return 0;
}

// Dummy xDel function
static void dummyDel(void* p) { (void)p; }

// Lightweight test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(a,b,desc) do { \
  g_total_tests++; \
  if((a)!=(b)) { \
    g_failed_tests++; \
    printf("EXPECT_EQ FAILED: %s | expected %d, got %d\n", (desc), (int)(b), (int)(a)); \
  } else { \
    printf("EXPECT_EQ PASSED: %s\n", (desc)); \
  } \
} while(0)

#define EXPECT_PTR_EQ(a,b,desc) do { \
  g_total_tests++; \
  if((void*)(a) != (void*)(b)) { \
    g_failed_tests++; \
    printf("EXPECT_PTR_EQ FAILED: %s | expected %p, got %p\n", (desc), (void*)(b), (void*)(a)); \
  } else { \
    printf("EXPECT_PTR_EQ PASSED: %s\n", (desc)); \
  } \
} while(0)

#define EXPECT_TRUE(cond,desc) do { \
  g_total_tests++; \
  if(!(cond)) { \
    g_failed_tests++; \
    printf("EXPECT_TRUE FAILED: %s\n", (desc)); \
  } else { \
    printf("EXPECT_TRUE PASSED: %s\n", (desc)); \
  } \
} while(0)

// Helper to reset test state
static void reset_test_state() {
  g_safety_ok = true;
  g_collation_rc = SQLITE_OK;
  g_last_name = nullptr;
  g_last_enc = 0;
  g_last_pCtx = nullptr;
  g_last_xCompare = nullptr;
  g_last_xDel = nullptr;
  g_enter_count = 0;
  g_leave_count = 0;
  g_api_exit_called = false;
  g_api_exit_rc = -1;
  g_safety_checked_calls = 0;
}

// Test 1: Armor enabled path with null zName should short-circuit to MISUSE_BKPT
// This validates the true branch when zName is null and API_ARMOR is enabled.
static void test_armor_null_name() {
  reset_test_state();
  g_safety_ok = true;
  g_test_db.mutex = nullptr;
  g_test_db.mallocFailed = 0;

  // Call with null zName to trigger early return
  int rc = sqlite3_create_collation_v2(&g_test_db, nullptr, 0, nullptr, dummyCompare, dummyDel);
  EXPECT_EQ(rc, SQLITE_MISUSE_BKPT, "Armor enabled: null zName should return MISUSE_BKPT");
  EXPECT_EQ(g_enter_count, 0, "Mutex should not be entered on early return (armor/null zName)");
  EXPECT_EQ(g_leave_count, 0, "Mutex should not be left on early return (armor/null zName)");
  EXPECT_FALSE(g_api_exit_called == true, "sqlite3ApiExit should not be called on early return");
}

// Test 2: Normal path with valid inputs should propagate rc from createCollation via sqlite3ApiExit
// This validates the normal execution path, including mutex behavior and data propagation.
static void test_normal_path_rc_propagation() {
  reset_test_state();
  g_safety_ok = true;
  g_collation_rc = 42; // expected rc from createCollation
  g_test_db.mutex = reinterpret_cast<sqlite3_mutex*>(0xABCDE); // sentinel
  g_test_db.mallocFailed = 0;

  int rc = sqlite3_create_collation_v2(&g_test_db, "test_collation", 0, nullptr, dummyCompare, dummyDel);

  EXPECT_EQ(rc, 42, "Normal path: rc should be propagated from createCollation via ApiExit");
  EXPECT_EQ(g_enter_count, 1, "Mutex should be entered exactly once");
  EXPECT_EQ(g_leave_count, 1, "Mutex should be left exactly once");
  EXPECT_TRUE(g_api_exit_called, "sqlite3ApiExit should be called in normal path");
  EXPECT_PTR_EQ((void*)g_last_name, (void*)"test_collation", "zName should be propagated to createCollation");
  EXPECT_EQ(g_last_enc, 0, "Encoding should be cast to u8 (lower 8 bits)");
  EXPECT_PTR_EQ((void*)g_last_pCtx, nullptr, "pCtx should be passed through to createCollation");
  EXPECT_PTR_EQ((void*)g_last_xCompare, (void*)dummyCompare, "xCompare should be passed through to createCollation");
  EXPECT_PTR_EQ((void*)g_last_xDel, (void*)dummyDel, "xDel should be passed through to createCollation");
}

// Test 3: Armor/Safety check false should short-circuit to MISUSE_BKPT without entering mutex
// This ensures the safety check is honored even if zName is non-null.
static void test_safety_check_false() {
  reset_test_state();
  g_safety_ok = false;
  g_test_db.mutex = reinterpret_cast<sqlite3_mutex*>(0x12345);
  g_test_db.mallocFailed = 0;

  int rc = sqlite3_create_collation_v2(&g_test_db, "safe_name", 0, nullptr, dummyCompare, dummyDel);
  EXPECT_EQ(rc, SQLITE_MISUSE_BKPT, "Safety check false: should return MISUSE_BKPT");
  EXPECT_EQ(g_enter_count, 0, "Mutex should not be entered when safety check fails");
  EXPECT_EQ(g_leave_count, 0, "Mutex should not be left when safety check fails");
}

// Test 4: Enc value is truncated to 8 bits (e.g., 0x123 becomes 0x23)
// This validates the enc casting behavior observed in the internal call to createCollation.
static void test_enc_truncation() {
  reset_test_state();
  g_safety_ok = true;
  g_collation_rc = 0;
  g_test_db.mutex = reinterpret_cast<sqlite3_mutex*>(0xAAAA);
  g_test_db.mallocFailed = 0;

  int rc = sqlite3_create_collation_v2(&g_test_db, "enc_test", 0x123, nullptr, dummyCompare, dummyDel);

  EXPECT_EQ(rc, 0, "Enc truncation: rc should come from createCollation (0 by default)");
  EXPECT_EQ(g_last_enc, static_cast<u8>(0x123), "Stored enc should be lower 8 bits (0x23)");
}

// Helper to print final summary
static void print_summary() {
  printf("\nTest Summary: %d tests, %d failures\n", g_total_tests, g_failed_tests);
  if(g_failed_tests != 0) {
    printf("Some tests FAILED. Review the messages above for details.\n");
  } else {
    printf("All tests PASSED.\n");
  }
}

// Entry point
int main() {
  printf("Starting sqlite3_create_collation_v2 unit tests (stand-alone mock).\n\n");

  // Run tests (explanations are included above each test)
  test_armor_null_name();
  test_normal_path_rc_propagation();
  test_safety_check_false();
  test_enc_truncation();

  print_summary();
  // Return non-zero on any failure for CI convenience
  return (g_failed_tests != 0) ? 1 : 0;
}