// sqlite3_wasm_db_reset_unit_tests.cpp
// A lightweight C++11 test suite for the focal method sqlite3_wasm_db_reset
// This test suite does not rely on Google Test. It uses a small, non-terminating
// assertion approach and a minimal test runner to exercise the function.
// The tests assume the project is built as a single unit including sqlite3-wasm.c
// so that sqlite3_open, sqlite3_exec, sqlite3_db_config, sqlite3_table_column_metadata
// and sqlite3_wasm_db_reset symbols are available at link time.

#include <functional>
#include <vector>
#include <emscripten/wasmfs.h>
#include <assert.h>
#include <string>
#include <iostream>


// Forward-declare the minimal C API pieces used by the focal function and by tests.
// These prototypes should match the ones provided by the amalgamated sqlite3_wasm.c
// or the linked sqlite3.c portion in the project build.

// Opaque SQLite database handle
extern "C" {
  typedef struct sqlite3 sqlite3;

  // Basic database lifecycle
  int sqlite3_open(const char *filename, sqlite3 **ppDb);
  int sqlite3_close(sqlite3*);

  // Execute SQL without callback, for simple statements use NULL callback
  int sqlite3_exec(sqlite3*, const char *sql, int (*xCallback)(void*,int,char**,char**), void *pArg, char **errmsg);

  // Metadata fetcher (used by sqlite3_wasm_db_reset; parameters vary in amalgamation)
  int sqlite3_table_column_metadata(sqlite3 *db, const char *zDbName, int iTable,
                                    const char *zTable, const char *zColumn,
                                    const char *zDataType, int *pNotNull,
                                    int *pPrimaryKey, int *pAutoinc);

  // DB configuration (variadic)
  int sqlite3_db_config(sqlite3*, int op, ...);

  // The focal method under test
  int sqlite3_wasm_db_reset(sqlite3 *pDb);
}

// Lightweight non-terminating assertion helpers
static int g_testTotal = 0;
static int g_testPassed = 0;
static std::vector<std::string> g_failMessages;

#define TEST_ASSERT(cond, msg) do { \
    ++g_testTotal; \
    if(!(cond)) { \
        g_failMessages.push_back(std::string("ASSERTION FAILED: ") + (msg)); \
    } else { \
        ++g_testPassed; \
    } \
} while(0)

static void printTestSummary() {
  std::cout << "Unit test summary: " << g_testPassed << " passed, " 
            << g_testTotal - g_testPassed << " failed, total " << g_testTotal << ".\n";
  for(const auto &m : g_failMessages) {
    std::cout << "  - " << m << "\n";
  }
}

// Candidate Keywords (Step 1 analysis) for test planning
// - pDb: the sqlite3* database handle used by sqlite3_wasm_db_reset
// - sqlite3_table_column_metadata: called unconditionally when pDb != NULL
// - sqlite3_db_config: used to set RESET_DATABASE flag before VACUUM
// - VACUUM: executed via sqlite3_exec when reset flag was accepted
// - sqlite3_exec: used to execute VACUUM and later test additional queries
// - sqlite3_open / sqlite3_close: used to prepare a live sqlite3 database
// - SQLITE_MISUSE / return codes: verify behavior for NULL pointer input and success path
// - In-memory database: ":memory:" used to avoid filesystem side effects
// The tests will exercise the following branches at a high level:
// 1) pDb is NULL -> expect non-zero (SQLITE_MISUSE) return code
// 2) pDb is valid and RESET_DATABASE call succeeds (rc == 0) -> VACUUM is invoked and final rc should be 0
// 3) After reset, the database remains usable (can run a simple query)

// Test 1: Reset with a NULL pointer should return an error (non-zero rc)
static bool test_db_reset_with_null_pointer() {
  int rc = sqlite3_wasm_db_reset(nullptr);
  // Expect non-zero (SQLITE_MISUSE). Since the exact numeric value may depend on
  // the SQLite build, we check for non-zero to confirm misuse path.
  TEST_ASSERT(rc != 0, "sqlite3_wasm_db_reset(nullptr) should return non-zero (misuse)");
  return true;
}

// Test 2: Reset an in-memory database and verify it remains usable
static bool test_db_reset_in_memory_db() {
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  TEST_ASSERT(rc == 0 && db != nullptr, "Failed to open in-memory database for reset test");

  if(db == nullptr || rc != 0) {
    return false;
  }

  // Prepare a small schema to ensure VACUUM has something to touch, then use VACUUM path
  rc = sqlite3_exec(db,
                    "CREATE TABLE IF NOT EXISTS t(a INTEGER);"
                    "INSERT INTO t(a) VALUES (1),(2),(3);",
                    nullptr, 0, nullptr);
  TEST_ASSERT(rc == 0, "Failed to create test table and insert data before reset");

  // Call the focal function to reset the database
  rc = sqlite3_wasm_db_reset(db);
  TEST_ASSERT(rc == 0, "sqlite3_wasm_db_reset on in-memory DB should return 0 on success");

  // After reset, ensure the database is still usable by running a simple query
  int rowsReturned = 0;
  auto countCallback = [](void*, int, char**, char**) -> int {
    // We expect at least one row for SELECT 1
    ++rowsReturned;
    return 0;
  };

  rc = sqlite3_exec(db, "SELECT 1", countCallback, nullptr, nullptr);
  TEST_ASSERT(rc == 0, "sqlite3_exec after reset should succeed");
  TEST_ASSERT(rowsReturned >= 1, "Expected at least one row from SELECT 1 after reset");

  // Cleanup
  sqlite3_close(db);
  return true;
}

int main() {
  // Note: The test harness relies on the amalgamated sqlite3-wasm.c being part of the
  // build. The prototypes above are kept minimal to avoid header dependencies.

  // Domain-specific driver: run tests
  test_db_reset_with_null_pointer();
  test_db_reset_in_memory_db();

  // Print results
  printTestSummary();
  // Return non-zero if any tests failed
  return (g_testTotal == g_testPassed) ? 0 : 1;
}