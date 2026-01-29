// Test suite for the focal method: sqlite3_icu_init
// Language: C++11
// Note: This test harness assumes a build environment where SQLite3 (sqlite3.h) is available,
// and that ICU-related extension code has been compiled into the test target (as in the focal icu.c).
// The tests exercise the wrapper sqlite3_icu_init by validating pass-through behavior to sqlite3IcuInit
// and basic interaction with a real SQLite3 database instance.

#include <unicode/ustring.h>
#include <unicode/uregex.h>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <cstring>
#include <unicode/ucol.h>
#include <cassert>


// Bring in C declarations for the SQLite3 API and ICU extension function(s).
extern "C" {
}

// Prototypes for the focal wrapper and its internal init function.
// These are declared with C linkage to match the implementation in icu.c (the focal file).
extern "C" int sqlite3_icu_init(
  sqlite3 *db,
  char **pzErrMsg,
  const sqlite3_api_routines *pApi
);

extern "C" int sqlite3IcuInit(sqlite3 *db);

// A minimal dummy API struct to satisfy the pApi parameter for the wrapper.
// We do not rely on any real ICU/SQLite extensions features here; the wrapper's
// behavior should be robust to a zeroed/pseudo API structure in unit tests.
extern "C" typedef struct sqlite3_api_routines sqlite3_api_routines;

// Simple assertion helpers to provide non-terminating assertions for test flow.
#define ASSERT_TRUE(cond, msg) do { \
  if (!(cond)) { \
    fprintf(stderr, "ASSERT_TRUE failed: %s\n", (msg)); \
    return 1; \
  } \
} while (0)

#define ASSERT_EQ(a, b, msg) do { \
  if ((a) != (b)) { \
    fprintf(stderr, "ASSERT_EQ failed: %s | %lld != %lld\n", (msg), static_cast<long long>(a), static_cast<long long>(b)); \
    return 1; \
  } \
} while (0)

// Test 1: Basic pass-through verification
// - Create an in-memory SQLite database.
// - Compute result via inner function sqlite3IcuInit(db).
// - Compute result via wrapper sqlite3_icu_init(db, &errMsg, &dummyApi).
// - Assert both results are equal, validating that the wrapper delegates exactly to the inner init.
int test_basic_pass_through() {
  printf("Running test_basic_pass_through...\n");

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == nullptr) {
    fprintf(stderr, "Failed to open in-memory database. rc=%d\n", rc);
    return 1;
  }

  // Prepare a dummy error message holder (not used by our test logic but required by the API).
  char *errMsg = nullptr;

  // Prepare a minimal dummy API structure to pass to the wrapper.
  sqlite3_api_routines dummyApi;
  std::memset(&dummyApi, 0, sizeof(dummyApi));

  // Inner init directly on the database.
  int inner = sqlite3IcuInit(db);

  // Wrapper init via the focal method.
  int wrapper = sqlite3_icu_init(db, &errMsg, &dummyApi);

  // Compare results. The wrapper should be a direct pass-through to sqlite3IcuInit.
  ASSERT_EQ(inner, wrapper, "wrapper should return the same value as inner sqlite3IcuInit");

  // Cleanup
  sqlite3_close(db);
  // errMsg is unused in this test (and typically managed by ICU code paths),
  // so only free if non-null.
  if (errMsg) sqlite3_free(errMsg);

  printf("test_basic_pass_through PASSED\n");
  return 0;
}

// Test 2: Consistency across multiple wrapper invocations on the same database
// - Open a memory database.
// - Call the wrapper several times and assert consistency with a single inner init call.
// - This exercises that repeated wrapper invocations do not corrupt state.
int test_wrapper_consistency_multiple_calls() {
  printf("Running test_wrapper_consistency_multiple_calls...\n");

  sqlite3 *db = nullptr;
  int rc = sqlite3_open(":memory:", &db);
  if (rc != SQLITE_OK || db == nullptr) {
    fprintf(stderr, "Failed to open in-memory database. rc=%d\n", rc);
    return 1;
  }

  char *errMsg = nullptr;
  sqlite3_api_routines dummyApi;
  std::memset(&dummyApi, 0, sizeof(dummyApi));

  // Compute a baseline using the inner init once.
  int baseline = sqlite3IcuInit(db);

  // Invoke the wrapper multiple times and compare against the baseline each time.
  for (int i = 0; i < 5; ++i) {
    int wrapper = sqlite3_icu_init(db, &errMsg, &dummyApi);
    ASSERT_EQ(baseline, wrapper, "wrapper results should be consistent across multiple invocations");
  }

  // Cleanup
  sqlite3_close(db);
  if (errMsg) sqlite3_free(errMsg);

  printf("test_wrapper_consistency_multiple_calls PASSED\n");
  return 0;
}

// Test 3: Consistency across different databases (memory vs file-backed)
// - Exercises that the wrapper is independent of the specific db backend type provided to sqlite3IcuInit.
// - Both paths should yield consistent results for the same logical ICU initialization behavior.
int test_wrapper_consistency_across_dbs() {
  printf("Running test_wrapper_consistency_across_dbs...\n");

  // First database: in-memory
  sqlite3 *memDb = nullptr;
  int rc1 = sqlite3_open(":memory:", &memDb);
  if (rc1 != SQLITE_OK || memDb == nullptr) {
    fprintf(stderr, "Failed to open in-memory database. rc=%d\n", rc1);
    return 1;
  }

  // Second database: file-based (temporary file)
  sqlite3 *fileDb = nullptr;
  rc1 = sqlite3_open("test_icu_wrapper.db", &fileDb);
  // If file cannot be created, fail gracefully.
  if (rc1 != SQLITE_OK || fileDb == nullptr) {
    fprintf(stderr, "Failed to open file-based database. rc=%d\n", rc1);
    sqlite3_close(memDb);
    return 1;
  }

  char *errMsg = nullptr;
  sqlite3_api_routines dummyApi;
  std::memset(&dummyApi, 0, sizeof(dummyApi));

  int innerMem = sqlite3IcuInit(memDb);
  int innerFile = sqlite3IcuInit(fileDb);

  int wrapperMem = sqlite3_icu_init(memDb, &errMsg, &dummyApi);
  int wrapperFile = sqlite3_icu_init(fileDb, &errMsg, &dummyApi);

  // Ensure inner initializations match wrappers for respective databases.
  ASSERT_EQ(innerMem, wrapperMem, "memory DB: wrapper should match inner init");
  ASSERT_EQ(innerFile, wrapperFile, "file DB: wrapper should match inner init");

  // Cleanup
  sqlite3_close(memDb);
  sqlite3_close(fileDb);
  if (errMsg) sqlite3_free(errMsg);

  // Optional: remove temporary file if needed (best-effort, ignore if not permitted)
  // remove("test_icu_wrapper.db");

  printf("test_wrapper_consistency_across_dbs PASSED\n");
  return 0;
}

// Main test runner
int main() {
  int failures = 0;

  // Step 2/3: Execute test suite
  failures += test_basic_pass_through();
  failures += test_wrapper_consistency_multiple_calls();
  failures += test_wrapper_consistency_across_dbs();

  if (failures == 0) {
    printf("ALL TESTS PASSED\n");
  } else {
    printf("SOME TESTS FAILED: %d failure(s)\n", failures);
  }

  return failures;
}