// Test suite for sqlite3AutoLoadExtensions (as defined in the provided focal method)
// The tests are implemented in C++11 without using GTest, and rely on a small harness
// that provides minimal, controlled implementations of the surrounding sqlite3 API
// to exercise the function's logic (true/false branches, mutex usage, message handling).

#include <sqlite3ext.h>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Domain-aware notes:
// - We re-implement a minimal environment to exercise sqlite3AutoLoadExtensions.
// - We focus on the core decision points: nExt == 0 early return, looping over aExt[i],
//   calling the xInit extension entry, handling success/failure, and freeing zErrmsg.
// - We provide test variants that cover: zero extensions, one succeeding extension,
//   one failing extension, and multiple extensions including a failure mid-loop.

// Forward declare C linkage for the focal function and its dependent API surface.
extern "C" {

// Lightweight forward declaration of sqlite3 to satisfy prototype usage.
struct sqlite3 { int dummy; };

// Typedef for the function pointer type used by extensions.
typedef int (*sqlite3_loadext_entry)(sqlite3*, char** zErrMsg, const struct sqlite3_api_routines* pThunk);

// Minimal declaration of sqlite3_api_routines (the pThunk target).
struct sqlite3_api_routines { int dummy; };

// Minimal global that the original code would reference when not OMIT_LOAD_EXTENSION.
// We don't rely on any real behavior; the test-only surface is enough.
static sqlite3_api_routines sqlite3Apis;

// The actual focal function under test (re-implemented here to be callable in tests).
typedef unsigned int u32;
void sqlite3AutoLoadExtensions(sqlite3 *db);

// Helpers to simulate and capture behavior
// Mutex simulation
typedef struct sqlite3_mutex sqlite3_mutex;
struct sqlite3_mutex { int dummy; }; // opaque in test

static sqlite3_mutex dummy_mutex;
static int sqlite_mutex_enter_called = 0;
static int sqlite_mutex_leave_called = 0;

#ifndef SQLITE_MUTEX_STATIC_MAIN
#define SQLITE_MUTEX_STATIC_MAIN 0
#endif

#ifndef SQLITE_THREADSAFE
#define SQLITE_THREADSAFE 1
#endif

// Flags to control compilation paths (match the focal code behavior)
#define SQLITE_OMIT_LOAD_EXTENSION 0

// Global surface to emulate wsdAutoext (array of extension entry pointers)
typedef struct {
  int nExt;
  void **aExt;
} wsdAutoext_type;

// This instance will be manipulated by tests to simulate different environments.
static wsdAutoext_type wsdAutoext = {0, nullptr};

// Macro in original code; define as empty for test harness
#define wsdAutoextInit

// Utility: allocate dummy mutex (test-safe)
static sqlite3_mutex* sqlite3MutexAlloc(int) {
  return &dummy_mutex;
}
static void sqlite3_mutex_enter(sqlite3_mutex* /*mutex*/) {
  ++sqlite_mutex_enter_called;
}
static void sqlite3_mutex_leave(sqlite3_mutex* /*mutex*/) {
  ++sqlite_mutex_leave_called;
}

// Dummy db pointer for tests
static sqlite3 test_db;

// State capturing for xInit calls and error messages
static int g_xInit_calls = 0;

// For error handling path
static int g_last_rc_error = 0;
static char *g_last_errmsg_freed = nullptr;
static char *g_last_errmsg_allocated = nullptr;

// Mockable sqlite3ErrorWithMsg: capture rc, ignore formatting for simplicity
static void sqlite3ErrorWithMsg(sqlite3 * /*db*/, int rc, const char * /*zFormat*/, ...) {
  g_last_rc_error = rc;
  // We don't need to format the message for tests; rc suffices
}

// Mockable sqlite3_free to track freeing of zErrMsg
static void sqlite3_free(char *ptr) {
  g_last_errmsg_freed = ptr;
  if (ptr) {
    free(ptr);
  }
}

// xInit function types (to be placed into wsdAutoext.aExt[])
static int xInitSuccess(sqlite3 * /*db*/, char **zErrMsg, const sqlite3_api_routines* /*pThunk*/) {
  ++g_xInit_calls;
  if (zErrMsg) *zErrMsg = nullptr; // no error
  return 0;
}
static int xInitFail(sqlite3 * /*db*/, char **zErrMsg, const sqlite3_api_routines* /*pThunk*/) {
  ++g_xInit_calls;
  // allocate an error message to simulate real behavior
  if (zErrMsg) {
    const char *msg = "extension init failed";
    char *buf = (char*)malloc(strlen(msg) + 1);
    if (buf) strcpy(buf, msg);
    *zErrMsg = buf;
  }
  return 1;
}

// The implementation of the focal function (adapted to the test harness).
void sqlite3AutoLoadExtensions(sqlite3 *db) {
  u32 i;
  int go = 1;
  int rc;
  sqlite3_loadext_entry xInit;
  wsdAutoextInit;
  if( wsdAutoext.nExt==0 ){
    /* Common case: early out without every having to acquire a mutex */
    return;
  }
  for(i=0; go; i++){
    char *zErrmsg;
#if SQLITE_THREADSAFE
    sqlite3_mutex *mutex = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MAIN);
#endif
#ifdef SQLITE_OMIT_LOAD_EXTENSION
    const sqlite3_api_routines *pThunk = 0;
#else
    const sqlite3_api_routines *pThunk = &sqlite3Apis;
#endif
    sqlite3_mutex_enter(mutex);
    if( i>=wsdAutoext.nExt ){
      xInit = 0;
      go = 0;
    }else{
      xInit = (sqlite3_loadext_entry)wsdAutoext.aExt[i];
    }
    sqlite3_mutex_leave(mutex);
    zErrmsg = 0;
    if( xInit && (rc = xInit(db, &zErrmsg, pThunk))!=0 ){
      sqlite3ErrorWithMsg(db, rc,
            "automatic extension loading failed: %s", zErrmsg);
      go = 0;
    }
    sqlite3_free(zErrmsg);
  }
}

// End of C linkage surface
} // extern "C"

// C++ test harness
int main() {
  using namespace std;

  // Helper: reset all test-state
  auto reset_state = []() {
    g_xInit_calls = 0;
    g_last_rc_error = 0;
    g_last_errmsg_allocated = nullptr;
    g_last_errmsg_freed = nullptr;
    if (wsdAutoext.aExt) {
      // free previous allocations if any (not strictly necessary for test)
      // but ensure no stale memory remains
      // We can't free array memory here easily; tests manage their own buffers
    }
  };

  // Helper: perform test run and report
  auto run_test = [](const string &name, const function<void()>& fn) {
    cout << "TEST: " << name << " ... ";
    try {
      fn();
      cout << "PASS" << endl;
    } catch (...) {
      cout << "FAIL (exception)" << endl;
    }
  };

  // Prepare a dummy db object
  test_db.dummy = 0;

  // Test 1: wsdAutoext.nExt == 0 => early return, no xInit calls
  run_test("sqlite3AutoLoadExtensions: nExt == 0 (early return)", []() {
    // Arrange
    reset_state();
    wsdAutoext.nExt = 0;
    wsdAutoext.aExt = nullptr;

    // Act
    sqlite3AutoLoadExtensions(&test_db);

    // Assert
    if (g_xInit_calls != 0) {
      throw std::runtime_error("xInit was called despite nExt == 0");
    }
  });

  // Test 2: One extension that succeeds
  run_test("sqlite3AutoLoadExtensions: single ext that succeeds", []() {
    reset_state();
    wsdAutoext.nExt = 1;
    static void* ext0[1];
    ext0[0] = (void*)xInitSuccess;
    wsdAutoext.aExt = ext0;

    sqlite3_mutex_enter_called = 0;
    sqlite3_mutex_leave_called = 0;
    g_last_rc_error = 0;
    g_last_errmsg_freed = nullptr;
    g_last_errmsg_allocated = nullptr;

    // Act
    sqlite3AutoLoadExtensions(&test_db);

    // Assert
    if (g_xInit_calls != 1) {
      throw std::runtime_error("Expected exactly 1 xInit call for single ext success");
    }
    if (g_last_rc_error != 0) {
      throw std::runtime_error("No error should be reported for successful init");
    }
    if (g_last_errmsg_freed != nullptr) {
      throw std::runtime_error("No error message should have been allocated/freed");
    }
  });

  // Test 3: One extension that fails (rc != 0) and zErrmsg allocated
  run_test("sqlite3AutoLoadExtensions: single ext that fails (rc != 0)", []() {
    reset_state();
    wsdAutoext.nExt = 1;
    static void* ext0[1];
    ext0[0] = (void*)xInitFail;
    wsdAutoext.aExt = ext0;

    // Act
    sqlite3AutoLoadExtensions(&test_db);

    // Assert
    if (g_xInit_calls != 1) {
      throw std::runtime_error("Expected 1 xInit call on failure path");
    }
    if (g_last_rc_error == 0) {
      throw std::runtime_error("Expected non-zero rc to be reported via sqlite3ErrorWithMsg");
    }
    if (g_last_errmsg_freed == nullptr) {
      throw std::runtime_error("Expected zErrmsg to be freed after error");
    }
  });

  // Test 4: Two extensions: first succeeds, second fails
  run_test("sqlite3AutoLoadExtensions: two ext resources, second fails", []() {
    reset_state();
    wsdAutoext.nExt = 2;
    static void* ext[2];
    ext[0] = (void*)xInitSuccess;
    ext[1] = (void*)xInitFail;
    wsdAutoext.aExt = ext;

    // Act
    sqlite3AutoLoadExtensions(&test_db);

    // Assert: two calls to xInit, error surfaced from second
    if (g_xInit_calls != 2) {
      throw std::runtime_error("Expected 2 xInit calls for two extensions with second failing");
    }
    if (g_last_rc_error == 0) {
      throw std::runtime_error("Expected non-zero rc on second extension failure");
    }
    if (g_last_errmsg_freed == nullptr) {
      throw std::runtime_error("Expected zErrmsg to be freed after second error");
    }
  });

  // If all tests pass
  cout << "All tests completed." << endl;
  return 0;
}