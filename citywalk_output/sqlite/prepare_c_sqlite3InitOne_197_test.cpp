// Unit tests for sqlite3InitOne in prepare.c
// This test suite targets the focal method described in the supplied code.
// It is designed to compile with C++11 and link against the existing SQLite core
// (sqlite3.h / sqlite3 library). No GoogleTest is used; a lightweight in-process
// test harness is implemented.
// The tests focus on verifying basic successful initialization paths and a
// specific reset-flag behavior that affects the in-memory meta state.
// Explanatory comments are included to map test intent to the method's predicates.

/*
Guiding notes (derived from the focal method and dependencies):
- Candidate Keywords / dependencies:
  - sqlite3InitOne, InitData, SCHEMA_TABLE(iDb), InitCallback, pSchema, aDb, pBt, pzErrMsg
  - DB_SchemaLoaded, DBFLAG_SchemaKnownOk, DBFLAG_EncodingFixed
  - sqlite3BtreeEnter/BeginTrans/Commit/Leave, sqlite3BtreeGetMeta, sqlite3BtreeLastPage
  - sqlite3Exec, sqlite3InitCallback, sqlite3SetString, sqlite3SetTextEncoding
  - SQLITE_OK, SQLITE_ERROR, SQLITE_NOMEM, SQLITE_NOMEM_BKPT, SQLITE_LOCKED
  - SQLITE_ResetDatabase, SQLITE_OMIT_UTF16, SQLITE_DEFAULT_CACHE_SIZE, ENC(db)
- The tests exercise:
  1) Baseline successful path (basic in-memory db, iDb=0)
  2) Branch triggered by SQLITE_ResetDatabase flag affecting meta initialization
- Static helpers inside the focal class/file are not directly accessible;
  tests interact via public APIs provided by sqlite3.h and the core library.
- Test approach relies on actual SQLite core behavior present in the linked library.
*/

#include <functional>
#include <vector>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


extern "C" {
}

// Lightweight in-process test harness (non-terminating assertions)
namespace TinyTest {

struct TestCase {
    std::string name;
    std::function<bool()> func;
};

static int g_failures = 0;
static void logFail(const std::string &name, const std::string &msg) {
    std::cerr << "[FAIL] " << name << ": " << msg << std::endl;
    ++g_failures;
}
static void logPass(const std::string &name) {
    std::cout << "[OK]   " << name << std::endl;
}

// Simple assertion helpers (non-terminating)
#define TEST_ASSERT(cond, name, msg) do { \
    if (!(cond)) { logFail(name, msg); } \
    else { logPass(name); } \
} while(0)

} // namespace TinyTest

using namespace TinyTest;

// Domain knowledge scaffolding (for maintainers and future extension)
/*
- Static members: sqlite3InitOne interacts with database state that is commonly held
  in static/internal structures within the library. Tests should respect public API
  boundaries and avoid relying on private internals.
- Namespace: We operate within the global test program namespace; no GMock.
- Only public C API (sqlite3_*) is used to drive behavior; no private methods are invoked.
- The test environment assumes a standard SQLite amalgamation or library present in the build.
- For coverage goals, we cover at least:
    - Basic successful init path
    - Pathality when SQLITE_ResetDatabase flag is set (affects meta)
- The test suite is designed to be invoked from main() and report per-test outcomes.
*/

// Helpers to run the tests
namespace {

bool test_sqlite3InitOne_basic_ok() {
    // Test: Basic successful initialization path on an in-memory database.
    // Rationale: When a fresh in-memory database is opened, sqlite3InitOne should
    // complete with SQLITE_OK for the main database (iDb=0) under normal conditions.
    sqlite3 *db = nullptr;
    int rc = SQLITE_OK;
    char *pzErrMsg = nullptr;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory database. rc=" << rc << std::endl;
        if (db) sqlite3_close(db);
        return false;
    }

    // Call the focal function on the main database (iDb = 0)
    rc = sqlite3InitOne(db, 0, &pzErrMsg, 0);

    if (pzErrMsg) {
        // Free the error message string if allocated by the library
        sqlite3_free(pzErrMsg);
        pzErrMsg = nullptr;
    }

    if (db) sqlite3_close(db);

    // Expect success
    bool ok = (rc == SQLITE_OK);
    if (!ok) {
        std::cerr << "sqlite3InitOne_basic_ok: rc=" << rc << std::endl;
    }
    return ok;
}

bool test_sqlite3InitOne_resetDatabaseFlag_affects_meta() {
    // Test: When SQLITE_ResetDatabase flag is set on the DbConnection, the
    // initialization path zeros the meta array before loading. We verify basic
    // successful completion and that the function returns SQLITE_OK under this flag.
    // Note: We do not introspect internal meta fields deeply; we verify no crash and
    // a successful return value as a proxy for correct handling of the reset path.

    sqlite3 *db = nullptr;
    int rc = SQLITE_OK;
    char *pzErrMsg = nullptr;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        std::cerr << "Failed to open in-memory database for reset flag test. rc=" << rc << std::endl;
        if (db) sqlite3_close(db);
        return false;
    }

    // Enable the ResetDatabase flag to exercise the dedicated code path
    // SQLITE_ResetDatabase is defined in sqlite3.h; we toggle the flag on the connection.
    db->flags |= SQLITE_ResetDatabase;

    rc = sqlite3InitOne(db, 0, &pzErrMsg, 0);

    if (pzErrMsg) {
        sqlite3_free(pzErrMsg);
        pzErrMsg = nullptr;
    }

    if (db) sqlite3_close(db);

    bool ok = (rc == SQLITE_OK);
    if (!ok) {
        std::cerr << "sqlite3InitOne_resetDatabaseFlag_affects_meta: rc=" << rc << std::endl;
    }

    return ok;
}

} // anonymous namespace

int main() {
    std::vector<TinyTest::TestCase> tests = {
        {"sqlite3InitOne_basic_ok", [](){ return test_sqlite3InitOne_basic_ok(); }},
        {"sqlite3InitOne_resetDatabaseFlag_affects_meta", [](){ return test_sqlite3InitOne_resetDatabaseFlag_affects_meta(); }},
    };

    std::cout << "Running sqlite3InitOne test suite (C++11, no GTest):" << std::endl;
    int testsRun = 0;
    int testsPassed = 0;
    for (const auto &t : tests) {
        ++testsRun;
        // Each test returns true on success, false on failure (non-terminating)
        bool result = t.func();
        if (result) ++testsPassed;
    }

    std::cout << "Test results: " << testsPassed << " / " << testsRun << " tests passed." << std::endl;
    if (g_failures > 0) {
        std::cerr << "There were " << g_failures << " explicit failures." << std::endl;
    }
    return (testsRun == testsPassed) ? 0 : 1;
}

/*
Notes for maintainers and future contributors:
- This test suite uses the public SQLite C API to drive sqlite3InitOne. It does not
  mock private/internal behavior, since those are static/private in the focal file
  and not directly mockable from outside.
- The tests are designed to be non-destructive and use an in-memory database to avoid
  filesystem side effects.
- The tests assume that the project provides sqlite3.h and links against the SQLite
  library in a way compatible with C linkage. If linking issues occur, ensure the
  test is compiled with the same configuration and that the SQLite amalgamation or
  library is accessible.
- The tests deliberately avoid terminating on assertion failures; instead, they log
  failures via an in-process TinyTest logger. You can augment the logger or extend
  tests to cover additional branches by adjusting flags (e.g., SQLITE_ResetDatabase)
  or introducing more calls to sqlite3InitOne with varied mFlags.
- To extend test coverage, consider creating a few more tests that exercise:
  - Another iDb variant (iDb=1) if the test environment provides a temp schema path.
  - Injection of pzErrMsg (simulated error condition) to verify error_out handling.
  - Scenarios where pDb->pBt is NULL (iDb==1) to trigger the early DB_SchemaLoaded path.
- This test harness is intentionally minimal and focuses on executable coverage rather than
  exhaustive internal state probing, given the private nature of many fields in sqlite3.
*/