// Lightweight C++11 unit test harness for the focal function sqlite3_totype_init
// This test suite exercises the sqlite3_totype_init function from totype.c
// without relying on Google Test. It uses a small internal test framework with
// non-terminating assertions and a minimal mock for sqlite3_create_function.

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>


// Forward declare SQLite-like types used by the focal function.
// We do not pull in the real SQLite headers; we only provide enough to compile
// and link against the focal code (totype.c).
typedef struct sqlite3 sqlite3;
typedef long long sqlite3_int64;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// Minimal constants used by the focal function
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_UTF8
#define SQLITE_UTF8 0
#endif
#ifndef SQLITE_DETERMINISTIC
#define SQLITE_DETERMINISTIC 0
#endif
#ifndef SQLITE_INNOCUOUS
#define SQLITE_INNOCUOUS 0
#endif

// Extension initialization macros (defined as no-ops for test purposes)
#define SQLITE_EXTENSION_INIT1
#define SQLITE_EXTENSION_INIT2(pApi) (void)0

// Forward declare the focal function so test runner can call it.
// The real signature is provided by SQLite, but this is sufficient for the test's
// linking against the provided totype.c file.
extern "C" int sqlite3_totype_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);

// -------------------- Mock SQLite-like environment --------------------

// Mocked sqlite3_create_function call capture and behavior
struct CallRecord {
  std::string zFunc;
  int nArg;
  unsigned int eTextRep;
  void *pApp;
  void *xFunc;
  void *xStep;
  void *xFinal;
};

static std::vector<CallRecord> g_calls;
static int g_callCount = 0;
static int g_firstReturn = SQLITE_OK;
static int g_secondReturn = SQLITE_OK;

// Reset mock state before each test
static void reset_mock_state() {
  g_calls.clear();
  g_callCount = 0;
  g_firstReturn = SQLITE_OK;
  g_secondReturn = SQLITE_OK;
}

// Set the return values for the first and second sqlite3_create_function calls
static void set_first_return(int v) { g_firstReturn = v; }
static void set_second_return(int v) { g_secondReturn = v; }

// The test harness uses C linkage for the mock to satisfy the focal code's call sites.
extern "C" int sqlite3_create_function(
  sqlite3 *db,
  const char *zFunc,
  int nArg,
  unsigned int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
  void (*xStep)(sqlite3_context*, int, sqlite3_value**),
  void (*xFinal)(sqlite3_context*)
) {
  ++g_callCount;
  CallRecord rec;
  rec.zFunc = zFunc ? zFunc : "";
  rec.nArg = nArg;
  rec.eTextRep = eTextRep;
  rec.pApp = pApp;
  rec.xFunc = (void*)xFunc;
  rec.xStep = (void*)xStep;
  rec.xFinal = (void*)xFinal;
  g_calls.push_back(rec);

  if (g_callCount == 1) return g_firstReturn;
  if (g_callCount == 2) return g_secondReturn;
  // For any additional calls, return OK by default
  return SQLITE_OK;
}

// -------------------- Test framework (non-terminating assertions) --------------------

static int g_test_failures = 0;

static void log_failure(const std::string& msg) {
  std::cerr << "[FAIL] " << msg << std::endl;
  ++g_test_failures;
}
static void log_success(const std::string& msg) {
  std::cout << "[PASS] " << msg << std::endl;
}
static bool assert_int_eq(int a, int b, const std::string& msg) {
  if (a == b) {
    log_success(msg + " (" + std::to_string(a) + " == " + std::to_string(b) + ")");
    return true;
  } else {
    log_failure(msg + " (expected " + std::to_string(b) + ", got " + std::to_string(a) + ")");
    return false;
  }
}
static bool assert_str_eq(const std::string& a, const std::string& b, const std::string& msg) {
  if (a == b) {
    log_success(msg + " (\"" + a + "\" == \"" + b + "\")");
    return true;
  } else {
    log_failure(msg + " (expected \"" + b + "\", got \"" + a + "\")");
    return false;
  }
}

// -------------------- Tests --------------------

/*
  Test 1: Normal path where both sqlite3_create_function calls succeed (rc == SQLITE_OK twice).
  Expect: sqlite3_totype_init returns SQLITE_OK and two functions were registered:
  "tointeger" and "toreal".
*/
static void test_normal_path_both_ok() {
  reset_mock_state();
  set_first_return(SQLITE_OK);
  set_second_return(SQLITE_OK);

  sqlite3 *db = nullptr;
  char *pzErrMsg = nullptr;
  sqlite3_api_routines *pApi = nullptr;

  int rc = sqlite3_totype_init(db, &pzErrMsg, pApi);

  if (!assert_int_eq(rc, SQLITE_OK, "test_normal_path_both_ok: rc should be SQLITE_OK")) {
    // still proceed to inspect calls for additional insight
  }

  // Verify two registrations occurred with expected names
  if (g_calls.size() >= 2) {
    assert_str_eq(g_calls[0].zFunc, "tointeger", "test_normal_path_both_ok: first function should be 'tointeger'");
    assert_str_eq(g_calls[1].zFunc, "toreal", "test_normal_path_both_ok: second function should be 'toreal'");
  } else {
    log_failure("test_normal_path_both_ok: insufficient number of sqlite3_create_function calls");
  }
}

/*
  Test 2: First sqlite3_create_function fails (rc != SQLITE_OK). Second call should not happen.
  Expect: sqlite3_totype_init returns the error code from the first call and only one registration occurs.
*/
static void test_first_call_fails() {
  reset_mock_state();
  set_first_return(1);      // non-OK on first call
  set_second_return(SQLITE_OK);

  sqlite3 *db = nullptr;
  char *pzErrMsg = nullptr;
  sqlite3_api_routines *pApi = nullptr;

  int rc = sqlite3_totype_init(db, &pzErrMsg, pApi);

  assert_int_eq(rc, 1, "test_first_call_fails: rc should reflect first-call error (1)");
  if (g_calls.size() != 1) {
    log_failure("test_first_call_fails: expected exactly one sqlite3_create_function call");
  }
  else {
    assert_str_eq(g_calls[0].zFunc, "tointeger", "test_first_call_fails: first call should be 'tointeger'");
  }
}

/*
  Test 3: First call OK, second call fails. Ensure rc reflects the second error and both
  registrations were attempted (two calls were made, but second failed).
*/
static void test_second_call_fails() {
  reset_mock_state();
  set_first_return(SQLITE_OK);
  set_second_return(2); // non-OK on second call

  sqlite3 *db = nullptr;
  char *pzErrMsg = nullptr;
  sqlite3_api_routines *pApi = nullptr;

  int rc = sqlite3_totype_init(db, &pzErrMsg, pApi);

  assert_int_eq(rc, 2, "test_second_call_fails: rc should reflect second-call error (2)");
  if (g_calls.size() != 2) {
    log_failure("test_second_call_fails: expected exactly two sqlite3_create_function calls");
  } else {
    assert_str_eq(g_calls[0].zFunc, "tointeger", "test_second_call_fails: first call should be 'tointeger'");
    assert_str_eq(g_calls[1].zFunc, "toreal", "test_second_call_fails: second call should be 'toreal'");
  }
}

// -------------------- Main --------------------

int main() {
  std::cout << "Running unit tests for sqlite3_totype_init (totype.c) with a lightweight framework...\n";

  test_normal_path_both_ok();
  test_first_call_fails();
  test_second_call_fails();

  int total = 3;
  if (g_test_failures == 0) {
    std::cout << "All tests passed (" << total << " scenarios).\n";
    return 0;
  } else {
    std::cout << g_test_failures << " test(s) failed out of " << total << " scenarios.\n";
    return 1;
  }
}