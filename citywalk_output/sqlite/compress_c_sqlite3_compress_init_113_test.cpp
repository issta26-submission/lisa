// Test suite for sqlite3_compress_init (C function) without using GTest.
// This test is written for C++11, uses the SQLite3 C API, and provides its own
// lightweight non-terminating assertion mechanism (EXPECT_* macros).

#include <sqlite3.h>
#include <iostream>
#include <sqlite3ext.h>
#include <zlib.h>


// Forward declaration of the focal function under test (C linkage)
extern "C" int sqlite3_compress_init(sqlite3 *db,
                                   char **pzErrMsg,
                                   const sqlite3_api_routines *pApi);

// A minimal dummy function to register as a pre-existing function in the DB.
// This is used to force a failure path in sqlite3_compress_init by creating a
// duplicate function named "compress" with the same arity.
static void dummyFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
  (void)ctx;
  (void)argc;
  (void)argv;
}

// Simple non-terminating assertion mechanism.
// These are intentionally non-fatal to maximize code coverage during testing.
static int g_test_total = 0;
static int g_test_failed = 0;

#define EXPECT_TRUE(cond) \
  do { \
    ++g_test_total; \
    if(!(cond)) { \
      ++g_test_failed; \
      std::cerr << "[EXPECT_TRUE] FAILED at " << __FILE__ << ":" << __LINE__ \
                << " - " #cond << std::endl; \
    } \
  } while(0)


// Step 1: Program Understanding and Candidate Keywords (documentation for test design)
// Candidate Keywords extracted from the focal method and its dependencies:
// - sqlite3_compress_init: initializes/loading of compression extension (compress/uncompress)
// - sqlite3_create_function: registers SQL-callable extension functions
// - compressFunc / uncompressFunc: the actual function callbacks (static in file)
// - SQLITE_UTF8, SQLITE_INNOCUOUS, SQLITE_DETERMINISTIC: function attributes
// - pApi and SQLITE_EXTENSION_INIT2: macro usage to initialize extension API pointers
// - rc / SQLITE_OK: return code flow control
// Step 2: Unit Test Generation uses:
// - True path: both functions registered successfully (expect SQLITE_OK)
// - False path: first registration (compress) fails due to naming conflict
// Step 3: Test Case Refinement leverages:
// - In-memory DB (":memory:") for isolation
// - Pre-registration of a function named "compress" to simulate collision
// - Non-terminating assertions via EXPECT_TRUE
// - No use of private members; only public SQLite API is invoked


// Test 1: True path - No pre-existing function named "compress".
// Expected: sqlite3_compress_init returns SQLITE_OK and registers both "compress" and "uncompress".
static void test_compress_init_success() {
  sqlite3 *db = nullptr;
  int rc = SQLITE_OK;
  char *errMsg = nullptr;

  // Open an in-memory database for isolated testing
  rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr);
  EXPECT_TRUE(rc == SQLITE_OK);

  // Ensure no pre-existing function named "compress" exists in this DB
  // (We simply proceed; if a function exists, this test would fail at init)

  // Call the focal function under test with a NULL extension API pointer.
  rc = sqlite3_compress_init(db, &errMsg, nullptr);

  // Expect the init to succeed (SQLITE_OK)
  EXPECT_TRUE(rc == SQLITE_OK);

  // Cleanup
  if (db) sqlite3_close(db);
  (void)errMsg; // Unused in this test; provided for API compatibility
}


// Test 2: False path - Pre-register a function named "compress" to simulate a
// collision that causes the first registration to fail.
// Expected: sqlite3_compress_init returns a non-SQLITE_OK code (likely SQLITE_ERROR).
static void test_compress_init_duplicate_preexisting() {
  sqlite3 *db = nullptr;
  int rc = SQLITE_OK;
  char *errMsg = nullptr;

  // Open an in-memory database
  rc = sqlite3_open(":memory:", &db);
  EXPECT_TRUE(db != nullptr);
  EXPECT_TRUE(rc == SQLITE_OK);

  // Pre-register a dummy function named "compress" to force a collision
  rc = sqlite3_create_function(db, "compress", 1,
                               SQLITE_UTF8 | SQLITE_INNOCUOUS,
                               0, dummyFunc, 0, 0);
  EXPECT_TRUE(rc == SQLITE_OK);

  // Now invoke the focal method; it should fail to create "compress" again
  rc = sqlite3_compress_init(db, &errMsg, nullptr);

  // Expect a non-OK return code due to the pre-existing function
  EXPECT_TRUE(rc != SQLITE_OK);

  // Cleanup
  if (db) sqlite3_close(db);
  (void)errMsg; // Unused in this test; provided for API compatibility
}


// Main function to drive tests without a test framework.
// Executes tests sequentially and reports a final summary.
int main() {
  std::cout << "Starting sqlite3_compress_init unit tests (C++11, non-GTest)" << std::endl;

  test_compress_init_success();
  test_compress_init_duplicate_preexisting();

  if (g_test_failed == 0) {
    std::cout << "[PASSED] All tests passed (" << g_test_total << " tests)." << std::endl;
    return 0;
  } else {
    std::cerr << "[FAILED] " << g_test_failed << " tests failed out of "
              << g_test_total << "." << std::endl;
    return 1;
  }
}