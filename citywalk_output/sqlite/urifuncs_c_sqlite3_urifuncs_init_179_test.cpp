// File: sqlite3ext.h
// Lightweight mock of necessary SQLite extension API declarations
// to enable unit testing of sqlite3_urifuncs_init without pulling the real SQLite library.

#ifndef SQLITE3EXT_H
#define SQLITE3EXT_H

// Core SQLite type placeholders (opaque in tests)
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;
typedef struct sqlite3_api_routines sqlite3_api_routines;

// SQLite result code and text encoding constants (mock values)
#define SQLITE_OK 0
#define SQLITE_UTF8 1

// Extension macro shim (no-op for unit tests)
#define SQLITE_EXTENSION_INIT2(pApi) (void)(pApi)
#define SQLITE_EXTENSION_INIT1 /* no-op in test environment */

// Minimal prototypes required by the focal code
int sqlite3_create_function(
  sqlite3 *db,
  const char *zFuncName,
  int nArg,
  int eTextRep,
  void *pApp,
  void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
  void (*xStep)(sqlite3_context*, int, sqlite3_value**),
  void (*xFinal)(sqlite3_context*, int, sqlite3_value**)
);

#endif // SQLITE3EXT_H

// File: urifuncs.c
// Focal method and dependencies (adapted for unit testing with a lightweight mock of SQLite API)

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <cstring>
#include <iostream>


// Static dummy implementations for the required function callbacks.
// In the real project these would implement the URI-related logic.
// Here we provide no-op bodies to enable linking and testing of registration behavior.

static void func_db_filename(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

static void func_uri_parameter(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

static void func_uri_boolean(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

static void func_uri_key(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

static void func_uri_int64(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

static void func_filename_database(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

static void func_filename_journal(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

static void func_filename_wal(
  sqlite3_context *context,
  int argc,
  sqlite3_value **argv
){
  (void)context; (void)argc; (void)argv;
}

int sqlite3_urifuncs_init(
  sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
){
{
  static const struct {
    const char *zFuncName;
    int nArg;
    void (*xFunc)(sqlite3_context*,int,sqlite3_value**);
  } aFunc[] = {
    { "sqlite3_db_filename",       1, func_db_filename       },
    { "sqlite3_uri_parameter",     2, func_uri_parameter     },
    { "sqlite3_uri_boolean",       3, func_uri_boolean       },
    { "sqlite3_uri_int64",         3, func_uri_int64         },
    { "sqlite3_uri_key",           2, func_uri_key           },
    { "sqlite3_filename_database", 1, func_filename_database },
    { "sqlite3_filename_journal",  1, func_filename_journal  },
    { "sqlite3_filename_wal",      1, func_filename_wal      },
  };
  int rc = SQLITE_OK;
  int i;
  SQLITE_EXTENSION_INIT2(pApi);
  (void)pzErrMsg;  /* Unused parameter */
  for(i=0; rc==SQLITE_OK && i<sizeof(aFunc)/sizeof(aFunc[0]); i++){
    rc = sqlite3_create_function(db, aFunc[i].zFuncName, aFunc[i].nArg,
                     SQLITE_UTF8, 0,
                     aFunc[i].xFunc, 0, 0);
  }
  return rc;
}
}

// File: test_urifuncs.cpp
// A lightweight C++11 test suite for sqlite3_urifuncs_init without GoogleTest.
// It uses a small in-file mock of the SQLite extension API to verify behavior.


// Include the focal code and its lightweight API mock.
// We provide a minimal sqlite3ext.h (above) and declare sqlite3_create_function for our mock.

extern "C" {
    // Forward declaration of the focal function (C linkage)
    int sqlite3_urifuncs_init(
      struct sqlite3 *db,
      char **pzErrMsg,
      const struct sqlite3_api_routines *pApi
    );
}

// They rely on a couple of opaque SQLite types. We'll declare them here to
// match the mock header's expectations for linkability.
struct sqlite3 {};
struct sqlite3_context {};
struct sqlite3_value {};
struct sqlite3_api_routines {};

// Mock implementation of sqlite3_create_function used by the focal code.
// This records calls and can be configured to return failure for testing
// propagation behavior.

static std::vector<std::string> g_reg_names;
static std::vector<int> g_reg_args;
static std::vector<int> g_reg_texts;

static std::vector<int> g_rc_sequence;
static size_t g_rc_pos = 0;

extern "C" int sqlite3_create_function(
    struct sqlite3 *db,
    const char *zFuncName,
    int nArg,
    int eTextRep,
    void *pApp,
    void (*xFunc)(struct sqlite3_context*,int,struct sqlite3_value**),
    void (*xStep)(struct sqlite3_context*,int,struct sqlite3_value**),
    void (*xFinal)(struct sqlite3_context*,int,struct sqlite3_value**)
){
    (void)db; (void)pApp; (void)xFunc; (void)xStep; (void)xFinal;
    g_reg_names.emplace_back(zFuncName ? zFuncName : "");
    g_reg_args.push_back(nArg);
    g_reg_texts.push_back(eTextRep);

    int rc = 0;
    if(g_rc_pos < g_rc_sequence.size()) {
        rc = g_rc_sequence[g_rc_pos++];
    }
    // Default to SQLITE_OK (0) if not configured
    return rc;
}

// Lightweight assertion helper that does not terminate on failure.
// It records failures and prints a message for debugging.
static int g_failures = 0;

#define ASSERT(cond, msg) do { \
  if(!(cond)) { \
    ++g_failures; \
    std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
  } \
} while(0)

static void reset_logs() {
  g_reg_names.clear();
  g_reg_args.clear();
  g_reg_texts.clear();
  g_rc_sequence.clear();
  g_rc_pos = 0;
}

static void test_all_functions_registered_success() {
  // Setup: all registrations succeed
  reset_logs();
  g_rc_sequence = {0,0,0,0,0,0,0,0};
  g_rc_pos = 0;

  struct sqlite3 *db = reinterpret_cast<struct sqlite3*>(0x1);
  char *errmsg = nullptr;
  const struct sqlite3_api_routines *pApi = nullptr;

  int rc = sqlite3_urifuncs_init(db, &errmsg, pApi);

  // Verify: return is OK and exactly 8 functions were registered with expected signatures
  ASSERT(rc == 0, "sqlite3_urifuncs_init should return SQLITE_OK when all registrations succeed");
  ASSERT(g_reg_names.size() == 8, "Should register 8 URI-function helpers");

  const char *expected_names[8] = {
    "sqlite3_db_filename",
    "sqlite3_uri_parameter",
    "sqlite3_uri_boolean",
    "sqlite3_uri_int64",
    "sqlite3_uri_key",
    "sqlite3_filename_database",
    "sqlite3_filename_journal",
    "sqlite3_filename_wal"
  };
  for (size_t i = 0; i < 8; ++i) {
    ASSERT(g_reg_names[i] == expected_names[i], "Registered function name mismatch at index " + std::to_string(i));
  }

  // Args and encoding checks
  const int expected_args[8] = {1,2,3,3,2,1,1,1};
  for (size_t i = 0; i < 8; ++i) {
    ASSERT(g_reg_args[i] == expected_args[i], "Arg count mismatch for " + std::string(g_reg_names[i]));
  }
  for (size_t i = 0; i < 8; ++i) {
    ASSERT(g_reg_texts[i] == SQLITE_UTF8, "Text encoding should be SQLITE_UTF8 for " + std::string(g_reg_names[i]));
  }
}

static void test_urifuncs_init_stops_on_failure_at_4th() {
  // Setup: fail on the 4th registration
  reset_logs();
  g_rc_sequence = {0, 0, 0, 5}; // fail on 4th call
  g_rc_pos = 0;

  struct sqlite3 *db = reinterpret_cast<struct sqlite3*>(0x2);
  char *errmsg = nullptr;
  const struct sqlite3_api_routines *pApi = nullptr;

  int rc = sqlite3_urifuncs_init(db, &errmsg, pApi);

  // Verify: should stop at 4th call and return the failure code (5)
  ASSERT(rc == 5, "sqlite3_urifuncs_init should propagate the non-OK rc from 4th registration");
  ASSERT(g_reg_names.size() == 4, "Should have attempted 4 registrations before failure");
}

static void test_urifuncs_init_stops_on_failure_next_call() {
  // Setup: fail on the 2nd registration
  reset_logs();
  g_rc_sequence = {0, 42}; // fail on 2nd call
  g_rc_pos = 0;

  struct sqlite3 *db = reinterpret_cast<struct sqlite3*>(0x3);
  char *errmsg = nullptr;
  const struct sqlite3_api_routines *pApi = nullptr;

  int rc = sqlite3_urifuncs_init(db, &errmsg, pApi);

  // Verify: should stop after 2nd and return 42
  ASSERT(rc == 42, "sqlite3_urifuncs_init should propagate the non-OK rc from 2nd registration");
  ASSERT(g_reg_names.size() == 2, "Should have attempted 2 registrations before failure");
}

int main() {
  test_all_functions_registered_success();
  test_urifuncs_init_stops_on_failure_at_4th();
  test_urifuncs_init_stops_on_failure_next_call();

  if (g_failures == 0) {
    std::cout << "All unit tests passed." << std::endl;
    return 0;
  } else {
    std::cerr << g_failures << " unit test(s) failed." << std::endl;
    return 1;
  }
}