// Test suite for sqlite3RegisterJsonFunctions (C/C++11, no GTest; plain asserts)
//
// This test harness uses the public SQLite C API to validate that the JSON
// functions registered by sqlite3RegisterJsonFunctions() behave as expected
// at runtime when JSON support is enabled in the build.
//
// Assumptions:
// - The project is built with JSON support (i.e., SQLITE_OMIT_JSON is not defined).
// - json.c (and its dependencies) are compiled and linked into the test binary.
// - sqlite3.h is available and the standard SQLite C API is usable from C++.
//
// Notes:
// - We test critical, observable behaviors by executing SQL statements that rely
//   on the registered JSON functions (e.g., json_valid, json_array_length).
// - We do not rely on GoogleTest/GMock; only standard C++11 and the SQLite API.
// - The tests are designed to be executable in a single main() and can be
//   extended easily with more cases if needed.
//
// How to run (example):
// g++ -std=c++11 -O2 -DSQLITE_OMIT_DEPRECATED=0 test_json_tests.cpp -lsqlite3 -o test_json_tests
// ./test_json_tests
//
// The test will:
// - Open an in-memory database
// - Call sqlite3RegisterJsonFunctions()
// - Execute a few representative queries that rely on the registered JSON
//   functions and verify the results via assertions

#include <cstdio>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cassert>


// SQLite C API
extern "C" {
  #include <sqlite3.h>
}

// Declaration of the focal function under test.
// It is defined in json.c (part of the project under test) and is expected to be
// linked into this test binary.
extern "C" void sqlite3RegisterJsonFunctions(void);

///////////////////////////////////////////////////////////////////////////////
// Helper wrapper: run a simple SQL query that returns an integer value.
// Returns true if the query succeeds and the first column equals 'expected'.
// This avoids depending on string formatting of JSON results and focuses on
// observable numeric outputs from JSON-related functions (e.g., json_valid, json_array_length).
///////////////////////////////////////////////////////////////////////////////
static bool runIntQuery(sqlite3* db, const char* sql, int expected) {
  sqlite3_stmt* stmt = nullptr;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
  if (rc != SQLITE_OK || !stmt) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << "\n";
    if (stmt) sqlite3_finalize(stmt);
    return false;
  }

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    std::cerr << "No row produced for query: " << sql << "\n";
    sqlite3_finalize(stmt);
    return false;
  }

  int val = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  // Compare numeric result
  if (val != expected) {
    std::cerr << "Unexpected result for query: " << sql
              << " got " << val << " expected " << expected << "\n";
    return false;
  }
  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Test 1: json_valid should return 1 for valid JSON
// This validates that the JSON function is registered and operates correctly.
///////////////////////////////////////////////////////////////////////////////
static bool test_json_valid_true(sqlite3* db) {
  // Ensure the JSON functions are registered first
  sqlite3RegisterJsonFunctions();

  // Valid JSON should yield 1 from json_valid(...)
  const char* sql = "SELECT json_valid('[1,2,3]')";
  return runIntQuery(db, sql, 1);
}

///////////////////////////////////////////////////////////////////////////////
// Test 2: json_valid should return 0 for invalid JSON
// This validates negative path coverage for the json_valid function.
///////////////////////////////////////////////////////////////////////////////
static bool test_json_valid_false(sqlite3* db) {
  // Ensure the JSON functions are registered first
  sqlite3RegisterJsonFunctions();

  // Invalid JSON should yield 0 from json_valid(...)
  const char* sql = "SELECT json_valid('not-json')";
  return runIntQuery(db, sql, 0);
}

///////////////////////////////////////////////////////////////////////////////
// Test 3: json_array_length should return the length of a valid JSON array
// We use a known array and verify the length is reported correctly.
///////////////////////////////////////////////////////////////////////////////
static bool test_json_array_length(sqlite3* db) {
  // Ensure the JSON functions are registered first
  sqlite3RegisterJsonFunctions();

  // json_array_length('[1,2,3]') should yield 3
  const char* sql = "SELECT json_array_length('[1,2,3]')";
  return runIntQuery(db, sql, 3);
}

///////////////////////////////////////////////////////////////////////////////
// Main test runner
// - Creates an in-memory database
// - Runs a suite of tests
// - Outputs a summary and returns appropriate exit code
///////////////////////////////////////////////////////////////////////////////
int main() {
  // If the JSON subsystem is omitted at compile time, skip JSON tests.
#ifndef SQLITE_OMIT_JSON
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || !db) {
    std::cerr << "Cannot open in-memory SQLite DB: " << sqlite3_errstr(rc) << "\n";
    return 1;
  }

  int failures = 0;

  // Run Test 1
  if (!test_json_valid_true(db)) {
    std::cerr << "Test 1 (json_valid true) failed.\n";
    ++failures;
  } else {
    std::cout << "Test 1 (json_valid true) passed.\n";
  }

  // Run Test 2
  if (!test_json_valid_false(db)) {
    std::cerr << "Test 2 (json_valid false) failed.\n";
    ++failures;
  } else {
    std::cout << "Test 2 (json_valid false) passed.\n";
  }

  // Run Test 3
  if (!test_json_array_length(db)) {
    std::cerr << "Test 3 (json_array_length) failed.\n";
    ++failures;
  } else {
    std::cout << "Test 3 (json_array_length) passed.\n";
  }

  sqlite3_close(db);

  if (failures == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << failures << " test(s) failed.\n";
    return 2;
  }
#else
  // If JSON support is omitted, clearly communicate the outcome.
  std::cerr << "SQLite was built with SQLITE_OMIT_JSON. JSON function tests are skipped.\n";
  return 0;
#endif
}

```