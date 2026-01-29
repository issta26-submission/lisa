// Unit test suite for sqlite3_vtshim_init
// Target: C++11, no Google Test. Uses a lightweight non-terminating assertion framework.
// Purpose: Validate that sqlite3_vtshim_init returns SQLITE_OK under typical inputs.
// Note: This test relies on real SQLite headers being available (sqlite3ext.h / sqlite3_api_routines).

#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <cstring>
#include <iostream>


// Bring in SQLite types and macros.
// Ensure C linkage for C headers when compiling as C++.
extern "C" {
}

// Optional: declare the function under test explicitly with C linkage.
// If the header provides a prototype, this is redundant; it's safe as an explicit declaration.
extern "C" int sqlite3_vtshim_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// Lightweight, non-terminating test assertion helpers
static int gTotalTests = 0;
static int gFailedTests = 0;

// Non-terminating equality assertion.
// In case of mismatch, log the failure but continue executing remaining tests.
#define EXPECT_EQ(actual, expected, description)                               \
  do {                                                                          \
    ++gTotalTests;                                                               \
    if ((actual) != (expected)) {                                              \
      ++gFailedTests;                                                            \
      std::cerr << "[ASSERTION FAILED] " << (description)                       \
                << " | Expected: " << (expected)                                 \
                << ", Actual: " << (actual) << std::endl;                    \
    }                                                                          \
  } while (0)

// Additional convenience macro for boolean checks (true/false)
#define EXPECT_TRUE(value, description) EXPECT_EQ((value), true, (description))
#define EXPECT_FALSE(value, description) EXPECT_EQ((value), false, (description))

// Test 1: Basic invocation with null db and a zeroed pApi.
// Expect SQLITE_OK. This covers the most common, safe usage path.
void test_vtshim_init_basic_null_db()
{
  // Domain knowledge: The function should be robust to a null db pointer
  // in this context as it primarily initializes the extension with the API pointer.
  sqlite3* db = nullptr;
  char* zErr = nullptr;

  // Create a zero-initialized dummy API struct. The macro SQLITE_EXTENSION_INIT2
  // should handle a valid pointer to the API struct even if its fields are zeroed.
  sqlite3_api_routines api;
  std::memset(&api, 0, sizeof(api));

  int rc = sqlite3_vtshim_init(db, &zErr, &api);

  EXPECT_EQ(rc, SQLITE_OK, "vtshim_init should return SQLITE_OK with null db and zeroed pApi");
}

// Test 2: Basic invocation with a non-null-ish db pointer and a zeroed pApi.
// This exercises a slightly more realistic scenario where the db argument is non-null
// but still does not rely on actual SQLite internals during initialization.
void test_vtshim_init_nonnull_db()
{
  // Non-null pointer value; the function should not dereference this value in a way that causes
  // a crash during initialization (we only rely on pApi for initialization).
  sqlite3* db = reinterpret_cast<sqlite3*>(0x1234);
  char* zErr = nullptr;

  sqlite3_api_routines api;
  std::memset(&api, 0, sizeof(api));

  int rc = sqlite3_vtshim_init(db, &zErr, &api);

  EXPECT_EQ(rc, SQLITE_OK, "vtshim_init should return SQLITE_OK with a non-null db pointer and zeroed pApi");
}

// Test 3: Basic invocation with a non-nullish pApi (still zeroed fields) and non-null pzErr.
// Ensures that error string handling parameters do not affect the success path.
void test_vtshim_init_with_error_ptr()
{
  sqlite3* db = nullptr;
  char* zErr = nullptr;

  sqlite3_api_routines api;
  std::memset(&api, 0, sizeof(api));

  int rc = sqlite3_vtshim_init(db, &zErr, &api);

  EXPECT_EQ(rc, SQLITE_OK, "vtshim_init should return SQLITE_OK even when pzErr is provided and pApi is zeroed");
}

// Runner to execute all tests and report summary
int main()
{
  test_vtshim_init_basic_null_db();
  test_vtshim_init_nonnull_db();
  test_vtshim_init_with_error_ptr();

  std::cout << "Total tests run: " << gTotalTests
            << ", Failures: " << gFailedTests << std::endl;

  // Return non-zero if there were failures to aid integration with simple build systems
  return gFailedTests;
}