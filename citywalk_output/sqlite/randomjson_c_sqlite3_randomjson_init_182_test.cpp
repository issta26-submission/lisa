/*
Unit Test Suite for sqlite3_randomjson_init (focal method)

Step 1 - Program Understanding and Candidate Keywords
- Core components and dependencies extracted from the focal method:
  - sqlite3: the database handle type used by the function
  - sqlite3_api_routines: API table pointer passed to the extension init
  - sqlite3_ext.h / SQLITE_EXTENSION_INIT2: macro usage to initialize extension API
  - sqlite3_create_function: function registration API used inside sqlite3_randomjson_init
  - randJsonFunc: callback implementation for the registered functions
  - pzErrMsg: error message output parameter (unused in function)
  - Static local variables: cOne, cZero (internal to the function)
  - Function names introduced by the init: "random_json" and "random_json5"
- The function flow is:
  1) Initialize extension API via SQLITE_EXTENSION_INIT2(pApi)
  2) Attempt to register "random_json" with 1 argument
  3) If successful, register "random_json5" with 1 argument
  4) Return the resulting rc (SQLITE_OK on success, else error code)

- Candidate Keywords to guide test design:
  - sqlite3, sqlite3_open, sqlite3_close
  - sqlite3_randomjson_init, sqlite3_create_function
  - random_json, random_json5
  - randJsonFunc (as a black-box callback)
  - SQLITE_OK, SQLITE_MISUSE
  - pApi, SQLITE_EXTENSION_INIT2
  - pzErrMsg (unused)
  - static int cOne, cZero (stateful within function)

Step 2 - Unit Test Generation (focus on sqlite3_randomjson_init)
The test suite below is implemented in C++11 (no Google Test). It uses the real SQLite C API (via sqlite3.h) to exercise the focal method under two primary scenarios:
- True path: Providing a valid sqlite3* db results in SQLITE_OK and registers the two functions.
- False path: Providing a null db results in a non-SQLITE_OK return code (e.g., SQLITE_MISUSE), hitting the else branch and not attempting to register functions.

Step 3 - Test Case Refinement
- Tests cover true/false branches of the internal condition (rc == SQLITE_OK).
- Static/memory semantics are indirectly covered by using a fresh in-memory database (new static state per test run).
- We avoid private/internal methods and rely only on public API for test interactions.
- A minimal, custom test harness prints PASS/FAIL messages and continues execution (non-terminating assertions).
- We supply a dummy sqlite3_api_routines instance (non-null) to avoid potential null-pointer pitfalls in the extension macro, while keeping the test self-contained.

Note: The tests are designed to be compiled together with randomjson.c (the focal implementation) and linked against the SQLite library. The tests do not rely on GTest and are self-contained in C++11.

Code:
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>


// Declaration of the focal function (C linkage)
extern "C" int sqlite3_randomjson_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);

// Lightweight test harness
#define TEST_PASS(msg) std::cout << "[PASS] " << msg << std::endl;
#define TEST_FAIL(msg) std::cerr << "[FAIL] " << msg << std::endl;

// Test 1: Init with a valid in-memory database should return SQLITE_OK
bool test_init_with_valid_db() {
  sqlite3* db = nullptr;
  int rc_open = sqlite3_open(":memory:", &db);
  if (rc_open != SQLITE_OK || db == nullptr) {
    TEST_FAIL("test_init_with_valid_db: failed to open in-memory database (rc=" << rc_open << ")");
    return false;
  }

  // Prepare a non-null dummy pApi to satisfy SQLITE_EXTENSION_INIT2(pApi)
  sqlite3_api_routines dummyApi;
  std::memset(&dummyApi, 0, sizeof(dummyApi));
  const sqlite3_api_routines* pApi = &dummyApi;

  // pzErrMsg is unused in the focal method; provide a non-null pointer as a courtesy
  char* err = nullptr;

  int rc = sqlite3_randomjson_init(db, &err, pApi);

  sqlite3_close(db);

  if (rc == SQLITE_OK) {
    TEST_PASS("test_init_with_valid_db: sqlite3_randomjson_init returned SQLITE_OK as expected");
    return true;
  } else {
    TEST_FAIL("test_init_with_valid_db: sqlite3_randomjson_init returned rc=" << rc << ", expected SQLITE_OK");
    return false;
  }
}

// Test 2: Init with a null db should hit the false branch and not return SQLITE_OK
bool test_init_with_null_db() {
  // Prepare a non-null dummy pApi to satisfy SQLITE_EXTENSION_INIT2(pApi)
  sqlite3_api_routines dummyApi;
  std::memset(&dummyApi, 0, sizeof(dummyApi));
  const sqlite3_api_routines* pApi = &dummyApi;

  // pzErrMsg is unused; pass nullptr
  int rc = sqlite3_randomjson_init(nullptr, nullptr, pApi);

  if (rc != SQLITE_OK) {
    TEST_PASS("test_init_with_null_db: sqlite3_randomjson_init returned non-SQLITE_OK as expected (rc=" << rc << ")");
    return true;
  } else {
    TEST_FAIL("test_init_with_null_db: sqlite3_randomjson_init unexpectedly returned SQLITE_OK");
    return false;
  }
}

// Optional: Additional test to ensure calling with a valid db does not crash on repeated calls
// (This tests stability rather than exact behavior of sqlite3_create_function duplications.)
bool test_init_with_valid_db_twice() {
  sqlite3* db = nullptr;
  int rc_open = sqlite3_open(":memory:", &db);
  if (rc_open != SQLITE_OK || db == nullptr) {
    TEST_FAIL("test_init_with_valid_db_twice: failed to open in-memory database (rc=" << rc_open << ")");
    return false;
  }

  sqlite3_api_routines dummyApi;
  std::memset(&dummyApi, 0, sizeof(dummyApi));
  const sqlite3_api_routines* pApi = &dummyApi;

  char* err = nullptr;

  int rc1 = sqlite3_randomjson_init(db, &err, pApi);
  int rc2 = SQLITE_OK;
  if (rc1 == SQLITE_OK) {
    // Second call to init on the same db; rc may vary based on how sqlite handles existing functions
    rc2 = sqlite3_randomjson_init(db, &err, pApi);
  }

  sqlite3_close(db);

  if (rc1 == SQLITE_OK && rc2 == SQLITE_OK) {
    TEST_PASS("test_init_with_valid_db_twice: both initializations returned SQLITE_OK");
    return true;
  } else {
    TEST_FAIL("test_init_with_valid_db_twice: rc1=" << rc1 << ", rc2=" << rc2);
    return false;
  }
}

int main() {
  int total = 0;
  int passed = 0;

  std::cout << "Running sqlite3_randomjson_init unit tests (C++11, no GTest)\n";

  if (test_init_with_valid_db()) { ++passed; } ++total;
  if (test_init_with_null_db()) { ++passed; } ++total;
  if (test_init_with_valid_db_twice()) { ++passed; } ++total;

  std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";
  return (passed == total) ? 0 : 1;
}