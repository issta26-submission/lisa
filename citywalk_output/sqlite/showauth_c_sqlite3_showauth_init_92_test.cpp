// Automated unit tests for sqlite3_showauth_init
// This test suite uses a lightweight, self-contained harness (no GoogleTest)
// and targets the focal method sqlite3_showauth_init along with its core dependency
// sqlite3_set_authorizer and the authorizer callback used by the function.
//
// Notes:
// - The test re-implements the essential parts of the sqlite3 interface in a
//   controlled, lightweight manner to avoid external dependencies.
// - All tests are non-terminating: failures are logged, but execution continues.
// - The implementation mirrors the provided focal method's behavior as closely as possible.

#include <stdio.h>
#include <sqlite3ext.h>
#include <cstdint>
#include <iostream>


// Step 1 & 2: Provide a minimal, self-contained C environment that mimics
// the parts of the SQLite API used by the focal method.
// We place this in an extern "C" block to ensure C linkage for the symbols.

extern "C" {

// Forward declare the opaque SQLite types used by the focal method.
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// Callback type used by sqlite3_set_authorizer
typedef int (*sqlite3_auth_cb)(void*, int, const char*, const char*, const char*, const char*);

// Lightweight constants to mimic SQLite status codes
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Simple macro to emulate SQLITE_EXTENSION_INIT2(pApi) with no-op behavior
#define SQLITE_EXTENSION_INIT2(pApi) do { (void)(pApi); } while(0)

// Global variables to observe interactions from sqlite3_showauth_init
sqlite3_auth_cb g_last_auth_cb = (sqlite3_auth_cb)0;
void*        g_last_clientData = NULL;
int          g_force_error  = 0;  // When non-zero, sqlite3_set_authorizer reports error

// A simple authorizer callback used by the focal method.
// It does minimal work here and always returns success.
int authCallback(void *pClientData, int op,
                 const char *z1, const char *z2,
                 const char *z3, const char *z4)
{
  (void)pClientData;
  (void)op;
  (void)z1;
  (void)z2;
  (void)z3;
  (void)z4;
  return 0;
}

// Stubbed sqlite3_set_authorizer that records parameters and simulates behavior.
int sqlite3_set_authorizer(sqlite3* db, sqlite3_auth_cb cb, void* pClientData)
{
  // Record the callback and client data for verification.
  g_last_auth_cb = cb;
  g_last_clientData = pClientData;

  // Simulate simple success/failure logic:
  // - If db is NULL: treat as error
  // - If explicit global flag is set: simulate error
  if (db == nullptr) return SQLITE_ERROR;
  if (g_force_error) return SQLITE_ERROR;

  return SQLITE_OK;
}

// The focal function under test, copied/adapted here to enable self-contained testing.
// It uses SQLITE_EXTENSION_INIT2 and then sets the authorizer to authCallback.
int sqlite3_showauth_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;  /* Unused parameter in this test harness */
  rc = sqlite3_set_authorizer(db, authCallback, 0);
  return rc;
}

} // extern "C"


// Step 3: Lightweight test harness (non-terminating, standalone, no GTest/GMock)

static int g_total_tests = 0;
static int g_failed_tests = 0;

// Helper to log a failed test with a descriptive message
static void log_failure(const char* test_name, const char* message)
{
  std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
  ++g_failed_tests;
}

// Test 1: Normal path
// - A non-null db is provided
// - sqlite3_set_authorizer reports SQLITE_OK
// - The callback pointer stored is the address of authCallback
// - The client data is 0
static bool test_normal_path()
{
  const char* test_name = "test_normal_path";
  // Reset global state observed by the stub
  g_last_auth_cb = (sqlite3_auth_cb)0;
  g_last_clientData = NULL;
  g_force_error = 0;

  // Prepare a non-null dummy db
  sqlite3 dummy_db;
  sqlite3 *db = &dummy_db;
  char* errMsg = NULL;
  sqlite3_api_routines* pApi = NULL;

  int rc = sqlite3_showauth_init(db, &errMsg, pApi);

  bool ok = (rc == SQLITE_OK) &&
            (g_last_auth_cb == authCallback) &&
            (g_last_clientData == NULL);

  if (!ok) {
    log_failure(test_name, "Expected rc=SQLITE_OK, callback=authCallback, clientData=NULL");
    if (rc != SQLITE_OK) {
      std::cerr << "  rc=" << rc << std::endl;
    }
    if (g_last_auth_cb != authCallback) {
      std::cerr << "  callback mismatch." << std::endl;
    }
    if (g_last_clientData != NULL) {
      std::cerr << "  clientData mismatch." << std::endl;
    }
  }

  ++g_total_tests;
  return ok;
}

// Test 2: Null database should yield an error
// - db == nullptr -> sqlite3_set_authorizer should return SQLITE_ERROR
// - The callback pointer and client data should still have been recorded
static bool test_null_db_path()
{
  const char* test_name = "test_null_db_path";

  g_last_auth_cb = (sqlite3_auth_cb)0;
  g_last_clientData = NULL;
  g_force_error = 0;

  char* errMsg = NULL;
  sqlite3_api_routines* pApi = NULL;

  int rc = sqlite3_showauth_init(nullptr, &errMsg, pApi);

  bool ok = (rc == SQLITE_ERROR) &&
            (g_last_auth_cb == authCallback) &&
            (g_last_clientData == NULL);

  if (!ok) {
    log_failure(test_name, "Expected rc=SQLITE_ERROR with cb=authCallback and clientData=NULL");
    if (rc != SQLITE_ERROR) {
      std::cerr << "  rc=" << rc << std::endl;
    }
    if (g_last_auth_cb != authCallback) {
      std::cerr << "  callback mismatch." << std::endl;
    }
    if (g_last_clientData != NULL) {
      std::cerr << "  clientData mismatch." << std::endl;
    }
  }

  ++g_total_tests;
  return ok;
}

// Test 3: Non-null db, but non-null pApi (integration of the macro only needs to be non-null; functionality is a no-op here)
static bool test_non_null_pApi_path()
{
  const char* test_name = "test_non_null_pApi_path";

  g_last_auth_cb = (sqlite3_auth_cb)0;
  g_last_clientData = NULL;
  g_force_error = 0;

  sqlite3 dummy_db;
  sqlite3 *db = &dummy_db;
  char* errMsg = NULL;
  // A non-null dummy API struct (opaque to this test)
  sqlite3_api_routines dummy_api;
  sqlite3_api_routines *pApi = &dummy_api;

  int rc = sqlite3_showauth_init(db, &errMsg, pApi);

  bool ok = (rc == SQLITE_OK) &&
            (g_last_auth_cb == authCallback) &&
            (g_last_clientData == NULL);

  if (!ok) {
    log_failure(test_name, "Expected rc=SQLITE_OK with cb=authCallback and clientData=NULL");
    if (rc != SQLITE_OK) {
      std::cerr << "  rc=" << rc << std::endl;
    }
    if (g_last_auth_cb != authCallback) {
      std::cerr << "  callback mismatch." << std::endl;
    }
    if (g_last_clientData != NULL) {
      std::cerr << "  clientData mismatch." << std::endl;
    }
  }

  ++g_total_tests;
  return ok;
}

// Test 4: Force sqlite3_set_authorizer to return error via global flag
static bool test_force_error_path()
{
  const char* test_name = "test_force_error_path";

  g_last_auth_cb = (sqlite3_auth_cb)0;
  g_last_clientData = NULL;
  g_force_error = 1;

  sqlite3 dummy_db;
  sqlite3 *db = &dummy_db;
  char* errMsg = NULL;
  sqlite3_api_routines* pApi = NULL;

  int rc = sqlite3_showauth_init(db, &errMsg, pApi);

  // After forcing error, rc should be SQLITE_ERROR and cb should have been recorded
  bool ok = (rc == SQLITE_ERROR) &&
            (g_last_auth_cb == authCallback) &&
            (g_last_clientData == NULL);

  if (!ok) {
    log_failure(test_name, "Expected rc=SQLITE_ERROR with cb=authCallback and clientData=NULL");
    if (rc != SQLITE_ERROR) {
      std::cerr << "  rc=" << rc << std::endl;
    }
    if (g_last_auth_cb != authCallback) {
      std::cerr << "  callback mismatch." << std::endl;
    }
    if (g_last_clientData != NULL) {
      std::cerr << "  clientData mismatch." << std::endl;
    }
  }

  // Reset flag for cleanliness
  g_force_error = 0;
  ++g_total_tests;
  return ok;
}

// Entry point for running all tests
int main()
{
  std::cout << "Running sqlite3_showauth_init unit tests (self-contained harness)..." << std::endl;

  bool s1 = test_normal_path();
  bool s2 = test_null_db_path();
  bool s3 = test_non_null_pApi_path();
  bool s4 = test_force_error_path();

  int passed = (s1 ? 1 : 0) + (s2 ? 1 : 0) + (s3 ? 1 : 0) + (s4 ? 1 : 0);
  int total = g_total_tests;

  std::cout << "Test results: " << passed << "/" << total << " passed" << std::endl;
  if (g_failed_tests > 0) {
    std::cout << g_failed_tests << " test(s) failed." << std::endl;
  } else {
    std::cout << "All tests passed." << std::endl;
  }

  // Return non-zero if any test failed
  return g_failed_tests;
}