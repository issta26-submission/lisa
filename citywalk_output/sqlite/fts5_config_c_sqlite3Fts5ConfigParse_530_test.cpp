// Lightweight C++11 unit tests for the focal C function:
// sqlite3Fts5ConfigParse in fts5_config.c
// - No GTest; a small in-file test harness is provided.
// - Tests aim to exercise observable behavior (return codes, output structure),
//   without requiring private access to internal static helpers.
// - Tests rely on the project's SQLite dependencies and FTS5 types provided by the
//   project headers (e.g., Fts5Global, Fts5Config).
//
// Important: This test file assumes the build environment provides the SQLite3 headers
// and the project's FTS5 headers (e.g., fts5Int.h) so that the types and the
// symbol sqlite3Fts5ConfigParse are linkable. The tests call into the real function
// and verify its behavior via public outputs (rc, ppOut, zErr).

#include <functional>
#include <vector>
#include <fts5Int.h>
#include <string>
#include <cstring>
#include <iostream>


// Forward declarations to enable C linkage for the focal function.
// These declarations will allow C++ code to call into the C implementation.
extern "C" {
  #include <sqlite3.h>             // For sqlite3, SQLITE_OK, SQLITE_ERROR, SQLITE_NOMEM, etc.
  #include "fts5Int.h"             // Provides Fts5Global, Fts5Config, and related types.
}

// Declaration of the focal function (C linkage)
extern "C" int sqlite3Fts5ConfigParse(
  Fts5Global *pGlobal,
  sqlite3 *db,
  int nArg,
  const char **azArg,
  Fts5Config **ppOut,
  char **pzErr
);

// Declaration of a helper (likely a no-op in test scaffolding) to free config objects.
// We declare it with C linkage as used by the production code.
extern "C" void sqlite3Fts5ConfigFree(Fts5Config *pConfig);

// Simple helper to print test results
static void logResult(const std::string &name, bool ok, const std::string &msg)
{
  if(ok){
    std::cout << "[PASS] " << name << "\n";
  }else{
    std::cout << "[FAIL] " << name << " - " << msg << "\n";
  }
}

// Test 1: Basic parsing path with minimal required args.
// Expect SQLITE_OK and a non-null Fts5Config output with azArg[1] used as zDb.
bool test_basic_parse_ok(std::string &outMsg) {
  // Arrange
  Fts5Global global = {};     // Default-initialized; production code may fill fields, but
                                // for basic path, zero-initialization suffices.
  sqlite3 *db = nullptr;
  // azArg is an array of nArg strings. The focal function uses azArg[1] and azArg[2].
  const char *a[3];
  a[0] = "CREATE VIRTUAL TABLE dummy"; // not used by the parser beyond array sizing
  a[1] = "test_db";                   // zDb
  a[2] = "test_table";                // zName

  Fts5Config *pOut = nullptr;
  char *zErr = nullptr;

  // Act
  int rc = sqlite3Fts5ConfigParse(&global, db, 3, a, &pOut, &zErr);

  // Assert: rc must be SQLITE_OK, pOut must be allocated, and zDb must match input.
  if( rc != SQLITE_OK ){
    outMsg = "expected SQLITE_OK but got " + std::to_string(rc);
    if( zErr ){
      // Non-fatal: return details but keep test non-terminating
      // Free error string to avoid leak in test environment
      sqlite3_free(zErr);
      zErr = nullptr;
    }
    if( pOut ){
      sqlite3Fts5ConfigFree(pOut);
      pOut = nullptr;
    }
    return false;
  }
  if( pOut == nullptr ){
    outMsg = "pOut is NULL despite SQLITE_OK";
    if( zErr ) sqlite3_free(zErr);
    return false;
  }

  // Validate observable fields we expect to be set by the parser
  // zDb should be a copy of azArg[1]
  if( pOut->zDb == nullptr || std::strcmp(pOut->zDb, "test_db") != 0 ){
    outMsg = "zDb mismatch: expected 'test_db'";
    sqlite3Fts5ConfigFree(pOut);
    if( zErr ) sqlite3_free(zErr);
    return false;
  }

  // zContent should be filled with a default content expression
  if( pOut->zContent == 0 ){
    outMsg = "zContent was not initialized by parse";
    sqlite3Fts5ConfigFree(pOut);
    if( zErr ) sqlite3_free(zErr);
    return false;
  }

  // Cleanup
  sqlite3Fts5ConfigFree(pOut);
  if( zErr ) sqlite3_free(zErr);

  return true;
}

// Test 2: Reserved table name should trigger an error path.
// If FTS5_RANK_NAME is defined in the included headers, attempting to create a table with
// that reserved name should produce SQLITE_ERROR and a non-null error message.
bool test_reserved_name(std::string &outMsg) {
#if defined(FTS5_RANK_NAME)
  Fts5Global global = {};
  sqlite3 *db = nullptr;
  const char *a[3];
  a[0] = "CREATE VIRTUAL TABLE dummy";
  a[1] = "test_db_reserved";
  a[2] = FTS5_RANK_NAME; // reserved name

  Fts5Config *pOut = nullptr;
  char *zErr = nullptr;

  int rc = sqlite3Fts5ConfigParse(&global, db, 3, a, &pOut, &zErr);

  // Expect a parse error due to reserved name
  if( rc != SQLITE_ERROR ){
    outMsg = "expected SQLITE_ERROR for reserved name, got: " + std::to_string(rc);
    if( zErr ) sqlite3_free(zErr);
    if( pOut ) sqlite3Fts5ConfigFree(pOut);
    return false;
  }
  if( zErr == nullptr ){
    // The function should provide an error message
    outMsg = "expected non-null error message for reserved name";
    if( pOut ) sqlite3Fts5ConfigFree(pOut);
    return false;
  }

  // Cleanup
  if( zErr ) sqlite3_free(zErr);
  if( pOut ) sqlite3Fts5ConfigFree(pOut);

  return true;
#else
  // If the macro isn't defined in this environment, skip the test gracefully.
  outMsg = "FTS5_RANK_NAME not defined; skipping reserved-name test";
  return true;
#endif
}

int main() {
  std::vector<std::string> testNames;
  testNames.push_back("test_basic_parse_ok");
  testNames.push_back("test_reserved_name_reserved_macro");

  int total = 0;
  int passed = 0;

  // Test 1: Basic parse ok
  {
    std::string msg;
    bool ok = test_basic_parse_ok(msg);
    logResult("test_basic_parse_ok", ok, msg);
    total++;
    if(ok) passed++;
  }

  // Test 2: Reserved name handling (if macro exists)
  {
    std::string msg;
    bool ok = test_reserved_name(msg);
    logResult("test_reserved_name (reserved macro)", ok, msg);
    total++;
    if(ok) passed++;
  }

  // Summary
  std::cout << "Tests passed: " << passed << " / " << total << "\n";

  return (passed == total) ? 0 : 1;
}