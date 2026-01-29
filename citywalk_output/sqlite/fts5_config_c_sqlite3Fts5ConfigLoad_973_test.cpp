// Unit test suite for sqlite3Fts5ConfigLoad
// Target: validate behavior of FTS5 config loader using a real SQLite in-memory DB.
// Notes:
// - This test uses the actual SQLite library and the FTS5 config APIs.
// - It assumes the project provides the Fts5Config type and related APIs in headers
//   (e.g., fts5Int.h). It compiles under C++11 as a standalone test runner (no GTest).
// - We run a few scenarios: default/ok path (current version), secure-delete version path,
//   and an invalid version path that should produce an error message.
// - Each test prints its result and includes comments explaining intent.

#include <vector>
#include <fts5Int.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cassert>


// Include the FTS5 internal config declarations.
// These headers are part of the project and provide:
 // - typedef struct Fts5Config Fts5Config;
 // - macros like FTS5_CURRENT_VERSION, FTS5_CURRENT_VERSION_SECUREDELETE
 // - the function: int sqlite3Fts5ConfigLoad(Fts5Config*, int)

// Ensure the FTS5 config loader is accessible with C linkage
extern "C" {
  int sqlite3Fts5ConfigLoad(Fts5Config *pConfig, int iCookie);
}

// Simple test harness helpers
static void fail_with_message(const std::string &msg) {
  std::cerr << "[FAILED] " << msg << std::endl;
}

static void pass_with_message(const std::string &msg) {
  std::cout << "[PASSED] " << msg << std::endl;
}

// Helper to execute an SQL command and report errors
static bool exec_sql(sqlite3* db, const std::string& sql) {
  char *err = nullptr;
  int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &err);
  if( rc != SQLITE_OK ) {
    std::cerr << "SQL error: " << (err ? err : "unknown") << std::endl;
    if( err ) sqlite3_free(err);
    return false;
  }
  if( err ) sqlite3_free(err);
  return true;
}

// Test 1: Load with current version, ensure ok and iCookie is set, defaults are applied
static void test_load_with_current_version_sets_defaults_and_cookie() {
  sqlite3 *db = nullptr;
  int rc_open = sqlite3_open(":memory:", &db);
  if( rc_open != SQLITE_OK || !db ) {
    fail_with_message("Could not open in-memory SQLite DB for Test 1");
    if( db ) sqlite3_close(db);
    return;
  }

  // Create the target config table: main.'test_config' with columns k (TEXT), v (INTEGER)
  // The SQL in sqlite3Fts5ConfigLoad uses: SELECT k, v FROM %Q.'%q_config'
  // We attach content into main.'test_config'
  if(!exec_sql(db, "CREATE TABLE IF NOT EXISTS 'main'.'test_config'(k TEXT, v INTEGER);")) {
    fail_with_message("Test 1: failed to create test_config table");
    sqlite3_close(db);
    return;
  }

  // Insert version value equal to FTS5_CURRENT_VERSION
  // Use string formatting to insert the macro value
  std::string sql_insert = "INSERT INTO 'main'.'test_config'(k, v) VALUES ('version', " +
                           std::to_string(FTS5_CURRENT_VERSION) + ");";
  if(!exec_sql(db, sql_insert)) {
    fail_with_message("Test 1: failed to insert version row");
    sqlite3_close(db);
    return;
  }

  // Prepare Fts5Config structure and call loader
  Fts5Config cfg;
  std::memset(&cfg, 0, sizeof(cfg));
  cfg.db = db;
  cfg.zDb = "main";
  cfg.zName = "test";
  char *err = NULL;
  cfg.pzErrmsg = &err;

  int cookie = 42;
  int rc = sqlite3Fts5ConfigLoad(&cfg, cookie);

  bool ok = (rc == SQLITE_OK) &&
            (cfg.iCookie == cookie) &&
            (cfg.iVersion == FTS5_CURRENT_VERSION);

  if( ok ) {
    pass_with_message("Test 1: load with current version succeeded and cookie/version set correctly.");
  } else {
    fail_with_message("Test 1: load with current version failed. See logs for details.");
  }

  // Cleanup
  if( err ) sqlite3_free(err);
  sqlite3_close(db);
}

// Test 2: Version is invalid (not current or secure-delete) -> should set error and produce message
static void test_load_with_invalid_version_produces_error_and_message() {
  sqlite3 *db = nullptr;
  int rc_open = sqlite3_open(":memory:", &db);
  if( rc_open != SQLITE_OK || !db ) {
    fail_with_message("Could not open in-memory SQLite DB for Test 2");
    if( db ) sqlite3_close(db);
    return;
  }

  if(!exec_sql(db, "CREATE TABLE IF NOT EXISTS 'main'.'test_config'(k TEXT, v INTEGER);")) {
    fail_with_message("Test 2: failed to create test_config table");
    sqlite3_close(db);
    return;
  }

  // Insert an invalid version (e.g., 9999)
  std::string sql_insert = "INSERT INTO 'main'.'test_config'(k, v) VALUES ('version', 9999);";
  if(!exec_sql(db, sql_insert)) {
    fail_with_message("Test 2: failed to insert invalid version row");
    sqlite3_close(db);
    return;
  }

  Fts5Config cfg;
  std::memset(&cfg, 0, sizeof(cfg));
  cfg.db = db;
  cfg.zDb = "main";
  cfg.zName = "test";
  char *err = NULL;
  cfg.pzErrmsg = &err;

  int rc = sqlite3Fts5ConfigLoad(&cfg, 7);

  bool ok = (rc == SQLITE_ERROR) && (err != NULL);
  if( ok ) {
    // Verify the error message mentions invalid fts5 file format and the found value
    std::string msg(err);
    bool contains_expected = (msg.find("invalid fts5 file format") != std::string::npos) &&
                           (msg.find("found 9999") != std::string::npos);
    if(contains_expected) {
      pass_with_message("Test 2: invalid version correctly produced error message with expected content.");
    } else {
      fail_with_message("Test 2: error message did not contain expected substrings. Message:" + msg);
    }
  } else {
    fail_with_message("Test 2: loader did not return SQLITE_ERROR for invalid version.");
  }

  if( err ) sqlite3_free(err);
  sqlite3_close(db);
}

// Test 3: Version is secure-delete current (FTS5_CURRENT_VERSION_SECUREDELETE) -> ok path, version set
static void test_load_with_securedelete_version_ok() {
  sqlite3 *db = nullptr;
  int rc_open = sqlite3_open(":memory:", &db);
  if( rc_open != SQLITE_OK || !db ) {
    fail_with_message("Could not open in-memory SQLite DB for Test 3");
    if( db ) sqlite3_close(db);
    return;
  }

  if(!exec_sql(db, "CREATE TABLE IF NOT EXISTS 'main'.'test_config'(k TEXT, v INTEGER);")) {
    fail_with_message("Test 3: failed to create test_config table");
    sqlite3_close(db);
    return;
  }

  // Insert a secure-delete version
  std::string sql_insert = "INSERT INTO 'main'.'test_config'(k, v) VALUES ('version', " +
                           std::to_string(FTS5_CURRENT_VERSION_SECUREDELETE) + ");";
  if(!exec_sql(db, sql_insert)) {
    fail_with_message("Test 3: failed to insert secure-delete version row");
    sqlite3_close(db);
    return;
  }

  Fts5Config cfg;
  std::memset(&cfg, 0, sizeof(cfg));
  cfg.db = db;
  cfg.zDb = "main";
  cfg.zName = "test";
  char *err = NULL;
  cfg.pzErrmsg = &err;

  int rc = sqlite3Fts5ConfigLoad(&cfg, 99);

  bool ok = (rc == SQLITE_OK) && (cfg.iVersion == FTS5_CURRENT_VERSION_SECUREDELETE);
  if( ok ) {
    pass_with_message("Test 3: load with secure-delete version succeeded and iVersion set accordingly.");
  } else {
    fail_with_message("Test 3: load with secure-delete version failed. rc=" + std::to_string(rc) +
                      ", iVersion=" + std::to_string(cfg.iVersion));
  }

  if( err ) sqlite3_free(err);
  sqlite3_close(db);
}

// Main: run all tests
int main() {
  std::cout << "Running sqlite3Fts5ConfigLoad unit tests (C++11, no GTest) ..." << std::endl;

  test_load_with_current_version_sets_defaults_and_cookie();
  test_load_with_invalid_version_produces_error_and_message();
  test_load_with_securedelete_version_ok();

  std::cout << "Unit tests completed." << std::endl;
  return 0;
}