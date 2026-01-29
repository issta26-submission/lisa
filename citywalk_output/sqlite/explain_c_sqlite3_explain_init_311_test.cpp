// File: explain_test_with_vt.cpp
// This test validates sqlite3_explain_init when the virtual table path is included
// (i.e., SQLITE_OMIT_VIRTUALTABLE is not defined). It mocks the sqlite3ExplainVtabInit
// function to verify that sqlite3_explain_init forwards the call and returns its value.

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3ext.h>
#include <cassert>


// Domain-specific: minimal stand-in types to compile the test without SQLite headers.
struct sqlite3 {};
struct sqlite3_api_routines {};

// SQLite-like constants and macros (minimal subset for testing)
#define SQLITE_OK 0
#define SQLITE_EXTENSION_INIT2(pApi) do { (void)(pApi); } while(0)

// Forward declaration of the focal method under test (as it appears in explain.c)
int sqlite3_explain_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);

// Provide the actual implementation identical to the focal method.
// This self-contained reproduction is used to test the logic in isolation.
int sqlite3_explain_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3ExplainVtabInit(db);
#endif
  return rc;
}

// Global counter to verify that sqlite3ExplainVtabInit gets invoked exactly once.
static int g_vtabInit_call_count = 0;

// Mock implementation of the dependent function sqlite3ExplainVtabInit.
// In the real extension, this would perform initialization of a virtual table.
// Here we simulate a successful initialization returning a known code (5) and count calls.
int sqlite3ExplainVtabInit(sqlite3 *db){
  (void)db; // unused in test
  ++g_vtabInit_call_count;
  return 5; // sentinel value to verify propagation
}

int main() {
  // Test: ensure that when virtual table support is enabled, sqlite3_explain_init
  // calls sqlite3ExplainVtabInit and propagates its return code.
  {
    g_vtabInit_call_count = 0;
    sqlite3 dummyDb;
    char* err = nullptr;
    int rc = sqlite3_explain_init(&dummyDb, &err, nullptr);

    // Expected: sqlite3ExplainVtabInit invoked, rc equals its return value (5).
    if (rc != 5) {
      printf("Test with VT enabled: unexpected rc %d (expected 5)\n", rc);
      return 1;
    }
    if (g_vtabInit_call_count != 1) {
      printf("Test with VT enabled: sqlite3ExplainVtabInit called %d times (expected 1)\n",
             g_vtabInit_call_count);
      return 1;
    }
    printf("Test with VT enabled passed: rc=%d, calls=%d\n", rc, g_vtabInit_call_count);
  }

  // Successful completion
  return 0;
}
  

// File: explain_test_without_vt.cpp
// This test validates sqlite3_explain_init when virtual table support is omitted
// (i.e., SQLITE_OMIT_VIRTUALTABLE is defined). The code path should not call
// sqlite3ExplainVtabInit and should return SQLITE_OK.


// Domain-specific minimal stand-ins
struct sqlite3 {};
struct sqlite3_api_routines {};

// Define the macro to omit the virtual table path so the code under test does not call sqlite3ExplainVtabInit
#define SQLITE_OMIT_VIRTUALTABLE
#define SQLITE_OK 0
#define SQLITE_EXTENSION_INIT2(pApi) do { (void)(pApi); } while(0)

// Re-declare the focal function (self-contained version for testing)
int sqlite3_explain_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);

int sqlite3_explain_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
){
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
#ifndef SQLITE_OMIT_VIRTUALTABLE
  rc = sqlite3ExplainVtabInit(db);
#endif
  return rc;
}

// Mock for sqlite3ExplainVtabInit to ensure it is not called when VT is omitted.
static int g_vtabInit_call_count = 0;
int sqlite3ExplainVtabInit(sqlite3 *db){
  (void)db;
  ++g_vtabInit_call_count;
  return 5;
}

int main() {
  // Test: ensure that when VT support is omitted, sqlite3_explain_init does not call sqlite3ExplainVtabInit
  // and returns SQLITE_OK regardless.
  {
    g_vtabInit_call_count = 0;
    sqlite3 dummyDb;
    char* err = nullptr;
    int rc = sqlite3_explain_init(&dummyDb, &err, nullptr);

    // Expected: rc is SQLITE_OK (0) and sqlite3ExplainVtabInit should not be called.
    if (rc != 0) {
      printf("Test VT omitted: unexpected rc %d (expected 0)\n", rc);
      return 1;
    }
    if (g_vtabInit_call_count != 0) {
      printf("Test VT omitted: sqlite3ExplainVtabInit called %d times (expected 0)\n",
             g_vtabInit_call_count);
      return 1;
    }
    printf("Test VT omitted passed: rc=%d, calls=%d\n", rc, g_vtabInit_call_count);
  }

  // Successful completion
  return 0;
}