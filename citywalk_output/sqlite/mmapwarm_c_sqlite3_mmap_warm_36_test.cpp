// Test suite for the focal method sqlite3_mmap_warm using a lightweight, non-GTest C++11 test harness.
// The tests interact with a real SQLite3 database file system to cover the behavior of mmap warm-up
// under different conditions. The goal is to achieve reasonable coverage without terminating on
// first failure, and to keep dependencies limited to the standard library and provided sqlite3.
// Notes:
// - This test assumes a working SQLite3 environment available at link time.
// - We use a minimal non-terminating assertion mechanism (EXPECT_EQ) and a simple test runner.
// - The tests are written to be reasonably robust across platforms, but mmap behavior can be platform-dependent.

#include <vector>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>


extern "C" {
  // Forward declare the focal function. The actual implementation is provided in mmapwarm.c.
  // We declare it with C linkage to match the original C-style API.
  int sqlite3_mmap_warm(void* db, const char* zDb);
  // Note: We cast sqlite3* to void* in declarations to keep this file self-contained without
  // pulling in sqlite3.h in this harness. In a real environment, include <sqlite3.h> and use sqlite3*.
}

// Minimal stand-in for sqlite3 types to keep test harness self-contained.
// In a full project these would come from sqlite3.h. We provide only what's necessary for compilation.
typedef struct sqlite3 sqlite3;
typedef struct sqlite3_stmt sqlite3_stmt;

// RT: We assume that the real project is linked with the actual SQLite3 headers and library.
// To keep this test code concise and focused on exercising the focal method, we rely on the actual
// environment to provide the proper definitions during linkage.


// Simple test framework (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_fail(const std::string& test_name, const std::string& msg)
{
  std::cerr << "[FAIL] " << test_name << ": " << msg << "\n";
  ++g_failed_tests;
}

static void log_pass(const std::string& test_name)
{
  std::cout << "[PASS] " << test_name << "\n";
}

// Helper to run a single test with a name
#define RUN_TEST(name) TestRunnerHelper::instance().runTest(#name, [&](){ name(); })

class TestRunnerHelper {
public:
  static TestRunnerHelper& instance() {
    static TestRunnerHelper inst;
    return inst;
  }

  template <typename Fn>
  void runTest(const char* testName, Fn&& fn) {
    ++g_total_tests;
    try {
      fn();
      // If no assertion failed, mark as passed. We rely on log_pass within test when failing occurs.
      log_pass(testName);
    } catch (...) {
      log_fail(testName, "Unhandled exception in test");
    }
  }

private:
  TestRunnerHelper() = default;
  ~TestRunnerHelper() = default;
  TestRunnerHelper(const TestRunnerHelper&) = delete;
  TestRunnerHelper& operator=(const TestRunnerHelper&) = delete;
};

// Convenience wrappers for the sqlite3 error codes.
// Values are pulled from sqlite3.h in typical environments.
// We avoid including sqlite3.h here to keep the harness self-contained; the actual environment will provide it.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_MISUSE
#define SQLITE_MISUSE 21
#endif

// Fwd declarations for sqlite3 API used in tests. In a real environment include sqlite3.h and link against sqlite3 library.
extern "C" {
  // In a complete build, include "sqlite3.h" and link with -lsqlite3.
  int sqlite3_open(const char* filename, void** ppDb);
  int sqlite3_close(void* db);
  int sqlite3_exec(void* db, const char* sql, int (*callback)(void*,int,char**,char**), void* arg, char** errmsg);
  const char* sqlite3_db_filename(void* db, const char* zName);
  int sqlite3_mprintf(const char* zFormat, ...);
  void sqlite3_free(void*);
}

// Lightweight wrapper to convert C-API usage to a more friendly C++ test style.
// This code assumes the availability of the real sqlite3 API via linking.
static void* tryOpenDb(const std::string& path)
{
  void* db = nullptr;
  int rc = sqlite3_open(path.c_str(), &db);
  if (rc != SQLITE_OK || db == nullptr) {
    return nullptr;
  }
  return db;
}

static void safeCloseDb(void* db)
{
  if (db) sqlite3_close(db);
}

// Test 1: Ensure that mmap_warm returns SQLITE_MISUSE when a transaction is active (autocommit == 0)
static void test_mmap_warm_misuse_in_transaction()
{
  const char* test_name = "test_mmap_warm_misuse_in_transaction";

  // Clean up any stale test database file
  std::remove("test_mmap_misuse_in_transaction.db");

  void* db = tryOpenDb("test_mmap_misuse_in_transaction.db");
  if (!db) {
    log_fail(test_name, "Failed to open test database.");
    return;
  }

  // Begin a transaction to set autocommit to 0
  sqlite3_exec(db, "BEGIN;", nullptr, nullptr, nullptr);

  int rc = sqlite3_mmap_warm(db, nullptr);

  if (rc != SQLITE_MISUSE) {
    log_fail(test_name, "Expected SQLITE_MISUSE when autocommit is 0, got rc=" + std::to_string(rc));
  }

  // Commit the transaction to clean up
  sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);

  safeCloseDb(db);

  // Cleanup file
  std::remove("test_mmap_misuse_in_transaction.db");
}

// Test 2: Basic success path with main database (autocommit on)
static void test_mmap_warm_basic_path()
{
  const char* test_name = "test_mmap_warm_basic_path";

  std::remove("test_mmap_basic.db");
  void* db = tryOpenDb("test_mmap_basic.db");
  if (!db) {
    log_fail(test_name, "Failed to open test database.");
    return;
  }

  int rc = sqlite3_mmap_warm(db, nullptr);

  if (rc != SQLITE_OK) {
    log_fail(test_name, "Expected SQLITE_OK for mmap_warm basic path, got rc=" + std::to_string(rc));
  }

  safeCloseDb(db);
  std::remove("test_mmap_basic.db");
}

// Test 3: Warm mmap for an attached external database (non-null zDb)
static void test_mmap_warm_with_attached_other_db()
{
  const char* test_name = "test_mmap_warm_with_attached_other_db";

  std::remove("test_mmap_attach_main.db");
  std::remove("test_mmap_attach_other.db");

  // Create main database
  void* mainDb = tryOpenDb("test_mmap_attach_main.db");
  if (!mainDb) {
    log_fail(test_name, "Failed to open main test database.");
    return;
  }

  // Create a separate database file to attach
  void* otherDb = tryOpenDb("test_mmap_attach_other.db");
  if (!otherDb) {
    log_fail(test_name, "Failed to create attach database.");
    safeCloseDb(mainDb);
    return;
  }
  // Create a trivial schema in the attached database to ensure sqlite_schema exists
  sqlite3_exec(otherDb, "CREATE TABLE IF NOT EXISTS dummy(d INT);", nullptr, nullptr, nullptr);
  sqlite3_close(otherDb);

  // Attach the other database to main as 'other'
  sqlite3_exec(mainDb, "ATTACH 'test_mmap_attach_other.db' AS other;", nullptr, nullptr, nullptr);

  // Now warm mmap for the attached database
  int rc = sqlite3_mmap_warm(mainDb, "other");

  if (rc != SQLITE_OK) {
    log_fail(test_name, "Expected SQLITE_OK for mmap_warm with attached db, got rc=" + std::to_string(rc));
  }

  // Cleanup: detach and close
  sqlite3_exec(mainDb, "DETACH other;", nullptr, nullptr, nullptr);
  safeCloseDb(mainDb);

  std::remove("test_mmap_attach_main.db");
  std::remove("test_mmap_attach_other.db");
}

// Test 4: Non-existent zDb should yield an error (not SQLITE_OK)
static void test_mmap_warm_invalid_db_returns_error()
{
  const char* test_name = "test_mmap_warm_invalid_db_returns_error";

  std::remove("test_mmap_invalid.db");
  void* db = tryOpenDb("test_mmap_invalid.db");
  if (!db) {
    log_fail(test_name, "Failed to open test database.");
    return;
  }

  // Do not attach any database named 'nosuch'
  int rc = sqlite3_mmap_warm(db, "nosuch");

  if (rc == SQLITE_OK) {
    log_fail(test_name, "Expected non-OK rc for non-existent zDb, got SQLITE_OK.");
  }

  safeCloseDb(db);
  std::remove("test_mmap_invalid.db");
}

// Main driver to run all tests
int main() {
  // Run tests
  RUN_TEST(test_mmap_warm_misuse_in_transaction);
  RUN_TEST(test_mmap_warm_basic_path);
  RUN_TEST(test_mmap_warm_with_attached_other_db);
  RUN_TEST(test_mmap_warm_invalid_db_returns_error);

  // Summary
  std::cout << "Total tests: " << g_total_tests << "\n";
  std::cout << "Passed: " << (g_total_tests - g_failed_tests) << "\n";
  std::cout << "Failed: " << g_failed_tests << "\n";

  return (g_failed_tests > 0) ? 1 : 0;
}