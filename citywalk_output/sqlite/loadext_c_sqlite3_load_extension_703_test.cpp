// Self-contained C++11 unit tests for the focal method: sqlite3_load_extension
// This test suite provides minimal C stubs to simulate the required sqlite3
// environment and dependencies, without relying on the real SQLite library.
// It validates entry/exit semantics, arg propagation, and error message handling.

#include <cstdio>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>


// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;
static int g_enter_calls = 0;
static int g_leave_calls = 0;
static int g_ext_rc = 0;
static const char* g_last_zFile = nullptr;
static const char* g_last_zProc = nullptr;
static const char* g_last_set_err = nullptr;

// Minimal sqlite3 fake structure used by the focal function
typedef struct sqlite3 {
  void* mutex;
} sqlite3;

// Simple mutex object to mimic sqlite3_mutex_enter/leave tracking
struct FakeMutex {
  bool locked;
  FakeMutex(): locked(false) {}
};

// Global fake mutex instance for test db
static FakeMutex s_fakeMutex;

// Forward declarations of the functions under test
extern "C" {

// Original focal function prototype (as used in the project)
int sqlite3_load_extension(sqlite3 *db,
                         const char *zFile,
                         const char *zProc,
                         char **pzErrMsg);

// Mocked internal function used by sqlite3_load_extension
int sqlite3LoadExtension(sqlite3 *db,
                       const char *zFile,
                       const char *zProc,
                       char **pzErrMsg);

// Mocked API exit function (passes through rc)
int sqlite3ApiExit(sqlite3 *db, int rc);

// Mutex helpers (track calls)
void sqlite3_mutex_enter(void*); // signature as in real SQLite
void sqlite3_mutex_leave(void*);

} // extern "C"

// Implementations of mocked internals

// sqlite3_mutex_enter: mark as entered and increment counter
void sqlite3_mutex_enter(void* m) {
  (void)m; // in this test we rely on the global counter via test harness
  g_enter_calls++;
  s_fakeMutex.locked = true;
}

// sqlite3_mutex_leave: mark as left and increment counter
void sqlite3_mutex_leave(void* m) {
  (void)m;
  g_leave_calls++;
  s_fakeMutex.locked = false;
}

// sqlite3ApiExit: in test, simply return rc (no transformation)
int sqlite3ApiExit(sqlite3 *db, int rc) {
  (void)db;
  (void)rc;
  return rc;
}

// sqlite3LoadExtension: simulates loading extension by returning a chosen rc
// and potentially setting an error message when zFile matches "bad.so".
int sqlite3LoadExtension(sqlite3 *db,
                       const char *zFile,
                       const char *zProc,
                       char **pzErrMsg)
{
  (void)db; // not used in our tests except for existence
  g_last_zFile = zFile;
  g_last_zProc = zProc;

  // If a specific "bad" extension is requested and caller asks for an error msg,
  // provide a diagnostic string to exercise the pzErrMsg path.
  if (pzErrMsg && zFile && std::strcmp(zFile, "bad.so") == 0) {
    *pzErrMsg = const_cast<char*>("extension load failed");
    g_last_set_err = *pzErrMsg;
  } else {
    g_last_set_err = nullptr;
  }

  return g_ext_rc;
}

// sqlite3_load_extension: wraps the internal loader with mutex handling
int sqlite3_load_extension(sqlite3 *db,
                         const char *zFile,
                         const char *zProc,
                         char **pzErrMsg)
{
  int rc;
  // Acquire mutex around the operation
  sqlite3_mutex_enter(db->mutex);
  // Call into the internal loader
  rc = sqlite3LoadExtension(db, zFile, zProc, pzErrMsg);
  // Process possible API-level adjustments
  rc = sqlite3ApiExit(db, rc);
  // Release mutex
  sqlite3_mutex_leave(db->mutex);
  return rc;
}

// Helper: initialize a sqlite3 instance with the test's fake mutex
static sqlite3 make_test_db() {
  sqlite3 db;
  db.mutex = &s_fakeMutex;
  return db;
}

// Simple test harness helpers
static int s_fail_snapshot = 0;

// Macro-like assert (non-terminating)
#define ASSERT(cond, msg) do { \
  ++g_tests_run; \
  if(!(cond)) { \
    ++g_tests_failed; \
    std::printf("ASSERT FAILED: %s\n", msg); \
  } \
} while(0)

// Individual test cases

// Test 1: Normal successful load should:
 // - return rc == 0
 // - call mutex enter/leave exactly once
 // - propagate zFile and zProc correctly
 // - not set an error message when zFile != "bad.so"
static void test_normal_case() {
  int previous_failures = g_tests_failed;
  s_fail_snapshot = g_tests_failed;

  // Reset state
  g_enter_calls = 0;
  g_leave_calls = 0;
  g_ext_rc = 0;
  g_last_zFile = nullptr;
  g_last_zProc = nullptr;
  char* err = nullptr;

  sqlite3 db = make_test_db();

  int rc = sqlite3_load_extension(&db, "ext.so", "sqlite3_extension_init", &err);

  ASSERT(rc == 0, "rc should be 0 for normal success");
  ASSERT(g_enter_calls == 1, "mutex enter should be called once");
  ASSERT(g_leave_calls == 1, "mutex leave should be called once");
  ASSERT(g_last_zFile && std::string(g_last_zFile) == "ext.so", "zFile should be propagated as ext.so");
  ASSERT(g_last_zProc && std::string(g_last_zProc) == "sqlite3_extension_init", "zProc should be propagated correctly");
  ASSERT(err == nullptr, "pzErrMsg should be null when no error");
  if (g_tests_failed == previous_failures) {
    std::printf("test_normal_case: PASS\n");
  } else {
    std::printf("test_normal_case: FAIL\n");
  }
}

// Test 2: Ensure non-zero return code from internal loader propagates
// - rc == g_ext_rc
// - mutex enter/leave counters increment
static void test_error_code_propagation() {
  int previous_failures = g_tests_failed;
  s_fail_snapshot = g_tests_failed;

  // Reset state
  g_enter_calls = 0;
  g_leave_calls = 0;
  g_ext_rc = 5; // simulate internal loader signaling error
  g_last_zFile = nullptr;
  g_last_zProc = nullptr;
  char* err = nullptr;

  sqlite3 db = make_test_db();

  int rc = sqlite3_load_extension(&db, "ext.so", "proc", &err);

  ASSERT(rc == 5, "rc should propagate the internal loader's rc value (5)");
  ASSERT(g_enter_calls == 1, "mutex enter should be called once");
  ASSERT(g_leave_calls == 1, "mutex leave should be called once");
  // zFile and zProc passed through
  ASSERT(g_last_zFile && std::string(g_last_zFile) == "ext.so", "zFile should be ext.so");
  ASSERT(g_last_zProc && std::string(g_last_zProc) == "proc", "zProc should be propagated as 'proc'");
  ASSERT(err == nullptr, "pzErrMsg should be null when not explicitly set by bad.so case");
  if (g_tests_failed == previous_failures) {
    std::printf("test_error_code_propagation: PASS\n");
  } else {
    std::printf("test_error_code_propagation: FAIL\n");
  }
}

// Test 3: Verify error message is set when zFile requests a bad extension
// - rc may be non-error but pzErrMsg should be set
static void test_pzErrMsg_capture_on_bad_file() {
  int previous_failures = g_tests_failed;
  s_fail_snapshot = g_tests_failed;

  // Reset state
  g_enter_calls = 0;
  g_leave_calls = 0;
  g_ext_rc = 0; // no extension error
  g_last_zFile = nullptr;
  g_last_zProc = nullptr;
  char* err = nullptr;

  sqlite3 db = make_test_db();

  int rc = sqlite3_load_extension(&db, "bad.so", "sqlite3_extension_init", &err);

  // rc remains 0 (internal loader success), but pzErrMsg should be populated
  ASSERT(rc == 0, "rc should be 0 even if pzErrMsg is set");
  ASSERT(g_enter_calls == 1, "mutex enter should be called once");
  ASSERT(g_leave_calls == 1, "mutex leave should be called once");
  ASSERT(g_last_zFile && std::string(g_last_zFile) == "bad.so", "zFile should be propagated as 'bad.so'");
  ASSERT(err != nullptr, "pzErrMsg should be non-null when using bad.so");
  if (err != nullptr) {
    ASSERT(std::string(err) == "extension load failed", "pzErrMsg should contain the diagnostic message");
  }
  if (g_tests_failed == previous_failures) {
    std::printf("test_pzErrMsg_capture_on_bad_file: PASS\n");
  } else {
    std::printf("test_pzErrMsg_capture_on_bad_file: FAIL\n");
  }
}

// Test 4: Ensure that when zProc is null, it's passed as null to the internal loader
static void test_null_zProc_propagation() {
  int previous_failures = g_tests_failed;
  s_fail_snapshot = g_tests_failed;

  // Reset state
  g_enter_calls = 0;
  g_leave_calls = 0;
  g_ext_rc = 0;
  g_last_zFile = nullptr;
  g_last_zProc = nullptr;
  char* err = nullptr;

  sqlite3 db = make_test_db();

  int rc = sqlite3_load_extension(&db, "ext_without_proc.so", nullptr, &err);

  ASSERT(rc == 0, "rc should be 0 for normal success with null zProc");
  ASSERT(g_enter_calls == 1, "mutex enter should be called once");
  ASSERT(g_leave_calls == 1, "mutex leave should be called once");
  ASSERT(g_last_zFile && std::string(g_last_zFile) == "ext_without_proc.so", "zFile should be propagated correctly");
  ASSERT(g_last_zProc == nullptr, "zProc should be propagated as nullptr when input is null");
  if (g_tests_failed == previous_failures) {
    std::printf("test_null_zProc_propagation: PASS\n");
  } else {
    std::printf("test_null_zProc_propagation: FAIL\n");
  }
}

// Main: run all tests and report summary
int main() {
  // Run tests
  test_normal_case();
  test_error_code_propagation();
  test_pzErrMsg_capture_on_bad_file();
  test_null_zProc_propagation();

  // Summary
  std::printf("Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
  return (g_tests_failed == 0) ? 0 : 1;
}