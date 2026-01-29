/*
  C++11 test suite for the focal method:
  int sqlite3_blobio_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)

  Assumptions:
  - The environment provides SQLite3 headers and library (libsqlite3).
  - The focal function is provided via blobio.c (linked into the test build).
  - We test behavior by manipulating the pre-registration state of sqlite3_user_functions
    (i.e., sqlite3_create_function) to exercise the two-branch logic:
      - When the first function registration succeeds, the second is attempted.
      - When the first function registration fails (function name already exists), the second
        is not attempted.
      - When the second registration would fail (second name already exists) after the first
        succeeds, the overall rc should reflect failure.

  Test approach (non-GTest, non-GMock, plain C++11 with manual harness):
  - Use in-memory SQLite database (":memory:").
  - Provide dummy xFunc callbacks for readblob and writeblob to register functions.
  - Validate return codes (SQLITE_OK vs non-OK) to cover true/false branches of each condition.
  - Use non-terminating, explicit EXPECT_* style macros to accumulate failures but continue tests.

  Build note:
  - Compile with something like:
      g++ -std=c++11 -O2 -I<path_to_sqlite3_headers> test_blobio.cpp blobio.c -lsqlite3
  - Ensure blobio.c uses the system sqlite3 API (the test does not mock internal macros beyond NULL pApi).
*/

#include <string.h>
#include <assert.h>
#include <sqlite3ext.h>
#include <sqlite3.h>
#include <string>
#include <iostream>



// Forward declaration of the focal function (C linkage)
extern "C" int sqlite3_blobio_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

// Dummy function implementations to be registered in tests
extern "C" void blobio_dummy_read(sqlite3_context*, int, sqlite3_value**);
extern "C" void blobio_dummy_write(sqlite3_context*, int, sqlite3_value**);

// Simple non-terminating test assertion macros (EXPECT_* style)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual) do { \
    ++g_total_tests; \
    if( (expected) != (actual) ) { \
        std::cerr << "EXPECT_EQ failed: expected " << (expected) << " but got " << (actual) \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failed_tests; \
    } \
} while(0)

// Test 1: Baseline success path - both registrations succeed when pApi is NULL
static void test_blobio_init_baseline_success()
{
    sqlite3 *db = nullptr;
    char *errmsg = nullptr;
    int rc = SQLITE_OK;

    int openRes = sqlite3_open(":memory:", &db);
    EXPECT_EQ(SQLITE_OK, openRes);
    if (db == nullptr || openRes != SQLITE_OK) {
        std::cerr << "Failed to open in-memory database for baseline test.\n";
        return;
    }

    // Call the focal function with NULL pApi
    rc = sqlite3_blobio_init(db, &errmsg, nullptr);
    EXPECT_TRUE(errmsg == nullptr);  // errmsg should be unused by the implementation here
    EXPECT_EQ(SQLITE_OK, rc);

    sqlite3_close(db);
}

// Test 2: First function registration should fail (readblob) due to pre-existing function
static void test_blobio_init_first_registration_failure()
{
    sqlite3 *db = nullptr;
    int rc = SQLITE_OK;

    int openRes = sqlite3_open(":memory:", &db);
    EXPECT_EQ(SQLITE_OK, openRes);
    if (db == nullptr || openRes != SQLITE_OK) {
        std::cerr << "Failed to open in-memory database for first-failure test.\n";
        return;
    }

    // Pre-register "readblob" so the first call inside sqlite3_blobio_init will fail
    // We provide a dummy xFunc with the required signature.
    sqlite3_value *dummyValue = nullptr;
    // The actual dummy function pointer must match the signature sqlite3_create_function expects
    // We'll declare a dummy function pointer to blobio_dummy_read
    rc = sqlite3_create_function(db, "readblob", 6, SQLITE_UTF8, 0,
                                 blobio_dummy_read, 0, 0);
    // Expect success in pre-registration of the first function
    EXPECT_EQ(SQLITE_OK, rc);

    // Now call the focal function; it should fail on the first registration attempt
    char *err = nullptr;
    rc = sqlite3_blobio_init(db, &err, nullptr);
    // The rc should be non-OK since "readblob" already exists
    EXPECT_TRUE(rc != SQLITE_OK);

    sqlite3_close(db);
}

// Test 3: Second function registration should fail (writeblob) after first succeeds
static void test_blobio_init_second_registration_failure()
{
    sqlite3 *db = nullptr;
    int rc = SQLITE_OK;

    int openRes = sqlite3_open(":memory:", &db);
    EXPECT_EQ(SQLITE_OK, openRes);
    if (db == nullptr || openRes != SQLITE_OK) {
        std::cerr << "Failed to open in-memory database for second-failure test.\n";
        return;
    }

    // Do NOT pre-register "readblob" so the first creation succeeds.
    // Pre-register "writeblob" so that when sqlite3_blobio_init attempts to register it,
    // the second registration fails.
    rc = sqlite3_create_function(db, "writeblob", 6, SQLITE_UTF8, 0,
                                 blobio_dummy_write, 0, 0);
    EXPECT_EQ(SQLITE_OK, rc);

    // Now call the focal function; since "readblob" is not present, first will succeed,
    // but second ("writeblob") should fail due to existing function.
    char *err = nullptr;
    rc = sqlite3_blobio_init(db, &err, nullptr);
    EXPECT_TRUE(rc != SQLITE_OK);

    sqlite3_close(db);
}

// Dummy function implementations (must be extern "C" with the exact signature)
extern "C" void blobio_dummy_read(sqlite3_context*, int, sqlite3_value**)
{
    // No-op for test purposes
}
extern "C" void blobio_dummy_write(sqlite3_context*, int, sqlite3_value**)
{
    // No-op for test purposes
}

// Driver to run tests from main (no GTest, direct calls)
int main() {
    std::cout << "Starting blobio_init test suite (C++11, non-GTest)..." << std::endl;

    test_blobio_init_baseline_success();
    test_blobio_init_first_registration_failure();
    test_blobio_init_second_registration_failure();

    std::cout << "Tests run: " << g_total_tests << ", Failures: " << g_failed_tests << std::endl;

    if (g_failed_tests != 0) {
        std::cerr << "Some tests failed. Please review the log above." << std::endl;
        return 1;
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}