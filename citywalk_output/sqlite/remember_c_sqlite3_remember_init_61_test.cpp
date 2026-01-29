// C++11 Test Suite for sqlite3_remember_init (remember.c)
// Notes:
// - This test suite assumes remember.c (the focal file) is compiled together with this test.
// - The tests rely on SQLite's in-memory database and the public C API.
// - No Google Test is used; a lightweight, non-terminating assertion macro is provided.
// - The tests exercise basic initialization, SQL invocation of the registered function, and behavior on closed DB.
// - Explanatory comments describe the intent and coverage of each unit test.

#include <assert.h>
#include <sqlite3ext.h>
#include <string>
#include <iostream>


// Forward declaration of the SQLite API routine struct to keep tests independent of internal headers.
// We only pass NULL in tests where the API table is not required.
typedef struct sqlite3_api_routines sqlite3_api_routines;

// Forward declaration of the focal function from remember.c.
// We declare it with C linkage to ensure correct linking when compiled in C++.
extern "C" int sqlite3_remember_init(
  class sqlite3 *db, 
  char **pzErrMsg, 
  const sqlite3_api_routines *pApi
);

// Forward declaration of the opaque SQLite types used in tests.
struct sqlite3;
struct sqlite3_stmt;

// Lightweight non-terminating test framework
static int g_failures = 0;

// EXPECT_TRUE: non-terminating assertion; records failures but continues execution.
#define EXPECT_TRUE(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE FAILED: " << msg << " (line " << __LINE__ << ")" << std::endl; \
    ++g_failures; \
  } \
} while(false)

// Helper macro for interface sanity
#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, msg)

// Test 1: Basic initialization and function registration.
// - Create an in-memory database.
// - Call sqlite3_remember_init to register the "remember" function.
// - Verify the initialization returns SQLITE_OK.
// - Verify that the registered function can be invoked from SQL and returns NULL (since rememberFunc has no body).
// - Clean up resources.
void test_RememberInit_BasicRegistration() {
  std::cout << "Running test_RememberInit_BasicRegistration..." << std::endl;

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr, "sqlite3_open should create in-memory DB");

  if (db == nullptr) {
    // Cannot proceed further
    return;
  }

  // Call the focal initialization function with a NULL pApi (valid for this test).
  char *errMsg = nullptr;
  rc = sqlite3_remember_init(db, &errMsg, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_remember_init should return SQLITE_OK on valid DB");

  if (errMsg != nullptr) {
    std::cerr << "Warning: sqlite3_remember_init reported error: " << errMsg << std::endl;
    sqlite3_free(errMsg);
  }

  // Prepare a SQL statement that uses the "remember" function registered by the init.
  sqlite3_stmt *stmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT remember('a','b');", -1, &stmt, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK, "prepare should succeed for SELECT remember(...)");

  rc = sqlite3_step(stmt);
  EXPECT_TRUE(rc == SQLITE_ROW, "submit should yield a single row result");

  // The remember function body is empty; SQLite should return NULL as the result.
  int type = sqlite3_column_type(stmt, 0);
  EXPECT_TRUE(type == SQLITE_NULL, "remember(...) should return NULL (no result)");

  rc = sqlite3_finalize(stmt);
  (void)rc; // suppress unused warning in release builds

  rc = sqlite3_close(db);
  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_close should succeed");
}

// Test 2: Initialization on a closed/destroyed DB should fail gracefully (non-OK return).
// - Open a DB, initialize, then close.
// - Attempt to initialize again on the closed DB and verify a non-OK return code is produced.
void test_RememberInit_OnClosedDb() {
  std::cout << "Running test_RememberInit_OnClosedDb..." << std::endl;

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr, "sqlite3_open should create in-memory DB");

  if (db == nullptr) {
    return;
  }

  // First initialization on a valid DB
  rc = sqlite3_remember_init(db, nullptr, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK, "Initial sqlite3_remember_init should succeed on valid DB");

  // Close the DB
  rc = sqlite3_close(db);
  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_close should succeed");

  // Attempt to initialize again on a closed DB
  rc = sqlite3_remember_init(db, nullptr, nullptr);
  // We do not assume a specific non-OK error code (depends on SQLite state), only that it's not SQLITE_OK
  EXPECT_TRUE(rc != SQLITE_OK, "sqlite3_remember_init should fail on closed/invalid DB (non-OK)");
}

// Test 3: Initialization with NULL pApi (domain knowledge requirement to test API integration flexibility).
// - Open DB and call sqlite3_remember_init with NULL pApi (valid path for extensions that only require pApi when needed).
// - Ensure it returns SQLITE_OK and that the function can be invoked via SQL as in Test 1.
void test_RememberInit_WithNullPApi() {
  std::cout << "Running test_RememberInit_WithNullPApi..." << std::endl;

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr, "sqlite3_open should create in-memory DB");

  if (db == nullptr) {
    return;
  }

  // Initialize with NULL pApi
  rc = sqlite3_remember_init(db, nullptr, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK, "sqlite3_remember_init should succeed with NULL pApi");

  // Validate function usage as in Test 1
  sqlite3_stmt *stmt = nullptr;
  rc = sqlite3_prepare_v2(db, "SELECT remember('x','y');", -1, &stmt, nullptr);
  EXPECT_TRUE(rc == SQLITE_OK, "prepare should succeed for SELECT remember(...) with NULL pApi");
  rc = sqlite3_step(stmt);
  EXPECT_TRUE(rc == SQLITE_ROW, "step should yield a row for remember(...)");

  int type = sqlite3_column_type(stmt, 0);
  EXPECT_TRUE(type == SQLITE_NULL, "remember(...) should return NULL even with NULL pApi");

  (void)sqlite3_finalize(stmt);
  (void)sqlite3_close(db);
}

// Main function: runs all tests and reports a final result.
// Following domain knowledge: "If gtest isn't allowed, call test methods from the main function."
int main() {
  std::cout << "Starting sqlite3_remember_init test suite (C++11, non-GTest)...\n" << std::endl;

  // Run tests
  test_RememberInit_BasicRegistration();
  test_RememberInit_OnClosedDb();
  test_RememberInit_WithNullPApi();

  // Summary
  if (g_failures == 0) {
    std::cout << "\nALL TESTS PASSED" << std::endl;
  } else {
    std::cout << "\nTESTS FAILED: " << g_failures << " failure(s) detected" << std::endl;
  }

  // Return non-zero on failure to integrate with CI expectations
  return g_failures;
}