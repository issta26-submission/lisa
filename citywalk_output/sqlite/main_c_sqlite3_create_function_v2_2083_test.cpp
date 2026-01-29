// Lightweight C++11 test suite for the focal method sqlite3_create_function_v2
// This test uses the public SQLite C API to exercise the wrapper introduced in
// main.c. No Google Test is used; tests are invoked from main().
// The tests are designed to be non-terminating on failure (non-fatal assertions)
// and print clear pass/fail results for each case.

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cassert>


extern "C" {
  #include "sqlite3.h"  // Ensure the project provides the SQLite public API header
}

// Global counter to verify that the destructor (xDestroy) is invoked
static int g_destructor_calls = 0;

// Destructor used for test 2 to verify that xDestroy is called when the DB is closed
static void test_udf_destructor(void* p) {
  (void)p; // p is not used for content here; we only track invocation count
  ++g_destructor_calls;
  // If p pointed to allocated memory, it would be freed here in a real scenario.
  // For safety in this minimal test, we don't attempt to dereference.
}

// Simple UDF used in test 1: returns the constant integer 42
static void test_udf_return_42(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
  (void)argc;
  (void)argv;
  sqlite3_result_int(ctx, 42);
}

/*
Test 1: Basic registration and invocation via SQL
- Purpose: Validate that sqlite3_create_function_v2 correctly registers a zero-argument
  UDF and that invoking the UDF from SQL returns the expected result.
- Approach:
  - Open an in-memory database.
  - Register a zero-argument function named "udf42" that returns 42.
  - Prepare and execute "SELECT udf42()".
  - Assert that the result is exactly 42.
- Why this tests the focal method:
  - sqlite3_create_function_v2 is exercised through a realistic usage path, including
    correct forwarding of parameters (zFunc, nArg, enc, p, xSFunc, xStep, xFinal, xDestroy).
  - It ensures that the wrapper integrates with the overall SQLite function registry.
*/
static bool test_basic_udf_v2_registration() {
  sqlite3* db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == nullptr) {
    printf("[Test 1] Failed to open in-memory DB: rc=%d\n", rc);
    return false;
  }

  // Register a zero-argument UDF named "udf42" that returns 42
  rc = sqlite3_create_function_v2(
      db,
      "udf42",      // zFunc
      0,            // nArg
      SQLITE_UTF8,  // enc
      nullptr,      // p
      test_udf_return_42, // xSFunc
      nullptr,             // xStep
      nullptr,             // xFinal
      nullptr              // xDestroy
  );
  if (rc != SQLITE_OK) {
    printf("[Test 1] sqlite3_create_function_v2 failed: rc=%d\n", rc);
    sqlite3_close(db);
    return false;
  }

  // Prepare and run: SELECT udf42()
  sqlite3_stmt* stmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT udf42()", -1, &stmt, nullptr);
  if (rc != SQLITE_OK || stmt == nullptr) {
    printf("[Test 1] Failed to prepare statement: rc=%d\n", rc);
    sqlite3_close(db);
    return false;
  }

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_ROW) {
    printf("[Test 1] Expected a row result, got rc=%d\n", rc);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
  }

  int value = sqlite3_column_int(stmt, 0);
  sqlite3_finalize(stmt);
  sqlite3_close(db);

  if (value != 42) {
    printf("[Test 1] Unexpected UDF result: expected 42, got %d\n", value);
    return false;
  }

  // Test passed
  printf("[Test 1] PASSED: Basic UDF registration and invocation\n");
  return true;
}

/*
Test 2: Destructor (xDestroy) invocation upon database close
- Purpose: Verify that the destructor provided via xDestroy is invoked when the
  database connection is closed, ensuring correct lifecycle handling of user data.
- Approach:
  - Open an in-memory database.
  - Allocate a non-null user data pointer (p) and pass it as the p parameter.
  - Provide a destructor function that increments a global counter when called.
  - Close the database and verify the destructor was invoked exactly once.
- Why this tests the focal method:
  - Confirms that sqlite3_create_function_v2 correctly forwards the xDestroy callback and p argument,
    and that the destructor is called on DB close, as per API contract.
*/
static bool test_udf_destructor_invoked_on_close() {
  g_destructor_calls = 0;
  sqlite3* db = nullptr;

  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == nullptr) {
    printf("[Test 2] Failed to open in-memory DB: rc=%d\n", rc);
    return false;
  }

  // Allocate non-null user data; destructor will clean it up
  int* userData = new int(0);

  // Register a zero-arg UDF with a destructor
  rc = sqlite3_create_function_v2(
      db,
      "udfDestroy",  // zFunc
      0,             // nArg
      SQLITE_UTF8,   // enc
      userData,      // p
      nullptr,       // xSFunc
      nullptr,       // xStep
      nullptr,       // xFinal
      test_udf_destructor // xDestroy
  );
  if (rc != SQLITE_OK) {
    printf("[Test 2] sqlite3_create_function_v2 failed: rc=%d\n", rc);
    sqlite3_close(db);
    delete userData; // Safety: in case destructor won't run
    return false;
  }

  // Optional invocation to ensure function is callable (even though we pass no xSFunc)
  sqlite3_stmt* stmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT udfDestroy()", -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    // If there is no implementation for xSFunc (NULL), this may still be fine since
    // we are only testing destructor timing on close; prepare may fail, but destructor
    // should still be invoked on close.
    // We'll continue to close to trigger destructor regardless.
    printf("[Test 2] Warning: prepare failed (rc=%d). Closing DB to test destructor.\n", rc);
  } else {
    rc = sqlite3_step(stmt);
    // If there is a row, simply finalize
    if (rc == SQLITE_ROW) {
      sqlite3_finalize(stmt);
    } else {
      sqlite3_finalize(stmt);
    }
  }

  // Close DB to trigger xDestroy
  sqlite3_close(db);

  // Destructor should have been called exactly once
  if (g_destructor_calls != 1) {
    printf("[Test 2] Destructor invocation count mismatch: expected 1, got %d\n", g_destructor_calls);
    return false;
  }

  printf("[Test 2] PASSED: Destructor (xDestroy) invoked on close\n");
  return true;
}

// Entry point: run all tests
int main() {
  int total = 2;
  int passed = 0;

  // Run Test 1: Basic UDF registration and invocation
  if (test_basic_udf_v2_registration()) {
    ++passed;
  }

  // Run Test 2: Destructor invocations on close
  if (test_udf_destructor_invoked_on_close()) {
    ++passed;
  }

  printf("Test summary: %d out of %d tests passed.\n", passed, total);

  return (passed == total) ? 0 : 1;
}