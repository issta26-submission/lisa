/*
  Unit test suite for the focal method sqlite3IcuInit located in icu.c

  Approach overview:
  - We assume a build environment where icu.c is compiled in the same project.
  - We provide a lightweight test harness (no GoogleTest) using only the C++11 standard library.
  - We mock the SQLite function registry by supplying a test double for sqlite3_create_function.
  - We exercise:
      1) Normal path: all ICU-related functions are registered (expected rc = SQLITE_OK, 12 registrations).
      2) Early failure on first registration: rc indicates error and only first call occurred.
      3) Early failure mid-sequence: rc indicates error and the function registers up to failure.
      4) Correct iContext handling: the first function receives a non-null context (db), others receive null.
  - Notes:
    - This test file focuses on functional behavior of sqlite3IcuInit and its interaction with sqlite3_create_function.
    - We rely on a minimal, self-contained mock of sqlite3 types/constants to keep the test self-contained.
    - Explanatory comments accompany each test case.

  How to run (high level):
  - Build icu.c together with this test file (or adapt to your project’s test runner).
  - Ensure the test harness uses the same translation unit layout as your build (or adapt as needed for linking).
  - Run the resulting executable; the test harness reports failures to stdout/stderr.

  Important: This test code is designed to complement your existing test infrastructure. It uses a minimal set of facilities
  and does not depend on Google Test or other external frameworks.

  Author: Generated for test coverage demonstration
*/

#include <unicode/ustring.h>
#include <unicode/uregex.h>
#include <vector>
#include <assert.h>
#include <cstdint>
#include <string>
#include <cstring>
#include <iostream>
#include <unicode/ucol.h>
#include <cassert>


// Forward declarations to allow compiling even if actual SQLite headers are not included.
// This is a lightweight, minimal surface suitable for a unit-test harness.
struct sqlite3;
struct sqlite3_context;
struct sqlite3_value;

typedef struct sqlite3 sqlite3;
typedef struct sqlite3_context sqlite3_context;
typedef struct sqlite3_value sqlite3_value;

// SQLite-like result codes (minimal subset for tests)
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Minimal enc/flags placeholders to mirror signature expectations
#define SQLITE_UTF8 1
#define SQLITE_UTF16 2
#define SQLITE_DETERMINISTIC 0
#define SQLITE_INNOCUOUS 0
#define SQLITE_DIRECTONLY 0
#define SQLITE_ANY 0

// Global test control knobs (in a real test, you might use a more sophisticated mechanism)
static int g_fail_at_index = -1; // -1 means never fail; otherwise fail on this registration index

// Structure to capture each registration attempt
struct RegCall {
    const char* name;
    int nArg;
    unsigned int enc;
    void* context; // pContext passed to sqlite3_create_function
    void (*xFunc)(sqlite3_context*, int, sqlite3_value**);
};

// Log of all sqlite3_create_function invocations performed by sqlite3IcuInit
static std::vector<RegCall> g_callLog;

// Mock implementation of sqlite3_create_function to intercept registrations performed by sqlite3IcuInit.
// This mock records each call and optionally simulates a failure to test early exit behavior.
extern "C" int sqlite3_create_function(
    sqlite3* db,
    const char* zName,
    int nArg,
    unsigned int enc,
    void* pApp,
    void (*xFunc)(sqlite3_context*, int, sqlite3_value**),
    void* pV2,
    void* pV3
) {
    RegCall rec;
    rec.name = zName;
    rec.nArg = nArg;
    rec.enc = enc;
    rec.context = pApp;
    rec.xFunc = xFunc;
    g_callLog.push_back(rec);

    // Simulate a failure at a specific call index if requested by the test harness
    int idx = (int)g_callLog.size() - 1;
    if (g_fail_at_index >= 0 && idx == g_fail_at_index) {
        return SQLITE_ERROR; // simulate error
    }
    return SQLITE_OK;
}

// Dummy, external-visibility function stubs for ICU-related callbacks.
// The real icu.c provides actual implementations, but for unit-testing the registry behavior
// we only need their signatures to exist at link-time.
extern "C" void icuLoadCollation(sqlite3_context*, int, sqlite3_value**) {}
extern "C" void icuRegexpFunc(sqlite3_context*, int, sqlite3_value**) {}
extern "C" void icuCaseFunc16(sqlite3_context*, int, sqlite3_value**) {}
extern "C" void icuLikeFunc(sqlite3_context*, int, sqlite3_value**) {}

// Forward declaration of the function under test (as it would exist in the production file icu.c).
extern "C" int sqlite3IcuInit(sqlite3 *db);

// Lightweight assertion helpers (non-terminating)
static int g_anyFailures = 0;

#define TEST_EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST FAILURE] " << (msg) << std::endl; \
        ++g_anyFailures; \
    } \
} while(0)

#define TEST_EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "[TEST FAILURE] " << (msg) << " Expected: " << (b) << " Actual: " << (a) << std::endl; \
        ++g_anyFailures; \
    } \
} while(0)

// Utility to reset the log between tests
static void resetLog() {
    g_callLog.clear();
    g_fail_at_index = -1;
}

// Test 1: Normal path - all registrations succeed
static void test_all_functions_registered() {
    resetLog();
    g_fail_at_index = -1; // no artificial failure

    sqlite3 db;
    int rc = sqlite3IcuInit(&db); // call into focal method
    TEST_EXPECT_EQ(rc, SQLITE_OK, "sqlite3IcuInit should return SQLITE_OK when all registrations succeed");

    // Expect exactly 12 registrations as per the provided scalars array
    TEST_EXPECT_EQ((int)g_callLog.size(), 12, "Expected 12 sqlite3_create_function calls in healthy path");

    // The first entry should be icu_load_collation with iContext enabled (context should be non-null)
    TEST_EXPECT_TRUE(std::strcmp(g_callLog[0].name, "icu_load_collation") == 0, "First registration should be icu_load_collation");
    TEST_EXPECT_TRUE(g_callLog[0].context == &db, "icu_load_collation should receive non-null context (db) for iContext");

    // All subsequent entries should have iContext == 0 (context == nullptr)
    for (size_t i = 1; i < g_callLog.size(); ++i) {
        TEST_EXPECT_TRUE(g_callLog[i].context == nullptr, "Subsequent registrations should have NULL context");
    }
}

// Test 2: Early failure on first registration
static void test_failure_on_first_registration() {
    resetLog();
    g_fail_at_index = 0; // fail on first call

    sqlite3 db;
    int rc = sqlite3IcuInit(&db);
    TEST_EXPECT_EQ(rc, SQLITE_ERROR, "sqlite3IcuInit should propagate failure from first registration");
    TEST_EXPECT_EQ((int)g_callLog.size(), 1, "Only the first registration should have been attempted on failure");
}

// Test 3: Early failure mid-sequence
static void test_failure_mid_sequence() {
    resetLog();
    g_fail_at_index = 5; // fail on the sixth registration (0-based index)

    sqlite3 db;
    int rc = sqlite3IcuInit(&db);
    TEST_EXPECT_EQ(rc, SQLITE_ERROR, "sqlite3IcuInit should propagate mid-sequence failure");
    TEST_EXPECT_TRUE((int)g_callLog.size() >= 1, "Some registrations should have occurred before failure");
    TEST_EXPECT_EQ((int)g_callLog.size(), 6, "Six registrations should have been attempted before the failure");
}

// Test 4: Context propagation correctness for the first function and null context for others
static void test_context_propagation() {
    resetLog();
    g_fail_at_index = -1; // ensure all succeed

    sqlite3 db;
    int rc = sqlite3IcuInit(&db);
    TEST_EXPECT_EQ(rc, SQLITE_OK, "sqlite3IcuInit should succeed for context propagation test");

    // First entry should have non-null context pointing to db
    TEST_EXPECT_TRUE(g_callLog.size() >= 1, "At least one registration should have occurred");
    TEST_EXPECT_TRUE(g_callLog[0].context == &db, "First registration (icu_load_collation) should receive db as context");

    // All others should have null context
    for (size_t i = 1; i < g_callLog.size(); ++i) {
        TEST_EXPECT_TRUE(g_callLog[i].context == nullptr, "Other registrations should have NULL context");
    }
}

// Entry point
int main() {
    std::cout << "Starting sqlite3IcuInit unit tests (non-GTest, simple harness)" << std::endl;

    test_all_functions_registered();
    test_failure_on_first_registration();
    test_failure_mid_sequence();
    test_context_propagation();

    if (g_anyFailures == 0) {
        std::cout << "[ALL TESTS PASSED] sqlite3IcuInit behaved as expected." << std::endl;
        return 0;
    } else {
        std::cerr << "[SOME TESTS FAILED] Number of failures: " << g_anyFailures << std::endl;
        return 2;
    }
}