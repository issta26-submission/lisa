// Test suite for sqlite3_intarray_create focusing on the focal method
// This test suite is designed to compile under C++11 without Google Test.
// It relies on a real SQLite3 library being present and linked with the test.
// The tests are written with a lightweight, non-terminating assertion mechanism
// (EXPECT_* macros) to maximize code coverage while continuing test execution.
//
// Step 1 (understanding): 
// - The focal method sqlite3_intarray_create allocates a sqlite3_intarray, stores it in *ppReturn,
//   registers a module via sqlite3_create_module_v2 using intarrayModule and intarrayFree,
//   and on success executes a CREATE VIRTUAL TABLE statement for a temp schema using the given name.
// - It returns SQLITE_OK on success, SQLITE_NOMEM if allocation fails, or another sqlite error code otherwise.
// - The body of the function is guarded by #ifndef SQLITE_OMIT_VIRTUALTABLE, so the virtual-table related
//   operations are omitted when SQLITE_OMIT_VIRTUALTABLE is defined at compile time.
//
// Step 2 (unit test generation): 
// - Tests focus on the positive path where the virtual table is created and sqlite3_intarray_create returns SQLITE_OK.
// - Validate that ppReturn is non-null after a successful call, and that the function returns SQLITE_OK. 
// - Use an in-memory SQLite database (":memory:") to exercise the actual path including module creation.
// - As we cannot modify the focal code, we rely on the real environment providing intarrayModule; 
//   the test will fail to link if the test suite is not compiled together with the corresponding module.
// - Additional test: create two intarrays with distinct names to ensure independent module registrations.
// - We do not attempt to force the SQLITE_OMIT_VIRTUALTABLE path at runtime; that path requires recompilation with a macro defined.
//
// Step 3 (test refinement): 
// - Implement a minimal test harness with non-terminating assertions to maximize coverage.
// - Provide explanatory comments for each test function.
//
// DOMAIN_KNOWLEDGE constraints observed:
// - Use only allowed libraries (C++ standard library + provided methods and SQLite in this scenario).
// - Access static helpers and not private members directly (we rely on the API surface).
// - Do not rely on gtest; implement a small, portable test harness and a main() entry point.
// - Ensure tests are executable and self-contained within a single translation unit for simplicity.

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3.h>
#include <test_intarray.h>
#include <string>
#include <iostream>
#include <cstdlib>


extern "C" {
}

// Prototyping the focal function from the project under test.
// We assume the environment links against the implementation that defines this symbol.
extern "C" int sqlite3_intarray_create(sqlite3 *db, const char *zName, sqlite3_intarray **ppReturn);

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

// Non-terminating expectation macros (simulate EXPECT_* style without requiring GTest)
#define EXPECT_TRUE(cond) do { \
  ++g_tests_run; \
  if(!(cond)) { \
    std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ \
              << " -> " #cond "\n"; \
    ++g_tests_failed; \
  } \
} while(0)

#define EXPECT_FALSE(cond) do { \
  ++g_tests_run; \
  if(cond) { \
    std::cerr << "EXPECT_FALSE failed at " << __FILE__ << ":" << __LINE__ \
              << " -> " #cond "\n"; \
    ++g_tests_failed; \
  } \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { \
  ++g_tests_run; \
  if((ptr) == nullptr) { \
    std::cerr << "EXPECT_NOT_NULL failed at " << __FILE__ << ":" << __LINE__ \
              << " -> null pointer\n"; \
    ++g_tests_failed; \
  } \
} while(0)

#define EXPECT_EQ(a, b) do { \
  ++g_tests_run; \
  if((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
              << " -> " #a " != " #b " (" << (a) << " != " << (b) << ")\n"; \
    ++g_tests_failed; \
  } \
} while(0)

#define EXPECT_OK(rc) EXPECT_EQ((rc), SQLITE_OK)

// Test 1: Basic positive path for sqlite3_intarray_create
// - Open an in-memory SQLite database.
// - Call sqlite3_intarray_create with a valid name.
// - Verify that the return code is SQLITE_OK and the out parameter is non-null.
// - This exercises the allocation, module registration attempt, and the CREATE VIRTUAL TABLE path
//   (provided SQLITE_OMIT_VIRTUALTABLE is not defined at compile time in the build of the library under test).
static void test_sqlite3_intarray_create_basic_positive() {
  // Description: Validate that sqlite3_intarray_create succeeds and returns a non-null pointer.
  // This confirms the primary responsibilities:
  // - memory allocation for sqlite3_intarray
  // - registration of the intarrayModule with sqlite3
  // - execution of the CREATE VIRTUAL TABLE statement (temp.<Name>)
  std::cout << "Running test: test_sqlite3_intarray_create_basic_positive" << std::endl;

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_OK(rc);
  EXPECT_NOT_NULL(db);
  if(db == nullptr) {
    std::cerr << "Aborting test due to null db handle." << std::endl;
    return;
  }

  sqlite3_intarray *pArray = nullptr;
  rc = sqlite3_intarray_create(db, "test_intarray_basic", &pArray);
  EXPECT_OK(rc);
  EXPECT_NOT_NULL(pArray);

  // Cleanup: Close database connection (sqlite3_intarray would be freed by library on close)
  rc = sqlite3_close(db);
  EXPECT_OK(rc);
  (void)rc; // suppress unused warning in release builds if any
}

// Test 2: Create two distinct intarrays to exercise multiple registrations.
// - Ensures that calling the focal method with a different name results in a separate module
//   and a separate sqlite3_intarray instance being allocated.
// - We primarily verify that the second call also succeeds and returns a non-null pointer.
// Note: We do not aggressively attempt to destroy/cleanup memory allocated by the test harness
// beyond closing the database, as actual cleanup of sqlite3_intarray instances is managed by
// the SQLite/vtab subsystem when the database is closed.
static void test_sqlite3_intarray_create_two_distinct() {
  std::cout << "Running test: test_sqlite3_intarray_create_two_distinct" << std::endl;

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_OK(rc);
  EXPECT_NOT_NULL(db);
  if(db == nullptr) {
    std::cerr << "Aborting test due to null db handle." << std::endl;
    return;
  }

  sqlite3_intarray *pArray1 = nullptr;
  rc = sqlite3_intarray_create(db, "test_intarray_one", &pArray1);
  EXPECT_OK(rc);
  EXPECT_NOT_NULL(pArray1);

  sqlite3_intarray *pArray2 = nullptr;
  rc = sqlite3_intarray_create(db, "test_intarray_two", &pArray2);
  EXPECT_OK(rc);
  EXPECT_NOT_NULL(pArray2);

  // Optional: verify distinct pointers (i.e., distinct allocations)
  EXPECT_FALSE(pArray1 == pArray2);

  rc = sqlite3_close(db);
  EXPECT_OK(rc);
  (void)rc;
}

// Test 3 (optional): Ensure behavior when creating with a very long name.
// This exercises the zName path and helps validate that the quoting/formatting in
// the CREATE VIRTUAL TABLE statement does not crash with long identifiers.
// We mainly assert that the call does not fail catastrophically and returns SQLITE_OK
// for a reasonably long valid name.
static void test_sqlite3_intarray_create_long_name() {
  std::cout << "Running test: test_sqlite3_intarray_create_long_name" << std::endl;

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  EXPECT_OK(rc);
  EXPECT_NOT_NULL(db);
  if(db == nullptr) {
    std::cerr << "Aborting test due to null db handle." << std::endl;
    return;
  }

  // Construct a relatively long but valid C-string name
  std::string longName = "intarray_" + std::string(256, 'x'); // 256 chars
  sqlite3_intarray *pArray = nullptr;
  rc = sqlite3_intarray_create(db, longName.c_str(), &pArray);
  EXPECT_OK(rc);
  EXPECT_NOT_NULL(pArray);

  rc = sqlite3_close(db);
  EXPECT_OK(rc);
  (void)rc;
}

// Entry point: Run all tests and report summary
int main() {
  std::cout << "Starting sqlite3_intarray_create test suite (C++11 non-GTest)" << std::endl;

  test_sqlite3_intarray_create_basic_positive();
  test_sqlite3_intarray_create_two_distinct();
  test_sqlite3_intarray_create_long_name();

  std::cout << "Test run complete. Total: " << g_tests_run
            << ", Failures: " << g_tests_failed << std::endl;

  // Return non-zero if any test failed
  if(g_tests_failed > 0) {
    return 1;
  }
  return 0;
}